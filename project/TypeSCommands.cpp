#include "TypeSCommands.h"

std::string server::typecommands::TypeSCommands::getRange()
{
    std::vector<size_t> range;
    try { range = _oscilloscope->getRangeSampleRate(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

    if( range.empty() )
        return getErrorMessage( ERROR_RANGE_EMPTY );
    return getSuccessMessage(range);
}

std::string server::typecommands::TypeSCommands::getCurrent()
{
    size_t value = 0;
    try { value = _oscilloscope->getSampleRate(); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_CURRENT_PROBLEM_GET, emsg ); };
    return getSuccessMessage( value );
}

std::string server::typecommands::TypeSCommands::setValue( const std::vector<std::string>& params )
{
    int new_s_value = 0;
    try { new_s_value = std::stoi( params[0] ); }
    catch( ... ) { return getErrorMessage( ERROR_SET_UNKNOWN_PARAMETR ); };

    try { new_s_value = _oscilloscope->setSampleRate( new_s_value ); }
    catch( const std::exception& emsg ) { return getErrorMessage( ERROR_SET_PROBLEM_SET, emsg ); };
    return getSuccessMessage( new_s_value );
}

std::string server::typecommands::TypeSCommands::call( const std::string& content, const size_t& i, const bool& isDualUse )
{
    std::string returnMessage = "";
    try
    {
        auto params = parseContent( content, i, 2 ).first;
        if( params.size() != 1 )
            returnMessage = getErrorMessage( ERROR_SAMPLE_EXTRA );
        else if( params[0] == "_" )
            returnMessage = getRange();
        else if( params[0] == "?" )
            returnMessage = getCurrent();
        else
            returnMessage = setValue( params );
        params.clear();
    }
    catch( ... )
    {
        returnMessage = getErrorMessage( ERROR_SAMPLE_EXTRA );
    };

    if( returnMessage.empty() )
        returnMessage = getErrorUnknownMessage();
    
    return returnMessage;
}


