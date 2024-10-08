#ifndef PROJECT_TYPE_PULSE_COMMANDS_H_
#define PROJECT_TYPE_PULSE_COMMANDS_H_

#include "TypeCommands.h"

namespace server
{

    namespace typecommands
    {

        class TypePCommands final : public TypeCommands
        {
        private:
            int _trig_CHx;
            int _trig_level;
            int _trig_comp;
 
            /** @brief getRange - Метод получения размера для команд типа V, пример:
             *                    * [prefix]:pulse=[delay],[points],[channel]:xor=valXor;
             *                    должно быть '_', чтобы вызвать метод
             *  @return - Пример:
             *            * [prefix]:sample=x1,x2,x3,...,xn:xor=valXor;
             *  @throw -  Пример:
             *            * [prefix]:sample=[error_code]:error=[error_name]:xor=valXor;
             *            + [error_code]: указывает на раздел, где была ошибка
             *            | ERROR_RANGE_PROBLEM_GET      - ошибка выполнения получения уровня
             *            | ERROR_RANGE_EMPTY            - ошибка связанная с тем, что считанное значение пусто
             *            + [error_name]: может быть что угодно
             * */
            std::string getRange();
    
            /** @brief getPulse - Метод считывания данных с канала, пример:
             *                    * osc:pget=[delay],[points],[channel]:xor=valXor;
             *                    Параметры:
             *                    | [delay] - задержка в милисекундах
             *                    | [points] - колличество точек
             *                    | [channel] - канал
             *  @return - пример:
             *            * osc:pget=x0,x1,x2,...,xn:xor=valXor;
             *            + Возвращаются все точки последовательно
             *  @throw -  пример:
             *            * osc:vset=[error_code]:error=[error_name]:xor=valXor;
             *            + [error_code]: указывает на раздел, где была ошибка
             *            | ERROR_GET_DELAY_UNKNOWN - Проблемы чтения данных
             *            | ERROR_GET_PROBLEM_GET   - Проблема считывания сигналов
             *            | ERROR_GET_MESSAGE_THROW - Проблема с подготовкой сообщения 
             *            + [error_name]: может быть что угодно
             * */
            std::string getPulse( const std::vector<std::string>& params );

            std::string pulseOperation( const std::string& content, const size_t& i );
        public:
            /** @brief вывзывается отдельно
             * */
            std::string trigOperation( const std::string& content, const size_t& i );

        public:
            TypePCommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                           const std::string& command ) : TypeCommands( osc, prefix, command ) {}

            ~TypePCommands() = default;

            /** @brief call - Вызов соответствующей команды
             *  @param command - команда
             *  @param content - контент
             *  @param i       - номер после =
             * */
            std::string call( const std::string& content, const size_t& i, const bool& isDualUse = false ) override;

        };

    };  // typecommands

};  // server

#endif  // PROJECT_TYPE_PULSE_COMMANDS_H_
