#include "pistache/include/endpoint.h"
#include "pistache/include/router.h"
#include "rapidjson/include/rapidjson/document.h"

using namespace std;
using namespace Net;
using namespace rapidjson;

// the http handler, linked to the endpoint
class NotesHandler : public Http::Handler {
  public:

    HTTP_PROTOTYPE(NotesHandler)
    void onRequest(const Http::Request& request, Http::ResponseWriter response);

  private:
    void RequestAllNotes(const Http::Request& request, 
                               Http::ResponseWriter& response);
    void CreateNote(const Http::Request& request, 
                          Http::ResponseWriter& response);
    void RequestNote(const Http::Request& request, 
                           Http::ResponseWriter& response);
    int AddNote(const string body);
    int GetNote(const int id, string &body);
    int numNotes = 0;
    map<int, string> notebook;
};

