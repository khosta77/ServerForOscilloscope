#include "TypeCommands.h"

void server::typecommands::TypeCommands::clear()
{
    for( auto& it : _param )
        it = "";
    _startXOR = 0;
}

void server::typecommands::TypeCommands::parseContent( const std::string& content, const size_t& i,
                                                       const size_t& end )
{
    clear();
    for( size_t j = i, pi = 0; j < content.size(); ++j )
    {
        if( ( content[j] == ',' ) || ( content[j] == ':' ) )
            ++pi;
        else
            _param[pi] += content[j];
        if( pi == end )
        {
            _startXOR = ( j + 1 );
            break;
        }
    }
}

std::string server::typecommands::TypeCommands::getErrorMessage( const std::string& prm, const std::string& err )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + prm + ":error=" + err + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorMessage()
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=-98:error=NO_METHOD:xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorUnknownMessage()
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=-99:error=UNKHOWN_COMMAND:xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const size_t& prm )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + std::to_string(prm) + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::vector<size_t>& prm )
{
    std::string message = ( _PREFIX + ":" + _COMMAND + "=" );
    for( const auto& it : prm )  // На больших значениях очень плохо
        message = ( message + std::to_string(it) + "," );
    return ( message + ":xor=valXor;" );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::vector<uint16_t>& prm )
{
    std::string message = ( _PREFIX + ":" + _COMMAND + "=" );
    for( const auto& it : prm )  // На больших значениях очень плохо
        message = ( message + std::to_string(it) + "," );
    return ( message + ":xor=valXor;" );
}

std::string server::typecommands::TypeCommands::getSuccessMessage()
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=0:xor=valXor;" ) );
}


