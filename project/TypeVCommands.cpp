#include "TypeVCommands.h"

std::string server::typecommands::TypeVCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeInputLevel(); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "RANGE_IS_NULL" );
    return getSuccessMessage(range);
}

std::string server::typecommands::TypeVCommands::getCurrent()
{
    int channel_number = 0;
    try { channel_number = std::stoi( _param[0] ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, emsg ); };

    if( channel_number >= _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "" );

    size_t valueV = 0;
    try { valueV = _oscilloscope->getInputLevel( channel_number ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_CURRENT_PROBLEM_GET, emsg ); };

    return getSuccessMessage( valueV );
}

std::string server::typecommands::TypeVCommands::setValue()
{
    int channel_number = 0;
    try { channel_number = std::stoi( _param[0] ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_SET_CHANNEL_NUMBER_UNKNOWN, emsg ); };

    if( channel_number >= _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "" );

    int new_v_value = 0;
    try { new_v_value = std::stoi( _param[1] ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_SET_UNKNOWN_NEW_LEVEL, emsg ); };

    try { _oscilloscope->setInputLevel( channel_number, new_v_value ); }
    catch( const std::string& emsg ) { return getErrorMessage( ERROR_SET_PROBLEM_SET, emsg ); };

    return getSuccessMessage();
}

std::string server::typecommands::TypeVCommands::call( const std::string& content, const size_t& i )
{
    try  // Это на случай, если что-то ужасное случится
    {
        parseContent( content, i, 3 );
        if( ( ( _param[0] == "_" ) && ( _param[1] == "_" ) ) )
            return getRange();
        if( _param[1] == "?" )
            return getCurrent();
        return setValue();
    }
    catch( const std::string& emsg )
    {
        return getErrorMessage( "VX_EXTRA", emsg );
    };
    return getErrorUnknownMessage();
}


