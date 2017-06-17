#include <cstdio>
#include <exception>
#include <string>

#include "UDPServer.hpp"
#include "Debug.hpp"


int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif
    int ret = 0;

    try {
        int port = 22222;
        UDPServer server(port);
    
        printf("Waiting for UDP data on port %d\n", port);
    
        int count = 0;
        while (true) {
            // 受信
            ClientInfo clientInfo;
            std::string str = server.ReceiveString(clientInfo);
            //std::string str = server.ReceiveString();   //!< 返信やクライアント情報不要の場合は引数不要
            DebugLog("[Received-%d] \"%s\" (%s:%d)", count++, str.c_str(), clientInfo.hostname.c_str(), clientInfo.port);
        
            // 送信
            server.Send(clientInfo, "OK");
        }
    } catch (std::exception& e) {
        DebugLog("Error: %s", e.what());
        ret = -1;
    }

#ifdef _WIN32
    WSACleanup();
#endif

    return ret;
}


