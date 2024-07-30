#include "Server.h"

void server::Server::socketInit()
{
    listener = socket( AF_INET, SOCK_STREAM, 0 );
    if( listener < 0 )
        throw server::ServerException( ( "listener = " + std::to_string(listener) ) );
}

void server::Server::fillAddress( const std::string& IP, const int& PORT )
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = inet_addr(IP.c_str());
}

void server::Server::bindInit()
{
    int b = bind( listener, ( struct sockaddr* )&addr, sizeof(addr) );
    if( b < 0 )
        throw server::ServerException( ( "bind = " + std::to_string(b) ) );
    listen( listener, 1 );
}

void server::Server::sockAccept( int& sock )
{
    sock = accept( listener, NULL, NULL );
    if( sock < 0 )
        throw server::ServerException( "Sock no accept" );
}

std::string server::Server::readFromRecv()
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

void server::Server::sendToSock( const std::string& msg )
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

bool server::Server::process()
{
    std::string content = readFromRecv();
    if( content.empty() )
        return false;
    sendToSock( _cmdec->decode(content) );
    return true;
}

server::Server::Server( const std::string& IP, const int& PORT, CommandDecoder* cmdec ) : _cmdec(cmdec)
{
    socketInit();
    fillAddress( IP, PORT );
    bindInit();
    std::cout << "server in system address: " << IP << ":" << PORT << std::endl;
}

int server::Server::run()
{
    for( ;; close(sock) )
    {
        sockAccept(sock);
        while( process() ) {;}
    }
    return 0;
}


