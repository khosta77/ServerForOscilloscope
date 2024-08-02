#include "TypeICommands.h"

std::string server::typecommands::TypeICommands::call( const std::string& content, const size_t& i )
{
    try  // Это на случай, если что-то ужасное случится
    {
        if( content == "channels" )
        {
            uint8_t ch = 0;
            try { ch = _oscilloscope->getChannelsSize(); }
            catch( const std::exception& emsg ) { return getErrorMessage( "getChSize", emsg ); };
            return getSuccessMessage(ch);
        }
        if( content == "whoami" )
        {
            std::string name = "";
            try { name = _oscilloscope->whoAmI(); }
            catch( const std::exception& emsg ) { return getErrorMessage( "WhoAmI", emsg ); };
            return getSuccessMessage(name);
        }
    }
    catch( const std::exception& emsg )
    {
        return getErrorMessage( "PULSE_EXTRA", emsg );
    };
    return getErrorUnknownMessage();
}


