
// YunTestDlg.h : 头文件
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



// CYunTestDlg 对话框
class CYunTestDlg : public CDialogEx
{
// 构造
public:
	CYunTestDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_YUNTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
public:
	//定义发送包缓冲区长度
	char SendBuffer[10*1024];

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	//定义控件字段长度
	CStatic m_Static[50];
	CEdit m_Edit[50];
	CLoadXml Xml;
	CGetSysInfomation CPUusage;
	
	
	//定义收包缓冲区长度
	char RecvBuff[100*1024];
	//数据包长度
	unsigned short Datalen;

	//处理推送消息的线程变量
	SOCKET send_Sock;
	//客户端响应事件  
    WSAEVENT g_ClientEvent;  
    bool g_bClientClose;
	
	HANDLE hEvent;
	HANDLE m_Thread;
	//接受缓冲区
	char MsgBuffer[BUFFER_SIZE];
	int nsize;
	//线程循环标志，是否为真
	bool mark_thread;
	CRITICAL_SECTION  m_Section;
	CCriticalSection m_Section1;

	//创建解包线程句柄
	HANDLE m_AyncThread;
    HANDLE m_AyncEvent;
	bool aync_mark;
	HANDLE m_RecvSemaphore;
	HANDLE m_AyncSemaphore;

	//推送心跳包缓冲区信息
	map<int,TSHEART> MapHeart;
	vector<TSHEART> vecHeart;
	int heartcount;       //心跳包个数
	int countdata;         //返回304心跳包个数

	//推送心跳包缓冲区信息
	map<int,NOTICEHEART> MapNotice;
	int noticecount;       //心跳包个数
	unsigned int noticeid;

	//压力相关参数
	vector<IODATA> iodata;
	std::string m_pressreqs[50];
	char m_pressreq[1024*5];
	CIOFUN m_ioFun;
	CMutilThread m_ThdData;
	//多账号功能序号
	int       accountcount;

	//压力参数数值定义
	__int64 m_SendSNum;         //发送成功数
	__int64 m_SendFNum;         //发送失败数
	__int64 m_RecvSNum;         //接受成功数
	__int64 m_RecvFNum;         //接受失败数
	__int64     m_ConnSNum;         //连接成功数
	__int64     m_ConnFNum;         //连接失败数
	map<int,vector<IODATA>> mapFun;     //组功能和数据组合到map队列中
	map<int,IODATA> MapFun;             //压力功能数据包

public:
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnRedis();
	afx_msg void OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnChangeEdit(UINT nID);
	CListCtrl m_ListCtl;

	CCheckComboBox m_ComBoxFun;
	afx_msg void OnCbnSelchangeComboFun();
	// 请求功能显示到edit编辑框
	void GenFunStr(int FunNo, CString & ReqString);
	afx_msg void OnBnClickedBtnSend();
	// 组包函数，根据功能号来组包，缓冲区数据定义在类里面
	void GeneratePack(int FunId);
	// 解包函数
	void AyncPack(char * &PackBody, unsigned short PackLen, unsigned short PackType);
	// 解包函数
	void DealPack(char * & PackData, unsigned int PackLength, unsigned int PackType);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//定义推送消息线程函数
	static DWORD WINAPI TUISONGMsg(LPVOID lpParam);
	//定义解包线程
	static DWORD WINAPI AyncMsg(LPVOID lpParam);
	//内部处理函数
	void DealMsg();
	afx_msg void OnBnClickedBtnPress();
	CButton m_CheckFun;
	CComboBox m_FunList;
	//接口功能号列表
	map<int,CString> mapport;
//	vector<FUNNO> vectorFun;
	// 通过接口号加载相应接口的xml文件
	void GetFunXml(int funid);
	//大功能号，通过功能号加载字典以及大包头类型
	unsigned short Fun;
	afx_msg void OnCbnSelchangeFunlist();
	// 推送解包函数
	void TSAnycPack(char *& PackData, unsigned short PackLen, unsigned short PackType, int recvflag);
protected:
	afx_msg LRESULT OnSocketmessage(WPARAM wParam, LPARAM lParam);
public:
	CListCtrl m_TSList;
	CButton m_CheckAccount;
	// 压力数据值
	void AddNum(__int64 Type, __int64 CountNum);
	afx_msg void OnBnClickedBtn300();
	CButton m_CheckPush;
};
