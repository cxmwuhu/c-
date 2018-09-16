#include "YunTest.h"

#pragma once

//定义缓冲区buff大小
#define IOCP_SIZE   1024*300

//定义IO状态字段枚举值
enum OPERATION_TYPE
{
	IO_SEND,    //用于投递发送信息状态
	IO_RECV,    //用于投递接受信息状态
	IO_NULL     //用于初始化的标志，不具有实际意义
};

//定义单个IO操作状态
//====================================================================================
//
//				单IO数据结构体定义(用于每一个重叠操作的参数)
//
//====================================================================================
typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED        m_overlapped;                 //定义重叠的IO结构（每个socket有单独的一个重叠io结构
	SOCKET            m_IOsocket;                   //定义每个重叠结构的socket套接字
	WSABUF            m_wsaBuff;                    //WSA类型的缓冲区，用于给每个重叠io使用
	char              m_Buff[IOCP_SIZE];           //WSABUF里面存放的缓冲区数据
	OPERATION_TYPE    m_iotype;                     //定义每个重叠结构的操作类型
	int               m_Serial;                      //定义每个socket的发送操作序号

	//初始化相关的参数,构造函数初始化
	_PER_IO_CONTEXT()
	{
		ZeroMemory(&m_overlapped,sizeof(m_overlapped));
		m_IOsocket = INVALID_SOCKET;
		ZeroMemory(m_Buff,IOCP_SIZE);
		m_wsaBuff.buf = m_Buff;
		m_wsaBuff.len = IOCP_SIZE;
		m_iotype = IO_NULL;
		m_Serial = 0;
	}

	//析够函数释放资源
	~_PER_IO_CONTEXT()
	{
		if(m_IOsocket!=INVALID_SOCKET)
		{
			closesocket(m_IOsocket);
			m_IOsocket = INVALID_SOCKET;
		}
	}

	//重置缓冲区
	void  ResetBuff()
	{
		ZeroMemory(m_Buff,IOCP_SIZE);
	}

}PER_IO_CONTEXT, *PPER_IO_CONTEXT;

/*
//定义每个用户的io信息

//====================================================================================
//
//				单用户数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//
//====================================================================================
typedef struct _PER_SIGNLE_CONTEXT
{
	UINT m_indexuser;	//用户索引
	PPER_IO_CONTEXT m_pSendPerIoContext;	//用户发送io
	PPER_IO_CONTEXT m_pRecvPerIoContext;	//用户接收io
	CRITICAL_SECTION m_csUser;				//每用户的关键代码段

	// 初始化
	_PER_SIGNLE_CONTEXT()
	{
		m_indexuser=0;
		m_pSendPerIoContext=new PER_IO_CONTEXT;
		m_pRecvPerIoContext=new PER_IO_CONTEXT;
		InitializeCriticalSection(&m_csUser);
	}
	// 释放掉Socket
	~_PER_SIGNLE_CONTEXT()
	{
		m_indexuser=0;
		delete m_pSendPerIoContext;
		m_pSendPerIoContext=NULL;
		delete m_pRecvPerIoContext;
		m_pRecvPerIoContext=NULL;
		DeleteCriticalSection(&m_csUser);
	}

} PER_SIGNLE_CONTEXT, *PPER_SIGNLE_CONTEXT;
*/
//====================================================================================
//
//				单句柄数据结构体定义(用于每一个完成端口，也就是每一个Socket的参数)
//
//====================================================================================
typedef struct _PER_SOCKET_CONTEXT
{
	//定义每个io的socket
	SOCKET                            m_Socket;                   //定义网络连接的socket
	int                               nindex;                       //定义每个socket的序号
//	CArray<_PER_SIGNLE_CONTEXT*>          m_arrayContext;               //// 客户端网络操作的上下文数据，
	                                                       // 也就是说对于每一个客户端Socket，是可以在上面同时投递多个IO请求的
	CArray<_PER_IO_CONTEXT*>          m_arrayContext;
	//初始化变量，构造函数初始化
	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		nindex = 0;
	}

	//初始化析构函数
	~_PER_SOCKET_CONTEXT()
	{
		if(m_Socket!=INVALID_SOCKET)
		{
		    closesocket(m_Socket);
		}
		//移除array数组
		for(int i=0; i<m_arrayContext.GetCount(); i++)
		{
			delete []m_arrayContext.GetAt(i);
		}
		m_arrayContext.RemoveAll();
	}

	//获取一个新的IO数组
//	_PER_SIGNLE_CONTEXT* GetNewIOContext()
	_PER_IO_CONTEXT* GetNewIOContext()
	{
	//	_PER_SIGNLE_CONTEXT* pSignleContext = new _PER_SIGNLE_CONTEXT();
		_PER_IO_CONTEXT* pIOContext = new _PER_IO_CONTEXT();
		m_arrayContext.Add(pIOContext);
		return pIOContext;
	}

	//从array数组里去除一个IOCONTEXT
//	void RemoveIOContext(_PER_SIGNLE_CONTEXT* pSignleContext)
	void RemoveIOContext(_PER_IO_CONTEXT* pSignleContext)
	{
		for(int b = 0; b<m_arrayContext.GetCount(); b++)
		{
			if(m_arrayContext.GetAt(b) == pSignleContext)
			{
				delete pSignleContext;
				pSignleContext = NULL;
				m_arrayContext.RemoveAt(b);
				break;
			}
		}
	}

}PER_SOCKET_CONTEXT, *PPER_SOCKET_CONTEXT;

class CIOFUN;
typedef struct _tagThreadParams_WORKER
{
	CIOFUN*    pIOCP;          //定义CIOCP的指针用于使用类函数里面的所有函数和数据
	int       nThreadno;      //线程编号
}THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;

class CIOFUN
{
public:
	CIOFUN(void);
	~CIOFUN(void);
	bool Start(void);
	// 停止服务器
	void Stop(void);

private:
	int                          mapid;                         //map功能里面的id号

	CDialog*                     m_pMain;                       // 主界面的界面指针，用于在主界面中显示消息

	//定义退出线程的事件句柄,用来通知线程系统退出的事件，为了能够更好的退出线程
	HANDLE                       m_ShutdownEvent;

	//定义完成端口句柄
	HANDLE                       m_IOCompletionPort;

	//定义工作者线程句柄
	HANDLE*                      m_ThreadWorker;

	int                          nThreadnum;               //生成的线程数量

	char                         IOaddr[30];              //压力地址

	int                          IOport;                  //压力端口

	CCriticalSection            m_IOSection;             //定义线程间的互斥变量，以防止线程冲突，加锁

	CRITICAL_SECTION             m_IOCS;                     //定义线程间的互斥变量，以防止线程冲突，加锁

	CArray<PER_SOCKET_CONTEXT*>   m_arrayclientcontext;    //定义客户端socket的context数组内容

	PER_SOCKET_CONTEXT*           pContext;                //创建单个IO_SOCKET指针

	int                       LastSendTime;            //定义上次发送时间，没有到数据包发送时，不需要发送

	int                       ThisSendTime;            //定义本次次发送时间，没有到数据包发送时，不需要发送

	int                           SendTickTime;            //发送间隔时间，读取界面配置选项
public:
	// 初始化IO信息
	bool _InitializeIOCP(void);
	// 初始化批量socket
	bool _InitializeSocket(void);

	//工作者线程
	static DWORD WINAPI _WorkerThread(LPVOID lparam);
	// 投递接受PER_IO_CONTEXT信息
	bool _PostRecv(PER_IO_CONTEXT * pIO_Context);
	// 投递发送业务操作
	bool _PostSend(PER_IO_CONTEXT * pIOSendContext);
	// 投递发送信息
	bool _DoSend(PER_IO_CONTEXT * pSendContext);
	// 做接受投递业务操作
	bool _DoRecv(PER_SOCKET_CONTEXT* pSocketContext,PER_IO_CONTEXT * pRecvContext);
	// 从数组里移除PER_SOCKET_CONTEXT信息
	void _RemoveContext(PER_SOCKET_CONTEXT* pSocketContext);
	// 清空客户端信息
	void _ClearContextList(void);
	// 处理完成端口上的错误信息
	bool HandleError(PER_SOCKET_CONTEXT* pContext,const DWORD& dwErr);
	// 最后释放资源
	void _DeInitialize(void);
};

