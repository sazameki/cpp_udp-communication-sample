#include <cstdio>
#include <exception>
#include <string>

#include "UDPServer.hpp"
#include "ipaddress_list.hpp"
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

        DebugLog("Local IP Address:");
        auto addrMap = GetLocalIpAddresses();
        for (auto keyValue : addrMap) {
            auto key = keyValue.first;
            auto value = keyValue.second;
            DebugLog("  - %s (%s)", value.c_str(), key.c_str());
        }

        DebugLog("Waiting for UDP data on port %d", port);
    
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


