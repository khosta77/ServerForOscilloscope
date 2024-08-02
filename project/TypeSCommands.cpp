#include "TypeSCommands.h"

std::string server::typecommands::TypeSCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeSampleRate(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY, "RANGE_IS_NULL" );
    return getSuccessMessage(range);
}

std::string server::typecommands::TypeSCommands::getCurrent()
{
    size_t value = 0;
    try { value = _oscilloscope->getSampleRate(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_CURRENT_PROBLEM_GET, emsg ); };
    return getSuccessMessage( value );
}

std::string server::typecommands::TypeSCommands::setValue()
{
    int new_s_value = 0;
    try { new_s_value = std::stoi( _param[0] ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_SET_UNKNOWN_PARAMETR, emsg ); };

    try { _oscilloscope->setSampleRate( new_s_value ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_SET_PROBLEM_SET, emsg ); };
    return getSuccessMessage();
}

std::string server::typecommands::TypeSCommands::call( const std::string& content, const size_t& i )
{
    try  // Это на случай, если что-то ужасное случится
    {
        parseContent( content, i, 2 );
        if( _param[0] == "_" )
            return getRange();
        if( _param[0] == "?" )
            return getCurrent();
        return setValue();
    }
    catch( const std::exception& emsg )
    {
        return getErrorMessage( "SAMPLE_EXTRA", emsg );
    };
    return getErrorUnknownMessage();
}


