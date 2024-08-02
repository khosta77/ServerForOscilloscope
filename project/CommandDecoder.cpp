#include "CommandDecoder.h"

std::string server::CommandDecoder::callCMD( const std::string& command, const std::string& content,
        const size_t& i )
{
    if( command == "vx" )
        return _tvc.call( content, i );
    if( command== "sample" )
        return _tsc.call( content, i );
    if( command == "pulse" )
        return _tpc.call( content, i );
    if( ( ( command == "channels" ) || ( command == "whoami" ) ) )
        return _tic.call( command, 0 );
    return std::string( ( _prefix + ":UNKHOWN_COMMAND;" ) );
}

std::pair<bool, size_t> server::CommandDecoder::findPrefix( const std::string& content )
{
    for( size_t i = 0; i <= _prefix.size(); ++i )
        if( content.substr( 0, i ) == _prefix )
            return std::pair<bool, size_t>( false, ( i + 1 ) );
    return std::pair<bool, size_t>( true, 0 );
}

std::string server::CommandDecoder::decode( const std::string& content )
{
    std::pair<bool, size_t> it = findPrefix(content);
    if( it.first )
        return std::string( ( _prefix + ":NO_PREFIX;" ) );
    std::string command = "";
    for( size_t i = it.second; i < content.size(); ++i )
    {
        if( content[i] == '=' )
            return callCMD( command, content, ( i + 1 ) );
        command += content[i];
    }
    return std::string( ( _prefix + ":NO_OSC;" ) );
}


