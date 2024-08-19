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

#if 0
void server::typecommands::TypeCommands::clear()
{
    if( !_param.empty() )
    {
        for( auto& it : _param )
        {
            if( !it.empty() )
                it.clear();
        }
        _param.clear();
    }
    _startXOR = 0;
}
#endif

std::pair<std::vector<std::string>, size_t> server::typecommands::TypeCommands::parseContent(
        const std::string& content, const size_t& i, const size_t& end )
{
    //clear();
    std::vector<std::string> param;
    size_t startXOR = 0;

    std::string buffer = "";  // Вот тут была зарыта собака
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

std::string server::typecommands::TypeCommands::getErrorMessage( const std::string& prm,
                                                                 const std::exception& emsg )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + prm + ":error=" + std::string( emsg.what() )
                         + ":xor=valXor;" ) );
}

std::string server::typecommands::TypeCommands::getErrorMessage( const std::string& prm,
                                                                 const std::string& emsg )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + prm + ":error=" + emsg + ":xor=valXor;" ) );
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

std::string server::typecommands::TypeCommands::getSuccessMessage( const std::string& prm )
{
    return std::string( ( _PREFIX + ":" + _COMMAND + "=" + prm + ":xor=valXor;" ) );
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


