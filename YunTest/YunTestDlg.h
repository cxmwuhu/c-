
// YunTestDlg.h : ͷ�ļ�
//
#include "MySocket.h"
#include "GetMac.h"
#include "RedisInfo.h"
#include "Control\CheckComboBox.h"
#include "LoadXml.h"
#include "GetSysInfomation.h"
#include "IOFUN.h"
#include "MutilThread.h"
#pragma once
#define BUFFER_SIZE  100*1024



// CYunTestDlg �Ի���
class CYunTestDlg : public CDialogEx
{
// ����
public:
	CYunTestDlg(CWnd* pParent = NULL);	// ��׼���캯��
// �Ի�������
	enum { IDD = IDD_YUNTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
public:
	//���巢�Ͱ�����������
	char SendBuffer[10*1024];

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();


public:
	CMySocket m_Mysock;
	CGetMac Mac;
	CRedisInfo m_redis;
	//����ؼ��ֶγ���
	CStatic m_Static[50];
	CEdit m_Edit[50];
	CLoadXml Xml;
	CGetSysInfomation CPUusage;
	
	
	//�����հ�����������
	char RecvBuff[100*1024];
	//���ݰ�����
	unsigned short Datalen;

	//����������Ϣ���̱߳���
	SOCKET send_Sock;
	//�ͻ�����Ӧ�¼�  
    WSAEVENT g_ClientEvent;  
    bool g_bClientClose;
	
	HANDLE hEvent;
	HANDLE m_Thread;
	//���ܻ�����
	char MsgBuffer[BUFFER_SIZE];
	int nsize;
	//�߳�ѭ����־���Ƿ�Ϊ��
	bool mark_thread;
	CRITICAL_SECTION  m_Section;
	CCriticalSection m_Section1;

	//��������߳̾��
	HANDLE m_AyncThread;
    HANDLE m_AyncEvent;
	bool aync_mark;
	HANDLE m_RecvSemaphore;
	HANDLE m_AyncSemaphore;

	//������������������Ϣ
	map<int,TSHEART> MapHeart;
	vector<TSHEART> vecHeart;
	int heartcount;       //����������
	int countdata;         //����304����������

	//������������������Ϣ
	map<int,NOTICEHEART> MapNotice;
	int noticecount;       //����������
	unsigned int noticeid;

	//ѹ����ز���
	vector<IODATA> iodata;
	std::string m_pressreqs[50];
	char m_pressreq[1024*5];
	CIOFUN m_ioFun;
	CMutilThread m_ThdData;
	//���˺Ź������
	int       accountcount;

	//ѹ��������ֵ����
	__int64 m_SendSNum;         //���ͳɹ���
	__int64 m_SendFNum;         //����ʧ����
	__int64 m_RecvSNum;         //���ܳɹ���
	__int64 m_RecvFNum;         //����ʧ����
	__int64     m_ConnSNum;         //���ӳɹ���
	__int64     m_ConnFNum;         //����ʧ����
	map<int,vector<IODATA>> mapFun;     //�鹦�ܺ�������ϵ�map������
	map<int,IODATA> MapFun;             //ѹ���������ݰ�

public:
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnRedis();
	afx_msg void OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEdit(UINT nID);
	CListCtrl m_ListCtl;

	CCheckComboBox m_ComBoxFun;
	afx_msg void OnCbnSelchangeComboFun();
	// ��������ʾ��edit�༭��
	void GenFunStr(int FunNo, CString & ReqString);
	afx_msg void OnBnClickedBtnSend();
	// ������������ݹ��ܺ�����������������ݶ�����������
	void GeneratePack(int FunId);
	// �������
	void AyncPack(char * &PackBody, unsigned short PackLen, unsigned short PackType);
	// �������
	void DealPack(char * & PackData, unsigned int PackLength, unsigned int PackType);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//����������Ϣ�̺߳���
	static DWORD WINAPI TUISONGMsg(LPVOID lpParam);
	//�������߳�
	static DWORD WINAPI AyncMsg(LPVOID lpParam);
	//�ڲ�������
	void DealMsg();
	afx_msg void OnBnClickedBtnPress();
	CButton m_CheckFun;
	CComboBox m_FunList;
	//�ӿڹ��ܺ��б�
	map<int,CString> mapport;
//	vector<FUNNO> vectorFun;
	// ͨ���ӿںż�����Ӧ�ӿڵ�xml�ļ�
	void GetFunXml(int funid);
	//���ܺţ�ͨ�����ܺż����ֵ��Լ����ͷ����
	unsigned short Fun;
	afx_msg void OnCbnSelchangeFunlist();
	// ���ͽ������
	void TSAnycPack(char *& PackData, unsigned short PackLen, unsigned short PackType, int recvflag);
protected:
	afx_msg LRESULT OnSocketmessage(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_TSList;
	CButton m_CheckAccount;
	// ѹ������ֵ
	void AddNum(__int64 Type, __int64 CountNum);
	afx_msg void OnBnClickedBtn300();
	CButton m_CheckPush;
};
