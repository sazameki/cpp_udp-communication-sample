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
    int ret = 0;

    try {
        UDPClient client("127.0.0.1", 22222);
       
        for (int i = 0; i < 10; i++) {
            // 送信
            client.Send("Hello");
            
            // 受信
            std::string ret = client.ReceiveString();
            DebugLog("[Reply] \"%s\"", ret.c_str());
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

