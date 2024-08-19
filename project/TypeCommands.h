#ifndef PROJECT_TYPE_COMMANDS_H_
#define PROJECT_TYPE_COMMANDS_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <chrono>
#include <exception>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <atomic>
#include <sstream>
#include <type_traits>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "oscilloscopes.h"

#define ERROR_RANGE_UNKHOWN_PARAMETR "param"
#define ERROR_RANGE_PROBLEM_GET      "-1"
#define ERROR_RANGE_EMPTY            "fail"

#define ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN "ch_unknown"
#define ERROR_CURRENT_PROBLEM_GET "throw_in_method"
#define ERROR_CURRENT_UNKNOWN_PARAMETR "param"

#define ERROR_SET_CHANNEL_NUMBER_UNKNOWN "fail"
#define ERROR_SET_UNKNOWN_NEW_LEVEL "fail"
#define ERROR_SET_PROBLEM_SET "fail"
#define ERROR_SET_UNKNOWN_PARAMETR "fail"

#define ERROR_GET_DELAY_UNKNOWN "parseContent"
#define ERROR_GET_PROBLEM_GET "getSignalFrame"
#define ERROR_GET_MESSAGE_THROW "getSuccessMessageGet"
#define ERROR_GET_CHANNEL_NUMBER_UNKNOWN "ch_unknown"

namespace server
{

    namespace typecommands
    {

        class TypeCommands
        {
        protected:
            oscilloscopes::Oscilloscope *_oscilloscope;
    
            const std::string _PREFIX;
            const std::string _COMMAND;

            /** @brief parseContent - парсер \content на наличие параметров
             *  @param content - вся строка с контентом
             *  @param i - номер после =
             *  @param end - колличество параметров, которые надо счить + 1
             * */
            std::pair<std::vector<std::string>, size_t> parseContent( const std::string& content,
                                                                      const size_t& i, const size_t& end );

            /** @brief sendToSock - метод для отправки данных в сокет
             *  @param sock - сокет
             *  @param message - сообщение.
             * */
            void sendToSock( const int& sock, const std::string& messsage );
            
            /** @brief - Метод для возвращения ошибки, если такая ситуация произошла
             *  @param prm - параметр, если требуется
             *  @param err - ошибка
             * */
            std::string getErrorMessage( const std::string& prm, const std::exception& emsg );
            std::string getErrorMessage( const std::string& prm, const std::string& emsg );

            std::string getErrorMessage();

            std::string getErrorUnknownMessage();

            /** @brief getSuccessMessage - Метод возвращает сообщение успеха соединения, для size_t
             *  @param prm - параметр, который надо вернуть
             * */
            std::string getSuccessMessage( const size_t& prm );
            std::string getSuccessMessage( const std::string& prm );
            std::string getSuccessMessage( const std::string& command,
                                           const std::vector<std::string>& params );

            /** @brief getSuccessMessage - Метод возвращает сообщение успеха соединения, для std::vector<T>
             *  @param prm - параметр, который надо вернуть
             * */
            std::string getSuccessMessage( const std::vector<size_t>& prm );
            std::string getSuccessMessage( const std::vector<float>& prm );

            /** @brief getSuccessMessage - Метод возвращает сообщение успеха соединения
             * */
            std::string getSuccessMessage();

        public:
            /** @brief TypeCommands Конструктор инициализации, для уменьшения строчек кода в дальнейшем
             * */
            TypeCommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                          const std::string& command ) : _oscilloscope(osc), _PREFIX(prefix),
                          _COMMAND(command) {}

            virtual std::string call( const std::string& content, const size_t& i, const bool& isDualUse ) = 0;
        };

    };  // typecommands

};  // server

#endif  // PROJECT_TYPE_COMMANDS_H_
