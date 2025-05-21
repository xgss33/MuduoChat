#ifndef MUDUO_INCLUDE_SERVER_OFFLINEMESSAGEMODEL_H
#define MUDUO_INCLUDE_SERVER_OFFLINEMESSAGEMODEL_H

#include <string>
#include <vector>

/// @brief 提供离线消息的存储, 查询和删除操作
class OfflineMessageModel
{
public:
    /// @brief 存储离线消息
    /// @param userid 接收方id
    /// @param message 消息内容
    void insert(int userid, const std::string& message);
    
    /// @brief 查询并获取指定用户的所有离线消息
    /// @param userid 指定用户id
    /// @return 返回离线消息列表
    std::vector<std::string> query(int userid);

    /// @brief 删除指定用户的所有离线消息
    /// @param userid 指定用户id
    void remove(int userid);
};

#endif // MUDUO_INCLUDE_SERVER_OFFLINEMESSAGEMODEL_H
