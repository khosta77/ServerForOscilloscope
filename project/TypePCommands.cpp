#include "TypePCommands.h"

std::string server::typecommands::TypePCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeSignalFrame(); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "range_is_null" );
    return getSuccessMessage(range);
}

std::vector<int> server::typecommands::TypePCommands::paramsToSizeT()
{
    return std::vector<int>{ std::stoi(_param[0]), std::stoi(_param[1]), std::stoi(_param[2]) };
}

std::string server::typecommands::TypePCommands::getPulse()
{
    std::vector<int> newParams;
    try { newParams = paramsToSizeT(); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_DELAY_UNKNOWN, emsg ); };

    if( newParams[2] > _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN, "" );

    if( newParams[0] <= -1 )
        return getErrorMessage( "NO_TRIG", "NO_TRIG" );

    if( newParams[0] > 0 )
        std::this_thread::sleep_for(std::chrono::milliseconds(newParams[0]));

    oscilloscopes::OscSigframe osf;
    try { osf = _oscilloscope->getSignalFrame( newParams[1] ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_PROBLEM_GET, emsg ); };

    if( newParams[2] == 0 )
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN, "" );  

    std::string returnMessage;
    try { returnMessage = getSuccessMessage( osf[( newParams[2] - 1 )]._signal ); }
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


