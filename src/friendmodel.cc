#include "server/user.h"
#include "server/friendmodel.h"
#include "server/dbutils.h"
#include "server/db.h"

#include <vector>

bool FriendModel::insert(int userid, int friendid)
{
    MySQL mysql;
    if(!mysql.connect()) return false;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return false;

    std::string sql = "INSERT INTO friend(userid, friendid) VALUES (?, ?), (?, ?)";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return false;
    }
    MYSQL_BIND bind[4]{};
    dbutils::bindInputInt(bind[0], userid);
    dbutils::bindInputInt(bind[1], friendid);
    dbutils::bindInputInt(bind[2], friendid);
    dbutils::bindInputInt(bind[3], userid);

    if(mysql_stmt_bind_param(stmt, bind) != 0)
    {
        mysql_stmt_close(stmt);
        return false;
    }
    if(mysql_stmt_execute(stmt) != 0)
    {
        mysql_stmt_close(stmt);
        return false;
    }
    mysql_stmt_close(stmt);
    return true;
}

std::vector<User> FriendModel::query(int userid)
{
    std::vector<User> vec;

    MySQL mysql;
    if (!mysql.connect()) return vec;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if (!stmt) return vec;

    std::string sql = 
    "SELECT u.id, u.name, u.state FROM friend f "
    "JOIN user u ON f.friendid = u.id "
    "WHERE f.userid = ?";

    if (mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return vec;
    }

    MYSQL_BIND bind{};
    dbutils::bindInputInt(bind, userid);

    if (mysql_stmt_bind_param(stmt, &bind) != 0)
    {
        mysql_stmt_close(stmt);
        return vec;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        mysql_stmt_close(stmt);
        return vec;
    }

    int friendid{};
    char name[50]{};
    unsigned long nameLength = 0;
    char state[50]{};
    unsigned long stateLength = 0;
    MYSQL_BIND bindResult[3]{};
    dbutils::bindOutputInt(bindResult[0], friendid);
    dbutils::bindOutputString(bindResult[1], name, 50, nameLength);
    dbutils::bindOutputString(bindResult[2], state, 50, stateLength);

    if (mysql_stmt_bind_result(stmt, bindResult) != 0)
    {
        mysql_stmt_close(stmt);
        return vec;
    }

    // 获取结果集
    if (mysql_stmt_store_result(stmt) != 0)
    {
        // TODO: 储存结果失败日志
        mysql_stmt_close(stmt);
        return vec;
    }

    // 逐行取出结果
    while (mysql_stmt_fetch(stmt) == 0)
    {
        // messagelength表示实际长度，注意字符串截断处理
        vec.emplace_back(User(friendid, std::string(name, nameLength), "", state));
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);
    return vec;
}