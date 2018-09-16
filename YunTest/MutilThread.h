#include "HqSocket.h"
#pragma once

struct STATIC_DATA
{
	enum STK_TYPE
	{
		INDEX = 0,				//指数
		STOCK = 1,				//股票
		FUND = 2,				//基金
		BOND = 3,				//债券
		OTHER_STOCK = 4,		//其它股票
		OPTION = 5,				//选择权
		EXCHANGE = 6,			//外汇
		FUTURE = 7,				//期货
		FTR_IDX = 8,			//期指
		RGZ = 9,				//认购证
		ETF = 10,				//ETF
		LOF = 11,				//LOF
		COV_BOND = 12,			//可转债
		TRUST = 13,				//信托
		WARRANT = 14,			//权证
		REPO = 15,				//回购
		COMM = 16,				//商品现货
	};
	WORD	m_wStkID;			//本市场内唯一标示,在本市场内的序号
	char	m_strLabel[10];		//代码

	char	m_strName[32];		//名称
	BYTE	m_cType;			//STK_TYPE
	BYTE	m_nPriceDigit;		//价格最小分辨率，非常重要，每一个DWORD类型的价格都要除以10^m_nPriceDigit才是真正的价格
	short	m_nVolUnit;			//成交量单位，每一成交量单位表示多少股
//	MWORD	m_mFloatIssued;		//流通股本
//	MWORD	m_mTotalIssued;		//总股本

	DWORD	m_dwLastClose;		//昨收
	DWORD	m_dwAdvStop;		//涨停
	DWORD	m_dwDecStop;		//跌停

	int		GetPriceMul() const;		//价格乘数，由m_nPriceDigit决定
};

struct HQSTATIC
{
	char	m_strLabel[10];		//代码
	DWORD	m_dwLastClose;		//昨收
	DWORD	m_dwAdvStop;		//涨停
	float   m_highstop;
	DWORD	m_dwDecStop;		//跌停
	float   m_lowstop;
};
//定义连接线路的socket结构体
struct SOCKET_INFO
{
	long      lidx;              //连接序号
	CHqSocket g_Sock;            //CHqSocket类
	SOCKET    m_Sock;            //定义连接的socket
	bool      g_busy;            //判断该socket是否是忙碌状态
	BOOL      bConnected;        //是否连接是连接上的
};

#pragma pack(1)
struct ACC_CMDHEAD
{
	unsigned short wCmdType;
	unsigned short wAttr;
	unsigned int nLen;	
	unsigned int nExpandInfo;	//扩展信息，如果是请求数据，响应要原样返回;
};

struct ACC_CMDHEADEX
{
	unsigned short wCmdType;
	unsigned short wAttr;
	unsigned int nLen;	
	unsigned __int64 nExpandInfo;	//扩展信息，如果是请求数据，响应要原样返回;
};

struct BIG_HEAD
{
	unsigned char	cSparate;//分隔符号'{', ':'等, 除了0和‘}’'H' tag
	unsigned short	type;
	unsigned short  attrs;
	unsigned short  length;
};

struct push_head
{
	unsigned short  sub_type;
	unsigned short  sub_attrs;          // 取值SubAttrsEnum
	unsigned short  sub_length;
	unsigned int    sub_extend;
};

struct ACC_SERVERLOGIN
{
	char m_cValid[8];		//服务器验证串;
	unsigned int nSerId;	//服务器id，该id可以用ip标示;
};

struct LOGIN_HEAD
{
	ACC_CMDHEAD accCmdHead;
	ACC_SERVERLOGIN accServerLogin;
};

struct LOGIN_HEADEX
{
	ACC_CMDHEADEX accCmdHead;
	ACC_SERVERLOGIN accServerLogin;
};

enum
{
	ACCCMD_KEEPALIVE = 0x8080,	//心跳数据包，通用服务器也使用该接口
	ACCCMD_SERVERLOGIN,			//应用服务器登陆, 通用服务器也使用该接口
	ACCCMD_REGISTER,			//用户注册
	ACCCMD_VALIDREGISTER,		//验证注册
	ACCCMD_USERLOGIN,			//用户登陆
	ACCCMD_USERQUIT,			//用户退出
	ACCCMD_KICKUSER,			//踢出用户
	ACCCMD_SELFSTK,				//取或者存自选股
	ACCCMD_RECOMFRIEND,			//推荐好友
	ACCCMD_MODIFYTELECODE,		//修改手机号码
	ACCCMD_CORELEVEL,			//取用户等级和积分 10 a
	ACCCMD_DEFAULTRIGHT,		//取用户默认权限	b
	ACCCMD_VALIDDEFAULTRIGHT,	//取用户验证注册成功的默认权限	c
	ACCCMD_USERNAMEMOBILELOGIN,	//用户名密码手机号码登陆	d
	ACCCMD_FETCHTELCODEID,		//取手机号码和id	e
	ACCCMD_SETUSERSHORTMSG,		//设置用户股票池短信提醒功能 f
	ACCCMD_FETCHPOOLNEWSTK,		//取新入池的股票
	ACCCMD_SETPREWARNING,		//设置股票预警信息
	ACCCMD_COMMINTERFACE,		// 客户端和服务器端通用接口
	ACCCMD_LOGINNOTIFY,	//用户登陆后通知行情
	ACCCMD_STATCOMMINTERFACE=0xA0A0,		// 统计用户信息通用接口
	ACCCMD_SMSREG = 0x9001,		//短信注册
	ACCCMD_SYNCSUBS = 0x9002,		//同步关系确认数据
	ACCCMD_USERONLINE = 0x9011,	//用户上下线
	ACCCMD_THREAD_TPYE = 0x9999,	//管道命令
	ACCCMD_END,	//无用,放在末位,仅用来判断指令的范围
};
#pragma pack()

class CMutilThread
{
public:
	CMutilThread(void);
	~CMutilThread(void);

public:
	FILE *fptext;
	// 读取二进制文件
	void ReadFile(char * filename, char *MarketCode);
	int  socketcount;                   //socket个数
	map<int,SOCKET_INFO> Socket_map;
	vector<SOCKET_INFO> vecSock;

//	char GenBuff[1024*5];
//	int  packoflen;

	CHqSocket  g_SockClass;
	//静态码表数据
	map<int,HQSTATIC> HqData;
	map<int,HQSTATIC> SHHqData;
	map<int,HQSTATIC> SZHqData;
	map<int,HQSTATIC> HKHqData;
	//300功能包用户推送标示
//	map<int,int> UserID;
	map<int,string> UserID;
	int userid;         //用户序号，用于遍历所有用户
	int stockid;        //股票代码序号，用于遍历所有代码
	// 创建多线程的函数
	void CreateThreadMash(void);

	HANDLE     *socket_thread;

	//创建socket线程句柄
	HANDLE     SocketHandle;

	HANDLE     m_SOCKSemp;   //创建个信号量用来创建socket

	int CPUnum;
	//socket个数
	int       Socket_Count;
	//创建socket线程处理函数
	bool p_sockstop;
	static DWORD WINAPI _SocketThread(LPVOID lparam);

	//退出线程事件
	HANDLE  ShutdownEvent;
	static DWORD WINAPI _NetThread(LPVOID lparam);

	HANDLE g_Event;   //等socket建立完成后才触发socket处理线程

	bool g_mutilstop;
	// 单独开启个线程创建socket线程
	void CreateSocket(void);

	//结构体之间的互斥锁
	CCriticalSection    g_mutil;
	//线程锁
	CCriticalSection    g_threadlock;
	// 初始化map结构体信息
	bool InitMap(void);
	// 释放map结构体信息
	void Uninit(void);
	// 释放某个链接
	void DisConnectSvr(SOCKET_INFO * LINK_INFO);
	// 获取空闲socket已发送数据
	SOCKET_INFO* GetLink(void);
	// 用完此连接后释放此次连接
	void SetFreeLink(SOCKET_INFO * SockData);
	//300添加预警功能函数
//	bool Generate300Pack(int PackType);
	bool Generate300Pack(int PackType, char *GenBuff, int *buffoflen);
	// 添加用户到map数组中
	void AddUser(void);
	// 创建socket
	int GenerateSock(void);
};

