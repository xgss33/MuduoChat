#include "server/offlinemessagemodel.h"
#include "server/dbutils.h"
#include "server/db.h"

#include <string>

void OfflineMessageModel::insert(int userid, const std::string& message)
{
    MySQL mysql;
    if(!mysql.connect()) return;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return;

    std::string sql = "INSERT INTO offlinemessage(userid, message) VALUES (?, ?)";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return;
    }
    MYSQL_BIND bind[2]{};
    unsigned long messageLength{};
    dbutils::bindInputInt(bind[0], userid);
    dbutils::bindInputString(bind[1], message, messageLength);

    if(mysql_stmt_bind_param(stmt, bind) != 0)
    {
        mysql_stmt_close(stmt);
        return;
    }
    if(mysql_stmt_execute(stmt) != 0)
    {
        mysql_stmt_close(stmt);
        return;
    }
    mysql_stmt_close(stmt);
    return;
}

std::vector<std::string> OfflineMessageModel::query(int userid)
{
    std::vector<std::string> messages;

    MySQL mysql;
    if (!mysql.connect()) return messages;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if (!stmt) return messages;

    std::string sql = "SELECT message FROM offlinemessage WHERE userid = ?";
    if (mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return messages;
    }

    MYSQL_BIND bind{};
    dbutils::bindInputInt(bind, userid);

    if (mysql_stmt_bind_param(stmt, &bind) != 0)
    {
        mysql_stmt_close(stmt);
        return messages;
    }

    if (mysql_stmt_execute(stmt) != 0)
    {
        mysql_stmt_close(stmt);
        return messages;
    }

    const unsigned long buffersize = 1024;
    char messagebuffer[buffersize]{};
    unsigned long messagelength = 0;

    MYSQL_BIND bindResult{};
    dbutils::bindOutputString(bindResult, messagebuffer, buffersize, messagelength);

    if (mysql_stmt_bind_result(stmt, &bindResult) != 0)
    {
        mysql_stmt_close(stmt);
        return messages;
    }

    // 获取结果集
    if (mysql_stmt_store_result(stmt) != 0)
    {
        // TODO: 储存结果失败日志
        mysql_stmt_close(stmt);
        return messages;
    }

    // 逐行取出结果
    while (mysql_stmt_fetch(stmt) == 0)
    {
        // messagelength表示实际长度，注意字符串截断处理
        messages.emplace_back(std::string(messagebuffer, messagelength));
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);
    return messages;
}

void OfflineMessageModel::remove(int userid)
{
    MySQL mysql;
    if(!mysql.connect()) return;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return;

    std::string sql = "DELETE FROM offlinemessage WHERE userid = ?";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return;
    }
    MYSQL_BIND bind{};
    dbutils::bindInputInt(bind, userid);

    if(mysql_stmt_bind_param(stmt, &bind) != 0)
    {
        mysql_stmt_close(stmt);
        return;
    }
    if(mysql_stmt_execute(stmt) != 0)
    {
        mysql_stmt_close(stmt);
        return;
    }
    mysql_stmt_close(stmt);
    return;
}
