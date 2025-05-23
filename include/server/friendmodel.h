#ifndef MUDUOO_INCLUDE_SERVER_FRIENDMODEL_H
#define MUDUOO_INCLUDE_SERVER_FRIENDMODEL_H

#include <vector>

class User;

class FriendModel
{
public:
    bool insert(int userid, int friendid);

    // 用户好友列表等信息一般只保存在本地, 如果全都要服务器拉取, 会很大压力
    // 鉴于小项目, 采用服务器数据库
    std::vector<User> query(int userid);
};

#endif // MUDUOO_INCLUDE_SERVER_FRIENDMODEL_H