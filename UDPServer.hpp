//
//  UDPServer.hpp
//
//  Created by numata on 2017/06/08.
//  Copyright © 2017 Satoshi Numata. All rights reserved.
//

#ifndef UDPServer_hpp
#define UDPServer_hpp

#define UDP_SERVER_BUFFER_SIZE     1024

#include <string>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#else
#include <netinet/in.h>
#endif


struct ClientInfo
{
    sockaddr_in from;
    std::string hostname;
    int         port;
};

class UDPServer
{
    int     port;
    int     sock;
    bool    isClosed;
    char    buffer[UDP_SERVER_BUFFER_SIZE];

public:
    UDPServer(int port);
    ~UDPServer();

public:
    void        Close();
    bool        IsClosed() const;
    ssize_t     Receive(char *inBuffer, size_t bufferSize);
    ssize_t     Receive(char *inBuffer, size_t bufferSize, ClientInfo& outClientInfo);
    std::string ReceiveString();
    std::string ReceiveString(ClientInfo& outClientInfo);
    ssize_t     Send(ClientInfo& clientInfo, const char *buffer, size_t length);
    ssize_t     Send(ClientInfo& clientInfo, const std::string& message);
    
};

#endif /* UDPServer_hpp */

