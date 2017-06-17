//
//  UDPServer.cpp
//
//  Created by numata on 2017/06/08.
//  Copyright © 2017 Satoshi Numata. All rights reserved.
//

#include "UDPServer.hpp"

#include <unistd.h>
#include <stdexcept>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


/*!
     コンストラクタ
     @port_     UDPサーバのポート番号
 */
UDPServer::UDPServer(int port_)
{
    port = port_;
    sock = 0;
    isClosed = true;
    
    // ソケットの生成
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        throw std::runtime_error("Failed to create a UDP socket.");
    }
    
    // ポート再利用の設定
    int reuseFlag = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuseFlag, (socklen_t)sizeof(reuseFlag)) == -1) {
        Close();
        throw std::runtime_error("Failed to set the reuse option to a server socket.");
    }
    
    // アドレスのバインド
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(sock, (sockaddr *)&addr, sizeof(addr)) == -1) {
        throw std::runtime_error("Failed to bind a UDP socket.");
    }

    isClosed = false;
}

/*!
     デストラクタ
 */
UDPServer::~UDPServer()
{
    Close();
}

/*!
     ソケットをクローズします。
 */
void UDPServer::Close()
{
    if (isClosed) {
        return;
    }
    close(sock);
    sock = 0;
    isClosed = true;
}

/*!
     ソケットが閉じられているかどうかをリターンします。
     @return    ソケットが閉じられていればtrue、そうでなければfalse
 */
bool UDPServer::IsClosed() const
{
    return isClosed;
}

/*!
     ソケットからデータを受信します。
     TCP/IPと異なり、受信時にエラーが発生しても自動的にソケットをクローズすることはありません。
     @param inBuffer    受信用のデータバッファ
     @param bufferSize  データバッファのサイズ
     @return 受信したデータのバイト数。ソケットがクローズされていた場合には0、エラーの場合は-1。
 */
ssize_t UDPServer::Receive(char *inBuffer, size_t bufferSize)
{
    if (isClosed) {
        throw std::runtime_error("UDPSender::Receive() was called after socket was closed.");
    }
    sockaddr_in fromAddr;
    socklen_t sinSize = sizeof(fromAddr);
    return recvfrom(sock, inBuffer, bufferSize, 0, (sockaddr *)&fromAddr, &sinSize);
}

/*!
     ソケットからデータを受信します。
     TCP/IPと異なり、受信時にエラーが発生しても自動的にソケットをクローズすることはありません。
     データ送信元の情報をClientInfo構造体に格納します。
     @param inBuffer        受信用のデータバッファ
     @param bufferSize      データバッファのサイズ
     @param outClientInfo   データ送信元の情報を格納する構造体
     @return 受信したデータのバイト数。ソケットがクローズされていた場合には0、エラーの場合は-1。
 */
ssize_t UDPServer::Receive(char *inBuffer, size_t bufferSize, ClientInfo& outClientInfo)
{
    if (isClosed) {
        throw std::runtime_error("UDPSender::Receive() was called after socket was closed.");
    }
    socklen_t sinSize = sizeof(outClientInfo.from);
    size_t len = recvfrom(sock, inBuffer, bufferSize, 0, (sockaddr *)&outClientInfo.from, &sinSize);
    if (len > 0) {
        outClientInfo.hostname = std::string(inet_ntoa(outClientInfo.from.sin_addr));
        outClientInfo.port = ntohs(outClientInfo.from.sin_port);
    }
    return len;
}

/*!
     ソケットから文字列データを受信します。
     @return 受信した文字列データ
 */
std::string UDPServer::ReceiveString()
{
    ssize_t readSize = Receive(buffer, UDP_SERVER_BUFFER_SIZE);
    buffer[readSize] = '\0';
    std::string ret(buffer);
    return ret;
}

/*!
     ソケットからデータを受信します。
     データ送信元の情報をClientInfo構造体に格納します。
     @param outClientInfo   データ送信元の情報を格納する構造体
     @return 受信した文字列データ
 */
std::string UDPServer::ReceiveString(ClientInfo& outClientInfo)
{
    ssize_t readSize = Receive(buffer, UDP_SERVER_BUFFER_SIZE, outClientInfo);
    buffer[readSize] = '\0';
    std::string ret(buffer);
    return ret;
}

/*!
     ClientInfo構造体で指定した送信先にデータを送信します。
     ClientInfo構造体は、Receive()またはReceiveString()メソッドの引数としてパケット受信時に取得できます。
     @param buffer  データのバッファ
     @param length  バッファのサイズ
     @return    送信したデータのバイト数
 */
ssize_t UDPServer::Send(ClientInfo& clientInfo, const void *buffer, size_t length)
{
    if (isClosed) {
        throw std::runtime_error("UDPSender::Send() was called after the socket was closed.");
    }
    return sendto(sock, buffer, length, 0,
                  (sockaddr *)&clientInfo.from, sizeof(clientInfo.from));
}

/*!
     ClientInfo構造体で指定した送信先に文字列データを送信します。
     ClientInfo構造体は、Receive()またはReceiveString()メソッドの引数としてパケット受信時に取得できます。
     @return    送信した文字列データのバイト数
 */
ssize_t UDPServer::Send(ClientInfo& clientInfo, const std::string& message)
{
    return Send(clientInfo, message.c_str(), message.length());
}

