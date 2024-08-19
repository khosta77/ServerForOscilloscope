#include "TypeVCommands.h"

std::string server::typecommands::TypeVCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeInputLevel(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "RANGE_IS_NULL" );
    return getSuccessMessage(range);
}

std::string server::typecommands::TypeVCommands::getCurrent( const std::vector<std::string>& params )
{
    int channel_number = 0;
    try { channel_number = std::stoi( params[0] ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, emsg ); };

    if( channel_number > _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "" );

    if( channel_number == 0 )
        return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "IS_NULL" );

    size_t valueV = 0;
    try { valueV = _oscilloscope->getInputLevel( ( channel_number - 1 ) ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_CURRENT_PROBLEM_GET, emsg ); };

    return getSuccessMessage( valueV );
}

std::string server::typecommands::TypeVCommands::setValue( const std::vector<std::string>& params )
{
    int channel_number = 0;
    try { channel_number = std::stoi( params[0] ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_SET_CHANNEL_NUMBER_UNKNOWN, emsg ); };

    if( channel_number >= _oscilloscope->getChannelsSize() )
        return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "" );

    if( channel_number == 0 )
        return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "IS_NULL" );

    int new_v_value = 0;
    try { new_v_value = std::stoi( params[1] ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_SET_UNKNOWN_NEW_LEVEL, emsg ); };

    try { new_v_value = _oscilloscope->setInputLevel( ( channel_number - 1 ), new_v_value ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_SET_PROBLEM_SET, emsg ); };

    return getSuccessMessage( new_v_value );
}

std::string server::typecommands::TypeVCommands::call( const std::string& content, const size_t& i, const bool& isDualUse )
{
    try
    {
        auto params = parseContent( content, i, 3 ).first;
        if( params.size() != 2 )
            return getErrorMessage( "1", "" );
        if( ( ( params[0] == "_" ) && ( params[1] == "_" ) ) )
            return getRange();
        if( params[1] == "?" )
            return getCurrent( params );
        return setValue( params );
    }
    catch( const std::exception& emsg )
    {
        return getErrorMessage( "VX_EXTRA", emsg );
    };
    return getErrorUnknownMessage();
}


