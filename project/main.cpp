#include "../HT6022_lib_cpp/project/hantek6022.h"
//#include "../oscPlug/oscilloscope_plug.h"
#include "Server.h"
#include "utils.h"

int main( int argc, char* argv[] )
{
    oscilloscopes::hantek::Hantek6022 oscilloscope;
   // oscilloscopes::plug::OscPlug oscilloscope;
    server::CommandDecoder cmdec( "osc", &oscilloscope );
    server::Server server( "127.0.0.1", 8000, &cmdec );
    return server.run();
}


