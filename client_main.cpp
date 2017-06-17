#include <cstdio>
#include <exception>

#include "UDPClient.hpp"
#include "Debug.hpp"


int main()
{
#ifdef _WIN32
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

    try {
        UDPClient client("10.211.55.2", 2222);
        
        for (int i = 0; i < 10; i++) {
            // 送信
            client.Send("Hello");
            
            // 受信
            std::string ret = client.ReceiveString();
            DebugLog("[Reply] \"%s\"", ret.c_str());
        }
    } catch (std::exception& e) {
        DebugLog("Error: %s", e.what());
#ifdef _WIN32
        WSACleanup();
#endif
        return -1;
    }
    
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}

