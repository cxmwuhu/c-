#include "HqSocket.h"
#pragma once

struct STATIC_DATA
{
	enum STK_TYPE
	{
		INDEX = 0,				//ָ��
		STOCK = 1,				//��Ʊ
		FUND = 2,				//����
		BOND = 3,				//ծȯ
		OTHER_STOCK = 4,		//������Ʊ
		OPTION = 5,				//ѡ��Ȩ
		EXCHANGE = 6,			//���
		FUTURE = 7,				//�ڻ�
		FTR_IDX = 8,			//��ָ
		RGZ = 9,				//�Ϲ�֤
		ETF = 10,				//ETF
		LOF = 11,				//LOF
		COV_BOND = 12,			//��תծ
		TRUST = 13,				//����
		WARRANT = 14,			//Ȩ֤
		REPO = 15,				//�ع�
		COMM = 16,				//��Ʒ�ֻ�
	};
	WORD	m_wStkID;			//���г���Ψһ��ʾ,�ڱ��г��ڵ����
	char	m_strLabel[10];		//����

	char	m_strName[32];		//����
	BYTE	m_cType;			//STK_TYPE
	BYTE	m_nPriceDigit;		//�۸���С�ֱ��ʣ��ǳ���Ҫ��ÿһ��DWORD���͵ļ۸�Ҫ����10^m_nPriceDigit���������ļ۸�
	short	m_nVolUnit;			//�ɽ�����λ��ÿһ�ɽ�����λ��ʾ���ٹ�
//	MWORD	m_mFloatIssued;		//��ͨ�ɱ�
//	MWORD	m_mTotalIssued;		//�ܹɱ�

	DWORD	m_dwLastClose;		//����
	DWORD	m_dwAdvStop;		//��ͣ
	DWORD	m_dwDecStop;		//��ͣ

	int		GetPriceMul() const;		//�۸��������m_nPriceDigit����
};

struct HQSTATIC
{
	char	m_strLabel[10];		//����
	DWORD	m_dwLastClose;		//����
	DWORD	m_dwAdvStop;		//��ͣ
	float   m_highstop;
	DWORD	m_dwDecStop;		//��ͣ
	float   m_lowstop;
};
//����������·��socket�ṹ��
struct SOCKET_INFO
{
	long      lidx;              //�������
	CHqSocket g_Sock;            //CHqSocket��
	SOCKET    m_Sock;            //�������ӵ�socket
	bool      g_busy;            //�жϸ�socket�Ƿ���æµ״̬
	BOOL      bConnected;        //�Ƿ������������ϵ�
};

#pragma pack(1)
struct ACC_CMDHEAD
{
	unsigned short wCmdType;
	unsigned short wAttr;
	unsigned int nLen;	
	unsigned int nExpandInfo;	//��չ��Ϣ��������������ݣ���ӦҪԭ������;
};

struct ACC_CMDHEADEX
{
	unsigned short wCmdType;
	unsigned short wAttr;
	unsigned int nLen;	
	unsigned __int64 nExpandInfo;	//��չ��Ϣ��������������ݣ���ӦҪԭ������;
};

struct BIG_HEAD
{
	unsigned char	cSparate;//�ָ�����'{', ':'��, ����0�͡�}��'H' tag
	unsigned short	type;
	unsigned short  attrs;
	unsigned short  length;
};

struct push_head
{
	unsigned short  sub_type;
	unsigned short  sub_attrs;          // ȡֵSubAttrsEnum
	unsigned short  sub_length;
	unsigned int    sub_extend;
};

struct ACC_SERVERLOGIN
{
	char m_cValid[8];		//��������֤��;
	unsigned int nSerId;	//������id����id������ip��ʾ;
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
	ACCCMD_KEEPALIVE = 0x8080,	//�������ݰ���ͨ�÷�����Ҳʹ�øýӿ�
	ACCCMD_SERVERLOGIN,			//Ӧ�÷�������½, ͨ�÷�����Ҳʹ�øýӿ�
	ACCCMD_REGISTER,			//�û�ע��
	ACCCMD_VALIDREGISTER,		//��֤ע��
	ACCCMD_USERLOGIN,			//�û���½
	ACCCMD_USERQUIT,			//�û��˳�
	ACCCMD_KICKUSER,			//�߳��û�
	ACCCMD_SELFSTK,				//ȡ���ߴ���ѡ��
	ACCCMD_RECOMFRIEND,			//�Ƽ�����
	ACCCMD_MODIFYTELECODE,		//�޸��ֻ�����
	ACCCMD_CORELEVEL,			//ȡ�û��ȼ��ͻ��� 10 a
	ACCCMD_DEFAULTRIGHT,		//ȡ�û�Ĭ��Ȩ��	b
	ACCCMD_VALIDDEFAULTRIGHT,	//ȡ�û���֤ע��ɹ���Ĭ��Ȩ��	c
	ACCCMD_USERNAMEMOBILELOGIN,	//�û��������ֻ������½	d
	ACCCMD_FETCHTELCODEID,		//ȡ�ֻ������id	e
	ACCCMD_SETUSERSHORTMSG,		//�����û���Ʊ�ض������ѹ��� f
	ACCCMD_FETCHPOOLNEWSTK,		//ȡ����صĹ�Ʊ
	ACCCMD_SETPREWARNING,		//���ù�ƱԤ����Ϣ
	ACCCMD_COMMINTERFACE,		// �ͻ��˺ͷ�������ͨ�ýӿ�
	ACCCMD_LOGINNOTIFY,	//�û���½��֪ͨ����
	ACCCMD_STATCOMMINTERFACE=0xA0A0,		// ͳ���û���Ϣͨ�ýӿ�
	ACCCMD_SMSREG = 0x9001,		//����ע��
	ACCCMD_SYNCSUBS = 0x9002,		//ͬ����ϵȷ������
	ACCCMD_USERONLINE = 0x9011,	//�û�������
	ACCCMD_THREAD_TPYE = 0x9999,	//�ܵ�����
	ACCCMD_END,	//����,����ĩλ,�������ж�ָ��ķ�Χ
};
#pragma pack()

class CMutilThread
{
public:
	CMutilThread(void);
	~CMutilThread(void);

public:
	FILE *fptext;
	// ��ȡ�������ļ�
	void ReadFile(char * filename, char *MarketCode);
	int  socketcount;                   //socket����
	map<int,SOCKET_INFO> Socket_map;
	vector<SOCKET_INFO> vecSock;

//	char GenBuff[1024*5];
//	int  packoflen;

	CHqSocket  g_SockClass;
	//��̬�������
	map<int,HQSTATIC> HqData;
	map<int,HQSTATIC> SHHqData;
	map<int,HQSTATIC> SZHqData;
	map<int,HQSTATIC> HKHqData;
	//300���ܰ��û����ͱ�ʾ
//	map<int,int> UserID;
	map<int,string> UserID;
	int userid;         //�û���ţ����ڱ��������û�
	int stockid;        //��Ʊ������ţ����ڱ������д���
	// �������̵߳ĺ���
	void CreateThreadMash(void);

	HANDLE     *socket_thread;

	//����socket�߳̾��
	HANDLE     SocketHandle;

	HANDLE     m_SOCKSemp;   //�������ź�����������socket

	int CPUnum;
	//socket����
	int       Socket_Count;
	//����socket�̴߳�����
	bool p_sockstop;
	static DWORD WINAPI _SocketThread(LPVOID lparam);

	//�˳��߳��¼�
	HANDLE  ShutdownEvent;
	static DWORD WINAPI _NetThread(LPVOID lparam);

	HANDLE g_Event;   //��socket������ɺ�Ŵ���socket�����߳�

	bool g_mutilstop;
	// �����������̴߳���socket�߳�
	void CreateSocket(void);

	//�ṹ��֮��Ļ�����
	CCriticalSection    g_mutil;
	//�߳���
	CCriticalSection    g_threadlock;
	// ��ʼ��map�ṹ����Ϣ
	bool InitMap(void);
	// �ͷ�map�ṹ����Ϣ
	void Uninit(void);
	// �ͷ�ĳ������
	void DisConnectSvr(SOCKET_INFO * LINK_INFO);
	// ��ȡ����socket�ѷ�������
	SOCKET_INFO* GetLink(void);
	// ��������Ӻ��ͷŴ˴�����
	void SetFreeLink(SOCKET_INFO * SockData);
	//300���Ԥ�����ܺ���
//	bool Generate300Pack(int PackType);
	bool Generate300Pack(int PackType, char *GenBuff, int *buffoflen);
	// ����û���map������
	void AddUser(void);
	// ����socket
	int GenerateSock(void);
};

