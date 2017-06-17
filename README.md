# unix_udp-comm-sample_cpp

C++でUDP/IPの通信を行うサーバ実装とクライアント実装のサンプルプログラムです。

## 開発環境
以下の環境で動作を確認しています。

- macOS 10.12上のXcode 8
- Windows 7 (SP1)上のVisual Studio 2017

## クライアントについて
以下の2つのファイルを追加すれば使用できます。

- UDPClient.hpp
- UDPClient.cpp

```
UDPClient *client = new UDPClient("127.0.0.1", 22222);

// データの送信
client->Send("Hello");
            
// データの受信
std::string reply = client->ReceiveString();
...
delete client;
```

※ Windows環境では、アプリケーションの起動時と終了時に、WinSockの初期化とクリーンアップのための関数呼び出しが必要になることに注意してください。また、[プロジェクト]-[<プロジェクト名>のプロパティ]から、[構成プロパティ]-[C/C++]-[詳細設定]の「必ず使用されるインクルード ファイル」に「stdafx.h」を指定する必要があります。

## サーバについて
以下の2つのファイルを追加すれば使用できます。

- UDPServer.hpp
- UDPServer.cpp

```
UDPServer server = new UDPServer(22222);

bool isRunning = true;
while (isRunning) {
    // データの受信
    ClientInfo clientInfo;
    std::string str = server->ReceiveString(clientInfo);
        
    // データの送信
    server->Send(clientInfo, "OK");  // 受信時に取得したClientInfo構造体を使ってクライアントに返信
}
...
// クライアント情報不要の場合は引数なしのReceiveString()関数を使用
std::string str = server->ReceiveString();
...
delete server;
```

※ Windows環境では、アプリケーションの起動時と終了時に、WinSockの初期化とクリーンアップのための関数呼び出しが必要になることに注意してください。また、[プロジェクト]-[<プロジェクト名>のプロパティ]から、[構成プロパティ]-[C/C++]-[詳細設定]の「必ず使用されるインクルード ファイル」に「stdafx.h」を指定する必要があります。
