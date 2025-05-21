#ifndef MUDUOCHAT_INCLUDE_SERVER_USER_H
#define MUDUOCHAT_INCLUDE_SERVER_USER_H

#include <string>

class User
{
public:
    User(int id=-1, const std::string& name="", const std::string& pwd="", const std::string& state="offline")
        : id_(id), name_(name), password_(pwd), state_(state) {}
    
    void setId(int id) { id_ = id; }
    void setName(const std::string& name) { name_ = name; }
    void setPassword(const std::string& pwd) { password_ = pwd; }
    void setState(const std::string& state) { state_ = state; }

    int getId() const { return id_; }
    const std::string& getName() const { return name_; }
    const std::string& getPassword() const { return password_; }
    const std::string& getState() const { return state_; }

private:
    int id_;
    std::string name_;
    std::string password_;
    std::string state_;
};

#endif // MUDUOCHAT_INCLUDE_SERVER_USER_H
