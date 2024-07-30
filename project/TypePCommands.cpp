#include "TypePCommands.h"

/** @brief stos - string to size_t
 *  @param str - string
 * */
static size_t stos( const std::string& str )
{
    size_t tmp;
    std::stringstream sstream(str);
    sstream >> tmp;
    return tmp;
}

std::string server::typecommands::TypePCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeSignalFrame(); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "range_is_null" );
    return getSuccessMessage(range);
}

std::vector<size_t> server::typecommands::TypePCommands::paramsToSizeT()
{
    return std::vector<size_t>{ stos(_param[0]), stos(_param[1]), stos(_param[2]) };
}

std::string server::typecommands::TypePCommands::getPulse()
{
    std::vector<size_t> newParams;
    try { newParams = paramsToSizeT(); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_DELAY_UNKNOWN, emsg ); };

    if( newParams[2] >= _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN, "" );

    std::this_thread::sleep_for(std::chrono::milliseconds(newParams[0]));

    oscilloscopes::OscSigframe osf;
    try { osf = _oscilloscope->getSignalFrame( newParams[1] ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_PROBLEM_GET, emsg ); };

    std::string returnMessage;
    try { returnMessage = getSuccessMessage( osf[newParams[2]]._signal ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_MESSAGE_THROW, emsg ); };
    return returnMessage;
}

std::string server::typecommands::TypePCommands::call( const std::string& content, const size_t& i )
{
    try  // Это на случай, если что-то ужасное случится
    {
        parseContent( content, i, 4 );
        if( ( ( _param[0] == "_" ) && ( _param[1] == "_" ) && ( _param[2] == "_" ) ) )
            return getRange();
        return getPulse();
    }
    catch( const std::string& emsg )
    {
        return getErrorMessage( "PULSE_EXTRA", emsg );
    };
    return getErrorUnknownMessage();
}


