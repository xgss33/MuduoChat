#include "server/chatservice.h"
#include "public.h"

#include <vector>
#include <string>

#include <muduo/base/Logging.h>
using namespace muduo;

ChatService* ChatService::instance()
{
    static ChatService service;
    return &service;
}

void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    int id = js["id"];
    std::string password = js["password"];
    std::unique_ptr<User> user = userModel_.query(id);

    json response;
    response["msgid"] = LOG_MSG_ACK;
    if(user != nullptr)
    {
        // db查询到了id
        if(user->getState() == "online")
        {
            // 用户已经登录, 不允许重复登录
            response["error"] = 1;
            response["errormsg"] = "用户已登录";
        }
        else
        {
            if(user->getPassword() == password)
            {
                {
                std::lock_guard<std::mutex> lock(mtx_);
                userConnMap_.insert({id,conn});
                }
                std::vector<std::string> offmessage = offlinemsgMd_.query(id);
                if(!offmessage.empty())
                {
                    response["offlinemessage"] = offmessage;
                    offlinemsgMd_.remove(id);
                }
                std::vector<User> friendVec = friendModel_.query(id);

                std::vector<std::string> vec;
                for (const auto &f : friendVec)
                {
                    json js;
                    js["id"] = f.getId();
                    js["name"] = f.getName();
                    js["state"] = f.getState();

                    vec.push_back(js.dump());
                }
                response["friends"] = vec;

                user->setState("online");
                userModel_.updateState(*user);
                response["error"] = 0;
                response["id"] = user->getId();
                response["name"] = user->getName();
            }
            else
            {
                response["error"] = 1;
                response["errormsg"] = "密码错误";
            }
        }
    }
    else
    {
        // 未注册
        response["error"] = 1;
        response["errormsg"] = "用户不存在";
    }
    conn->send(response.dump());
};

void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    std::string name = js["name"];
    std::string password = js["password"];

    User user;
    user.setName(name);
    user.setPassword(password);
    bool state = userModel_.insert(user);
    json response;
    response["msgid"] = REG_MES_ACK;
    if(state)
    {   
        response["error"] = 0;
        response["id"] = user.getId();
    }
    else
    {
        response["error"] = 1;
    }
    conn->send(response.dump());
}

MsgHandler ChatService::getHandler(int msgid)
{
    auto it = msgHandlerMap_.find(msgid);
    if(it == msgHandlerMap_.end())
    {
        // 返回lambda表达式, 打印错误日志
        return [=](const TcpConnectionPtr&, json&, Timestamp)
        {
            LOG_ERROR << "msgid:" << msgid << " can not find handler";
        };
    }
    else
    {
        return msgHandlerMap_[msgid];
    }
}

void ChatService::clientCloseException(const TcpConnectionPtr& conn)
{
    User user;
    {
        std::lock_guard<std::mutex> lock(mtx_);
        for(auto it = userConnMap_.begin(); it != userConnMap_.end(); ++it)
        {
            if(it->second == conn)
            {
                user.setId(it->first);
                userConnMap_.erase(it);
                break;
            }
        }
    }
    if(user.getId() != -1)
    {
        user.setState("offline");
        userModel_.updateState(user);    
    }
}

void ChatService::oneChat(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    int toID = js["to"];

    {
        std::lock_guard<std::mutex> lock(mtx_);
        auto it = userConnMap_.find(toID);
        if(it != userConnMap_.end())
        {
            it->second->send(js.dump());
            return;
        }
    }

    // 存储离线消息
    offlinemsgMd_.insert(toID, js.dump());
}


ChatService::ChatService()
{
    msgHandlerMap_.insert({LOG_MSG, std::bind(&ChatService::login, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({REG_MES, std::bind(&ChatService::reg, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
    msgHandlerMap_.insert({ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)});
}

void ChatService::reset()
{
    userModel_.resetState();
}

void ChatService::addFriend(const TcpConnectionPtr& conn, json& js, Timestamp time)
{
    int userid = js["id"];
    int friendid = js["friendid"];
    friendModel_.insert(userid, friendid);
}