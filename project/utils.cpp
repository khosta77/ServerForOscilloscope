#include "utils.h"

std::tuple<std::string, std::string, int> readArg( int argc, char* argv[] )
{
    if( ( ( argc <= 1 ) || ( argc >= 4 ) ) )
        return std::tuple<std::string, std::string, int>( "", "", 0 );
    std::string prefix = std::string( argv[1] );
    std::string ip = std::string( argv[2] );
    int port = std::stoi( std::string( argv[3] ) );
    return std::tuple<std::string, std::string, int>( prefix, ip, port );
}


