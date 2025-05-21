#include "server/dbutils.h"

void dbutils::bindInputString(MYSQL_BIND& bind, const std::string& intput, unsigned long& length)
{
    length = static_cast<unsigned long>(intput.length());
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = const_cast<char*>(intput.c_str());
    bind.buffer_length = length;
    bind.is_null = 0;
    bind.length = &length;
}

void dbutils::bindInputInt(MYSQL_BIND& bind, int& intput)
{
    bind.buffer_type = MYSQL_TYPE_LONG;
    bind.buffer = &intput;
    bind.is_null = 0;
    bind.length = nullptr;
}

void dbutils::bindOutputString(MYSQL_BIND& bind, char* output, unsigned long outputLen, unsigned long& length)
{
    bind.buffer_type = MYSQL_TYPE_STRING;
    bind.buffer = output;
    bind.buffer_length = outputLen;
    bind.is_null = 0;
    bind.length = &length;
}

void dbutils::bindOutputInt(MYSQL_BIND& bind, int& output)
{
    bind.buffer_type = MYSQL_TYPE_LONG;
    bind.buffer = &output;
    bind.is_null = 0;
    bind.length = nullptr;
}