
// YunTest.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif
#define HEARTID    3000
#define TUISONGID    3001
#define STRESS_DATA  5000

//定义推送预警心跳事件ID
#define AlarmID305      6000

//定义推送预警心跳事件ID
#define NOTICE102      8000

#include "resource.h"		// 主符号


// CYunTestApp:
// 有关此类的实现，请参阅 YunTest.cpp
//

//定义每个功能的类型和名称

//定义每个功能下的字典请求结构体
struct DictFun
{
	TCHAR Dictname[50];   //每个请求字段的名称
	TCHAR Dicttype[50];   //每个请求字段的类型，方便以后组包和解包使用
};

//定义功能号下的所有数据
struct ComFun
{
	int FunID;       //定义功能号
	TCHAR FunName[50];       //定义功能名称
	vector<DictFun> IDList;    //定义每个功能下面的所有请求字段
	//定义每个功能下的Ans列表
	map<int,vector<DictFun>> AnsList;  
};

//定义应答包的字典
struct AnsFun
{
	int flag;                    //应答标记
	vector<DictFun> AnsList;     //应答包里面的所有字典列表
};

//定义压力相关结构
struct IODATA
{
	char strdata[1024*10];
	int datalen;
};

//定义压力相关结构
struct FUNNO
{
	int Funid;
	CString Funname;
};

//定义应答功能号对应应答功能字典

//定义推送心跳缓冲区内容
struct TSHEART
{
	char HeartBuff[10*1024];          //推送心跳缓冲区
	int heartlen;                     //推送心跳包长度
	TSHEART(){ZeroMemory(HeartBuff,10*1024);heartlen=0;}
};

//定义102推送心跳缓冲区内容
struct NOTICEHEART
{
	char NoticeBuff[10*1024];          //推送心跳缓冲区
	int noticelen;                     //推送心跳包长度
	NOTICEHEART(){ZeroMemory(NoticeBuff,10*1024);noticelen=0;}
};


class CYunTestApp : public CWinApp
{
public:
	CYunTestApp();
	int socknum;

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CYunTestApp theApp;