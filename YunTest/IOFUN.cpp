#include "StdAfx.h"
#include "IOFUN.h"
#include "YunTestDlg.h"


// 释放指针和句柄资源的宏

// 释放指针宏
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// 释放句柄宏
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// 释放Socket宏
#define RELEASE_SOCKET(x)               {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}

#pragma pack(1)
 //定义行情包头
 struct HQ_HEAD
 {
	 unsigned char	cSparate;//分隔符号'{', ':'等, 除了0和‘}’'H' tag
	 unsigned short	type;
	 unsigned short  attrs;
	 unsigned short  length;
 };

 //推送包头
struct sub_head
{
    unsigned short  sub_type;
    unsigned short  sub_attrs;          // 取值SubAttrsEnum
    unsigned short  sub_length;
    unsigned int    sub_extend;
};
 //定义对齐方式
#pragma pack()

CIOFUN::CIOFUN(void):
	m_ShutdownEvent(NULL),
	m_IOCompletionPort(NULL),
	m_ThreadWorker(NULL),
	m_pMain(NULL)
{
	nThreadnum = 0;               //生成的线程数量
	IOport = 0;                  //压力端口
	LastSendTime = 0;            //定义上次发送时间，没有到数据包发送时，不需要发送
	ThisSendTime = 0;            //定义本次次发送时间，没有到数据包发送时，不需要发送
	SendTickTime = 0;            //发送间隔时间，读取界面配置选项
	mapid = 0;
}


CIOFUN::~CIOFUN(void)
{
	this->Stop();
}


bool CIOFUN::Start(void)
{
	InitializeCriticalSection(&m_IOCS);
	//建立系统退出事件
	m_ShutdownEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		/*
	HANDLE CreateEvent(
LPSECURITY_ATTRIBUTES lpEventAttributes,
BOOL bManualReset, 
BOOL bInitialState,
LPCSTR lpName
);
bManualReset:TRUE，使用ResetEvent()手动重置为无信号状态；FALSE，当一个等待线程被释放时,自动重置状态为无信号状态。
bInitialState：指定事件对象的初始状态，当TRUE,初始状态为有信号状态；当FALSE,初始状态为无信号状态。
下面主要演示一下采用CreateEvent实现多线程。
例子很简单，主要测试CreateEvent中bManualReset:和bInitialState参数的取值在线程调用中信号状态的情况。
*/
	//初始化io系统
	if(_InitializeIOCP() == false)
	{
		printf("初始化io结构失败:%d\n",WSAGetLastError());
		return false;
	}
	if(_InitializeSocket() == false)
	{
		printf("初始化批量socket失败:%d\n",WSAGetLastError());
		return false;
	}
	return true;
}


// 停止服务器
void CIOFUN::Stop(void)
{
	//把退出事件调整为有信号事件
	if(m_ShutdownEvent)
	{
	    SetEvent(m_ShutdownEvent);
	    //退出完成端口，以便让线程全部安全退出

		/*
		PostQueuedCompletionStatus函数，向每个工作者线程都发送—个特殊的完成数据包。该函数会指示每个线程都“立即结束并退出”.下面是PostQueuedCompletionStatus函数的定义： 
BOOL PostQueuedCompletionStatus( 
    HANDLE CompletlonPort, 
    DW0RD  dwNumberOfBytesTrlansferred, 
    DWORD  dwCompletlonKey, 
LPOVERLAPPED lpoverlapped, 

); 

其中，CompletionPort参数指定想向其发送一个完成数据包的完成端口对象。而就dwNumberOfBytesTransferred,dwCompletionKey和lpOverlapped这三个参数来说.
每—个都允许我们指定—个值,直接传递给GetQueuedCompletionStatus函数中对应的参数。这样—来。—个工作者线程收到传递过来的三个GetQueuedCompletionStatus函数参数后，
便可根据由这三个参数的某一个设置的特殊值，决定何时应该退出。例如,可用dwCompletionPort参数传递0值,而—个工作者线程会将其解释成中止指令。一旦所有工作者线程都已关闭,
便可使用CloseHandle函数,关闭完成端口。最终安全退出程序。 
PostQueuedCompletionStatus函数提供了一种方式来与线程池中的所有线程进行通信。如，当用户终止服务应用程序时，我们想要所有线程都完全利索地退出。
但是如果各线程还在等待完成端口而又没有已完成的I/O 请求，那么它们将无法被唤醒。 
通过为线程池中的每个线程都调用一次PostQueuedCompletionStatus，我们可以将它们都唤醒。
每个线程会对GetQueuedCompletionStatus的返回值进行检查，如果发现应用程序正在终止，那么它们就可以进行清理工作并正常地退出
*/

	    for(int i=0; i<nThreadnum; i++)
	    {
		    PostQueuedCompletionStatus(m_IOCompletionPort,0,(DWORD)NULL,NULL);
	    }
	    //等待所有的客户端资源退出
	    WaitForMultipleObjects(nThreadnum,m_ThreadWorker,TRUE,INFINITE);
	    //清除客户端列表
	    this->_ClearContextList();
	    //释放资源
	    this->_DeInitialize();
		mapid = 0;
		DeleteCriticalSection(&m_IOCS);
	    printf("停止完成端口操作完成\n");
	}
}


// 初始化IO信息
bool CIOFUN::_InitializeIOCP(void)
{
	//创建完成端口
	/*
	HANDLE WINAPI CreateIoCompletionPort(
  __in          HANDLE FileHandle,
  __in          HANDLE ExistingCompletionPort,
  __in          ULONG_PTR CompletionKey,
  __in          DWORD NumberOfConcurrentThreads
);
一个I/O完成端口关联一个或多个文件句柄，也可以在创建I/O完成端口的时候 没有关联任何文件句柄。
一个I/O完成端口关联一个打开文件的实例使应用程序可以收到包括这个文件异步I/O操作的完成通知。

参数：
FileHandle 打开重叠IO完成端口的文件句柄
如果设置这个参数为INVALID_HANDLE_VALUE，那 么，CreateIoCompletionPort 会创建一个不关联任何文件的完成端口，而且ExistingCompletionPort 必须设置为NULL,CompletionKey 也将被忽略。

ExistingCompletionPort  完成端口句柄
如果指定一个已经存在的完成端口，函数将关联FileHandle 指定的文件，并返回已存在的完成端口句柄，函数不会创建一个新的完成端口。
如果这个参数为NULL，函数创建一个与FileHandle指定的文 件关联的完成端口，并返回一个新的完成端口句柄。

CompletionKey 单文件句柄，包含指定文件每次IO完成包数据信息。

NumberOfConcurrentThreads 系统允许在完成端口上并发处理IO完成包的最大线程数量。
如果 ExistingCompletionPort 为NULL，此参数忽略

返回值：
如果执行成功，函数返回关联指定文件的完成端口句柄；否则，返回NULL

备注：
CreateIoCompletionPort 提供这个功能：I/O系统可以被用来向列队的I/O完成端口发送I/O完成通知包。
当 你执行一个已经关联一个完成端口的文件I/O操作，I/O系统将会在这个I/O操作完成的时候向I/O完成端口发送一个完成通知包，I/O完成端口将以先 进先出的方式放置这个I/O完成通知包，并使用GetQueuedCompletionStatus 接收I/O完成通知包。
虽然允许任何数量的 线程来调用GetQueuedCompletionStatus 等待一个I/O完成端口，但每个线程只能同时间内关联一个I/O完成端口，且此端口是线程最后检查的那个端口。
当一个包被放入队列中，系统首先会 检查有多少个关联此端口的线程在运行，如果运行的线程的数量少于NumberOfConcurrentThreads的值，
那么允许其中的一个等 待线程去处理包。当一个运行的线程完成处理，将再次调用GetQueuedCompletionStatus ，此时系统允许另一个等待线程去处理包。
系 统也允许一个等待的线程处理包如果运行的线程进入任何形式的等待状态，当这个线程从等待状态进入运行状态，可能会有一个很短的时期活动线程的数量会超过 NumberOfConcurrentThreads 的值，此时，系统会通过不允许任何新的活动线程快速的减少线程个数，直到活动线程少于NumberOfConcurrentThreads 的值。
*/
	m_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if(m_IOCompletionPort == NULL)
	{
		printf("完成端口创建失败:%d\n",WSAGetLastError());
		return false;
	}
	//创建工作者线程函数。计算最优线程个数
	SYSTEM_INFO Systeminfo;
	GetSystemInfo(&Systeminfo);
	nThreadnum = Systeminfo.dwNumberOfProcessors*2;
//	nThreadnum = 2;
	DWORD ThreadID;
	m_ThreadWorker = new HANDLE[nThreadnum];
	for(int i=0; i<nThreadnum; i++)
	{
		THREADPARAMS_WORKER *pthread = new THREADPARAMS_WORKER;
		pthread->pIOCP = this;
		pthread->nThreadno = i+1;

		m_ThreadWorker[i] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)_WorkerThread,(void*)pthread,0,&ThreadID);
		printf("IO完成端口工作者线程创建成功:%d\n",ThreadID);
	}
	return true;
}


// 初始化批量socket
bool CIOFUN::_InitializeSocket(void)
{
		//初始化套接字
	WORD nRequestVer;
	nRequestVer = MAKEWORD(2,2);
	WSAData wsaData;
	DWORD ret;
	ret = WSAStartup(nRequestVer,&wsaData);
	if(ret != 0)
	{
		printf("初始化套接字失败:%d\n",WSAGetLastError());
		return false;
	}
	CString linknum = _T("");

	CYunTestDlg *pio = (CYunTestDlg*)AfxGetMainWnd();
	pio->GetDlgItem(IDC_EDIT_NUMBER)->GetWindowText(linknum);
	int m_Conncount = _ttoi(linknum);
	//获取ip地址
	CString IOip = _T(""),tempport = _T("");
	pio->GetDlgItem(IDC_EDIT_IPADDR)->GetWindowText(IOip);
	int tempioip = IOip.GetLength();
	for(int i=0; i<tempioip; i++)
	{
		IOaddr[i] = IOip.GetAt(i);
	}
	IOaddr[tempioip] = '\0';

	pio->GetDlgItem(IDC_EDIT_PORT)->GetWindowText(tempport);
	IOport = _ttoi(tempport);
	
	//获取发送间隔时间
	SendTickTime = pio->GetDlgItemInt(IDC_EDIT_SECOND);
	//增加socket地址组装
	SOCKADDR_IN sockclientaddr;
	sockclientaddr.sin_addr.S_un.S_addr = inet_addr(IOaddr);
	sockclientaddr.sin_family = AF_INET;
	sockclientaddr.sin_port = htons(IOport);
	PER_SOCKET_CONTEXT *pPer_Socket_Context;
	pContext = new PER_SOCKET_CONTEXT;
//	PER_SIGNLE_CONTEXT *pIOSignleContext;
	PER_IO_CONTEXT *pIOSignleContext = new PER_IO_CONTEXT;
	SOCKET clientsocket;
	char tempbuff[1024*5] = {0};
	for(int i=0; i<m_Conncount; i++)
	{
		clientsocket =  WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
		if(clientsocket == INVALID_SOCKET)
		{
			printf("WSASocket()创建失败:%d\n",WSAGetLastError());
		}
		else
		{
			printf("WSASocket创建成功:%d\n",clientsocket);
		}
		WSAIoctl(clientsocket,FIONBIO,NULL,0,NULL,0,NULL,NULL,NULL);
		int retconn = connect(clientsocket,(sockaddr*)&sockclientaddr,sizeof(sockclientaddr));
		if(retconn == 0)
		{
			printf("socket连接成功:%d\n",clientsocket);
			pPer_Socket_Context = new PER_SOCKET_CONTEXT;

			pPer_Socket_Context->nindex = i;
			pPer_Socket_Context->m_Socket = clientsocket;
			
			//1把socket绑定到完成端口中
			HANDLE tempio = ::CreateIoCompletionPort((HANDLE)pPer_Socket_Context->m_Socket,m_IOCompletionPort,(DWORD)pPer_Socket_Context,0);
			if(tempio == NULL)
			{
				printf("完成端口创建失败:%d\n",WSAGetLastError());
				
				RELEASE_HANDLE(pPer_Socket_Context);
			}
			//1把创建好的完成端口数据PER_SOCKET_CONTEXT加到array列表中
			//1需要加锁，以防止再有连接上来取数据时出现错误
			EnterCriticalSection(&m_IOCS);
			m_arrayclientcontext.Add(pPer_Socket_Context);
			LeaveCriticalSection(&m_IOCS);
			pio->AddNum(0,1);
		}
	}
	for(int j=0; j<m_arrayclientcontext.GetCount(); j++)
	{
		//1投递接收包
		pContext = m_arrayclientcontext.GetAt(j);
		pIOSignleContext = pContext->GetNewIOContext();
		pIOSignleContext->m_iotype = IO_RECV;
		pIOSignleContext->m_IOsocket = pContext->m_Socket;
		pIOSignleContext->m_IOsocket = m_arrayclientcontext[j]->m_Socket;
		_PostRecv(pIOSignleContext);

		printf("发送第一个包\n");
		CYunTestApp *tmpapp = (CYunTestApp*)AfxGetApp();
		CYunTestDlg *pIODlg = (CYunTestDlg*)tmpapp->m_pMainWnd;
		pContext = m_arrayclientcontext.GetAt(j);
		pIOSignleContext = pContext->GetNewIOContext();
		pIOSignleContext->m_IOsocket = pContext->m_Socket;
		pIOSignleContext->m_iotype = IO_SEND;
		memset(pIOSignleContext->m_Buff,0,IOCP_SIZE);
	//	memset(pIOSignleContext->m_wsaBuff.buf,0,IOCP_SIZE);
	//	memcpy(pIOSignleContext->m_wsaBuff.buf,pIODlg->MapFun[0].strdata,pIODlg->MapFun[0].datalen);
		memcpy(pIOSignleContext->m_Buff,pIODlg->MapFun[0].strdata,pIODlg->MapFun[0].datalen);
		pIOSignleContext->m_wsaBuff.len = pIODlg->MapFun[0].datalen;
		_PostSend(pIOSignleContext);
		LastSendTime = GetTickCount();


//		pIOSignleContext->m_iotype = IO_RECV;
//		pIOSignleContext->m_IOsocket = m_arrayclientcontext[j]->m_Socket;
//		_PostRecv(pIOSignleContext);
	}
	return true;
}

//工作者线程
DWORD WINAPI CIOFUN::_WorkerThread(LPVOID lparam)
{
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lparam;
	CIOFUN* pIOCPFun = (CIOFUN*)pParam->pIOCP;
	int nThreadNo = (int)pParam->nThreadno;

	printf("工作者线程启动，ID: %d.\n",nThreadNo);
	OVERLAPPED           *pOverlapped = NULL;
	PER_SOCKET_CONTEXT   *pSocketContext = NULL;
	DWORD                dwBytesTransfered = 0;
	//判断退出事件是否有信号
	while(WAIT_OBJECT_0 != WaitForSingleObject(pIOCPFun->m_ShutdownEvent,0))
	{
	//	printf("线程开始\n");
		//获取绑定的完成端口
		/*
		BOOL WINAPI GetQueuedCompletionStatus(  
        __in   HANDLE          CompletionPort,    // 这个就是我们建立的那个唯一的完成端口  
        __out  LPDWORD         lpNumberOfBytes,   //这个是操作完成后返回的字节数  
        __out  PULONG_PTR      lpCompletionKey,   // 这个是我们建立完成端口的时候绑定的那个自定义结构体参数  
        __out  LPOVERLAPPED    *lpOverlapped,     // 这个是我们在连入Socket的时候一起建立的那个重叠结构  
        __in   DWORD           dwMilliseconds     // 等待完成端口的超时时间，如果线程不需要做其他的事情，那就INFINITE就行了  
        );
	    */
		bool IOStatus = GetQueuedCompletionStatus(pIOCPFun->m_IOCompletionPort,&dwBytesTransfered,(PULONG_PTR)&pSocketContext,&pOverlapped,INFINITE);
		printf("当前线程号:%d\n",GetCurrentThreadId());
		//校验完成端口获取的PER_SOCKET_CONTEXT是否正常
		if((DWORD)pSocketContext == NULL)
		{
			printf("获取完成端口失败，退出线程\n");
			break;
		}
		if(!IOStatus)    //获取完成端口失败，释放相应资源
		{
			int errcode = WSAGetLastError();
			if(!(pIOCPFun->HandleError(pSocketContext,errcode)))
			{
				printf("不在此定义的错误信息内：%d\n",errcode);
				break;
			}
			continue;
		}
		else     //完成端口获取成功，进行相应的操作业务
		{
			/*
			#define CONTAINING_RECORD(address, type, field) ((type *)( \
    (PCHAR)(address) - \
    (ULONG_PTR)(&((type *)0)->field)))

用于取得内存中任何结构体的首地址，要提供的参数是：结构体中某个成员（field）的地址address、结构体的类型type、提供地址那个成员的名字field。 

(ULONG_PTR)(&((type *)0)->field)),它表示结构体type的field字段相对type首地址的内存地址偏移量，
也可以如此理解：结构体首地址从0开始数，field的偏移就是(ULONG_PTR)(&((type *)0)->field))

type结构体的field字段的地址减去field字段在结构体中的偏移。就是结构体的首地址

*/
			//读取IO_CONTEXT信息
			PER_IO_CONTEXT *pIOText = CONTAINING_RECORD(pOverlapped,PER_IO_CONTEXT,m_overlapped);
			//判断连接的socket是否有断开
			if((dwBytesTransfered == 0)&&(pIOText->m_iotype == IO_SEND||pIOText->m_iotype == IO_RECV))
			{
				printf("服务器断开连接：%d,收包值:%d,IO类型:%d\n",pIOText->m_IOsocket,dwBytesTransfered,pIOText->m_iotype);
				CYunTestApp *pApp = (CYunTestApp*)AfxGetApp();
				CYunTestDlg *pDlg = (CYunTestDlg*)pApp->m_pMainWnd;
				pDlg->AddNum(1,1);
				pDlg->AddNum(0,-1);
				pIOCPFun->_RemoveContext(pSocketContext);    //需要清除完成端口的socket数据
				break;
			}
			else
			{
				//判断获取的socket的状态
				switch(pIOText->m_iotype)
				{
				case IO_SEND:
					{
						pIOCPFun->_DoSend(pIOText);
					}break;
				case IO_RECV:
					{
						pIOCPFun->_DoRecv(pSocketContext,pIOText);
						printf("IO_recv\n");
					}break;
				default:
					{
						printf("线程参数没有此IOTYPE类型\n");
					}break;
				}
			}
		}
	}
	TRACE("线程指令退出\n");
	RELEASE(lparam);
	return 1;
}

// 投递接受PER_IO_CONTEXT信息
bool CIOFUN::_PostRecv(PER_IO_CONTEXT * pIO_Context)
{
	/*
	 int WSARecv(  
 2     SOCKET s,                      // 当然是投递这个操作的套接字  
 3      LPWSABUF lpBuffers,            // 接收缓冲区   
 4                                         // 这里需要一个由WSABUF结构构成的数组  
 5      DWORD dwBufferCount,           // 数组中WSABUF结构的数量，设置为1即可  
 6      LPDWORD lpNumberOfBytesRecvd,  // 如果接收操作立即完成，这里会返回函数调用所接收到的字节数  
 7      LPDWORD lpFlags,               // 说来话长了，我们这里设置为0 即可  
 8      LPWSAOVERLAPPED lpOverlapped,  // 这个Socket对应的重叠结构  
 9      NULL                           // 这个参数只有完成例程模式才会用到，  
10                                         // 完成端口中我们设置为NULL即可  
11 );  

         其实里面的参数，如果你们熟悉或者看过我以前的重叠I/O的文章，应该都比较熟悉，只需要注意其中的两个参数：

LPWSABUF lpBuffers;
        这里是需要我们自己new 一个 WSABUF 的结构体传进去的；

        如果你们非要追问 WSABUF 结构体是个什么东东？我就给各位多说两句，就是在ws2def.h中有定义的，定义如下：

         而且好心的微软还附赠了注释，真不容易….

         看到了吗？如果对于里面的一些奇怪符号你们看不懂的话，也不用管他，只用看到一个ULONG和一个CHAR*就可以了，这不就是一个是缓冲区长度，一个是缓冲区指针么？至于那个什么 FAR…..让他见鬼去吧，现在已经是32位和64位时代了……

        这里需要注意的，我们的应用程序接到数据到达的通知的时候，其实数据已经被咱们的主机接收下来了，我们直接通过这个WSABUF指针去系统缓冲区拿数据就好了，而不像那些没用重叠I/O的模型，接收到有数据到达的通知的时候还得自己去另外recv，太低端了……这也是为什么重叠I/O比其他的I/O性能要好的原因之一。

LPWSAOVERLAPPED lpOverlapped
         这个参数就是我们所谓的重叠结构了，就是这样定义，然后在有Socket连接进来的时候，生成并初始化一下，然后在投递第一个完成请求的时候，作为参数传递进去就可以，

        在第一个重叠请求完毕之后，我们的这个OVERLAPPED 结构体里，就会被分配有效的系统参数了，并且我们是需要每一个Socket上的每一个I/O操作类型，都要有一个唯一的Overlapped结构去标识。

        这样，投递一个WSARecv就讲完了，至于_DoRecv()需要做些什么呢？其实就是做两件事：

        (1) 把WSARecv里这个缓冲区里收到的数据显示出来；

        (2) 发出下一个WSARecv()；

        Over……

 

        至此，我们终于深深的喘口气了，完成端口的大部分工作我们也完成了，也非常感谢各位耐心的看我这么枯燥的文字一直看到这里，真是一个不容易的事情！！


		默认 socket 是阻塞的

解阻塞与非阻塞recv返回值没有区分，都是
<0 出错
=0 连接关闭
>0 接收到数据大小，

特别：返回值<0时并且(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)的情况下认为连接是正常的，继续接收。
只是阻塞模式下recv会阻塞着接收数据，非阻塞模式下如果没有数据会返回，不会阻塞着读，因此需要循环读取）。

返回说明：   
成功执行时，返回接收到的字节数。另一端已关闭则返回0。失败返回-1，errno被设为以下的某个值   
EAGAIN：套接字已标记为非阻塞，而接收操作被阻塞或者接收超时
EBADF：sock不是有效的描述词
ECONNREFUSE：远程主机阻绝网络连接
EFAULT：内存空间访问出错
EINTR：操作被信号中断
EINVAL：参数无效
ENOMEM：内存不足
ENOTCONN：与面向连接关联的套接字尚未被连接上
ENOTSOCK：sock索引的不是套接字

当返回值是0时，为正常关闭连接；

思考：
当对侧没有send，即本侧的套接字s的接收缓冲区无数据，返回值是什么
（EAGAIN，原因为超时，待测）
*/
//	EnterCriticalSection(&m_IOCS);           
    // 继续投递接收I/O请求
	pIO_Context->m_iotype = IO_RECV;
    DWORD nFlags = 0;
	DWORD recvBuflen = 0;
	WSABUF *m_wsarecvbuf = &(pIO_Context->m_wsaBuff);
    OVERLAPPED *p_recv = &(pIO_Context->m_overlapped);  
	pIO_Context->ResetBuff();
	int recvret = WSARecv(pIO_Context->m_IOsocket,m_wsarecvbuf,1,&recvBuflen,&nFlags,p_recv,NULL);
	if(recvret<0)
	{
		int retrecv = WSAGetLastError();
		if((recvret == SOCKET_ERROR)&&(retrecv != ERROR_IO_PENDING))
		{
			printf("PostRecv投递接收失败:d\n",retrecv);
			LeaveCriticalSection(&m_IOCS);
			CYunTestApp *ppSendApp = (CYunTestApp*)AfxGetApp();
			CYunTestDlg *ppSendDlg = (CYunTestDlg*)ppSendApp->m_pMainWnd;
			ppSendDlg->AddNum(5,1);
			return false;
		}
	}
	CYunTestApp *ppSendApp = (CYunTestApp*)AfxGetApp();
	CYunTestDlg *ppSendDlg = (CYunTestDlg*)ppSendApp->m_pMainWnd;
	ppSendDlg->AddNum(4,1);
//	LeaveCriticalSection(&m_IOCS);
	return true;
}


// 投递发送业务操作
bool CIOFUN::_PostSend(PER_IO_CONTEXT * pIOSendContext)
{
	CYunTestApp *ppSendApp = (CYunTestApp*)AfxGetApp();
	CYunTestDlg *ppSendDlg = (CYunTestDlg*)ppSendApp->m_pMainWnd;
	DWORD SendBytes = 0;
	WSABUF *wsaSendBuf = &pIOSendContext->m_wsaBuff;
	OVERLAPPED *p_ol = &pIOSendContext->m_overlapped;
	pIOSendContext->m_iotype = IO_SEND;
	ThisSendTime = GetTickCount();
	int nSendRet = WSASend(pIOSendContext->m_IOsocket,wsaSendBuf,1,&SendBytes,0,p_ol,NULL);
	if((nSendRet==SOCKET_ERROR)&&(ERROR_IO_PENDING != WSAGetLastError()))
	{
		int err = WSAGetLastError();
		printf("投递发送失败:%d\n",err);
		
		ppSendDlg->AddNum(3,1);
		return false;
	}
//	ppSendDlg->AddNum(2,(__int64)SendBytes);
	ppSendDlg->AddNum(2,1);
	LastSendTime = ThisSendTime;
	return true;
}



// 投递发送信息
bool CIOFUN::_DoSend(PER_IO_CONTEXT * pSendContext)
{
	//组发送IO的数据
	CYunTestApp *pApp = (CYunTestApp*)AfxGetApp();
	CYunTestDlg *pDlg = (CYunTestDlg*)pApp->m_pMainWnd;
	//获取功能信息数据到平台，扫描vector功能列表里面的每个功能号发送到后台服务
	int FunSize = pDlg->MapFun.size();
    if(pDlg->m_CheckAccount.GetCheck())               //单功能多账号
	{
		EnterCriticalSection(&m_IOCS);
		pDlg->GeneratePack(300);
		memset(pSendContext->m_Buff,0,IOCP_SIZE);
		memcpy(pSendContext->m_Buff,pDlg->SendBuffer,pDlg->Datalen);
		pSendContext->m_iotype = IO_SEND;
		pSendContext->m_wsaBuff.len = pDlg->Datalen;
		LeaveCriticalSection(&m_IOCS);
		_PostSend(pSendContext);
	}
	else
	{
	    EnterCriticalSection(&m_IOCS);
	    if(mapid>=FunSize)
	    {
		    mapid = 0;
		    memset(pSendContext->m_Buff,0,IOCP_SIZE);
		    memcpy(pSendContext->m_Buff,pDlg->MapFun[0].strdata,pDlg->MapFun[0].datalen);
		    pSendContext->m_iotype = IO_SEND;
		    pSendContext->m_wsaBuff.len = pDlg->MapFun[0].datalen;
	     //	printf("_DoSend-1发送数据包长度:%d,socket:%d,mapid:%d\n",pDlg->MapFun[0].datalen,pSendContext->m_IOsocket,mapid);
		    printf("0-发送字节数:%d\n",pDlg->MapFun[0].datalen);
		    mapid++;
		    LeaveCriticalSection(&m_IOCS);
		    return _PostSend(pSendContext);
	    }
	    else
	    {
		    memset(pSendContext->m_Buff,0,IOCP_SIZE);
		    memcpy(pSendContext->m_Buff,pDlg->MapFun[mapid].strdata,pDlg->MapFun[mapid].datalen);
		    pSendContext->m_iotype = IO_SEND;
		    pSendContext->m_wsaBuff.len = pDlg->MapFun[mapid].datalen;
	     //	printf("_DoSend发送数据包长度:%d,socket:%d,mapid:%d\n",pDlg->MapFun[mapid].datalen,pSendContext->m_IOsocket,mapid);
		    printf("1-发送字节数:%d\n",pDlg->MapFun[mapid].datalen);
		    mapid++;
		    LeaveCriticalSection(&m_IOCS);
		    return _PostSend(pSendContext);
	    }
	}
}


// 做接受投递业务操作
bool CIOFUN::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext,PER_IO_CONTEXT * pRecvContext)
{
	return _PostRecv(pRecvContext);
}


// 从数组里移除PER_SOCKET_CONTEXT信息
void CIOFUN::_RemoveContext(PER_SOCKET_CONTEXT* pSocketContext)
{
	EnterCriticalSection(&m_IOCS);
	for(int i=0; i<m_arrayclientcontext.GetCount(); i++)
	{
		if(pSocketContext == m_arrayclientcontext.GetAt(i))
		{
			m_arrayclientcontext.RemoveAt(i);
			RELEASE_SOCKET(pSocketContext->m_Socket);
			break;
		}
	}
	LeaveCriticalSection(&m_IOCS);
}

// 清空客户端信息
void CIOFUN::_ClearContextList(void)
{
	EnterCriticalSection(&m_IOCS);
	for(int i=0; i<m_arrayclientcontext.GetCount(); i++)
	{
		delete m_arrayclientcontext.GetAt(i);
	}
	m_arrayclientcontext.RemoveAll();
	LeaveCriticalSection(&m_IOCS);
}



// 处理完成端口上的错误信息
bool CIOFUN::HandleError(PER_SOCKET_CONTEXT* pSocContext,const DWORD& dwErr)
{
	if(dwErr == WAIT_TIMEOUT)
	{
		//判断客户端是否还存活状态
		CYunTestApp *pApp = (CYunTestApp*)AfxGetApp();
		CYunTestDlg *pDlg = (CYunTestDlg*)pApp->m_pMainWnd;
		char StressHeart[10] = {0};
		HQ_HEAD *pStressHeart = (HQ_HEAD*)StressHeart;
		pStressHeart->cSparate = '{';
		pStressHeart->attrs = 0;
		pStressHeart->length = 0;
		pStressHeart->type = 0;    //心跳包头
		StressHeart[7] = '\0';
		int nret = send(pSocContext->m_Socket,StressHeart,7,0);    //此数据包错误，应该发送心跳包
		if(nret == -1)
		{
			printf("客户端socket状态断开\n");
			this->_RemoveContext(pSocContext);
			return true;
		}
		else
		{
			printf("socket网络超时，请等待\n");
			return true;
		}
	}
	//检测客户端socket异常退出
	else if(dwErr == ERROR_NETNAME_DELETED)
	{
		printf("客户端信息异常退出\n");
		_RemoveContext(pSocContext);
		return true;
	}
	else
	{
		printf("检测到完成端口错误\n");
		return false;
	}
}

// 最后释放资源
void CIOFUN::_DeInitialize(void)
{
	//释放退出事件句柄
	RELEASE_HANDLE(m_ShutdownEvent);
	//释放每个线程句柄
	for(int i=0; i<nThreadnum; i++)
	{
		RELEASE_HANDLE(m_ThreadWorker[i]);
	}
	RELEASE(m_ThreadWorker);
	//释放完成端口句柄
	RELEASE_HANDLE(m_IOCompletionPort);
}