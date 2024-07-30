#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <chrono>
#include <exception>
#include <string>
#include <unordered_map>
#include <tuple>
#include <algorithm>
#include <iomanip>
#include <thread>
#include <atomic>
#include <sstream>
#include <type_traits>

#include "oscilloscope_plug.h"

#define SERVER_ADRESS std::string("192.168.1.65")
#define SERVER_PORT 8000

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

#include "TypeVCommands.h"
#include "TypeSCommands.h"
#include "TypePCommands.h"

#define PREFIX std::string("osc")

class CommandDecoder
{
private:
    TypeVCommands _tvc;
    TypeSCommands _tsc;
    TypePCommands _tpc;

    std::string makeCommandTrig( const std::string& str, const size_t& i )
    {
        return std::string("osc:NO_USE;");
    }

    std::string callCMD( const std::string& command, const std::string& content, const size_t& i )
    {
        if( command == "vx" )
            return _tvc.call( content, i );
        if( command== "sample" )
            return _tsc.call( content, i );
        if( command == "pulse" )
            return _tpc.call( content, i );
        return std::string( ( PREFIX + ":UNKHOWN_COMMAND;" ) );
    }

    std::pair<bool, size_t> findPrefix( const std::string& content )
    {
        const std::string prefix = PREFIX;
        for( size_t i = 0; i <= prefix.size(); ++i )
            if( content.substr( 0, i ) == prefix )
                return std::pair<bool, size_t>( false, ( i + 1 ) );
        return std::pair<bool, size_t>( true, 0 );
    }

public:
    CommandDecoder( oscilloscopes::Oscilloscope *osc ) : _tvc( osc, PREFIX, "vx" ),
        _tsc( osc, PREFIX, "scale" ), _tpc( osc, PREFIX, "pulse" ) {}

    ~CommandDecoder() = default;

    std::string decode( const std::string& content )
    {
        auto it = findPrefix(content);
        if( it.first )
            return std::string( ( PREFIX + ":NO_PREFIX;" ) );
        std::string command = "";
        for( size_t i = it.second; i < content.size(); ++i )        
        {
            if( content[i] == '=' )
                return callCMD( command, content, ( i + 1 ) );
            command += content[i];
        }
        return std::string( ( PREFIX + ":NO_OSC;" ) );
    }
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

    void socketInit()
    {
        listener = socket( AF_INET, SOCK_STREAM, 0 );
        if(listener < 0)
            throw ServerException( ( "listener = " + std::to_string(listener) ) );
    }
    
    void fillAddress( const std::string& IP, const int& PORT )
    {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(PORT);
        addr.sin_addr.s_addr = inet_addr(IP.c_str());
    }

    void bindInit()
    {
        int b = bind( listener, ( struct sockaddr* )&addr, sizeof(addr) );
        if( b < 0 )
            throw ServerException( ( "bind = " + std::to_string(b) ) );
        listen( listener, 1 );
    }

    void sockAccept( int& sock )
    {
        sock = accept( listener, NULL, NULL );
        if( sock < 0 )
            throw ServerException( "Sock no accept" );
    }


    std::string readFromRecv()
    {
        std::string receivedData;
        char buffer[1024];
        while(true)
        {
            int bytesReceived = recv( sock, buffer, 1024, 0 );
            if (bytesReceived <= 0)
                break;
            receivedData.append(buffer, bytesReceived);
            if( receivedData.back() == ';' )
                break;
        }
        return receivedData;
    }

    void sendToSock( const std::string& msg )
    {
        std::cout << "Start send" << std::endl;
        const char* dataPtr = msg.c_str();
        size_t dataSize = msg.length();
        size_t totalSent = 0;
        while( totalSent < dataSize )
        {
            int bytesSent = send( sock, ( dataPtr + totalSent ), ( dataSize - totalSent ), 0 );
            std::cout << bytesSent << std::endl;
            if( bytesSent == -1 )
                break;
            totalSent += bytesSent;
        }
    }

    bool process()
    {
        std::string content = readFromRecv();
        if( content.empty() )
            return false;
        sendToSock( _cmdec->decode(content) );
        return true;
    }

public:
    Server( const std::string& IP, const int& PORT, CommandDecoder* cmdec ) : _cmdec(cmdec)
    {
        socketInit();
        fillAddress( IP, PORT );
        bindInit();
        std::cout << "server in system address: " << IP << ":" << PORT << std::endl;
    }

    int run()
    {
        for( ;; close(sock) )
        {
            sockAccept(sock);
            while( process() ) {;}
        }
        return 0;
    }
};

int main()
{
    oscilloscopes::plug::OscPlug op;
    CommandDecoder cmdec(&op);
    Server server( SERVER_ADRESS, SERVER_PORT, &cmdec );
    return server.run();
}


