#include <cstdio>
#include <string>

#include "UDPServer.hpp"


int main()
{
    int port = 2222;
    UDPServer server(port);
    
    printf("Waiting for UDP data on port %d\n", port);
    
    int count = 0;
    while (true) {
        // 受信
        ClientInfo clientInfo;
        std::string str = server.ReceiveString(clientInfo);
        //std::string str = server.ReceiveString();   //!< 返信やクライアント情報不要の場合は引数不要
        printf("Received-%d: [%s] (%s:%d)\n", count++, str.c_str(), clientInfo.hostname.c_str(), clientInfo.port);
        
        // 送信
        server.Send(clientInfo, "OK");
    }
    
    return 0;
}


