//
//  UDPClient.cpp
//
//  Created by numata on 2017/06/08.
//  Copyright © 2017 Satoshi Numata. All rights reserved.
//

#include "UDPClient.hpp"

#include <stdexcept>

#ifdef _WIN32
#pragma comment( lib, "ws2_32.lib" )
#else
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#endif


/*!
     コンストラクタ
     指定されたホスト名とポート番号のソケットにデータを送信するクライアントを作成します。
     @param hostname_   ホスト名
     @param port_       ポート番号
 */
UDPClient::UDPClient(const std::string& hostname_, int port_)
{
    hostname = hostname_;
    port = port_;
    isClosed = true;
    
    // ソケットの生成
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Failed to create a UDP socket for sending.");
    }
    
    // ホスト名の解決
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    in_addr addr;
    addr.s_addr = inet_addr(hostname_.c_str());
    if (addr.s_addr == -1) {
        hostent *host = gethostbyname(hostname.c_str());
        if (!host) {
            throw std::runtime_error("Failed to get a host entry.");
        }
        memcpy(&addr, (in_addr *)*host->h_addr_list, sizeof(in_addr));
    }
    hostname = std::string(inet_ntoa(addr));
    serverAddr.sin_addr = addr;
    
    isClosed = false;
}

/*!
     デストラクタ
 */
UDPClient::~UDPClient()
{
    Close();
}

/*!
     接続先のホスト名（IPアドレス）を取得します。
     @return    接続先のホスト名
 */
std::string UDPClient::GetRemoteHost() const
{
    return hostname;
}

/*!
     接続先のポート番号を取得します。
     @return    接続先のポート番号
 */
int UDPClient::GetRemotePort() const
{
    return port;
}

/*!
     ソケットをクローズします。
 */
void UDPClient::Close()
{
    if (isClosed) {
        return;
    }
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
    sock = -1;
}

/*!
     ソケットからデータを受信します。
     TCP/IPと異なり、受信時にエラーが発生しても自動的にソケットをクローズすることはありません。
     @param buffer  受信用のデータバッファ
     @param length  データバッファのサイズ
     @return 受信したデータのバイト数。ソケットがクローズされていた場合には0、エラーの場合は-1。
 */
ssize_t UDPClient::Receive(char *buffer, size_t length)
{
    sockaddr_in fromAddr;
    socklen_t fromAddrLen = sizeof(fromAddr);
    return recvfrom(sock, buffer, length, 0, (sockaddr *)&fromAddr, &fromAddrLen);
}

/*!
     ソケットから文字列データを受信します。
     TCP/IPと異なり、受信時にエラーが発生しても自動的にソケットをクローズすることはありません。
     @return 受信した文字列データ
 */
std::string UDPClient::ReceiveString()
{
    ssize_t readSize = Receive(buffer, UDP_CLIENT_BUFFER_SIZE);
    buffer[readSize] = '\0';
    std::string ret = buffer;
    return ret;
}

/*!
     データを送信します。
     @param buffer  データのバッファ
     @param length  バッファのサイズ
     @return    送信したデータのバイト数
 */
ssize_t UDPClient::Send(const char *buffer, size_t length)
{
    if (isClosed) {
        throw std::runtime_error("UDPSender::Send() was called after the socket was closed.");
    }
    return sendto(sock, buffer, length, 0,
                  (sockaddr *)&serverAddr, sizeof(serverAddr));
}

/*!
     文字列データを送信します。
     @param message 送信する文字列データ
     @return    送信したデータのバイト数
 */
ssize_t UDPClient::Send(const std::string& message)
{
    return Send(message.c_str(), message.length());
}

