#ifndef PROJECT_COMMAND_DECODER_H_
#define PROJECT_COMMAND_DECODER_H_

#include "TypeVCommands.h"
#include "TypeSCommands.h"
#include "TypePCommands.h"
#include "TypeICommands.h"

namespace server
{

    class CommandDecoder
    {
    private:
        const std::string _prefix;

        typecommands::TypeVCommands _tvc;
        typecommands::TypeSCommands _tsc;
        typecommands::TypePCommands _tpc;
        typecommands::TypeICommands _tic;

        std::string callCMD( const std::string& command, const std::string& content, const size_t& i );
        std::pair<bool, size_t> findPrefix( const std::string& content );

    public:
        CommandDecoder( const std::string& prefix, oscilloscopes::Oscilloscope *osc ) : _prefix(prefix),
            _tvc( osc, _prefix, "vx" ), _tsc( osc, _prefix, "sample" ), _tpc( osc, _prefix, "pulse" ),
            _tic( osc, _prefix, "info" ) {}

        ~CommandDecoder() = default;

        std::string decode( const std::string& content );

    };  // CommandDecoder

};  // server

#endif  // PROJECT_COMMAND_DECODER_H_
