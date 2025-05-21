#ifndef MUDUOCHAT_INCLUDE_CHATSERVER_CHATSERVER_H
#define MUDUOCHAT_INCLUDE_CHATSERVER_CHATSERVER_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    ChatServer(EventLoop* loop, 
               const InetAddress& listenAddr, 
               const string& nameArg);
    void start();

private:
    void onConnection(const TcpConnectionPtr&);
    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

    TcpServer server_;
    EventLoop* loop_;
};

#endif // MUDUOCHAT_INCLUDE_CHATSERVER_CHATSERVER_H