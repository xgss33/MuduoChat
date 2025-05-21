#ifndef MUDUO_INCLUDE_SERVER_DBUTILS_H
#define MUDUO_INCLUDE_SERVER_DBUTILS_H

#include <string>
#include <mysql/mysql.h>

/// @brief 数据库预处理参数绑定相关工具函数
namespace dbutils
{
    /// @brief 绑定输入字符串参数
    /// @param bind 预处理绑定结构体引用
    /// @param input 要绑定的字符串输入
    /// @param length 输入字符串长度，函数内部会更新此值
    void bindInputString(MYSQL_BIND& bind, const std::string& intput, unsigned long& length);

    void bindInputInt(MYSQL_BIND& bind, int& intput);

    void bindOutputString(MYSQL_BIND& bind, char* output, unsigned long outputLen, unsigned long& length);

    void bindOutputInt(MYSQL_BIND& bind, int& output);
}

#endif // MUDUO_INCLUDE_SERVER_DBUTILS_H