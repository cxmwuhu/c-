#include "YunTest.h"

#pragma once

//���建����buff��С
#define IOCP_SIZE   1024*300

//����IO״̬�ֶ�ö��ֵ
enum OPERATION_TYPE
{
	IO_SEND,    //����Ͷ�ݷ�����Ϣ״̬
	IO_RECV,    //����Ͷ�ݽ�����Ϣ״̬
	IO_NULL     //���ڳ�ʼ���ı�־��������ʵ������
};

//���嵥��IO����״̬
//====================================================================================
//
//				��IO���ݽṹ�嶨��(����ÿһ���ص������Ĳ���)
//
//====================================================================================
typedef struct _PER_IO_CONTEXT
{
	OVERLAPPED        m_overlapped;                 //�����ص���IO�ṹ��ÿ��socket�е�����һ���ص�io�ṹ
	SOCKET            m_IOsocket;                   //����ÿ���ص��ṹ��socket�׽���
	WSABUF            m_wsaBuff;                    //WSA���͵Ļ����������ڸ�ÿ���ص�ioʹ��
	char              m_Buff[IOCP_SIZE];           //WSABUF�����ŵĻ���������
	OPERATION_TYPE    m_iotype;                     //����ÿ���ص��ṹ�Ĳ�������
	int               m_Serial;                      //����ÿ��socket�ķ��Ͳ������

	//��ʼ����صĲ���,���캯����ʼ��
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

	//���������ͷ���Դ
	~_PER_IO_CONTEXT()
	{
		if(m_IOsocket!=INVALID_SOCKET)
		{
			closesocket(m_IOsocket);
			m_IOsocket = INVALID_SOCKET;
		}
	}

	//���û�����
	void  ResetBuff()
	{
		ZeroMemory(m_Buff,IOCP_SIZE);
	}

}PER_IO_CONTEXT, *PPER_IO_CONTEXT;

/*
//����ÿ���û���io��Ϣ

//====================================================================================
//
//				���û����ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================
typedef struct _PER_SIGNLE_CONTEXT
{
	UINT m_indexuser;	//�û�����
	PPER_IO_CONTEXT m_pSendPerIoContext;	//�û�����io
	PPER_IO_CONTEXT m_pRecvPerIoContext;	//�û�����io
	CRITICAL_SECTION m_csUser;				//ÿ�û��Ĺؼ������

	// ��ʼ��
	_PER_SIGNLE_CONTEXT()
	{
		m_indexuser=0;
		m_pSendPerIoContext=new PER_IO_CONTEXT;
		m_pRecvPerIoContext=new PER_IO_CONTEXT;
		InitializeCriticalSection(&m_csUser);
	}
	// �ͷŵ�Socket
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
//				��������ݽṹ�嶨��(����ÿһ����ɶ˿ڣ�Ҳ����ÿһ��Socket�Ĳ���)
//
//====================================================================================
typedef struct _PER_SOCKET_CONTEXT
{
	//����ÿ��io��socket
	SOCKET                            m_Socket;                   //�����������ӵ�socket
	int                               nindex;                       //����ÿ��socket�����
//	CArray<_PER_SIGNLE_CONTEXT*>          m_arrayContext;               //// �ͻ���������������������ݣ�
	                                                       // Ҳ����˵����ÿһ���ͻ���Socket���ǿ���������ͬʱͶ�ݶ��IO�����
	CArray<_PER_IO_CONTEXT*>          m_arrayContext;
	//��ʼ�����������캯����ʼ��
	_PER_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		nindex = 0;
	}

	//��ʼ����������
	~_PER_SOCKET_CONTEXT()
	{
		if(m_Socket!=INVALID_SOCKET)
		{
		    closesocket(m_Socket);
		}
		//�Ƴ�array����
		for(int i=0; i<m_arrayContext.GetCount(); i++)
		{
			delete []m_arrayContext.GetAt(i);
		}
		m_arrayContext.RemoveAll();
	}

	//��ȡһ���µ�IO����
//	_PER_SIGNLE_CONTEXT* GetNewIOContext()
	_PER_IO_CONTEXT* GetNewIOContext()
	{
	//	_PER_SIGNLE_CONTEXT* pSignleContext = new _PER_SIGNLE_CONTEXT();
		_PER_IO_CONTEXT* pIOContext = new _PER_IO_CONTEXT();
		m_arrayContext.Add(pIOContext);
		return pIOContext;
	}

	//��array������ȥ��һ��IOCONTEXT
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
	CIOFUN*    pIOCP;          //����CIOCP��ָ������ʹ���ຯ����������к���������
	int       nThreadno;      //�̱߳��
}THREADPARAMS_WORKER,*PTHREADPARAM_WORKER;

class CIOFUN
{
public:
	CIOFUN(void);
	~CIOFUN(void);
	bool Start(void);
	// ֹͣ������
	void Stop(void);

private:
	int                          mapid;                         //map���������id��

	CDialog*                     m_pMain;                       // ������Ľ���ָ�룬����������������ʾ��Ϣ

	//�����˳��̵߳��¼����,����֪ͨ�߳�ϵͳ�˳����¼���Ϊ���ܹ����õ��˳��߳�
	HANDLE                       m_ShutdownEvent;

	//������ɶ˿ھ��
	HANDLE                       m_IOCompletionPort;

	//���幤�����߳̾��
	HANDLE*                      m_ThreadWorker;

	int                          nThreadnum;               //���ɵ��߳�����

	char                         IOaddr[30];              //ѹ����ַ

	int                          IOport;                  //ѹ���˿�

	CCriticalSection            m_IOSection;             //�����̼߳�Ļ���������Է�ֹ�̳߳�ͻ������

	CRITICAL_SECTION             m_IOCS;                     //�����̼߳�Ļ���������Է�ֹ�̳߳�ͻ������

	CArray<PER_SOCKET_CONTEXT*>   m_arrayclientcontext;    //����ͻ���socket��context��������

	PER_SOCKET_CONTEXT*           pContext;                //��������IO_SOCKETָ��

	int                       LastSendTime;            //�����ϴη���ʱ�䣬û�е����ݰ�����ʱ������Ҫ����

	int                       ThisSendTime;            //���屾�δη���ʱ�䣬û�е����ݰ�����ʱ������Ҫ����

	int                           SendTickTime;            //���ͼ��ʱ�䣬��ȡ��������ѡ��
public:
	// ��ʼ��IO��Ϣ
	bool _InitializeIOCP(void);
	// ��ʼ������socket
	bool _InitializeSocket(void);

	//�������߳�
	static DWORD WINAPI _WorkerThread(LPVOID lparam);
	// Ͷ�ݽ���PER_IO_CONTEXT��Ϣ
	bool _PostRecv(PER_IO_CONTEXT * pIO_Context);
	// Ͷ�ݷ���ҵ�����
	bool _PostSend(PER_IO_CONTEXT * pIOSendContext);
	// Ͷ�ݷ�����Ϣ
	bool _DoSend(PER_IO_CONTEXT * pSendContext);
	// ������Ͷ��ҵ�����
	bool _DoRecv(PER_SOCKET_CONTEXT* pSocketContext,PER_IO_CONTEXT * pRecvContext);
	// ���������Ƴ�PER_SOCKET_CONTEXT��Ϣ
	void _RemoveContext(PER_SOCKET_CONTEXT* pSocketContext);
	// ��տͻ�����Ϣ
	void _ClearContextList(void);
	// ������ɶ˿��ϵĴ�����Ϣ
	bool HandleError(PER_SOCKET_CONTEXT* pContext,const DWORD& dwErr);
	// ����ͷ���Դ
	void _DeInitialize(void);
};

