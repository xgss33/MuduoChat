#include "server/chatserver.h"

int main()
{
    EventLoop loop;
    InetAddress listenAddr("127.0.0.1", 12345);
    ChatServer server(&loop, listenAddr, "ChatServer");
    server.start();
    loop.loop();

    return 0;
}