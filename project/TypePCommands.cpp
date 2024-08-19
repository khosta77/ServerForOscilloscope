#include "TypePCommands.h"

std::string server::typecommands::TypePCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeSignalFrame(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "range_is_null" );
    return getSuccessMessage(range);
}

std::vector<int> server::typecommands::TypePCommands::paramsToSizeT( const std::vector<std::string>& params )
{
    return std::vector<int>{ std::stoi( params[0] ), std::stoi( params[1] ), std::stoi( params[2]) };
}

std::string server::typecommands::TypePCommands::getPulse( const std::vector<std::string>& params )
{
    std::vector<int> newParams;
    try { newParams = paramsToSizeT(params); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_GET_DELAY_UNKNOWN, emsg ); };

    if( newParams[2] > _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN, "" );

    if( newParams[0] <= -1 )
        return getErrorMessage( "NO_TRIG", "NO_TRIG" );

    if( newParams[0] > 0 )
        std::this_thread::sleep_for(std::chrono::milliseconds(newParams[0]));

    oscilloscopes::OscSigframe osf;
    try { osf = _oscilloscope->getSignalFrame( newParams[1] ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_GET_PROBLEM_GET, emsg ); };

    if( newParams[2] == 0 )
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN, "" );  

    std::string returnMessage;
    try {
        if( newParams[2] == 0 )
            returnMessage = getSuccessMessage( osf[( newParams[2] - 1 )]._signal );
        else
        {
            std::vector<float> vec = osf[0]._signal;
            for( size_t i = 1; i < _oscilloscope->getChannelsSize(); ++i )
            {
                for( const auto& it : osf[i]._signal )
                    vec.push_back( it );
            }
            returnMessage = getSuccessMessage( vec );
        }
    } catch( const std::exception& emsg ) { return getErrorMessage( ERROR_GET_MESSAGE_THROW, emsg ); };
    return returnMessage;
}

std::string server::typecommands::TypePCommands::call( const std::string& content, const size_t& i )
{
    try  // Это на случай, если что-то ужасное случится
    {
        auto params = parseContent( content, i, 4 ).first;
        if( params.size() != 3 )
            return getErrorMessage( "1", "" );
        if( ( ( params[0] == "_" ) && ( params[1] == "_" ) && ( params[2] == "_" ) ) )
            return getRange();
        return getPulse( params );
    }
    catch( const std::exception& emsg )
    {
        return getErrorMessage( "PULSE_EXTRA", emsg );
    };
    return getErrorUnknownMessage();
}


