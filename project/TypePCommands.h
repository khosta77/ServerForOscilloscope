#ifndef PROJECT_TYPE_PULSE_COMMANDS_H_
#define PROJECT_TYPE_PULSE_COMMANDS_H_

#include "TypeCommands.h"

class TypePCommands final : public TypeCommands
{
private:

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
    std::string getRange() override
    {
        std::vector<size_t> range;
        try { range = _oscilloscope->getRangeSignalFrame(); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

        if( range.empty() )
            return getErrorMessage( ERROR_RANGE_EMPTY, "range_is_null" );
        return getSuccessMessage(range);
    }
    
    /** @brief getCurrent - В данным сценарии не использовать
     * */
    std::string getCurrent() override { return getErrorMessage(); }

    /** @brief setValue - В данным сценарии не использовать
     * */
    std::string setValue() override { return getErrorMessage(); }

    /** @brief stos - string to size_t
     *  @param str - string
     * */
    size_t stos( const std::string& str )
    {
        size_t tmp;
        std::stringstream sstream(str);
        sstream >> tmp;
        return tmp;
    }

    std::vector<size_t> paramsToSizeT()
    {
        return std::vector<size_t>{ stos(_param[0]), stos(_param[1]), stos(_param[2]) };
    }

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
    std::string getPulse() override
    {
        std::vector<size_t> newParams;
        try { newParams = paramsToSizeT(); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_DELAY_UNKNOWN, emsg ); };

        if( newParams[2] >= _oscilloscope->getChannelsSize() )
            return getErrorMessage( ERROR_GET_CHANNEL_NUMBER_UNKNOWN, "" );

        std::this_thread::sleep_for(std::chrono::milliseconds(newParams[0]));

        oscilloscopes::OscSigframe osf;
        try { osf = _oscilloscope->getSignalFrame( newParams[1] ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_PROBLEM_GET, emsg ); };

        std::string returnMessage;
        try { returnMessage = getSuccessMessage( osf[newParams[2]]._signal ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_GET_MESSAGE_THROW, emsg ); }; 
        return returnMessage;
    }

public:
    TypePCommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                   const std::string& command ) : TypeCommands( osc, prefix, command, 4 ) {}

    ~TypePCommands() { _param.clear(); }

    /** @brief call - Вызов соответствующей команды
     *  @param command - команда
     *  @param content - контент
     *  @param i       - номер после =
     * */
    std::string call( const std::string& content, const size_t& i ) override
    {
        try  // Это на случай, если что-то ужасное случится
        {
            parseContent( content, i, 4 );
            if( ( ( _param[0] == "_" ) && ( _param[1] == "_" ) && ( _param[2] == "_" ) ) )
                return getRange();
            return getPulse();
        }
        catch( const std::string& emsg )
        {
            return getErrorMessage( "PULSE_EXTRA", emsg );
        };
        return getErrorUnknownMessage();
    }
};

#endif  // PROJECT_TYPE_PULSE_COMMANDS_H_
