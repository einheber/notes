#include "handler.h"
#include "pistache/include/endpoint.h"
#include "pistache/include/router.h"

using namespace std;
using namespace Net;


int main() {
    // set endpoint options
    auto opts = Http::Endpoint::options().threads(1).flags(Tcp::Options::InstallSignalHandler);

    // create a listener tied to the http handler
    Http::listenAndServe<NotesHandler>("localhost:80", opts);
}
