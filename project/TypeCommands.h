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

// OSCILLOSCOPE_ERROR_INVALID_PARAM 1
// OSCILLOSCOPE_SUCCESS             0
// OSCILLOSCOPE_ERROR_ACCESS        -3
// OSCILLOSCOPE_ERROR_NO_DEVICE     -4
// OSCILLOSCOPE_ERROR_TIMEOUT       -7
// OSCILLOSCOPE_ERROR_NO_MEM        -11
// OSCILLOSCOPE_ERROR_OTHER         -99

#define ERROR_RANGE_UNKHOWN_PARAMETR         "-31"
#define ERROR_RANGE_PROBLEM_GET              "-41"
#define ERROR_RANGE_EMPTY                    "-61"

#define ERROR_CURRENT_UNKNOWN_PARAMETR       "-32"
#define ERROR_CURRENT_PROBLEM_GET            "-42"
#define ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN "-51"

#define ERROR_SET_UNKNOWN_NEW_LEVEL          "-33"
#define ERROR_SET_UNKNOWN_PARAMETR           "-34"
#define ERROR_SET_PROBLEM_SET                "-43"
#define ERROR_SET_CHANNEL_NUMBER_UNKNOWN     "-52"

#define ERROR_GET_DELAY_UNKNOWN              "-35"
#define ERROR_GET_PROBLEM_GET                "-44"
#define ERROR_GET_CHANNEL_NUMBER_UNKNOWN     "-53"
#define ERROR_GET_MESSAGE_THROW              "-62"

#define ERROR_INFO_EXTRA                     "-21"
#define ERROR_PULSE_EXTRA                    "-22"
#define ERROR_SAMPLE_EXTRA                   "-23"
#define ERROR_VX_EXTRA                       "-24"
#define ERROR_TRIG_EXTRA                     "-25"

#define ERROR_NO_PREFIX                      "-94"
#define ERROR_NO_OSC                         "-95"
#define ERROR_NO_METHOD                      "-96"
#define ERROR_UNKNOWN_MESSAGE                "-97"
#define ERROR_UNKNOWN_COMMAND                "-98"

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
            std::string getErrorMessage( const std::string& prm, const std::string& emsg = "" );
            std::string getErrorMessage( const std::string& command, const int& codeError );

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
