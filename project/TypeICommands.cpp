#include "TypeICommands.h"

std::string server::typecommands::TypeICommands::call( const std::string& content, const size_t& i, const bool& isDualUse )
{
    try
    {
        if( content == "channels" )
        {
            uint8_t ch = 0;
            try { ch = _oscilloscope->getChannelsSize(); }
            catch( ... ) { return getErrorMessage( ERROR_INFO_EXTRA ); };
            return getSuccessMessage(ch);
        }
        if( content == "whoami" )
        {
            std::string name = "";
            try { name = _oscilloscope->whoAmI(); }
            catch( ... ) { return getErrorMessage( ERROR_INFO_EXTRA ); };
            return getSuccessMessage(name);
        }
    }
    catch( const std::exception& emsg )
    {
        return getErrorMessage( ERROR_INFO_EXTRA );
    };
    return getErrorUnknownMessage();
}


