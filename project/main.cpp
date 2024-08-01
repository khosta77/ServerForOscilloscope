//#include "../HT6022_lib_cpp/project/hantek6022.h"
#include "../oscPlug/oscilloscope_plug.h"
#include "Server.h"

#define SERVER_ADRESS std::string("192.168.1.65")
#define SERVER_PORT 8000

#define PREFIX std::string("osc")

int main()
{
    //oscilloscopes::hantek::Hantek6022 oscilloscope;
    oscilloscopes::plug::OscPlug oscilloscope;
    server::CommandDecoder cmdec( PREFIX, &oscilloscope );
    server::Server server( SERVER_ADRESS, SERVER_PORT, &cmdec );
    return server.run();
}


