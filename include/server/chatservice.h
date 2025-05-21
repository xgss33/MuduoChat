#ifndef MUDUOCHAT_INCLUDE_SERVER_CHATSERVICE_H
#define MUDUOCHAT_INCLUDE_SERVER_CHATSERVICE_H

#include "server/usermodel.h"

#include <unordered_map>
#include <functional>
#include <mutex>

#include <muduo/net/TcpConnection.h>
#include <nlohmann/json.hpp>
using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

using MsgHandler = std::function<void(const TcpConnectionPtr& conn, json& js, Timestamp time)>;

// TODO: 单例模式改进
class ChatService
{
public:
    static ChatService* instance();
    
    void login(const TcpConnectionPtr& conn, json& js, Timestamp time);
    void reg(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // TODP: 实现异步消息发送队列
    //       oneChat只负责往消息队列添加消息,然后等待唤醒线程
    //       2. 拿出conn时处于online, io时候变成了offline
    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time);
    // TODO: 实现反向映射 unordered_map<TcpConnectionPtr, int>
    //       TcpConnectionPtr要作为key需要自定义哈希函数
    void clientCloseException(const TcpConnectionPtr& conn);
    
    MsgHandler getHandler(int msgid);

private:
    ChatService();
    std::unordered_map<int, MsgHandler> msgHandlerMap_;
    std::unordered_map<int, TcpConnectionPtr> userConnMap_;
    std::mutex mtx_;
    UserModel userModel_;
};

#endif // MUDUOCHAT_INCLUDE_SERVER_CHATSERVICE_H