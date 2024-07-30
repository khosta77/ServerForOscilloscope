#ifndef PROJECT_TYPE_V_COMMANDS_H_
#define PROJECT_TYPE_V_COMMANDS_H_

#include "TypeCommands.h"

class TypeVCommands final : public TypeCommands
{
private:

    /** @brief getRange - Метод получения размера для команд типа V, пример:
     *                    * [prefix]:vx=[channel],[level]:xor=valXor;
     *                    В параметрах [channel],[level] - должно быть _ и _, чтобы вызвать данный метод
     *  @return - Пример:
     *            * [prefix]:vx=x1,x2,x3,...,xn:xor=valXor;
     *  @throw -  Пример:
     *            * [prefix]:vx=[error_code]:error=[error_name]:xor=valXor;
     *            + [error_code]: указывает на раздел, где была ошибка
     *            | ERROR_RANGE_PROBLEM_GET      - ошибка выполнения получения уровня
     *            | ERROR_RANGE_EMPTY            - ошибка связанная с тем, что считанное значение пусто
     *            + [error_name]: может быть что угодно
     * */
    std::string getRange() override
    {
        std::vector<size_t> range;
        try { range = _oscilloscope->getRangeInputLevel(); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_RANGE_PROBLEM_GET, emsg ); };

        if( range.empty() )
            return getErrorMessage( ERROR_RANGE_EMPTY, "RANGE_IS_NULL" );
        return getSuccessMessage(range);
    }

    /** @brief getCurrent - Получение текущего уровня выставленного на осциллографа пример команды:
     *                      * [prefix]:vx=[channel],[level]:xor=valXor;
     *                      В параметрах [channel],[level] - должно быть ? и ?, чтобы вызвать данный метод
     *  @return - Пример:
     *            * osc:vx=[value]:xor=valXor;
     *  @throw -  Пример:
     *            * osc:vx=[error_code]:error=[error_name]:xor=valXor;
     *            + [error_code]: указывает на раздел, где была ошибка
     *            | ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN - ошибка перевода числа в int
     *            | ERROR_CURRENT_PROBLEM_GET            - ошибка выполнения получения уровня
     *            + [error_name]: может быть что угодно
     * */
    std::string getCurrent() override
    {
        int channel_number = 0;
        try { channel_number = std::stoi( _param[0] ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, emsg ); };

        if( channel_number >= _oscilloscope->getChannelsSize() )
            return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "" );

        size_t valueV = 0;
        try { valueV = _oscilloscope->getInputLevel( channel_number ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_CURRENT_PROBLEM_GET, emsg ); };

        return getSuccessMessage( valueV );
    }

    /** @brief setValue - Метод установки значения уровня по вертикали на осциллографе пример команды:
     *                    * [prefix]:vx=[channel],[level]:xor=valXor;
     *                    Параметры:
     *                    | [channel] - номер доступного канал
     *                    | [level]   - новое значение в канале
     *  @return - Пример:
     *            * osc:vset=0:xor=valXor;
     *  @throw -  Пример:
     *            * osc:vset=[error_code]:error=[error_name]:xor=valXor;
     *            + [error_code]: указывает на раздел, где была ошибка
     *            | ERROR_SET_CHANNEL_NUMBER_UNKNOWN - ошибка перевода номера канал в int
     *            | ERROR_SET_UNKNOWN_NEW_LEVEL      - ошибка перевода уровня в int
     *            | ERROR_SET_PROBLEM_SET            - ошибка в методе
     *            + [error_name]: может быть что угодно
     * */
    std::string setValue() override
    {
        int channel_number = 0;
        try { channel_number = std::stoi( _param[0] ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_SET_CHANNEL_NUMBER_UNKNOWN, emsg ); };
        
        if( channel_number >= _oscilloscope->getChannelsSize() )
            return getErrorMessage( ERROR_CURRENT_CHANNEL_NUMBER_UNKNOWN, "" );

        int new_v_value = 0;
        try { new_v_value = std::stoi( _param[1] ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_SET_UNKNOWN_NEW_LEVEL, emsg ); };
        
        try { _oscilloscope->setInputLevel( channel_number, new_v_value ); }
        catch( const std::string& emsg ) { return getErrorMessage( ERROR_SET_PROBLEM_SET, emsg ); };
        
        return getSuccessMessage();
    }

    /** @brief getPulse - В данным сценарии не использовать
     * */
    std::string getPulse() override { return getErrorMessage(); }

public:
    TypeVCommands( oscilloscopes::Oscilloscope *osc, const std::string& prefix,
                   const std::string& command ) : TypeCommands( osc, prefix, command, 2 ) {}

    ~TypeVCommands() { _param.clear(); }

    /** @brief call - Вызов соответствующей команды
     *  @param command - команда
     *  @param content - контент
     *  @param i       - номер после =
     * */
    std::string call( const std::string& content, const size_t& i ) override
    {
        try  // Это на случай, если что-то ужасное случится
        {
            parseContent( content, i, 3 );
            if( ( ( _param[0] == "_" ) && ( _param[1] == "_" ) ) )
                return getRange();
            if( _param[1] == "?" )
                return getCurrent();
            return setValue();
        }
        catch( const std::string& emsg )
        {
            return getErrorMessage( "VX_EXTRA", emsg );
        };
        return getErrorUnknownMessage();
    }
};

#endif  // PROJECT_TYPE_V_COMMANDS_H_
