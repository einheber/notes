#include <iostream>
#include <sstream>
#include "handler.h"
#include "pistache/include/endpoint.h"
#include "pistache/include/optional.h"
#include "pistache/include/router.h"
#include "rapidjson/include/rapidjson/document.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"
#include "rapidjson/include/rapidjson/writer.h"

using namespace std;
using namespace Net;
using namespace rapidjson;
using namespace Http;

// --------------------------------------------------------------------
// insert body as a new note and return its note number
int NotesHandler::AddNote(const string body)
{
  numNotes++;
  notebook.insert(pair<int,string>(numNotes, body));
  return numNotes;
}

// --------------------------------------------------------------------
// find a note by id and populate body if found
// return 1 if found or 0 if not found -- TODO: could use an optional
int NotesHandler::GetNote(const int id, string &body)
{
  auto it = notebook.find(id);
  if(it != notebook.end()) {
    body = it->second;
    return 1;
  }
  return 0;
}

// --------------------------------------------------------------------
// the pistache request handler
void NotesHandler::onRequest(const Http::Request& request, 
                                   Http::ResponseWriter response)
{
    Uri::Query uri = request.query();
    // examine the resource being requested
    if(request.resource() == "/api/notes") {
        if(request.method() == Http::Method::Get) {
            // get all existing notes
            RequestAllNotes(request, response);
        } else if(request.method() == Http::Method::Post) {
            // create a note
            CreateNote(request, response); 
        }
    } else if(request.resource().find("/api/notes/") == 0) {
        // get a specific note
        RequestNote(request, response);
    } else {
        // reject requests not related to notes
        response.send(Http::Code::Not_Found, "This is only a note server!");
    }
}

// --------------------------------------------------------------------
// create a note, store it, and respond with the new note as JSON
void NotesHandler::CreateNote(const Http::Request& request, 
                              Http::ResponseWriter& response)
{
      Document doc;

      // parse the body of the request into a JSON document
      doc.Parse(request.body().c_str());

      Value::MemberIterator body = doc.FindMember("body");
      if(body == doc.MemberEnd()) {
          // if body is missing, return 406: Not Acceptable
          response.send(Http::Code::Not_Acceptable, 
                        "Malformed request: note body not specified");
      } else {
          // insert the body as a new note
          Value id;
          id.SetInt(AddNote(body->value.GetString()));

          // modify the JSON document to include the note id
          doc.AddMember("id", id, doc.GetAllocator());

          // return the document as a response
          StringBuffer buffer;
          Writer<StringBuffer> writer(buffer);
          doc.Accept(writer);
          response.send(Http::Code::Not_Acceptable, buffer.GetString());
      }
}

// --------------------------------------------------------------------
// look up a note by id and respond with it as JSON
void NotesHandler::RequestNote(const Http::Request& request, 
                                     Http::ResponseWriter& response)
{
    string body; 

    // get the id number
    int id = stoi(request.resource().substr(strlen("/api/notes/")));
    if(!GetNote(id, body)) {
      // bad note id
      response.send(Http::Code::Not_Acceptable, "Invalid note id requested");
    } else {
      // build a JSON doc with the retrieved content
      Document doc;
      doc.Parse("{}");

      Value notebody;
      int len = body.size();
      notebody.SetString(body.c_str(), static_cast<SizeType>(len), 
                         doc.GetAllocator());
      doc.AddMember("body", notebody, doc.GetAllocator());

      Value noteid;
      noteid.SetInt(id);
      doc.AddMember("id", noteid, doc.GetAllocator());

      // stringify the JSON doc and send as the response
      StringBuffer buffer;
      Writer<StringBuffer> writer(buffer);
      doc.Accept(writer);
      response.send(Http::Code::Ok, buffer.GetString());
    }
}


// --------------------------------------------------------------------
// get every note (with optional query) and respond with them as JSON
void NotesHandler::RequestAllNotes(const Http::Request& request, 
                                         Http::ResponseWriter& response)
{
    stringstream allnotes; 
    string query = request.query().get("query").getOrElse("");

    allnotes << "[" << endl;

    for(auto it = notebook.begin(); it != notebook.end(); it++) {
        int id = it->first;
        string body = it->second;

        // verify the note matches the search query if present
        if(body.find(query) == string::npos)
          continue;

        // build a JSON doc with the retrieved content
        Document doc;
        doc.Parse("{}");

        Value notebody;
        int len = body.size();
        notebody.SetString(body.c_str(), static_cast<SizeType>(len), 
                           doc.GetAllocator());
        doc.AddMember("body", notebody, doc.GetAllocator());

        Value noteid;
        noteid.SetInt(id);
        doc.AddMember("id", noteid, doc.GetAllocator());

        // stringify the JSON doc and append it to the allnotes string
        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        doc.Accept(writer);
        allnotes << buffer.GetString() << endl;
    }

    allnotes << "]";
    response.send(Http::Code::Ok, allnotes.str());
}

