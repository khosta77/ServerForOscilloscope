#ifndef PROJECT_COMMAND_DECODER_H_
#define PROJECT_COMMAND_DECODER_H_

#include "TypeVCommands.h"
#include "TypeSCommands.h"
#include "TypePCommands.h"

namespace server
{

    class CommandDecoder
    {
    private:
        const std::string _prefix;

        typecommands::TypeVCommands _tvc;
        typecommands::TypeSCommands _tsc;
        typecommands::TypePCommands _tpc;

        std::string callCMD( const std::string& command, const std::string& content, const size_t& i );
        std::pair<bool, size_t> findPrefix( const std::string& content );

    public:
        CommandDecoder( const std::string prefix, oscilloscopes::Oscilloscope *osc ) : _prefix(prefix),
            _tvc( osc, _prefix, "vx" ), _tsc( osc, _prefix, "scale" ), _tpc( osc, _prefix, "pulse" ) {}

        ~CommandDecoder() = default;

        std::string decode( const std::string& content );

    };  // CommandDecoder

};  // server

#endif  // PROJECT_COMMAND_DECODER_H_
