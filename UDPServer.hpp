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
#include <netinet/in.h>

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
    ssize_t     Send(ClientInfo& clientInfo, const void *buffer, size_t length);
    ssize_t     Send(ClientInfo& clientInfo, const std::string& message);
    
};

#endif /* UDPServer_hpp */

