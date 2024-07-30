#ifndef PROJECT_SERVER_H_
#define PROJECT_SERVER_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "CommandDecoder.h"

namespace server
{

    class MyException : public std::exception
    {
    public:
        explicit MyException( const std::string& msg ) : m_msg(msg) {}
        const char *what() const noexcept override
        {
            return m_msg.c_str();
        }

    private:
        std::string m_msg;
    };

    class ServerException : public MyException
    {
    public:
        ServerException( const std::string& m ) : MyException(m) {}
    };

    class Server
    {
    private:
        int sock;
        int listener;
        struct sockaddr_in addr;
        CommandDecoder *_cmdec;

        /** @brief socketInit - инициализация сокета
         * */
        void socketInit();

        /** @brief fillAddress - введение адреса
         * */
        void fillAddress( const std::string& IP, const int& PORT );

        /** @brief bindInit - инициализация bind
         * */
        void bindInit();

        /** @brief sockAccept - подверждение сокета
         * */
        void sockAccept( int& sock );

        /** @breif readFromRecv - считывание данных с сокета
         * */
        std::string readFromRecv();

        /** @brief sendToSock - отправить данные в сокет
         * */
        void sendToSock( const std::string& msg );

        /** @brief process процесс считывания/записи
         * */
        bool process();

    public:
        Server( const std::string& IP, const int& PORT, CommandDecoder* cmdec );
        
        /** @brief run - запуск сервера
         * */
        int run();

    };  // Server

};  // server

#endif  // PROJECT_SERVER_H_
