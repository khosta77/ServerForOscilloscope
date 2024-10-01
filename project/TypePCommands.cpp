#include "TypePCommands.h"

static inline std::vector<int> paramsToSizeT( const std::vector<std::string>& params )
{
    return std::vector<int>{ std::stoi( params[0] ), std::stoi( params[1] ), std::stoi( params[2]) };
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
    {
        return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN );
    }

    if( newParams[0] <= -1 )  // Срабатывание по триггеру
    {
        if( newParams[2] != _trig_CHx )
        {
            return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN );
        }

        oscilloscopes::OscSignal os;

        try
        {
            _oscilloscope->onTrigger();
            os = _oscilloscope->getSignalFromTrigger( _trig_CHx, _trig_level, _trig_comp );
        }
        catch( const std::exception& emsg ) { return getErrorMessage( ERROR_TRIG_EXTRA, emsg ); };
        
        if( os._signal.empty() )
            return getErrorMessage( ERROR_TRIG_EXTRA );
        
        return getSuccessMessage( _trig_CHx, 25, os._signal );
    }
    
    // Обработка моментально/с задержкой считывания
    if( newParams[0] > 0 )  // Задержка
        std::this_thread::sleep_for(std::chrono::milliseconds(newParams[0]));

    oscilloscopes::OscSigframe osf;
    try
    {
        osf = _oscilloscope->getSignalFrame( newParams[1] );
        for( auto it = osf.begin(); it != osf.end(); ++it )
        {
            if( it->second._signal.size() != newParams[1] )
                it->second._signal.resize(newParams[1]);
        }
    }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_GET_PROBLEM_GET, emsg ); };

    std::string returnMessage;
    try
    {
        if( newParams[2] != 0 )  // Если мы хотим вернуть конкретный канал
        {
            uint8_t currentCHx = ( newParams[2] - 1 );
            returnMessage = getSuccessMessage( currentCHx, 25, osf[currentCHx]._signal );
        }
        else  // Если хотим вернуть все каналы
        {
            uint8_t CHS = _oscilloscope->getChannelsSize();
            std::vector<int> vec( ( osf[0]._signalSize * CHS ) );
            for( size_t i = 0, j = 0; i <  osf[0]._signalSize; ++i )
            {
                // Производим их миксование, тоесть будет так, что [V0_0,V0_1,V1_0,V1_1]
                for( uint8_t chx = 0; chx < CHS; ++chx )
                {
                    vec[j++] = osf[chx]._signal[i];
                }
            }
            returnMessage = getSuccessMessage( 0, 25, vec );
        }
    }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_GET_MESSAGE_THROW, emsg ); };
    return returnMessage;
}

std::string server::typecommands::TypePCommands::pulseOperation( const std::string& content, const size_t& i )
{
    std::string message = "";
    try
    {
        auto params = parseContent( content, i, 4 ).first;
        if( params.size() != 3 )
        {
            std::cout << content << std::endl;
            message = getErrorMessage( ERROR_TRIG_EXTRA );
        }
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
            return getErrorMessage( ERROR_TRIG_EXTRA, ( "(" + content + ")") );
        std::vector<int> newParams;
        try
        {
            _oscilloscope->offTrigger();
            _trig_CHx = std::stoi( params[0] );
            _trig_level = std::stoi( params[1] );
            _trig_comp = std::stoi( params[2] );
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


