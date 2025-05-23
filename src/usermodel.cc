#include "server/usermodel.h"
#include "server/db.h"
#include "server/dbutils.h"

#include <string>

// TODO: log
bool UserModel::insert(User& user)
{
    MySQL mysql;
    if(!mysql.connect()) return false;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return false;

    std::string sql = "INSERT INTO user(name, password, state) VALUES (?, ?, ?)";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return false;
    }
    MYSQL_BIND bind[3]{};
    std::string name = user.getName();
    std::string password = user.getPassword();
    std::string state = user.getState();
    unsigned long nameLength{}, passwordLength{}, stateLength{};
    dbutils::bindInputString(bind[0], name, nameLength);
    dbutils::bindInputString(bind[1], password, passwordLength);
    dbutils::bindInputString(bind[2], state, stateLength);

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
    user.setId(mysql_stmt_insert_id(stmt));
    mysql_stmt_close(stmt);
    return true;
}

std::unique_ptr<User> UserModel::query(int id)
{
    MySQL mysql;
    if(!mysql.connect()) return nullptr;

    MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return nullptr;

    std::string sql = "SELECT id, name, password, state FROM user WHERE id = ?";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    MYSQL_BIND bindParam{};
    dbutils::bindInputInt(bindParam, id);
    if(mysql_stmt_bind_param(stmt, &bindParam) != 0)
    {
        mysql_stmt_close(stmt);
        return nullptr;
    }
    if(mysql_stmt_execute(stmt)!=0)
    {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    MYSQL_BIND bindResult[4]{};
    int outputId{};
    char name[64]{};
    unsigned long nameLength{};
    char password[64]{};
    unsigned long passwordLength{};
    char state[32]{};
    unsigned long stateLength{};
    dbutils::bindOutputInt(bindResult[0], outputId);
    dbutils::bindOutputString(bindResult[1], name, 64, nameLength);
    dbutils::bindOutputString(bindResult[2], password, 64, passwordLength);
    dbutils::bindOutputString(bindResult[3], state, 32, stateLength);

    if (mysql_stmt_bind_result(stmt, bindResult) != 0) {
        mysql_stmt_close(stmt);
        return nullptr;
    }

    if (mysql_stmt_store_result(stmt) != 0) {
        mysql_stmt_free_result(stmt);
        mysql_stmt_close(stmt);
        return nullptr;
    }

    if (mysql_stmt_fetch(stmt) == 0) {
        auto user = std::make_unique<User>();
        user->setId(outputId);
        user->setName(std::string(name, nameLength));
        user->setPassword(std::string(password, passwordLength));
        user->setState(std::string(state, stateLength));

        mysql_stmt_free_result(stmt);
        mysql_stmt_close(stmt);
        return user;
    }

    mysql_stmt_free_result(stmt);
    mysql_stmt_close(stmt);
    return nullptr;
}

bool UserModel::updateState(User& user)
{
    MySQL mysql;
    if(!mysql.connect()) return false;

      MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return false;

    std::string sql = "UPDATE user SET state = ? WHERE id = ?";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
    {
        mysql_stmt_close(stmt);
        return false;
    }

    MYSQL_BIND bind[2]{};
    std::string state = user.getState();
    unsigned long stateLength{};
    dbutils::bindInputString(bind[0], state, stateLength);
    int id = user.getId();
    dbutils::bindInputInt(bind[1], id);

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

bool UserModel::resetState()
{
    MySQL mysql;
    if(!mysql.connect()) return false;

      MYSQL_STMT* stmt = mysql_stmt_init(mysql.getConnection());
    if(!stmt) return false;

    std::string sql = "UPDATE user SET state = 'offline' WHERE state = 'online'";
    if(mysql_stmt_prepare(stmt, sql.c_str(), static_cast<unsigned long>(sql.length())) != 0)
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