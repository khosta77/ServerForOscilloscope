#ifndef PROJECT_TYPE_INFO_COMMANDS_H_
#define PROJECT_TYPE_INFO_COMMANDS_H_

#include "TypeCommands.h"

namespace server
{

    namespace typecommands
    {

        class TypeICommands final : public TypeCommands
        {
        public:
            TypeICommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                           const std::string& command ) : TypeCommands( osc, prefix, command ) {}

            ~TypeICommands() = default;

            /** @brief call - Вызов соответствующей команды
             *  @param command - команда
             *  @param content - контент
             *  @param i       - номер после =
             * */
            std::string call( const std::string& content, const size_t& i, const bool& isDualUse = false ) override;

        };

    };  // typecommands

};  // server

#endif  // PROJECT_TYPE_INFO_COMMANDS_H_
