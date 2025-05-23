#include "server/chatserver.h"
#include "server/chatservice.h"

#include <csignal>

EventLoop* g_loop = nullptr;

void resetHandler(int)
{
    ChatService::instance()->reset();
    g_loop->quit();
}

int main()
{
    EventLoop loop;
    g_loop = &loop;

    std::signal(SIGINT, resetHandler);

    InetAddress listenAddr("127.0.0.1", 12345);
    ChatServer server(&loop, listenAddr, "ChatServer");
    server.start();
    loop.loop();

    return 0;
}   