#ifndef PROJECT_TYPE_INFO_COMMANDS_H_
#define PROJECT_TYPE_INFO_COMMANDS_H_

#include "TypeCommands.h"

namespace server
{

    namespace typecommands
    {

        class TypeICommands final : public TypeCommands
        {
        private:

            /** @brief getCurrent - В данным сценарии не использовать
             * */
            std::string getRange() override { return getErrorMessage(); }
    
            /** @brief getCurrent - В данным сценарии не использовать
             * */
            std::string getCurrent() override { return getErrorMessage(); }

            /** @brief setValue - В данным сценарии не использовать
             * */
            std::string setValue() override { return getErrorMessage(); }

            /** @brief getCurrent - В данным сценарии не использовать
             * */
            std::string getPulse() override { return getErrorMessage(); }

        public:
            TypeICommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                           const std::string& command ) : TypeCommands( osc, prefix, command ) {}

            ~TypeICommands() { _param.clear(); }

            /** @brief call - Вызов соответствующей команды
             *  @param command - команда
             *  @param content - контент
             *  @param i       - номер после =
             * */
            std::string call( const std::string& content, const size_t& i ) override;

        };

    };  // typecommands

};  // server

#endif  // PROJECT_TYPE_INFO_COMMANDS_H_
