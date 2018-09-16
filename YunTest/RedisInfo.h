#pragma once

#include <string>
using namespace std;

class CRedisInfo
{
public:
	CRedisInfo(void);
	~CRedisInfo(void);
	// redis连接函数
	int RedisConn(string hostip, int redisport);

//private:
public:
	redisContext* m_Redis;
	redisReply* m_reply;
	//返回内容是多个，数组vector向量内型
	vector<std::string> RedisData;
	//返回的是字符串的格式REDIS_REPLY_STATUS,通过str查找字符串内容，len表示字符串长度
	string redis_string;
	//返回的内型是REDIS_REPLY_STRING，内容查找str值，长度是len
	string reply_str;
	//返回的类型是REDIS_REPLY_INTEGER，返回整数，从integer字段获取值；
	int reply_int;
	//返回的类型是REDIS_REPLY_ERROR，表示出错，查看出错信息，如上的str,len字段；
	string reply_err;
	//返回的类型是REDIS_REPLY_NIL，没有数据返回
public:
	// redis命令查找
	int RedisExcute(string Exstring);
	string operator=(const string &s);
};

