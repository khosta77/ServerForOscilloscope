#include "utils.h"

std::tuple<std::string, std::string, int> readArg( int argc, char* argv[] )
{
    if( argc != 4 )
    {
        std::cout << "default: hant, 127.0.0.1, 34100" << std::endl;
        return std::tuple<std::string, std::string, int>( "hant", "127.0.0.1", 34100 );
    }
    std::string prefix = std::string( argv[1] );
    std::string ip = std::string( argv[2] );
    int port = std::stoi( std::string( argv[3] ) );
    return std::tuple<std::string, std::string, int>( prefix, ip, port );
}


