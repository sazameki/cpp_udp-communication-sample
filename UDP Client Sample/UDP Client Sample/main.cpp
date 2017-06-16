#include <cstdio>
#include "UDPClient.hpp"


int main(int argc, char** argv)
{
    try {
        UDPClient client("127.0.0.1", 2222);
        
        for (int i = 0; i < 10; i++) {
            // 送信
            client.Send("Hello");
            
            // 受信
            std::string ret = client.ReceiveString();
            printf("Reply: [%s]\n", ret.c_str());
        }
    } catch (std::exception& e) {
        printf("Error: %s\n", e.what());
    }
    
    return 0;
}

