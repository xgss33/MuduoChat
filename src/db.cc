#include "server/db.h"

#include <muduo/base/Logging.h>

static std::string server = "127.0.0.1";
static std::string user = "root";
static std::string password = "123456";
static std::string dbname = "chat";

MySQL::MySQL()
{
    conn_ = mysql_init(nullptr);
}
MySQL::~MySQL()
{
    if(conn_ != nullptr)
    {
        mysql_close(conn_);
    }
}

bool MySQL::connect()
{
    MYSQL* p = mysql_real_connect(conn_, server.c_str(), user.c_str(),
                                    password.c_str(), dbname.c_str(), 3306, nullptr, 0);
    if(p != nullptr)
    {
        mysql_query(conn_, "set names gbk");
        LOG_INFO << "connect mysql success!";
    }
    else
    {
        LOG_INFO << "connect mysql failed!";
    }
    return p;                     
}

bool MySQL::update(std::string sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":" 
                    << sql << "更新失败!";
        return false;
    }
    return true;
}

MYSQL_RES* MySQL::query(std::string sql)
{
    if(mysql_query(conn_, sql.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ":"
                    << sql << "查询失败!";
        return nullptr;
    }
    return mysql_use_result(conn_);
}

MYSQL* MySQL::getConnection()
{
    return conn_;
}