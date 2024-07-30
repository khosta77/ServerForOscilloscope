#include "oscilloscope_plug.h"
#include "Server.h"

#define SERVER_ADRESS std::string("192.168.1.65")
#define SERVER_PORT 8000

#define PREFIX std::string("osc")

int main()
{
    oscilloscopes::plug::OscPlug op;
    server::CommandDecoder cmdec( PREFIX, &op );
    server::Server server( SERVER_ADRESS, SERVER_PORT, &cmdec );
    return server.run();
}


