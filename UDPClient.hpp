//
//  UDPClient.hpp
//
//  Created by numata on 2017/06/08.
//  Copyright © 2017 Satoshi Numata. All rights reserved.
//

#ifndef UDPClient_hpp
#define UDPClient_hpp

#include <string>
#include <netinet/in.h>


#define UDP_CLIENT_BUFFER_SIZE     1024


class UDPClient
{
    std::string hostname;
    int         port;
    int         sock;
    bool        isClosed;
    sockaddr_in serverAddr;
    char        buffer[UDP_CLIENT_BUFFER_SIZE];
    
public:
    UDPClient(const std::string& host, int port);
    ~UDPClient();

public:
    std::string GetRemoteHost() const;
    int         GetRemotePort() const;
    void        Close();
    ssize_t     Receive(char *buffer, size_t length);
    std::string ReceiveString();
    ssize_t     Send(const void *buffer, size_t length);
    ssize_t     Send(const std::string& message);

};

#endif /* UDPClient_hpp */

