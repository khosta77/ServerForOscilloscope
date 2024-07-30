#ifndef PROJECT_TYPE_SAMPLE_COMMANDS_H_
#define PROJECT_TYPE_SAMPLE_COMMANDS_H_

#include "TypeCommands.h"

namespace server
{

    namespace typecommands
    {
        class TypeSCommands final : public TypeCommands
        {
        private:

            /** @brief getRange - Метод получения размера для команд типа V, пример:
             *                    * [prefix]:sample=[value]:xor=valXor;
             *                    должно быть '_', чтобы вызвать метод
             *  @return - Пример:
             *            * [prefix]:sample=x1,x2,x3,...,xn:xor=valXor;
             *  @throw -  Пример:
             *            * [prefix]:sample=[error_code]:error=[error_name]:xor=valXor;
             *            + [error_code]: указывает на раздел, где была ошибка
             *            | ERROR_RANGE_UNKHOWN_PARAMETR - ошибка чтения запроса параметра
             *            | ERROR_RANGE_PROBLEM_GET      - ошибка выполнения получения уровня
             *            | ERROR_RANGE_EMPTY            - ошибка связанная с тем, что считанное значение пусто
             *            + [error_name]: может быть что угодно
             * */
            std::string getRange() override;

            /** @brief getCurrent - Получение текущего уровня выставленного на осциллографа пример команды:
             *                      * osc:scurrent=?:xor=valXor;
             *                      если вместо '?' будет, метод не вызовется
             *  @return - Пример:
             *            * [prefix]:sample=[value]:xor=valXor;
             *  @throw -  Пример:
             *            * [prefix]:sample=[error_code]:error=[error_name]:xor=valXor;
             *            + [error_code]: указывает на раздел, где была ошибка
             *            | ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN - ошибка перевода числа в int
             *            | ERROR_CURRENT_PROBLEM_GET            - ошибка выполнения получения уровня
             *            + [error_name]: может быть что угодно
             * */
            std::string getCurrent() override;

            /** @brief setValue - Метод установки значения уровня по вертикали на осциллографе пример команды:
             *                    * [prefix]:sample=[value]:xor=valXor;
             *                    Параметры:
             *                    | [value] - Новое значение
             *  @return - Пример:
             *            * [prefix]:sample=0:xor=valXor;
             *  @throw -  Пример:
             *            * [prefix]:sample=[error_code]:error=[error_name]:xor=valXor;
             *            + [error_code]: указывает на раздел, где была ошибка
             *            | ERROR_SET_UNKNOWN_PARAMETR - ошибка перевода параметра в int
             *            | ERROR_SET_PROBLEM_SET      - ошибка в методе
             *            + [error_name]: может быть что угодно
             * */
            std::string setValue() override;

            /** @brief getPulse - В данным сценарии не использовать
             * */
            std::string getPulse() override { return getErrorMessage(); }

        public:
            TypeSCommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                           const std::string& command ) : TypeCommands( osc, prefix, command, 1 ) {}

            ~TypeSCommands() { _param.clear(); }

            /** @brief call - Вызов соответствующей команды
             *  @param command - команда
             *  @param content - контент
             *  @param i       - номер после =
             * */
            std::string call( const std::string& content, const size_t& i ) override;

        };

    };  // typecommands

};  // server

#endif  // PROJECT_TYPE_SAMPLE_COMMANDS_H_
