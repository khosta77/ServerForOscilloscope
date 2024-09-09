#include "TypePCommands.h"

static std::vector<int> paramsToSizeT( const std::vector<std::string>& params )
{
    return std::vector<int>{ std::stoi( params[0] ), std::stoi( params[1] ), std::stoi( params[2]) };
}

/** @brief stos - string to typename
 *  @param str - string
 * */
template<typename T>
static T stot( const std::string& str )
{
    T tmp;
    std::stringstream sstream(str);
    sstream >> tmp;
    return tmp;
}


std::string server::typecommands::TypePCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeSignalFrame(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "range_is_null" );
    return getSuccessMessage(range);
}

std::string server::typecommands::TypePCommands::getPulse( const std::vector<std::string>& params )
{
    std::vector<int> newParams;
    try { newParams = paramsToSizeT(params); }
    catch( ... ) { return getErrorMessage( ERROR_GET_DELAY_UNKNOWN ); };

    if( newParams[2] > _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN );


    if( newParams[0] <= -1 )
    {
        if( newParams[2] != _trig_CHx )
            return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN );

        oscilloscopes::OscSignal os;
        try
        {
            _oscilloscope->onTrigger();
            os = _oscilloscope->getSignalFromTrigger( _trig_CHx, _trig_level, _trig_comp );
        } catch( const std::exception& emsg ) { return getErrorMessage( ERROR_TRIG_EXTRA, emsg ); };
        if( os._signal.empty() )
            return getErrorMessage( ERROR_TRIG_EXTRA );
        return getSuccessMessage( os._signal );
    }
    else
    {
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
                std::vector<int8_t> vec = osf[0]._signal;  // TODO: Переделать, замиксовать
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
}

std::string server::typecommands::TypePCommands::pulseOperation( const std::string& content, const size_t& i )
{
    std::string message = "";
    try
    {
        auto params = parseContent( content, i, 4 ).first;
        if( params.size() != 3 )
            message = getErrorMessage( ERROR_TRIG_EXTRA );
        else if( ( ( params[0] == "_" ) && ( params[1] == "_" ) && ( params[2] == "_" ) ) )
            message = getRange();
        else
            message = getPulse( params );
        params.clear();
    }
    catch( ... )
    {
        message = getErrorMessage( ERROR_PULSE_EXTRA );
    };
    return message;
}

std::string server::typecommands::TypePCommands::trigOperation( const std::string& content, const size_t& i )
{
    std::string message = "";
    try
    {
        auto params = parseContent( content, i, 4 ).first;
        if( params.size() != 3 )
            return getErrorMessage( ERROR_TRIG_EXTRA, "trig_param_error" );
        std::vector<int> newParams;
        try
        {
            _oscilloscope->offTrigger();
            _trig_CHx = stot<uint8_t>( params[0] );
            _trig_level = stot<float>( params[1] );
            _trig_comp = stot<uint8_t>( params[2] );
        }
        catch( const std::exception& emsg ) { return getErrorMessage( ERROR_GET_DELAY_UNKNOWN, emsg ); }
        message = getSuccessMessage( "trig", params );
        params.clear();
        return message;
    }
    catch( ... )
    {
        message = getErrorMessage( ERROR_TRIG_EXTRA );
    };
    return message;
}

std::string server::typecommands::TypePCommands::call( const std::string& content, const size_t& i,
                                                       const bool& isDualUse )
{
    std::string returnMessage = ( ( !isDualUse ) ? pulseOperation( content, i ) : trigOperation( content, i ) );
    if( returnMessage.empty() )
        returnMessage = getErrorUnknownMessage();
    return returnMessage;
}


