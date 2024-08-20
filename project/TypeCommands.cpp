#include "TypeCommands.h"

/** @brief write - записывает огромный массив в строку капец как быстро, исправил ошибку старого метода, 
 *                 теперь все тип-топ
 *  @param prm - вектор
 *  @param prefix - префикс
 *  @param command - команда
 * */
template<typename T>
static std::string write( const std::vector<T>& prm, const std::string& prefix, const std::string& command )
{
    std::ostringstream oss;
    oss << ( prefix + ":" + command + "=" );
    for( size_t i = 0, _I = ( prm.size() - 1 ), I = prm.size() ; i < I ; ++i )
    {
        oss << prm[i];
        if( i != _I )
            oss << ",";
    }
    oss << ":xor=valXor;";
    return oss.str();
}

std::pair<std::vector<std::string>, size_t> server::typecommands::TypeCommands::parseContent(
        const std::string& content, const size_t& i, const size_t& end )
{
    std::vector<std::string> param;
    size_t startXOR = 0;
    std::string buffer = "";
    for( size_t j = i, pi = 0; j < content.size(); ++j )
    {
        if( ( content[j] == ',' ) || ( content[j] == ':' ) || ( content[j] == '.' ) )
        {
            ++pi;
            param.push_back(buffer);
            buffer = "";
        }
        else
            buffer += content[j];

        if( pi == end )
        {
            startXOR = ( j + 1 );
            buffer.clear();
            break;
        }
    }
    buffer.clear();
    return std::pair<std::vector<std::string>, size_t>( param, startXOR );
}

void sendToSock( const int& sock, const std::string& messsage )
{
    const char* dataPtr = messsage.c_str();
    size_t dataSize = messsage.length();
    size_t totalSent = 0;
    while( totalSent < dataSize )
    {
        int bytesSent = send( sock, ( dataPtr + totalSent ), ( dataSize - totalSent ), 0 );
        if( bytesSent == -1 )
            break;
        totalSent += bytesSent;
    }
}


std::string server::typecommands::TypeCommands::getErrorMessage( const std::string& prm,
                                                                 const std::exception& emsg )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=bad," + prm + ":error=" + std::string( emsg.what() )
                         + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorMessage( const std::string& prm,
                                                                 const std::string& emsg )
{
    if( emsg == "" )
        return std::string( ( _PREFIX + ":" + _COMMAND + "=bad:error=" + prm + ":xor=valXor;" ) );
    return std::string( ( _PREFIX + ":" + _COMMAND + "=bad" + prm + ":error=" + emsg + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorMessage()
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=bad:error=" + ERROR_NO_METHOD + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorUnknownMessage()
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=bad:error=" + ERROR_UNKNOWN_MESSAGE + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorMessage( const std::string& command,
                                                                 const int& codeError )
{
    return std::string( ( _PREFIX + ":" + command + "=bad:error="
                + std::to_string(codeError) + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const size_t& prm )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + std::to_string(prm) + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::string& prm )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + prm + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::string& command,
                                                                   const std::vector<std::string>& params )
{
    std::ostringstream oss;
    oss << ( _PREFIX + ":" + command + "=ok," );
    for( size_t i = 0, _I = ( params.size() - 1 ), I = params.size() ; i < I ; ++i )
    {
        oss << params[i];
        if( i != _I )
            oss << ",";
    }
    oss << ":xor=valXor;";
    return oss.str();
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::vector<size_t>& prm )
{
    return write<size_t>( prm, _PREFIX, _COMMAND );
}

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::vector<float>& prm )
{
    return write<float>( prm, _PREFIX, _COMMAND );
}

std::string server::typecommands::TypeCommands::getSuccessMessage()
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=0:xor=valXor;" ) );
}


