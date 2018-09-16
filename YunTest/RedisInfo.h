#pragma once

#include <string>
using namespace std;

class CRedisInfo
{
public:
	CRedisInfo(void);
	~CRedisInfo(void);
	// redis���Ӻ���
	int RedisConn(string hostip, int redisport);

//private:
public:
	redisContext* m_Redis;
	redisReply* m_reply;
	//���������Ƕ��������vector��������
	vector<std::string> RedisData;
	//���ص����ַ����ĸ�ʽREDIS_REPLY_STATUS,ͨ��str�����ַ������ݣ�len��ʾ�ַ�������
	string redis_string;
	//���ص�������REDIS_REPLY_STRING�����ݲ���strֵ��������len
	string reply_str;
	//���ص�������REDIS_REPLY_INTEGER��������������integer�ֶλ�ȡֵ��
	int reply_int;
	//���ص�������REDIS_REPLY_ERROR����ʾ�����鿴������Ϣ�����ϵ�str,len�ֶΣ�
	string reply_err;
	//���ص�������REDIS_REPLY_NIL��û�����ݷ���
public:
	// redis�������
	int RedisExcute(string Exstring);
	string operator=(const string &s);
};

