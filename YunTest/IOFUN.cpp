#include "StdAfx.h"
#include "IOFUN.h"
#include "YunTestDlg.h"


// �ͷ�ָ��;����Դ�ĺ�

// �ͷ�ָ���
#define RELEASE(x)                      {if(x != NULL ){delete x;x=NULL;}}
// �ͷž����
#define RELEASE_HANDLE(x)               {if(x != NULL && x!=INVALID_HANDLE_VALUE){ CloseHandle(x);x = NULL;}}
// �ͷ�Socket��
#define RELEASE_SOCKET(x)               {if(x !=INVALID_SOCKET) { closesocket(x);x=INVALID_SOCKET;}}

#pragma pack(1)
 //���������ͷ
 struct HQ_HEAD
 {
	 unsigned char	cSparate;//�ָ�����'{', ':'��, ����0�͡�}��'H' tag
	 unsigned short	type;
	 unsigned short  attrs;
	 unsigned short  length;
 };

 //���Ͱ�ͷ
struct sub_head
{
    unsigned short  sub_type;
    unsigned short  sub_attrs;          // ȡֵSubAttrsEnum
    unsigned short  sub_length;
    unsigned int    sub_extend;
};
 //������뷽ʽ
#pragma pack()

CIOFUN::CIOFUN(void):
	m_ShutdownEvent(NULL),
	m_IOCompletionPort(NULL),
	m_ThreadWorker(NULL),
	m_pMain(NULL)
{
	nThreadnum = 0;               //���ɵ��߳�����
	IOport = 0;                  //ѹ���˿�
	LastSendTime = 0;            //�����ϴη���ʱ�䣬û�е����ݰ�����ʱ������Ҫ����
	ThisSendTime = 0;            //���屾�δη���ʱ�䣬û�е����ݰ�����ʱ������Ҫ����
	SendTickTime = 0;            //���ͼ��ʱ�䣬��ȡ��������ѡ��
	mapid = 0;
}


CIOFUN::~CIOFUN(void)
{
	this->Stop();
}


bool CIOFUN::Start(void)
{
	InitializeCriticalSection(&m_IOCS);
	//����ϵͳ�˳��¼�
	m_ShutdownEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		/*
	HANDLE CreateEvent(
LPSECURITY_ATTRIBUTES lpEventAttributes,
BOOL bManualReset, 
BOOL bInitialState,
LPCSTR lpName
);
bManualReset:TRUE��ʹ��ResetEvent()�ֶ�����Ϊ���ź�״̬��FALSE����һ���ȴ��̱߳��ͷ�ʱ,�Զ�����״̬Ϊ���ź�״̬��
bInitialState��ָ���¼�����ĳ�ʼ״̬����TRUE,��ʼ״̬Ϊ���ź�״̬����FALSE,��ʼ״̬Ϊ���ź�״̬��
������Ҫ��ʾһ�²���CreateEventʵ�ֶ��̡߳�
���Ӻܼ򵥣���Ҫ����CreateEvent��bManualReset:��bInitialState������ȡֵ���̵߳������ź�״̬�������
*/
	//��ʼ��ioϵͳ
	if(_InitializeIOCP() == false)
	{
		printf("��ʼ��io�ṹʧ��:%d\n",WSAGetLastError());
		return false;
	}
	if(_InitializeSocket() == false)
	{
		printf("��ʼ������socketʧ��:%d\n",WSAGetLastError());
		return false;
	}
	return true;
}


// ֹͣ������
void CIOFUN::Stop(void)
{
	//���˳��¼�����Ϊ���ź��¼�
	if(m_ShutdownEvent)
	{
	    SetEvent(m_ShutdownEvent);
	    //�˳���ɶ˿ڣ��Ա����߳�ȫ����ȫ�˳�

		/*
		PostQueuedCompletionStatus��������ÿ���������̶߳����͡��������������ݰ����ú�����ָʾÿ���̶߳��������������˳���.������PostQueuedCompletionStatus�����Ķ��壺 
BOOL PostQueuedCompletionStatus( 
    HANDLE CompletlonPort, 
    DW0RD  dwNumberOfBytesTrlansferred, 
    DWORD  dwCompletlonKey, 
LPOVERLAPPED lpoverlapped, 

); 

���У�CompletionPort����ָ�������䷢��һ��������ݰ�����ɶ˿ڶ��󡣶���dwNumberOfBytesTransferred,dwCompletionKey��lpOverlapped������������˵.
ÿ��������������ָ������ֵ,ֱ�Ӵ��ݸ�GetQueuedCompletionStatus�����ж�Ӧ�Ĳ��������������������������߳��յ����ݹ���������GetQueuedCompletionStatus����������
��ɸ�����������������ĳһ�����õ�����ֵ��������ʱӦ���˳�������,����dwCompletionPort��������0ֵ,�������������̻߳Ὣ����ͳ���ָֹ�һ�����й������̶߳��ѹر�,
���ʹ��CloseHandle����,�ر���ɶ˿ڡ����հ�ȫ�˳����� 
PostQueuedCompletionStatus�����ṩ��һ�ַ�ʽ�����̳߳��е������߳̽���ͨ�š��磬���û���ֹ����Ӧ�ó���ʱ��������Ҫ�����̶߳���ȫ�������˳���
����������̻߳��ڵȴ���ɶ˿ڶ���û������ɵ�I/O ������ô���ǽ��޷������ѡ� 
ͨ��Ϊ�̳߳��е�ÿ���̶߳�����һ��PostQueuedCompletionStatus�����ǿ��Խ����Ƕ����ѡ�
ÿ���̻߳��GetQueuedCompletionStatus�ķ���ֵ���м�飬�������Ӧ�ó���������ֹ����ô���ǾͿ��Խ������������������˳�
*/

	    for(int i=0; i<nThreadnum; i++)
	    {
		    PostQueuedCompletionStatus(m_IOCompletionPort,0,(DWORD)NULL,NULL);
	    }
	    //�ȴ����еĿͻ�����Դ�˳�
	    WaitForMultipleObjects(nThreadnum,m_ThreadWorker,TRUE,INFINITE);
	    //����ͻ����б�
	    this->_ClearContextList();
	    //�ͷ���Դ
	    this->_DeInitialize();
		mapid = 0;
		DeleteCriticalSection(&m_IOCS);
	    printf("ֹͣ��ɶ˿ڲ������\n");
	}
}


// ��ʼ��IO��Ϣ
bool CIOFUN::_InitializeIOCP(void)
{
	//������ɶ˿�
	/*
	HANDLE WINAPI CreateIoCompletionPort(
  __in          HANDLE FileHandle,
  __in          HANDLE ExistingCompletionPort,
  __in          ULONG_PTR CompletionKey,
  __in          DWORD NumberOfConcurrentThreads
);
һ��I/O��ɶ˿ڹ���һ�������ļ������Ҳ�����ڴ���I/O��ɶ˿ڵ�ʱ�� û�й����κ��ļ������
һ��I/O��ɶ˿ڹ���һ�����ļ���ʵ��ʹӦ�ó�������յ���������ļ��첽I/O���������֪ͨ��

������
FileHandle ���ص�IO��ɶ˿ڵ��ļ����
��������������ΪINVALID_HANDLE_VALUE���� ô��CreateIoCompletionPort �ᴴ��һ���������κ��ļ�����ɶ˿ڣ�����ExistingCompletionPort ��������ΪNULL,CompletionKey Ҳ�������ԡ�

ExistingCompletionPort  ��ɶ˿ھ��
���ָ��һ���Ѿ����ڵ���ɶ˿ڣ�����������FileHandle ָ�����ļ����������Ѵ��ڵ���ɶ˿ھ�����������ᴴ��һ���µ���ɶ˿ڡ�
����������ΪNULL����������һ����FileHandleָ������ ����������ɶ˿ڣ�������һ���µ���ɶ˿ھ����

CompletionKey ���ļ����������ָ���ļ�ÿ��IO��ɰ�������Ϣ��

NumberOfConcurrentThreads ϵͳ��������ɶ˿��ϲ�������IO��ɰ�������߳�������
��� ExistingCompletionPort ΪNULL���˲�������

����ֵ��
���ִ�гɹ����������ع���ָ���ļ�����ɶ˿ھ�������򣬷���NULL

��ע��
CreateIoCompletionPort �ṩ������ܣ�I/Oϵͳ���Ա��������жӵ�I/O��ɶ˿ڷ���I/O���֪ͨ����
�� ��ִ��һ���Ѿ�����һ����ɶ˿ڵ��ļ�I/O������I/Oϵͳ���������I/O������ɵ�ʱ����I/O��ɶ˿ڷ���һ�����֪ͨ����I/O��ɶ˿ڽ����� ���ȳ��ķ�ʽ�������I/O���֪ͨ������ʹ��GetQueuedCompletionStatus ����I/O���֪ͨ����
��Ȼ�����κ������� �߳�������GetQueuedCompletionStatus �ȴ�һ��I/O��ɶ˿ڣ���ÿ���߳�ֻ��ͬʱ���ڹ���һ��I/O��ɶ˿ڣ��Ҵ˶˿����߳��������Ǹ��˿ڡ�
��һ��������������У�ϵͳ���Ȼ� ����ж��ٸ������˶˿ڵ��߳������У�������е��̵߳���������NumberOfConcurrentThreads��ֵ��
��ô�������е�һ���� ���߳�ȥ���������һ�����е��߳���ɴ������ٴε���GetQueuedCompletionStatus ����ʱϵͳ������һ���ȴ��߳�ȥ�������
ϵ ͳҲ����һ���ȴ����̴߳����������е��߳̽����κ���ʽ�ĵȴ�״̬��������̴߳ӵȴ�״̬��������״̬�����ܻ���һ���̵ܶ�ʱ�ڻ�̵߳������ᳬ�� NumberOfConcurrentThreads ��ֵ����ʱ��ϵͳ��ͨ���������κ��µĻ�߳̿��ٵļ����̸߳�����ֱ����߳�����NumberOfConcurrentThreads ��ֵ��
*/
	m_IOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,0,0);
	if(m_IOCompletionPort == NULL)
	{
		printf("��ɶ˿ڴ���ʧ��:%d\n",WSAGetLastError());
		return false;
	}
	//�����������̺߳��������������̸߳���
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
		printf("IO��ɶ˿ڹ������̴߳����ɹ�:%d\n",ThreadID);
	}
	return true;
}


// ��ʼ������socket
bool CIOFUN::_InitializeSocket(void)
{
		//��ʼ���׽���
	WORD nRequestVer;
	nRequestVer = MAKEWORD(2,2);
	WSAData wsaData;
	DWORD ret;
	ret = WSAStartup(nRequestVer,&wsaData);
	if(ret != 0)
	{
		printf("��ʼ���׽���ʧ��:%d\n",WSAGetLastError());
		return false;
	}
	CString linknum = _T("");

	CYunTestDlg *pio = (CYunTestDlg*)AfxGetMainWnd();
	pio->GetDlgItem(IDC_EDIT_NUMBER)->GetWindowText(linknum);
	int m_Conncount = _ttoi(linknum);
	//��ȡip��ַ
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
	
	//��ȡ���ͼ��ʱ��
	SendTickTime = pio->GetDlgItemInt(IDC_EDIT_SECOND);
	//����socket��ַ��װ
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
			printf("WSASocket()����ʧ��:%d\n",WSAGetLastError());
		}
		else
		{
			printf("WSASocket�����ɹ�:%d\n",clientsocket);
		}
		WSAIoctl(clientsocket,FIONBIO,NULL,0,NULL,0,NULL,NULL,NULL);
		int retconn = connect(clientsocket,(sockaddr*)&sockclientaddr,sizeof(sockclientaddr));
		if(retconn == 0)
		{
			printf("socket���ӳɹ�:%d\n",clientsocket);
			pPer_Socket_Context = new PER_SOCKET_CONTEXT;

			pPer_Socket_Context->nindex = i;
			pPer_Socket_Context->m_Socket = clientsocket;
			
			//1��socket�󶨵���ɶ˿���
			HANDLE tempio = ::CreateIoCompletionPort((HANDLE)pPer_Socket_Context->m_Socket,m_IOCompletionPort,(DWORD)pPer_Socket_Context,0);
			if(tempio == NULL)
			{
				printf("��ɶ˿ڴ���ʧ��:%d\n",WSAGetLastError());
				
				RELEASE_HANDLE(pPer_Socket_Context);
			}
			//1�Ѵ����õ���ɶ˿�����PER_SOCKET_CONTEXT�ӵ�array�б���
			//1��Ҫ�������Է�ֹ������������ȡ����ʱ���ִ���
			EnterCriticalSection(&m_IOCS);
			m_arrayclientcontext.Add(pPer_Socket_Context);
			LeaveCriticalSection(&m_IOCS);
			pio->AddNum(0,1);
		}
	}
	for(int j=0; j<m_arrayclientcontext.GetCount(); j++)
	{
		//1Ͷ�ݽ��հ�
		pContext = m_arrayclientcontext.GetAt(j);
		pIOSignleContext = pContext->GetNewIOContext();
		pIOSignleContext->m_iotype = IO_RECV;
		pIOSignleContext->m_IOsocket = pContext->m_Socket;
		pIOSignleContext->m_IOsocket = m_arrayclientcontext[j]->m_Socket;
		_PostRecv(pIOSignleContext);

		printf("���͵�һ����\n");
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

//�������߳�
DWORD WINAPI CIOFUN::_WorkerThread(LPVOID lparam)
{
	THREADPARAMS_WORKER* pParam = (THREADPARAMS_WORKER*)lparam;
	CIOFUN* pIOCPFun = (CIOFUN*)pParam->pIOCP;
	int nThreadNo = (int)pParam->nThreadno;

	printf("�������߳�������ID: %d.\n",nThreadNo);
	OVERLAPPED           *pOverlapped = NULL;
	PER_SOCKET_CONTEXT   *pSocketContext = NULL;
	DWORD                dwBytesTransfered = 0;
	//�ж��˳��¼��Ƿ����ź�
	while(WAIT_OBJECT_0 != WaitForSingleObject(pIOCPFun->m_ShutdownEvent,0))
	{
	//	printf("�߳̿�ʼ\n");
		//��ȡ�󶨵���ɶ˿�
		/*
		BOOL WINAPI GetQueuedCompletionStatus(  
        __in   HANDLE          CompletionPort,    // ����������ǽ������Ǹ�Ψһ����ɶ˿�  
        __out  LPDWORD         lpNumberOfBytes,   //����ǲ�����ɺ󷵻ص��ֽ���  
        __out  PULONG_PTR      lpCompletionKey,   // ��������ǽ�����ɶ˿ڵ�ʱ��󶨵��Ǹ��Զ���ṹ�����  
        __out  LPOVERLAPPED    *lpOverlapped,     // ���������������Socket��ʱ��һ�������Ǹ��ص��ṹ  
        __in   DWORD           dwMilliseconds     // �ȴ���ɶ˿ڵĳ�ʱʱ�䣬����̲߳���Ҫ�����������飬�Ǿ�INFINITE������  
        );
	    */
		bool IOStatus = GetQueuedCompletionStatus(pIOCPFun->m_IOCompletionPort,&dwBytesTransfered,(PULONG_PTR)&pSocketContext,&pOverlapped,INFINITE);
		printf("��ǰ�̺߳�:%d\n",GetCurrentThreadId());
		//У����ɶ˿ڻ�ȡ��PER_SOCKET_CONTEXT�Ƿ�����
		if((DWORD)pSocketContext == NULL)
		{
			printf("��ȡ��ɶ˿�ʧ�ܣ��˳��߳�\n");
			break;
		}
		if(!IOStatus)    //��ȡ��ɶ˿�ʧ�ܣ��ͷ���Ӧ��Դ
		{
			int errcode = WSAGetLastError();
			if(!(pIOCPFun->HandleError(pSocketContext,errcode)))
			{
				printf("���ڴ˶���Ĵ�����Ϣ�ڣ�%d\n",errcode);
				break;
			}
			continue;
		}
		else     //��ɶ˿ڻ�ȡ�ɹ���������Ӧ�Ĳ���ҵ��
		{
			/*
			#define CONTAINING_RECORD(address, type, field) ((type *)( \
    (PCHAR)(address) - \
    (ULONG_PTR)(&((type *)0)->field)))

����ȡ���ڴ����κνṹ����׵�ַ��Ҫ�ṩ�Ĳ����ǣ��ṹ����ĳ����Ա��field���ĵ�ַaddress���ṹ�������type���ṩ��ַ�Ǹ���Ա������field�� 

(ULONG_PTR)(&((type *)0)->field)),����ʾ�ṹ��type��field�ֶ����type�׵�ַ���ڴ��ַƫ������
Ҳ���������⣺�ṹ���׵�ַ��0��ʼ����field��ƫ�ƾ���(ULONG_PTR)(&((type *)0)->field))

type�ṹ���field�ֶεĵ�ַ��ȥfield�ֶ��ڽṹ���е�ƫ�ơ����ǽṹ����׵�ַ

*/
			//��ȡIO_CONTEXT��Ϣ
			PER_IO_CONTEXT *pIOText = CONTAINING_RECORD(pOverlapped,PER_IO_CONTEXT,m_overlapped);
			//�ж����ӵ�socket�Ƿ��жϿ�
			if((dwBytesTransfered == 0)&&(pIOText->m_iotype == IO_SEND||pIOText->m_iotype == IO_RECV))
			{
				printf("�������Ͽ����ӣ�%d,�հ�ֵ:%d,IO����:%d\n",pIOText->m_IOsocket,dwBytesTransfered,pIOText->m_iotype);
				CYunTestApp *pApp = (CYunTestApp*)AfxGetApp();
				CYunTestDlg *pDlg = (CYunTestDlg*)pApp->m_pMainWnd;
				pDlg->AddNum(1,1);
				pDlg->AddNum(0,-1);
				pIOCPFun->_RemoveContext(pSocketContext);    //��Ҫ�����ɶ˿ڵ�socket����
				break;
			}
			else
			{
				//�жϻ�ȡ��socket��״̬
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
						printf("�̲߳���û�д�IOTYPE����\n");
					}break;
				}
			}
		}
	}
	TRACE("�߳�ָ���˳�\n");
	RELEASE(lparam);
	return 1;
}

// Ͷ�ݽ���PER_IO_CONTEXT��Ϣ
bool CIOFUN::_PostRecv(PER_IO_CONTEXT * pIO_Context)
{
	/*
	 int WSARecv(  
 2     SOCKET s,                      // ��Ȼ��Ͷ������������׽���  
 3      LPWSABUF lpBuffers,            // ���ջ�����   
 4                                         // ������Ҫһ����WSABUF�ṹ���ɵ�����  
 5      DWORD dwBufferCount,           // ������WSABUF�ṹ������������Ϊ1����  
 6      LPDWORD lpNumberOfBytesRecvd,  // ������ղ���������ɣ�����᷵�غ������������յ����ֽ���  
 7      LPDWORD lpFlags,               // ˵�������ˣ�������������Ϊ0 ����  
 8      LPWSAOVERLAPPED lpOverlapped,  // ���Socket��Ӧ���ص��ṹ  
 9      NULL                           // �������ֻ���������ģʽ�Ż��õ���  
10                                         // ��ɶ˿�����������ΪNULL����  
11 );  

         ��ʵ����Ĳ��������������Ϥ���߿�������ǰ���ص�I/O�����£�Ӧ�ö��Ƚ���Ϥ��ֻ��Ҫע�����е�����������

LPWSABUF lpBuffers;
        ��������Ҫ�����Լ�new һ�� WSABUF �Ľṹ�崫��ȥ�ģ�

        ������Ƿ�Ҫ׷�� WSABUF �ṹ���Ǹ�ʲô�������Ҿ͸���λ��˵���䣬������ws2def.h���ж���ģ��������£�

         ���Һ��ĵ�΢��������ע�ͣ��治���ס�.

         ��������������������һЩ��ַ������ǿ������Ļ���Ҳ���ù�����ֻ�ÿ���һ��ULONG��һ��CHAR*�Ϳ����ˣ��ⲻ����һ���ǻ��������ȣ�һ���ǻ�����ָ��ô�������Ǹ�ʲô FAR��..��������ȥ�ɣ������Ѿ���32λ��64λʱ���ˡ���

        ������Ҫע��ģ����ǵ�Ӧ�ó���ӵ����ݵ����֪ͨ��ʱ����ʵ�����Ѿ������ǵ��������������ˣ�����ֱ��ͨ�����WSABUFָ��ȥϵͳ�����������ݾͺ��ˣ���������Щû���ص�I/O��ģ�ͣ����յ������ݵ����֪ͨ��ʱ�򻹵��Լ�ȥ����recv��̫�Ͷ��ˡ�����Ҳ��Ϊʲô�ص�I/O��������I/O����Ҫ�õ�ԭ��֮һ��

LPWSAOVERLAPPED lpOverlapped
         �����������������ν���ص��ṹ�ˣ������������壬Ȼ������Socket���ӽ�����ʱ�����ɲ���ʼ��һ�£�Ȼ����Ͷ�ݵ�һ����������ʱ����Ϊ�������ݽ�ȥ�Ϳ��ԣ�

        �ڵ�һ���ص��������֮�����ǵ����OVERLAPPED �ṹ����ͻᱻ������Ч��ϵͳ�����ˣ�������������Ҫÿһ��Socket�ϵ�ÿһ��I/O�������ͣ���Ҫ��һ��Ψһ��Overlapped�ṹȥ��ʶ��

        ������Ͷ��һ��WSARecv�ͽ����ˣ�����_DoRecv()��Ҫ��Щʲô�أ���ʵ�����������£�

        (1) ��WSARecv��������������յ���������ʾ������

        (2) ������һ��WSARecv()��

        Over����

 

        ���ˣ�������������Ĵ������ˣ���ɶ˿ڵĴ󲿷ֹ�������Ҳ����ˣ�Ҳ�ǳ���л��λ���ĵĿ�����ô���������һֱ�����������һ�������׵����飡��


		Ĭ�� socket ��������

�������������recv����ֵû�����֣�����
<0 ����
=0 ���ӹر�
>0 ���յ����ݴ�С��

�ر𣺷���ֵ<0ʱ����(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)���������Ϊ�����������ģ��������ա�
ֻ������ģʽ��recv�������Ž������ݣ�������ģʽ�����û�����ݻ᷵�أ����������Ŷ��������Ҫѭ����ȡ����

����˵����   
�ɹ�ִ��ʱ�����ؽ��յ����ֽ�������һ���ѹر��򷵻�0��ʧ�ܷ���-1��errno����Ϊ���µ�ĳ��ֵ   
EAGAIN���׽����ѱ��Ϊ�������������ղ������������߽��ճ�ʱ
EBADF��sock������Ч��������
ECONNREFUSE��Զ�����������������
EFAULT���ڴ�ռ���ʳ���
EINTR���������ź��ж�
EINVAL��������Ч
ENOMEM���ڴ治��
ENOTCONN�����������ӹ������׽�����δ��������
ENOTSOCK��sock�����Ĳ����׽���

������ֵ��0ʱ��Ϊ�����ر����ӣ�

˼����
���Բ�û��send����������׽���s�Ľ��ջ����������ݣ�����ֵ��ʲô
��EAGAIN��ԭ��Ϊ��ʱ�����⣩
*/
//	EnterCriticalSection(&m_IOCS);           
    // ����Ͷ�ݽ���I/O����
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
			printf("PostRecvͶ�ݽ���ʧ��:d\n",retrecv);
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


// Ͷ�ݷ���ҵ�����
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
		printf("Ͷ�ݷ���ʧ��:%d\n",err);
		
		ppSendDlg->AddNum(3,1);
		return false;
	}
//	ppSendDlg->AddNum(2,(__int64)SendBytes);
	ppSendDlg->AddNum(2,1);
	LastSendTime = ThisSendTime;
	return true;
}



// Ͷ�ݷ�����Ϣ
bool CIOFUN::_DoSend(PER_IO_CONTEXT * pSendContext)
{
	//�鷢��IO������
	CYunTestApp *pApp = (CYunTestApp*)AfxGetApp();
	CYunTestDlg *pDlg = (CYunTestDlg*)pApp->m_pMainWnd;
	//��ȡ������Ϣ���ݵ�ƽ̨��ɨ��vector�����б������ÿ�����ܺŷ��͵���̨����
	int FunSize = pDlg->MapFun.size();
    if(pDlg->m_CheckAccount.GetCheck())               //�����ܶ��˺�
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
	     //	printf("_DoSend-1�������ݰ�����:%d,socket:%d,mapid:%d\n",pDlg->MapFun[0].datalen,pSendContext->m_IOsocket,mapid);
		    printf("0-�����ֽ���:%d\n",pDlg->MapFun[0].datalen);
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
	     //	printf("_DoSend�������ݰ�����:%d,socket:%d,mapid:%d\n",pDlg->MapFun[mapid].datalen,pSendContext->m_IOsocket,mapid);
		    printf("1-�����ֽ���:%d\n",pDlg->MapFun[mapid].datalen);
		    mapid++;
		    LeaveCriticalSection(&m_IOCS);
		    return _PostSend(pSendContext);
	    }
	}
}


// ������Ͷ��ҵ�����
bool CIOFUN::_DoRecv(PER_SOCKET_CONTEXT* pSocketContext,PER_IO_CONTEXT * pRecvContext)
{
	return _PostRecv(pRecvContext);
}


// ���������Ƴ�PER_SOCKET_CONTEXT��Ϣ
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

// ��տͻ�����Ϣ
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



// ������ɶ˿��ϵĴ�����Ϣ
bool CIOFUN::HandleError(PER_SOCKET_CONTEXT* pSocContext,const DWORD& dwErr)
{
	if(dwErr == WAIT_TIMEOUT)
	{
		//�жϿͻ����Ƿ񻹴��״̬
		CYunTestApp *pApp = (CYunTestApp*)AfxGetApp();
		CYunTestDlg *pDlg = (CYunTestDlg*)pApp->m_pMainWnd;
		char StressHeart[10] = {0};
		HQ_HEAD *pStressHeart = (HQ_HEAD*)StressHeart;
		pStressHeart->cSparate = '{';
		pStressHeart->attrs = 0;
		pStressHeart->length = 0;
		pStressHeart->type = 0;    //������ͷ
		StressHeart[7] = '\0';
		int nret = send(pSocContext->m_Socket,StressHeart,7,0);    //�����ݰ�����Ӧ�÷���������
		if(nret == -1)
		{
			printf("�ͻ���socket״̬�Ͽ�\n");
			this->_RemoveContext(pSocContext);
			return true;
		}
		else
		{
			printf("socket���糬ʱ����ȴ�\n");
			return true;
		}
	}
	//���ͻ���socket�쳣�˳�
	else if(dwErr == ERROR_NETNAME_DELETED)
	{
		printf("�ͻ�����Ϣ�쳣�˳�\n");
		_RemoveContext(pSocContext);
		return true;
	}
	else
	{
		printf("��⵽��ɶ˿ڴ���\n");
		return false;
	}
}

// ����ͷ���Դ
void CIOFUN::_DeInitialize(void)
{
	//�ͷ��˳��¼����
	RELEASE_HANDLE(m_ShutdownEvent);
	//�ͷ�ÿ���߳̾��
	for(int i=0; i<nThreadnum; i++)
	{
		RELEASE_HANDLE(m_ThreadWorker[i]);
	}
	RELEASE(m_ThreadWorker);
	//�ͷ���ɶ˿ھ��
	RELEASE_HANDLE(m_IOCompletionPort);
}