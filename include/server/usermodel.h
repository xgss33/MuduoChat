#ifndef MUDUOCHAT_INCLUDE_SERVER_USERMODEL_H
#define MUDUOCHAT_INCLUDE_SERVER_USERMODEL_H

#include "server/user.h"

#include <memory>

class UserModel
{
public:
    bool insert(User& user);
    std::unique_ptr<User> query(int id);
    bool updateState(User& user);
    bool resetState();
};

#endif // MUDUOCHAT_INCLUDE_SERVER_USERMODEL_H