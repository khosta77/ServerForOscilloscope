//#include "../HT6022_lib_cpp/project/hantek6022.h"
#include "../oscPlug/oscilloscope_plug.h"
#include "Server.h"
#include "utils.h"

int main( int argc, char* argv[] )
{
    //oscilloscopes::hantek::Hantek6022 oscilloscope;
    oscilloscopes::plug::OscPlug oscilloscope;
    auto arg = readArg( argc, argv );
    if( std::get<0>(arg) == "" )
        return -1;
    server::CommandDecoder cmdec( std::get<0>(arg), &oscilloscope );
    server::Server server( std::get<1>(arg), std::get<2>(arg), &cmdec );
    return server.run();
}


