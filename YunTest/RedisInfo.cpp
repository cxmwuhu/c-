#include "StdAfx.h"
#include "RedisInfo.h"

//����UTF16��UTF8ת��;
 char * UTF16_T_UTF8( const wstring& strUTF16 )  
 {  
	 int nUTF8Length = ::WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,NULL,0,0,0); 
	 char * pStrUtf8=new char[nUTF8Length];
	 WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,pStrUtf8,nUTF8Length,0,0);  
	 return pStrUtf8;  
 }  

//����UTF8��UTF16ת��;
 wstring UTF8_T_UTF16( const string& strUTF8 )  
 {  
	 int nUTF16Length = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,NULL,0);  
	 nUTF16Length += 1;  
	 wstring strUTF16(nUTF16Length ,' ');  
	 int nRet = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,&strUTF16[0],nUTF16Length);  
	 ASSERT(0 != nRet);  
	 return strUTF16;  
 }  


char convertHexToChar(const char* hex){
    int ch;
    std::istringstream(hex) >> std::hex >> ch;
    return (char)ch;
}

std::string parse(const std::string& s){
    std::string res;
    int pos = 0;
    for (int i = 0; i < s.size(); pos++) {
        if(s[i] == '\\'){
            char* hex = new char[2];
            s.copy(hex, 2, i + 2);

            char ch = convertHexToChar(hex);
            printf("convert 0x%s to %d %c\n", hex, ch, ch);

            res.append(1, ch);
            i += 4;
        }else{
            res.append(1, s[i]);
            i++;
        }
    }

    return res;
}

CRedisInfo::CRedisInfo(void)
{
	m_Redis = NULL;
	m_reply = NULL;
}


CRedisInfo::~CRedisInfo(void)
{
}


// redis���Ӻ���
int CRedisInfo::RedisConn(string hostip, int redisport)
{
	if(m_Redis)
	{
		redisFree(m_Redis);
		m_Redis = NULL;
	}
	struct timeval redistv;
	
	redistv.tv_sec = ::GetPrivateProfileInt(L"REDIS",L"REDISSEC",10,L".\\HQ.INI");
	redistv.tv_usec = 0;
	WSADATA wsaData;
	int err;
	WORD wRequest;
	wRequest = MAKEWORD(2,2);
	err = WSAStartup(wRequest,&wsaData);
	TCHAR redisip[20] = {0};
	::GetPrivateProfileString(L"REDIS",L"REDISIP",L"127.0.0.1",redisip,20,L".\\HQ.INI");
	redisport = ::GetPrivateProfileInt(L"REDIS",L"REDISPORT",6379,L".\\HQ.INI");
	
	m_Redis = redisConnect(hostip.c_str(),redisport);
	if(m_Redis==NULL||m_Redis->err)
	{
		char errmsg[300] = {0};
		strcpy(errmsg,m_Redis->errstr);
		printf("Connect to redisServer faile:%s\n",errmsg); 
		redisFree(m_Redis);
		
		m_Redis = NULL;
		return 1;
	}
	return 0;
}


// redis�������
int CRedisInfo::RedisExcute(string Exstring)
{
	int markdata = 0;
	char *reqstr = new char[Exstring.length()+1];
	memset(reqstr,0,Exstring.length()+1);
	printf("%s\n",Exstring.c_str());

	memcpy(reqstr,Exstring.c_str(),Exstring.length());
	char *req = new char[Exstring.length()];
	memset(req,0,Exstring.length());
	
	int strpos = 0,k=0;
	while(*(reqstr+strpos)!='\0')
	{
		if(*(reqstr+strpos) == '\\')
		{
			char *byte=new char[2];
			memset(byte,0,2);
			memcpy(byte,reqstr+strpos+2,2);
			char tempch = convertHexToChar(byte);
			*(req+k) = tempch;
			strpos += 4;
			delete []byte;
			k++;
		}
		else
		{
			*(req+k) = *(reqstr+strpos);
	        strpos++;
			k++;
		}
	}
	string res;
	res.append(req,k);
	wchar_t tempcmd[50] = {0};
	::GetPrivateProfileString(L"REDIS",L"COMMAND",L"LRANGE",tempcmd,50,L".\\HQ.INI");
	string cmdstr = UTF16_T_UTF8(tempcmd);
	if(strcmp(cmdstr.c_str(),"LRANGE") == 0)
	{
		printf("%d\n",res.size());
	    m_reply = (redisReply *)redisCommand(m_Redis,"lrange %b 0 -1",res.c_str(),res.size());
	    delete []req;
	    delete []reqstr;
	}
	if(strcmp(cmdstr.c_str(),"ttl") == 0)
	{
        m_reply = (redisReply *)redisCommand(m_Redis,"ttl %b",res.c_str(),res.size());
	    delete []req;
	    delete []reqstr;
	}
	if(strcmp(cmdstr.c_str(),"scan") == 0)
	{
		m_reply = (redisReply *)redisCommand(m_Redis,"scan %b",res.c_str(),res.size());
	    delete []req;
	    delete []reqstr;
	}
	/*
	else
	{
		m_reply = (redisReply *)redisCommand(m_Redis,Exstring.c_str()); 
	}
	*/
	if( NULL == m_reply)  
    {  
        printf("Execut command1 failure\n");  
        redisFree(m_Redis);     // ����ִ��ʧ�ܣ��ͷ��ڴ�  
        return NULL;  
    }  
	/*
	��֪�������˾���ʲô�������ݣ�
REDIS_REPLY_STATUS      ��ʾ״̬������ͨ��str�ֶβ鿴���ַ���������len�ֶ�
REDIS_REPLY_ERROR       ��ʾ�����鿴������Ϣ�����ϵ�str,len�ֶ�
REDIS_REPLY_INTEGER    ������������integer�ֶλ�ȡֵ
REDIS_REPLY_NIL            û�����ݷ���
REDIS_REPLY_STRING      �����ַ������鿴str,len�ֶ�
REDIS_REPLY_ARRAY       ����һ�����飬�鿴elements��ֵ�������������ͨ��
                                      element[index]�ķ�ʽ��������Ԫ�أ�ÿ������Ԫ����
                                      һ��redisReply�����ָ��
		
	*/
	 //�õ�������������
	 if(m_reply->type == REDIS_REPLY_ARRAY)
	 {
	     int elementsum = m_reply->elements;
		 redisReply *m_replyelement = NULL;
		 for(int j=0;j<elementsum;j++)
		 {
			 //˵��ȡ�����ַ���
			 if(m_reply->element[j]->type == REDIS_REPLY_STRING)
			 {	 
				 char *newchar = new char[(m_reply->element[j]->len)*4+1];
				 memset(newchar,0,(m_reply->element[j]->len)*4+1);
				 int recvpos=0,recvkv=0,pos=0;
				 while(recvpos<m_reply->element[j]->len)
				 {
					 unsigned char tmpch = *(m_reply->element[j]->str+recvpos);
					 unsigned char *temch = new unsigned char[5];
					 memset(temch,0,5);
					 sprintf((char*)temch,"\\x%02x",tmpch);
					 memcpy(newchar+pos,temch,4);
					 pos += 4;
					 recvpos++;
					 recvkv++;
					 delete []temch;
				 }
				 string temstr = newchar;
				 printf("keyֵ:%s\n",temstr.c_str());
				 delete []newchar;
				 RedisData.push_back(temstr);
			 }
		 }
		 markdata = m_reply->type;
	 }
	 //������ص�������REDIS_REPLY_STATUS��ֱ�Ӷ�ȡ�ַ�����������ݣ��ַ�������ȡlenֵ
	 if(m_reply->type == REDIS_REPLY_STATUS)
	 {
		 printf("������ֶ�����:%s,�ַ�������:%d\n",m_reply->str,m_reply->len);
		 redis_string = m_reply->str;
		 markdata = m_reply->type;
	 }
	 if(m_reply->type == REDIS_REPLY_STRING)  //��������Ϊstring����
	 {
		 printf("������ֶ�����:%s,�ַ�������:%d\n",m_reply->str,m_reply->len);
		 char *newchar = new char[(m_reply->len)*4+1];
		 memset(newchar,0,(m_reply->len)*4+1);
		 int recvpos=0,recvkv=0,pos=0;
		 while(recvpos<m_reply->len)
		 {
		     unsigned char tmpch = *(m_reply->str+recvpos);
			 unsigned char *temch = new unsigned char[5];
			 memset(temch,0,5);
			 sprintf((char*)temch,"\\x%02x",tmpch);
			 memcpy(newchar+pos,temch,4);
			 pos += 4;
			 recvpos++;
			 recvkv++;
			 delete []temch;
		 }
		 reply_str = newchar;
		 markdata = m_reply->type;
		 delete []newchar;
	 }
	 if(m_reply->type == REDIS_REPLY_INTEGER)   //���ص�����������
	 {
		 printf("���ص�����������:%d\n",m_reply->integer);
		 reply_int = m_reply->integer;
		 markdata = m_reply->type;
	 }
	 if(m_reply->type == REDIS_REPLY_ERROR)
	 {
		 printf("������Ϣ������:%s\n",m_reply->str);
		 reply_err = m_reply->str;
		 markdata = m_reply->type;
	 }
    freeReplyObject(m_reply);  
	printf("Succeed to execute command[%s],����:%d\n", Exstring.c_str(),markdata);  
	return markdata;
}
