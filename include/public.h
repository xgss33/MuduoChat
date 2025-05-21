#ifndef MUDUOCHAT_INCLUDE_PUBLIC_H
#define MUDUOCHAT_INCLUDE_PUBLIC_H

enum EnMsgType
{
    LOG_MSG = 1, // 登录消息
    LOG_MSG_ACK, // 登录响应消息
    REG_MES, // 注册消息
    REG_MES_ACK, // 注册响应消息
    ONE_CHAT_MSG // 聊天信息 
};

#endif // MUDUOCHAT_INCLUDE_PUBLIC_H

// // json
// // 注册
// {"msgid":3, "name":"test", "password":"123456"}
// // 登录
// {"msgid":1, "id":1, "password":"123456"}
// {"msgid":1, "id":2, "password":"12345"}
// // ptp
// {"msgid":5, "to":2,"from":1, "msg":"hello"}