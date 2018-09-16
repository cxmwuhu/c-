
// YunTestDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "YunTest.h"
#include "YunTestDlg.h"
#include "afxdialogex.h"

#define  WM_SOCKETMESSAGE  (WM_USER+100)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
protected:
	
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_ACTIVATE()

END_MESSAGE_MAP()


// CYunTestDlg �Ի���




CYunTestDlg::CYunTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CYunTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYunTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_ListCtl);
	DDX_Control(pDX, IDC_COMBO_FUN, m_ComBoxFun);
	DDX_Control(pDX, IDC_CHECK_FUN, m_CheckFun);
	DDX_Control(pDX, IDC_FUNLIST, m_FunList);
	DDX_Control(pDX, IDC_LIST_TUISONG, m_TSList);
	DDX_Control(pDX, IDC_CHECK_ACCOUNT, m_CheckAccount);
	DDX_Control(pDX, IDC_CHECK_PUSH, m_CheckPush);
}

BEGIN_MESSAGE_MAP(CYunTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CYunTestDlg::OnBnClickedBtnConnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CYunTestDlg::OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_REDIS, &CYunTestDlg::OnBnClickedBtnRedis)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DATA, &CYunTestDlg::OnLvnItemchangedListData)
	ON_CBN_SELCHANGE(IDC_COMBO_FUN, &CYunTestDlg::OnCbnSelchangeComboFun)
	//�Զ���editcombox����;
	ON_CONTROL_RANGE(EN_CHANGE, 2100, 2100+19, OnChangeEdit)
	ON_BN_CLICKED(IDC_BTN_SEND, &CYunTestDlg::OnBnClickedBtnSend)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_PRESS, &CYunTestDlg::OnBnClickedBtnPress)
	ON_CBN_SELCHANGE(IDC_FUNLIST, &CYunTestDlg::OnCbnSelchangeFunlist)
	ON_MESSAGE(WM_SOCKETMESSAGE, OnSocketmessage)
	ON_BN_CLICKED(IDC_BTN_300, &CYunTestDlg::OnBnClickedBtn300)
END_MESSAGE_MAP()


//�Զ���ascii��utf8ת������;
 void AsciiToUtf(LPCSTR srcStr,LPWSTR dstStr)
 {
	 DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, srcStr, -1, NULL, 0);
	 MultiByteToWideChar (CP_UTF8, 0,srcStr, -1, dstStr, dwNum);
 }

//����UTF16��UTF8ת��;
 char * UTF16_2_UTF8( const wstring& strUTF16 )  
 {  
	 int nUTF8Length = ::WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,NULL,0,0,0); 
	 char * pStrUtf8=new char[nUTF8Length];
	 WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,pStrUtf8,nUTF8Length,0,0);  
	 return pStrUtf8;  
 }  

//����UTF8��UTF16ת��;
 wstring UTF8_2_UTF16( const string& strUTF8 )  
 {  
	 int nUTF16Length = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,NULL,0);  
	 nUTF16Length += 1;  
	 wstring strUTF16(nUTF16Length ,' ');  
	 int nRet = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,&strUTF16[0],nUTF16Length);  
	 ASSERT(0 != nRet);  
	 return strUTF16;  
 }  

 string GBKToUTF8(const std::string& strGBK)  
{  
    string strOutUTF8 = "";  
    WCHAR * str1;  
    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);  
    str1 = new WCHAR[n];  
    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);  
    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);  
    char * str2 = new char[n];  
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);  
    strOutUTF8 = str2;  
    delete[]str1;  
    str1 = NULL;  
    delete[]str2;  
    str2 = NULL;  
    return strOutUTF8;  
}  
  
string UTF8ToGBK(const std::string& strUTF8)  
{  
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);  
    WCHAR * wszGBK = new wchar_t[len*2 + 2];  
    memset(wszGBK, 0, len * 2 + 2);  
    MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, wszGBK, len);  
  
    len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);  
    char *szGBK = new char[len + 1];  
    memset(szGBK, 0, len + 1);  
    WideCharToMultiByte(CP_ACP,0, wszGBK, -1, szGBK, len, NULL, NULL);  
    //strUTF8 = szGBK;  
    std::string strTemp(szGBK);  
    delete[]szGBK;  
    delete[]wszGBK;  
    return strTemp;  
}  

 //����CRC32ֵ
 unsigned int GetCrc32(char* InStr,unsigned int len)
 {     
    //����Crc32�Ĳ�ѯ��  
    unsigned int Crc32Table[256];   
    int i,j;     
    unsigned int Crc;     
    for (i = 0; i < 256; i++)  
    {     
        Crc = i;     
        for (j = 0; j < 8; j++)  
        {     
            if (Crc & 1)     
                Crc = (Crc >> 1) ^ 0xEDB88320;          
            else    
                Crc >>= 1;   
        }     
        Crc32Table[i] = Crc;     
    }     
    //��ʼ����CRC32У��ֵ  
    Crc=0xffffffff;     
    for(unsigned int i=0; i<len; i++){    
        Crc = (Crc >> 8)^ Crc32Table[(Crc & 0xFF) ^ InStr[i]];     
    }  
      
    Crc ^= 0xFFFFFFFF;  
    return Crc;     
}

 //ʮ����ת������-short����
char *short_T_byte(unsigned short data,char *str)
{
	static int shortlen = sizeof(unsigned short)*8;
	for(int f=shortlen-1;f>=0;f--,data>>=1)
	{
		str[f] = (01&data)+'\0';
	}
	str[shortlen] = '\0';
	return str;
}

//ʮ����ת������
char *ok(int n,char *b)
{
    static int LEN=8*sizeof(int);
    for(int i=LEN-1;i>=0;i--,n>>=1)
        b[i]=(01&n)+'0';
    b[LEN]='\0';
    return b;
}

//ʮ����ת������
char *ok64(__int64 n,char *b)
{
    static int LEN=8*sizeof(__int64);
    for(int i=LEN-1;i>=0;i--,n>>=1)
        b[i]=(01&n)+'0';
    b[LEN]='\0';
    return b;
}

//���嵽URLencode��ת��;
 inline BYTE toHex(const BYTE &x)
 {
	 return x > 9 ? x + 55: x + 48;
 }

#pragma pack(1)
 //���������ͷ
 struct HQ_HEAD
 {
	 unsigned char	cSparate;//�ָ�����'{', ':'��, ����0�͡�}��'H' tag
	 unsigned short	type;
	 unsigned short  attrs;
	 unsigned short  length;
 };

 struct sub_tuisong_head
 {
	 unsigned short  sub_type;
	 unsigned short  sub_attrs;          // ȡֵSubAttrsEnum
	 unsigned short  sub_length;
	 unsigned int    sub_extend;
 };

 struct Sub_Head
 {
	 unsigned short	sub_type;
	 unsigned short  sub_attrs;
	 unsigned short  sub_length;
 };
 //������뷽ʽ
#pragma pack()

 //�������������ܵ�ö������
const TCHAR *pReqStruct[] = {
	_T("req_string"),
	_T("req_byte"),
	_T("req_char"),
	_T("req_short"),
	_T("req_float"),
	_T("req_int"),
	_T("req_stringarray"),
	_T("req_time"),
	_T("req_long64"),
	_T("req_char2"),
	_T("req_char3"),
	_T("req_customdata"),
	_T("req_130data"),
	_T("req_encrypass"),
	_T("req_intarray"),
	_T("req_shortarray"),
	_T("req_account"),
	_T("req_uint")
};

const TCHAR *pAnsStruct[] = {
	_T("ans_stringarray"),
	_T("ans_unit64"),
	_T("ans_string"),
	_T("ans_byte"),
	_T("ans_char"),
	_T("ans_short"),
	_T("ans_int"),
	_T("ans_int24"),
	_T("ans_bytearray"),
	_T("ans_long"),
	_T("ans_long64"),
//	_T("ans_chararray"),
	_T("ans_intarray"),
//	_T("ans_uint24"),
	_T("ans_customdata"),
	_T("ans_130data")
};

// CYunTestDlg ��Ϣ�������

BOOL CYunTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ�����ͻ������Ķ������
	memset(SendBuffer,0,10*1024);
	memset(RecvBuff,0,100*1024);
	Datalen = 0;
	//����list-control���
	m_ListCtl.SetExtendedStyle(LVS_REPORT|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_ListCtl.DeleteAllItems();
	m_TSList.SetExtendedStyle(LVS_REPORT|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_TSList.DeleteAllItems();
	//��ʼ��ip��port
	TCHAR hqip[30] = {0};
	::GetPrivateProfileString(L"CONFIG",L"IP",L"127.0.0.1",hqip,30,L".\\HQ.INI");
	GetDlgItem(IDC_EDIT_IPADDR)->SetWindowText(hqip);
	TCHAR hqport[10] = {0};
	::GetPrivateProfileString(L"CONFIG",L"port",L"19999",hqport,10,L".\\HQ.INI");
	GetDlgItem(IDC_EDIT_PORT)->SetWindowText(hqport);

	//��ȡ���ܽڵ��combox��Ͽ�
	
	TCHAR FunName[100*100] = {0};
	TCHAR FunSection[100] = {0};
	int pos=0,gg=0;
	vector<CString> V_Fun;CString FunString = _T("");
	::GetPrivateProfileSection(L"CONFIG",FunName,100*100,L".\\FunConfig.ini");
	while(true)
	{
		FunSection[pos++] = FunName[gg++];
		if(FunName[gg] == 0)
		{
			FunString.Format(L"%ws",FunSection);
			V_Fun.push_back(FunString);
			FunString = _T("");
			memset(FunSection,0,100);
			pos = 0;
			if(FunName[gg+1]==0)
			{
				break;
			}
			gg++;
		}
	}
	for(unsigned int d = 0; d<V_Fun.size(); d++)
	{
		int npos = V_Fun[d].Find(L"=");
		FUNNO strFun;
		//��ȡ���ܺ�
		strFun.Funid = _ttoi(V_Fun[d].Left(npos));
		//��ȡ�ӿ�����
		int strLen = V_Fun[d].GetLength();
		strFun.Funname = V_Fun[d].Right(strLen-npos-1);
		mapport.insert(map<int,CString>::value_type(strFun.Funid,strFun.Funname));
		CString FunStr = _T("");FunStr.Format(L"%d  %s",strFun.Funid,strFun.Funname);
		m_FunList.InsertString(d,FunStr);
	}
	
	//����xml�ֵ��ļ�
	TCHAR dict[50] = {0};
	::GetPrivateProfileString(L"DICT",L"DICTNAME",L"HQYUN.xml",dict,50,L".\\fUNCONFIG.INI");
	Fun = GetPrivateProfileInt(L"DICT",L"Fun",1000,L".\\fUNCONFIG.INI");
	
	size_t len = wcslen(dict) + 1;
    size_t converted = 0;
    char *CStr;
    CStr=(char*)malloc(len*sizeof(char));
    wcstombs_s(&converted, CStr, len, dict, _TRUNCATE);
	
	bool xmlmk = Xml.DictXml(CStr);
	if(xmlmk == false)
	{
		AfxMessageBox(L"xml�����ֵ����ʧ��\n");
	}
	free(CStr);
	//�ֵ������Ϻ󣬵���combox���ݿ�
	for(unsigned int i=0; i<Xml.XmlFun.size(); i++)
	{
		CString xmlstr = _T("");
		xmlstr.Format(_T("%d %s"),Xml.XmlFun[i].FunID,Xml.XmlFun[i].FunName);
		m_ComBoxFun.InsertString(i,xmlstr);
	}
	m_ComBoxFun.SetCurSel(0);
	//����combox������ı༭��Χ  m_ComBoxFun
	CRect SelRect;
	GetDlgItem(IDC_STATIC_FUNC)->GetWindowRect(&SelRect);
	ScreenToClient(&SelRect);

	CRect ComRect;
	GetDlgItem(IDC_COMBO_FUN)->GetWindowRect(&ComRect);
	ScreenToClient(&ComRect);

	for(int i=0; i<50; i++)
	{
		m_Static[i].Create((LPCTSTR)"CStatic"
			, WS_CHILD|SS_LEFT
			, CRect(SelRect.left,ComRect.bottom+5+i*16,SelRect.left+162,SelRect.bottom+5+i*16+ComRect.Height())
			, this
			, 2000+i);
		m_Static[i].SetFont(GetFont());

	//	ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL    WS_CHILD|WS_VISIBLE|WS_TABSTOP |WS_BORDER|ES_LEFT|ES_AUTOHSCROLL
//|WS_HSCROLL| WS_VSCROLL
		m_Edit[i].Create(ES_MULTILINE|WS_CHILD|WS_EX_CLIENTEDGE|WS_TABSTOP|ES_LEFT|ES_AUTOHSCROLL|WS_BORDER
			, CRect(SelRect.left+100,ComRect.bottom+5+i*16,ComRect.right,ComRect.bottom+5+i*16+SelRect.Height())
			, this
			, 2100+i);
		m_Edit[i].SetFont(GetFont());
	}
	OnCbnSelchangeComboFun();

	m_TSList.InsertColumn(0,L"���",LVCFMT_LEFT,50);
	m_TSList.InsertColumn(1,L"���鷵�ر��",LVCFMT_LEFT,100);
	m_TSList.InsertColumn(2,L"ͨ���������ݣ���Ϣ��Ϣ",LVCFMT_LEFT,200);
	//�򿪿���̨����
	if ( AllocConsole() )
	{
		FILE * fp = freopen ("CONOUT$", "w", stdout ); 
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		
		SetConsoleTitle(_T("���Ե�����ʾ��Ϣ"));
		CloseHandle(hConsole);
		printf("*************�Ʒ��������ʾ��Ϣ:*************\n");
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CYunTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CYunTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CYunTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CYunTestDlg::OnBnClickedBtnConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	unsigned char macdata[1024] = {0};
	Mac.getLocalMac(macdata);
	CString ipaddr = _T(""),ipport=_T("");
	GetDlgItem(IDC_EDIT_IPADDR)->GetWindowText(ipaddr);
	::WritePrivateProfileString(_T("CONFIG"),_T("IP"),ipaddr,_T(".\\HQ.INI"));
	GetDlgItem(IDC_EDIT_PORT)->GetWindowText(ipport);
	::WritePrivateProfileString(_T("CONFIG"),_T("PORT"),ipport,_T(".\\HQ.INI"));
	int accountport = _ttoi(ipport);
	char ip[100] = {0};
	::wsprintfA(ip,"%ls",(LPCTSTR)ipaddr);
	if(!m_CheckPush.GetCheck())
	{
	    int ret = m_Mysock.ConnectEx(ip,accountport,5);
		if(ret!=0)
		{
			//socket���ӷ�����ʧ��
			closesocket(m_Mysock.m_Socket);
			return;
		}
		else
		{
			SetTimer(HEARTID,25000,NULL);
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӳɹ�"));
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(0);
			GetDlgItem(IDC_BTN_DISCONNECT)->SetWindowTextW(_T("�Ͽ�����"));
			GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(1);	
		}
	}
//	printf("socket1��ֵ:%d\n",m_Mysock.m_Socket);
	/*
	if(ret!=0)
	{
		//socket���ӷ�����ʧ��
		closesocket(m_Mysock.m_Socket);
		return;
	}
	else
	{
	    GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӳɹ�"));
		GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(0);
		GetDlgItem(IDC_BTN_DISCONNECT)->SetWindowTextW(_T("�Ͽ�����"));
	    GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(1);	
	}
//	SetTimer(HEARTID,25000,NULL);
*/
	//����������ص�socket�������¼�
	WSADATA wsaData;
	if(WSAStartup(MAKEWORD(2,2),&wsaData) == -1)
	{
		printf("socket��ʼ��ʧ��:%d\n",WSAGetLastError());
	}
	send_Sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(send_Sock == INVALID_SOCKET)
	{
		printf("���ͷ�����socket����ʧ��:%d\n",GetLastError());
	}
	//����socket����ҵ��
	struct sockaddr_in sendaddr;
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_addr.S_un.S_addr = inet_addr(ip);
	sendaddr.sin_port = htons(accountport);

	int sockret = connect(send_Sock,(struct sockaddr*)&sendaddr,sizeof(sockaddr_in));
	if(sockret != 0)
	{
		printf("����socket���ӷ�������ַʧ��\n");
	}
	//�������͵�½��
	if(m_CheckPush.GetCheck())
	{
		//��¼֮ǰ��Ҫ��֤�·�����������������Ӳ��ɹ�
		LOGIN_HEADEX loginpack;
		memset(&loginpack,0,sizeof(LOGIN_HEADEX));
		//��ȡ��ͷ��Ϣ
		loginpack.accCmdHead.wCmdType = ACCCMD_SERVERLOGIN;
		loginpack.accCmdHead.wAttr = 0x8000;
	//	loginpack.accCmdHead.nExpandInfo = 0;
		loginpack.accCmdHead.nLen = sizeof(ACC_SERVERLOGIN);
		//��ȡACC_SERVERLOGIN��������
		CString szValid=_T("");

		GetPrivateProfileStringW(_T("ADDALARM"),_T("LOGINVALIDSTRING"),_T("GWGWGWGW"),szValid.GetBuffer(10),10,_T(".\\ALARM.ini"));
		szValid.ReleaseBuffer();
		int len = szValid.GetLength();
		char szString[10];
		memset(szString,0,len+1);
		for(int f=0; f<len; f++)
		{
			szString[f] = szValid.GetAt(f);
		}
		szString[len] = '\0';
		strncpy(loginpack.accServerLogin.m_cValid,szString,8);
		srand(time(NULL));
		
		loginpack.accServerLogin.nSerId = rand();
		int loginret = send(send_Sock,(char*)&loginpack,sizeof(LOGIN_HEADEX),0);
		if(loginret>0)
		{
			printf("��¼���ͷ���ɹ�,��¼������:%d\n",loginret);
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowTextW(_T("���ӳɹ�"));
			GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(0);
			GetDlgItem(IDC_BTN_DISCONNECT)->SetWindowTextW(_T("�Ͽ�����"));
			GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(1);
		}
	}
	g_ClientEvent = WSACreateEvent();
	int sl = WSAEventSelect(send_Sock,g_ClientEvent,FD_READ|FD_CLOSE);

	nsize = 0;
	g_bClientClose = FALSE;


	//����������Ϣ���̺߳���
	m_Thread = NULL;
	hEvent = ::CreateEventA(NULL,TRUE,FALSE,NULL);
	heartcount = 0;
	noticecount = 0;
	memset(MsgBuffer,0,100*1024);
	mark_thread = true;
	m_RecvSemaphore = ::CreateSemaphore(NULL,1,2,NULL);
	m_AyncSemaphore = ::CreateSemaphore(NULL,1,100,NULL);

	//��������߳������Ϣ
	aync_mark = true;
	m_AyncEvent = CreateEventA(NULL,TRUE,FALSE,NULL);

	::InitializeCriticalSection(&m_Section);
	//304������������ʼ��
	countdata = 0;

	//102������ϢӦ���
	noticeid = 0;
	//�����߳��հ�����������Ϣ
	
	DWORD threadid;
	m_Thread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)TUISONGMsg,this,0,&threadid);
	if(m_Thread == NULL)
	{
		printf("�̴߳���ʧ��:%d\n",WSAGetLastError());
	}
	printf("�����̴߳����ɹ�:%d\n",threadid);
	SetEvent(hEvent);   //�����ź���Ϊ���ź�

	//��������߳�
	DWORD Ayncid;
	m_AyncThread = ::CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)AyncMsg,this,0,&Ayncid);
	if(m_AyncThread == NULL)
	{
		printf("����̴߳���ʧ��:%d\n",WSAGetLastError());
	}
	printf("����̴߳����ɹ�:%d\n",Ayncid);
	SetEvent(m_AyncEvent);
	ReleaseSemaphore(m_AyncSemaphore,1,NULL);

	m_TSList.DeleteAllItems();
	int listhead = m_TSList.GetHeaderCtrl()->GetItemCount();
	for(int i=listhead-1;i>0;i--)
	{
		m_TSList.DeleteItem(i);
	}
	SetTimer(TUISONGID,30000,NULL);
	Sleep(100);
	SetTimer(AlarmID305,55000,NULL);
	Sleep(300);
	SetTimer(NOTICE102,55000,NULL);
}

void CYunTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ʱ��������
	if(nIDEvent == 3000)
	{
		//��������
		char heartbuff[10] = {0},recvheart[10] = {0};
	    struct HQ_HEAD *pHQHead = (struct HQ_HEAD*)heartbuff;
		pHQHead->cSparate = '{';
		pHQHead->type = 0;
		pHQHead->attrs = 0;
		pHQHead->length = 0;
		printf("socketֵ:%d\n",m_Mysock.m_Socket);
		//if(m_Mysock.m_Socket!=INVALID_SOCKET)
		{
			if(m_Mysock.SendPack(heartbuff,sizeof(HQ_HEAD))<0)
			{
				printf("����������ʧ��\n");
				KillTimer(nIDEvent);
			}
			if(m_Mysock.RecvPack(recvheart,sizeof(HQ_HEAD))<=0)
			{
				printf("�������հ�ʧ��\n");
				KillTimer(nIDEvent);
			}
		}
		/*
		double cpus = CPUusage.CpuUseage();
		printf("CPUʹ���ʣ�%0.2f\n",cpus);
		CString cpustr = _T("");cpustr.Format(L"%0.2f",cpus);
		GetDlgItem(IDC_EDIT_CPU)->SetWindowText(cpustr);
		MEMORYSTATUS mem;
		int mb=1024*1024;
        GlobalMemoryStatus(&mem);
		printf("��Ч���:%f,�����ڴ�:%f,���ڴ�:%f\n",mem.dwAvailPhys/mb,mem.dwTotalVirtual/mb,mem.dwTotalPhys/mb);
		*/
	}
	if(nIDEvent == 3001)
	{
		//��������
		char pushbuff[20] = {0},recvheart[10] = {0};
		if(m_CheckPush.GetCheck())
		{
			struct ACC_CMDHEADEX *pHeart = (struct ACC_CMDHEADEX*)pushbuff;
			pHeart->wCmdType = ACCCMD_KEEPALIVE;
			pHeart->wAttr = 0x8000;
			pHeart->nLen = 0;
			pHeart->nExpandInfo = 0;
			EnterCriticalSection(&m_Section);
			int loginret = send(send_Sock,pushbuff,sizeof(ACC_CMDHEADEX),0);
			LeaveCriticalSection(&m_Section);
			if(loginret>0)
			{
				printf("���������ͳɹ�,����������:%d\n",loginret);
			}
		}
		else
		{
	    struct HQ_HEAD *pHQHead = (struct HQ_HEAD*)pushbuff;
		pHQHead->cSparate = '{';
		pHQHead->type = 0;
		pHQHead->attrs = 0;
		pHQHead->length = 0;
		EnterCriticalSection(&m_Section);
		if(send_Sock!=INVALID_SOCKET)
		{
			int sendret = send(send_Sock,pushbuff,sizeof(HQ_HEAD),0);
			if(sendret<0)
			{
				printf("����������ʧ��\n");
				KillTimer(nIDEvent);
			}
			printf("send data:%d\n",sendret);
		}
		LeaveCriticalSection(&m_Section);
		}
	}
	if(nIDEvent == 6000)
	{
		//���͹�ƱԤ����Ϣ���������Լ��������
		for(unsigned int i = 0; i<MapHeart.size(); i++)
		{
			EnterCriticalSection(&m_Section);
			if(send_Sock!=INVALID_SOCKET)
			{
				int heartret = send(send_Sock,MapHeart[i].HeartBuff,MapHeart[i].heartlen,0);
				if(heartret<0)
				{
					printf("����������ʧ��:%d\n",WSAGetLastError());
					//KillTimer(nIDEvent);
				}
				printf("send data:%d\n",heartret);
			}
			LeaveCriticalSection(&m_Section);
		}
	}
	if(nIDEvent == 8000)
	{
		//���͹���Ԥ����Ϣ���������Լ��������
		for(unsigned int i = 0; i<MapNotice.size(); i++)
		{
			EnterCriticalSection(&m_Section);
			if(send_Sock!=INVALID_SOCKET)
			{
				int heartret = send(send_Sock,MapNotice[i].NoticeBuff,MapNotice[i].noticelen,0);
				if(heartret<0)
				{
					printf("notice����������ʧ��:%d\n",WSAGetLastError());
				}
				printf("send data:%d\n",heartret);
			}
			LeaveCriticalSection(&m_Section);
		}
	}
	if(nIDEvent == 5000)
	{
		//��ʾѹ�������������ݵ���ֵ
		CString m_Data = _T("");
		//��ʾ��������ֵ
		m_Data.Format(L"%I64d",m_ConnSNum);
		GetDlgItem(IDC_CONN_SUCCNUM)->SetWindowText(m_Data);

		m_Data.Format(L"%I64d",m_ConnFNum);
		GetDlgItem(IDC_CONN_FAILNUM)->SetWindowText(m_Data);
	
		m_Data.Format(L"%I64d",m_SendSNum);
		GetDlgItem(IDC_SEND_SUCCNUM)->SetWindowText(m_Data);
	
		m_Data.Format(L"%I64d",m_SendFNum);
		GetDlgItem(IDC_SEND_FAILNUM)->SetWindowText(m_Data);
		m_Data.Format(L"%I64d",m_RecvSNum);
		GetDlgItem(IDC_RECV_SUCCNUM)->SetWindowText(m_Data);
		m_Data.Format(L"%I64d",m_RecvFNum);
		GetDlgItem(IDC_RECV_FAILNUM)->SetWindowText(m_Data);
	}
	CDialogEx::OnTimer(nIDEvent);
}

DWORD WINAPI CYunTestDlg::TUISONGMsg(LPVOID lpParam)
{
	CYunTestDlg *pp = (CYunTestDlg*)lpParam;
	printf("�̴߳�������ʼ\n");
	WSAEVENT arrayEvent[1];
	arrayEvent[0] = pp->g_ClientEvent;
	while(pp->mark_thread)
	{
		//�ȴ��߳̿�ʼ
		DWORD nEventIndex = WSAWaitForMultipleEvents(1,arrayEvent,FALSE,WSA_INFINITE,FALSE);
		if(nEventIndex == WSA_WAIT_FAILED)
		{
			printf("�ȴ�ʧ��:%d\n",WSAGetLastError());
		}
		WSANETWORKEVENTS nRecvEvent;
		WSAEnumNetworkEvents(pp->send_Sock,pp->g_ClientEvent,&nRecvEvent);
		if(nRecvEvent.lNetworkEvents&FD_READ)
		{
			//��ȡ�ͻ�����Ϣ
			int nErrorCode = nRecvEvent.iErrorCode[FD_READ_BIT];
			if(nErrorCode != 0)
			{
				printf("FD_READ ����:%d\n",nErrorCode);
				break;      //socket�ѽ���ʧ�ܡ��˳��̺߳���
			}
			if (!pp->g_bClientClose)  
            {  
                //Read data from the socket  
				EnterCriticalSection(&pp->m_Section);
                int nret = recv(pp->send_Sock, (pp->MsgBuffer)+(pp->nsize), BUFFER_SIZE-(pp->nsize), 0);  
                if(SOCKET_ERROR == nret)  
                {  
                    printf("recv() failed\n");  
					LeaveCriticalSection(&pp->m_Section);
                }  
                else  
                {
				    pp->nsize += nret;  
					printf("�հ�ֵ:%d\n",nret);
					LeaveCriticalSection(&pp->m_Section);
					ReleaseSemaphore(pp->m_AyncSemaphore,1,NULL);
                }  
            }  
            else  
            {  
                printf("FD_READ quit\n");  
                break;  
            }
		}
		else if(nRecvEvent.lNetworkEvents&FD_CLOSE)
		{
			//�ͻ������ӹر�  
            int nErrorCode = nRecvEvent.iErrorCode[FD_CLOSE_BIT];  
            if (nErrorCode != 0)  
            {  
                printf("FD_CLOSE failed with error %d\n", nErrorCode);  
            }  
            printf("FD_CLOSE close socket\n");  
			closesocket(pp->send_Sock);
            break;  
		}
		else  
        {  
            printf("FD_NULL quit\n");  
            break;  
        } 
	}
	return 1;
}

//����߳�
DWORD WINAPI CYunTestDlg::AyncMsg(LPVOID lpParam)
{
	printf("����߳̿�ʼ\n");
	CYunTestDlg *paync = (CYunTestDlg*)lpParam;
	while(paync->aync_mark)
	{
		DWORD nEventid = ::WaitForSingleObject(paync->m_AyncSemaphore,INFINITE);
		printf("1\n");
		if(nEventid == WAIT_OBJECT_0)              //�����̱߳���������ʼ����
		{
			paync->DealMsg();
		}
	}
	return 1;
}

void CYunTestDlg::DealMsg()
{
	if(m_CheckPush.GetCheck())
	{
		//ֱ�����ͷ�����
		EnterCriticalSection(&m_Section);
		ACC_CMDHEADEX *pDealMsg = (struct ACC_CMDHEADEX*)MsgBuffer;
		if(nsize>=sizeof(ACC_CMDHEADEX)+pDealMsg->nLen)
		{
			printf("nsize = %d,������:%d,������:%d\n",nsize,pDealMsg->nLen,pDealMsg->wCmdType);
			//��С������
			if(pDealMsg->nLen>sizeof(sub_tuisong_head))
			{
				sub_tuisong_head *pTSmsg = (sub_tuisong_head *)(MsgBuffer+sizeof(ACC_CMDHEADEX));
				printf("�յ�С����Ϣ��С��ͷ����:%d��С��ͷ����:%d\n",pTSmsg->sub_type,pTSmsg->sub_length);
				LeaveCriticalSection(&m_Section);
				if(pTSmsg->sub_type == ACCCMD_KEEPALIVE)
				{
					printf("�յ�����������\n");
				}
				else
				{
					char *MsgData = new char[pTSmsg->sub_length+1];
					memset(MsgData,0,pTSmsg->sub_length+1);
					EnterCriticalSection(&m_Section);
					memcpy(MsgData,MsgBuffer+sizeof(ACC_CMDHEADEX)+sizeof(sub_tuisong_head),pTSmsg->sub_length);
					LeaveCriticalSection(&m_Section);
					TSAnycPack(MsgData,pTSmsg->sub_length,pTSmsg->sub_type,0);
					delete []MsgData;
				}
			}
			EnterCriticalSection(&m_Section);
			int nleng = nsize - (sizeof(ACC_CMDHEADEX)+(pDealMsg->nLen));
			nsize = nsize-(sizeof(ACC_CMDHEADEX)+(pDealMsg->nLen));
			memmove(MsgBuffer,MsgBuffer+sizeof(ACC_CMDHEADEX)+pDealMsg->nLen,nleng);
			memset(MsgBuffer+nleng,0,BUFFER_SIZE-nleng);
			if(nsize>0)
			{
				ReleaseSemaphore(m_AyncSemaphore,1,NULL);
			}
			LeaveCriticalSection(&m_Section);
			printf("ʣ�°�����:%d\n",nsize);
		}
		LeaveCriticalSection(&m_Section);
	}
	else
	{
	EnterCriticalSection(&m_Section);
	HQ_HEAD *pMsg = (HQ_HEAD*)MsgBuffer;
	if(nsize>=sizeof(HQ_HEAD)+(pMsg->length))
	{
		printf("nsize = %d,������:%d,������:%d\n",nsize,pMsg->length,pMsg->type);
		switch(pMsg->type)
		{
		case 3001:
		{
			//���          char   //0:������������صĳɹ����
			//1:������������ص�ʧ�ܱ��
			//2:ͨ�÷��������ص�����
			//3:ͨ�÷������������͵�����,�������ݵķָ�������0
			char hqflag = *(MsgBuffer+sizeof(HQ_HEAD));
			int hqint = hqflag;
			if(pMsg->length>sizeof(HQ_HEAD)+1)
			{
			sub_tuisong_head *pTSmsg = (sub_tuisong_head *)(MsgBuffer+sizeof(HQ_HEAD)+1);
			printf("�յ�С����Ϣ��С��ͷ����:%d��С��ͷ����:%d,�յ������:%d\n",pTSmsg->sub_type,pTSmsg->sub_length,hqint);
			LeaveCriticalSection(&m_Section);
			switch(hqint)
			{
			case 0:         //0:������������صĳɹ����,û�к����Ӧ�����ݷ���
				{
					m_ListCtl.DeleteAllItems();
					int nHeadNum = m_ListCtl.GetHeaderCtrl()->GetItemCount();
					for(int i=nHeadNum-1; i>=0; i--)
					{
						m_ListCtl.DeleteColumn (i);
					}
					m_ListCtl.InsertItem(0,_T(""));
					m_ListCtl.SetItemText(0,0,L"1");
					m_ListCtl.SetItemText(0,1,L"0,�������ݷ��سɹ�");
				}break;
			case 1:                 //1:������������ص�ʧ�ܱ��
				{
					m_ListCtl.InsertItem(0,_T(""));
					m_ListCtl.SetItemText(0,0,L"1");
					m_ListCtl.SetItemText(0,1,L"1,������������ص�ʧ�ܱ��");
				}break;
			case 2:                 //2:ͨ�÷��������ص�����,������������Ҫ����
				{
					
					if(pTSmsg->sub_length == 0&&(pTSmsg->sub_type)!=0)
					{
						m_ListCtl.DeleteAllItems();
						int nHeadNum = m_ListCtl.GetHeaderCtrl()->GetItemCount();
						for(int i=nHeadNum-1; i>=0; i--)
						{
							m_ListCtl.DeleteColumn (i);
						}
						m_ListCtl.InsertColumn(0,L"���",LVCFMT_LEFT,50);
						m_ListCtl.InsertColumn(1,L"������񷵻���Ϣ",LVCFMT_LEFT,150);
						m_ListCtl.InsertItem(0,_T(""));
						m_ListCtl.SetItemText(0,0,L"1");
						m_ListCtl.SetItemText(0,1,L"2,ͨ��������Ϣ");
					}
					else
					{
					char *MsgData = new char[pTSmsg->sub_length+1];
					memset(MsgData,0,pTSmsg->sub_length+1);
					EnterCriticalSection(&m_Section);
					memcpy(MsgData,MsgBuffer+sizeof(HQ_HEAD)+1+sizeof(sub_tuisong_head),pTSmsg->sub_length);
					LeaveCriticalSection(&m_Section);
					TSAnycPack(MsgData,pTSmsg->sub_length,pTSmsg->sub_type,hqint);
					delete []MsgData;
					}
				}break;
			case 3:            //3:ͨ�÷������������͵�����,�������ݵķָ�������0
				{
					char *MsgData = new char[pTSmsg->sub_length+1];
					memset(MsgData,0,pTSmsg->sub_length+1);
					EnterCriticalSection(&m_Section);
					memcpy(MsgData,MsgBuffer+sizeof(HQ_HEAD)+1+sizeof(sub_tuisong_head),pTSmsg->sub_length);
					LeaveCriticalSection(&m_Section);
					TSAnycPack(MsgData,pTSmsg->sub_length,pTSmsg->sub_type,hqint);
					delete []MsgData;
				}break;
			}
			}
		}break;
		case 0: //������
		    {
				printf("��������������\n");
		    }break;
		}
		EnterCriticalSection(&m_Section);
		int nleng = nsize - (sizeof(HQ_HEAD)+(pMsg->length));
		nsize = nsize-(sizeof(HQ_HEAD)+(pMsg->length));
		memmove(MsgBuffer,MsgBuffer+sizeof(HQ_HEAD)+pMsg->length,nleng);
		memset(MsgBuffer+nleng,0,BUFFER_SIZE-nleng);
		if(nsize>0)
		{
			ReleaseSemaphore(m_AyncSemaphore,1,NULL);
		}
		LeaveCriticalSection(&m_Section);
		printf("ʣ�°�����:%d\n",nsize);
	}
	LeaveCriticalSection(&m_Section);
	}
}

// ���ͽ������
void CYunTestDlg::TSAnycPack(char *& PackData, unsigned short PackLen, unsigned short PackType, int recvflag)
{
	//���ص�������ͷ+����ͷ�������ݰ���Ҫ�ȼ�ȥͷ���ֵ����ݣ��ٽ���������
	//����ͷ
	if(PackLen == 0)
	{
		return;
	}
	int offset=0;
	if(PackType == 304)
	{
		SYSTEMTIME systm;
		GetLocalTime(&systm);
		CString timestr = _T("");
		timestr.Format(L"%d-%d-%d %d-%d-%d-%d",systm.wYear,systm.wMonth,systm.wDay,systm.wHour,systm.wMinute,systm.wSecond,systm.wMilliseconds);
		CString AlarmStr = _T("");
		wstring wprocode;
		wstring wproname;
		wstring wmsgstr;
		int msgid = 0;
		m_TSList.InsertItem(countdata,_T(""));
		CString countnum = _T("");countnum.Format(L"%d",countdata);
		m_TSList.SetItemText(countdata,0,countnum);
		if(recvflag == 3)
		{
			m_TSList.SetItemText(countdata,1,L"3:ͨ�÷������������͵�����");
		}
		if(PackLen>offset)
		{
			// ��ϢID                      int
			msgid = *(int*)(PackData+offset);
			offset += 4;
		}
		if(PackLen>offset)
		{
			// ��Ʊ����                    string
			short procodelen = *(short*)(PackData+offset);
			offset += 2;
			char *procode = new char[procodelen+1];
			memset(procode,0,procodelen+1);
			memcpy(procode,PackData+offset,procodelen);
			offset += procodelen;
			wprocode = UTF8_2_UTF16(procode);
			delete []procode;
		}
		if(PackLen>offset)
		{
			// ��Ʊ����                    string
			short pronamelen = *(short*)(PackData+offset);
			offset += 2;
			char *proname = new char[pronamelen+1];
			memset(proname,0,pronamelen+1);
			memcpy(proname,PackData+offset,pronamelen);
			offset += pronamelen;
			wproname = UTF8_2_UTF16(proname);
			delete []proname;
		}
		if(PackLen>offset)
		{
			// ��Ϣ����                    string
			short maglen = *(short*)(PackData+offset);
			offset += 2;
			char *msgstr = new char[maglen+1];
			memset(msgstr,0,maglen+1);
			memcpy(msgstr,PackData+offset,maglen);
			offset += maglen;
			wmsgstr = UTF8_2_UTF16(msgstr);
			delete []msgstr;
		}
		AlarmStr.Format(L"�յ�����ʱ�䣺%s,��Ϣ����:%d,��Ϣid:%d,��Ʊ����:%s,��Ʊ����:%s,��Ϣ����:%s",timestr,304,msgid,wprocode.c_str(),wproname.c_str(),wmsgstr.c_str());
		m_TSList.SetItemText(countdata,2,AlarmStr);
		countdata = countdata + 1;
		return;
	}
	if(PackType == 310)
	{
		CString AlarmStr = _T("");
		int noticeval;
		wstring wproname(nullptr);
		wstring wmsgstr(nullptr);
		wstring wpurl(nullptr);
		int msgid = 0;
		m_TSList.InsertItem(countdata,_T(""));
		CString countnum = _T("");countnum.Format(L"%d",countdata);
		m_TSList.SetItemText(countdata,0,countnum);
		if(recvflag == 3)
		{
			m_TSList.SetItemText(countdata,1,L"3:ͨ�÷������������͵�����");
		}
		if(PackLen>offset)
		{
			// ��ϢID                      int
			msgid = *(int*)(PackData+offset);
			offset += 4;
			CString msgidstr = _T("");msgidstr.Format(L"%d",msgid);
		}
		if(PackLen>offset)
		{
			// ��������                    byte         (0:��ͨ����[���Ӳ�����ʱ���Բ�����] 1:�����[���Ӳ���ΪURL])
			unsigned char noticetype = *(PackData+offset);
			offset += 1;
			noticeval = noticetype;
		}
		if(PackLen>offset)
		{
			if(noticeval == 0)
			{
				// ���Ӳ���                    string
				short pronamelen = *(short*)(PackData+offset);
				offset += 2;
				char *proname = new char[pronamelen+1];
				memset(proname,0,pronamelen+1);
				memcpy(proname,PackData+offset,pronamelen);
				offset += pronamelen;
				wproname = UTF8_2_UTF16(proname);
				delete []proname;
			}
			if(noticeval == 1)
			{
				// ���Ӳ���                    string
				short urllen = *(short*)(PackData+offset);
				offset += 2;
				char *purl = new char[urllen+1];
				memset(purl,0,urllen+1);
				memcpy(purl,PackData+offset,urllen);
				offset += urllen;
				wpurl = UTF8_2_UTF16(purl);
				delete []purl;
			}
		}
		if(PackLen>offset)
		{
			// ��Ϣ����                    string
			short maglen = *(short*)(PackData+offset);
			offset += 2;
			char *msgstr = new char[maglen+1];
			memset(msgstr,0,maglen+1);
			memcpy(msgstr,PackData+offset,maglen);
			offset += maglen;
			wmsgstr = UTF8_2_UTF16(msgstr);
			delete []msgstr;
		}
		if(noticeval == 0)
		{
			AlarmStr.Format(L"��Ϣid:%d,��������:%d,���Ӳ���:%s,��Ϣ����:%s",msgid,noticeval,wproname.c_str(),wmsgstr.c_str());
		}
		else
		{
			AlarmStr.Format(L"��Ϣid:%d,��������:%d,���Ӳ���:%s,��Ϣ����:%s",msgid,noticeval,wpurl.c_str(),wmsgstr.c_str());
		}
		m_TSList.SetItemText(countdata,2,AlarmStr);
		countdata = countdata + 1;
		return;
	}
	if(PackType == 515)
	{
		//��Ϣ���� - ���������Android��Ϣ(������ִ��Ϣ)   sub_type=515
		CString AlarmStr = _T("");
		wstring wproname(nullptr);
		wstring wpmsg(nullptr);
		int msgid = 0;
		m_TSList.InsertItem(countdata,_T(""));
		CString countnum = _T("");countnum.Format(L"%d",countdata);
		m_TSList.SetItemText(countdata,0,countnum);
		if(recvflag == 3)
		{
			m_TSList.SetItemText(countdata,1,L"3:ͨ�÷������������͵�����");
		}
		if(PackLen>offset)
		{
			//���ӱ��                    byte      (Ԥ������)
			unsigned char mark = *(PackData+offset);
			msgid = mark;
			offset += 1;
			CString msgidstr = _T("");msgidstr.Format(L"%d",msgid);
		}
		if(PackLen>offset)
		{
			// ��Ϣ����                    ����data(json��ʽ)
			short pronamelen = *(short*)(PackData+offset);
			offset += 2;
			char *proname = new char[pronamelen+1];
			memset(proname,0,pronamelen+1);
			memcpy(proname,PackData+offset,pronamelen);
			offset += pronamelen;
			wproname = UTF8_2_UTF16(proname);
			delete []proname;
		}
		if(PackLen>offset)
		{
			//��ִ��Ϣ                    String    (ͨ��516Э�鴦��)
			short msglen = *(short*)(PackData+offset);
			offset += 2;
			char *pmsg = new char[msglen+1];
			memset(pmsg,0,msglen+1);
			memcpy(pmsg,PackData+offset,msglen);
			offset += msglen;
			wpmsg = UTF8_2_UTF16(pmsg);
			delete []pmsg;
		}
		AlarmStr.Format(L"���ӱ��:%d,��Ϣ����:%d,��ִ��Ϣ:%s",msgid,wproname.c_str(),wpmsg.c_str());
		m_TSList.SetItemText(countdata,2,AlarmStr);
		countdata = countdata + 1;
		return;
	}
	m_ListCtl.DeleteAllItems();
	int nHeadNum = m_ListCtl.GetHeaderCtrl()->GetItemCount();
	for(int i=nHeadNum-1; i>=0; i--)
	{
		m_ListCtl.DeleteColumn (i);
	}
	//����Ӧ���ֵ�������listctrl����
	if(PackType == 302||PackType == 312)
	{
		//����Ŀ             short   (0 ʧ��  >0�ɹ�)
		unsigned short ntolnum = *(unsigned short*)(PackData+offset);
	    if(ntolnum == 0)                      //��ʾʧ��
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
		if(ntolnum>0)
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
	}
	else if(PackType==330||PackType==300||PackType==301||PackType==307||PackType==308||PackType==100||PackType==101/*||PackType==102*/||PackType==500||PackType==512||PackType==516||PackType==517||PackType==520)
	{
		//���               byte     ��0 �ɹ�  1ʧ�ܣ�
		unsigned char nResultcode = *(PackData+offset);
		int ntolnum = nResultcode;
		if(ntolnum == 0)                      //0 �ɹ�
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
		if(ntolnum == 1)                    //1ʧ��
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
	}
	else if(PackType==519)
	{
		//���               byte     ��0 �ɹ�  1ʧ�ܣ�
		unsigned char nResultcode = *(PackData+offset);
		int ntolnum = nResultcode;
		if(ntolnum == 0)                      //0 �ɹ�
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
		if(ntolnum != 0)                    //1ʧ��
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
	}
	else if(PackType == 518||PackType == 521)
	{
		//����Ŀ             short   (<0 ʧ��  �����ʶ������Ϣ����)
		unsigned short ntolnum = *(unsigned short*)(PackData+offset);
		if(ntolnum<0)                      //��ʾʧ��
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
		if(ntolnum>0)
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
	}
	else
	{
		if(PackType!=304)
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
			{
				m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
			}
		}
	}
	
	printf("��listctrl�ؼ��б����\n");
	m_ListCtl.InsertColumn(0,L"���",LVCFMT_LEFT,50);
	m_ListCtl.InsertItem(0,_T(""));
	m_ListCtl.SetItemText(0,0,L"1");
	if(recvflag == 2)
	{
		m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
	}
	switch(PackType)
	{
	case 100:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			//���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==0)               //0 �ɹ�
			{
				//��Ϣ������                  byte
				unsigned char msgbyte = *(PackData+offset);
				offset += 1;
				int msgnum = msgbyte;
				CString msgstr = _T("");msgstr.Format(L"%d",msgnum);
				m_ListCtl.SetItemText(0,3,msgstr);
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 101:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			//���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(nRes==0)               //0 �ɹ�
			{
				m_ListCtl.SetItemText(0,2,L"0,�ɹ�");
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 102:
		{
			//���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			if(nRes==0)               //0 �ɹ�
			{
				//��ϢID             unsigned int
				noticeid = *(unsigned int*)(PackData+offset);
				offset += sizeof(unsigned int);
				printf("���泣������102���ص���ϢID:%u\n",noticeid);
			}
			if(nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				printf("������Ϣ:%s\n",str);
				delete []str;
			}
		}break;
	case 500:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			//���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(nRes==0)               //0 �ɹ�
			{
				m_ListCtl.SetItemText(0,2,L"0,�ɹ�");
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 512:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			//���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==0)               //0 �ɹ�
			{
				//��Ϣ����                    ����data(ԭ�е��Ͻӿ�(317)��ʽ����json��ʽ)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *data = new char[strlen+1];
				memset(data,0,strlen+1);
				memcpy(data,PackData+offset,strlen);
				offset += strlen;
				wstring strdata = UTF8_2_UTF16(data);
				m_ListCtl.SetItemText(0,3,strdata.c_str());
				delete []data;
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 515:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			//���ӱ��                    byte      (Ԥ������)
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0)               //0 �ɹ�
			{
				//��Ϣ����                    ����data(json��ʽ)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *data = new char[strlen+1];
				memset(data,0,strlen+1);
				memcpy(data,PackData+offset,strlen);
				offset += strlen;
				wstring strdata = UTF8_2_UTF16(data);
				m_ListCtl.SetItemText(0,3,strdata.c_str());
				delete []data;
			}
			if(PackLen-offset>0)          
			{
				//��ִ��Ϣ                    String    (ͨ��516Э�鴦��)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 516:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			// ���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==0)               //0 �ɹ�
			{
				m_ListCtl.SetItemText(0,2,L"0,�ɹ�");
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 517:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			// ���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==0)               //0 �ɹ�
			{
				m_ListCtl.SetItemText(0,2,L"0,�ɹ�");
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 518:
		{
			m_ListCtl.SetItemText(0,1,L"2:���鷵��ͨ������");
			//����Ŀ             short   (<0 ʧ��  �����ʶ������Ϣ����)
			short ntolnum = *(short*)(PackData+offset);
			offset += 2;
			CString tolstr = _T("");tolstr.Format(L"%d",ntolnum);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,tolstr);
			if(PackLen-offset>0&&ntolnum>0)               //0 �ɹ�
			{
				//json����           string[]  // 
				short jsoncount = *(short*)(PackData+offset);
				offset += 2;
				CString jsonstr = _T("");jsonstr.Format(L"%d",jsoncount);
				m_ListCtl.SetItemText(0,3,jsonstr);
				for(int i=0; i<jsoncount; i++)
				{
					m_ListCtl.InsertItem(i+1,_T(""));
					CString xh = _T("");xh.Format(L"%d",i+2);
					m_ListCtl.SetItemText(0,0,xh);
					//�ⵥ��json������
					short jsonlen = *(short*)(PackData+offset);
					offset += 2;
					char *jsoncode = new char[jsonlen+1];
					memset(jsoncode,0,jsonlen+1);
					memcpy(jsoncode,PackData+offset,jsonlen);
					offset += jsonlen;
					wstring jsonStr = UTF8_2_UTF16(jsoncode);
					m_ListCtl.SetItemText(i,4,jsonStr.c_str());
					delete []jsoncode;
				}
			}
			if(PackLen-offset>0&&ntolnum<1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 519:
		{
			//����Ŀ             short   (<0 ʧ��  �����ʶ������Ϣ����)
			short ntolnum = *(short*)(PackData+offset);
			offset += 2;
			CString tolstr = _T("");tolstr.Format(L"%d",ntolnum);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,tolstr);
			if(PackLen-offset>0&&ntolnum>0)               //0 �ɹ�
			{
				//json����           string[]  // 
				short jsoncount = *(short*)(PackData+offset);
				offset += 2;
				CString jsonstr = _T("");jsonstr.Format(L"%d",jsoncount);
				m_ListCtl.SetItemText(0,3,jsonstr);
				for(int i=0; i<jsoncount; i++)
				{
					m_ListCtl.InsertItem(i+1,_T(""));
					CString xh = _T("");xh.Format(L"%d",i+2);
					m_ListCtl.SetItemText(0,0,xh);
					//�ⵥ��json������
					short jsonlen = *(short*)(PackData+offset);
					offset += 2;
					char *jsoncode = new char[jsonlen+1];
					memset(jsoncode,0,jsonlen+1);
					memcpy(jsoncode,PackData+offset,jsonlen);
					offset += jsonlen;
					wstring jsonStr = UTF8_2_UTF16(jsoncode);
					m_ListCtl.SetItemText(i,4,jsonStr.c_str());
					delete []jsoncode;
				}
			}
			if(PackLen-offset>0&&ntolnum<1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 520:
		{
			// ���                        byte         ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==0)               //0 �ɹ�
			{
				m_ListCtl.SetItemText(0,2,L"0,�ɹ�");
			}
			if(PackLen-offset>0&&nRes==1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 521:
		{
			//����Ŀ             short   (<0 ʧ��  �����ʶ������Ϣ����)
			short ntolnum = *(short*)(PackData+offset);
			offset += 2;
			CString tolstr = _T("");tolstr.Format(L"%d",ntolnum);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,tolstr);
			if(PackLen-offset>0&&ntolnum>0)               //0 �ɹ�
			{
				//json����           string[]  // 
				short jsoncount = *(short*)(PackData+offset);
				offset += 2;
				CString jsonstr = _T("");jsonstr.Format(L"%d",jsoncount);
				m_ListCtl.SetItemText(0,3,jsonstr);
				for(int i=0; i<jsoncount; i++)
				{
					m_ListCtl.InsertItem(i+1,_T(""));
					CString xh = _T("");xh.Format(L"%d",i+2);
					m_ListCtl.SetItemText(0,0,xh);
					//�ⵥ��json������
					short jsonlen = *(short*)(PackData+offset);
					offset += 2;
					char *jsoncode = new char[jsonlen+1];
					memset(jsoncode,0,jsonlen+1);
					memcpy(jsoncode,PackData+offset,jsonlen);
					offset += jsonlen;
					wstring jsonStr = UTF8_2_UTF16(jsoncode);
					m_ListCtl.SetItemText(i,4,jsonStr.c_str());
					delete []jsoncode;
				}
			}
			if(PackLen-offset>0&&ntolnum<1)          //1ʧ��
			{
				//�����                      int   
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString errstr = _T("");errstr.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,errstr);
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	case 300:
		{
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==0)
			{
				//Ԥ��ID    int      ���ɹ���ʱ�򣬷��ظ���ֵ��
				int Alarmval = *(int*)(PackData+offset);
				offset += 4;
				CString AlarmStr = _T("");AlarmStr.Format(L"%d",Alarmval);
				m_ListCtl.SetItemText(0,3,AlarmStr);
			}
			if(PackLen-offset>0&&nRes==1)
			{
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,3,wstr.c_str());
				delete []str;
			}
		}break;
	case 301:
		{
			unsigned char nResult = *(PackData+offset);
			offset += 1;
			int nRes = nResult;
			CString nResVal = _T("");nResVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,nResVal);
			if(PackLen-offset>0&&nRes==1)
			{
				//����      String   ��ʧ�ܵ�ʱ�򣬷���ʧ��ԭ��
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,3,wstr.c_str());
				delete []str;
			}
		}break;
	case 302:
		{
			//����Ŀ             short   (0 ʧ��  >0�ɹ�)
			unsigned short ntolnum = *(unsigned short*)(PackData+offset);
			offset += 2;
			CString ntolnumVal = _T("");ntolnumVal.Format(L"%d",ntolnum);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,ntolnumVal);
			if((PackLen-offset>0)&&ntolnum>0)
			{
				//�������󷵻���Ŀ   short 
				unsigned short num = *(unsigned short*)(PackData+offset);
				offset += 2;
				CString strnum = _T("");strnum.Format(L"%d",num);
				m_ListCtl.SetItemText(0,3,strnum);
				for(int i=0; i<num; i++)
				{
					m_ListCtl.InsertItem(i+1,_T(""));
					CString rownum = _T(""); rownum.Format(L"%d",i+2);
					m_ListCtl.SetItemText(i+1,0,rownum);
					if(PackLen-offset>0)
					{
						//��Ϣ����           int     (�������ֹɼ�Ԥ������Ϣ���� 0:�ɼ�Ԥ�� 1:��Ϣ����)
						unsigned int msgtype = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString msgstr = _T("");msgstr.Format(L"%d",msgtype);
						m_ListCtl.SetItemText(i,4,msgstr);
					}
					if(PackLen-offset>0)
					{
						//��Ʊ����           string
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring wcode = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(i,5,wcode.c_str());
						delete []code;
					}
					if(PackLen-offset>0)
					{
						//��Ʊ����           string
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring wname = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(i,6,wname.c_str());
						delete []name;
					}
					if(PackLen-offset>0)
					{
						//Ԥ����ʷ��Ϣ       String
						short hismsglen = *(short*)(PackData+offset);
						offset += 2;
						char *hismsg = new char[hismsglen+1];
						memset(hismsg,0,hismsglen+1);
						memcpy(hismsg,PackData+offset,hismsglen);
						offset += hismsglen;
						wstring whismsg = UTF8_2_UTF16(hismsg);
						m_ListCtl.SetItemText(i,7,whismsg.c_str());
						delete []hismsg;
					}
				}
			}
			if((PackLen-offset>0)&&ntolnum==0)
			{
				//����               String   (ʧ��ԭ��)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,3,wstr.c_str());
				delete []str;
			}
		}break;
	case 312:
		{
			//����Ŀ             short   (0 ʧ��  >0�ɹ�)
			unsigned short ntolnum = *(unsigned short*)(PackData+offset);
			offset += 2;
			CString ntolnumVal = _T("");ntolnumVal.Format(L"%d",ntolnum);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,ntolnumVal);
			if((PackLen-offset>0)&&ntolnum>0)
			{
				//�������󷵻���Ŀ   short 
				int noticeval = 0;  //��������
				unsigned short num = *(unsigned short*)(PackData+offset);
				offset += 2;
				CString strnum = _T("");strnum.Format(L"%d",num);
				m_ListCtl.SetItemText(0,3,strnum);
				for(int i=0; i<num; i++)
				{
					m_ListCtl.InsertItem(i+1,_T(""));
					CString rownum = _T(""); rownum.Format(L"%d",i+2);
					m_ListCtl.SetItemText(i+1,0,rownum);
					if(PackLen>offset)
					{
						// ��ϢID                      int
						int msgid = *(int*)(PackData+offset);
						offset += 4;
						CString msgidstr = _T("");msgidstr.Format(L"%d",msgid);
						m_ListCtl.SetItemText(i,4,msgidstr);
					}
					if(PackLen>offset)
					{
						// ��������                    byte         (0:��ͨ����[���Ӳ�����ʱ���Բ�����] 1:�����[���Ӳ���ΪURL])
						unsigned char noticetype = *(PackData+offset);
						offset += 1;
						noticeval = noticetype;
						CString notice = _T("");notice.Format(L"%d",noticeval);
						m_ListCtl.SetItemText(i,5,notice);
					}
					if(PackLen>offset)
					{
						if(noticeval == 0)
						{
							// ���Ӳ���                    string
							short pronamelen = *(short*)(PackData+offset);
							offset += 2;
							char *proname = new char[pronamelen+1];
							memset(proname,0,pronamelen+1);
							memcpy(proname,PackData+offset,pronamelen);
							offset += pronamelen;
							wstring wproname = UTF8_2_UTF16(proname);
							m_ListCtl.SetItemText(i,6,wproname.c_str());
							delete []proname;
						}
						if(noticeval == 1)
						{
							// ���Ӳ���                    string
							short urllen = *(short*)(PackData+offset);
							offset += 2;
							char *purl = new char[urllen+1];
							memset(purl,0,urllen+1);
							memcpy(purl,PackData+offset,urllen);
							offset += urllen;
							wstring wpurl = UTF8_2_UTF16(purl);
							m_ListCtl.SetItemText(i,6,wpurl.c_str());
							delete []purl;
						}
					}
					if(PackLen>offset)
					{
						// ��Ϣ����                    string
						short maglen = *(short*)(PackData+offset);
						offset += 2;
						char *msgstr = new char[maglen+1];
						memset(msgstr,0,maglen+1);
						memcpy(msgstr,PackData+offset,maglen);
						offset += maglen;
						wstring wmsgstr = UTF8_2_UTF16(msgstr);
						m_ListCtl.SetItemText(i,7,wmsgstr.c_str());
						delete []msgstr;
					}
				}
			}
			if((PackLen-offset>0)&&ntolnum==0)
			{
				//����               String   (ʧ��ԭ��)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,3,wstr.c_str());
				delete []str;
			}
		}break;
	case 313:
		{
			m_ListCtl.SetItemText(0,0,L"1");
			//�����             int 
			if(PackLen>offset)
			{
			    int errcode = *(int*)(PackData+offset);
			    offset += 4;
			    CString errcodestr = _T("");errcodestr.Format(L"%d",errcode);
			    m_ListCtl.SetItemText(0,1,errcodestr);
			}
			if(PackLen>offset)
			{
				//ʧ��ԭ�����ֱ�ʶ   String
				short errstrlen = *(short*)(PackData+offset);
				offset += 2;
				char *errstr = new char[errstrlen+1];
				memset(errstr,0,errstrlen+1);
				memcpy(errstr,PackData+offset,errstrlen);
				offset += errstrlen;
				wstring errUTF16 = UTF8_2_UTF16(errstr);
				m_ListCtl.SetItemText(0,2,errUTF16.c_str());
				delete []errstr;
			}
			if(PackLen>offset)
			{
				//���������         binary       (���������ԭ������)
				short errstrlen = *(short*)(PackData+offset);
				offset += 2;
				char *errstr = new char[errstrlen+1];
				memset(errstr,0,errstrlen+1);
				memcpy(errstr,PackData+offset,errstrlen);
				offset += errstrlen;
				wstring errUTF16 = UTF8_2_UTF16(errstr);
				m_ListCtl.SetItemText(0,3,errUTF16.c_str());
				delete []errstr;
			}
		}break;
	case 307:
		{
			//���               byte     ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nRes = *(PackData+offset);
			offset += 1;
			int Resval = nRes;
			CString ntolnumVal = _T("");ntolnumVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,ntolnumVal);
			if((PackLen-offset>0)&&Resval == 0)
			{
				//��Ŀ               short 
				unsigned short num = *(unsigned short*)(PackData+offset);
				offset += 2;
				CString strnum = _T("");strnum.Format(L"%d",num);
				m_ListCtl.SetItemText(0,3,strnum);
				for(int i=0; i<num; i++)
				{
					m_ListCtl.InsertItem(i+1,_T(""));
					CString rownum = _T(""); rownum.Format(L"%d",i+2);
					m_ListCtl.SetItemText(i+1,0,rownum);
					if(PackLen-offset>0)
					{
						//Ԥ��ID             int
						unsigned int msgtype = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString msgstr = _T("");msgstr.Format(L"%d",msgtype);
						m_ListCtl.SetItemText(i,4,msgstr);
					}
					if(PackLen-offset>0)
					{
						//��Ʊ����           string
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring wcode = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(i,5,wcode.c_str());
						delete []code;
					}
					if(PackLen-offset>0)
					{
						//��Ʊ����           string
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring wname = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(i,6,wname.c_str());
						delete []name;
					}
					if(PackLen-offset>0)
					{
						//�۸����           float     (-1��ʾδ�趨 ����ͬ)
						float highprice = *(float*)(PackData+offset);
						offset += sizeof(float);
						CString pricehigh = _T("");pricehigh.Format(L"%f",highprice);
						m_ListCtl.SetItemText(i,7,pricehigh);
					}
					if(PackLen-offset>0)
					{
						//�۸����           float
						float lowprice = *(float*)(PackData+offset);
						offset += sizeof(float);
						CString pricelow = _T("");pricelow.Format(L"%f",lowprice);
						m_ListCtl.SetItemText(i,8,pricelow);
					}
					if(PackLen-offset>0)
					{
						//���Ƿ�             float
						float increase = *(float*)(PackData+offset);
						offset += sizeof(float);
						CString increasestr = _T("");increasestr.Format(L"%f",increase);
						m_ListCtl.SetItemText(i,9,increasestr);
					}
					if(PackLen-offset>0)
					{
						//���Ƿ���Ч         byte     ��0 һ��  1 ���ã�
						unsigned char increasebyte = *(PackData+offset);
						offset += 1;
						int inamount = increasebyte;
						CString inamountstr = _T("");inamountstr.Format(L"%d",inamount);
						m_ListCtl.SetItemText(i,10,inamountstr);
					}
					if(PackLen-offset>0)
					{
						//�յ���             float
						float dropval = *(float*)(PackData+offset);
						offset += sizeof(float);
						CString dropvalstr = _T("");dropvalstr.Format(L"%f",dropval);
						m_ListCtl.SetItemText(i,11,dropvalstr);
					}
					if(PackLen-offset>0)
					{
						//�յ�����Ч         byte     ��0 һ��  1 ���ã�
						unsigned char dropbyte = *(PackData+offset);
						offset += 1;
						int dropamount = dropbyte;
						CString dropamountstr = _T("");dropamountstr.Format(L"%d",dropamount);
						m_ListCtl.SetItemText(i,12,dropamountstr);
					}
					if(PackLen-offset>0)
					{
						//�ջ�����           float
						float rateval = *(float*)(PackData+offset);
						offset += sizeof(float);
						CString ratevalstr = _T("");ratevalstr.Format(L"%f",rateval);
						m_ListCtl.SetItemText(i,13,ratevalstr);
					}
					if(PackLen-offset>0)
					{
						//�ջ�������Ч       byte     ��0 һ��  1 ���ã�
						unsigned char ratebyte = *(PackData+offset);
						offset += 1;
						int rateamount = ratebyte;
						CString rateamountstr = _T("");rateamountstr.Format(L"%d",rateamount);
						m_ListCtl.SetItemText(i,14,rateamountstr);
					}
					if(PackLen-offset>0)
					{
						// ÿ�յ���           byte     ��0 �ر�  1 ������
						unsigned char mine = *(PackData+offset);
						offset += 1;
						int mineamount = mine;
						CString mineamountstr = _T("");mineamountstr.Format(L"%d",mineamount);
						m_ListCtl.SetItemText(i,15,mineamountstr);
					}
					if(PackLen-offset>0)
					{
						//С��λ����         byte
						unsigned char decimal = *(PackData+offset);
						offset += 1;
						int decimalamount = decimal;
						CString decimalamountstr = _T("");decimalamountstr.Format(L"%d",decimalamount);
						m_ListCtl.SetItemText(i,16,decimalamountstr);
					}
				}
			}
			if((PackLen-offset>0)&&Resval==1)
			{
				//����               String   (ʧ��ԭ��)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,3,wstr.c_str());
				delete []str;
			}
		}break;
	case 308:
		{
			//���               byte     ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nRes = *(PackData+offset);
			offset += 1;
			int Resval = nRes;
			CString ntolnumVal = _T("");ntolnumVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,ntolnumVal);
			if(PackLen-offset>0&&Resval==1)
			{
				//����               String   (ʧ��ԭ��)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,3,wstr.c_str());
				delete []str;
			}
		}break;
	case 330:
		{
			//���               byte     ��0 �ɹ�  1ʧ�ܣ�
			unsigned char nRes = *(PackData+offset);
			offset += 1;
			int Resval = nRes;
			CString ntolnumVal = _T("");ntolnumVal.Format(L"%d",nRes);
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,2,ntolnumVal);
			if(PackLen-offset>0&&Resval==0)                               //�ɹ�
			{
				//�⵱ǰ����ֵ                  int   ��-1��δ֪   0���ر�   1��������
				int attrval = *(int*)(PackData+offset);
				offset += 4;
				CString AttrVal = _T(""); AttrVal.Format(L"%d",attrval);
				m_ListCtl.SetItemText(0,3,AttrVal);
			}
			if(PackLen-offset>0&&Resval==1)
			{
				//�����                      int  
				int errorno = *(int*)(PackData+offset);
				offset += 4;
				CString ErrorVal = _T(""); ErrorVal.Format(L"%d",errorno);
				m_ListCtl.SetItemText(0,3,ErrorVal);
				//����               String   (ʧ��ԭ��)
				short strlen = *(short*)(PackData+offset);
				offset += 2;
				char *str = new char[strlen+1];
				memset(str,0,strlen+1);
				memcpy(str,PackData+offset,strlen);
				offset += strlen;
				wstring wstr = UTF8_2_UTF16(str);
				m_ListCtl.SetItemText(0,4,wstr.c_str());
				delete []str;
			}
		}break;
	}
}


void CYunTestDlg::OnBnClickedBtnDisconnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_Mysock.Close(m_Mysock.m_Socket);
	m_Mysock.Close(m_Mysock.m_Socket1);
	GetDlgItem(IDC_BTN_DISCONNECT)->SetWindowText(L"�Ͽ��ɹ�");
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(0);
	GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(L"��������");
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(1);
	KillTimer(HEARTID);
	KillTimer(TUISONGID);
	KillTimer(AlarmID305);
	MapHeart.clear();
	vecHeart.clear();
	MapNotice.clear();
	heartcount = 0;
	noticecount = 0;
	if(send_Sock!=INVALID_SOCKET)
	{
		closesocket(send_Sock);
		send_Sock = INVALID_SOCKET;
	}
	DeleteCriticalSection(&m_Section);
	g_bClientClose = true;
	if(hEvent!=NULL)
	{
	    CloseHandle(hEvent);
	}
	if(m_Thread!=NULL)
	{
	    CloseHandle(m_Thread);
	}
}


void CYunTestDlg::OnBnClickedBtnRedis()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	wchar_t *redisipaddr = new wchar_t[50];
	memset(redisipaddr,0,50);
	::GetPrivateProfileString(L"REDIS",L"REDISIP",L"127.0.0.1",redisipaddr,20,L".\\HQ.INI");
	char *ipaddr = UTF16_2_UTF8(redisipaddr);
	int redisport = ::GetPrivateProfileInt(L"REDIS",L"REDISPORT",6379,L".\\HQ.INI");
	int redisconn = m_redis.RedisConn("10.15.207.97",6382);
	if(redisconn == 1)
	{
		delete []ipaddr;
		printf("connect redisserver failed");
		return;
	}
	delete []ipaddr;
	
	CString redisStr = _T("");
	GetDlgItem(IDC_EDIT_SENDDATA)->GetWindowText(redisStr);
	wchar_t *redisval = new wchar_t[1024];
	memset(redisval,0,1024);
	int szlen = redisStr.GetLength();
	for(int i=0;i<szlen;i++)
	{
		redisval[i] = redisStr.GetAt(i);
	}
	char *redisstr = UTF16_2_UTF8(redisval);
	int redis_mark = m_redis.RedisExcute(redisstr);
	switch(redis_mark)
	{
	case 1:     //�յ�����string
		{
			//��utf8תutf16
			wstring recvstr = UTF8_2_UTF16(m_redis.reply_str);
			GetDlgItem(IDC_EDIT_RECVDATA)->SetWindowText(recvstr.c_str());
		}break;
	case 2:   //�յ���������REDIS_REPLY_ARRAY
		{
			vector<string>::iterator arrayit;
			CString tempstring = _T("");
			
			for(unsigned int k=0;k<m_redis.RedisData.size();k++)
			{
				wstring cstr = UTF8_2_UTF16(m_redis.RedisData[k]);
				CString mag=_T("");mag.Format(L"%s",cstr.c_str());
				printf("keyֵ��%s\n",m_redis.RedisData[k].c_str());
				tempstring += mag;
				tempstring += L"  ";
			}
			/*
			for(arrayit=m_redis.RedisData.begin();arrayit!=m_redis.RedisData.end();arrayit++)
			{
				wstring cstr = UTF8_2_UTF16(*arrayit);
				CString mag=_T("");mag.Format(L"%ws",cstr.c_str());
				printf("�յ�����������:%s",*arrayit);
				tempstring += mag;
				tempstring += L"  ";
			}
			*/
			GetDlgItem(IDC_EDIT_RECVDATA)->SetWindowText(tempstring);
		}break;
	case 3:    //�յ�REDIS_REPLY_INTEGER
		{
			CString msg = _T("");msg.Format(L"%d",m_redis.reply_int);
			GetDlgItem(IDC_EDIT_RECVDATA)->SetWindowText(msg);
		}break;
	case 4:    //�յ����ǿհ�REDIS_REPLY_NIL
		{
			GetDlgItem(IDC_EDIT_RECVDATA)->SetWindowText(L"�յ�������");
		}break;
	case 5:   //�յ�����REDIS_REPLY_STATUS
		{
			wstring cstr = UTF8_2_UTF16(m_redis.redis_string);
			CString msg = _T("");msg.Format(L"%s",cstr.c_str());
			GetDlgItem(IDC_EDIT_RECVDATA)->SetWindowText(msg);
		}break;
	case 6:    //�յ����Ǵ�����ϢREDIS_REPLY_ERROR
		{
			wstring cstr = UTF8_2_UTF16(m_redis.reply_err);
			CString msg = _T("");msg.Format(L"%s",cstr.c_str());
			GetDlgItem(IDC_EDIT_RECVDATA)->SetWindowText(msg);
		}break;
	}
	return;
}


void CYunTestDlg::OnLvnItemchangedListData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}


void CYunTestDlg::OnCbnSelchangeComboFun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//��ʾ���༭�����
	for(int i=0; i<50; i++)
	{
		m_Edit[i].SetWindowText(_T(""));
		m_Edit[i].ShowWindow(SW_HIDE);
		m_Static[i].ShowWindow(SW_HIDE);
		
	}
	//��ȡ���ܺ�����combox�������е�λ��
	int nFunPos = m_ComBoxFun.GetCurSel();
	if(nFunPos == CB_ERR)
	{
		return;
	}
	//��ȡѡ��Ĺ��ܺ�
	int FunNo = Xml.XmlFun[nFunPos].FunID;
	CString FunNoSTR = _T("");FunNoSTR.Format(_T("%d"),FunNo);
	//��ȡ�����ֶ����ƺͶ�Ӧ��edit�༭�������
	int IDofNum = Xml.XmlFun[nFunPos].IDList.size();
	for(int k=0; k<IDofNum; k++)
	{
		CString ReqName = _T("");ReqName.Format(_T("%s"),Xml.XmlFun[nFunPos].IDList[k].Dictname);
		m_Static[k].SetWindowText(ReqName);
		m_Static[k].ShowWindow(SW_SHOW);
		//��ȡ�༭�������
		TCHAR EditCon[100] = {0};
		::GetPrivateProfileString(FunNoSTR,ReqName,L"",EditCon,100,L".\\HQYUN.ini");
		m_Edit[k].SetWindowText(EditCon);
		m_Edit[k].ShowWindow(SW_SHOW);
	}
	CString str=_T("");
	GenFunStr(FunNo,str);
}

void CYunTestDlg::OnChangeEdit(UINT nID)
{
	for(int i=0; i<50; i++)
	{
		if(m_Edit[i].GetDlgCtrlID() == nID)
		{
			m_Edit[i].SetModify(true);
		}
	}
	//����edit�༭���е�����
	int nFun = m_ComBoxFun.GetCurSel();     //�õ�combox��������combox��Ͽ��е�λ��
	int nFunIdx = Xml.XmlFun[nFun].FunID;
	CString edittext = _T("");
	//���ܺŵ��ַ���
	CString strFunNo = _T("");strFunNo.Format(_T("%d"),nFunIdx);
	for(unsigned int j=0; j<Xml.XmlFun[nFun].IDList.size(); j++)
	{
		m_Edit[j].GetWindowText(edittext);
		//�����ֶε�ֵ
		CString dictname = _T("");dictname.Format(_T(""),Xml.XmlFun[nFun].IDList[j].Dictname);
		::WritePrivateProfileString(strFunNo,dictname,edittext,L".\\HQYUN.ini");
	}
	CString str=_T("");
	GenFunStr(nFunIdx,str);
}


// ��������ʾ��edit�༭��
void CYunTestDlg::GenFunStr(int FunNo, CString & ReqString)
{
	int FunIdx = m_ComBoxFun.GetCurSel();     //��ȡ��ǰѡ�еĹ��ܺŵ�λ��
	FunNo = Xml.XmlFun[FunIdx].FunID;
	CString FunNoStr = _T("");FunNoStr.Format(L"%d",FunNo);
	//��ȡ���������ֵ�
	CString EditStr = _T("");
	for(unsigned int i=0; i<Xml.XmlFun[FunIdx].IDList.size(); i++)
	{
		//��ȡ�����ֶ�����
		CString tempname = _T("");tempname.Format(L"%s",Xml.XmlFun[FunIdx].IDList[i].Dictname);
		m_Edit[i].GetWindowText(EditStr);
		::WritePrivateProfileString(FunNoStr,tempname,EditStr,L".\\HQYUN.ini");
		CString Comreq = _T("");Comreq.Format(L"%s=%s",tempname,EditStr);
		ReqString += Comreq;
		ReqString += L"  ";
	}
	GetDlgItem(IDC_EDIT_SENDDATA)->SetWindowText(ReqString);
}


void CYunTestDlg::OnBnClickedBtnSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���
	int nFunIdx = m_ComBoxFun.GetCurSel();
	for(int i=0; i<50; i++)
	{
		CString EditStr = _T("");
		m_Edit[i].GetWindowText(EditStr);
		if(EditStr.GetLength()>0)
		{
			m_Edit[i].SetModify(true);
		}
	}
	//���Ͱ�֮ǰ�ж���socket�Ƿ���Ч
	if(!m_CheckPush.GetCheck())
	{
	    if(m_Mysock.m_Socket==SOCKET_ERROR)
	    {
		    m_Mysock.Close(m_Mysock.m_Socket);
		    printf("socket error\n");
		    return;
	    }
	}
	//���������ʵ�����
	int FunNo = Xml.XmlFun[nFunIdx].FunID;
	if(FunNo == 2972)
	{
		return;
	}
	GeneratePack(FunNo);
	if(FunNo == 305&&Fun == 3001)
	{
		TSHEART heartstruct;
		memset(heartstruct.HeartBuff,0,10*1024);
		heartstruct.heartlen = 0;
		memcpy(heartstruct.HeartBuff,SendBuffer,Datalen);
		heartstruct.heartlen = Datalen;
		vecHeart.push_back(heartstruct);
		MapHeart.insert(pair<int,TSHEART>(heartcount,heartstruct));
		heartcount++;
	}
	if(FunNo == 102&&Fun == 3001)
	{
		NOTICEHEART noticestruct;
		memset(noticestruct.NoticeBuff,0,10*1024);
		noticestruct.noticelen = 0;
		memcpy(noticestruct.NoticeBuff,SendBuffer,Datalen);
		noticestruct.noticelen = Datalen;
		MapNotice.insert(pair<int,NOTICEHEART>(noticecount,noticestruct));
		noticecount++;
	}
	if(Fun == 3001)
	{
		//�������ͽӿڰ���Ϣ
		int nBytes = 0,nsum = 0;
		if((FunNo != 305) || (FunNo!=102))
		{
		     while(Datalen>0)
		     {
		         int sendret = send(send_Sock,SendBuffer+nBytes,Datalen,0);
			     Datalen -= sendret;
			     nBytes += sendret;
		    }
		}
	}
	else
	{
	    //������������ݰ�
	    if(m_Mysock.SendPack(SendBuffer,Datalen)<=0)
	    {
		    printf("send data fail\n");
		    return;
	    }
	    //���հ�ͷ���ݣ����ڰ�ͷ���������ֵ���б仯�������հ�ʱ���հ�ͷ������ֽ���
	    char RecvHead[15] = {0};
	    int recvdatalen = 0;
	
	    //char RecvHead[sizeof(HQ_HEAD)+1] = {0};
	    if(m_Mysock.RecvPack(RecvHead,sizeof(HQ_HEAD))<=0)
	    {
		    printf("�հ�ʧ��:%d\n",WSAGetLastError());
		    return;
	    }
	    HQ_HEAD *pRecvHead = (HQ_HEAD*)RecvHead;
	    printf("�յ�����������:%d,�յ����ĳ���:%d,�յ�������:%d\n",pRecvHead->type,pRecvHead->length,pRecvHead->attrs);
	    int compress = pRecvHead->attrs&0x2;
	    int pml = pRecvHead->attrs&0x8;
	    printf("����ַ������������Ƿ���intΪ:%d,ѹ����־:%d\n",pml,compress);
	    //�հ���
	    char *RecvBody = new char[pRecvHead->length+1];
	    memset(RecvBody,0,pRecvHead->length+1);
	    if(pRecvHead->length>0)
	    {
		    int bodylen = m_Mysock.RecvPack(RecvBody,pRecvHead->length);
	        if(bodylen<=0)
	        {
		        printf("�հ���ʧ��\n");
		        return;
	        }
	    }
	    //�����
	    if(pRecvHead->type>=1000&&pRecvHead->type<=2967&&pRecvHead->type!=2961)
	    {
	        AyncPack(RecvBody,pRecvHead->length,pRecvHead->type);
	    }
	    else
	    {
		    DealPack(RecvBody,pRecvHead->length,pRecvHead->type);
	    }
	    delete []RecvBody;
	}
}


// ������������ݹ��ܺ�����������������ݶ�����������
void CYunTestDlg::GeneratePack(int FunId)
{
	memset(SendBuffer,0,10*1024);
	Datalen=0;
	if(m_CheckPush.GetCheck())
	{
		struct ACC_CMDHEADEX *pPushHead = (struct ACC_CMDHEADEX*)SendBuffer;
		Datalen += sizeof(ACC_CMDHEADEX);
	}
	else
	{
	    struct HQ_HEAD *pHQHead = (struct HQ_HEAD*)SendBuffer;
	    Datalen += sizeof(struct HQ_HEAD);
		if(Fun == 3001)
		{
			*(SendBuffer+Datalen) = 2;
			Datalen += 1;
		}
	}
	int nComboxIndex = m_ComBoxFun.GetCurSel();
	int nFun = Xml.XmlFun[nComboxIndex].FunID;
//	Datalen += sizeof(struct HQ_HEAD);
//	if(Fun == 3001)
//	{
//	    *(SendBuffer+Datalen) = 2;
//	    Datalen += 1;
//	}
	sub_tuisong_head *pTSHead = (sub_tuisong_head*)(SendBuffer+Datalen);
	if(Fun == 3001)
	{
		Datalen += sizeof(struct sub_tuisong_head);
	}
	//�������ֵ�ֵ
	int DictSize = Xml.XmlFun[nComboxIndex].IDList.size();
	CString EditFiled = _T("");
	string temputf;     //req_stringֵ
	float tempfloat = 0;
	string tempUTF;
	for(int i=0; i<DictSize; i++)
	{
		int f=0;
		for(f=0;f<=17;f++)
		{
			if(_tcscmp(Xml.XmlFun[nComboxIndex].IDList[i].Dicttype,pReqStruct[f])==0)
				break;
		}
		switch(f)
		{
		case req_string:     //����string����
			{
				m_Edit[i].GetWindowText(EditFiled);
		        //unicode����תUTF8��ʽ��
		        tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				if(tempUTF.length()!=0)
				{
				    *(short*)(SendBuffer+Datalen) = tempUTF.length();
				    Datalen += sizeof(unsigned short);
				    memcpy(SendBuffer+Datalen,tempUTF.c_str(),tempUTF.length());
				    Datalen += tempUTF.length(); 
				}
			}break;
		case req_account:     //����string����
			{
				string pushuser("sunlingdan");
				char addstr[10] = {0};
				sprintf(addstr,"%d",accountcount);
				pushuser.append(addstr,strlen(addstr));
				if(pushuser.length()!=0)
				{
					*(short*)(SendBuffer+Datalen) = pushuser.length();
					Datalen += sizeof(unsigned short);
					memcpy(SendBuffer+Datalen,pushuser.c_str(),pushuser.length());
					Datalen += pushuser.length(); 
				}
				accountcount = accountcount+1;
			}break;
		case req_byte:   //byte����
			{
				m_Edit[i].GetWindowText(EditFiled);
				if(EditFiled.GetLength()!=0)
				{
				    unsigned char szbyte = _ttoi(EditFiled);
				    *(SendBuffer+Datalen) = szbyte;
				    Datalen += 1;
				}
			}break;
		case req_char:      //charһ���ַ�
			{
				m_Edit[i].GetWindowText(EditFiled);
				char szchar = _ttoi(EditFiled);
				*(SendBuffer+Datalen) = szchar;
				Datalen += 1;
			}break;
		case req_short:    //short��2λ
			{
				m_Edit[i].GetWindowText(EditFiled);
		        //unicode����תUTF8��ʽ��
		        tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				if(!tempUTF.empty())
				{
				short tempshort = 0;
				if(nFun == 2956)
				{
					memcpy(&tempshort,tempUTF.c_str(),2);
				}
				else
				{
			        tempshort = (short)atoi(tempUTF.c_str());
				}
				*(short*)(SendBuffer+Datalen) = tempshort;
				Datalen += 2;
				}
			}break;
		case req_float:    //float��4λ
			{
				m_Edit[i].GetWindowText(EditFiled);
				//unicode����תUTF8��ʽ��
				temputf = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
				EditFiled.ReleaseBuffer();
				if(nFun == 2956)
				{
					memcpy(&tempfloat,temputf.c_str(),sizeof(float));
				}
				else
				{
					tempfloat = (float)atof(temputf.c_str());
				}
				*(float*)(SendBuffer+Datalen) = tempfloat;
				Datalen += sizeof(float);
			}break;
		case req_int:    //int��4λ
			{
				m_Edit[i].GetWindowText(EditFiled);
		        //unicode����תUTF8��ʽ��
		        tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				int tempint = atoi(tempUTF.c_str());
				*(int*)(SendBuffer+Datalen) = tempint;
				Datalen += 4;
			}break;
		case req_uint:    //int��4λ
			{
				m_Edit[i].GetWindowText(EditFiled);
				//unicode����תUTF8��ʽ��
				tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
				EditFiled.ReleaseBuffer();
				unsigned int tempuint = atoi(tempUTF.c_str());
				*(unsigned int*)(SendBuffer+Datalen) = tempuint;
				Datalen += 4;
				if(nFun == 102)
				{
					*(unsigned int*)(SendBuffer+Datalen) = noticeid;
					Datalen += 4;
				}
			}break;
		case req_intarray:           //int������
			{
				m_Edit[i].GetWindowText(EditFiled);
		        //unicode����תUTF8��ʽ��
		        tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				if(tempUTF.length()!=0)
				{
				int templen = Datalen;
				//��ֵ�ַ����ĳ��ȣ�����ȡ�����ַ������飬�˲���û�м��������ֵ���ȰѴ�λ�ÿ�λ���ַ�������ֵ����
				Datalen += sizeof(short);
				int pos=0,strnum=0;
				//�����ַ�������ʼλ�ã������ַ�������ֹλ�ã��ж��ַ����ĳ���ʹ��
				//��ֹ���һ������ֵû��ȡ�����Զ��Ž���
				int strpos=0,strend=0;
				string tempstring;
				for(unsigned int k=0; k<tempUTF.size(); )
				{
					if(tempUTF[k] == ',')
					{
						strend = k-strpos;
						char *tempstr = new char[strend+1];
						memset(tempstr,0,strend+1);
						tempUTF.copy(tempstr,strend,strpos);
						int arrayval = atoi(tempstr);
						*(int*)(SendBuffer+Datalen) = arrayval;
						Datalen += sizeof(int);
						delete []tempstr;
						k++;
						strpos = k;
						strnum += 1;
					}
					else
					{
						k++;
					}
				}
				*(short*)(SendBuffer+templen) = strnum;
				}
			}break;
		case req_shortarray:           //short������
			{
				m_Edit[i].GetWindowText(EditFiled);
				//unicode����תUTF8��ʽ��
				tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
				EditFiled.ReleaseBuffer();
				if(tempUTF.length()!=0)
				{
					int templen = Datalen;
					//��ֵ�ַ����ĳ��ȣ�����ȡ�����ַ������飬�˲���û�м��������ֵ���ȰѴ�λ�ÿ�λ���ַ�������ֵ����
					Datalen += sizeof(short);
					int pos=0,strnum=0;
					//�����ַ�������ʼλ�ã������ַ�������ֹλ�ã��ж��ַ����ĳ���ʹ��
					//��ֹ���һ������ֵû��ȡ�����Զ��Ž���
					int strpos=0,strend=0;
					short arrayval = 0;
					string tempstring;
					for(unsigned int k=0; k<tempUTF.size(); )
					{
						if(tempUTF[k] == ',')
						{
							strend = k-strpos;
							char *tempstr = new char[strend+1];
							memset(tempstr,0,strend+1);
							tempUTF.copy(tempstr,strend,strpos);
							arrayval = atoi(tempstr);
							memcpy(&arrayval,tempstr,sizeof(short));
							printf("shortֵ:%d\n",arrayval);
							
							*(short*)(SendBuffer+Datalen) = arrayval;
						//	memcpy(SendBuffer+Datalen,&arrayval,
							
							Datalen += 2;
							delete []tempstr;
							k++;
							strpos = k;
							strnum += 1;
						}
						else
						{
							k++;
						}
					}
					*(short*)(SendBuffer+templen) = strnum;
					printf("short���ݸ���:%d\n",strnum);
				}
			}break;
		case req_stringarray:      //�ַ�����������
			{
				m_Edit[i].GetWindowText(EditFiled);
		        //unicode����תUTF8��ʽ��
		        tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				if(tempUTF.length()!=0)
				{
				int templength = Datalen;  //���������֪�����ٸ����Ȱѳ������λ�ÿ�λ
				//��ֵ�ַ����ĳ��ȣ�����ȡ�����ַ������飬�˲���û�м��������ֵ���ȰѴ�λ�ÿ�λ���ַ�������ֵ����
				Datalen += sizeof(short);
				int pos=0,strnum=0;
				//�����ַ�������ʼλ�ã������ַ�������ֹλ�ã��ж��ַ����ĳ���ʹ��
				//��ֹ���һ������ֵû��ȡ�����Զ��Ž���
				int strpos=0,strend=0;
				string tempstring;
				for(unsigned int k=0; k<tempUTF.size(); )
				{
					if(tempUTF[k] == ',')
					{
						strend = k-strpos;
						char *tempstr = new char[strend+1];
						memset(tempstr,0,strend+1);
						tempUTF.copy(tempstr,strend-strpos,strpos);
						*(short*)(SendBuffer+Datalen) = strend;
						Datalen += 2;
						memcpy(SendBuffer+Datalen,tempstr,strend);
						Datalen += strend-strpos;
						delete []tempstr;
						k++;
						strpos = k;
						strnum += 1;
					}
					else
					{
						k++;
					}
				}
				*(short*)(SendBuffer+templength) = strnum;
				}
			}break;
		case req_time:     //������int��
			{
				m_Edit[i].GetWindowText(EditFiled);
				tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				int temptime = atoi(tempUTF.c_str());
				*(int*)(SendBuffer+Datalen) = temptime;
				Datalen += sizeof(int);
			}break;
		case req_long64:   //64λint��
			{
				m_Edit[i].GetWindowText(EditFiled);
				tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				__int64 temp64 = _atoi64(tempUTF.c_str());
				Datalen += sizeof(__int64);
			}break;
		case req_char2:    //2���ֽڵ�char����
			{
				m_Edit[i].GetWindowText(EditFiled);
				tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				*(short*)(SendBuffer+Datalen) = 2;
				Datalen += 2;
				memcpy(SendBuffer+Datalen,tempUTF.c_str(),2);
				Datalen += 2;
			}break;
		case req_char3:     //Ҳ��char�����ֶ�
			{
				m_Edit[i].GetWindowText(EditFiled);
				tempUTF = UTF16_2_UTF8(EditFiled.GetBuffer(EditFiled.GetLength()));
		        EditFiled.ReleaseBuffer();
				char sztemp = atoi(tempUTF.c_str());
				*(SendBuffer+Datalen) = sztemp;
				Datalen += 1;
			}break;
		case req_130data:    //2972�ӿڣ����˻�֮���ͨѶ��������   //��Կ���ܺ�����ģ�short + 128byte,�˲��ֵ���д���Ա��ڷֿ�ʹ��
			{
			}break;
		case req_encrypass:    // //2972�ӿڣ����˻�֮���ͨѶ��������   //��Կ���ܺ�����ģ�short + 128byte,�˲��ֵ���д���Ա��ڷֿ�ʹ��
			{
			}break;
		default:
			break;
		}
	}
	//���а���}��β������Ҫ�Խ�β��}�ַ�
	*(SendBuffer+Datalen) = '}';
	Datalen += 1;

	if(m_CheckPush.GetCheck())
	{
		struct ACC_CMDHEADEX *pPushHead = (struct ACC_CMDHEADEX*)SendBuffer;
		pPushHead->wCmdType = 0xa0a1;
		pPushHead->wAttr = 0x8200;
		pPushHead->nExpandInfo = 0;
		pPushHead->nLen = Datalen - sizeof(ACC_CMDHEADEX) -1;
		switch(Fun)
		{
		case 3001:
			{
				pTSHead->sub_type = FunId;                                                //С��ͷ����
				pTSHead->sub_length = Datalen-sizeof(struct ACC_CMDHEADEX)-sizeof(sub_tuisong_head)-1;      //С��ͷ����
				if(FunId==517||FunId==518||FunId==519||FunId==520||FunId==521||FunId==330)
				{
					pTSHead->sub_attrs = 0x0001;                                                          //С��ͷ����
				}
				else
				{
					pTSHead->sub_attrs = 0;
				}
				pTSHead->sub_extend = 0;                                                             //С��ͷ��չ����
				Datalen -= 1;
			}break;
		}
	}
	else
	{
		struct HQ_HEAD *pHQHead = (struct HQ_HEAD*)SendBuffer;
		//����ͷ
		pHQHead->cSparate = '{';
		pHQHead->attrs = 0;
		switch(Fun)
		{
		case 1000:
			{
				pHQHead->type = FunId;
				pHQHead->length = Datalen - sizeof(struct HQ_HEAD)-1;
				Datalen -= 1;
			}break;
		case 3001:
			{
				pHQHead->type = 3001;                                                     //���ͷ����
				pHQHead->length = Datalen - sizeof(struct HQ_HEAD)-1;                     //���ͷ����
				pTSHead->sub_type = FunId;                                                //С��ͷ����
				pTSHead->sub_length = Datalen-sizeof(struct HQ_HEAD)-sizeof(sub_tuisong_head)-1-1;      //С��ͷ����
				if(FunId==517||FunId==518||FunId==519||FunId==520||FunId==521)
				{
					pTSHead->sub_attrs = 0x0001;                                                          //С��ͷ����
				}
				else
				{
					pTSHead->sub_attrs = 0;
				}
				pTSHead->sub_extend = 0;                                                             //С��ͷ��չ����
				Datalen -= 1;
			}break;
		}
	}
	
	/*
	//����ͷ
	pHQHead->cSparate = '{';
	pHQHead->attrs = 0;
	//pHQHead->type = FunId;
	switch(Fun)
	{
	case 1000:
		{
			pHQHead->type = FunId;
	        pHQHead->length = Datalen - sizeof(struct HQ_HEAD)-1;
			Datalen -= 1;
		}break;
	case 3001:
		{
			pHQHead->type = 3001;                                                     //���ͷ����
			pHQHead->length = Datalen - sizeof(struct HQ_HEAD)-1;                     //���ͷ����
			pTSHead->sub_type = FunId;                                                //С��ͷ����
	        pTSHead->sub_length = Datalen-sizeof(struct HQ_HEAD)-sizeof(sub_tuisong_head)-1-1;      //С��ͷ����
			if(FunId==517||FunId==518||FunId==519||FunId==520||FunId==521)
			{
			    pTSHead->sub_attrs = 0x0001;                                                          //С��ͷ����
			}
			else
			{
				pTSHead->sub_attrs = 0;
			}
			pTSHead->sub_extend = 0;                                                             //С��ͷ��չ����
			Datalen -= 1;
		}break;
	}
	*/
}

// �������
void CYunTestDlg::AyncPack(char * &PackBody, unsigned short PackLen, unsigned short PackType)
{
	//�����������ÿһ�������н���������ĵ��Ƚϻ�����һ�����Ľ���Ƚ�����������ȽϷ���
	m_ListCtl.DeleteAllItems();
	int nHeadNum = m_ListCtl.GetHeaderCtrl()->GetItemCount();
	for(int i=nHeadNum-1; i>=0; i--)
	{
        m_ListCtl.DeleteColumn (i);
	}
	int offset=0;
	for(int n=1;n<=500;n++)
    {
        m_ListCtl.InsertItem(n,_T(""));
		m_ListCtl.SetItemText(n,0,(LPCTSTR)(n+1));
    }
	//����Ӧ���ֵ�������listctrl����
	//���幦�ܺ�����Ӧ��combox��Ͽ�������λ��
	m_ListCtl.InsertColumn(0,L"���",LVCFMT_LEFT,50);

	switch(PackType)
	{
	case 1001:
		{
			char nflag = *(PackBody+offset);
			int nMark = nflag;
			if(nMark == 0)
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			else
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
		}break;
	case 1004:
		{
			int idvalue = *(int*)(SendBuffer+sizeof(HQ_HEAD));
			if(idvalue == 0)
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			else
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
		}break;
	case 2331:
		{
			char FundType = *(PackBody+offset);
			int Fundint = FundType;
			if(Fundint!=5)
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			if(Fundint==5)
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
		}break;
	case 2935:
		{
			char nflag = *(PackBody+offset);
			int nMark = nflag;
			if(nMark==0)
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			if(nMark!=0)
			{
				for(unsigned int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
		}break;
	default:
		{
			for(unsigned int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	        {
		        m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	        }
		}break;
	}
	printf("��listctrl�ؼ��б����\n");
	switch(PackType)
	{
	case 1000:
		{
			//����Ӧ������ֶθ���,map��������ĸ���
			m_ListCtl.InsertItem(0,_T(""));
			//��Ӧ�������������ַ�ֶΣ����ֶ����ַ��������ʽstring����
			//�������������
			short hqaddrnum = *(short*)(PackBody+offset);
			offset += 2;
			//�������ַ�����ֶ�
			int nRow = 0;
			if(PackLen>offset)
			{
				for(int j=0;j<hqaddrnum;j++)
				{
					short hqaddrlength = *(short*)(PackBody+offset);
					offset += 2;
					char *hqaddr = new char[hqaddrlength+1];
					memset(hqaddr,0,hqaddrlength+1);
					memcpy(hqaddr,PackBody+offset,hqaddrlength);
					offset += hqaddrlength;
					wstring str = UTF8_2_UTF16(hqaddr);
					CString No = _T("");No.Format(L"%d",j+1);
					//nRow = m_ListCtl.InsertItem(j,No);// ������
					m_ListCtl.SetItemText(j,0,No);
					m_ListCtl.SetItemText(j, 1, str.c_str());//��������������
					delete []hqaddr;
				}
			}
			//����ί�е�ַ�ֶ�
			if(PackLen>offset)
			{
				short Entrustaddrnum = *(short*)(PackBody+offset);   //ί�е�ַ����
				offset += 2;
				for(int t=0;t<Entrustaddrnum;t++)
				{
					short enlength = *(short*)(PackBody+offset);
					offset += 2;
					char *Enaddr = new char[enlength+1];
					memset(Enaddr,0,enlength+1);
					memcpy(Enaddr,PackBody+offset,enlength);
					offset += enlength;
					wstring str = UTF8_2_UTF16(Enaddr);
					CString No = _T("");No.Format(L"%d",t+1);
					m_ListCtl.SetItemText(t,0,No);
					m_ListCtl.SetItemText(t, 2, str.c_str());//��������������
					delete []Enaddr;
				}
			}
			//�⹫����Ϣ  string����
			if(PackLen>offset)
			{
				short Noticelength = *(short*)(PackBody+offset);   //������Ϣ�ַ�������
				offset += 2;
				if(Noticelength!=0)
				{
					char *NoticeMsg = new char[Noticelength+1];
					memset(NoticeMsg,0,Noticelength+1);
					memcpy(NoticeMsg,PackBody+offset,Noticelength);
					offset+=Noticelength;
					wstring str = UTF8_2_UTF16(NoticeMsg);
					CString wNotice = _T("");wNotice.Format(L"%ws",str.c_str());
					m_ListCtl.SetItemText(0, 3, wNotice);//��������������
					delete []NoticeMsg;
				}
			}
			//���°汾��       String
			if(PackLen>offset)
			{
				short Verlength = *(short*)(PackBody+offset);   //������Ϣ�ַ�������
				offset += 2;
				char *VerMsg = new char[Verlength+1];
				memset(VerMsg,0,Verlength+1);
				memcpy(VerMsg,PackBody+offset,Verlength);
				offset+=Verlength;
				wstring Verstr = UTF8_2_UTF16(VerMsg);
				m_ListCtl.SetItemText(0, 4, Verstr.c_str());//��������������
				delete []VerMsg;
			}
			//�����ص�ַ       String
			if(PackLen>offset)
			{
				short Addrlength = *(short*)(PackBody+offset);   //������Ϣ�ַ�������
				offset += 2;
				char *AddrMsg = new char[Addrlength+1];
				memset(AddrMsg,0,Addrlength+1);
				memcpy(AddrMsg,PackBody+offset,Addrlength);
				offset+=Addrlength;
				wstring Addrstr = UTF8_2_UTF16(AddrMsg);
				m_ListCtl.SetItemText(0, 5, Addrstr.c_str());//��������������
				delete []AddrMsg;
			}
			//��������       byte
			if(PackLen>offset)
			{
				unsigned char UpdateMsg = *(PackBody+offset);
				offset += 1;
				int tempch = UpdateMsg;
				CString tempstr = _T("");tempstr.Format(L"%d",tempch);
				m_ListCtl.SetItemText(0, 6, tempstr);//��������������
			}
			//ǿ������       byte      1ǿ�� 0��ǿ��
			if(PackLen>offset)
			{
				unsigned char FuMsg = *(PackBody+offset);
				offset += 1;
				int tempFU = FuMsg;
				CString FU = _T("");FU.Format(L"%d",tempFU);
				m_ListCtl.SetItemText(0, 7, FU);//��������������
			}
			//�Ƿ���ʾ��¼   byte      1��ʾ 0����ʾ�����û�и�λ��ʾ����ʾ
			if(PackLen>offset)
			{
				unsigned char NLMsg = *(PackBody+offset);
				offset += 1;
				int tempNL = NLMsg;
				CString NL = _T("");NL.Format(L"%d",tempNL);
				m_ListCtl.SetItemText(0, 8, NL);//��������������
			}
			//�û���Ӫ��ip�� byte      0��ʾδ֪����0��ʾ��Ч��
			if(PackLen>offset)
			{
				unsigned char IPMsg = *(PackBody+offset);
				offset += 1;
				int tempIP = IPMsg;
				CString IPstr = _T("");IPstr.Format(L"%d",tempIP);
				m_ListCtl.SetItemText(0, 9, IPstr);//��������������
			}

			//ͳ����Ϣʱ���� short   ��λ��,���Ϊ0��ʾ��ͳ����Ϣ.	//20120307������
			if(PackLen>offset)
			{
				short Timestap = *(short*)(PackBody+offset);
				offset += 2;
				CString TStamp = _T("");TStamp.Format(L"%u",Timestap);
				m_ListCtl.SetItemText(0, 10, TStamp);//��������������
			}
			//������ʾ����   String
			if(PackLen>offset)
			{
				short UNlength = *(short*)(PackBody+offset);   //������Ϣ�ַ�������
				offset += 2;
				char *UNMsg = new char[UNlength+1];
				memset(UNMsg,0,UNlength+1);
				memcpy(UNMsg,PackBody+offset,UNlength);
				offset+=UNlength;
				wstring UNstr = UTF8_2_UTF16(UNMsg);
				m_ListCtl.SetItemText(0, 11, UNstr.c_str());//��������������
				delete []UNMsg;
			}
			//����crc        short 
			if(PackLen>offset)
			{
				short CRCValue = *(short*)(PackBody+offset);
				offset += 2;
				CString tempCRC = _T("");tempCRC.Format(L"%u",CRCValue);
				m_ListCtl.SetItemText(0, 12, tempCRC);//��������������
			}
			//������ʾ����   char	//0����û�и��ֶα�ʾǿ�Ƶ�����1ÿ�յ���һ�Σ�2ͬһ����ֻ����һ�Σ�3����������
			if(PackLen>offset)
			{
				unsigned char NoticeType = *(PackBody+offset);
				offset += 1;
				int tempNotice = NoticeType;
				CString Notice = _T("");Notice.Format(L"%d",tempNotice);
				m_ListCtl.SetItemText(0, 13, Notice);//��������������
			}

			//���ȵ�ַ       String[]  ��ַ��ʽΪ ip:port:id  //id �����ӣ�id����Ӫ��id 2015-3-23
			if(PackLen>offset)
			{
				short Addrnum = *(short*)(PackBody+offset);   //ί�е�ַ����
				offset += 2;
				for(int f=0;f<Addrnum;f++)
				{
					short Addrlength = *(short*)(PackBody+offset);
					offset += 2;
					char *Addr = new char[Addrlength+1];
					memset(Addr,0,Addrlength+1);
					memcpy(Addr,PackBody+offset,Addrlength);
					offset += Addrlength;
					wstring str = UTF8_2_UTF16(Addr);
					CString No = _T("");No.Format(L"%d",f+1);

					m_ListCtl.SetItemText(f,0,No);
					m_ListCtl.SetItemText(f, 14, str.c_str());//��������������
					delete []Addr;
				}
			}
			//���в�ͬ�����ַ�б���  short
			if(PackLen>offset)
			{
				short ServerNum = *(short*)(PackBody+offset);
				offset += 2;
				CString tempSvr = _T("");tempSvr.Format(L"%u",ServerNum);
				m_ListCtl.SetItemText(0, 15, tempSvr);//��������������
			}
			//����id             int
			if(PackLen>offset)
			{
				int ServerID = *(int*)(PackBody+offset);
				offset += 4;
				CString tempSvrID = _T("");tempSvrID.Format(L"%d",ServerID);
				m_ListCtl.SetItemText(0, 16, tempSvrID);//��������������
			}
			//��������ַ         String[]	//��������ַ��ʽ��ip:port:id
			if(PackLen>offset)
			{
				short Servernum = *(short*)(PackBody+offset);  
				offset += 2;
				for(int m=0;m<Servernum;m++)
				{
					short Serlength = *(short*)(PackBody+offset);
					offset += 2;
					char *Ser = new char[Serlength+1];
					memset(Ser,0,Serlength+1);
					memcpy(Ser,PackBody+offset,Serlength);
					offset += Serlength;
					wstring str = UTF8_2_UTF16(Ser);
					CString No = _T("");No.Format(L"%d",m+1);
					m_ListCtl.SetItemText(m,0,No);
					m_ListCtl.SetItemText(m, 17, str.c_str());//��������������
					delete []Ser;
				}
			}
		}break;
	case 1001:
		{
			//�Ƿ���ʾ��¼   byte      1��ʾ 0����ʾ�����û�и�λ��ʾ����ʾ
			if(PackLen>offset)
			{
				char nflag = *(PackBody+offset);
				offset += 1;
				int nMark = nflag;
				if(nMark == 0)
				{
					//����Ӧ������ֶθ���,map��������ĸ���
					CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
					m_ListCtl.SetItemText(0, 1, Flag);//��������������
					//ȯ������	short��������Ŀ��
					short QSnum = *(short*)(PackBody+offset);
					CString QS = _T("");QS.Format(L"%d",QSnum);
					m_ListCtl.SetItemText(0, 2, QS);
					offset += 2;
					for(int i=0; i<QSnum; i++)
					{
						short strlen = *(short*)(PackBody+offset);
						offset += 2;
						char *QSValue = new char[strlen+1];
						memset(QSValue,0,strlen+1);
						memcpy(QSValue,PackBody+offset,strlen);
						wstring cstr = UTF8_2_UTF16(QSValue);
						CString No = _T("");No.Format(L"%d",i+1);
						m_ListCtl.SetItemText(i,0,No);
						m_ListCtl.SetItemText(i,3,cstr.c_str());
						offset += strlen;
						delete []QSValue;
					}
				}
				if(nMark == 1)
				{
					CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
					m_ListCtl.SetItemText(0, 1, Flag);//��������������
					//�����ַ���		char	//1��ʾ��ȯ�������ַ��0��ʾ����ȯ�������ַ
					if(PackLen>offset)
					{
						char hqmark = *(PackBody+offset);
						offset += 1;
						int HQmark = hqmark;
						CString HQflag = _T("");HQflag.Format(L"%d",HQmark);
						m_ListCtl.SetItemText(0, 2, HQflag);//��������������
					}
					//ί���Ƿ�֧�ֻ��ֵ�	char	//1��ʾ֧�ֻ��ֵ䣬0��ʾ��֧��
					if(PackLen>offset)
					{
						char ISDict = *(PackBody+offset);
						offset += 1;
						int DICT = ISDict;
						CString isDict = _T("");isDict.Format(L"%d",DICT);
						m_ListCtl.SetItemText(0, 3, isDict);//��������������
					}
					//ί�е�ַ��Ŀ��short��������Ŀ��
					if(PackLen>offset)
					{
						short WTnum = *(short*)(PackBody+offset);
						offset += 2;
						CString wtcount = _T("");wtcount.Format(L"%d",WTnum);
						m_ListCtl.SetItemText(0, 4, wtcount);//��������������
						//ί�е�ַ	string
						if(PackLen>offset)
						{
							for(int q=0;q<WTnum;q++)
							{
								short WTlen = *(short*)(PackBody+offset);
								offset += 2;
								char *wtaddr = new char[WTlen+1];
								memset(wtaddr,0,WTlen+1);
								memcpy(wtaddr,PackBody+offset,WTlen);
								offset += WTlen;
								wstring addrstr = UTF8_2_UTF16(wtaddr);
								CString xh = _T("");xh.Format(L"%d",q+1);
								m_ListCtl.SetItemText(q,0,xh);
								m_ListCtl.SetItemText(q,5,addrstr.c_str());
								delete []wtaddr;
								//��ί�ж˿�	short
								short WTport = *(short*)(PackBody+offset);
								offset += 2;
								CString wtport = _T("");wtport.Format(L"%d",WTport);
								m_ListCtl.SetItemText(q,6,wtport);
							}
						}
					}
					//ȯ����������������ַ����short	//��������ַ���Ϊ0��û�иü������ֶ�	
					if(PackLen>offset)
					{
						short HQnum = *(short*)(PackBody+offset);
						offset += 2;
						CString hqcount = _T("");hqcount.Format(L"%d",HQnum);
						m_ListCtl.SetItemText(0, 7, hqcount);//��������������
						//ί�е�ַ	string
						if(PackLen>offset)
						{
							for(int w=0;w<HQnum;w++)
							{
								short HQlen = *(short*)(PackBody+offset);
								offset += 2;
								char *hqaddr = new char[HQlen+1];
								memset(hqaddr,0,HQlen+1);
								memcpy(hqaddr,PackBody+offset,HQlen);
								offset += HQlen;
								wstring hqaddrstr = UTF8_2_UTF16(hqaddr);
								CString hqxh = _T("");hqxh.Format(L"%d",w+1);
								m_ListCtl.SetItemText(w,0,hqxh);
								m_ListCtl.SetItemText(w,8,hqaddrstr.c_str());
								delete []hqaddr;
								//��ί�ж˿�	short
								short HQport = *(short*)(PackBody+offset);
								offset += 2;
								CString wtport = _T("");wtport.Format(L"%d",HQport);
								m_ListCtl.SetItemText(w,9,wtport);
							}
						}
					}
				}
			}
		}break;
	case 1003:
		{
			//���幫��������������
			short QSnum=0;   //ȯ�̸���
			short SXnum = 0;  //��չ������Ŀ
			short ADDRnum = 0; //��ַ��Ŀ,��ַ��Ŀ����չ������ͬһ����
			if(PackLen>offset)
			{
				//���ַ��ϢCRCֵ short��
				short CRCval = *(short*)(PackBody+offset);
				offset += 2;
				CString CRCfg = _T("");CRCfg.Format(L"%u",CRCval);
				m_ListCtl.SetItemText(0,1,CRCfg);
				//��short           ȯ����Ŀ
				if(PackLen>offset)
				{
					QSnum = *(short*)(PackBody+offset);
					offset += 2;
					CString QSCount = _T("");QSCount.Format(L"%u",QSnum);
					m_ListCtl.SetItemText(0,2,QSCount);
					//��ȯ������ string   ,�������涼��ȯ�̸��������б���forѭ��
					if(PackLen>offset)
					{
						for(int i=0; i<QSnum; i++)
						{
							//��String      ȯ������
							short QSNamelen = *(short*)(PackBody+offset);
							offset += 2;
							if(QSNamelen!=0)
							{
								char *QSName = new char[QSNamelen+1];
								memset(QSName,0,QSNamelen+1);
								memcpy(QSName,PackBody+offset,QSNamelen);
								wstring QSstr = UTF8_2_UTF16(QSName);
								offset += QSNamelen;
								m_ListCtl.SetItemText(i*SXnum,3,QSstr.c_str());
								delete []QSName;
							}
							if(PackLen>offset)
							{
								//��char        ȯ��������ƴ��
								char QSP = *(PackBody+offset);
								offset += 1;
								CString zmstr = _T("");zmstr.Format(L"%c",QSP);
								m_ListCtl.SetItemText(i*SXnum,4,zmstr);
							}
							if(PackLen>offset)
							{
								//��short	������չ������Ŀ
								SXnum = *(short*)(PackBody+offset);
								offset+=2;
								CString SXnumVal=_T("");SXnumVal.Format(L"%d",SXnum);
								m_ListCtl.SetItemText(i*SXnum,5,SXnumVal);

								if(QSnum*SXnum>50)
								{
									for(int w=0; w<QSnum*SXnum;w++)
									{
										CString val=_T("");val.Format(L"%d",w+1);
										m_ListCtl.SetItemText(w,0,val);
									}
								}

								for(int r=0; r<SXnum; r++)
								{
									// char     �������
									char AttrNo = *(PackBody+offset);
									offset += 1;
									int attrno = AttrNo;
									CString Attr=_T("");Attr.Format(L"%d",attrno);
									m_ListCtl.SetItemText(r+i*SXnum,6,Attr);

									//char	����ֵ
									char AttrVal = *(PackBody+offset);
									offset += 1;
									int attrval = AttrVal;
									CString Attr2=_T("");Attr2.Format(L"%d",attrno);
									m_ListCtl.SetItemText(r+i*SXnum,7,Attr2);
								}
							}
							if(PackLen>offset)
							{
								//��short	��ַ��Ŀ
								ADDRnum = *(short*)(PackBody+offset);
								offset+=2;
								CString ADDRnumVal=_T("");ADDRnumVal.Format(L"%d",SXnum);
								m_ListCtl.SetItemText(i*SXnum,8,ADDRnumVal);

								if(ADDRnum>SXnum)
								{
									if(QSnum*ADDRnum>50)
									{
										for(int l=0; l<QSnum*SXnum;l++)
										{
											CString val1=_T("");val1.Format(L"%d",l+1);
											m_ListCtl.SetItemText(l,0,val1);
										}
									}
								}

								for(int t=0; t<ADDRnum; t++)
								{
									// string   ��ַ
									if(PackLen>offset)
									{
										short strlen = *(short*)(PackBody+offset);
										offset += 2;
										if(strlen>0)
										{
											char *addrstr = new char[strlen+1];
											memset(addrstr,0,strlen+1);
											memcpy(addrstr,PackBody+offset,strlen);
											wstring tempaddr = UTF8_2_UTF16(addrstr);
											offset += strlen;
											m_ListCtl.SetItemText(t+i*ADDRnum,9,tempaddr.c_str());
											delete []addrstr;
										}
									}
									//��short    �˿�
									if(PackLen>offset)
									{
										short port = *(short*)(PackBody+offset);
										offset += 2;
										CString addrport = _T("");addrport.Format(L"",port);
										m_ListCtl.SetItemText(t+i*ADDRnum,10,addrport);
									}
									//ip��ַ��Ӫ�����  char
									if(PackLen>offset)
									{
										char iptype = *(PackBody+offset);
										offset += 1;
										int IPType = iptype;
										CString IPstr=_T("");IPstr.Format(L"%d",IPType);
										m_ListCtl.SetItemText(t+i*ADDRnum,11,IPstr);
									}
									if(PackLen>offset)
									{
										//char     ��ַ����	 //1��ʾί�е�ַ��2��ʾ��������ַ
										char addrtype = *(PackBody+offset);
										offset += 1;
										int addrval = addrtype;
										CString Addr=_T("");Addr.Format(L"%d",addrval);
										m_ListCtl.SetItemText(t+i*ADDRnum,12,Addr);
									}
								}
							}
						}
					}
				}
			}
		}break;
	case 1004:
		{
			//���ж������ֶ���0����������ֵ
			////����Ӫҵ��Ӫҵ����Ŀ
			short CityCnt = 0;
			int idvalue = *(int*)(SendBuffer+sizeof(HQ_HEAD));
			if(idvalue == 0)     //Ϊ0��ʾ��������,����String[]     ��������
			{
				short CityNum = *(short*)(PackBody+offset);
				offset += 2;
				for(int i=0; i<CityNum; i++)
				{
					m_ListCtl.InsertItem(i,_T(""));
					CString CityNo=_T("");CityNo.Format(L"%d",i+1);
					m_ListCtl.SetItemText(i,0,CityNo);
					short stringlen = *(short*)(PackBody+offset);
					offset += 2;
					if(stringlen>0)
					{
						char *CityStr = new char[stringlen+1];
						memset(CityStr,0,stringlen+1);
						memcpy(CityStr,PackBody+offset,stringlen);
						offset += stringlen;
						wstring tempstr = UTF8_2_UTF16(CityStr);
						m_ListCtl.SetItemText(i,1,tempstr.c_str());
						delete []CityStr;
					}
				}
			}
			else
			{
				if(PackLen>offset)
				{
					//short        ����������Ŀ
					CityCnt = *(short*)(PackBody+offset);
					CString cicnt = _T("");cicnt.Format(L"%d",CityCnt);
					offset += 2;
					if(CityCnt>0)
					{

						for(int j=0; j<CityCnt; j++)
						{
							m_ListCtl.InsertItem(j,_T(""));
							CString CityGS=_T("");CityGS.Format(L"%d",j+1);
							m_ListCtl.SetItemText(0,1,cicnt);
							m_ListCtl.SetItemText(j,0,CityGS);
						}
						for(int j=0;j<CityCnt;j++)
						{
							//String    Ӫҵ������
							short YYBlen = *(short*)(PackBody+offset);
							offset += 2;
							if(YYBlen>0)
							{
								char *yybName = new char[YYBlen+1];
								memset(yybName,0,YYBlen+1);
								memcpy(yybName,PackBody+offset,YYBlen);
								offset += YYBlen;
								wstring YybName = UTF8_2_UTF16(yybName);
								m_ListCtl.SetItemText(j,2,YybName.c_str());
								delete []yybName;
							}
							//String    Ӫҵ����ַ
							if(PackLen>offset)
							{
								YYBlen = *(short*)(PackBody+offset);
								offset += 2;
								if(YYBlen>0)
								{
									char *yybaddr = new char[YYBlen+1];
									memset(yybaddr,0,YYBlen+1);
									memcpy(yybaddr,PackBody+offset,YYBlen);
									offset += YYBlen;
									wstring YybAddr = UTF8_2_UTF16(yybaddr);
									m_ListCtl.SetItemText(j,3,YybAddr.c_str());
									delete []yybaddr;
								}
							}
							//String    Ӫҵ���绰
							if(PackLen>offset)
							{
								YYBlen = *(short*)(PackBody+offset);
								offset += 2;
								if(YYBlen>0)
								{
									char *yybtel = new char[YYBlen+1];
									memset(yybtel,0,YYBlen+1);
									memcpy(yybtel,PackBody+offset,YYBlen);
									offset += YYBlen;
									wstring YybTel = UTF8_2_UTF16(yybtel);
									m_ListCtl.SetItemText(j,4,YybTel.c_str());
									delete []yybtel;
								}
							}
							//String    Ӫҵ��google������ַ
							if(PackLen>offset)
							{
								YYBlen = *(short*)(PackBody+offset);
								offset += 2;
								if(YYBlen>0)
								{
									char *goaddr = new char[YYBlen+1];
									memset(goaddr,0,YYBlen+1);
									memcpy(goaddr,PackBody+offset,YYBlen);
									offset += YYBlen;
									wstring GoAddr = UTF8_2_UTF16(goaddr);
									m_ListCtl.SetItemText(j,5,GoAddr.c_str());
									delete []goaddr;
								}
							}
							//String    Ӫҵ������
							if(PackLen>offset)
							{
								YYBlen = *(short*)(PackBody+offset);
								offset += 2;
								if(YYBlen>0)
								{
									char *ltaddr = new char[YYBlen+1];
									memset(ltaddr,0,YYBlen+1);
									memcpy(ltaddr,PackBody+offset,YYBlen);
									offset += YYBlen;
									wstring LTAddr = UTF8_2_UTF16(ltaddr);
									m_ListCtl.SetItemText(j,6,LTAddr.c_str());
									delete []ltaddr;
								}
							}
							//String    Ӫҵ��weidu
							if(PackLen>offset)
							{
								YYBlen = *(short*)(PackBody+offset);
								offset += 2;
								if(YYBlen>0)
								{
									char *lotude = new char[YYBlen+1];
									memset(lotude,0,YYBlen+1);
									memcpy(lotude,PackBody+offset,YYBlen);
									offset += YYBlen;
									wstring LoTude = UTF8_2_UTF16(lotude);
									m_ListCtl.SetItemText(j,7,LoTude.c_str());
									delete []lotude;
								}
							}
						}
					}

				}
			}
		}break;
	case 1901:
		{
			m_ListCtl.InsertItem(0,_T(""));
			if(PackLen>offset)
			{
				//��  �Ƿ�ͨ��       byte       1ͨ�� 0ûͨ��
				char chPass = *(PackBody+offset);
				offset += 1;
				int szch = chPass;
				CString PassVal=_T("");PassVal.Format(L"%d",szch);
				m_ListCtl.SetItemText(0,0,L"1");
				m_ListCtl.SetItemText(0,1,PassVal);
			}
			if(PackLen>offset)
			{
				//����ʾ��Ϣ       String
				short Msglen = *(short*)(PackBody+offset);
				offset += 2;

				char *Msg = new char[Msglen+1];
				memset(Msg,0,Msglen+1);
				memcpy(Msg,PackBody+offset,Msglen);
				offset += Msglen;
				wstring NMsg = UTF8_2_UTF16(Msg);
				m_ListCtl.SetItemText(0,2,NMsg.c_str());
				delete []Msg;
			}
		}break;
	case 1902:
		{
			//�����հ����ֶ�
			unsigned char nResult = ' ';
			CString byte=_T("");
			if(PackLen>offset)
			{
				//��ͬ�����       byte       1�ɹ� 0ʧ��
				nResult = *(PackBody+offset);
				offset += 1;
				int Rbyte = nResult;
				byte.Format(L"%d",Rbyte);
				m_ListCtl.SetItemText(0,0,L"1");
				m_ListCtl.SetItemText(0,1,byte);
			}
			if(PackLen>offset)
			{
				//����ѡ����       String[]
				short CodeNum = *(short*)(PackBody+offset);
				offset += 2;
				for(int i=0;i<CodeNum;i++)
				{
					m_ListCtl.InsertItem(i,_T(""));
					CString Value=_T("");Value.Format(L"%d",i+1);
					m_ListCtl.SetItemText(i,0,Value);
					m_ListCtl.SetItemText(0,1,byte);
					short Codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *CodeVal = new char[Codelen+1];
					memset(CodeVal,0,Codelen+1);
					memcpy(CodeVal,PackBody+offset,Codelen);
					wstring Code = UTF8_2_UTF16(CodeVal);
					m_ListCtl.SetItemText(i,2,Code.c_str());
					delete []CodeVal;
				}
			}
		}break;
	case 1903:
		{
			//����ʾ��Ϣ       String
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short Noticelen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *Notice = new char[Noticelen+1];
					memset(Notice,0,Noticelen+1);
					memcpy(Notice,PackBody+offset,Noticelen);
					offset += Noticelen;
					wstring NoticeStr = UTF8_2_UTF16(Notice);

					m_ListCtl.SetItemText(0,1,NoticeStr.c_str());
					delete []Notice;
				}
			}
		}break;
	case 1904:
		{
			//�Ƿ���Ч       byte       1��Ч 0��Ч
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				unsigned char IsVal = *(PackBody+offset);
				offset += 1;
				int isval = IsVal;
				CString Val=_T("");Val.Format(L"%d",isval);
				m_ListCtl.SetItemText(0,1,Val);
			}
			if(PackLen>offset)
			{
				short PWlen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *PW = new char[PWlen+1];
					memset(PW,0,PWlen+1);
					memcpy(PW,PackBody+offset,PWlen);
					offset += PWlen;
					wstring PWStr = UTF8_2_UTF16(PW);
					m_ListCtl.SetItemText(0,2,PWStr.c_str());
					delete []PW;
				}
			}
		}break;
	case 1905:
		{
			//�Ƿ���Ч       byte       1��Ч 0��Ч
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short Phonelen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *Phone = new char[Phonelen+1];
					memset(Phone,0,Phonelen+1);
					memcpy(Phone,PackBody+offset,Phonelen);
					offset += Phonelen;
					wstring PhoneStr = UTF8_2_UTF16(Phone);
					m_ListCtl.SetItemText(0,1,PhoneStr.c_str());
					delete []Phone;
				}
			}
			if(PackLen>offset)
			{
				short PWlen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *PW = new char[PWlen+1];
					memset(PW,0,PWlen+1);
					memcpy(PW,PackBody+offset,PWlen);
					offset += PWlen;
					wstring PWStr = UTF8_2_UTF16(PW);
					m_ListCtl.SetItemText(0,2,PWStr.c_str());
					delete []PW;
				}
			}
		}break;
	case 2000:
		{
			//��¼��         short
			short Num = 0;
			if(PackLen>offset)
			{
				Num = *(short*)(PackBody+offset);
				CString codecount=_T("");codecount.Format(L"%d",Num);
				offset += 2;
				for(int i=0;i<Num;i++)
				{
					m_ListCtl.InsertItem(i,_T(""));
					CString CodeNum=_T("");CodeNum.Format(L"%d",i+1);
					m_ListCtl.SetItemText(i,0,CodeNum);
				}
				m_ListCtl.SetItemText(0,1,codecount);
			}
			for(int j=0;j<Num;j++)
			{
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					if(PackLen>offset)
					{
						char *ProductCode = new char[codelen+1];
						memset(ProductCode,0,codelen+1);
						memcpy(ProductCode,PackBody+offset,codelen);
						offset += codelen;
						wstring ProductCodeStr = UTF8_2_UTF16(ProductCode);
						m_ListCtl.SetItemText(j,2,ProductCodeStr.c_str());
						delete []ProductCode;
					}
				}
				if(PackLen>offset)
				{
					short Namelen = *(short*)(PackBody+offset);
					offset += 2;
					if(PackLen>offset)
					{
						char *Name = new char[Namelen+1];
						memset(Name,0,Namelen+1);
						memcpy(Name,PackBody+offset,Namelen);
						offset += Namelen;
						wstring NameStr = UTF8_2_UTF16(Name);
						m_ListCtl.SetItemText(j,3,NameStr.c_str());
						delete []Name;
					}
				}
			}
		}break;
	case 2100:
		{
			short Num = 0;     //��¼��
			if(PackLen>offset)
			{
				Num = *(short*)(PackBody+offset);
				CString codecount=_T("");codecount.Format(L"%d",Num);
				offset += 2;
				for(int i=0;i<Num;i++)
				{
					m_ListCtl.InsertItem(i,_T(""));
					CString CodeNum=_T("");CodeNum.Format(L"%d",i+1);
					m_ListCtl.SetItemText(i,0,CodeNum);
				}
				m_ListCtl.SetItemText(0,1,codecount);
			}
			for(int j=0;j<Num;j++)
			{
				//"����" type="ans_string"
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					if(PackLen>offset)
					{
						char *ProductCode = new char[codelen+1];
						memset(ProductCode,0,codelen+1);
						memcpy(ProductCode,PackBody+offset,codelen);
						offset += codelen;
						wstring ProductCodeStr = UTF8_2_UTF16(ProductCode);
						m_ListCtl.SetItemText(j,2,ProductCodeStr.c_str());
						delete []ProductCode;
					}
				}
				//"����" type="ans_string"
				if(PackLen>offset)
				{
					short Namelen = *(short*)(PackBody+offset);
					offset += 2;
					if(PackLen>offset)
					{
						char *Name = new char[Namelen+1];
						memset(Name,0,Namelen+1);
						memcpy(Name,PackBody+offset,Namelen);
						offset += Namelen;
						wstring NameStr = UTF8_2_UTF16(Name);
						m_ListCtl.SetItemText(j,3,NameStr.c_str());
						delete []Name;
					}
				}
				if(PackLen>offset)
				{
					//��С����λ��
					unsigned char pointval = *(PackBody+offset);
					offset += 1;
					int PtVal = pointval;
					CString PtStr=_T("");PtStr.Format(L"%d",PtVal);
					m_ListCtl.SetItemText(j,4,PtStr);
				}
				if(PackLen>offset)
				{
					//�����¼�
					int newprice = *(int*)(PackBody+offset);
					offset += 4;
					CString NPStr=_T("");NPStr.Format(L"%d",newprice);
					m_ListCtl.SetItemText(j,5,NPStr);
				}
				if(PackLen>offset)
				{
					//�����ռ�
					int YTrice = *(int*)(PackBody+offset);
					offset += sizeof(int);
					CString YTriceStr=_T("");YTriceStr.Format(L"%d",YTrice);
					m_ListCtl.SetItemText(j,6,YTriceStr);
				}
				if(PackLen>offset)
				{
					//��ɽ���
					int MKVal = *(int*)(PackBody+offset);
					offset += 4;
					CString MKValStr=_T("");MKValStr.Format(L"%d",MKVal);
					m_ListCtl.SetItemText(j,7,MKValStr);
				}
				if(PackLen>offset)
				{
					//��ɽ���
					int MKMoney = *(int*)(PackBody+offset);
					offset += 4;
					CString MKMoneyStr=_T("");MKMoneyStr.Format(L"%d",MKMoney);
					m_ListCtl.SetItemText(j,8,MKMoneyStr);
				}
				if(PackLen>offset)
				{
					//����߼�
					int hprice = *(int*)(PackBody+offset);
					offset += 4;
					CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
					m_ListCtl.SetItemText(j,9,hpriceStr);
				}
				if(PackLen>offset)
				{
					//����ͼ�
					int lprice = *(int*)(PackBody+offset);
					offset += 4;
					CString LowStr=_T("");LowStr.Format(L"%d",lprice);
					m_ListCtl.SetItemText(j,10,LowStr);
				}
				if(PackLen>offset)
				{
					//���޹���
					unsigned char NoticeVal = *(PackBody+offset);
					offset += 1;
					int IsVal = NoticeVal;
					CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
					m_ListCtl.SetItemText(j,11,IsValStr);
				}
				if(PackLen>offset)
				{
					//������ָ��id" type="ans_short"
					short IndexID = *(short*)(PackBody+offset);
					offset += 2;
					CString indexstr=_T("");indexstr.Format(L"%d",IndexID);
					m_ListCtl.SetItemText(j,12,indexstr);
				}
			}
			if(PackLen>offset)
			{
				//�ܼ�¼��" type="ans_short
				short TNum = *(short*)(PackBody+offset);
				offset += 2;
				CString TNumStr=_T("");TNumStr.Format(L"%d",TNum);
				m_ListCtl.SetItemText(0,13,TNumStr);
			}
		}break;
	case 2200:
		{
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short codelen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *ProductCode = new char[codelen+1];
					memset(ProductCode,0,codelen+1);
					memcpy(ProductCode,PackBody+offset,codelen);
					offset += codelen;
					wstring ProductCodeStr = UTF8_2_UTF16(ProductCode);
					m_ListCtl.SetItemText(0,1,ProductCodeStr.c_str());
					delete []ProductCode;
				}
			}
			//"����" type="ans_string"
			if(PackLen>offset)
			{
				short Namelen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *Name = new char[Namelen+1];
					memset(Name,0,Namelen+1);
					memcpy(Name,PackBody+offset,Namelen);
					offset += Namelen;
					wstring NameStr = UTF8_2_UTF16(Name);
					m_ListCtl.SetItemText(0,2,NameStr.c_str());
					delete []Name;
				}
			}
			if(PackLen>offset)
			{
				//����
				unsigned char Type = *(PackBody+offset);
				offset += 1;
				int IsVal = Type;
				CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
				m_ListCtl.SetItemText(0,3,IsValStr);
			}
			if(PackLen>offset)
			{
				//��С����λ��
				unsigned char point = *(PackBody+offset);
				offset += 1;
				int pointval = point;
				CString pointStr=_T("");pointStr.Format(L"%d",pointval);
				m_ListCtl.SetItemText(0,4,pointStr);
			}
			if(PackLen>offset)
			{
				short AHcodelen = *(short*)(PackBody+offset);
				offset += 2;
				if(PackLen>offset)
				{
					char *AHcode = new char[AHcodelen+1];
					memset(AHcode,0,AHcodelen+1);
					memcpy(AHcode,PackBody+offset,AHcodelen);
					offset += AHcodelen;
					wstring AHcodeStr = UTF8_2_UTF16(AHcode);
					m_ListCtl.SetItemText(0,5,AHcodeStr.c_str());
					delete []AHcode;
				}
			}
			if(PackLen>offset)
			{
				//���ܹɱ�
				int tval = *(int*)(PackBody+offset);
				offset += 4;
				CString LowStr=_T("");LowStr.Format(L"%d",tval);
				m_ListCtl.SetItemText(0,6,LowStr);
			}
			if(PackLen>offset)
			{
				//����ͣ��
				int HPrice = *(int*)(PackBody+offset);
				offset += 4;
				CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
				m_ListCtl.SetItemText(0,7,HPriceStr);
			}
			if(PackLen>offset)
			{
				//����ͼ�
				int Lowprice = *(int*)(PackBody+offset);
				offset += 4;
				CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
				m_ListCtl.SetItemText(0,8,LowpriceStr);
			}
		}break;
	case 2201:
		{
			short RecdNum=0;
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��������         int
				int tval = *(int*)(PackBody+offset);
				offset += 4;
				CString LowStr=_T("");LowStr.Format(L"%d",tval);
				m_ListCtl.SetItemText(0,1,LowStr);
			}
			if(PackLen>offset)
			{
				//��ɽ���         int
				int HPrice = *(int*)(PackBody+offset);
				offset += 4;
				CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
				m_ListCtl.SetItemText(0,2,HPriceStr);
			}
			if(PackLen>offset)
			{
				//��ɽ���         int
				int Lowprice = *(int*)(PackBody+offset);
				offset += 4;
				CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
				m_ListCtl.SetItemText(0,3,LowpriceStr);
			}
			if(PackLen>offset)
			{
				//��Ϣ����       byte    1�� 0��
				unsigned char Msg = *(PackBody+offset);
				offset += 1;
				int Msgval = Msg;
				CString MsgStr=_T("");MsgStr.Format(L"%d",Msgval);
				m_ListCtl.SetItemText(0,4,MsgStr);
			}
			if(PackLen>offset)
			{
				//��������       byte    0-5��ֵ
				unsigned char star = *(PackBody+offset);
				offset += 1;
				int starval = star;
				CString starStr=_T("");starStr.Format(L"%d",starval);
				m_ListCtl.SetItemText(0,5,starStr);
			}
			if(PackLen>offset)
			{
				//����λ��       int         ���һ����ʱ�������ش�
				int datapos = *(int*)(PackBody+offset);
				offset += 4;
				CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
				m_ListCtl.SetItemText(0,6,dataposStr);
			}
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				CString RecdNumStr=_T("");RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,7,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,XHno);
			}
			if(PackLen>offset)
			{
				for(int j=0; j<RecdNum; j++)
				{
					//��ʱ��           int
					int timeval = *(int*)(PackBody+offset);
					offset += 4;
					CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
					m_ListCtl.SetItemText(j,8,timevalStr);
					if(PackLen>offset)
					{
						//�����¼�         int
						int NewPrice = *(int*)(PackBody+offset);
						offset += 4;
						CString NewPriceStr=_T("");NewPriceStr.Format(L"%d",NewPrice);
						m_ListCtl.SetItemText(j,9,NewPriceStr);
					}
					if(PackLen>offset)
					{
						//��ɽ���         int
						int CJVal = *(int*)(PackBody+offset);
						offset += 4;
						CString CJValStr=_T("");CJValStr.Format(L"%d",CJVal);
						m_ListCtl.SetItemText(j,10,CJValStr);
					}
					if(PackLen>offset)
					{
						//�����           int
						int AVprice = *(int*)(PackBody+offset);
						offset += 4;
						CString AVpriceStr=_T("");AVpriceStr.Format(L"%d",AVprice);
						m_ListCtl.SetItemText(j,11,AVpriceStr);
					}
				}
			}
		}break;
	case 2202:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);

			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,XHno);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			if(PackLen>offset)
			{
				for(int j=0; j<RecdNum; j++)
				{
					//������           int
					int timeval = *(int*)(PackBody+offset);
					offset += 4;
					CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
					m_ListCtl.SetItemText(j,2,timevalStr);
					if(PackLen>offset)
					{
						//�⿪�̼�         int
						int OPrice = *(int*)(PackBody+offset);
						offset += 4;
						CString OPriceStr=_T("");OPriceStr.Format(L"%d",OPrice);
						m_ListCtl.SetItemText(j,3,OPriceStr);
					}
					if(PackLen>offset)
					{
						//����߼�         int
						int mhVal = *(int*)(PackBody+offset);
						offset += 4;
						CString mhValStr=_T("");mhValStr.Format(L"%d",mhVal);
						m_ListCtl.SetItemText(j,4,mhValStr);
					}
					if(PackLen>offset)
					{
						//����ͼ�         int
						int mdprice = *(int*)(PackBody+offset);
						offset += 4;
						CString mdpriceStr=_T("");mdpriceStr.Format(L"%d",mdprice);
						m_ListCtl.SetItemText(j,5,mdpriceStr);
					}
					if(PackLen>offset)
					{
						//�����̼�         int
						int Sprice = *(int*)(PackBody+offset);
						offset += 4;
						CString SpriceStr=_T("");SpriceStr.Format(L"%d",Sprice);
						m_ListCtl.SetItemText(j,6,SpriceStr);
					}
					if(PackLen>offset)
					{
						//��ɽ���         int
						int CJValue = *(int*)(PackBody+offset);
						offset += 4;
						CString CJValueStr=_T("");CJValueStr.Format(L"%d",CJValue);
						m_ListCtl.SetItemText(j,7,CJValueStr);
					}
					if(PackLen>offset)
					{
						//��ɽ���         int
						int CJMoney = *(int*)(PackBody+offset);
						offset += 4;
						CString CJMoneyStr=_T("");CJMoneyStr.Format(L"%d",CJMoney);
						m_ListCtl.SetItemText(j,8,CJMoneyStr);
					}
				}
			}
		}break;
	case 2203:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//���¼�         int
				int newprice = *(int*)(PackBody+offset);
				offset += 4;
				CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
				m_ListCtl.SetItemText(0,1,newpriceStr);
			}
			if(PackLen>offset)
			{
				//�ɽ���         int
				int cjval = *(int*)(PackBody+offset);
				offset += 4;
				CString cjvalStr=_T("");cjvalStr.Format(L"%d",cjval);
				m_ListCtl.SetItemText(0,2,cjvalStr);
			}
			if(PackLen>offset)
			{
				//�ɽ���         int
				int cjmoney = *(int*)(PackBody+offset);
				offset += 4;
				CString cjmoneyStr=_T("");cjmoneyStr.Format(L"%d",cjmoney);
				m_ListCtl.SetItemText(0,3,cjmoneyStr);
			}
			if(PackLen>offset)
			{
				//��߼�         int
				int hprice = *(int*)(PackBody+offset);
				offset += 4;
				CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
				m_ListCtl.SetItemText(0,4,hpriceStr);
			}
			if(PackLen>offset)
			{
				//��ͼ�         int
				int lprice = *(int*)(PackBody+offset);
				offset += 4;
				CString lpriceStr=_T("");lpriceStr.Format(L"%d",lprice);
				m_ListCtl.SetItemText(0,5,lpriceStr);
			}
			if(PackLen>offset)
			{
				//���̼�         int
				int oprice = *(int*)(PackBody+offset);
				offset += 4;
				CString opriceStr=_T("");opriceStr.Format(L"%d",oprice);
				m_ListCtl.SetItemText(0,6,opriceStr);
			}
			if(PackLen>offset)
			{
				//������         int
				int sprice = *(int*)(PackBody+offset);
				offset += 4;
				CString spriceStr=_T("");spriceStr.Format(L"%d",sprice);
				m_ListCtl.SetItemText(0,7,spriceStr);
			}
			if(PackLen>offset)
			{
				//��ͨ�ɱ�       int
				int lgval = *(int*)(PackBody+offset);
				offset += 4;
				CString lgvalStr=_T("");lgvalStr.Format(L"%d",lgval);
				m_ListCtl.SetItemText(0,8,lgvalStr);
			}
			if(PackLen>offset)
			{
				//ί��		int��100
				int wb = *(int*)(PackBody+offset);
				offset += 4;
				CString wbStr=_T("");wbStr.Format(L"%d",wb);
				m_ListCtl.SetItemText(0,9,wbStr);
			}
			if(PackLen>offset)
			{
				//����		int��10000
				int lb = *(int*)(PackBody+offset);
				offset += 4;
				CString lbStr=_T("");lbStr.Format(L"%d",lb);
				m_ListCtl.SetItemText(0,10,lbStr);
			}
			if(PackLen>offset)
			{
				//��ָ֤��	int
				int sindex = *(int*)(PackBody+offset);
				offset += 4;
				CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
				m_ListCtl.SetItemText(0,11,sindexStr);
			}
			if(PackLen>offset)
			{
				//��ָ֤������	int
				int indexspri = *(int*)(PackBody+offset);
				offset += 4;
				CString indexspriStr=_T("");indexspriStr.Format(L"%d",indexspri);
				m_ListCtl.SetItemText(0,12,indexspriStr);
			}
			if(PackLen>offset)
			{
				//��ָ֤��	int
				int sindex = *(int*)(PackBody+offset);
				offset += 4;
				CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
				m_ListCtl.SetItemText(0,13,sindexStr);
			}
			if(PackLen>offset)
			{
				//��֤������	int
				int sindexsprice = *(int*)(PackBody+offset);
				offset += 4;
				CString sindexspriceStr=_T("");sindexspriceStr.Format(L"%d",sindexsprice);
				m_ListCtl.SetItemText(0,14,sindexspriceStr);
			}
			if(PackLen>offset)
			{
				//���̳ɽ���	int	//��ָ��û������
				int incjval = *(int*)(PackBody+offset);
				offset += 4;
				CString incjvalStr=_T("");incjvalStr.Format(L"%d",incjval);
				m_ListCtl.SetItemText(0,15,incjvalStr);
			}
			if(PackLen>offset)
			{
				//����		int	//��ָ��û������
				int scount = *(int*)(PackBody+offset);
				offset += 4;
				CString scountStr=_T("");scountStr.Format(L"%d",scount);
				m_ListCtl.SetItemText(0,16,scountStr);
			}
		}break;
	case 2204:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//������         int
				int zsprice = *(int*)(PackBody+offset);
				offset += 4;
				CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				m_ListCtl.SetItemText(0,1,zspriceStr);
			}
			if(PackLen>offset)
			{
				//��¼��         short       ���������̣��嵵���鴫10���������鴫6
				RecdNum = *(short*)(PackBody+offset);
				offset += sizeof(short);
				RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,XHno);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//������         int
				if(PackLen>offset)
				{
					int bsprice = *(int*)(PackBody+offset);
					offset += 4;
					CString bspriceStr=_T("");bspriceStr.Format(L"%d",bsprice);
					m_ListCtl.SetItemText(j,3,bspriceStr);
				}
				//������         int
				if(PackLen>offset)
				{
					int bsval = *(int*)(PackBody+offset);
					offset += 4;
					CString bsvalStr=_T("");bsvalStr.Format(L"%d",bsval);
					m_ListCtl.SetItemText(j,4,bsvalStr);
				}
			}
		}break;
	case 2205:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//������         int
				int zsprice = *(int*)(PackBody+offset);
				offset += 4;
				CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				m_ListCtl.SetItemText(0,1,zspriceStr);
			}
			if(PackLen>offset)
			{
				//����λ��       int	���淵�صĵ�һ����¼��¼��
				int datapos = *(int*)(PackBody+offset);
				offset += 4;
				CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
				m_ListCtl.SetItemText(0,2,dataposStr);
			}
			if(PackLen>offset)
			{
				//����λ��       int	���淵�صĵ�һ����¼��¼��
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,3,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,XHno);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//ʱ��           int
				if(PackLen>offset)
				{
					int timeval = *(int*)(PackBody+offset);
					offset += 4;
					CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
					m_ListCtl.SetItemText(j,4,timevalStr);
				}
				//�ɽ���         int
				if(PackLen>offset)
				{
					int cjprice = *(int*)(PackBody+offset);
					offset += 4;
					CString cjpriceStr=_T("");cjpriceStr.Format(L"%d",cjprice);
					m_ListCtl.SetItemText(j,5,cjpriceStr);
				}
				//�ɽ���         int
				if(PackLen>offset)
				{
					int cjvalue = *(int*)(PackBody+offset);
					offset += 4;
					CString cjvalueStr=_T("");cjvalueStr.Format(L"%d",cjvalue);
					m_ListCtl.SetItemText(j,6,cjvalueStr);
				}
			}
		}break;
	case 2206:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//���Ǽ���       short
				short szcount = *(short*)(PackBody+offset);
				offset += 2;
				CString szcountStr=_T("");szcountStr.Format(L"%d",szcount);
				m_ListCtl.SetItemText(0,1,szcountStr);
			}
			if(PackLen>offset)
			{
				//ƽ�̼���       short
				short ppcount = *(short*)(PackBody+offset);
				offset += 2;
				CString ppcountStr=_T("");ppcountStr.Format(L"%d",ppcount);
				m_ListCtl.SetItemText(0,2,ppcountStr);
			}
			if(PackLen>offset)
			{
				//�µ�����       short
				short xdcount = *(short*)(PackBody+offset);
				offset += 2;
				CString xdcountStr=_T("");xdcountStr.Format(L"%d",xdcount);
				m_ListCtl.SetItemText(0,3,xdcountStr);
			}
			if(PackLen>offset)
			{
				//����		int
				int avprice = *(int*)(PackBody+offset);
				offset += 4;
				CString avpriceStr=_T("");avpriceStr.Format(L"%d",avprice);
				m_ListCtl.SetItemText(0,4,avpriceStr);
			}
			if(PackLen>offset)
			{
				//��Ȩ����	int
				int jqprice = *(int*)(PackBody+offset);
				offset += 4;
				CString jqpriceStr=_T("");jqpriceStr.Format(L"%d",jqprice);
				m_ListCtl.SetItemText(0,5,jqpriceStr);
			}
			if(PackLen>offset)
			{
				//����ͨ��ֵ	int
				int zltval = *(int*)(PackBody+offset);
				offset += 4;
				CString zltvalStr=_T("");zltvalStr.Format(L"%d",zltval);
				m_ListCtl.SetItemText(0,6,zltvalStr);
			}
			if(PackLen>offset)
			{
				//����ֵ	int
				int totalval = *(int*)(PackBody+offset);
				offset += 4;
				CString totalvalStr=_T("");totalvalStr.Format(L"%d",totalval);
				m_ListCtl.SetItemText(0,7,totalvalStr);
			}
		}break;
	case 2207:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				//����λ��       int	���淵�صĵ�һ����¼��¼��
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);

			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,XHno);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				if(PackLen>offset)
				{
					//����	String	//����
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *nameval = new char[namelen+1];
					memset(nameval,0,namelen+1);
					memcpy(nameval,PackBody+offset,namelen);
					wstring NameStr = UTF8_2_UTF16(nameval);
					offset += namelen;
					m_ListCtl.SetItemText(j,2,NameStr.c_str());
					delete []nameval;
				}
				if(PackLen>offset)
				{
					//С��λ��   byte
					unsigned char pointval = *(PackBody+offset);
					offset += 1;
					int pointvalue = pointval;
					CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
					m_ListCtl.SetItemText(j,3,pointvalStr);
				}
				if(PackLen>offset)
				{
					//���¼۸�	int
					int newprice = *(int*)(PackBody+offset);
					offset += 4;
					CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
					m_ListCtl.SetItemText(j,4,newpriceStr);
				}
				if(PackLen>offset)
				{
					//������	int
					int zsprice = *(int*)(PackBody+offset);
					offset += 4;
					CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
					m_ListCtl.SetItemText(j,5,zspriceStr);
				}
				if(PackLen>offset)
				{
					//�ɽ���	int	//ֵΪ0�Ͳ���ʾ���ɽ���Ϊ�򣬿ͻ��˸�����Ļ��С��ʾΪ����ߴ�0��2ΪС������
					int cjmoney = *(int*)(PackBody+offset);
					offset += 4;
					CString cjmoneyStr=_T("");cjmoneyStr.Format(L"%d",cjmoney);
					m_ListCtl.SetItemText(j,6,cjmoneyStr);
				}
			}
		}break;
	case 2300:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);

			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,XHno);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				if(PackLen>offset)
				{
					//����           String
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *codeval = new char[codelen+1];
					memset(codeval,0,codelen+1);
					memcpy(codeval,PackBody+offset,codelen);
					wstring codevalStr = UTF8_2_UTF16(codeval);
					offset += codelen;
					m_ListCtl.SetItemText(j,2,codevalStr.c_str());
					delete []codeval;
				}
				if(PackLen>offset)
				{
					//����	String	//����
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *nameval = new char[namelen+1];
					memset(nameval,0,namelen+1);
					memcpy(nameval,PackBody+offset,namelen);
					wstring NameStr = UTF8_2_UTF16(nameval);
					offset += namelen;
					m_ListCtl.SetItemText(j,3,NameStr.c_str());
					delete []nameval;
				}
				if(PackLen>offset)
				{
					//С��λ��   byte
					unsigned char pointval = *(PackBody+offset);
					offset += 1;
					int pointvalue = pointval;
					CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
					m_ListCtl.SetItemText(j,4,pointvalStr);
				}
				if(PackLen>offset)
				{
					//��ֵ           int
					int jzvalue = *(int*)(PackBody+offset);
					offset += 4;
					CString jzvalueStr=_T("");jzvalueStr.Format(L"%d",jzvalue);
					m_ListCtl.SetItemText(j,5,jzvalueStr);
				}
				if(PackLen>offset)
				{
					//���ھ�ֵ       int
					int sqjz = *(int*)(PackBody+offset);
					offset += 4;
					CString sqjzStr=_T("");sqjzStr.Format(L"%d",sqjz);
					m_ListCtl.SetItemText(j,6,sqjzStr);
				}
				if(PackLen>offset)
				{
					//�ۼƾ�ֵ       int
					int ljjz = *(int*)(PackBody+offset);
					offset += 4;
					CString ljjzStr=_T("");ljjzStr.Format(L"%d",ljjz);
					m_ListCtl.SetItemText(j,7,ljjzStr);
				}
				if(PackLen>offset)
				{
					//��ֹ����       int
					int enddate = *(int*)(PackBody+offset);
					offset += 4;
					CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
					m_ListCtl.SetItemText(j,8,enddateStr);
				}
			}
			if(PackLen>offset)
			{
				//�ܼ�¼��       short
				short tolcnt = *(short*)(PackBody+offset);
				offset += 2;
				CString tolcntStr=_T("");tolcntStr.Format(L"%d",tolcnt);
				m_ListCtl.SetItemText(0,9,tolcntStr);
			}
		}break;
	case 2301:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);

			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,XHno);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				if(PackLen>offset)
				{
					//����           String
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *codeval = new char[codelen+1];
					memset(codeval,0,codelen+1);
					memcpy(codeval,PackBody+offset,codelen);
					wstring codevalStr = UTF8_2_UTF16(codeval);
					offset += codelen;
					m_ListCtl.SetItemText(j,2,codevalStr.c_str());
					delete []codeval;
				}
				if(PackLen>offset)
				{
					//����	String	//����
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *nameval = new char[namelen+1];
					memset(nameval,0,namelen+1);
					memcpy(nameval,PackBody+offset,namelen);
					wstring NameStr = UTF8_2_UTF16(nameval);
					offset += namelen;
					m_ListCtl.SetItemText(j,3,NameStr.c_str());
					delete []nameval;
				}
				if(PackLen>offset)
				{
					//С��λ��   byte
					unsigned char pointval = *(PackBody+offset);
					offset += 1;
					int pointvalue = pointval;
					CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
					m_ListCtl.SetItemText(j,4,pointvalStr);
				}
				if(PackLen>offset)
				{
					//�������       int
					int wfvalue = *(int*)(PackBody+offset);
					offset += 4;
					CString wfvalueStr=_T("");wfvalueStr.Format(L"%d",wfvalue);
					m_ListCtl.SetItemText(j,5,wfvalueStr);
				}
				if(PackLen>offset)
				{
					//7���껯������  int
					int Sevenpoint = *(int*)(PackBody+offset);
					offset += 4;
					CString SevenpointStr=_T("");SevenpointStr.Format(L"%d",Sevenpoint);
					m_ListCtl.SetItemText(j,6,SevenpointStr);
				}
				if(PackLen>offset)
				{
					//�����������   int
					int sqwfsy = *(int*)(PackBody+offset);
					offset += 4;
					CString sqwfsyStr=_T("");sqwfsyStr.Format(L"%d",sqwfsy);
					m_ListCtl.SetItemText(j,7,sqwfsyStr);
				}
				if(PackLen>offset)
				{
					//�����껯������ int
					int sqnhsy = *(int*)(PackBody+offset);
					offset += 4;
					CString sqnhsyStr=_T("");sqnhsyStr.Format(L"%d",sqnhsy);
					m_ListCtl.SetItemText(j,8,sqnhsyStr);
				}
				if(PackLen>offset)
				{
					//��ֹ����       int
					int enddate = *(int*)(PackBody+offset);
					offset += 4;
					CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
					m_ListCtl.SetItemText(j,9,enddateStr);
				}
			}
			if(PackLen>offset)
			{
				//�ܼ�¼��       short
				short tolcnt = *(short*)(PackBody+offset);
				offset += 2;
				CString tolcntStr=_T("");tolcntStr.Format(L"%d",tolcnt);
				m_ListCtl.SetItemText(0,10,tolcntStr);
			}
		}break;
	case 2310:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//����           String
				short codelen = *(short*)(PackBody+offset);
				offset += 2;
				char *codeval = new char[codelen+1];
				memset(codeval,0,codelen+1);
				memcpy(codeval,PackBody+offset,codelen);
				wstring codevalStr = UTF8_2_UTF16(codeval);
				offset += codelen;
				m_ListCtl.SetItemText(0,1,codevalStr.c_str());
				delete []codeval;
			}
			if(PackLen>offset)
			{
				//����	String	//����
				short namelen = *(short*)(PackBody+offset);
				offset += 2;
				char *nameval = new char[namelen+1];
				memset(nameval,0,namelen+1);
				memcpy(nameval,PackBody+offset,namelen);
				wstring NameStr = UTF8_2_UTF16(nameval);
				offset += namelen;
				m_ListCtl.SetItemText(0,2,NameStr.c_str());
				delete []nameval;
			}
			if(PackLen>offset)
			{
				//����           byte
				unsigned char tYPE = *(PackBody+offset);
				offset += 1;
				int tYPEvalue = tYPE;
				CString tYPEStr=_T("");tYPEStr.Format(L"%d",tYPEvalue);
				m_ListCtl.SetItemText(0,3,tYPEStr);
			}
			if(PackLen>offset)
			{
				//С��λ��   byte
				unsigned char pointval = *(PackBody+offset);
				offset += 1;
				int pointvalue = pointval;
				CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				m_ListCtl.SetItemText(0,4,pointvalStr);
			}
		}break;
	case 2311:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����           int
						int datetime = *(int*)(PackBody+offset);
						offset += 4;
						CString DateStr=_T("");DateStr.Format(L"%d",datetime);
						m_ListCtl.SetItemText(j,2,DateStr);
					}
					if(PackLen>offset)
					{
						//��ֵ           int
						int jzval = *(int*)(PackBody+offset);
						offset += 4;
						CString jzvalStr=_T("");jzvalStr.Format(L"%d",jzval);
						m_ListCtl.SetItemText(j,3,jzvalStr);
					}
					if(PackLen>offset)
					{
						//�ۼƾ�ֵ       int
						int ljjzval = *(int*)(PackBody+offset);
						offset += 4;
						CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
						m_ListCtl.SetItemText(j,4,ljjzvalStr);
					}
				}
			}
		}break;
	case 2312:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����           String
						short codelen = *(short*)(PackBody+offset);
						offset += 2;
						char *codeval = new char[codelen+1];
						memset(codeval,0,codelen+1);
						memcpy(codeval,PackBody+offset,codelen);
						wstring codevalStr = UTF8_2_UTF16(codeval);
						offset += codelen;
						m_ListCtl.SetItemText(j,2,codevalStr.c_str());
						delete []codeval;
					}
					if(PackLen>offset)
					{
						//����	String	//����
						short namelen = *(short*)(PackBody+offset);
						offset += 2;
						char *nameval = new char[namelen+1];
						memset(nameval,0,namelen+1);
						memcpy(nameval,PackBody+offset,namelen);
						wstring NameStr = UTF8_2_UTF16(nameval);
						offset += namelen;
						m_ListCtl.SetItemText(j,3,NameStr.c_str());
						delete []nameval;
					}
					if(PackLen>offset)
					{
						//���ڳֹ�       int
						int sqcgval = *(int*)(PackBody+offset);
						offset += 4;
						CString sqcgvalStr=_T("");sqcgvalStr.Format(L"%d",sqcgval);
						m_ListCtl.SetItemText(j,4,sqcgvalStr);
					}
					if(PackLen>offset)
					{
						//���ڳֹ�       int
						int bqcgval = *(int*)(PackBody+offset);
						offset += 4;
						CString bqcgvalStr=_T("");bqcgvalStr.Format(L"%d",bqcgval);
						m_ListCtl.SetItemText(j,5,bqcgvalStr);
					}
					if(PackLen>offset)
					{
						//ռ����%        int       ������λС������100ת������
						int zjjval = *(int*)(PackBody+offset);
						offset += 4;
						CString zjjvalStr=_T("");zjjvalStr.Format(L"%d",zjjval);
						m_ListCtl.SetItemText(j,6,zjjvalStr);
					}
					if(PackLen>offset)
					{
						//ռ��ͨ�ɱ�%    int       ������λС������100ת������
						int zltgbval = *(int*)(PackBody+offset);
						offset += 4;
						CString zltgbvalStr=_T("");zltgbvalStr.Format(L"%d",zltgbval);
						m_ListCtl.SetItemText(j,7,zltgbvalStr);
					}
				}
			}
		}break;
	case 2313:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//��ֹ����       int       ��ʽ�硰20080331�������µ�������ǰ
						int datetime = *(int*)(PackBody+offset);
						offset += 4;
						CString DateStr=_T("");DateStr.Format(L"%d",datetime);
						m_ListCtl.SetItemText(j,2,DateStr);
					}
					if(PackLen>offset)
					{
						//�ڳ��ܷݶ�     int       ��λ����
						int qczgbval = *(int*)(PackBody+offset);
						offset += 4;
						CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
						m_ListCtl.SetItemText(j,3,qczgbvalStr);
					}
					if(PackLen>offset)
					{
						//�������깺     int       ��λ����
						int bqzsgval = *(int*)(PackBody+offset);
						offset += 4;
						CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
						m_ListCtl.SetItemText(j,4,bqzsgvalStr);
					}
					if(PackLen>offset)
					{
						//���������     int       ��λ����
						int bqzshval = *(int*)(PackBody+offset);
						offset += 4;
						CString bqzshvalStr=_T("");bqzshvalStr.Format(L"%d",bqzshval);
						m_ListCtl.SetItemText(j,5,bqzshvalStr);
					}
				}
			}
		}break;
	case 2314:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//��ֹ����       int       ��ʽ�硰20080331�������µ�������ǰ
						int datetime = *(int*)(PackBody+offset);
						offset += 4;
						CString DateStr=_T("");DateStr.Format(L"%d",datetime);
						m_ListCtl.SetItemText(j,2,DateStr);
					}
					if(PackLen>offset)
					{
						//�����˻���     int
						int qczgbval = *(int*)(PackBody+offset);
						offset += 4;
						CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
						m_ListCtl.SetItemText(j,3,qczgbvalStr);
					}
					if(PackLen>offset)
					{
						//�����ݶ�       int
						int bqzsgval = *(int*)(PackBody+offset);
						offset += 4;
						CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
						m_ListCtl.SetItemText(j,4,bqzsgvalStr);
					}
					if(PackLen>offset)
					{
						//�������зݶ�   int       ��λ����
						int bqzshval = *(int*)(PackBody+offset);
						offset += 4;
						CString bqzshvalStr=_T("");bqzshvalStr.Format(L"%d",bqzshval);
						m_ListCtl.SetItemText(j,5,bqzshvalStr);
					}
					if(PackLen>offset)
					{
						//���˳��зݶ�   int       ��λ����
						int grcyfeval = *(int*)(PackBody+offset);
						offset += 4;
						CString grcyfevalStr=_T("");grcyfevalStr.Format(L"%d",grcyfeval);
						m_ListCtl.SetItemText(j,6,grcyfevalStr);
					}
				}
			}
		}break;
	case 2315:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				char *Msg = new char[RecdNum+1];
				memset(Msg,0,RecdNum+1);
				memcpy(Msg,PackBody+offset,RecdNum);
				offset += RecdNum;
				wstring contMsg = UTF8_2_UTF16(Msg);
				m_ListCtl.SetItemText(0,1,contMsg.c_str());
				delete []Msg;
			}
		}break;
	case 2316:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����           int
						int datetime = *(int*)(PackBody+offset);
						offset += 4;
						CString DateStr=_T("");DateStr.Format(L"%d",datetime);
						m_ListCtl.SetItemText(j,2,DateStr);
					}
					if(PackLen>offset)
					{
						//�ۼƾ�ֵ������     int
						int qczgbval = *(int*)(PackBody+offset);
						offset += 4;
						CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
						m_ListCtl.SetItemText(j,3,qczgbvalStr);
					}
				}
			}
		}break;
	case 2320:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//ID             int
						int nId = *(int*)(PackBody+offset);
						offset += 4;
						CString nIdStr=_T("");nIdStr.Format(L"%d",nId);
						m_ListCtl.SetItemText(j,2,nIdStr);
					}
					if(PackLen>offset)
					{
						//����           String
						short ntiocelen = *(short*)(PackBody+offset);
						offset += 2;
						char *Msg = new char[ntiocelen+1];
						memset(Msg,0,ntiocelen+1);
						memcpy(Msg,PackBody+offset,ntiocelen);
						offset += ntiocelen;
						wstring contMsg = UTF8_2_UTF16(Msg);
						m_ListCtl.SetItemText(j,3,contMsg.c_str());
						delete []Msg;
					}
				}
			}
		}break;
	case 2321:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//����           String
				short ntiocelen = *(short*)(PackBody+offset);
				offset += 2;
				char *Msg = new char[ntiocelen+1];
				memset(Msg,0,ntiocelen+1);
				memcpy(Msg,PackBody+offset,ntiocelen);
				offset += ntiocelen;
				wstring contMsg = UTF8_2_UTF16(Msg);
				m_ListCtl.SetItemText(0,1,contMsg.c_str());
				delete []Msg;
			}
			if(PackLen>offset)
			{
				//����           String
				short contextlen = *(short*)(PackBody+offset);
				offset += 2;
				char *textMsg = new char[contextlen+1];
				memset(textMsg,0,contextlen+1);
				memcpy(textMsg,PackBody+offset,contextlen);
				offset += contextlen;
				wstring contMsgStr = UTF8_2_UTF16(textMsg);
				m_ListCtl.SetItemText(0,2,contMsgStr.c_str());
				delete []textMsg;
			}
		}break;
	case 2322:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����           String
						short codelen = *(short*)(PackBody+offset);
						offset += 2;
						char *codeMsg = new char[codelen+1];
						memset(codeMsg,0,codelen+1);
						memcpy(codeMsg,PackBody+offset,codelen);
						offset += codelen;
						wstring codeMsgStr = UTF8_2_UTF16(codeMsg);
						m_ListCtl.SetItemText(j,2,codeMsgStr.c_str());
						delete []codeMsg;
					}
					if(PackLen>offset)
					{
						//����           String
						short namelen = *(short*)(PackBody+offset);
						offset += 2;
						char *nameMsg = new char[namelen+1];
						memset(nameMsg,0,namelen+1);
						memcpy(nameMsg,PackBody+offset,namelen);
						offset += namelen;
						wstring nameMsgstr = UTF8_2_UTF16(nameMsg);
						m_ListCtl.SetItemText(j,3,nameMsgstr.c_str());
						delete []nameMsg;
					}
				}
			}
		}break;
	case 2330:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short crcval = *(short*)(PackBody+offset);
				offset += 2;
				CString CRCval=_T("");CRCval.Format(L"%d",crcval);
				m_ListCtl.SetItemText(0,1,CRCval);
			}
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����	 String
						short ntiocelen = *(short*)(PackBody+offset);
						offset += 2;
						char *Msg = new char[ntiocelen+1];
						memset(Msg,0,ntiocelen+1);
						memcpy(Msg,PackBody+offset,ntiocelen);
						offset += ntiocelen;
						wstring contMsg = UTF8_2_UTF16(Msg);
						m_ListCtl.SetItemText(j,3,contMsg.c_str());
						delete []Msg;
					}
				}
			}
		}break;
	case 2331:
		{
			char FundType = *(PackBody+offset);
			offset += 1;
			int Fundint = FundType;
			CString FundTypeStr = _T("");FundTypeStr.Format(L"%d",Fundint);

			//�ܼ�¼��       short
			short tolNum = *(short*)(PackBody+offset);
			offset += 2;
			CString tolStr = _T("");tolStr.Format(L"%d",tolNum);

			//��¼��		short
			short RecdNum=*(short*)(PackBody+offset);
			offset += 2;
			CString RecdNumStr=_T("");RecdNumStr.Format(L"%d",RecdNum);
			if(Fundint!=5)
			{
				m_ListCtl.InsertItem(0,_T(""));
				m_ListCtl.SetItemText(0,0,L"1");
				m_ListCtl.SetItemText(0,1,tolStr);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����           String
						short codelen = *(short*)(PackBody+offset);
						offset += 2;
						char *codeMsg = new char[codelen+1];
						memset(codeMsg,0,codelen+1);
						memcpy(codeMsg,PackBody+offset,codelen);
						offset += codelen;
						wstring codeMsgStr = UTF8_2_UTF16(codeMsg);
						m_ListCtl.SetItemText(j,3,codeMsgStr.c_str());
						delete []codeMsg;
					}
					if(PackLen>offset)
					{
						//����           String
						short namelen = *(short*)(PackBody+offset);
						offset += 2;
						char *nameMsg = new char[namelen+1];
						memset(nameMsg,0,namelen+1);
						memcpy(nameMsg,PackBody+offset,namelen);
						offset += namelen;
						wstring nameMsgStr = UTF8_2_UTF16(nameMsg);
						m_ListCtl.SetItemText(j,4,nameMsgStr.c_str());
						delete []nameMsg;
					}
					if(PackLen>offset)
					{
						//��������       int
						int TodayDate = *(int*)(PackBody+offset);
						offset += 4;
						CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
						m_ListCtl.SetItemText(j,5,TodayDateStr);
					}
					if(PackLen>offset)
					{
						//���¾�ֵ       int*10000
						int zxjzval = *(int*)(PackBody+offset);
						offset += 4;
						CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
						m_ListCtl.SetItemText(j,6,zxjzvalStr);
					}
					if(PackLen>offset)
					{
						//�ۼƾ�ֵ       int*10000 
						int ljjzval = *(int*)(PackBody+offset);
						offset += 4;
						CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
						m_ListCtl.SetItemText(j,7,ljjzvalStr);
					}
					if(PackLen>offset)
					{
						//��������       int  
						int sqdateval = *(int*)(PackBody+offset);
						offset += 4;
						CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
						m_ListCtl.SetItemText(j,8,sqdatevalStr);
					}
					if(PackLen>offset)
					{
						//���ھ�ֵ       int*10000  
						int sqval = *(int*)(PackBody+offset);
						offset += 4;
						CString sqvalStr=_T("");sqvalStr.Format(L"%d",sqval);
						m_ListCtl.SetItemText(j,9,sqvalStr);
					}
					if(PackLen>offset)
					{
						//�����ۼ�       int*10000  
						int sqljval = *(int*)(PackBody+offset);
						offset += 4;
						CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
						m_ListCtl.SetItemText(j,10,sqljvalStr);
					}
					if(PackLen>offset)
					{
						//��������       int*10000  
						int rzzval = *(int*)(PackBody+offset);
						offset += 4;
						CString rzzvalStr=_T("");rzzvalStr.Format(L"%d",rzzval);
						m_ListCtl.SetItemText(j,11,rzzvalStr);
					}
					if(PackLen>offset)
					{
						//��������       int*10000  
						int zzzval = *(int*)(PackBody+offset);
						offset += 4;
						CString zzzvalStr=_T("");zzzvalStr.Format(L"%d",zzzval);
						m_ListCtl.SetItemText(j,12,zzzvalStr);
					}
					if(PackLen>offset)
					{
						//��������       int*10000  
						int mzzval = *(int*)(PackBody+offset);
						offset += 4;
						CString mzzvalStr=_T("");mzzvalStr.Format(L"%d",mzzval);
						m_ListCtl.SetItemText(j,13,mzzvalStr);
					}
					if(PackLen>offset)
					{
						//��������       int*10000  
						int yzzval = *(int*)(PackBody+offset);
						offset += 4;
						CString yzzvalStr=_T("");yzzvalStr.Format(L"%d",yzzval);
						m_ListCtl.SetItemText(j,14,yzzvalStr);
					}
				}
			}
			if(Fundint==5)
			{  
				m_ListCtl.InsertItem(0,_T(""));
				m_ListCtl.SetItemText(0,0,L"1");
				m_ListCtl.SetItemText(0,1,tolStr);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//����           String
						short codelen = *(short*)(PackBody+offset);
						offset += 2;
						char *codeMsg = new char[codelen+1];
						memset(codeMsg,0,codelen+1);
						memcpy(codeMsg,PackBody+offset,codelen);
						offset += codelen;
						wstring codeMsgStr = UTF8_2_UTF16(codeMsg);
						m_ListCtl.SetItemText(j,3,codeMsgStr.c_str());
						delete []codeMsg;
					}
					if(PackLen>offset)
					{
						//����           String
						short namelen = *(short*)(PackBody+offset);
						offset += 2;
						char *nameMsg = new char[namelen+1];
						memset(nameMsg,0,namelen+1);
						memcpy(nameMsg,PackBody+offset,namelen);
						offset += namelen;
						wstring nameMsgStr = UTF8_2_UTF16(nameMsg);
						m_ListCtl.SetItemText(j,4,nameMsgStr.c_str());
						delete []nameMsg;
					}
					if(PackLen>offset)
					{
						//��������       int
						int TodayDate = *(int*)(PackBody+offset);
						offset += 4;
						CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
						m_ListCtl.SetItemText(j,5,TodayDateStr);
					}
					if(PackLen>offset)
					{
						//�������       int*10000
						int zxjzval = *(int*)(PackBody+offset);
						offset += 4;
						CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
						m_ListCtl.SetItemText(j,6,zxjzvalStr);
					}
					if(PackLen>offset)
					{
						//����������     int*10000 
						int ljjzval = *(int*)(PackBody+offset);
						offset += 4;
						CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
						m_ListCtl.SetItemText(j,7,ljjzvalStr);
					}
					if(PackLen>offset)
					{
						//��������       int  
						int sqdateval = *(int*)(PackBody+offset);
						offset += 4;
						CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
						m_ListCtl.SetItemText(j,8,sqdatevalStr);
					}
					if(PackLen>offset)
					{
						//�������       int*10000  
						int sqljval = *(int*)(PackBody+offset);
						offset += 4;
						CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
						m_ListCtl.SetItemText(j,9,sqljvalStr);
					}
					if(PackLen>offset)
					{
						//����������     int*10000  
						int rzzval = *(int*)(PackBody+offset);
						offset += 4;
						CString rzzvalStr=_T("");rzzvalStr.Format(L"%d",rzzval);
						m_ListCtl.SetItemText(j,10,rzzvalStr);
					}
				}
			}
		}break;
	case 2500:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����           int
			int dateval = *(int*)(PackBody+offset);
			offset += 4;
			CString dateStr = _T("");dateStr.Format(L"%d",dateval);
			m_ListCtl.SetItemText(0,1,dateStr);
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
					{
						//��������       String
						short ntiocelen = *(short*)(PackBody+offset);
						offset += 2;
						char *Msg = new char[ntiocelen+1];
						memset(Msg,0,ntiocelen+1);
						memcpy(Msg,PackBody+offset,ntiocelen);
						offset += ntiocelen;
						wstring contMsg = UTF8_2_UTF16(Msg);
						m_ListCtl.SetItemText(j,3,contMsg.c_str());
						delete []Msg;
					}
					if(PackLen>offset)
					{
						//С��λ��       byte
						unsigned char pointpos = *(PackBody+offset);
						offset += 1;
						int pointwz = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointwz);
						m_ListCtl.SetItemText(j,4,pointstr);
					}
					if(PackLen>offset)
					{
						//������ֵ       int
						int hlval = *(int*)(PackBody+offset);
						offset += 4;
						CString hlvalstr = _T("");hlvalstr.Format(L"%d",hlval);
						m_ListCtl.SetItemText(j,5,hlvalstr);
					}
				}
			}
		}break;
	case 2600:
		{
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��һID         int         ��һ����ID��û����һ������0

			if(PackLen>offset)
			{
				int nIdval = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
				m_ListCtl.SetItemText(0,1,IdStr);
			}
			//����           String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,PackBody+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,2,textStr.c_str());
				delete []context;
			}
			// Url��ַ        String, ��ʽ: ip:port:id,�ͻ��˸���ƽ̨����url;�ýӿڽ������ڹ㷢֤ȯ
			if(PackLen>offset)
			{
				short urladdrlen = *(short*)(PackBody+offset);
				offset += 2;
				char *urladdr = new char[urladdrlen+1];
				memset(urladdr,0,urladdrlen+1);
				memcpy(urladdr,PackBody+offset,urladdrlen);
				offset += urladdrlen;
				wstring urladdrStr = UTF8_2_UTF16(urladdr);
				m_ListCtl.SetItemText(0,2,urladdrStr.c_str());
				delete []urladdr;
			}
		}break;
	case 2601:
		{
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//ID            int         ��������������Ϣ��id�����û�з���0
			if(PackLen>offset)
			{
				int nIdval = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
				m_ListCtl.SetItemText(0,1,IdStr);
			}
			//����          String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,PackBody+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,2,textStr.c_str());
				delete []context;
			}
		}break;
	case 2602:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//������Ϣ����  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			//��ʼλ��      short	//������Ѷ��һ�����
			if(PackLen>offset)
			{
				short beginpos = *(short*)(PackBody+offset);
				offset += 2;
				CString beginStr = _T("");beginStr.Format(L"%d",beginpos);
				m_ListCtl.SetItemText(0,2,beginStr);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//����          String
				if(PackLen>offset)
				{
					short contextlen = *(short*)(PackBody+offset);
					offset += 2;
					char *context = new char[contextlen+1];
					memset(context,0,contextlen+1);
					memcpy(context,PackBody+offset,contextlen);
					offset += contextlen;
					wstring textStr = UTF8_2_UTF16(context);
					m_ListCtl.SetItemText(j,3,textStr.c_str());
					delete []context;
				}
			}
		}break;
	case 2700:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");

			//������Ϣ����  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����ID         int
				if(PackLen>offset)
				{
					int TypeId = *(int*)(PackBody+offset);
					offset += 2;
					CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
					m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
				//����          String
				if(PackLen>offset)
				{
					short contextlen = *(short*)(PackBody+offset);
					offset += 2;
					char *context = new char[contextlen+1];
					memset(context,0,contextlen+1);
					memcpy(context,PackBody+offset,contextlen);
					offset += contextlen;
					wstring textStr = UTF8_2_UTF16(context);
					m_ListCtl.SetItemText(j,3,textStr.c_str());
					delete []context;
				}
			}
		}break;
	case 2701:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//������Ϣ����  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//ID             int
				if(PackLen>offset)
				{
					int TypeId = *(int*)(PackBody+offset);
					offset += 2;
					CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
					m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
				//����           String
				if(PackLen>offset)
				{
					short contextlen = *(short*)(PackBody+offset);
					offset += 2;
					char *context = new char[contextlen+1];
					memset(context,0,contextlen+1);
					memcpy(context,PackBody+offset,contextlen);
					offset += contextlen;
					wstring textStr = UTF8_2_UTF16(context);
					m_ListCtl.SetItemText(j,3,textStr.c_str());
					delete []context;
				}
			}
		}break;
	case 2702:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//������Ϣ����  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//ID             int
				if(PackLen>offset)
				{
					int TypeId = *(int*)(PackBody+offset);
					offset += 2;
					CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
					m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
				//����           String
				if(PackLen>offset)
				{
					short contextlen = *(short*)(PackBody+offset);
					offset += 2;
					char *context = new char[contextlen+1];
					memset(context,0,contextlen+1);
					memcpy(context,PackBody+offset,contextlen);
					offset += contextlen;
					wstring textStr = UTF8_2_UTF16(context);
					m_ListCtl.SetItemText(j,3,textStr.c_str());
					delete []context;
				}
				//�Ƿ�������     byte    0û���Ӳ˵���1��;
				if(PackLen>offset)
				{
					unsigned char Type = *(PackBody+offset);
					int bytetype = Type;
					offset += 1;
					CString bytetypestr = _T("");bytetypestr.Format(L"%d",Type);
					m_ListCtl.SetItemText(j,4,bytetypestr);
				}
			}
		}break;
	case 2710:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����           String
			if(PackLen>offset)
			{
				short topiclen = *(short*)(PackBody+offset);
				offset += 2;
				char *topic = new char[topiclen+1];
				memset(topic,0,topiclen+1);
				memcpy(topic,PackBody+offset,topiclen);
				offset += topiclen;
				wstring topicStr = UTF8_2_UTF16(topic);
				m_ListCtl.SetItemText(0,1,topicStr.c_str());
				delete []topic;
			}
			//����           String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,PackBody+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,3,textStr.c_str());
				delete []context;
			}
		}break;
	case 2900:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�µ���ID       int     Ϊ 0 ʱ��ʾû������Ϣ
			if(PackLen>offset)
			{
				int idlen = *(int*)(PackBody+offset);
				offset += 4;
				CString idstr=_T("");idstr.Format(L"%d",idlen);
				m_ListCtl.SetItemText(0,1,idstr);
			}
			//����           String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,PackBody+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,3,textStr.c_str());
				delete []context;
			}
		}break;
	case 2901:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�¹��ID       int     Ϊ 0 ʱ��ʾû���¹��
			if(PackLen>offset)
			{
				int newsidlen = *(int*)(PackBody+offset);
				offset += 4;
				CString newsidlenstr=_T("");newsidlenstr.Format(L"%d",newsidlen);
				m_ListCtl.SetItemText(0,1,newsidlenstr);
			}
			//�������       String
			if(PackLen>offset)
			{
				short newslen = *(short*)(PackBody+offset);
				offset += 2;
				char *news = new char[newslen+1];
				memset(news,0,newslen+1);
				memcpy(news,PackBody+offset,newslen);
				offset += newslen;
				wstring newsStr = UTF8_2_UTF16(news);
				m_ListCtl.SetItemText(0,3,newsStr.c_str());
				delete []news;
			}
		}break;
	case 2902:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��ͼƬID       int      Ϊ 0 ʱ��ʾû����ͼƬ
			if(PackLen>offset)
			{
				int piclen = *(int*)(PackBody+offset);
				offset += 4;
				CString picstr=_T("");picstr.Format(L"%d",piclen);
				m_ListCtl.SetItemText(0,1,picstr);
			}
			//ͼƬ��ʽ       char
			if(PackLen>offset)
			{
				char pictype = *(PackBody+offset);
				offset+=1;
				int picval = pictype;
				CString picstr = _T("");picstr.Format(L"%d",picval);
				m_ListCtl.SetItemText(0,2,picstr);
			}
		    //	ͼƬ����       byte[]   ���û����ͼƬ���ɴ������ֽڳ��ȵ�����, ͼƬ�����谴���鷽ʽ���л�(����2�ֽڱ���ֽ�������)
			if(PackLen>offset)
			{
				short picdatalen = *(short*)(PackBody+offset);
				offset += 2;
				char *picdata = new char[picdatalen+1];
				memset(picdata,0,picdatalen+1);
				memcpy(picdata,PackBody+offset,picdatalen);
				wstring picdatastr = UTF8_2_UTF16(picdata);
				m_ListCtl.SetItemText(0,3,picdatastr.c_str());
				delete []picdata;
			}
		}break;
	case 2903:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//ע����	byte     0 ��ʾ��ȷ���û������ֻ�������֤�룻
			unsigned char nResult = *(PackBody+offset);
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2904:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��֤���       byte     ��֤�����
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
			//[�û�id	int]	 	0.��֤�ɹ������ʱ�����û�id(�յ���id��ͻ���Ҫ�����id���Ժ��½ʱ���ã��������֤ʧ��û�и��ֶ�
			int userid = *(int*)(PackBody+offset);
			offset += 4;
			CString useridstr = _T("");useridstr.Format(L"%d",userid);
			m_ListCtl.SetItemText(0,2,useridstr);
			//[long		rightȨ��λ��]  //��֤�ɹ����ص��û�Ĭ��Ȩ�޺����ڣ�ʧ��û�з�
			__int64 qxval = *(__int64*)(PackBody+offset);
			offset += 8;
			char *b = new char[65];
			memset(b,0,65);
			for(int i=0;i<64;i++,qxval<<=1)
			{
				b[i] = (01&qxval)+'0';
				m_ListCtl.InsertItem(i+1,_T(""));
				int qxdata = b[i];
				CString qxdatastr=_T("");qxdatastr.Format(L"%d",qxdata);
				m_ListCtl.SetItemText(i,3,qxdatastr);
			}
			delete []b;
			//[int[]		Ȩ�����ڣ���right��0λ��63��Ӧ]
			short sznum = *(short*)(PackBody+offset);
			offset += 2;
			for(int j=0; j<sznum; j++)
			{
				int data = *(int*)(PackBody+offset);
				offset += 4;
				CString datastr = _T("");datastr.Format(L"%d",data);
				m_ListCtl.SetItemText(j,4,datastr);
			}
		}break;
	case 2907:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			int causelen = *(int*)(PackBody+offset);
			offset += 4;
			CString Datastr=_T("");Datastr.Format(L"%d",causelen);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			m_ListCtl.SetItemText(0,1,Datastr);
		}break;
	case 2908:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ӧ�����		byte	0��//��ʾ�����Ѿ��ύ�������֪ͨ�û�
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
		}
	case 2909:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ӧ�����		byte	0��//��ʾ�����Ѿ��ύ�������֪ͨ�û�
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
		}
	case 2910:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ӧ�����		byte	0��//��ʾ�����Ѿ��ύ�������֪ͨ�û�
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
			//�ȼ�		int
			int gradeval = *(int*)(PackBody+offset);
			offset += 4;
			CString gradestr = _T("");gradestr.Format(L"%d",gradeval);
			m_ListCtl.SetItemText(0,2,gradestr);
		}
	case 2911:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ӧ�����		byte	0��//��ʾ�����Ѿ��ύ�������֪ͨ�û�
			char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			
			//��Ʊ�����б� string[] //ȡ��Ʊ������и��ֶΣ������һ��ȡ��ѡ�ɿ���û�и��ֶ�
			short codenum = *(short*)(PackBody+offset);
			offset += 2;
			for(int i=0; i<codenum; i++)
			{
				m_ListCtl.InsertItem(i,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
				short codelen = *(short*)(PackBody+offset);
				offset += 2;
				char *code = new char[codelen+1];
				memset(code,0,codelen+1);
				memcpy(code,PackBody+offset,codelen);
				wstring codestr = UTF8_2_UTF16(code);
				m_ListCtl.SetItemText(i,2,codestr.c_str());
				delete []code;
			}
			m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2912:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ӧ�����		byte	0��//��ʾ�����Ѿ��ύ�������֪ͨ�û�
			char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			
			//��Ʊ�����б� string[] //ȡ��Ʊ������и��ֶΣ������һ��ȡ��ѡ�ɿ���û�и��ֶ�
			short codenum = *(short*)(PackBody+offset);
			offset += 2;
			for(int i=0; i<codenum; i++)
			{
				m_ListCtl.InsertItem(i,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
				short codelen = *(short*)(PackBody+offset);
				offset += 2;
				char *code = new char[codelen+1];
				memset(code,0,codelen+1);
				memcpy(code,PackBody+offset,codelen);
				wstring codestr = UTF8_2_UTF16(code);
				m_ListCtl.SetItemText(i,2,codestr.c_str());
				delete []code;
			}
			m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2913:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//������Ѷ����	short	//��ʾ�����ж��٣�id��string����Ŀ��
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				
				m_ListCtl.InsertItem(i,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
				
			}
			for(int j=0; j<RecdNum; j++)
			{
				//id		char	//�������ͣ������涨��  1(��ʱͼ������ 2(k�߹�����3(�������) 4(��ҳ��Ѷ����)��ע����ͬ���͵��ı������ж���
				if(PackLen>offset)
				{
					char idtype = *(PackBody+offset);
					offset += 1;
				    int idval = idtype;
				    CString idstr = _T("");idstr.Format(L"%d",idval);
					m_ListCtl.SetItemText(j,2,idstr);
				}
				//�����ı�	string	//id���͵Ĺ����ı�
				if(PackLen>offset)
				{
					short contextlen = *(short*)(PackLen+offset);
					offset += 2;
					char *text = new char[contextlen+1];
					memset(text,0,contextlen+1);
					memcpy(text,PackBody+offset,contextlen);
					offset += contextlen;
					wstring textstr = UTF8_2_UTF16(text);
					m_ListCtl.SetItemText(j,3,textstr.c_str());
					delete []text;
				}
			}
		}break;
	case 2914:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			char *wmltext = new char[PackLen+1];
			memset(wmltext,0,PackLen+1);
			memcpy(wmltext,PackBody+offset,PackLen);
			offset += PackLen;
			wstring wmlstr = UTF8_2_UTF16(wmltext);
			m_ListCtl.SetItemText(0,1,wmlstr.c_str());
			delete []wmltext;
		}break;
	case 2915:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ȩƽ��ί��۸�	int
			int avgprice = *(int*)(PackLen+offset);
			offset += 4;
			CString avgpricestr = _T("");avgpricestr.Format(L"%d",avgprice);
			m_ListCtl.SetItemText(0,1,avgpricestr);
			//ί������		int
			int tolbuyval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolbuyvalstr = _T("");tolbuyvalstr.Format(L"%d",tolbuyval);
			m_ListCtl.SetItemText(0,2,tolbuyvalstr);
			//��Ȩƽ��ί���۸�	int
			int sellval = *(int*)(PackLen+offset);
			offset += 4;
			CString sellvalstr = _T("");sellvalstr.Format(L"%d",sellval);
			m_ListCtl.SetItemText(0,3,sellvalstr);
			//ί������		int
			int tolsellval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolsellvalstr = _T("");tolsellvalstr.Format(L"%d",tolsellval);
			m_ListCtl.SetItemText(0,4,tolsellvalstr);
			//��¼��         	short       //6��10��������
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.SetItemText(0,5,RecdNumStr);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//������         	int
				if(PackLen>offset)
				{
					int buysell = *(int*)(PackBody+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					m_ListCtl.SetItemText(j,6,buysellstr);
				}
				//������         	int
				if(PackLen>offset)
				{
					int buysellval = *(int*)(PackBody+offset);
					offset += 4;
					CString buysellvalstr = _T("");buysellvalstr.Format(L"%d",buysellval);
					m_ListCtl.SetItemText(j,7,buysellvalstr);
				}
			}
		}break;
	case 2916:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����λ��       int
			int datapos = *(int*)(PackBody+offset);
			offset += 4;
			CString dataposstr = _T("");dataposstr.Format(L"%d",datapos);
			m_ListCtl.SetItemText(0,1,dataposstr);
			//��¼��         	short       //6��10��������
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.SetItemText(0,2,RecdNumStr);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//������         	int
				if(PackLen>offset)
				{
					int datetime = *(int*)(PackBody+offset);
					offset += 4;
					CString datetimestr = _T("");datetimestr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,3,datetimestr);
				}
				//������         	int
				if(PackLen>offset)
				{
					int buysell = *(int*)(PackBody+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					m_ListCtl.SetItemText(j,4,buysellstr);
				}
				//������         	int
				if(PackLen>offset)
				{
					int buysellval = *(int*)(PackBody+offset);
					offset += 4;
					CString buysellvalstr = _T("");buysellvalstr.Format(L"%d",buysellval);
					m_ListCtl.SetItemText(j,5,buysellvalstr);
				}
			}
		}break;
	case 2917:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��������ID	int
			if(PackLen>offset)
			{
			int dataid = *(int*)(PackLen+offset);
			offset += 4;
			CString dataidstr = _T("");dataidstr.Format(L"%d",dataid);
			m_ListCtl.SetItemText(0,1,dataidstr);
			}
			if(PackLen>offset)
			{
			//��������������	char	    1��ʾֻ��һ�����������2��ʾ��������
			char buyselltype = *(PackBody+offset);
			offset += 1;
			int buysellval = buyselltype;
			CString buysellstr = _T("");buysellstr.Format(L"%d",buysellval);
			m_ListCtl.SetItemText(0,2,buysellstr);
			}
			//������������	char	    0��ʾ��, 1��ʾ��
			if(PackLen>offset)
			{
			char buysell = *(PackBody+offset);
			offset += 1;
			int buysellvalue = buysell;
			CString buysellvaluestr = _T("");buysellvaluestr.Format(L"%d",buysellvalue);
			m_ListCtl.SetItemText(0,3,buysellvaluestr);
			}
			//�����۸�		int	 
			if(PackLen>offset)
			{
			int priceval = *(int*)(PackLen+offset);
			offset += 4;
			CString pricevalstr = _T("");pricevalstr.Format(L"%d",priceval);
			m_ListCtl.SetItemText(0,4,pricevalstr);
			}
			//���򣨻���������  int
			if(PackLen>offset)
			{
			int tolalval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolalvalstr = _T("");tolalvalstr.Format(L"%d",tolalval);
			m_ListCtl.SetItemText(0,5,tolalvalstr);
			}
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,6,datevalstr);
				}
			}
			//������������	char	    0��ʾ��, 1��ʾ��
			if(PackLen>offset)
			{
			char buysell2 = *(PackBody+offset);
			offset += 1;
			int buysell2value = buysell2;
			CString buysell2valuestr = _T("");buysell2valuestr.Format(L"%d",buysell2value);
			m_ListCtl.SetItemText(0,7,buysell2valuestr);
			}
			//�����۸�		int	 
			if(PackLen>offset)
			{
			int priceval2 = *(int*)(PackLen+offset);
			offset += 4;
			CString priceval2str = _T("");priceval2str.Format(L"%d",priceval2);
			m_ListCtl.SetItemText(0,8,priceval2str);
			}
			//���򣨻���������  int
			if(PackLen>offset)
			{
			int tolalval2 = *(int*)(PackLen+offset);
			offset += 4;
			CString tolalval2str = _T("");tolalval2str.Format(L"%d",tolalval2);
			m_ListCtl.SetItemText(0,9,tolalval2str);
			}
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			short RecdNum2 = *(short*)(PackBody+offset);
			offset += 2;
			if(RecdNum2>RecdNum)
			{
			    for(int m=0; m<RecdNum; m++)
			    {
				    m_ListCtl.InsertItem(m+1,_T(""));
				    CString xhval = _T("");xhval.Format(L"%d",m+2);
				    m_ListCtl.SetItemText(m+1,0,xhval);
			    }
			}
			for(int n=0; n<RecdNum2; n++)
			{
				//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
				if(PackLen>offset)
				{
					int dateval2 = *(int*)(PackBody+offset);
					offset += 4;
					CString dateval2str = _T("");dateval2str.Format(L"%d",dateval2);
					m_ListCtl.SetItemText(n,10,dateval2str);
				}
			}
		}break;
	case 2918:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddx			short			��ʾ3λС��
				if(PackLen>offset)
				{
					short ddxval = *(short*)(PackBody+offset);
					offset += 2;
					CString ddxvalstr = _T("");ddxvalstr.Format(L"%d",ddxval);
					m_ListCtl.SetItemText(j,3,ddxvalstr);
				}
			}
			
		}break;
	case 2919:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddy			short			��ʾ3λС��
				if(PackLen>offset)
				{
					short ddyval = *(short*)(PackBody+offset);
					offset += 2;
					CString ddyvalstr = _T("");ddyvalstr.Format(L"%d",ddyval);
					m_ListCtl.SetItemText(j,3,ddyvalstr);
				}
			}
		}break;
	case 2920:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddz			int24			��ʾ3λС��
				if(PackLen>offset)
				{
					//m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24�Զ����ʽ����3��char���͵�����
					char *byte = new char[4];
					memset(byte,0,4);
					memcpy(byte,PackBody+offset,3);
					offset += 3;
					wstring bytestr = UTF8_2_UTF16(byte);
					m_ListCtl.SetItemText(j,3,bytestr.c_str());
					delete []byte;
					char byteval[4]={0};
					byteval[0] = *(PackBody+offset-3);
					byteval[0] = byteval[0]&0x7F;
					byteval[1] = *(PackBody+offset-2);
					byteval[2] = *(PackBody+offset-1);
					string str;str.append(byteval[0],1);str.append(byteval[1],1);str.append(byteval[3],1);
					str.append(byteval[4],3);
					wstring strstr = UTF8_2_UTF16(str);
					CString tempstr = _T("");tempstr.Format(L"%s-%s",bytestr.c_str(),strstr.c_str());
				}
				//ddzwidth		char			ddz�ߵĿ��
				if(PackLen>offset)
				{
					char ddzwidth = *(PackBody+offset);
					offset += 1;
					int ddzwidthval = ddzwidth;
					CString ddzwidthvalstr = _T("");ddzwidthvalstr.Format(L"%d",ddzwidthval);
					m_ListCtl.SetItemText(j,4,ddzwidthvalstr);
				}
			}
		}break;
	case 2921:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//���뵥��	uint24		//uint24���޷��ŵ�24λ����
				if(PackLen>offset)
				{
					//m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24�Զ����ʽ����3��char���͵�����
					char *buybyte = new char[4];
					memset(buybyte,0,4);
					memcpy(buybyte,PackBody+offset,3);
					offset += 3;
					wstring buybytestr = UTF8_2_UTF16(buybyte);
					m_ListCtl.SetItemText(j,3,buybytestr.c_str());
					delete []buybyte;
				}
				//��������	uint24		//uint24���޷��ŵ�24λ����
				if(PackLen>offset)
				{
					//m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24�Զ����ʽ����3��char���͵�����
					char *sellbyte = new char[4];
					memset(sellbyte,0,4);
					memcpy(sellbyte,PackBody+offset,3);
					offset += 3;
					wstring sellbytestr = UTF8_2_UTF16(sellbyte);
					m_ListCtl.SetItemText(j,4,sellbytestr.c_str());
					delete []sellbyte;
				}
				//�����е����ϱ���	short	��1000		�����С�����ش�
				if(PackLen>offset)
				{
					short buybl = *(short*)(PackBody+offset);
					offset += 2;
					CString buyblstr = _T("");buyblstr.Format(L"%d",buybl);
					m_ListCtl.SetItemText(j,5,buyblstr);
				}
				//��������ϱ���	short	��1000 		��������ش�
				if(PackLen>offset)
				{
					short buybig = *(short*)(PackBody+offset);
					offset += 2;
					CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
					m_ListCtl.SetItemText(j,6,buybigstr);
				}
				//�����ش󵥱���		short	��1000 		�ش�
				if(PackLen>offset)
				{
					short buybiggest = *(short*)(PackBody+offset);
					offset += 2;
					CString buybiggeststr = _T("");buybiggeststr.Format(L"%d",buybiggest);
					m_ListCtl.SetItemText(j,7,buybiggeststr);
				}
				//�����е����ϱ���	short	��1000		�����С�����ش�
				if(PackLen>offset)
				{
					short sellmid = *(short*)(PackBody+offset);
					offset += 2;
					CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
					m_ListCtl.SetItemText(j,8,sellmidstr);
				}
				//���������ϱ���	short	��1000 		��������ش�
				if(PackLen>offset)
				{
					short sellbig = *(short*)(PackBody+offset);
					offset += 2;
					CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
					m_ListCtl.SetItemText(j,9,sellbigstr);
				}
				//�����ش󵥱���		short	��1000 		�ش�
				if(PackLen>offset)
				{
					short sellbiggest = *(short*)(PackBody+offset);
					offset += 2;
					CString sellbiggeststr = _T("");sellbiggeststr.Format(L"%d",sellbiggest);
					m_ListCtl.SetItemText(j,8,sellbiggeststr);
				}
			}
		}break;
	case 2922:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//ddx��ֵ	short	��1000	
				if(PackLen>offset)
				{
					short dateval = *(short*)(PackBody+offset);
					offset += 2;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
			}
		}break;
	case 2923:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//������ֵ	int24		//int24��24λ����	
				if(PackLen>offset)
				{
					char *dataval = new char[4];
					memset(dataval,0,4);
					memcpy(dataval,PackBody+offset,3);
					offset += 3;
					wstring datavalstr = UTF8_2_UTF16(dataval);
					m_ListCtl.SetItemText(j,2,datavalstr.c_str());
					delete []dataval;
				}
			}
		}break;
	case 2924:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����		int	
				if(PackLen>offset)
				{
					int tolbuy = *(int*)(PackBody+offset);
					offset += 2;
					CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);
					m_ListCtl.SetItemText(j,2,tolbuystr);
				}
				//����		int
				if(PackLen>offset)
				{
					int tolsell = *(int*)(PackBody+offset);
					offset += 2;
					CString tolsellstr = _T("");tolsellstr.Format(L"%d",tolsell);
					m_ListCtl.SetItemText(j,3,tolsellstr);
				}
			}
		}break;
		
	case 2927:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// �����е�����		short	��10000	
			short buymid = *(short*)(PackBody+offset);
			offset += 2;
			CString buymidstr = _T("");buymidstr.Format(L"%d",buymid);
			m_ListCtl.SetItemText(0,1,buymidstr);
			// ����󵥱���		short	��10000
			short buybig = *(short*)(PackBody+offset);
			offset += 2;
			CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
			m_ListCtl.SetItemText(0,2,buybigstr);
			// �����ش󵥱���		short	��10000	
			short buybigger = *(short*)(PackBody+offset);
			offset += 2;
			CString buybiggerstr = _T("");buybiggerstr.Format(L"%d",buybigger);
			m_ListCtl.SetItemText(0,3,buybiggerstr);
			// �����е�����		short	��10000		
			short sellmid = *(short*)(PackBody+offset);
			offset += 2;
			CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
			m_ListCtl.SetItemText(0,4,sellmidstr);
			// �����󵥱���		short	��10000
			short sellbig = *(short*)(PackBody+offset);
			offset += 2;
			CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
			m_ListCtl.SetItemText(0,5,sellbigstr);
			// �����ش󵥱���		short	��10000
			short sellbigger = *(short*)(PackBody+offset);
			offset += 2;
			CString sellbiggerstr = _T("");sellbiggerstr.Format(L"%d",sellbigger);
			m_ListCtl.SetItemText(0,6,sellbiggerstr);
		}break;
	case 2928:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//�����ʽ�ֵ		short��10000			��ʾ2λС��
				if(PackLen>offset)
				{
					short ddyval = *(short*)(PackBody+offset);
					offset += 2;
					CString ddyvalstr = _T("");ddyvalstr.Format(L"%d",ddyval);
					m_ListCtl.SetItemText(j,3,ddyvalstr);
				}
			}
		}break;
	case 2929:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//ί�ж�������	int[]	    ί�ж������飬�õ�λ�ǹ�
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����           String
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackBody+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(j,2,codestr.c_str());
					delete []code;
				}
				if(PackLen>offset)
				{
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackBody+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(j,3,namestr.c_str());
					delete []name;
				}
				//���׵���	  int  *100  
				if(PackLen>offset)
				{
					int gxds = *(short*)(PackBody+offset);
					offset += 4;
					CString gxdsstr = _T("");gxdsstr.Format(L"%d",gxds);
					m_ListCtl.SetItemText(j,4,gxdsstr);
				}
				//�ǵ�		  short*10000 
				if(PackLen>offset)
				{
					int zdval = *(short*)(PackBody+offset);
					offset += 2;
					CString zdvalstr = _T("");zdvalstr.Format(L"%d",zdval);
					m_ListCtl.SetItemText(j,5,zdvalstr);
				}
			}
		}break;
	case 2930:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����		int
			int tolsell = *(int*)(PackBody+offset);
			offset += 4;
			CString tolsellstr = _T("");tolsellstr.Format(L"%d",tolsell);m_ListCtl.SetItemText(0,1,tolsellstr);
			//(��)����	int	//�۸�λ���о�̬������λ��ȷ��
			int sellprice = *(int*)(PackBody+offset);
			offset += 4;
			CString sellpricestr = _T("");sellpricestr.Format(L"%d",sellprice);m_ListCtl.SetItemText(0,2,sellpricestr);
			//����		int
			int tolbuy = *(int*)(PackBody+offset);
			offset += 4;
			CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);m_ListCtl.SetItemText(0,3,tolbuystr);
			//(��)����	int
			int buyprice = *(int*)(PackBody+offset);
			offset += 4;
			CString buypricestr = _T("");buypricestr.Format(L"%d",buyprice);m_ListCtl.SetItemText(0,4,buypricestr);
			//ddx		short��1000
			short ddxval = *(short*)(PackBody+offset);
			offset += 2;
			CString ddxvalstr = _T("");ddxvalstr.Format(L"%d",ddxval);
			m_ListCtl.SetItemText(0,5,ddxvalstr);
			// ������	int
			int datanum = *(int*)(PackBody+offset);
			offset += 4;
			CString datanumstr = _T("");datanumstr.Format(L"%d",datanum);m_ListCtl.SetItemText(0,6,datanumstr);
			//�ش���	int
			int buymid = *(int*)(PackBody+offset);
			offset += 4;
			CString buymidstr = _T("");buymidstr.Format(L"%d",buymid);
			m_ListCtl.SetItemText(0,7,buymidstr);
			// �ش�����	int
			int buybig = *(int*)(PackBody+offset);
			offset += 4;
			CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
			m_ListCtl.SetItemText(0,8,buybigstr);
			// ����	int	
			int buybigger = *(int*)(PackBody+offset);
			offset += 4;
			CString buybiggerstr = _T("");buybiggerstr.Format(L"%d",buybigger);
			m_ListCtl.SetItemText(0,9,buybiggerstr);
			// ������	int		
			int sellmid = *(int*)(PackBody+offset);
			offset += 4;
			CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
			m_ListCtl.SetItemText(0,10,sellmidstr);
			// ����	int
			int sellbig = *(int*)(PackBody+offset);
			offset += 4;
			CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
			m_ListCtl.SetItemText(0,11,sellbigstr);
			// ������	int	
			int sellbigger = *(int*)(PackBody+offset);
			offset += 4;
			CString sellbiggerstr = _T("");sellbiggerstr.Format(L"%d",sellbigger);
			m_ListCtl.SetItemText(0,12,sellbiggerstr);
			// С��	int
			int smallbuy = *(int*)(PackBody+offset);
			offset += 4;
			CString smallbuystr = _T("");smallbuystr.Format(L"%d",smallbuy);
			m_ListCtl.SetItemText(0,13,smallbuystr);
			// С����	int	
			int smallsell = *(int*)(PackBody+offset);
			offset += 4;
			CString smallsellstr = _T("");smallsellstr.Format(L"%d",smallsell);
			m_ListCtl.SetItemText(0,14,smallsellstr);
		}break;
	case 2931:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�����е�����		short	��10000
			short tolsell = *(short*)(PackBody+offset);
			offset += 2;
			CString tolsellstr = _T("");tolsellstr.Format(L"%d",tolsell);m_ListCtl.SetItemText(0,1,tolsellstr);
			//����󵥱���		short	��10000
			short sellprice = *(short*)(PackBody+offset);
			offset += 2;
			CString sellpricestr = _T("");sellpricestr.Format(L"%d",sellprice);m_ListCtl.SetItemText(0,2,sellpricestr);
			//�����ش󵥱���	short	��10000
			short tolbuy = *(short*)(PackBody+offset);
			offset += 2;
			CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);m_ListCtl.SetItemText(0,3,tolbuystr);
			//�����е�����		short	��10000
			short buyprice = *(short*)(PackBody+offset);
			offset += 2;
			CString buypricestr = _T("");buypricestr.Format(L"%d",buyprice);m_ListCtl.SetItemText(0,4,buypricestr);
			//�����󵥱���		short	��10000
			short ddxval = *(short*)(PackBody+offset);
			offset += 2;
			CString ddxvalstr = _T("");ddxvalstr.Format(L"%d",ddxval);
			m_ListCtl.SetItemText(0,5,ddxvalstr);
			// �����ش󵥱���	short	��10000	
			short datanum = *(short*)(PackBody+offset);
			offset += 2;
			CString datanumstr = _T("");datanumstr.Format(L"%d",datanum);m_ListCtl.SetItemText(0,6,datanumstr);
			//����������		short
			short buymid = *(short*)(PackBody+offset);
			offset += 2;
			CString buymidstr = _T("");buymidstr.Format(L"%d",buymid);
			m_ListCtl.SetItemText(0,7,buymidstr);
			// �����������		short
			short buybig = *(short*)(PackBody+offset);
			offset += 2;
			CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
			m_ListCtl.SetItemText(0,8,buybigstr);
			//�����Ի�����		short
			short buybigger = *(short*)(PackBody+offset);
			offset += 2;
			CString buybiggerstr = _T("");buybiggerstr.Format(L"%d",buybigger);
			m_ListCtl.SetItemText(0,9,buybiggerstr);
			//�����»�����		short		
			short sellmid = *(short*)(PackBody+offset);
			offset += 2;
			CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
			m_ListCtl.SetItemText(0,10,sellmidstr);
			// λ��			short	    ���ص����ݵĵ�һ����¼�����
			short sellbig = *(short*)(PackBody+offset);
			offset += 2;
			CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
			m_ListCtl.SetItemText(0,11,sellbigstr);
			// ��¼��		short
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,12,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����		char	    //0��������룻1�����������2�������Ի���3�������»�
				if(PackLen>offset)
				{
					char type = *(PackBody+offset);
					offset += 1;
					int typeval = type;
					CString typestr = _T("");typestr.Format(L"%d",typeval);m_ListCtl.SetItemText(j,13,typestr);
				}
				//ʱ��		short	    //ʱ��HHMM
				if(PackLen>offset)
				{
					short timedata = *(short*)(PackBody+offset);
			        offset += 2;
			        CString timedatastr = _T("");timedatastr.Format(L"%d",timedata);
			        m_ListCtl.SetItemText(j,14,timedatastr);
				}
				//��ص���		int
				if(PackLen>offset)
				{
					int gxds = *(short*)(PackBody+offset);
					offset += 4;
					CString gxdsstr = _T("");gxdsstr.Format(L"%d",gxds);
					m_ListCtl.SetItemText(j,15,gxdsstr);
				}
			}
		}break;
	case 2932:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// λ��			short	    ���ص����ݵĵ�һ����¼�����
			short position = *(short*)(PackBody+offset);
			offset += 2;
			CString positionstr = _T("");positionstr.Format(L"%d",position);
			m_ListCtl.SetItemText(0,1,positionstr);
			// ��¼��		short
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,2,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//��Ʊ����	string
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackBody+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(j,3,codestr.c_str());
					delete []code;
				}
				if(PackLen>offset)
				{
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackBody+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(j,4,namestr.c_str());
					delete []name;
				}
				//����		char	    //0��������룻1�����������2�������Ի���3�������»�
				if(PackLen>offset)
				{
					char type = *(PackBody+offset);
					offset += 1;
					int typeval = type;
					CString typestr = _T("");typestr.Format(L"%d",typeval);m_ListCtl.SetItemText(j,5,typestr);
				}
				//ʱ��		short	    //ʱ��HHMM
				if(PackLen>offset)
				{
					short timedata = *(short*)(PackBody+offset);
			        offset += 2;
			        CString timedatastr = _T("");timedatastr.Format(L"%d",timedata);
			        m_ListCtl.SetItemText(j,6,timedatastr);
				}
				//��ص���		int
				if(PackLen>offset)
				{
					int gxds = *(short*)(PackBody+offset);
					offset += 4;
					CString gxdsstr = _T("");gxdsstr.Format(L"%d",gxds);
					m_ListCtl.SetItemText(j,7,gxdsstr);
				}
			}
		}break;
	case 2933:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// ��¼��		short
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				m_ListCtl.SetItemText(i,0,xh);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//���tag	char	//��ǣ��ñ�Ƕ��������˵��
				if(PackLen>offset)
				{
					char type = *(PackBody+offset);
					offset += 1;
					int typeval = type;
					CString typestr = _T("");typestr.Format(L"%d",typeval);m_ListCtl.SetItemText(j,2,typestr);
				}
				//udd		int	//�۸���ֵ��С����λ��2200�е�С��λ�����壬����۸�λ���͸�ֵ��ͬ
				if(PackLen>offset)
				{
					int udd = *(short*)(PackBody+offset);
			        offset += 4;
			        CString uddstr = _T("");uddstr.Format(L"%d",udd);
			        m_ListCtl.SetItemText(j,3,uddstr);
				}
				// upp		int	//�۸���ֵ
				if(PackLen>offset)
				{
					int upp = *(short*)(PackBody+offset);
			        offset += 4;
			        CString uppstr = _T("");uppstr.Format(L"%d",upp);
			        m_ListCtl.SetItemText(j,4,uppstr);
				}
				//ls		int	//�۸���ֵ
				if(PackLen>offset)
				{
					int ls = *(short*)(PackBody+offset);
					offset += 4;
					CString lsstr = _T("");lsstr.Format(L"%d",ls);
					m_ListCtl.SetItemText(j,5,lsstr);
				}
				//hs		int	//�۸���ֵ
				if(PackLen>offset)
				{
					int hs = *(short*)(PackBody+offset);
					offset += 4;
					CString hsstr = _T("");hsstr.Format(L"%d",hs);
					m_ListCtl.SetItemText(j,6,hsstr);
				}
				//d3		short	//*100
				if(PackLen>offset)
				{
					int d3 = *(short*)(PackBody+offset);
					offset += 4;
					CString d3str = _T("");d3str.Format(L"%d",d3);
					m_ListCtl.SetItemText(j,7,d3str);
				}
			}
		}break;
	case 2934:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,XHno);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			if(PackLen>offset)
			{
				for(int j=0; j<RecdNum; j++)
				{
					//������           int
					int timeval = *(int*)(PackBody+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    m_ListCtl.SetItemText(j,2,timevalStr);
					if(PackLen>offset)
					{
						//�⿪�̼�         int
					    int OPrice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString OPriceStr=_T("");OPriceStr.Format(L"%d",OPrice);
				        m_ListCtl.SetItemText(j,3,OPriceStr);
					}
					if(PackLen>offset)
					{
						//����߼�         int
					    int mhVal = *(int*)(PackBody+offset);
				        offset += 4;
				        CString mhValStr=_T("");mhValStr.Format(L"%d",mhVal);
				        m_ListCtl.SetItemText(j,4,mhValStr);
					}
					if(PackLen>offset)
					{
						//����ͼ�         int
					    int mdprice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString mdpriceStr=_T("");mdpriceStr.Format(L"%d",mdprice);
				        m_ListCtl.SetItemText(j,5,mdpriceStr);
					}
					if(PackLen>offset)
					{
						//�����̼�         int
					    int Sprice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString SpriceStr=_T("");SpriceStr.Format(L"%d",Sprice);
				        m_ListCtl.SetItemText(j,6,SpriceStr);
					}
					if(PackLen>offset)
					{
						//��ɽ���         int
					    int CJValue = *(int*)(PackBody+offset);
				        offset += 4;
				        CString CJValueStr=_T("");CJValueStr.Format(L"%d",CJValue);
				        m_ListCtl.SetItemText(j,7,CJValueStr);
					}
					if(PackLen>offset)
					{
						//��ɽ���         int
					    int CJMoney = *(int*)(PackBody+offset);
				        offset += 4;
				        CString CJMoneyStr=_T("");CJMoneyStr.Format(L"%d",CJMoney);
				        m_ListCtl.SetItemText(j,8,CJMoneyStr);
					}
				}
			}
		}break;
	case 2935:
		{
			//�Ƿ���ʾ��¼   byte      1��ʾ 0����ʾ�����û�и�λ��ʾ����ʾ
			if(PackLen>offset)
			{
			    char nflag = *(PackBody+offset);
			    offset += 1;
				int nMark = nflag;
				if(nMark == 0)
				{
					//����Ӧ������ֶθ���,map��������ĸ���
			        CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
			        m_ListCtl.SetItemText(0, 1, Flag);//��������������
					//ȯ������	short��������Ŀ��
					short QSnum = *(short*)(PackBody+offset);
					CString QS = _T("");QS.Format(L"%d",QSnum);
					m_ListCtl.SetItemText(0, 2, QS);
					offset += 2;
					for(int i=0; i<QSnum; i++)
					{
						short strlen = *(short*)(PackBody+offset);
						offset += 2;
						char *QSValue = new char[strlen+1];
						memset(QSValue,0,strlen+1);
						memcpy(QSValue,PackBody+offset,strlen);
						wstring cstr = UTF8_2_UTF16(QSValue);
						CString No = _T("");No.Format(L"%d",i+1);
						m_ListCtl.SetItemText(i,0,No);
						m_ListCtl.SetItemText(i,3,cstr.c_str());
						offset += strlen;
						delete []QSValue;
					}
				}
				if(nMark != 0)
				{
					CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
			        m_ListCtl.SetItemText(0, 1, Flag);//��������������
					//�����ȯ��wap���ߵ绰��Ŀ��short��������Ŀ��
					if(PackLen>offset)
					{
					    short WTnum = *(short*)(PackBody+offset);
					    offset += 2;
						CString wtcount = _T("");wtcount.Format(L"%d",WTnum);
					    m_ListCtl.SetItemText(0, 2, wtcount);//��������������
						//ί�е�ַ	string
						if(PackLen>offset)
						{
							for(int q=0;q<WTnum;q++)
							{
								//����			char	//1��wap��2�绰
								char type = *(PackBody+offset);
								offset += 1;
								int typeval = type;CString typevalstr = _T("");typevalstr.Format(L"%d",typeval);
								//wap/�绰��ʾ��	string
								short WTlen = *(short*)(PackBody+offset);
								offset += 2;
								char *wtaddr = new char[WTlen+1];
								memset(wtaddr,0,WTlen+1);
								memcpy(wtaddr,PackBody+offset,WTlen);
								offset += WTlen;
								wstring addrstr = UTF8_2_UTF16(wtaddr);
								CString xh = _T("");xh.Format(L"%d",q+1);
								m_ListCtl.SetItemText(q,0,xh);
								m_ListCtl.SetItemText(q,3,typevalstr);
								m_ListCtl.SetItemText(q,4,addrstr.c_str());
								delete []wtaddr;
								//wap/�绰		string
								short telephonelen = *(short*)(PackBody+offset);
								offset += 2;
								char *phone = new char[telephonelen+1];
								memset(phone,0,telephonelen+1);
								memcpy(phone,PackBody+offset,telephonelen);
								offset += telephonelen;
								wstring phonestr = UTF8_2_UTF16(phone);
								m_ListCtl.SetItemText(q,5,phonestr.c_str());
								delete []phone;
							}
						}
					}
				}	
			}
		}break;
	case 2936:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��֤���       byte     ��֤�����
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
			// ���λỰ��ʶ	long     �Ե�½�ɹ�������ͬʱ�᷵��8���ֽڵ�long�Ự��ʶ��Ϣ���ͻ����ڱ��ε�½�Ự��Ҫ����ñ�ʶ��
			__int64 userid = *(__int64*)(PackBody+offset);
			offset += 8;
			CString useridstr = _T("");useridstr.Format(L"%I64d",userid);
			m_ListCtl.SetItemText(0,2,useridstr);
			
			//[long		rightȨ��λ��]  //��֤�ɹ����ص��û�Ĭ��Ȩ�޺����ڣ�ʧ��û�з�
			__int64 qxval = *(__int64*)(PackBody+offset);
			offset += 8;
			char *b = new char[65];
			memset(b,0,65);
			char *qxdata = ok64(qxval,b);
			wstring qxdatastr = UTF8_2_UTF16(qxdata);
			m_ListCtl.SetItemText(0,3,qxdatastr.c_str());
			delete []b;
			//[int[]		Ȩ�����ڣ���right��0λ��63��Ӧ]
			short sznum = *(short*)(PackBody+offset);
			offset += 2;
			for(int j=0; j<sznum; j++)
			{
				int data = *(int*)(PackBody+offset);
				offset += 4;
				CString datastr = _T("");datastr.Format(L"%d",data);
				m_ListCtl.SetItemText(j,4,datastr);
			}
			//short	  ��ʾ����Ŀ
			short noticenum = *(short*)(PackBody+offset);
			offset += 2;
			CString noticenumstr = _T("");noticenumstr.Format(L"%d",noticenum);
			m_ListCtl.SetItemText(0,5,noticenumstr);
			if(noticenum>=sznum)
			{
				for(int m=0; m<noticenum; m++)
				{
					m_ListCtl.InsertItem(m+1,_T(""));
					CString xhval = _T("");xhval.Format(L"%d",m+2);m_ListCtl.SetItemText(m+1,0,xhval);
				}
				for(int n=0; n<noticenum; n++)
				{
				//	byte	  Ȩ����ʾλ����0��ʼ���
					if(PackLen>offset)
					{
					unsigned char bytenotic = *(PackBody+offset);
					offset += 1;
					int ddzwidthval = bytenotic;
					CString ddzwidthvalstr = _T("");ddzwidthvalstr.Format(L"%d",ddzwidthval);
					m_ListCtl.SetItemText(n,6,ddzwidthvalstr);
					}
					//Strint ��ʾ��
					if(PackLen>offset)
					{
					short strlen = *(short*)(PackBody+offset);
					offset += 2;
					char *notice = new char[strlen+1];
					memset(notice,0,strlen+1);
					memcpy(notice,PackBody+offset,strlen);
					offset += strlen;
					wstring noticestr = UTF8_2_UTF16(notice);
					m_ListCtl.SetItemText(n,7,noticestr.c_str());
					delete []notice;
					}
				}
			}
		}break;
	case 2937:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//�����Ŀ	short		//ָ���һ������Ŀ
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			int j=0;
			while(PackLen-offset>0)
			{
				//������		string
				m_ListCtl.InsertItem(j+1,_T(""));
				CString xhval = _T("");xhval.Format(L"%d",j+2);
				m_ListCtl.SetItemText(j+1,0,xhval);
				if(PackLen>offset)
				{
					short strlen = *(short*)(PackBody+offset);
					offset += 2;
					char *classname = new char[strlen+1];
					memset(classname,0,strlen+1);
					memcpy(classname,PackBody+offset,strlen);
					offset += strlen;
					wstring classnamestr = UTF8_2_UTF16(classname);
					m_ListCtl.SetItemText(j,2,classnamestr.c_str());
					delete []classname;
				}
				//�ý����ʾ��ʽ	char	//�ý���ڽ�������ʾ��ʽ��0��ʾ�б�ʽ��1��ʾ�ڲ˵���ʽ	 
				if(PackLen>offset)
				{
					char distype = *(PackBody+offset);
			        offset+=1;
			        int distypeval = distype;CString distypeStr = _T("");distypeStr.Format(L"%d",distypeval);
			        m_ListCtl.SetItemText(j,3,distypeStr);
				}
				//����Ʒ��		char	//���ֶν�����Ҷ�����Ч��ΪҶ����ʾ��Ʊ���ͣ���2200�ӿڣ����Թ�ָ�ڻ�����һ����������ͣ�127��������ͽ������ڿͻ��������ǹ�ָ�ڻ����ͣ��ڲ�������Ȼ���չ�ָ�ڻ���8�����ʹ���	 
				if(PackLen>offset)
				{
					char classtype = *(PackBody+offset);
			        offset+=1;
			        int classtypeval = classtype;CString classtypeStr = _T("");classtypeStr.Format(L"%d",classtypeval);
			        m_ListCtl.SetItemText(j,4,classtypeStr);
				}
				//�ý������		char	//0��ʾ�ýڵ���Ҷ��㣬�����short�Ǹý���id��1��ʾ�ý���Ƿ�Ҷ��㣬�����short��¼���ӽ����Ŀ
				if(PackLen>offset)
				{
					char jdtype = *(PackBody+offset);
			        offset+=1;
			        int jdtypeval = jdtype;CString jdtypeStr = _T("");jdtypeStr.Format(L"%d",jdtypeval);
			        m_ListCtl.SetItemText(j,5,jdtypeStr);
				}
				//������Ŀ������id	short
				if(PackLen>offset)
				{
					unsigned short numid = *(unsigned short*)(PackBody+offset);
			        offset+=2;
			        CString numidStr = _T("");numidStr.Format(L"%d",numid);
			        m_ListCtl.SetItemText(j,6,numidStr);
				}
				j++;
			}
		}break;
	case 2938:
		{
			//�ж�������־�����Ի�������ֶ�
			char bsflag = *(SendBuffer+Datalen-1);
			int nflag = bsflag;
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����Ŀ	short	//�÷���֤ȯ����Ŀ
			short tolnum = *(short*)(PackBody+offset);
			offset+=2;
			CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
			m_ListCtl.SetItemText(0,1,tolnumStr);
			// ��Ŀ	short	//���нṹ����Ŀ
			short count = *(short*)(PackBody+offset);
			offset+=2;
			CString countStr = _T("");countStr.Format(L"%d",count);
			m_ListCtl.SetItemText(0,2,countStr);
			int m=0;
			for(int i=0; i<count; i++)
			{
				CString xhval= _T("");xhval.Format(L"%d",m+2);
				m_ListCtl.InsertItem(m+1,_T(""));
			    m_ListCtl.SetItemText(m+1,0,xhval);
				//����		 string
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackBody+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(m,3,codestr.c_str());
					delete []code;
				}
				//����		 string	
				if(PackLen>offset)
				{
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackBody+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(m,4,namestr.c_str());
					delete []name;
				}
				//�۸�λ��	 char
				if(PackLen>offset)
				{
					char pricepos = *(PackBody+offset);
			        offset+=1;
			        int priceposval = pricepos;CString priceposStr = _T("");priceposStr.Format(L"%d",priceposval);
			        m_ListCtl.SetItemText(m,5,priceposStr);
				}
				//�ɽ�����λ	 short 
				if(PackLen>offset)
				{
					unsigned short cjlval = *(unsigned short*)(PackBody+offset);
			        offset+=2;
			        CString cjlStr = _T("");cjlStr.Format(L"%d",cjlval);
			        m_ListCtl.SetItemText(m,6,cjlStr);
				}
				//����		 int 
				if(PackLen>offset)
				{
					int newval = *(int*)(PackBody+offset);
			        offset+=4;
			        CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			        m_ListCtl.SetItemText(m,7,newvalStr);
				}
				//����		 int
				if(PackLen>offset)
				{
					int zsval = *(int*)(PackBody+offset);
			        offset+=4;
			        CString zsvalStr = _T("");zsvalStr.Format(L"%d",zsval);
			        m_ListCtl.SetItemText(m,8,zsvalStr);
				}
				//����		 int	  //�ɽ���
				if(PackLen>offset)
				{
					int tolval = *(int*)(PackBody+offset);
			        offset+=4;
			        CString tolvalStr = _T("");tolvalStr.Format(L"%d",tolval);
			        m_ListCtl.SetItemText(m,9,tolvalStr);
				}
				//���		 int
				if(PackLen>offset)
				{
					unsigned int highval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			        m_ListCtl.SetItemText(m,10,highvalStr);
				}
				//���		 int  
				if(PackLen>offset)
				{
					unsigned int lowval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			        m_ListCtl.SetItemText(m,11,lowvalStr);
				}
				//����		 int
				if(PackLen>offset)
				{
					unsigned int openval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString openvalStr = _T("");openvalStr.Format(L"%d",openval);
			        m_ListCtl.SetItemText(m,12,openvalStr);
				}
				//�ɽ���	 int
				if(PackLen>offset)
				{
					unsigned int cjval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString cjvalStr = _T("");cjvalStr.Format(L"%d",cjval);
			        m_ListCtl.SetItemText(m,13,cjvalStr);
				}
	            //�����	 int
				if(PackLen>offset)
				{
					unsigned int settleval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString settlevalStr = _T("");settlevalStr.Format(L"%d",settleval);
			        m_ListCtl.SetItemText(m,14,settlevalStr);
				}
	            //������	 int	//ǰ���
				if(PackLen>offset)
				{
					unsigned int zsettleval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString zsettlevalStr = _T("");zsettlevalStr.Format(L"%d",zsettleval);
			        m_ListCtl.SetItemText(m,15,zsettlevalStr);
				}
	            //�ֲ�		 int	//���ڻ�����ָ��������
				if(PackLen>offset)
				{
					unsigned int holdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString holdvalStr = _T("");holdvalStr.Format(L"%d",holdval);
			        m_ListCtl.SetItemText(m,16,holdvalStr);
				}
	            //���ճֲ�	 int	//���ڻ�����ָ��������
				if(PackLen>offset)
				{
					unsigned int zholdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString zholdvalStr = _T("");zholdvalStr.Format(L"%d",zholdval);
			        m_ListCtl.SetItemText(m,17,zholdvalStr);
				}
				unsigned short buysellnum=0;
				if(nflag == 1)
				{
				    //���������ݸ���   short
				    if(PackLen>offset)
				    {
					    buysellnum = *(unsigned short*)(PackBody+offset);
			            offset+=2;
			            CString buysellnumStr = _T("");buysellnumStr.Format(L"%d",buysellnum);
			            m_ListCtl.SetItemText(m,18,buysellnumStr);
				    }
					for(int f=0; f<buysellnum; f++)
					{
						for(int k=0; k<buysellnum; k++)
						{
							m_ListCtl.InsertItem(m+f+1,_T(""));
							CString xhval2=_T("");xhval2.Format(L"%d",m+f+1);m_ListCtl.SetItemText(m+f,0,xhval2);
						}
					    //�����۸�      int
					    if(PackLen>offset)
				        {
					        unsigned int bsprival = *(unsigned int*)(PackBody+offset);
			                offset+=4;
			                CString bsprivalStr = _T("");bsprivalStr.Format(L"%d",bsprival);
			                m_ListCtl.SetItemText(m+f,19,bsprivalStr);
				        }
					    //��������      int  
					    if(PackLen>offset)
				        {
					        unsigned int bsnumval = *(unsigned int*)(PackBody+offset);
			                offset+=4;
			                CString bsnumvalStr = _T("");bsnumvalStr.Format(L"%d",bsnumval);
			                m_ListCtl.SetItemText(m+f,20,bsnumvalStr);
				        }
					}
					m += buysellnum;
				}
			}	
		}break;
	case 2939:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����		 string
			if(PackLen>offset)
			{
			    short codelen = *(short*)(PackBody+offset);
				offset += 2;
				char *code = new char[codelen+1];
				memset(code,0,codelen+1);
				memcpy(code,PackBody+offset,codelen);
				offset += codelen;
				wstring codestr = UTF8_2_UTF16(code);
				m_ListCtl.SetItemText(0,1,codestr.c_str());
				delete []code;
			}
			//����		 string	
			if(PackLen>offset)
			{
				short namelen = *(short*)(PackBody+offset);
				offset += 2;
				char *name = new char[namelen+1];
				memset(name,0,namelen+1);
				memcpy(name,PackBody+offset,namelen);
				offset += namelen;
				wstring namestr = UTF8_2_UTF16(name);
				m_ListCtl.SetItemText(0,2,namestr.c_str());
				delete []name;
			}
			//����		 char	//��2200�ӿڵĹ�Ʊ����
			if(PackLen>offset)
			{
				char type = *(PackBody+offset);
			    offset+=1;
			    int typeval = type;CString typeStr = _T("");typeStr.Format(L"%d",typeval);
			    m_ListCtl.SetItemText(0,3,typeStr);
			}
			//�۸�λ��	 char
			if(PackLen>offset)
			{
				char pricepos = *(PackBody+offset);
			    offset+=1;
			    int priceposval = pricepos;CString priceposStr = _T("");priceposStr.Format(L"%d",priceposval);
			    m_ListCtl.SetItemText(0,4,priceposStr);
			}
			//�ɽ�����λ	 short 
			if(PackLen>offset)
			{
				unsigned short cjlval = *(unsigned short*)(PackBody+offset);
			    offset+=2;
			    CString cjlStr = _T("");cjlStr.Format(L"%d",cjlval);
			    m_ListCtl.SetItemText(0,5,cjlStr);
			}
			//����		 int
			if(PackLen>offset)
			{
				int zsval = *(int*)(PackBody+offset);
			    offset+=4;
			    CString zsvalStr = _T("");zsvalStr.Format(L"%d",zsval);
			    m_ListCtl.SetItemText(0,6,zsvalStr);
		    }
			//��ͣ		 int
			if(PackLen>offset)
			{
				unsigned int highval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			    m_ListCtl.SetItemText(0,7,highvalStr);
			}
			//��ͣ		 int  
			if(PackLen>offset)
			{
				unsigned int lowval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			    m_ListCtl.SetItemText(0,8,lowvalStr);
			}
			//��ͨ��	 int	//���ڻ�����ָ�����ճֲ֣���λ����
			if(PackLen>offset)
			{
				unsigned int lypval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString lypvalStr = _T("");lypvalStr.Format(L"%d",lypval);
			    m_ListCtl.SetItemText(0,9,lypvalStr);
			}
			//�ܹɱ�	 int	//����Ʒ��������
			if(PackLen>offset)
			{
				unsigned int cjval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString cjvalStr = _T("");cjvalStr.Format(L"%d",cjval);
			    m_ListCtl.SetItemText(0,10,cjvalStr);
			}
			//������ȯ���   char   //1��������ȯ��ģ�0����  	  2013-9-23
			if(PackLen>offset)
			{
				char rzrqflag = *(PackBody+offset);
				offset += 1;
				int rzrqval = rzrqflag;CString rzrqstr=_T("");rzrqstr.Format(L"%d",rzrqval);
				m_ListCtl.SetItemText(0,11,rzrqstr);
			}
	        //��������λ     int    //�����Ҫ���ڸ۹ɵ�ί��ʹ�õĽ��������������г�����ֵ�ͳɽ�����λ��ͬ     2015-8-29
		    if(PackLen>offset)
			{
				unsigned int tradeunit = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString tradeunitStr = _T("");tradeunitStr.Format(L"%d",tradeunit);
			    m_ListCtl.SetItemText(0,12,tradeunitStr);
			}
			//֤ȯ��չ����	 char	//0��Ч��1�������壬2��������	2016-4-8
			if(PackLen>offset)
			{
				char codeflag = *(PackBody+offset);
				offset += 1;
				int codeflagval = codeflag;CString codeflagstr=_T("");codeflagstr.Format(L"%d",codeflagval);
				m_ListCtl.SetItemText(0,13,codeflagstr);
			}
		}break;
	case 2940:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//���ݱ��(tag)	 char	//����ۡ��ֲֺ������ֶα�ǣ�Ϊ0û����3�����ݣ�1����3������ 
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//����		 int
			if(PackLen>offset)
			{
				unsigned int newval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			    m_ListCtl.SetItemText(0,2,newvalStr);
			}
			//��		 int
			if(PackLen>offset)
			{
				int openval = *(int*)(PackBody+offset);
			    offset+=4;
			    CString openvalStr = _T("");openvalStr.Format(L"%d",openval);
			    m_ListCtl.SetItemText(0,3,openvalStr);
		    }
			//���		 int
			if(PackLen>offset)
			{
				unsigned int highval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			    m_ListCtl.SetItemText(0,4,highvalStr);
			}
			//���		 int
			if(PackLen>offset)
			{
				unsigned int lowval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			    m_ListCtl.SetItemText(0,5,lowvalStr);
			}
			//�ɽ���	 int	//Ҳ������
			if(PackLen>offset)
			{
				unsigned int cjlval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString cjlvalStr = _T("");cjlvalStr.Format(L"%d",cjlval);
			    m_ListCtl.SetItemText(0,6,cjlvalStr);
			}
			//�ܶ�		 int
			if(PackLen>offset)
			{
				unsigned int cjval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString cjvalStr = _T("");cjvalStr.Format(L"%d",cjval);
			    m_ListCtl.SetItemText(0,7,cjvalStr);
			}
			//����		 int
			if(PackLen>offset)
			{
				unsigned int innerval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString innervalStr = _T("");innervalStr.Format(L"%d",innerval);
			    m_ListCtl.SetItemText(0,8,innervalStr);
			}
	        //����		 int
		    if(PackLen>offset)
			{
				unsigned int tradeunit = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString tradeunitStr = _T("");tradeunitStr.Format(L"%d",tradeunit);
			    m_ListCtl.SetItemText(0,9,tradeunitStr);
			}
			//����		 int
			if(PackLen>offset)
			{
				unsigned int avgpri = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString avgpriStr = _T("");avgpriStr.Format(L"%d",avgpri);
			    m_ListCtl.SetItemText(0,10,avgpriStr);
			}
			//tagΪ1������3���ֶ�	  
	 // �����	 int	//����Ʒ���ڻ�����ָ���������
			if(TAGval == 1)
			{
				if(PackLen>offset)
			    {
				    unsigned int settlepri = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString settlepriStr = _T("");settlepriStr.Format(L"%d",settlepri);
			        m_ListCtl.SetItemText(0,11,settlepriStr);
			    }
				//�ֲ�	 	 int	//�ڻ�����ָ���������
				if(PackLen>offset)
			    {
				    unsigned int holdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString holdvalStr = _T("");holdvalStr.Format(L"%d",holdval);
			        m_ListCtl.SetItemText(0,12,holdvalStr);
			    }
				//����		 int	//�ڻ�����ָ���������
				if(PackLen>offset)
			    {
				    unsigned int addholdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString addholdvalStr = _T("");addholdvalStr.Format(L"%d",addholdval);
			        m_ListCtl.SetItemText(0,13,addholdvalStr);
			    }
			}
			//����		 short
			if(PackLen>offset)
			{
				unsigned short val = *(unsigned short*)(PackBody+offset);
			    offset+=2;
			    CString valStr = _T("");valStr.Format(L"%d",val);
			    m_ListCtl.SetItemText(0,14,valStr);
			}
			//�����̼�¼��   short 
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
			    offset+=2;
			    RecdNumStr.Format(L"%d",RecdNum);
			    m_ListCtl.SetItemText(0,15,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval=_T("");xhval.Format(L"%d",i+2);m_ListCtl.SetItemText(i+1,0,xhval);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//������         int
				if(PackLen>offset)
			    {
				    unsigned int bspridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bspridvalStr = _T("");bspridvalStr.Format(L"%d",bspridval);
			        m_ListCtl.SetItemText(j,16,bspridvalStr);
			    }
				//������         int
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,17,bsdvalStr);
			    }
			}
		}break;
	case 2941:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֱֲ��(tag)	char	    //1�����ֲ֣�0�������ֲ�
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//����λ��	 int	     ��һ����ʱ���ݵ�λ��
			if(PackLen>offset)
			{
				unsigned int newval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			    m_ListCtl.SetItemText(0,2,newvalStr);
			}
			//��Ŀ		 short	     ��ʱ������Ŀ
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
			    offset+=2;
			    RecdNumStr.Format(L"%d",RecdNum);
			    m_ListCtl.SetItemText(0,3,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval=_T("");xhval.Format(L"%d",i+2);m_ListCtl.SetItemText(i+1,0,xhval);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//ʱ��	 int
				if(PackLen>offset)
			    {
				    unsigned int TIME = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString TIMEStr = _T("");TIMEStr.Format(L"%d",TIME);
			        m_ListCtl.SetItemText(j,4,TIMEStr);
			    }
				//������         int
				if(PackLen>offset)
			    {
				    unsigned int bspridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bspridvalStr = _T("");bspridvalStr.Format(L"%d",bspridval);
			        m_ListCtl.SetItemText(j,5,bspridvalStr);
			    }
				//�ɽ���	 int	     �ɽ�����ʾ�ǵ�ǰ�ɽ�����������
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,6,bsdvalStr);
			    }
				//�ֲ���	 int	     �ܵĳֲ�����tagΪ0û�и��ֶ�
				if(TAGval != 0)
				{
				    if(PackLen>offset)
			        {
				        unsigned int HOLDval = *(unsigned int*)(PackBody+offset);
			            offset+=4;
			            CString HOLDvalStr = _T("");HOLDvalStr.Format(L"%d",HOLDval);
			            m_ListCtl.SetItemText(j,7,HOLDvalStr);
			        }
				}
			}
		}break;
	case 2942:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֱֲ��(tag)	char	    //1�����ֲ֣�0�������ֲ�
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//��Ϣ������     char   
			if(PackLen>offset)
			{
				char msgnum = *(PackBody+offset);
			    offset+=1;
			    int msgnumval = msgnum;CString msgnumStr = _T("");msgnumStr.Format(L"%d",msgnumval);
			    m_ListCtl.SetItemText(0,2,msgnumStr);
			}
			//��������       char        0-5��ֵ 
			if(PackLen>offset)
			{
				char STAR = *(PackBody+offset);
			    offset+=1;
			    int STARval = STAR;CString STARStr = _T("");STARStr.Format(L"%d",STARval);
			    m_ListCtl.SetItemText(0,3,STARStr);
			}
			//����λ��	 SHORT	     ��һ����ʱ���ݵ�λ��
			if(PackLen>offset)
			{
				unsigned short newval = *(unsigned short*)(PackBody+offset);
			    offset+=2;
			    CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			    m_ListCtl.SetItemText(0,4,newvalStr);
			}
			//��Ŀ		 short	     ��ʱ������Ŀ
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
			    offset+=2;
			    RecdNumStr.Format(L"%d",RecdNum);
			    m_ListCtl.SetItemText(0,5,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval=_T("");xhval.Format(L"%d",i+2);m_ListCtl.SetItemText(i+1,0,xhval);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//ʱ��	 int
				if(PackLen>offset)
			    {
				    unsigned int TIME = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString TIMEStr = _T("");TIMEStr.Format(L"%d",TIME);
			        m_ListCtl.SetItemText(j,6,TIMEStr);
			    }
				//������         int
				if(PackLen>offset)
			    {
				    unsigned int bspridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bspridvalStr = _T("");bspridvalStr.Format(L"%d",bspridval);
			        m_ListCtl.SetItemText(j,7,bspridvalStr);
			    }
				//�ɽ���	 int	     �ɽ�����ʾ�ǵ�ǰ�ɽ�����������
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,8,bsdvalStr);
			    }
				//����           int 
				if(PackLen>offset)
			    {
				    unsigned int avgval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString avgvalStr = _T("");avgvalStr.Format(L"%d",avgval);
			        m_ListCtl.SetItemText(j,9,avgvalStr);
			    }
				if(TAGval!=0)
				{
				//�ֲ���	 int	     �ܵĳֲ�����tagΪ0û�и��ֶ�
				    if(PackLen>offset)
			        {
				        unsigned int HOLDval = *(unsigned int*)(PackBody+offset);
			            offset+=4;
			            CString HOLDvalStr = _T("");HOLDvalStr.Format(L"%d",HOLDval);
			            m_ListCtl.SetItemText(j,10,HOLDvalStr);
			        }
				}
			}
		}break;
	case 2943:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��¼��         short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
			    offset+=2;
			    RecdNumStr.Format(L"%d",RecdNum);
			    m_ListCtl.SetItemText(0,1,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval=_T("");xhval.Format(L"%d",i+2);m_ListCtl.SetItemText(i+1,0,xhval);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//����		 string
			    if(PackLen>offset)
			    {
			        short codelen = *(short*)(PackBody+offset);
				    offset += 2;
				    char *code = new char[codelen+1];
				    memset(code,0,codelen+1);
				    memcpy(code,PackBody+offset,codelen);
				    offset += codelen;
				    wstring codestr = UTF8_2_UTF16(code);
				    m_ListCtl.SetItemText(j,2,codestr.c_str());
				    delete []code;
			    }
			    //����		 string	
			    if(PackLen>offset)
			    {
				    short namelen = *(short*)(PackBody+offset);
				    offset += 2;
				    char *name = new char[namelen+1];
				    memset(name,0,namelen+1);
				    memcpy(name,PackBody+offset,namelen);
				    offset += namelen;
				    wstring namestr = UTF8_2_UTF16(name);
				    m_ListCtl.SetItemText(j,3,namestr.c_str());
				    delete []name;
			    }
				//��Ʊ����	  char 
				if(PackLen>offset)
			    {
				    char stocktype = *(PackBody+offset);
			        offset+=1;
					int stocktypeval = stocktype;
			        CString stocktypeStr = _T("");stocktypeStr.Format(L"%d",stocktypeval);
			        m_ListCtl.SetItemText(j,4,stocktypeStr);
			    }
			}
		}break;
	case 2944:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֱֲ��(tag)	char	    //1�����ֲ֣�0�������ֲ�
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//��¼��         short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
			    offset+=2;
			    RecdNumStr.Format(L"%d",RecdNum);
			    m_ListCtl.SetItemText(0,1,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval=_T("");xhval.Format(L"%d",i+2);m_ListCtl.SetItemText(i+1,0,xhval);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//����           int
				if(PackLen>offset)
			    {
				    unsigned int TIME = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString TIMEStr = _T("");TIMEStr.Format(L"%d",TIME);
			        m_ListCtl.SetItemText(j,2,TIMEStr);
			    }
				//���̼�         int
				if(PackLen>offset)
			    {
				    unsigned int openpridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString openpridvalStr = _T("");openpridvalStr.Format(L"%d",openpridval);
			        m_ListCtl.SetItemText(j,3,openpridvalStr);
			    }
				//��߼�         int
				if(PackLen>offset)
			    {
				    unsigned int highval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			        m_ListCtl.SetItemText(j,4,highvalStr);
			    }
				//��ͼ�         int
				if(PackLen>offset)
			    {
				    unsigned int lowval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			        m_ListCtl.SetItemText(j,5,lowvalStr);
			    }
				//���̼�         int 
				if(PackLen>offset)
			    {
				    unsigned int closeval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString closevalStr = _T("");closevalStr.Format(L"%d",closeval);
			        m_ListCtl.SetItemText(j,6,closevalStr);
			    }
				//�ɽ���         int
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,7,bsdvalStr);
			    }
				//�ɽ���         int
				if(PackLen>offset)
			    {
				    unsigned int cjmoney = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString cjmoneyStr = _T("");cjmoneyStr.Format(L"%d",cjmoney);
			        m_ListCtl.SetItemText(j,8,cjmoneyStr);
			    }
				//�ֲ���	 int	     �ܵĳֲ�����tagΪ0û�и��ֶ�
				if(TAGval!=0)
				{
				    if(PackLen>offset)
			        {
				        unsigned int HOLDval = *(unsigned int*)(PackBody+offset);
			            offset+=4;
			            CString HOLDvalStr = _T("");HOLDvalStr.Format(L"%d",HOLDval);
			            m_ListCtl.SetItemText(j,10,HOLDvalStr);
			        }
				}
			}
		}break;
	case 2945:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֱֲ��(tag)	char	    //1�����ֲ֣�0�������ֲ�
			int TAGval = 0;
			if(PackLen>offset)
			{
				short strlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[strlen+1];
				memset(context,0,strlen+1);
				memcpy(context,PackBody+offset,strlen);
				offset += strlen;
				wstring textstr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,1,textstr.c_str());
				delete []context;
			}
		}break;
	case 2946:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//int	�¹���id
			if(PackLen>offset)
			{
				int noticeid = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr = _T("");IdStr.Format(L"%d",noticeid);
				m_ListCtl.SetItemText(0,1,IdStr);
			}
			//String 	��������
			if(PackLen>offset)
			{
				short strlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[strlen+1];
				memset(context,0,strlen+1);
				memcpy(context,PackBody+offset,strlen);
				offset += strlen;
				wstring textstr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,2,textstr.c_str());
				delete []context;
			}
		}break;
	case 2947:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			//short	��ʾ��Ϣ��Ŀ
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i,_T(""));
				CString XHVAL = _T("");XHVAL.Format(L"%d",i+1);
			    m_ListCtl.SetItemText(i,0,XHVAL);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			//int	�¹���id
			for(int j=0; j<RecdNum; j++)
			{
			    if(PackLen>offset)
			    {
				    int noticeid = *(int*)(PackBody+offset);
				    offset += 4;
				    CString IdStr = _T("");IdStr.Format(L"%d",noticeid);
				    m_ListCtl.SetItemText(j,2,IdStr);
			    }
			    //String 	��������
			    if(PackLen>offset)
			    {
				    short strlen = *(short*)(PackBody+offset);
				    offset += 2;
				    char *context = new char[strlen+1];
				    memset(context,0,strlen+1);
				    memcpy(context,PackBody+offset,strlen);
				    offset += strlen;
				    wstring textstr = UTF8_2_UTF16(context);
				    m_ListCtl.SetItemText(j,3,textstr.c_str());
				    delete []context;
			    }
			}
		}break;
	case 2948:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//String 	��������
			if(PackLen>offset)
			{
				short strlen = *(short*)(PackBody+offset);
				offset += 2;
				char *context = new char[strlen+1];
				memset(context,0,strlen+1);
				memcpy(context,PackBody+offset,strlen);
				offset += strlen;
				wstring textstr = UTF8_2_UTF16(context);
				m_ListCtl.SetItemText(0,1,textstr.c_str());
				delete []context;
			}
			//int	�û�id
			if(PackLen>offset)
			{
				int noticeid = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr = _T("");IdStr.Format(L"%d",noticeid);
				m_ListCtl.SetItemText(0,2,IdStr);
			}
		}break;
		
	case 2955:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�б�Χ       short
			short listrange = 0;
			if(PackLen>offset)
			{
				listrange = *(short*)(PackBody+offset);
				offset += 2;
				CString range = _T("");range.Format(L"%d",listrange);m_ListCtl.SetItemText(0,1,range);
			}
			//�б��ֶ�����	short	���ֶζ��������˵��
			unsigned short listattr = 0;
			if(PackLen>offset)
			{
				listattr = *(short*)(PackBody+offset);
				offset += 2;
				CString listattrStr = _T("");listattrStr.Format(L"%d",listattr);
				m_ListCtl.SetItemText(0,2,listattrStr);
			}
			
			char *tempbyte = new char[17];
			memset(tempbyte,0,17);
			char *attrutitude = short_T_byte(listattr,tempbyte);
			printf("�б��ֶ�����:%s",tempbyte);
			
			//�ܼ�¼��	short	�������˸������ܼ�¼��
			if(PackLen>offset)
			{
				short tolnum = *(short*)(PackBody+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,3,tolnumStr);
			}
			//��¼��         short	���м�¼��
			if(PackLen>offset)
			{
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				CString RecdNumStr = _T("");RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,4,RecdNumStr);
			}
			if(RecdNum>0)
			{
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval = _T("");xhval.Format(L"%d",i+2);
			    m_ListCtl.SetItemText(i+1,0,xhval);
			}
			}
			if(RecdNum>0)
			{
			for(int j=0; j<RecdNum; j++)
			{
				//����           String
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackBody+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(j,5,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLen>offset)
				{
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackBody+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(j,6,namestr.c_str());
					delete []name;
				}
				//С��λ��       byte
				if(PackLen>offset)
				{
					unsigned char pointpos = *(PackBody+offset);
					offset += 1;
					int pointval = pointpos;CString pointstr=_T("");pointstr.Format(L"%d",pointval);
					m_ListCtl.SetItemText(j,7,pointstr);
				}
				//��Ʊ����       byte
				if(PackLen>offset)
				{
					unsigned char stocktype = *(PackBody+offset);
					offset += 1;
					int stocktypeval = stocktype;CString stocktypestr=_T("");stocktypestr.Format(L"%d",stocktypeval);
					m_ListCtl.SetItemText(j,8,stocktypestr);
				}
				//���ռ�         int
				if(PackLen>offset)
				{
					int zsprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(j,9,zspricestr);
				}
				//���̼�         int
				if(PackLen>offset)
				{
					int openprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(j,10,openpricestr);
				}
				//���¼�         int
				if(PackLen>offset)
				{
					int newprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(j,11,newpricestr);
				}
				//��߼�         int
				if(PackLen>offset)
				{
					int highprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(j,12,highpricestr);
				}
				//��ͼ�         int
				if(PackLen>offset)
				{
					int lowprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(j,13,lowpricestr);
				}
				//�ɽ���         int
				if(PackLen>offset)
				{
					int cjmoney = *(int*)(PackBody+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(j,14,cjmoneystr);
				}
				//������ָ���ɷֹɵ�id    short
				if(listrange==105||listrange==113||listrange==114)
				{
				    if(PackLen>offset)
				    {
					    short indexid = *(short*)(PackBody+offset);
				        offset += 2;
					    CString indexidstr = _T("");indexidstr.Format(L"%u",indexid);
					    m_ListCtl.SetItemText(j,15,indexidstr);
				    }
				}
				//int position = attrutitude[0];
				if(listattr!=0&&listattr&(0x0001))
				{
					//�ɽ���λ	  short
				    if(PackLen>offset)
				    {
					    short cjpos = *(short*)(PackBody+offset);
				        offset += 2;
					    CString cjposstr = _T("");cjposstr.Format(L"%u",cjpos);
					    m_ListCtl.SetItemText(j,16,cjposstr);
				    }
					//�ɽ���         int
				    if(PackLen>offset)
				    {
					    unsigned int cjvalue = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					    m_ListCtl.SetItemText(j,17,cjvaluestr);
				    }
				}
				if(listattr!=0&&listattr&(0x0002))
				{
					//���̳ɽ���     int       ------1λ
					if(PackLen>offset)
				    {
					    unsigned int innerval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString innervalstr = _T("");innervalstr.Format(L"%u",innerval);
					    m_ListCtl.SetItemText(j,18,innervalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0004))
				{
					//����           int       ------2λ
					if(PackLen>offset)
				    {
					    unsigned int xsval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString xsvalstr = _T("");xsvalstr.Format(L"%u",xsval);
					    m_ListCtl.SetItemText(j,19,xsvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0008))
				{
					//����           short��100------3λ
					if(PackLen>offset)
				    {
					    short lbval = *(short*)(PackBody+offset);
				        offset += 2;
						float lbvalues = (float)lbval/100;
					    CString lbvalstr = _T("");lbvalstr.Format(L"%3f",lbvalues);
					    m_ListCtl.SetItemText(j,20,lbvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0010))
				{
					//����           short��10000----4λ
					if(PackLen>offset)
				    {
					    unsigned short hsval = *(unsigned short*)(PackBody+offset);
				        offset += 2;
						double hsvalue = hsval/10000;
					    CString hsvalstr = _T("");hsvalstr.Format(L"%lf",hsvalue);
					    m_ListCtl.SetItemText(j,21,hsvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0020))
				{
					//����           short��10000----5λ  ��������
					if(PackLen>offset)
				    {
					    short zsval = *(short*)(PackBody+offset);
				        offset += 2;
						double zsvalue = zsval/10000;
					    CString zsvalstr = _T("");zsvalstr.Format(L"%lf",zsvalue);
					    m_ListCtl.SetItemText(j,22,zsvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0040))
				{
					//ί��           short��10000----6λ  ��������
					if(PackLen>offset)
				    {
					    short wbval = *(short*)(PackBody+offset);
				        offset += 2;
						double wbvalue = wbval/10000;
					    CString wbvalstr = _T("");wbvalstr.Format(L"%lf",wbvalue);
					    m_ListCtl.SetItemText(j,23,wbvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0080))
				{
					//������Ŀ       byte 0��ʾ��----7λ
					if(PackLen>offset)
				    {
					    unsigned char noticeval = *(PackBody+offset);
				        offset += 1;
					    int notice = noticeval;CString noticevalstr = _T("");noticevalstr.Format(L"%u",notice);
					    m_ListCtl.SetItemText(j,24,noticevalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0100))
				{
					//��ӯ��	  int��100  ��������
					if(PackLen>offset)
				    {
					    int sylval = *(int*)(PackBody+offset);
				        offset += 4;
					    CString sylvalstr = _T("");sylvalstr.Format(L"%d",sylval/100);
					    m_ListCtl.SetItemText(j,25,sylvalstr);
				    }
					//�о���	  int��100  ��������
					if(PackLen>offset)
				    {
					    int sjlval = *(int*)(PackBody+offset);
				        offset += 4;
					    CString sjlvalstr = _T("");sjlvalstr.Format(L"%d",sjlval/100);
					    m_ListCtl.SetItemText(j,26,sjlvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0200))
				{
					//��һ		  int
					if(PackLen>offset)
				    {
					    unsigned int sell = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString sellstr = _T("");sellstr.Format(L"%u",sell);
					    m_ListCtl.SetItemText(j,27,sellstr);
				    }
					//��һ		  int
					if(PackLen>offset)
				    {
					    int buy = *(int*)(PackBody+offset);
				        offset += 4;
					    CString buystr = _T("");buystr.Format(L"%u",buy);
					    m_ListCtl.SetItemText(j,28,buystr);
				    }
				}
				//ͳ���ֶ�----------10λ
				if(listattr!=0&&listattr&(0x0400))
				{
					//7���Ƿ�	  int��10000  ��������
					if(PackLen>offset)
				    {
					    int zindex = *(int*)(PackBody+offset);
				        offset += 4;
						double zindexval = zindex/10000;
					    CString zindexstr = _T("");zindexstr.Format(L"%lf",zindexval);
					    m_ListCtl.SetItemText(j,29,zindexstr);
				    }
					//7�ջ���	  int��10000
					if(PackLen>offset)
				    {
					    unsigned int sevenval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
						double sevenvalue = sevenval/10000;
					    CString sevenvalstr = _T("");sevenvalstr.Format(L"%lf",sevenvalue);
					    m_ListCtl.SetItemText(j,30,sevenvalstr);
				    }
					//30���Ƿ�	  int��10000  ��������
					if(PackLen>offset)
				    {
					    int z30index = *(int*)(PackBody+offset);
				        offset += 4;
						double z30indexval = z30index/10000;
					    CString z30indexstr = _T("");z30indexstr.Format(L"%lf",z30indexval);
					    m_ListCtl.SetItemText(j,31,z30indexstr);
				    }
					//30�ջ���	  int��10000
					if(PackLen>offset)
				    {
					    unsigned int thirtyval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
						double thirtyvalval = thirtyval/10000;
					    CString thirtyvalstr = _T("");thirtyvalstr.Format(L"%lf",thirtyvalval);
					    m_ListCtl.SetItemText(j,32,thirtyvalstr);
				    }
				}
				//level2ͳ���ֶ�--11λ
				if(listattr!=0&&listattr&(0x0800))
				{
					//����ddx	  short��1000  ��������
					if(PackLen>offset)
				    {
					    short dddx = *(short*)(PackBody+offset);
				        offset += 2;
						double dddxval = dddx/1000;
					    CString dddxstr = _T("");dddxstr.Format(L"%lf",dddxval);
					    m_ListCtl.SetItemText(j,33,dddxstr);
				    }
					//����ddy	  short��1000  ��������
					if(PackLen>offset)
				    {
					    short dddy = *(short*)(PackBody+offset);
				        offset += 2;
						double dddyval = dddy/1000;
					    CString dddystr = _T("");dddystr.Format(L"%lf",dddyval);
					    m_ListCtl.SetItemText(j,34,dddystr);
				    }
					//����ddz	  int��1000  ��������
					if(PackLen>offset)
				    {
					    int dddz = *(int*)(PackBody+offset);
				        offset += 4;
						double dddzval = dddz/1000;
					    CString dddzstr = _T("");dddzstr.Format(L"%lf",dddzval);
					    m_ListCtl.SetItemText(j,35,dddzstr);
				    }
					//60��ddx	  int��1000  ��������
					if(PackLen>offset)
				    {
					    int sixddxval = *(int*)(PackBody+offset);
				        offset += 4;
						double sixddxvalue = sixddxval/1000;
					    CString sixddxvalstr = _T("");sixddxvalstr.Format(L"%lf",sixddxvalue);
					    m_ListCtl.SetItemText(j,36,sixddxvalstr);
				    }
					//60��ddy	  int��1000  ��������
					if(PackLen>offset)
				    {
					    int sixddyval = *(int*)(PackBody+offset);
				        offset += 4;
						double sixddyvalue = sixddyval/1000;
					    CString sixddyvalstr = _T("");sixddyvalstr.Format(L"%lf",sixddyvalue);
					    m_ListCtl.SetItemText(j,37,sixddyvalstr);
				    }
					//10��ddx��ɫ������ char
					if(PackLen>offset)
				    {
					    char ddxredval = *(PackBody+offset);
				        offset += 1;
					    int ddxred = ddxredval;CString ddxredvalstr = _T("");ddxredvalstr.Format(L"%u",ddxred);
					    m_ListCtl.SetItemText(j,38,ddxredvalstr);
				    }
					//10��ddx������ɫ�� char
					if(PackLen>offset)
				    {
					    char ddxredcval = *(PackBody+offset);
				        offset += 1;
					    int ddxredc = ddxredcval;CString ddxredcvalstr = _T("");ddxredcvalstr.Format(L"%u",ddxredc);
					    m_ListCtl.SetItemText(j,39,ddxredcvalstr);
				    }
				}
				////level2ͳ���ֶ�--12λ, 20111020����
				if(listattr!=0&&listattr&(0x1000))
				{
					//�����ʽ�����     int         //�ʽ𾻶�=�ʽ�����-�ʽ�����
					if(PackLen>offset)
				    {
					    unsigned int incap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString incapstr = _T("");incapstr.Format(L"%u",incap);
					    m_ListCtl.SetItemText(j,40,incapstr);
				    }
					//�����ʽ�����     int 
					if(PackLen>offset)
				    {
					    unsigned int outcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString outcapstr = _T("");outcapstr.Format(L"%u",outcap);
					    m_ListCtl.SetItemText(j,41,outcapstr);
				    }
					//5���ʽ�����      int
					if(PackLen>offset)
				    {
					    unsigned int fiveincap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString fiveincapstr = _T("");fiveincapstr.Format(L"%u",fiveincap);
					    m_ListCtl.SetItemText(j,42,fiveincapstr);
				    }
					//5���ʽ�����      int 
					if(PackLen>offset)
				    {
					    unsigned int fiveoutcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString fiveoutcapstr = _T("");fiveoutcapstr.Format(L"%u",fiveoutcap);
					    m_ListCtl.SetItemText(j,43,fiveoutcapstr);
				    }
					//5���ʽ�ɽ���    int 
					if(PackLen>offset)
				    {
					    unsigned int fivecjcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString fivecjcapstr = _T("");fivecjcapstr.Format(L"%u",fivecjcap);
					    m_ListCtl.SetItemText(j,44,fivecjcapstr);
				    }
					//30���ʽ�����     int
					if(PackLen>offset)
				    {
					    unsigned int thirtyincap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString thirtyincapstr = _T("");thirtyincapstr.Format(L"%u",thirtyincap);
					    m_ListCtl.SetItemText(j,45,thirtyincapstr);
				    }
					//30���ʽ�����      int 
					if(PackLen>offset)
				    {
					    unsigned int thirtyoutcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString thirtyoutcapstr = _T("");thirtyoutcapstr.Format(L"%u",thirtyoutcap);
					    m_ListCtl.SetItemText(j,46,thirtyoutcapstr);
				    }
					//30���ʽ�ɽ���    int 
					if(PackLen>offset)
				    {
					    unsigned int thirtycjcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString thirtycjcapstr = _T("");thirtycjcapstr.Format(L"%u",thirtycjcap);
					    m_ListCtl.SetItemText(j,47,thirtycjcapstr);
				    }
				}
				//��Ʒ����������--13λ, 20120307����
				if(listattr!=0&&listattr&(0x2000))
				{
					//���ճֲ���       int
					if(PackLen>offset)
				    {
					    unsigned int zhold = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString zholdstr = _T("");zholdstr.Format(L"%u",zhold);
					    m_ListCtl.SetItemText(j,48,zholdstr);
				    }
					//���ս����       int 
					if(PackLen>offset)
				    {
					    unsigned int zsettle = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString zsettlestr = _T("");zsettlestr.Format(L"%u",zsettle);
					    m_ListCtl.SetItemText(j,49,zsettlestr);
				    }
					//�ֲ���           int
					if(PackLen>offset)
				    {
					    unsigned int hold = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString holdstr = _T("");holdstr.Format(L"%u",hold);
					    m_ListCtl.SetItemText(j,50,holdstr);
				    }
					//�����           int 
					if(PackLen>offset)
				    {
					    unsigned int settlecap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString settlecapstr = _T("");settlecapstr.Format(L"%u",settlecap);
					    m_ListCtl.SetItemText(j,51,settlecapstr);
				    }
				}
				//level2�������---14, 20120508
				if(listattr!=0&&listattr&(0x4000))
				{
					//��������       short  //�޷���
					if(PackLen>offset)
				    {
					    unsigned short jgval = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString jgvalstr = _T("");jgvalstr.Format(L"%u",jgval);
					    m_ListCtl.SetItemText(j,52,jgvalstr);
				    }
					//����������       short  //�޷���
					if(PackLen>offset)
				    {
					    unsigned short jgvalsell = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString jgvalsellstr = _T("");jgvalsellstr.Format(L"%u",jgvalsell);
					    m_ListCtl.SetItemText(j,53,jgvalsellstr);
				    }
					//�����Ի���       short  //�޷���
					if(PackLen>offset)
				    {
					    unsigned short eathold = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString eatholdstr = _T("");eatholdstr.Format(L"%u",eathold);
					    m_ListCtl.SetItemText(j,54,eatholdstr);
				    }
					//�����»���       short  //�޷���
					if(PackLen>offset)
				    {
					    unsigned short jgtcap = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString jgtcapstr = _T("");jgtcapstr.Format(L"%u",jgtcap);
					    m_ListCtl.SetItemText(j,55,jgtcapstr);
				    }
					//�����Ի���       int
					if(PackLen>offset)
				    {
					    unsigned int eatval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString eatvalstr = _T("");eatvalstr.Format(L"%u",eatval);
					    m_ListCtl.SetItemText(j,56,eatvalstr);
				    }
					//�����»���       int 
					if(PackLen>offset)
				    {
					    unsigned int sellvalue = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString sellvaluestr = _T("");sellvaluestr.Format(L"%u",sellvalue);
					    m_ListCtl.SetItemText(j,57,sellvaluestr);
				    }
				}
				//������ȯ���λ-------------15   20130717 
				if(listattr!=0&&listattr&(0x8000))
				{
					//�ù��Ƿ���������ȯ��� char   1�ǣ�0��
					if(PackLen>offset)
				    {
					    char rzrqtag = *(PackBody+offset);
				        offset += 1;
						int rzrqtagval = rzrqtag;
					    CString rzrqtagstr = _T("");rzrqtagstr.Format(L"%u",rzrqtag);
					    m_ListCtl.SetItemText(j,58,rzrqtagstr);
				    }
				}
			}
			}
			delete []tempbyte;
		}break;
	case 2956:
		{
			unsigned short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//���ȵĸ�λ2�ֽ�  short	//��2�ֽں����ĳ���short�ϲ���int����ʾ���ȣ���������λ��λ
			short highzj = *(short*)(PackBody+offset);
			offset += 2;
			CString highzjStr = _T("");highzjStr.Format(L"%d",highzj);
			m_ListCtl.SetItemText(0,1,highzjStr);
			//ѹ��ǰ����	    int		//δѹ��ʱ���ݳ��ȣ��ó���ָѹ��������δѹ��ǰ�����ݳ���
			int compresslen = *(int*)(PackBody+offset);
			offset += 4;
			CString CompStr = _T("");CompStr.Format(L"%d",compresslen);
			m_ListCtl.SetItemText(0,2,CompStr);
			//���crc	    int
			int staticCRC = *(int*)(PackBody+offset);
			offset+=4;
			CString codeCRC = _T("");codeCRC.Format(L"%d",codeCRC);
			m_ListCtl.SetItemText(0,3,codeCRC);
			//��Ŀ		    short	
			RecdNum = *(unsigned short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				CString GS = _T("");GS.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,GS);
			}
			m_ListCtl.SetItemText(0,4,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����		String	�������г�����
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackBody+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(j,5,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLen>offset)
				{
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackBody+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(j,6,namestr.c_str());
					delete []name;
				}
				//ƴ��		String	//����Ƕ��������á�`���ַ��ָ�
				if(PackLen>offset)
				{
					short pinglen = *(short*)(PackBody+offset);
					offset += 2;
					char *pingyin = new char[pinglen+1];
					memset(pingyin,0,pinglen+1);
					memcpy(pingyin,PackBody+offset,pinglen);
					offset += pinglen;
					wstring pingyinstr = UTF8_2_UTF16(pingyin);
					m_ListCtl.SetItemText(j,7,pingyinstr.c_str());
					delete []pingyin;
				}
			}
		}break;
	case 2957:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����ʱ�� 	    int
			int newtime = *(int*)(PackBody+offset);
			CString newtimeStr = _T("");newtimeStr.Format(L"%d",newtime);
			m_ListCtl.SetItemText(0,1,newtimeStr);
		}break;
	case 2958:
		{
			unsigned short RecdNum=0;
			CString RecdNumStr=_T("");
			//��Ŀ		    short	
			RecdNum = *(unsigned short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				CString GS = _T("");GS.Format(L"%d",i+1);
				m_ListCtl.InsertItem(i,_T(""));
				m_ListCtl.SetItemText(i,0,GS);
			}
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����	    int        //YYYYMMDD
				if(PackLen>offset)
				{
					unsigned int datetime = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString DateStr = _T("");DateStr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,2,DateStr);
				}
				//����       int*10000  //����������
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,3,csdataStr);
				}
				//����       int*10000  //����������
				if(PackLen>offset)
				{
					int adddata = *(int*)(PackBody+offset);
					offset += 4;
					CString adddataStr = _T("");adddataStr.Format(L"%d",adddata);
					m_ListCtl.SetItemText(j,4,adddataStr);
				}
			}
		}break;
	case 2959:
		{
			unsigned short RecdNum=0;
			CString RecdNumStr=_T("");

			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����             short              //20120216�Ķ�
			short tolnum = *(short*)(PackBody+offset);
			offset += 2;
			CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
			m_ListCtl.SetItemText(0,1,tolnumStr);
			//λ��             short			
			short position = *(short*)(PackBody+offset);
			offset += 2;
			CString posStr = _T("");posStr.Format(L"%d",position);
			m_ListCtl.SetItemText(0,2,posStr);
			//��Ŀ		    short	
			RecdNum = *(unsigned short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				CString GS = _T("");GS.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,GS);
			}
			m_ListCtl.SetItemText(0,3,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����		String	�������г�����
				if(PackLen>offset)
				{
					short codelen = *(short*)(PackBody+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackBody+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(j,4,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLen>offset)
				{
					short namelen = *(short*)(PackBody+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackBody+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(j,5,namestr.c_str());
					delete []name;
				}
				//��ע��     int	
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,6,csdataStr);
				}
				//�Ƿ�       int*10000  //����������
				if(PackLen>offset)
				{
					int adddata = *(int*)(PackBody+offset);
					offset += 4;
					CString adddataStr = _T("");adddataStr.Format(L"%d",adddata);
					m_ListCtl.SetItemText(j,7,adddataStr);
				}
			}
		}break;
	case 2960:
		{
			unsigned short RecdNum=0;
			CString RecdNumStr=_T("");

			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�۸�С��λ��	    byte
			unsigned char pricepos = *(PackBody+offset);
			offset += 1;
			int priceposval = pricepos;
			CString priceposvalStr = _T("");priceposvalStr.Format(L"%d",priceposval);
			m_ListCtl.SetItemText(0,1,priceposvalStr);
			//��ע������       short		
			short position = *(short*)(PackBody+offset);
			offset += 2;
			CString posStr = _T("");posStr.Format(L"%d",position);
			m_ListCtl.SetItemText(0,2,posStr);
			//��Ŀ		    short	
			RecdNum = *(unsigned short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				CString GS = _T("");GS.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,GS);
			}
			m_ListCtl.SetItemText(0,3,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����	    int        //YYYYMMDD
				if(PackLen>offset)
				{
					unsigned int datetime = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString dateStr = _T("");dateStr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,4,dateStr);
				}
				//��ע��     int	
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,5,csdataStr);
				}
				//�۸�       int        // 
				if(PackLen>offset)
				{
					unsigned int pricedata = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString pricedataStr = _T("");pricedataStr.Format(L"%d",pricedata);
					m_ListCtl.SetItemText(j,6,pricedataStr);
				}
			}
		}break;
    case 2962:
	    {
			short RecdNum=0;
			CString RecdNumStr=_T("");
	
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//������         String
			if(PackLen>offset)
			{
			    short bkcodelen = *(short*)(PackBody+offset);
			    offset += 2;
			    char *bkcode = new char[bkcodelen+1];
			    memset(bkcode,0,bkcodelen+1);
			    memcpy(bkcode,PackBody+offset,bkcodelen);
			    offset += bkcodelen;
			    wstring bkcodestr = UTF8_2_UTF16(bkcode);
				m_ListCtl.SetItemText(0,1,bkcodestr.c_str());
				delete []bkcode;
			}
			//�������         String		
			if(PackLen>offset)
			{
			    short bknamelen = *(short*)(PackBody+offset);
			    offset += 2;
			    char *bkname = new char[bknamelen+1];
			    memset(bkname,0,bknamelen+1);
			    memcpy(bkname,PackBody+offset,bknamelen);
			    offset += bknamelen;
			    wstring bknamestr = UTF8_2_UTF16(bkname);
				m_ListCtl.SetItemText(0,2,bknamestr.c_str());
				delete []bkname;
			}
			//�۸�С��λ��	    byte
			//�۸�С��λ��	    byte
			if(PackLen>offset)
			{
			    unsigned char pricepos = *(PackBody+offset);
			    offset += 1;
			    int priceposval = pricepos;
			    CString priceposvalStr = _T("");priceposvalStr.Format(L"%d",priceposval);
			    m_ListCtl.SetItemText(0,3,priceposvalStr);
			}
			//��Ŀ		    short	
			RecdNum = *(unsigned short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				CString GS = _T("");GS.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
				m_ListCtl.SetItemText(i+1,0,GS);
			}
			m_ListCtl.SetItemText(0,4,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//����	    int        //YYYYMMDD
				if(PackLen>offset)
				{
					unsigned int datetime = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString dateStr = _T("");dateStr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,5,dateStr);
				}
				//��ע��     int	
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,6,csdataStr);
				}
				//�۸�       int        // 
				if(PackLen>offset)
				{
					unsigned int pricedata = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString pricedataStr = _T("");pricedataStr.Format(L"%d",pricedata);
					m_ListCtl.SetItemText(j,7,pricedataStr);
				}
			}
	    }break;
	case 2963:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
	
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//short	��
			if(PackLen>offset)
			{
			    short year = *(short*)(PackBody+offset);
			    offset += 2;
			    CString yearStr=_T("");yearStr.Format(L"%d",year);
			    m_ListCtl.SetItemText(0,1,yearStr);
			}
			// char     ��
			if(PackLen>offset)
			{
			    char month = *(PackBody+offset);
			    offset += 1;
			    int monthval = month;CString monthStr=_T("");monthStr.Format(L"%d",monthval);
			    m_ListCtl.SetItemText(0,2,monthStr);
			}
			// char     ��
			if(PackLen>offset)
			{
			    char day = *(PackBody+offset);
			    offset += 1;
			    int dayval = day;CString dayStr=_T("");dayStr.Format(L"%d",dayval);
			    m_ListCtl.SetItemText(0,3,dayStr);
			}
			//char     ʱ
			if(PackLen>offset)
			{
			    char hour = *(PackBody+offset);
			    offset += 1;
			    int hourval = hour;CString hourStr=_T("");hourStr.Format(L"%d",hourval);
			    m_ListCtl.SetItemText(0,4,hourStr);
			}
			//char     ��
			if(PackLen>offset)
			{
			    char minute = *(PackBody+offset);
			    offset += 1;
			    int minuteval = minute;CString minuteStr=_T("");minuteStr.Format(L"%d",minuteval);
			    m_ListCtl.SetItemText(0,5,minuteStr);
			}
			//char     ��	
			if(PackLen>offset)
			{
			    char second = *(PackBody+offset);
			    offset += 1;
			    int secondval = second;CString secondStr=_T("");secondStr.Format(L"%d",secondval);
			    m_ListCtl.SetItemText(0,6,secondStr);
			}
		}break;
	case 2964:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");

			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//short    ����Ŀ
			short tolnum = *(short*)(PackBody+offset);
			offset += 2;
			CString tolnumStr=_T("");tolnumStr.Format(L"%d",tolnum);
			m_ListCtl.SetItemText(0,1,tolnumStr);
			if(PackLen>offset)
			{
				////short    ���м�¼��
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					// String ��Ʊ����
					if(PackLen>offset)
				    {
						short codelen = *(short*)(PackBody+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackBody+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,3,codestr.c_str());
						delete []code;
					}
					//short  ����������
					if(PackLen>offset)
				    {
						short ybnum = *(short*)(PackBody+offset);
						offset += 2;
						CString ybnumstr = _T("");ybnumstr.Format(L"%d",ybnum);
						m_ListCtl.SetItemText(j,4,ybnumstr);
					}
					if(PackLen>offset)
				    {
					    //int    ���̼۸񣬿����������̼۸�Ҳ�����ǽ������к�ļ۸�
					    unsigned int spvalue = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString spvaluestr = _T("");spvaluestr.Format(L"%u",spvalue);
						m_ListCtl.SetItemText(j,5,spvaluestr);
				    }
					if(PackLen>offset)
				    {
					    //int    �����ɼ�
					    unsigned int ybpricevalue = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString ybpricevaluestr = _T("");ybpricevaluestr.Format(L"%u",ybpricevalue);
						m_ListCtl.SetItemText(j,6,ybpricevaluestr);
				    }
					if(PackLen>offset)
				    {
					    //int    �ɽ�����λ����Ԫ���ͻ��˿�����ʾ��2λС������Ԫ
					    unsigned int cjvalue = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
						m_ListCtl.SetItemText(j,6,cjvaluestr);
				    }
					if(PackLen>offset)
				    {
					    //int    ƽ���ɼ�
					    unsigned int avgprice = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString avgpricestr = _T("");avgpricestr.Format(L"%u",avgprice);
						m_ListCtl.SetItemText(j,7,avgpricestr);
				    }
					if(PackLen>offset)
				    {
					    //int    ����ֵ����λ����Ԫ���ͻ��˿�����ʾ��2λС��������Ԫ
					    unsigned int tolmoney = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString tolmoneystr = _T("");tolmoneystr.Format(L"%u",tolmoney);
						m_ListCtl.SetItemText(j,8,tolmoneystr);
				    }
					if(PackLen>offset)
				    {
					    //int    ָ����ǰ����������ֵռ��֤��ָȫ����������ֵ�ٷֱȣ�2λС��
					    int index = *(int*)(PackBody+offset);
						offset += 4;
						CString indexstr = _T("");indexstr.Format(L"%u",index);
						m_ListCtl.SetItemText(j,9,indexstr);
				    }
					if(PackLen>offset)
				    {
					    //int    ��̬��ӯ��
					    int staticdata = *(int*)(PackBody+offset);
						offset += 4;
						CString staticdatastr = _T("");staticdatastr.Format(L"%u",staticdata);
						m_ListCtl.SetItemText(j,10,staticdatastr);
				    }
					if(PackLen>offset)
				    {
					    //short  ָ������,ǰnλΪָ��������ֵ�����һλ��ָ��������Ϣ��1Ϊ�ص�ָ��,2Ϊȫòָ��
					    short indextype = *(short*)(PackBody+offset);
						offset += 2;
						CString indextypestr = _T("");indextypestr.Format(L"%u",indextype);
						m_ListCtl.SetItemText(j,11,indextypestr);
				    }
				}
			}
		}break;
	case 2965:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				////short    ���м�¼��
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+1);
					m_ListCtl.InsertItem(i,_T(""));
					m_ListCtl.SetItemText(i,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					// char             ���Ǻ��µ�������ֵ,��Χ-100~100
					if(PackLen>offset)
				    {
						char zdval = *(PackBody+offset);
						offset += 1;
						int zdvalue = zdval;CString zdstr = _T("");zdstr.Format(L"%d",zdval);
						m_ListCtl.SetItemText(j,2,zdstr);
					}
				}
			}
		}break;
	case 2966:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLen>offset)
			{
				////short    ���м�¼��
				RecdNum = *(short*)(PackBody+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+1);
					m_ListCtl.InsertItem(i,_T(""));
					m_ListCtl.SetItemText(i,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					// ����	    int        //YYYYMMDD
					if(PackLen>offset)
				    {
						unsigned int datetime = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString datestr = _T("");datestr.Format(L"%u",datetime);
						m_ListCtl.SetItemText(j,2,datestr);
					}
					// ÿ����     int*10000  //	
					if(PackLen>offset)
				    {
						unsigned int pergu = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString pergustr = _T("");pergustr.Format(L"%u",pergu);
						m_ListCtl.SetItemText(j,3,pergustr);
					}
					// ÿ����     int*10000  //
					if(PackLen>offset)
				    {
						unsigned int pergup = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString pergupstr = _T("");pergupstr.Format(L"%u",pergup);
						m_ListCtl.SetItemText(j,4,pergupstr);
					}
					//��ɼ�     int*10000  //
					if(PackLen>offset)
				    {
						unsigned int pgprice = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString pgpricestr = _T("");pgpricestr.Format(L"%u",pgprice);
						m_ListCtl.SetItemText(j,5,pgpricestr);
					}
					//ÿ�ɺ���   int*10000  //
					if(PackLen>offset)
				    {
						int permoney = *(int*)(PackBody+offset);
						offset += 4;
						CString permoneystr = _T("");permoneystr.Format(L"%u",permoney);
						m_ListCtl.SetItemText(j,6,permoneystr);
					}
				}
			}
		}break;
	case 2967:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//���:                    char
			if(PackLen>offset)
			{
				char result = *(PackBody+offset);
				int nresult = result;
				CString resultstr = _T("");resultstr.Format(L"%d",nresult);
				m_ListCtl.SetItemText(0,1,resultstr);
			}
		}break;
	
	}
}


// �������
void CYunTestDlg::DealPack(char * & PackData, unsigned int PackLength, unsigned int PackType)
{
		//�����������ÿһ�������н���������ĵ��Ƚϻ�����һ�����Ľ���Ƚ�����������ȽϷ���
	m_ListCtl.DeleteAllItems();
	int nHeadNum = m_ListCtl.GetHeaderCtrl()->GetItemCount();
	for(int i=nHeadNum-1; i>=0; i--)
	{
        m_ListCtl.DeleteColumn (i);
	}
	for(int n=1;n<=500;n++)
    {
        m_ListCtl.InsertItem(n,_T(""));
		m_ListCtl.SetItemText(n,0,(LPCTSTR)(n+1));
    }
	unsigned int offset=0;
	switch(PackType)
	{
	case 2961:
		{
			unsigned short RecdNum=0;
			int k = 0;
			short tjnum = 0; //;����Ŀ
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��Ŀ		    short	
			RecdNum = *(unsigned short*)(PackData+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				wstring YYSstr;
				//��Ӫ����    String
				if(PackLength>offset)
				{
					unsigned short YYSlen = *(unsigned short*)(PackData+offset);
					offset += 2;
					char *YYS = new char[YYSlen+1];
					memset(YYS,0,YYSlen+1);
					memcpy(YYS,PackData+offset,YYSlen);
					offset += YYSlen;
					YYSstr = UTF8_2_UTF16(YYS);
					
					delete []YYS;
				}
				//;����Ŀ    short   //���нṹ����Ŀ
				tjnum = *(short*)(PackData+offset);
				offset += 2;
				CString tjnumstr = _T("");tjnumstr.Format(L"%d",tjnum);
				for(int m=0;m<tjnum;m++)
				{
					CString sonnum = _T("");sonnum.Format(L"%d",m+k+2);
				    m_ListCtl.InsertItem(m+k+1,_T(""));
				    m_ListCtl.SetItemText(m+k+1,0,sonnum);
				}
				m_ListCtl.SetItemText(j+k,2,YYSstr.c_str());
				m_ListCtl.SetItemText(j+k,3,tjnumstr);
				for(int n=0; n<tjnum; n++)
				{
					int typeval = 0;
					//��ʽ      char    //1��ʾwap��ʽ��2��ʾ���ŷ�ʽ
				    if(PackLength>offset)
				    {
					    char type = *(PackData+offset);
					    offset += 1;typeval = type;
					    CString typevalStr = _T("");typevalStr.Format(L"%d",typeval);
					    m_ListCtl.SetItemText(n+k,4,typevalStr);
				    }
					if(typeval == 1)
					{
					//wap/������ʾ��   String
				    if(PackLength>offset)
				    {
					    unsigned short msgnoticelen = *(unsigned short*)(PackData+offset);
					    offset += 2;
					    char *msgnotice = new char[msgnoticelen+1];
					    memset(msgnotice,0,msgnoticelen+1);
					    memcpy(msgnotice,PackData+offset,msgnoticelen);
					    offset += msgnoticelen;
					    wstring msgnoticestr = UTF8_2_UTF16(msgnotice);
					    m_ListCtl.SetItemText(n+k,5,msgnoticestr.c_str());
					    delete []msgnotice;
				    }
					//wap/������       String
					if(PackLength>offset)
				    {
					    unsigned short msgcodelen = *(unsigned short*)(PackData+offset);
					    offset += 2;
					    char *msgcode = new char[msgcodelen+1];
					    memset(msgcode,0,msgcodelen+1);
					    memcpy(msgcode,PackData+offset,msgcodelen);
					    offset += msgcodelen;
					    wstring msgcodestr = UTF8_2_UTF16(msgcode);
					    m_ListCtl.SetItemText(n+k,6,msgcodestr.c_str());
					    delete []msgcode;
				    }
					}
					if(typeval == 2)
					{
					//ָ����           String
					if(PackLength>offset)
				    {
					    unsigned short codelen = *(unsigned short*)(PackData+offset);
					    offset += 2;
					    char *code = new char[codelen+1];
					    memset(code,0,codelen+1);
					    memcpy(code,PackData+offset,codelen);
					    offset += codelen;
					    wstring codestr = UTF8_2_UTF16(code);
					    m_ListCtl.SetItemText(n+k,7,codestr.c_str());
					    delete []code;
				    }
					//��ʾ��           String
					if(PackLength>offset)
				    {
					    unsigned short noticelen = *(unsigned short*)(PackData+offset);
					    offset += 2;
					    char *notice = new char[noticelen+1];
					    memset(notice,0,noticelen+1);
					    memcpy(notice,PackData+offset,noticelen);
					    offset += noticelen;
					    wstring noticestr = UTF8_2_UTF16(notice);
					    m_ListCtl.SetItemText(n+k,8,noticestr.c_str());
					    delete []notice;
				    }
					}
				}
				k += tjnum;
			}
		}break;
	case 2968:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLength>offset)
			{
				////short    ���м�¼��
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+1);
					m_ListCtl.InsertItem(i,_T(""));
					m_ListCtl.SetItemText(i,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					// ��Ʊ����      String
					if(PackLength>offset)
				    {
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,2,codestr.c_str());
						delete []code;
					}
					//��Ʊ����	   String
					if(PackLength>offset)
				    {
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(j,3,namestr.c_str());
						delete []name;
					}
					// ��Ʊ����      char
					if(PackLength>offset)
				    {
						char codetype = *(PackData+offset);
						offset += 1;
						int codeval = codetype;
						CString codetypestr = _T("");codetypestr.Format(L"%d",codeval);
						m_ListCtl.SetItemText(j,4,codetypestr);
					}
					//С��λ��      char
					if(PackLength>offset)
				    {
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointval = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointval);
						m_ListCtl.SetItemText(j,5,pointstr);
					}
					//���¼۸�      int
					if(PackLength>offset)
				    {
						int newprice = *(int*)(PackData+offset);
						offset += 4;
						CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
						m_ListCtl.SetItemText(j,6,newpricestr);
					}
					//�Ƿ�          int*10000
					if(PackLength>offset)
				    {
						int zfval = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalstr = _T("");zfvalstr.Format(L"%d",zfval);
						m_ListCtl.SetItemText(j,7,zfvalstr);
					}
				}
			}
		}break;
	case 2969:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			if(PackLength>offset)
			{
				////short    ���м�¼��
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+1);
					m_ListCtl.InsertItem(i,_T(""));
					m_ListCtl.SetItemText(i,0,GS);
				}
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					// ��Ʊ����      String
					if(PackLength>offset)
				    {
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,2,codestr.c_str());
						delete []code;
					}
					//��Ʊ����	   String
					if(PackLength>offset)
				    {
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(j,3,namestr.c_str());
						delete []name;
					}
					//С��λ��      char
					if(PackLength>offset)
				    {
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointval = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointval);
						m_ListCtl.SetItemText(j,4,pointstr);
					}
					//���¼۸�      int
					if(PackLength>offset)
				    {
						int newprice = *(int*)(PackData+offset);
						offset += 4;
						CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
						m_ListCtl.SetItemText(j,5,newpricestr);
					}
					//�Ƿ�          int*10000
					if(PackLength>offset)
				    {
						int zfval = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalstr = _T("");zfvalstr.Format(L"%d",zfval);
						m_ListCtl.SetItemText(j,6,zfvalstr);
					}
					//���id        short	//�ð��id,���������id�����Ӧ�ɷֹ�
					if(PackLength>offset)
				    {
						short bkid = *(short*)(PackData+offset);
						offset += 4;
						CString bkidstr = _T("");bkidstr.Format(L"%d",bkid);
						m_ListCtl.SetItemText(j,7,bkidstr);
					}
				}
			}
		}break;
	case 2970:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֱֲ��(tag)	char	    //1�����ֲ֣�0�������ֲ�
			char holdtag = *(PackData+offset);
			offset += 1;
			int holdval = holdtag;
			CString HoldStr=_T("");HoldStr.Format(L"%d",holdval);
			m_ListCtl.SetItemText(0,1,HoldStr);
			//����λ��	 int	     ��һ����ʱ���ݵ�λ��
			int dateval = *(int*)(PackData+offset);
			offset += 4;
			CString datevalStr = _T("");datevalStr.Format(L"%d",dateval);
			m_ListCtl.SetItemText(0,2,datevalStr);
			if(PackLength>offset)
			{
				//��¼��         short
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
					m_ListCtl.SetItemText(i+1,0,GS);
				}
				m_ListCtl.SetItemText(0,3,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLength>offset)
				    {
					    //ʱ��	 int	     ��ʽ��hhmmss
					    unsigned int datevalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString datevaluestr = _T("");datevaluestr.Format(L"%u",datevalue);
						m_ListCtl.SetItemText(j,4,datevaluestr);
				    }
					if(PackLength>offset)
				    {
					    //�۸�	 int	     �۸����1λ��ʾ�ñʽ����Ƿ�������
					    unsigned int pricevalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString pricevaluestr = _T("");pricevaluestr.Format(L"%u",pricevalue);
						m_ListCtl.SetItemText(j,5,pricevaluestr);
				    }
					if(PackLength>offset)
				    {
					    //�ɽ���	 int	     �ɽ�����ʾ�ǵ�ǰ�ɽ�����������
					    unsigned int cjvalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
						m_ListCtl.SetItemText(j,6,cjvaluestr);
				    }
					if(holdval != 0)
					{
					    if(PackLength>offset)
				        {
					        //�ֲ���	 int	     �ܵĳֲ�����tagΪ0û�и��ֶ�
					        unsigned int holdvalue = *(unsigned int*)(PackData+offset);
						    offset += 4;
						    CString holdvaluestr = _T("");holdvaluestr.Format(L"%u",holdvalue);
						    m_ListCtl.SetItemText(j,7,holdvaluestr);
				        }
					}
				}
			}
		}break;
	case 2971:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����λ��       int
			int datapos = *(int*)(PackData+offset);
			offset += 4;
			CString dataposstr = _T("");dataposstr.Format(L"%d",datapos);
			m_ListCtl.SetItemText(0,1,dataposstr);
			//��¼��         	short       //6��10��������
			RecdNum = *(short*)(PackData+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.SetItemText(0,2,RecdNumStr);
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//������         	int
				if(PackLength>offset)
				{
					int datetime = *(int*)(PackData+offset);
					offset += 4;
					CString datetimestr = _T("");datetimestr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,3,datetimestr);
				}
				//������         	int
				if(PackLength>offset)
				{
					int buysell = *(int*)(PackData+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					m_ListCtl.SetItemText(j,4,buysellstr);
				}
				//������         	int
				if(PackLength>offset)
				{
					int buysellval = *(int*)(PackData+offset);
					offset += 4;
					CString buysellvalstr = _T("");buysellvalstr.Format(L"%d",buysellval);
					m_ListCtl.SetItemText(j,5,buysellvalstr);
				}
			}
		}break;
	case 2972:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//���          char   //0:������������صĳɹ����
                                //1:������������ص�ʧ�ܱ��
                                //2:�˻����ص�����
			char tag = '0';
			int tagval = 0;
			if(PackLength>offset)
			{
				tag = *(PackData+offset);
				offset += 1;
				tagval = tag;
				CString tagstr = _T("");tagstr.Format(L"%d",tagval);
				m_ListCtl.SetItemText(0,1,tagstr);
			}
			if(tagval == 2)
			{
				// ����          char*N	//������ֻ���ڱ��Ϊ2��ʱ�����,��ʾ�˻����صĽ��
				if(PackLength>offset)
				{
					//������ͨ���и�sub_head�ṹ,�����Ҫ��������ip�ֶ�,���ֶξ�λ��sub_head�ֶκ��4���ֽ�,����������sub_head�ĺ���

				}
			}
		}break;
	case 2973:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
	
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�����ֶ�      short	//�����˵��
			short attrval = 0;
			if(PackLength>offset)
			{
			    attrval = *(short*)(PackData+offset);
			    offset += 2;
			    CString attrstr = _T("");attrstr.Format(L"%d",attrval);
			    m_ListCtl.SetItemText(0,1,attrstr);
			}
			//��0λ, ����������Ϣ, ���и����ֶ�������ӿ�2933˵��
			if(attrval&0x0001)
			{
				if(PackLength>offset)
				{
				    //���tag	char	//���
				    char tag = *(PackData+offset);
				    offset += 1; 
					int tagval = tag;
				    CString tagstr = _T("");tagstr.Format(L"%d",tagval);
				    m_ListCtl.SetItemText(0,2,tagstr);
				}
				//udd		int	//�۸���ֵ��С����λ��2200�е�С��λ�����壬����۸�λ���͸�ֵ��ͬ
				if(PackLength>offset)
				{
					int uddval = *(int*)(PackData+offset);
					offset += 4;
					CString uddstr = _T("");uddstr.Format(L"%d",uddval);
					m_ListCtl.SetItemText(0,3,uddstr);
				}
				//upp		int	//�۸���ֵ
				if(PackLength>offset)
				{
					int uppval = *(int*)(PackData+offset);
					offset += 4;
					CString uppstr = _T("");uppstr.Format(L"%d",uppval);
					m_ListCtl.SetItemText(0,4,uppstr);
				}
				//ls		int	//�۸���ֵ
				if(PackLength>offset)
				{
					int lsval = *(int*)(PackData+offset);
					offset += 4;
					CString lsstr = _T("");lsstr.Format(L"%d",lsval);
					m_ListCtl.SetItemText(0,5,lsstr);
				}
				// hs		int	//�۸���ֵ	   
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
					offset += 4;
					CString hsstr = _T("");hsstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(0,6,hsstr);
				}
				// d3		short	//*100
				if(PackLength>offset)
				{
					unsigned short d3val = *(unsigned short*)(PackData+offset);
					offset += 2;
					CString d3str = _T("");d3str.Format(L"%u",d3val);
					m_ListCtl.SetItemText(0,7,d3str);
				}
			}
			////��1λ,���������Ϣ
			if(attrval&0x0002)
			{
				//�����Ŀ     short
				if(PackLength>offset)
				{
					RecdNum = *(short*)(PackData+offset);
			        offset += 2;
			        RecdNumStr.Format(L"%d",RecdNum);
			        m_ListCtl.SetItemText(0,8,RecdNumStr);
			        for(int i=0; i<RecdNum; i++)
			        {
				        m_ListCtl.InsertItem(i+1,_T(""));
				        CString xh = _T("");xh.Format(L"%d",i+2);
				        m_ListCtl.SetItemText(i+1,0,xh);
			        }
					for(int j=0; j<RecdNum; j++)
			        {
				      //������     String
				        if(PackLength>offset)
				        {
							short codelen = *(short*)(PackData+offset);
							offset += 2;
							char *code = new char[codelen+1];
							memset(code,0,codelen+1);
							memcpy(code,PackData+offset,codelen);
							offset += codelen;
							wstring codestr = UTF8_2_UTF16(code);
							m_ListCtl.SetItemText(j,9,codestr.c_str());
							delete []code;
						}
						//�������     String
				        if(PackLength>offset)
				        {
							short namelen = *(short*)(PackData+offset);
							offset += 2;
							char *name = new char[namelen+1];
							memset(name,0,namelen+1);
							memcpy(name,PackData+offset,namelen);
							offset += namelen;
							wstring namestr = UTF8_2_UTF16(name);
							m_ListCtl.SetItemText(j,10,namestr.c_str());
							delete []name;
						}
						//С����λ��   char
						if(PackLength>offset)
				        {
							char pointpos = *(PackData+offset);
							offset += 1;
							int pointval = pointpos;
							CString pointstr = _T("");pointstr.Format(L"%d",pointval);
							m_ListCtl.SetItemText(j,11,pointstr);
						}
						//���¼۸�     int 
						if(PackLength>offset)
				        {
							int newprice = *(int*)(PackData+offset);
							offset += 4;
							CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
							m_ListCtl.SetItemText(j,12,newpricestr);
						}
						//����Ƿ�     int*10000 
						if(PackLength>offset)
				        {
							int zfbk = *(int*)(PackData+offset);
							offset += 4;
							CString zfbkstr = _T("");zfbkstr.Format(L"%d",zfbk);
							m_ListCtl.SetItemText(j,13,zfbkstr);
						}
						//���id       short  //�������ø�id������ɷֹ�
						if(PackLength>offset)
				        {
							short bkid = *(short*)(PackData+offset);
							offset += 2;
							CString bkidstr = _T("");bkidstr.Format(L"%d",bkid);
							m_ListCtl.SetItemText(j,14,bkidstr);
						}
						//�ʽ�����     int   //��λ:��Ԫ
						if(PackLength>offset)
				        {
							int input = *(int*)(PackData+offset);
							offset += 4;
							CString inputstr = _T("");inputstr.Format(L"%d",input);
							m_ListCtl.SetItemText(j,15,inputstr);
						}
					}
				}
			}
			////��2λ,�����ʽ�������Ϣ
			if(attrval&0x0004)
			{
				//���մ��ʽ�����     int  //��λ:��Ԫ,      ��Ϊ��ֵ��20150331
				if(PackLength>offset)
				{
					int largeinput = *(int*)(PackData+offset);
					offset += 4;
					CString largeinputstr = _T("");largeinputstr.Format(L"%d",largeinput);
					m_ListCtl.SetItemText(0,16,largeinputstr);
				}
				//5�մ��ʽ�����      int  //��λ:��Ԫ,      ��Ϊ��ֵ��20150331
				if(PackLength>offset)
				{
					int fivelargeinput = *(int*)(PackData+offset);
					offset += 4;
					CString fivelargeinputstr = _T("");fivelargeinputstr.Format(L"%d",fivelargeinput);
					m_ListCtl.SetItemText(0,17,fivelargeinputstr);
				}
				//30�մ��ʽ�����     int  //��λ:��Ԫ,      ��Ϊ��ֵ��20150331
				if(PackLength>offset)
				{
					int thirtylargeinput = *(int*)(PackData+offset);
					offset += 4;
					CString thirtylargeinputstr = _T("");thirtylargeinputstr.Format(L"%d",thirtylargeinput);
					m_ListCtl.SetItemText(0,18,thirtylargeinputstr);
				}
				//����ddx              short*1000 
				if(PackLength>offset)
				{
					short todayddx = *(short*)(PackData+offset);
					offset += 2;
					CString todayddxstr = _T("");todayddxstr.Format(L"%d",todayddx);
					m_ListCtl.SetItemText(0,19,todayddxstr);
				}
				//60��ddx              short*1000
				if(PackLength>offset)
				{
					short sixtyddx = *(short*)(PackData+offset);
					offset += 2;
					CString sixtyddxstr = _T("");sixtyddxstr.Format(L"%d",sixtyddx);
					m_ListCtl.SetItemText(0,20,sixtyddxstr);
				}
				//���10�������������� char
				if(PackLength>offset)
				{
					char inputnum = *(PackData+offset);
					offset += 1;
					int inputnumval = inputnum;
					CString inputnumstr = _T("");inputnumstr.Format(L"%d",inputnumval);
					m_ListCtl.SetItemText(0,21,inputnumstr);
				}
				//���10�����������   char
				if(PackLength>offset)
				{
					char inputday = *(PackData+offset);
					offset += 1;
					int inputdayval = inputday;
					CString inputdaystr = _T("");inputdaystr.Format(L"%d",inputdayval);
					m_ListCtl.SetItemText(0,22,inputdaystr);
				}
			}
			////��3λ,�׶�ͳ����Ϣ
			if(attrval&0x0008)
			{
				//7���Ƿ�	  int��10000  ��������
				if(PackLength>offset)
				{
					int servezf = *(int*)(PackData+offset);
					offset += 4;
					CString servenstr = _T("");servenstr.Format(L"%d",servezf);
					m_ListCtl.SetItemText(0,23,servenstr);
				}
				//7�ջ���	  int��10000
				if(PackLength>offset)
				{
					int servehs = *(int*)(PackData+offset);
					offset += 4;
					CString servehsstr = _T("");servehsstr.Format(L"%d",servehs);
					m_ListCtl.SetItemText(0,24,servehsstr);
				}
				//30���Ƿ�	  int��10000  ��������
				if(PackLength>offset)
				{
					int thirtyzf = *(int*)(PackData+offset);
					offset += 4;
					CString thirtyzfstr = _T("");thirtyzfstr.Format(L"%d",thirtyzf);
					m_ListCtl.SetItemText(0,25,thirtyzfstr);
				}
				//30�ջ���	  int��10000
				if(PackLength>offset)
				{
					int thirtyhs = *(int*)(PackData+offset);
					offset += 4;
					CString thirtyhsstr = _T("");thirtyhsstr.Format(L"%d",thirtyhs);
					m_ListCtl.SetItemText(0,26,thirtyhsstr);
				}
			}
		}break;
	case 2974:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");

			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//int	��������	YYYYMMDD
			if(PackLength>offset)
			{
				unsigned int fbdate = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString fbdatestr = _T("");fbdatestr.Format(L"%u",fbdate);
				m_ListCtl.SetItemText(0,1,fbdatestr);
			}
			//int	������		YYYYMMDD
			if(PackLength>offset)
			{
				unsigned int reportdate = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString reportdatestr = _T("");reportdatestr.Format(L"%u",reportdate);
				m_ListCtl.SetItemText(0,2,reportdatestr);
			}
			//int	��������	YYYYMMDD
			if(PackLength>offset)
			{
				unsigned int ssdate = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ssdatestr = _T("");ssdatestr.Format(L"%u",ssdate);
				m_ListCtl.SetItemText(0,3,ssdatestr);
			}
			//int	ÿ������*10000
			if(PackLength>offset)
			{
				int mgsy = *(int*)(PackData+offset);
				offset += 4;
				CString mgsystr = _T("");mgsystr.Format(L"%d",mgsy);
				m_ListCtl.SetItemText(0,4,mgsystr);
			}
			//int	ÿ�ɾ��ʲ�*10000
			if(PackLength>offset)
			{
				int mgjzc = *(int*)(PackData+offset);
				offset += 4;
				CString mgjzcstr = _T("");mgjzcstr.Format(L"%d",mgjzc);
				m_ListCtl.SetItemText(0,5,mgjzcstr);
			}
			//int	���ʲ�������*10000	
			if(PackLength>offset)
			{
				int jzcsy = *(int*)(PackData+offset);
				offset += 4;
				CString jzcsystr = _T("");jzcsystr.Format(L"%d",jzcsy);
				m_ListCtl.SetItemText(0,6,jzcsystr);
			}
			//int	ÿ�ɾ�Ӫ�ֽ�*10000	
			if(PackLength>offset)
			{
				int mgjyxj = *(int*)(PackData+offset);
				offset += 4;
				CString mgjyxjstr = _T("");mgjyxjstr.Format(L"%d",mgjyxj);
				m_ListCtl.SetItemText(0,7,mgjyxjstr);
			}
			//int	ÿ�ɹ�����*10000	
			if(PackLength>offset)
			{
				int mggjj = *(int*)(PackData+offset);
				offset += 4;
				CString mggjjstr = _T("");mggjjstr.Format(L"%d",mggjj);
				m_ListCtl.SetItemText(0,8,mggjjstr);
			}
			//int	ÿ��δ����*10000	
			if(PackLength>offset)
			{
				int mgwfp = *(int*)(PackData+offset);
				offset += 4;
				CString mgwfpstr = _T("");mgwfpstr.Format(L"%d",mgwfp);
				m_ListCtl.SetItemText(0,9,mgwfpstr);
			}
			//int	�ɶ�Ȩ���*10000	
			if(PackLength>offset)
			{
				int gdqyb = *(int*)(PackData+offset);
				offset += 4;
				CString gdqybstr = _T("");gdqybstr.Format(L"%d",gdqyb);
				m_ListCtl.SetItemText(0,10,gdqybstr);
			}
			//int	������ͬ��*10000
			if(PackLength>offset)
			{
				int jlrtb = *(int*)(PackData+offset);
				offset += 4;
				CString jlrtbstr = _T("");jlrtbstr.Format(L"%d",jlrtb);
				m_ListCtl.SetItemText(0,11,jlrtbstr);
			}
			//int	��Ӫ����ͬ��*10000
			if(PackLength>offset)
			{
				int zysr = *(int*)(PackData+offset);
				offset += 4;
				CString zysrstr = _T("");zysrstr.Format(L"%d",zysr);
				m_ListCtl.SetItemText(0,12,zysrstr);
			}
			//int	����ë����*10000
			if(PackLength>offset)
			{
				int xsmll = *(int*)(PackData+offset);
				offset += 4;
				CString xsmllstr = _T("");xsmllstr.Format(L"%d",xsmll);
				m_ListCtl.SetItemText(0,13,xsmllstr);
			}
			//int	����ÿ�ɾ���*10000	
			if(PackLength>offset)
			{
				int tzmgjz = *(int*)(PackData+offset);
				offset += 4;
				CString tzmgjzstr = _T("");tzmgjzstr.Format(L"%d",tzmgjz);
				m_ListCtl.SetItemText(0,14,tzmgjzstr);
			}
			//int	���ʲ�  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int tolmoney = *(int*)(PackData+offset);
				offset += 4;
				CString tolmoneystr = _T("");tolmoneystr.Format(L"%d",tolmoney);
				m_ListCtl.SetItemText(0,15,tolmoneystr);
			}
			//int	�����ʲ�  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int ldzc = *(int*)(PackData+offset);
				offset += 4;
				CString ldzcstr = _T("");ldzcstr.Format(L"%d",ldzc);
				m_ListCtl.SetItemText(0,16,ldzcstr);
			}
			//int	�̶��ʲ�  //��λ:ǧԪ	
			if(PackLength>offset)
			{
				int gdzc = *(int*)(PackData+offset);
				offset += 4;
				CString gdzcstr = _T("");gdzcstr.Format(L"%d",gdzc);
				m_ListCtl.SetItemText(0,17,gdzcstr);
			}
			//int	�����ʲ�  //��λ:ǧԪ	
			if(PackLength>offset)
			{
				int wxzc = *(int*)(PackData+offset);
				offset += 4;
				CString wxzcstr = _T("");wxzcstr.Format(L"%d",wxzc);
				m_ListCtl.SetItemText(0,18,wxzcstr);
			}
			//int	������ծ  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int ldfz = *(int*)(PackData+offset);
				offset += 4;
				CString ldfzstr = _T("");ldfzstr.Format(L"%d",ldfz);
				m_ListCtl.SetItemText(0,19,ldfzstr);
			}
			//int	���ڸ�ծ  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int cqfz = *(int*)(PackData+offset);
				offset += 4;
				CString cqfzstr = _T("");cqfzstr.Format(L"%d",cqfz);
				m_ListCtl.SetItemText(0,20,cqfzstr);
			}
			//int	�ܸ�ծ  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int tolfz = *(int*)(PackData+offset);
				offset += 4;
				CString tolfzstr = _T("");tolfzstr.Format(L"%d",tolfz);
				m_ListCtl.SetItemText(0,21,tolfzstr);
			}
			//int	�ɶ�Ȩ��  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int gdqy = *(int*)(PackData+offset);
				offset += 4;
				CString gdqystr = _T("");gdqystr.Format(L"%d",gdqy);
				m_ListCtl.SetItemText(0,22,gdqystr);
			}
			//int	�ʱ�������  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int zbgjj = *(int*)(PackData+offset);
				offset += 4;
				CString zbgjjstr = _T("");zbgjjstr.Format(L"%d",zbgjj);
				m_ListCtl.SetItemText(0,23,zbgjjstr);
			}
			//int	��Ӫ�ֽ�����  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int jyzjll = *(int*)(PackData+offset);
				offset += 4;
				CString jyzjllstr = _T("");jyzjllstr.Format(L"%d",jyzjll);
				m_ListCtl.SetItemText(0,24,jyzjllstr);
			}
			//int	Ͷ���ֽ�����  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int tzxjll = *(int*)(PackData+offset);
				offset += 4;
				CString tzxjllstr = _T("");tzxjllstr.Format(L"%d",tzxjll);
				m_ListCtl.SetItemText(0,25,tzxjllstr);
			}
			//int	�����ֽ�����  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int czxjll = *(int*)(PackData+offset);
				offset += 4;
				CString czxjllstr = _T("");czxjllstr.Format(L"%d",czxjll);
				m_ListCtl.SetItemText(0,26,czxjllstr);
			}
			//int	�ֽ����Ӷ�  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int xjzje = *(int*)(PackData+offset);
				offset += 4;
				CString xjzjestr = _T("");xjzjestr.Format(L"%d",xjzje);
				m_ListCtl.SetItemText(0,27,xjzjestr);
			}
			//int	��Ӫ����  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int zysr = *(int*)(PackData+offset);
				offset += 4;
				CString zysrstr = _T("");zysrstr.Format(L"%d",zysr);
				m_ListCtl.SetItemText(0,28,zysrstr);
			}
			//int	��Ӫ����  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int zylr = *(int*)(PackData+offset);
				offset += 4;
				CString zylrstr = _T("");zylrstr.Format(L"%d",zylr);
				m_ListCtl.SetItemText(0,29,zylrstr);
			}
			//int	Ӫҵ����  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int yylr = *(int*)(PackData+offset);
				offset += 4;
				CString yylrstr = _T("");yylrstr.Format(L"%d",yylr);
				m_ListCtl.SetItemText(0,30,yylrstr);
			}
			//int	Ͷ������  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int tzsy = *(int*)(PackData+offset);
				offset += 4;
				CString tzsystr = _T("");tzsystr.Format(L"%d",tzsy);
				m_ListCtl.SetItemText(0,31,tzsystr);
			}
			//int	Ӫҵ����֧  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int yywsr = *(int*)(PackData+offset);
				offset += 4;
				CString yywsrstr = _T("");yywsrstr.Format(L"%d",yywsr);
				m_ListCtl.SetItemText(0,32,yywsrstr);
			}
			//int	�����ܶ�  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int lrze = *(int*)(PackData+offset);
				offset += 4;
				CString lrzestr = _T("");lrzestr.Format(L"%d",lrze);
				m_ListCtl.SetItemText(0,33,lrzestr);
			}
			//int	������  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int jlr = *(int*)(PackData+offset);
				offset += 4;
				CString jlrstr = _T("");jlrstr.Format(L"%d",jlr);
				m_ListCtl.SetItemText(0,34,jlrstr);
			}
			//int	δ��������  //��λ:ǧԪ
			if(PackLength>offset)
			{
				int wfplr = *(int*)(PackData+offset);
				offset += 4;
				CString wfplrstr = _T("");wfplrstr.Format(L"%d",wfplr);
				m_ListCtl.SetItemText(0,35,wfplrstr);
			}
			//int	�ܹɱ�	//��λ:���
			if(PackLength>offset)
			{
				int zgb = *(int*)(PackData+offset);
				offset += 4;
				CString zgbstr = _T("");zgbstr.Format(L"%d",zgb);
				m_ListCtl.SetItemText(0,36,zgbstr);
			}
			//int	�����۹ɷݺϼ�	//��λ:���
			if(PackLength>offset)
			{
				int wxsgb = *(int*)(PackData+offset);
				offset += 4;
				CString wxsgbstr = _T("");wxsgbstr.Format(L"%d",wxsgb);
				m_ListCtl.SetItemText(0,37,wxsgbstr);
			}
			//int	A��	//��λ:���
			if(PackLength>offset)
			{
				int Ag = *(int*)(PackData+offset);
				offset += 4;
				CString Agstr = _T("");Agstr.Format(L"%d",Ag);
				m_ListCtl.SetItemText(0,38,Agstr);
			}
			//int	B��	//��λ:���
			if(PackLength>offset)
			{
				int Bg = *(int*)(PackData+offset);
				offset += 4;
				CString Bgstr = _T("");Bgstr.Format(L"%d",Bg);
				m_ListCtl.SetItemText(0,39,Bgstr);
			}
			//int	�����������ʹ�	//��λ:���
			if(PackLength>offset)
			{
				int jwsswzg = *(int*)(PackData+offset);
				offset += 4;
				CString jwsswzgstr = _T("");jwsswzgstr.Format(L"%d",jwsswzg);
				m_ListCtl.SetItemText(0,40,jwsswzgstr);
			}
			//int	������ͨ�ɷ�	//��λ:���
			if(PackLength>offset)
			{
				int qtltgf = *(int*)(PackData+offset);
				offset += 4;
				CString qtltgfstr = _T("");qtltgfstr.Format(L"%d",qtltgf);
				m_ListCtl.SetItemText(0,41,qtltgfstr);
			}
			//int	���۹ɷݺϼ�	//��λ:���
			if(PackLength>offset)
			{
				int xsgfhj = *(int*)(PackData+offset);
				offset += 4;
				CString xsgfhjstr = _T("");xsgfhjstr.Format(L"%d",xsgfhj);
				m_ListCtl.SetItemText(0,42,xsgfhjstr);
			}
			//int	���ҳֹ�	//��λ:���
			if(PackLength>offset)
			{
				int gjcg = *(int*)(PackData+offset);
				offset += 4;
				CString gjcgstr = _T("");gjcgstr.Format(L"%d",gjcg);
				m_ListCtl.SetItemText(0,43,gjcgstr);
			}
			//int	���з��˳ֹ�	//��λ:���
			if(PackLength>offset)
			{
				int gjfrcg = *(int*)(PackData+offset);
				offset += 4;
				CString gjfrcgstr = _T("");gjfrcgstr.Format(L"%d",gjfrcg);
				m_ListCtl.SetItemText(0,44,gjfrcgstr);
			}
			//int	���ڷ��˳ֹ�	//��λ:���
			if(PackLength>offset)
			{
				int jlfrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jlfrcgstr = _T("");jlfrcgstr.Format(L"%d",jlfrcg);
				m_ListCtl.SetItemText(0,45,jlfrcgstr);
			}
			//int	������Ȼ�˳ֹ�	//��λ:���
			if(PackLength>offset)
			{
				int jlzrrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jlzrrcgstr = _T("");jlzrrcgstr.Format(L"%d",jlzrrcg);
				m_ListCtl.SetItemText(0,46,jlzrrcgstr);
			}
			//int	���������˹ɷ�	//��λ:���
			if(PackLength>offset)
			{
				int qtfqrgf = *(int*)(PackData+offset);
				offset += 4;
				CString qtfqrgfstr = _T("");qtfqrgfstr.Format(L"%d",qtfqrgf);
				m_ListCtl.SetItemText(0,47,qtfqrgfstr);
			}
			//int	ļ�����˹ɷ�
			if(PackLength>offset)
			{
				int mjfrgf = *(int*)(PackData+offset);
				offset += 4;
				CString mjfrgfstr = _T("");mjfrgfstr.Format(L"%d",mjfrgf);
				m_ListCtl.SetItemText(0,48,mjfrgfstr);
			}
			//int	���ⷨ�˳ֹ�	//��λ:���
			if(PackLength>offset)
			{
				int jwfrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jwfrcgstr = _T("");jwfrcgstr.Format(L"%d",jwfrcg);
				m_ListCtl.SetItemText(0,49,jwfrcgstr);
			}
			//int	������Ȼ�˳ֹ�	//��λ:���
			if(PackLength>offset)
			{
				int jwzrrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jwzrrcgstr = _T("");jwzrrcgstr.Format(L"%d",jwzrrcg);
				m_ListCtl.SetItemText(0,50,jwzrrcgstr);
			}
			//int	���ȹɻ�����	//��λ:���
			if(PackLength>offset)
			{
				int yxg = *(int*)(PackData+offset);
				offset += 4;
				CString yxgstr = _T("");yxgstr.Format(L"%d",yxg);
				m_ListCtl.SetItemText(0,51,yxgstr);
			}
		}break;
	case 2976:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
		
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
		}break;
	case 2977:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
	
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//λ��		short	    ���ص����ݵĵ�һ����¼�����
			if(PackLength>offset)
			{
				short position = *(short*)(PackData+offset);
				offset += 2;
				CString posstr = _T("");posstr.Format(L"%d",position);
				m_ListCtl.SetItemText(0,1,posstr);
			}
			//λ��		short	    ���ص����ݵĵ�һ����¼�����
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int i=0;i<RecdNum;i++)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
				}
				for(int j=0;j<RecdNum;j++)
				{
					//��Ʊ����	string
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,3,codestr.c_str());
						delete []code;
					}
					//��Ʊ����	string
					if(PackLength>offset)
					{
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(j,4,namestr.c_str());
						delete []name;
					}
					//����	char	    //���ͺ�2976һ��
					if(PackLength>offset)
					{
						char type = *(PackData+offset);
						offset += 1;
						int typeval = type;CString typeSTR = _T("");typeSTR.Format(L"%d",typeval);
						m_ListCtl.SetItemText(j,5,typeSTR);
					}
					//ʱ��	short	    //ʱ��HHMM
					if(PackLength>offset)
					{
						short datetime = *(short*)(PackData+offset);
						offset += 2;
						CString datestr = _T("");datestr.Format(L"%d",datetime);
						m_ListCtl.SetItemText(j,6,datestr);
					}
					//��ص���	int
					if(PackLength>offset)
					{
						int xgl = *(int*)(PackData+offset);
						offset += 4;
						CString xglstr = _T("");xglstr.Format(L"%d",xgl);
						m_ListCtl.SetItemText(j,7,xglstr);
					}
				}
			}
		}break;
	case 2978:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
	
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֶ�����        int   ���ͷ��ص��ֶΣ�������˵���������ö�Ӧ��
			unsigned int attr = 0;
			if(PackLength>offset)
			{
				attr = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString attrstr = _T("");attrstr.Format(L"%u",attr);
				m_ListCtl.SetItemText(0,1,attrstr);
			}
			//�ܼ�¼��Ŀ      short   �Ը����б����ͷ��ص������õĸ���������Ʒ�б��ص��Ǹ�id��������Ŀ
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,2,tolnumstr);
			}
			//λ��		short	    ���ص����ݵĵ�һ����¼�����
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,3,RecdNumStr);
				for(int i=0;i<RecdNum;i++)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
				}
				for(int j=0;j<RecdNum;j++)
				{
					//����		 string�����ֶα��У������ֶ��Ƿ��и����ֶ����ԣ�
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,4,codestr.c_str());
						delete []code;
					}
					if(attr&0x0001)
					{
					    //��Ʊ����	string
					    if(PackLength>offset)
					    {
						    short namelen = *(short*)(PackData+offset);
						    offset += 2;
						    char *name = new char[namelen+1];
						    memset(name,0,namelen+1);
						    memcpy(name,PackData+offset,namelen);
						    offset += namelen;
						    wstring namestr = UTF8_2_UTF16(name);
						    m_ListCtl.SetItemText(j,5,namestr.c_str());
						    delete []name;
						}
					}
					if(attr&0x0002)
					{
					    //����                 char,1λ�����ͺͼ۸�λ��   
					    if(PackLength>offset)
					    {
							char type = *(PackData+offset);
							offset += 1;
							int typeval = type;CString typestr = _T("");typestr.Format(L"%d",typeval);
							m_ListCtl.SetItemText(j,6,typestr);
						}
						//�۸�λ��             char
						if(PackLength>offset)
					    {
							char pricepos = *(PackData+offset);
							offset += 1;
							int priceposval = pricepos;CString priceposstr = _T("");priceposstr.Format(L"%d",priceposval);
							m_ListCtl.SetItemText(j,7,priceposstr);
						}
					}
					//2λ���ɽ�����λ
					if(attr&0x0004)
					{
					    //�ɽ�����λ           short   
					    if(PackLength>offset)
					    {
							short cjldw = *(short*)(PackData+offset);
							offset += 2;
							CString cjldwstr = _T("");cjldwstr.Format(L"%d",cjldw);
							m_ListCtl.SetItemText(j,8,cjldwstr);
						}
					}
					//3λ����ͨ�ɱ�
					if(attr&0x0008)
					{
					    //��ͨ�ɱ�             int   
					    if(PackLength>offset)
					    {
							int ltgb = *(int*)(PackData+offset);
							offset += 4;
							CString ltgbstr = _T("");ltgbstr.Format(L"%d",ltgb);
							m_ListCtl.SetItemText(j,9,ltgbstr);
						}
					}
					//4λ���ܹɱ�
					if(attr&0x0010)
					{
					    //�ܹɱ�               int 
					    if(PackLength>offset)
					    {
							unsigned int zgb = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zgbstr = _T("");zgbstr.Format(L"%u",zgb);
							m_ListCtl.SetItemText(j,10,zgbstr);
						}
					}
					//5λ������
					if(attr&0x0020)
					{
					    //����                 int 
					    if(PackLength>offset)
					    {
							unsigned int zsprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
							m_ListCtl.SetItemText(j,11,zspricestr);
						}
					}
					//6λ����ͣ�۸�͵�ͣ�۸�
					if(attr&0x0040)
					{
					    //��ͣ�۸�             int
					    if(PackLength>offset)
					    {
							unsigned int highprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
							m_ListCtl.SetItemText(j,12,highpricestr);
						}
						//��ͣ�۸�             int
					    if(PackLength>offset)
					    {
							unsigned int lowprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
							m_ListCtl.SetItemText(j,13,lowpricestr);
						}
					}
					//7λ�����ս����
					if(attr&0x0080)
					{
					    //���ս����           int
					    if(PackLength>offset)
					    {
							unsigned int ysettleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString ysettlepricestr = _T("");ysettlepricestr.Format(L"%u",ysettleprice);
							m_ListCtl.SetItemText(j,14,ysettlepricestr);
						}
					}
					//8λ�����ճֲ���
					if(attr&0x0100)
					{
					    //���ճֲ���           int 
					    if(PackLength>offset)
					    {
							unsigned int yholdprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString yholdpricestr = _T("");yholdpricestr.Format(L"%u",yholdprice);
							m_ListCtl.SetItemText(j,15,yholdpricestr);
						}
					}
					//9λ����̬����ʱ�䣬HHMMSS
					if(attr&0x0200)
					{
					    //��̬����ʱ�䣬       int  HHMMSS 
					    if(PackLength>offset)
					    {
							unsigned int Dynctime = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString Dynctimestr = _T("");Dynctimestr.Format(L"%u",Dynctime);
							m_ListCtl.SetItemText(j,16,Dynctimestr);
						}
					}
					//10λ������
					if(attr&0x0200)
					{
					    //����                 int
					    if(PackLength>offset)
					    {
							unsigned int openprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
							m_ListCtl.SetItemText(j,17,openpricestr);
						}
					}
					//11λ����ߺ����
					if(attr&0x0800)
					{
					    //���                 int
					    if(PackLength>offset)
					    {
							unsigned int zgprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zgpricestr = _T("");zgpricestr.Format(L"%u",zgprice);
							m_ListCtl.SetItemText(j,18,zgpricestr);
						}
						//���                 int
					    if(PackLength>offset)
					    {
							unsigned int zdprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zdpricestr = _T("");zdpricestr.Format(L"%u",zdprice);
							m_ListCtl.SetItemText(j,19,zdpricestr);
						}
					}
					//12λ������
					if(attr&0x1000)
					{
					    //����                 int
					    if(PackLength>offset)
					    {
							unsigned int newprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
							m_ListCtl.SetItemText(j,20,newpricestr);
						}
					}
					//13λ������
					if(attr&0x2000)
					{
					    //����                 int
					    if(PackLength>offset)
					    {
							unsigned int avgprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString avgpricestr = _T("");avgpricestr.Format(L"%u",avgprice);
							m_ListCtl.SetItemText(j,21,avgpricestr);
						}
					}
					//14λ���ɽ��� 
					if(attr&0x4000)
					{
					    //�ɽ���               int
					    if(PackLength>offset)
					    {
							unsigned int cjlval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString cjlvalstr = _T("");cjlvalstr.Format(L"%u",cjlval);
							m_ListCtl.SetItemText(j,22,cjlvalstr);
						}
					}
					//15λ������ 
					if(attr&0x8000)
					{
					    //����                 int
					    if(PackLength>offset)
					    {
							unsigned int xsval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString xsvalstr = _T("");xsvalstr.Format(L"%u",xsval);
							m_ListCtl.SetItemText(j,23,xsvalstr);
						}
					}
					//16λ���ɽ��� 
					if(attr&0x10000)
					{
					    //�ɽ���               int
					    if(PackLength>offset)
					    {
							unsigned int cjeval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString cjevalstr = _T("");cjevalstr.Format(L"%u",cjeval);
							m_ListCtl.SetItemText(j,24,cjevalstr);
						}
					}
					//17λ������ 
					if(attr&0x20000)
					{
					    //����                 int
					    if(PackLength>offset)
					    {
							unsigned int innerval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString innervalstr = _T("");innervalstr.Format(L"%u",innerval);
							m_ListCtl.SetItemText(j,25,innervalstr);
						}
					}
					//18λ���ֲ��� 
					if(attr&0x40000)
					{
					    //�ֲ���               int
					    if(PackLength>offset)
					    {
							unsigned int holdval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString holdvalstr = _T("");holdvalstr.Format(L"%u",holdval);
							m_ListCtl.SetItemText(j,26,holdvalstr);
						}
					}
					//19λ������� 
					if(attr&0x80000)
					{
					    //�����               int
					    if(PackLength>offset)
					    {
							unsigned int settleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString settlepricestr = _T("");settlepricestr.Format(L"%u",settleprice);
							m_ListCtl.SetItemText(j,27,settlepricestr);
						}
					}
					//20λ����һ�ۺ���һ�� 
					if(attr&0x100000)
					{
					    //��һ��                int
					    if(PackLength>offset)
					    {
							unsigned int sell1price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell1pricestr = _T("");sell1pricestr.Format(L"%u",sell1price);
							m_ListCtl.SetItemText(j,28,sell1pricestr);
						}
						//��һ��                int
					    if(PackLength>offset)
					    {
							unsigned int buy1price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy1pricestr = _T("");buy1pricestr.Format(L"%u",buy1price);
							m_ListCtl.SetItemText(j,29,buy1pricestr);
						}
					}
					//21λ����һ������һ�� 
					if(attr&0x200000)
					{
					    //��һ��                int
					    if(PackLength>offset)
					    {
							unsigned int sell1val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell1valstr = _T("");sell1valstr.Format(L"%u",sell1val);
							m_ListCtl.SetItemText(j,30,sell1valstr);
						}
						//��һ��                int
					    if(PackLength>offset)
					    {
							unsigned int buy1val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy1valstr = _T("");buy1valstr.Format(L"%u",buy1val);
							m_ListCtl.SetItemText(j,31,buy1valstr);
						}
					}
					//22λ�������ۺ������ 
					if(attr&0x400000)
					{
					    //������                int
					    if(PackLength>offset)
					    {
							unsigned int sell2price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell2pricestr = _T("");sell2pricestr.Format(L"%u",sell2price);
							m_ListCtl.SetItemText(j,32,sell2pricestr);
						}
						//�����                int
					    if(PackLength>offset)
					    {
							unsigned int buy2price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy2pricestr = _T("");buy2pricestr.Format(L"%u",buy2price);
							m_ListCtl.SetItemText(j,33,buy2pricestr);
						}
					}
					//23λ��������������� 
					if(attr&0x800000)
					{
					    //������               int
					    if(PackLength>offset)
					    {
							unsigned int sell2val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell2valstr = _T("");sell2valstr.Format(L"%u",sell2val);
							m_ListCtl.SetItemText(j,34,sell2valstr);
						}
						//�����                int
					    if(PackLength>offset)
					    {
							unsigned int buy2val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy2valstr = _T("");buy2valstr.Format(L"%u",buy2val);
							m_ListCtl.SetItemText(j,35,buy2valstr);
						}
					}
					//24λ�������ۺ������� 
					if(attr&0x1000000)
					{
					    //������                int
					    if(PackLength>offset)
					    {
							unsigned int sell3price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell3pricestr = _T("");sell3pricestr.Format(L"%u",sell3price);
							m_ListCtl.SetItemText(j,36,sell3pricestr);
						}
						//������                int
					    if(PackLength>offset)
					    {
							unsigned int buy3price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy3pricestr = _T("");buy3pricestr.Format(L"%u",buy3price);
							m_ListCtl.SetItemText(j,37,buy3pricestr);
						}
					}
					//25λ���������������� 
					if(attr&0x2000000)
					{
					    //������               int
					    if(PackLength>offset)
					    {
							unsigned int sell3val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell3valstr = _T("");sell3valstr.Format(L"%u",sell3val);
							m_ListCtl.SetItemText(j,38,sell3valstr);
						}
						//������                int
					    if(PackLength>offset)
					    {
							unsigned int buy3val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy3valstr = _T("");buy3valstr.Format(L"%u",buy3val);
							m_ListCtl.SetItemText(j,39,buy3valstr);
						}
					}
					//26λ�����ļۺ����ļ� 
					if(attr&0x4000000)
					{
					    //���ļ�                int
					    if(PackLength>offset)
					    {
							unsigned int sell4price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell4pricestr = _T("");sell4pricestr.Format(L"%u",sell4price);
							m_ListCtl.SetItemText(j,40,sell4pricestr);
						}
						//���ļ�                int
					    if(PackLength>offset)
					    {
							unsigned int buy4price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy4pricestr = _T("");buy4pricestr.Format(L"%u",buy4price);
							m_ListCtl.SetItemText(j,41,buy4pricestr);
						}
					}
					//27λ���������������� 
					if(attr&0x8000000)
					{
					    //������               int
					    if(PackLength>offset)
					    {
							unsigned int sell4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell4valstr = _T("");sell4valstr.Format(L"%u",sell4val);
							m_ListCtl.SetItemText(j,42,sell4valstr);
						}
						//������                int
					    if(PackLength>offset)
					    {
							unsigned int buy4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy4valstr = _T("");buy4valstr.Format(L"%u",buy4val);
							m_ListCtl.SetItemText(j,43,buy4valstr);
						}
					}
					//28λ������ۺ������ 
					if(attr&0x10000000)
					{
					    //�����                int
					    if(PackLength>offset)
					    {
							unsigned int sell5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell5pricestr = _T("");sell5pricestr.Format(L"%u",sell5price);
							m_ListCtl.SetItemText(j,44,sell5pricestr);
						}
						//�����                int
					    if(PackLength>offset)
					    {
							unsigned int buy5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy5pricestr = _T("");buy5pricestr.Format(L"%u",buy5price);
							m_ListCtl.SetItemText(j,45,buy5pricestr);
						}
					}
					//29λ���������������� 
					if(attr&0x20000000)
					{
					    //������               int
					    if(PackLength>offset)
					    {
							unsigned int sell5val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell5valstr = _T("");sell5valstr.Format(L"%u",sell5val);
							m_ListCtl.SetItemText(j,46,sell5valstr);
						}
						//������                int
					    if(PackLength>offset)
					    {
							unsigned int buy5val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy5valstr = _T("");buy5valstr.Format(L"%u",buy5val);
							m_ListCtl.SetItemText(j,47,buy5valstr);
						}
					}
					//30λ��������ȯ���  //2015-3-13���� 
					if(attr&0x40000000)
					{
					    //������ȯ���         char //2015-3-13����
					    if(PackLength>offset)
					    {
							char rzrqflag = *(PackData+offset);
							offset += 1;
							int flagval = rzrqflag;CString rzrqflagstr = _T("");rzrqflagstr.Format(L"%u",flagval);
							m_ListCtl.SetItemText(j,48,rzrqflagstr);
						}
					}
					//31λ����Ϣ���ױ��  //2015-3-13����
					if(attr&0x80000000)
					{
						//��Ϣ����ʱ��         int  //2015-3-13����
						if(PackLength>offset)
					    {
							unsigned int dltime = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString dltimestr = _T("");dltimestr.Format(L"%u",dltime);
							m_ListCtl.SetItemText(j,49,dltimestr);
						}
					}
				}
			}
		}break;
	case 2981:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�����Ŀ	short		//ָ���һ������Ŀ
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				
				int i = 0;
				m_ListCtl.SetItemText(0,1,RecdNumStr);
				while(PackLength-offset>0)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
					
					//������		string
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(i,2,codestr.c_str());
						delete []code;
					}
					//�ý����ʾ��ʽ	char	//�ý���ڽ�������ʾ��ʽ��0��ʾ�б�ʽ��1��ʾ�ڲ˵���ʽ
					if(PackLength>offset)
					{
						char type = *(PackData+offset);
						offset += 1;
						int typeval = type;CString typeSTR = _T("");typeSTR.Format(L"%d",typeval);
						m_ListCtl.SetItemText(i,3,typeSTR);
					}
					//����Ʒ��		char
					if(PackLength>offset)
					{
						char typevalue = *(PackData+offset);
						offset += 1;
						int typevalueval = typevalue;CString typevalueSTR = _T("");typevalueSTR.Format(L"%d",typevalueval);
						m_ListCtl.SetItemText(i,4,typevalueSTR);
					}
					//��������ַ�          char    //Ϊ0��ʾ��Ч�ַ�
					if(PackLength>offset)
					{
						char typeszm = *(PackData+offset);
						offset += 1;
						CString typeszmSTR = _T("");typeszmSTR.Format(L"%c",typeszm);
						m_ListCtl.SetItemText(i,5,typeszmSTR);
					}
					//�ý������		char	//0��ʾ�ýڵ���Ҷ��㣬�����short�Ǹý���id��1��ʾ�ý���Ƿ�Ҷ��㣬�����short��¼���ӽ����Ŀ
					if(PackLength>offset)
					{
						char jdtype = *(PackData+offset);
						offset += 1;
						int jdtypeval = jdtype;CString jdtypeSTR = _T("");jdtypeSTR.Format(L"%d",jdtypeval);
						m_ListCtl.SetItemText(i,6,jdtypeSTR);
					}
					//������Ŀ������id	short
					if(PackLength>offset)
					{
						unsigned short typenum = *(unsigned short*)(PackData+offset);
						offset += 2;
						CString typenumSTR = _T("");typenumSTR.Format(L"%d",typenum);
						m_ListCtl.SetItemText(i,7,typenumSTR);
					}
					i += 1;
				}
			}
		}break;
	case 2982:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//����             short              //20120216�Ķ�
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset+=2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumstr);
			}
			//λ��             short		
			if(PackLength>offset)
			{
				short position = *(short*)(PackData+offset);
				offset+=2;
				CString positionstr = _T("");positionstr.Format(L"%d",position);
				m_ListCtl.SetItemText(0,2,positionstr);
			}
			//��Ŀ		    short
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				int i = 0;
				m_ListCtl.SetItemText(0,3,RecdNumStr);
				while(PackLength-offset>0)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
					
					//����	    String
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(i,4,codestr.c_str());
						delete []code;
					}
					//��Ʊ����   String
					if(PackLength>offset)
					{
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(i,5,namestr.c_str());
						delete []name;
					}
					//��ע��     int	
					if(PackLength>offset)
					{
						unsigned int gzdval = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString gzdSTR = _T("");gzdSTR.Format(L"%u",gzdval);
						m_ListCtl.SetItemText(i,6,gzdSTR);
					}
					//�Ƿ�       int*10000  //����������
					if(PackLength>offset)
					{
						int zfvalue = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalueSTR = _T("");zfvalueSTR.Format(L"%d",zfvalue);
						m_ListCtl.SetItemText(i,7,zfvalueSTR);
					}
					//С����λ�� char
					if(PackLength>offset)
					{
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointposval = pointpos;CString pointposSTR = _T("");pointposSTR.Format(L"%d",pointposval);
						m_ListCtl.SetItemText(i,8,pointposSTR);
					}
					//���¼�     int
					if(PackLength>offset)
					{
						unsigned int newprice = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newpriceSTR = _T("");newpriceSTR.Format(L"%u",newprice);
						m_ListCtl.SetItemText(i,9,newpriceSTR);
					}
					i += 1;
				}
			}
		}break;
	case 2984:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ܼ�¼��	short	�������˸������ܼ�¼��
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset+=2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumstr);
			}
			//��¼��         short	���м�¼��
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i = 0; i<RecdNum; i++)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
				}
				m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					//����           String
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,3,codestr.c_str());
						delete []code;
					}
					//����           String
					if(PackLength>offset)
					{
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(j,4,namestr.c_str());
						delete []name;
					}
					//С��λ��       byte
					if(PackLength>offset)
					{
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointposval = pointpos;CString pointposSTR = _T("");pointposSTR.Format(L"%d",pointposval);
						m_ListCtl.SetItemText(j,5,pointposSTR);
					}
					//���¼�         int	
					if(PackLength>offset)
					{
						unsigned int newprice = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newpriceSTR = _T("");newpriceSTR.Format(L"%u",newprice);
						m_ListCtl.SetItemText(j,6,newpriceSTR);
					}
					//�Ƿ�           int*10000
					if(PackLength>offset)
					{
						int zfvalue = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalueSTR = _T("");zfvalueSTR.Format(L"%d",zfvalue);
						m_ListCtl.SetItemText(j,7,zfvalueSTR);
					}
					
					//����id       short
					if(PackLength>offset)
					{
						unsigned short bkid = *(unsigned short*)(PackData+offset);
						offset += 2;
						CString bkidSTR = _T("");bkidSTR.Format(L"%u",bkid);
						m_ListCtl.SetItemText(j,8,bkidSTR);
					}
					//����           String
					if(PackLength>offset)
					{
						short code1len = *(short*)(PackData+offset);
						offset += 2;
						char *code1 = new char[code1len+1];
						memset(code1,0,code1len+1);
						memcpy(code1,PackData+offset,code1len);
						offset += code1len;
						wstring code1str = UTF8_2_UTF16(code1);
						m_ListCtl.SetItemText(j,9,code1str.c_str());
						delete []code1;
					}
					//����           String
					if(PackLength>offset)
					{
						short name1len = *(short*)(PackData+offset);
						offset += 2;
						char *name1 = new char[name1len+1];
						memset(name1,0,name1len+1);
						memcpy(name1,PackData+offset,name1len);
						offset += name1len;
						wstring name1str = UTF8_2_UTF16(name1);
						m_ListCtl.SetItemText(j,10,name1str.c_str());
						delete []name1;
					}
					//С��λ��       byte
					if(PackLength>offset)
					{
						char pointpos1 = *(PackData+offset);
						offset += 1;
						int pointpos1val = pointpos1;CString pointpos1STR = _T("");pointpos1STR.Format(L"%d",pointpos1val);
						m_ListCtl.SetItemText(j,11,pointpos1STR);
					}
					//���¼�         int	
					if(PackLength>offset)
					{
						unsigned int newprice1 = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newprice1STR = _T("");newprice1STR.Format(L"%u",newprice1);
						m_ListCtl.SetItemText(j,12,newprice1STR);
					}
					//�Ƿ�           int*10000
					if(PackLength>offset)
					{
						int zfvalue1 = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalue1STR = _T("");zfvalue1STR.Format(L"%d",zfvalue1);
						m_ListCtl.SetItemText(j,13,zfvalue1STR);
					}
				}
			}
		}break;
	case 2985:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// ���ݼ��       char
			if(PackLength>offset)
			{
				char datepos = *(PackData+offset);
				offset+=1;
				int dateposval = datepos;CString dateposstr = _T("");dateposstr.Format(L"%d",dateposval);
				m_ListCtl.SetItemText(0,1,dateposstr);
			}
			//������������   char	    //
			if(PackLength>offset)
			{
				char zsdatepos = *(PackData+offset);
				offset+=1;
				int zsdateposval = zsdatepos;CString zsdateposstr = _T("");zsdateposstr.Format(L"%d",zsdateposval);
				m_ListCtl.SetItemText(0,2,zsdateposstr);
			}
			//����λ��       short		
			if(PackLength>offset)
			{
				short position = *(short*)(PackData+offset);
				offset+=2;
				CString positionstr = _T("");positionstr.Format(L"%d",position);
				m_ListCtl.SetItemText(0,3,positionstr);
			}
			//��¼��         short
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				int i = 0;
				m_ListCtl.SetItemText(0,4,RecdNumStr);
				while(PackLength-offset>0)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
					
					//ʱ��           int
					if(PackLength>offset)
					{
						unsigned int datetime = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString datetimestr = _T("");datetimestr.Format(L"%u",datetime);
						 m_ListCtl.SetItemText(i,5,datetimestr);
					}
					//���¼�     int
					if(PackLength>offset)
					{
						unsigned int newprice = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newpriceSTR = _T("");newpriceSTR.Format(L"%u",newprice);
						m_ListCtl.SetItemText(i,6,newpriceSTR);
					}
					//�ɽ���         int	   //�ܳɽ���
					if(PackLength>offset)
					{
						unsigned int cjval = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString cjvalSTR = _T("");cjvalSTR.Format(L"%u",cjval);
						m_ListCtl.SetItemText(i,7,cjvalSTR);
					}
					//����           int  
					if(PackLength>offset)
					{
						int avgvalue = *(int*)(PackData+offset);
						offset += 4;
						CString avgvalueSTR = _T("");avgvalueSTR.Format(L"%d",avgvalue);
						m_ListCtl.SetItemText(i,8,avgvalueSTR);
					}
					//�ֲ���         int      //�ֲܳ���
					if(PackLength>offset)
					{
						unsigned int holdvalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString holdvalueSTR = _T("");holdvalueSTR.Format(L"%u",holdvalue);
						m_ListCtl.SetItemText(i,9,holdvalueSTR);
					}
					i += 1;
				}
			}
		}break;
	case 2987:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");

			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ֶ���������   int
			unsigned int attr = 0;
			if(PackLength>offset)
			{
				attr = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString attrstr = _T("");attrstr.Format(L"%u",attr);
				m_ListCtl.SetItemText(0,1,attrstr);
			}
			//�ܼ�¼��       short
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,2,tolnumstr);
			}
			//��¼��         short
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,3,RecdNumStr);
				for(int i=0;i<RecdNum;i++)
				{
					CString xh=_T("");xh.Format(L"%d",i+2);
					m_ListCtl.InsertItem(i+1,_T(""));
			        m_ListCtl.SetItemText(i+1,0,xh);
				}
				for(int j=0;j<RecdNum;j++)
				{
					//����           String
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,4,codestr.c_str());
						delete []code;
					}
					//����           String
					if(PackLength>offset)
					{
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(j,5,namestr.c_str());
						delete []name;
					}
					////0λ
					if(attr&0x0001)
					{
					    //С��λ��       byte	//���ռۡ����¼ۡ����ߵͼ�С��λ
					    if(PackLength>offset)
					    {
						    unsigned char pointpos = *(PackData+offset);
						    offset += 1;
							int pointposval = pointpos;CString pointposstr=_T("");pointposstr.Format(L"%d",pointposval);
						    m_ListCtl.SetItemText(j,6,pointposstr);
						}
						//��Ʊ����       byte
						if(PackLength>offset)
					    {
						    unsigned char stocktype = *(PackData+offset);
						    offset += 1;
							int stocktypeval = stocktype;CString stocktypestr=_T("");stocktypestr.Format(L"%d",stocktypeval);
						    m_ListCtl.SetItemText(j,7,stocktypestr);
						}
						//�ɽ���λ	  short
						if(PackLength>offset)
			            {
				            short cjtype = *(short*)(PackData+offset);
				            offset += 2;
				            CString cjtypestr = _T("");cjtypestr.Format(L"%d",cjtype);
				            m_ListCtl.SetItemText(0,8,cjtypestr);
			            }
						//���ռ�         int
						if(PackLength>offset)
			            {
				            unsigned int zsprice = *(unsigned int*)(PackData+offset);
				            offset += 4;
				            CString zspricestr = _T("");zspricestr.Format(L"%d",zsprice);
				            m_ListCtl.SetItemText(0,9,zspricestr);
			            }
					}
					//1λ
					if(attr&0x0002)
					{
					    //���¼�         int   
					    if(PackLength>offset)
					    {
							unsigned int newprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
							m_ListCtl.SetItemText(j,10,newpricestr);
						}
					}
					// //2λ
					if(attr&0x0004)
					{
					    //���̼�         int
					    if(PackLength>offset)
					    {
							unsigned int openprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
							m_ListCtl.SetItemText(j,11,openpricestr);
						}
						//��߼�         int
						if(PackLength>offset)
					    {
							unsigned int highprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
							m_ListCtl.SetItemText(j,12,highpricestr);
						}
						//��ͼ�         int
						if(PackLength>offset)
					    {
							unsigned int lowprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
							m_ListCtl.SetItemText(j,13,lowpricestr);
						}
					}
					//3λ
					if(attr&0x0008)
					{
					    //�ɽ���         int      //�ܳɽ���  
					    if(PackLength>offset)
					    {
							int cje = *(int*)(PackData+offset);
							offset += 4;
							CString cjestr = _T("");cjestr.Format(L"%d",cje);
							m_ListCtl.SetItemText(j,14,cjestr);
						}
					}
					//4λ
					if(attr&0x0010)
					{
					   // �ɽ���         int	   //�ܳɽ��� 
					    if(PackLength>offset)
					    {
							unsigned int cjval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString cjvalstr = _T("");cjvalstr.Format(L"%u",cjval);
							m_ListCtl.SetItemText(j,15,cjvalstr);
						}
					}
					//5λ
					if(attr&0x0020)
					{
					    //��ʽ����       int
					    if(PackLength>offset)
					    {
							unsigned int msprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString mspricestr = _T("");mspricestr.Format(L"%u",msprice);
							m_ListCtl.SetItemText(j,16,mspricestr);
						}
						//��ʽ����С��λ byte
						if(PackLength>offset)
					    {
						    unsigned char mspointpos = *(PackData+offset);
						    offset += 1;
							int mspointposval = mspointpos;CString mspointposstr=_T("");mspointposstr.Format(L"%d",mspointposval);
						    m_ListCtl.SetItemText(j,17,mspointposstr);
						}
					}
					//6λ
					if(attr&0x0040)
					{
					    //����������     int
					    if(PackLength>offset)
					    {
							int yhbdl = *(int*)(PackData+offset);
							offset += 4;
							CString yhbdlstr = _T("");yhbdlstr.Format(L"%d",yhbdl);
							m_ListCtl.SetItemText(j,18,yhbdlstr);
						}
						//����������С��λ byte
						if(PackLength>offset)
					    {
						    unsigned char bdpointpos = *(PackData+offset);
						    offset += 1;
							int bdpointposval = bdpointpos;CString bdpointposstr=_T("");bdpointposstr.Format(L"%d",bdpointposval);
						    m_ListCtl.SetItemText(j,19,bdpointposstr);
						}
					}
					//7λ�����ս����
					if(attr&0x0080)
					{
					     //��ļ۸�������  int 
					    if(PackLength>offset)
					    {
							int bpricemgx = *(int*)(PackData+offset);
							offset += 4;
							CString bpricemgxstr = _T("");bpricemgxstr.Format(L"%d",bpricemgx);
							m_ListCtl.SetItemText(j,20,bpricemgxstr);
						}
						//��ļ۸�������С��λ byte
						if(PackLength>offset)
					    {
						    unsigned char bpricemgxpos = *(PackData+offset);
						    offset += 1;
							int bpricemgxposval = bpricemgxpos;CString bpricemgxposstr=_T("");bpricemgxposstr.Format(L"%d",bpricemgxposval);
						    m_ListCtl.SetItemText(j,21,bpricemgxposstr);
						}
					}
					//8λ
					if(attr&0x0100)
					{
					    //��ļ۸���������� int 
					    if(PackLength>offset)
					    {
							int bprice2mgx = *(int*)(PackData+offset);
							offset += 4;
							CString bprice2mgxstr = _T("");bprice2mgxstr.Format(L"%d",bprice2mgx);
							m_ListCtl.SetItemText(j,22,bprice2mgxstr);
						}
						//��ļ۸����������С��λ byte
						if(PackLength>offset)
					    {
						    unsigned char bprice2mgxpos = *(PackData+offset);
						    offset += 1;
							int bprice2mgxposval = bprice2mgxpos;CString bprice2mgxposstr=_T("");bprice2mgxposstr.Format(L"%d",bprice2mgxposval);
						    m_ListCtl.SetItemText(j,23,bprice2mgxposstr);
						}
					}
					//9λ����̬����ʱ�䣬HHMMSS
					if(attr&0x0200)
					{
					    //ʱ��������       int
					    if(PackLength>offset)
					    {
							int timemgx = *(int*)(PackData+offset);
							offset += 4;
							CString timemgxstr = _T("");timemgxstr.Format(L"%d",timemgx);
							m_ListCtl.SetItemText(j,24,timemgxstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char timemgxpos = *(PackData+offset);
						    offset += 1;
							int timemgxposval = timemgxpos;CString timemgxposstr=_T("");timemgxposstr.Format(L"%d",timemgxposval);
						    m_ListCtl.SetItemText(j,25,timemgxposstr);
						}
					}
					//10λ
					if(attr&0x0200)
					{
					   //������������       int
					    if(PackLength>offset)
					    {
							int posmgx = *(int*)(PackData+offset);
							offset += 4;
							CString posmgxstr = _T("");posmgxstr.Format(L"%d",posmgx);
							m_ListCtl.SetItemText(j,26,posmgxstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char posmgxpos = *(PackData+offset);
						    offset += 1;
							int posmgxval = posmgxpos;CString posmgxposstr=_T("");posmgxposstr.Format(L"%d",posmgxval);
						    m_ListCtl.SetItemText(j,27,posmgxposstr);
						}
					}
					//11λ����ߺ����
					if(attr&0x0800)
					{
					    //���                 int
					     //�޷�������������       int
					    if(PackLength>offset)
					    {
							int wfxmgx = *(int*)(PackData+offset);
							offset += 4;
							CString wfxmgxstr = _T("");wfxmgxstr.Format(L"%d",wfxmgx);
							m_ListCtl.SetItemText(j,28,wfxmgxstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char wfxmgxpos = *(PackData+offset);
						    offset += 1;
							int wfxmgxval = wfxmgxpos;CString wfxmgxposstr=_T("");wfxmgxposstr.Format(L"%d",wfxmgxval);
						    m_ListCtl.SetItemText(j,29,wfxmgxposstr);
						}
					}
					//12λ������
					if(attr&0x1000)
					{
					    //�ܸ���                 int
					    if(PackLength>offset)
					    {
							int ggl = *(int*)(PackData+offset);
							offset += 4;
							CString gglstr = _T("");gglstr.Format(L"%d",ggl);
							m_ListCtl.SetItemText(j,30,gglstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char gglpos = *(PackData+offset);
						    offset += 1;
							int gglposval = gglpos;CString gglposstr=_T("");gglposstr.Format(L"%d",gglposval);
						    m_ListCtl.SetItemText(j,31,gglposstr);
						}
					}
					//13λ������
					if(attr&0x2000)
					{
					    //ʵ��(��Ч)�ܸ���       int
					    if(PackLength>offset)
					    {
							int sjggl = *(int*)(PackData+offset);
							offset += 4;
							CString sjgglstr = _T("");sjgglstr.Format(L"%d",sjggl);
							m_ListCtl.SetItemText(j,32,sjgglstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char sjgglpos = *(PackData+offset);
						    offset += 1;
							int sjgglposval = sjgglpos;CString sjgglposstr=_T("");sjgglposstr.Format(L"%d",sjgglposval);
						    m_ListCtl.SetItemText(j,33,sjgglposstr);
						}
					}
					//14λ���ɽ��� 
					if(attr&0x4000)
					{
					     //�����                  int
					    if(PackLength>offset)
					    {
							int yjl = *(int*)(PackData+offset);
							offset += 4;
							CString yjlstr = _T("");yjlstr.Format(L"%d",yjl);
							m_ListCtl.SetItemText(j,34,yjlstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char yjlpos = *(PackData+offset);
						    offset += 1;
							int yjlposval = yjlpos;CString yjlposstr=_T("");yjlposstr.Format(L"%d",yjlposval);
						    m_ListCtl.SetItemText(j,35,yjlposstr);
						}
					}
					//15λ������ 
					if(attr&0x8000)
					{
					     //ʱ���ֵ                int
					    if(PackLength>offset)
					    {
							int sjjz = *(int*)(PackData+offset);
							offset += 4;
							CString sjjzstr = _T("");sjjzstr.Format(L"%d",sjjz);
							m_ListCtl.SetItemText(j,36,sjjzstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char sjjzpos = *(PackData+offset);
						    offset += 1;
							int sjjzposval = sjjzpos;CString sjjzposstr=_T("");sjjzposstr.Format(L"%d",sjjzposval);
						    m_ListCtl.SetItemText(j,37,sjjzposstr);
						}
					}
					//16λ���ɽ��� 
					if(attr&0x10000)
					{
					     //���ڼ�ֵ                int
					    if(PackLength>offset)
					    {
							int nzjz = *(int*)(PackData+offset);
							offset += 4;
							CString nzjzstr = _T("");nzjzstr.Format(L"%d",nzjz);
							m_ListCtl.SetItemText(j,38,nzjzstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char nzjzpos = *(PackData+offset);
						    offset += 1;
							int nzjzposval = nzjzpos;CString nzjzposstr=_T("");nzjzposstr.Format(L"%d",nzjzposval);
						    m_ListCtl.SetItemText(j,39,nzjzposstr);
						}
					}
					//17λ������ 
					if(attr&0x20000)
					{
					     //ʱ��ֵ����              int
					    if(PackLength>offset)
					    {
							int sjzhs = *(int*)(PackData+offset);
							offset += 4;
							CString sjzhsstr = _T("");sjzhsstr.Format(L"%d",sjzhs);
							m_ListCtl.SetItemText(j,40,sjzhsstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char sjzhspos = *(PackData+offset);
						    offset += 1;
							int sjzhsposval = sjzhspos;CString sjzhsposstr=_T("");sjzhsposstr.Format(L"%d",sjzhsposval);
						    m_ListCtl.SetItemText(j,41,sjzhsposstr);
						}
					}
					//18λ���ֲ��� 
					if(attr&0x40000)
					{
						//ֵ����                  byte  //ʵֵ:1 ��ֵ:2 ƽֵ:3
						if(PackLength>offset)
					    {
						    unsigned char valtype = *(PackData+offset);
						    offset += 1;
							int valtypeval = valtype;CString valtypestr=_T("");valtypestr.Format(L"%d",valtypeval);
						    m_ListCtl.SetItemText(j,42,valtypestr);
						}
					     //ʵ�����ֵ               int
					    if(PackLength>offset)
					    {
							int sxjtz = *(int*)(PackData+offset);
							offset += 4;
							CString sxjtzstr = _T("");sxjtzstr.Format(L"%d",sxjtz);
							m_ListCtl.SetItemText(j,43,sxjtzstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char sxjtzpos = *(PackData+offset);
						    offset += 1;
							int sxjtzposval = sxjtzpos;CString sxjtzposstr=_T("");sxjtzposstr.Format(L"%d",sxjtzposval);
						    m_ListCtl.SetItemText(j,44,sxjtzposstr);
						}
					}
					//19λ������� 
					if(attr&0x80000)
					{
					     //�껯����ʱ����           int
					    if(PackLength>offset)
					    {
							int nhdq = *(int*)(PackData+offset);
							offset += 4;
							CString nhdqstr = _T("");nhdqstr.Format(L"%d",nhdq);
							m_ListCtl.SetItemText(j,45,nhdqstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char nhdqpos = *(PackData+offset);
						    offset += 1;
							int nhdqposval = nhdqpos;CString nhdqposstr=_T("");nhdqposstr.Format(L"%d",nhdqposval);
						    m_ListCtl.SetItemText(j,46,nhdqposstr);
						}
					}
					//20λ����һ�ۺ���һ�� 
					if(attr&0x100000)
					{
					     //��Ȩ�۸�                 int
					    if(PackLength>offset)
					    {
							int xqjg = *(int*)(PackData+offset);
							offset += 4;
							CString xqjgstr = _T("");xqjgstr.Format(L"%d",xqjg);
							m_ListCtl.SetItemText(j,47,xqjgstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char xqjgpos = *(PackData+offset);
						    offset += 1;
							int xqjgposval = xqjgpos;CString xqjgposstr=_T("");xqjgposstr.Format(L"%d",xqjgposval);
						    m_ListCtl.SetItemText(j,48,xqjgposstr);
						}
					}
					//21λ����һ������һ�� 
					if(attr&0x200000)
					{
					    //�޷�������              int
					    if(PackLength>offset)
					    {
							int wfxllval = *(int*)(PackData+offset);
							offset += 4;
							CString wfxllvalstr = _T("");wfxllvalstr.Format(L"%d",wfxllval);
							m_ListCtl.SetItemText(j,49,wfxllvalstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char wfxllvalpos = *(PackData+offset);
						    offset += 1;
							int wfxllvalval = wfxllvalpos;CString wfxllvalposstr=_T("");wfxllvalposstr.Format(L"%d",wfxllvalval);
						    m_ListCtl.SetItemText(j,50,wfxllvalposstr);
						}
					}
					//22λ�������ۺ������ 
					if(attr&0x400000)
					{
					    //��ʷ������              int
					    if(PackLength>offset)
					    {
							int lsbdl = *(int*)(PackData+offset);
							offset += 4;
							CString lsbdlstr = _T("");lsbdlstr.Format(L"%d",lsbdl);
							m_ListCtl.SetItemText(j,51,lsbdlstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char lsbdlpos = *(PackData+offset);
						    offset += 1;
							int lsbdlval = lsbdlpos;CString lsbdlposstr=_T("");lsbdlposstr.Format(L"%d",lsbdlval);
						    m_ListCtl.SetItemText(j,52,lsbdlposstr);
						}
					}
					//23λ��������������� 
					if(attr&0x800000)
					{
					     //����������              int
					    if(PackLength>offset)
					    {
							int fxsyl = *(int*)(PackData+offset);
							offset += 4;
							CString fxsylstr = _T("");fxsylstr.Format(L"%d",fxsyl);
							m_ListCtl.SetItemText(j,53,fxsylstr);
						}
						//ʱ��������С��λ  byte
						if(PackLength>offset)
					    {
						    unsigned char fxsylpos = *(PackData+offset);
						    offset += 1;
							int fxsylposval = fxsylpos;CString fxsylposstr=_T("");fxsylposstr.Format(L"%d",fxsylposval);
						    m_ListCtl.SetItemText(j,54,fxsylposstr);
						}
					}
					//24λ�������ۺ������� 
					if(attr&0x1000000)
					{
					    //����                    byte
					    if(PackLength>offset)
					    {
							unsigned char bcval = *(PackData+offset);
						    offset += 1;
							int bcvalval = bcval;CString bcvalstr=_T("");bcvalstr.Format(L"%d",bcvalval);
						    m_ListCtl.SetItemText(j,55,bcvalstr);
						}
					}
					//25λ���������������� 
					if(attr&0x2000000)
					{
					    //Ȩ����                  byte  //��Ȩ1/��Ȩ-1
					    if(PackLength>offset)
					    {
							char qtype = *(PackData+offset);
						    offset += 1;
							int qtypeval = qtype;CString qtypestr=_T("");qtypestr.Format(L"%d",qtypeval);
						    m_ListCtl.SetItemText(j,56,qtypestr);
						}
					}
					//26λ�����ļۺ����ļ� 
					if(attr&0x4000000)
					{
					    //��Ȩ��                   int
					    if(PackLength>offset)
					    {
							unsigned int xqdate = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString xqdatestr = _T("");xqdatestr.Format(L"%u",xqdate);
							m_ListCtl.SetItemText(j,57,xqdatestr);
						}
					}
					//27λ���������������� 
					if(attr&0x8000000)
					{
					    //��һ��                   int
					    if(PackLength>offset)
					    {
							unsigned int sell4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell4valstr = _T("");sell4valstr.Format(L"%u",sell4val);
							m_ListCtl.SetItemText(j,58,sell4valstr);
						}
						//��һ��                   int
					    if(PackLength>offset)
					    {
							unsigned int buy4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy4valstr = _T("");buy4valstr.Format(L"%u",buy4val);
							m_ListCtl.SetItemText(j,59,buy4valstr);
						}
					}
					//28λ������ۺ������ 
					if(attr&0x10000000)
					{
					    //��һ��                   int
					    if(PackLength>offset)
					    {
							unsigned int sell5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell5pricestr = _T("");sell5pricestr.Format(L"%u",sell5price);
							m_ListCtl.SetItemText(j,60,sell5pricestr);
						}
						//��һ��                   int
					    if(PackLength>offset)
					    {
							unsigned int buy5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy5pricestr = _T("");buy5pricestr.Format(L"%u",buy5price);
							m_ListCtl.SetItemText(j,61,buy5pricestr);
						}
					}
					//29λ���������������� 
					if(attr&0x20000000)
					{
					    //�ֲ���                   int
					    if(PackLength>offset)
					    {
							unsigned int holdval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString holdvalstr = _T("");holdvalstr.Format(L"%u",holdval);
							m_ListCtl.SetItemText(j,62,holdvalstr);
						}
					}
					//30λ��������ȯ���  //2015-3-13���� 
					if(attr&0x40000000)
					{
					    //�����                   int
					    if(PackLength>offset)
					    {
							unsigned int settleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString settlepricestr = _T("");settlepricestr.Format(L"%u",settleprice);
							m_ListCtl.SetItemText(j,63,settlepricestr);
						}
					}
					//31λ����Ϣ���ױ��  //2015-3-13����
					if(attr&0x80000000)
					{
						//������                 int
						if(PackLength>offset)
					    {
							unsigned int ysettleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString ysettlepricestr = _T("");ysettlepricestr.Format(L"%u",ysettleprice);
							m_ListCtl.SetItemText(j,64,ysettlepricestr);
						}
					}
				}
			}
				
		}break;
	case 2990:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
		
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�б�Χ       short
			short listrange = 0;
			if(PackLength>offset)
			{
				listrange = *(short*)(PackData+offset);
				offset += 2;
				CString range = _T("");range.Format(L"%d",listrange);m_ListCtl.SetItemText(0,1,range);
			}
			//�б��ֶ�����	short	���ֶζ��������˵��
			unsigned int listattr = 0;
			if(PackLength>offset)
			{
				listattr = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString listattrStr = _T("");listattrStr.Format(L"%d",listattr);
				m_ListCtl.SetItemText(0,2,listattrStr);
			}
			//�ܼ�¼��	short	�������˸������ܼ�¼��
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,3,tolnumStr);
			}
			//��¼��         short	���м�¼��
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				CString RecdNumStr = _T("");RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,4,RecdNumStr);
			}
			if(RecdNum>0)
			{
			for(int i=0; i<RecdNum; i++)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval = _T("");xhval.Format(L"%d",i+2);
			    m_ListCtl.SetItemText(i+1,0,xhval);
			}
			}
			if(RecdNum>0)
			{
			for(int j=0; j<RecdNum; j++)
			{
				//����           String
				if(PackLength>offset)
				{
					short codelen = *(short*)(PackData+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackData+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(j,5,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLength>offset)
				{
					short namelen = *(short*)(PackData+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackData+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(j,6,namestr.c_str());
					delete []name;
				}
				//С��λ��       byte
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointval = pointpos;CString pointstr=_T("");pointstr.Format(L"%d",pointval);
					m_ListCtl.SetItemText(j,7,pointstr);
				}
				//��Ʊ����       byte
				if(PackLength>offset)
				{
					unsigned char stocktype = *(PackData+offset);
					offset += 1;
					int stocktypeval = stocktype;CString stocktypestr=_T("");stocktypestr.Format(L"%d",stocktypeval);
					m_ListCtl.SetItemText(j,8,stocktypestr);
				}
				//���ռ�         int
				if(PackLength>offset)
				{
					int zsprice = *(int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(j,9,zspricestr);
				}
				//���̼�         int
				if(PackLength>offset)
				{
					int openprice = *(int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(j,10,openpricestr);
				}
				//���¼�         int
				if(PackLength>offset)
				{
					int newprice = *(int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(j,11,newpricestr);
				}
				//��߼�         int
				if(PackLength>offset)
				{
					int highprice = *(int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(j,12,highpricestr);
				}
				//��ͼ�         int
				if(PackLength>offset)
				{
					int lowprice = *(int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(j,13,lowpricestr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					int cjmoney = *(int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(j,14,cjmoneystr);
				}
				//������ָ���ɷֹɵ�id    short
				if(listrange==105||listrange==113||listrange==114)
				{
				    if(PackLength>offset)
				    {
					    short indexid = *(short*)(PackData+offset);
				        offset += 2;
					    CString indexidstr = _T("");indexidstr.Format(L"%u",indexid);
					    m_ListCtl.SetItemText(j,15,indexidstr);
				    }
				}
				//int position = attrutitude[0];
				if(listattr!=0&&listattr&(0x0001))
				{
					//�ɽ���λ	  short
				    if(PackLength>offset)
				    {
					    short cjpos = *(short*)(PackData+offset);
				        offset += 2;
					    CString cjposstr = _T("");cjposstr.Format(L"%u",cjpos);
					    m_ListCtl.SetItemText(j,16,cjposstr);
				    }
					//�ɽ���         int
				    if(PackLength>offset)
				    {
					    unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					    m_ListCtl.SetItemText(j,17,cjvaluestr);
				    }
				}
				if(listattr!=0&&listattr&(0x0002))
				{
					//���̳ɽ���     int       ------1λ
					if(PackLength>offset)
				    {
					    unsigned int innerval = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString innervalstr = _T("");innervalstr.Format(L"%u",innerval);
					    m_ListCtl.SetItemText(j,18,innervalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0004))
				{
					//����           int       ------2λ
					if(PackLength>offset)
				    {
					    unsigned int xsval = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString xsvalstr = _T("");xsvalstr.Format(L"%u",xsval);
					    m_ListCtl.SetItemText(j,19,xsvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0008))
				{
					//����           short��100------3λ
					if(PackLength>offset)
				    {
					    short lbval = *(short*)(PackData+offset);
				        offset += 2;
						float lbvalues = (float)lbval/100;
					    CString lbvalstr = _T("");lbvalstr.Format(L"%3f",lbvalues);
					    m_ListCtl.SetItemText(j,20,lbvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0010))
				{
					//����           short��10000----4λ
					if(PackLength>offset)
				    {
					    unsigned short hsval = *(unsigned short*)(PackData+offset);
				        offset += 2;
						double hsvalue = hsval/10000;
					    CString hsvalstr = _T("");hsvalstr.Format(L"%lf",hsvalue);
					    m_ListCtl.SetItemText(j,21,hsvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0020))
				{
					//����           short��10000----5λ  ��������
					if(PackLength>offset)
				    {
					    short zsval = *(short*)(PackData+offset);
				        offset += 2;
						double zsvalue = zsval/10000;
					    CString zsvalstr = _T("");zsvalstr.Format(L"%lf",zsvalue);
					    m_ListCtl.SetItemText(j,22,zsvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0040))
				{
					//ί��           short��10000----6λ  ��������
					if(PackLength>offset)
				    {
					    short wbval = *(short*)(PackData+offset);
				        offset += 2;
						double wbvalue = wbval/10000;
					    CString wbvalstr = _T("");wbvalstr.Format(L"%lf",wbvalue);
					    m_ListCtl.SetItemText(j,23,wbvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0080))
				{
					//������Ŀ       byte 0��ʾ��----7λ
					if(PackLength>offset)
				    {
					    unsigned char noticeval = *(PackData+offset);
				        offset += 1;
					    int notice = noticeval;CString noticevalstr = _T("");noticevalstr.Format(L"%u",notice);
					    m_ListCtl.SetItemText(j,24,noticevalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0100))
				{
					//��ӯ��	  int��100  ��������
					if(PackLength>offset)
				    {
					    int sylval = *(int*)(PackData+offset);
				        offset += 4;
					    CString sylvalstr = _T("");sylvalstr.Format(L"%d",sylval/100);
					    m_ListCtl.SetItemText(j,25,sylvalstr);
				    }
					//�о���	  int��100  ��������
					if(PackLength>offset)
				    {
					    int sjlval = *(int*)(PackData+offset);
				        offset += 4;
					    CString sjlvalstr = _T("");sjlvalstr.Format(L"%d",sjlval/100);
					    m_ListCtl.SetItemText(j,26,sjlvalstr);
				    }
				}
				if(listattr!=0&&listattr&(0x0200))
				{
					//��һ		  int
					if(PackLength>offset)
				    {
					    unsigned int sell = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString sellstr = _T("");sellstr.Format(L"%u",sell);
					    m_ListCtl.SetItemText(j,27,sellstr);
				    }
					//��һ		  int
					if(PackLength>offset)
				    {
					    int buy = *(int*)(PackData+offset);
				        offset += 4;
					    CString buystr = _T("");buystr.Format(L"%u",buy);
					    m_ListCtl.SetItemText(j,28,buystr);
				    }
				}
				//ͳ���ֶ�----------10λ
				if(listattr!=0&&listattr&(0x0400))
				{
					//7���Ƿ�	  int��10000  ��������
					if(PackLength>offset)
				    {
					    int zindex = *(int*)(PackData+offset);
				        offset += 4;
						double zindexval = zindex/10000;
					    CString zindexstr = _T("");zindexstr.Format(L"%lf",zindexval);
					    m_ListCtl.SetItemText(j,29,zindexstr);
				    }
					//7�ջ���	  int��10000
					if(PackLength>offset)
				    {
					    unsigned int sevenval = *(unsigned int*)(PackData+offset);
				        offset += 4;
						double sevenvalue = sevenval/10000;
					    CString sevenvalstr = _T("");sevenvalstr.Format(L"%lf",sevenvalue);
					    m_ListCtl.SetItemText(j,30,sevenvalstr);
				    }
					//30���Ƿ�	  int��10000  ��������
					if(PackLength>offset)
				    {
					    int z30index = *(int*)(PackData+offset);
				        offset += 4;
						double z30indexval = z30index/10000;
					    CString z30indexstr = _T("");z30indexstr.Format(L"%lf",z30indexval);
					    m_ListCtl.SetItemText(j,31,z30indexstr);
				    }
					//30�ջ���	  int��10000
					if(PackLength>offset)
				    {
					    unsigned int thirtyval = *(unsigned int*)(PackData+offset);
				        offset += 4;
						double thirtyvalval = thirtyval/10000;
					    CString thirtyvalstr = _T("");thirtyvalstr.Format(L"%lf",thirtyvalval);
					    m_ListCtl.SetItemText(j,32,thirtyvalstr);
				    }
				}
				//level2ͳ���ֶ�--11λ
				if(listattr!=0&&listattr&(0x0800))
				{
					//����ddx	  short��1000  ��������
					if(PackLength>offset)
				    {
					    short dddx = *(short*)(PackData+offset);
				        offset += 2;
						double dddxval = dddx/1000;
					    CString dddxstr = _T("");dddxstr.Format(L"%lf",dddxval);
					    m_ListCtl.SetItemText(j,33,dddxstr);
				    }
					//����ddy	  short��1000  ��������
					if(PackLength>offset)
				    {
					    short dddy = *(short*)(PackData+offset);
				        offset += 2;
						double dddyval = dddy/1000;
					    CString dddystr = _T("");dddystr.Format(L"%lf",dddyval);
					    m_ListCtl.SetItemText(j,34,dddystr);
				    }
					//����ddz	  int��1000  ��������
					if(PackLength>offset)
				    {
					    int dddz = *(int*)(PackData+offset);
				        offset += 4;
						double dddzval = dddz/1000;
					    CString dddzstr = _T("");dddzstr.Format(L"%lf",dddzval);
					    m_ListCtl.SetItemText(j,35,dddzstr);
				    }
					//60��ddx	  int��1000  ��������
					if(PackLength>offset)
				    {
					    int sixddxval = *(int*)(PackData+offset);
				        offset += 4;
						double sixddxvalue = sixddxval/1000;
					    CString sixddxvalstr = _T("");sixddxvalstr.Format(L"%lf",sixddxvalue);
					    m_ListCtl.SetItemText(j,36,sixddxvalstr);
				    }
					//60��ddy	  int��1000  ��������
					if(PackLength>offset)
				    {
					    int sixddyval = *(int*)(PackData+offset);
				        offset += 4;
						double sixddyvalue = sixddyval/1000;
					    CString sixddyvalstr = _T("");sixddyvalstr.Format(L"%lf",sixddyvalue);
					    m_ListCtl.SetItemText(j,37,sixddyvalstr);
				    }
					//10��ddx��ɫ������ char
					if(PackLength>offset)
				    {
					    char ddxredval = *(PackData+offset);
				        offset += 1;
					    int ddxred = ddxredval;CString ddxredvalstr = _T("");ddxredvalstr.Format(L"%u",ddxred);
					    m_ListCtl.SetItemText(j,38,ddxredvalstr);
				    }
					//10��ddx������ɫ�� char
					if(PackLength>offset)
				    {
					    char ddxredcval = *(PackData+offset);
				        offset += 1;
					    int ddxredc = ddxredcval;CString ddxredcvalstr = _T("");ddxredcvalstr.Format(L"%u",ddxredc);
					    m_ListCtl.SetItemText(j,39,ddxredcvalstr);
				    }
				}
				////level2ͳ���ֶ�--12λ, 20111020����
				if(listattr!=0&&listattr&(0x1000))
				{
					//�����ʽ�����     int         //�ʽ𾻶�=�ʽ�����-�ʽ�����
					if(PackLength>offset)
				    {
					    unsigned int incap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString incapstr = _T("");incapstr.Format(L"%u",incap);
					    m_ListCtl.SetItemText(j,40,incapstr);
				    }
					//�����ʽ�����     int 
					if(PackLength>offset)
				    {
					    unsigned int outcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString outcapstr = _T("");outcapstr.Format(L"%u",outcap);
					    m_ListCtl.SetItemText(j,41,outcapstr);
				    }
					//5���ʽ�����      int
					if(PackLength>offset)
				    {
					    unsigned int fiveincap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString fiveincapstr = _T("");fiveincapstr.Format(L"%u",fiveincap);
					    m_ListCtl.SetItemText(j,42,fiveincapstr);
				    }
					//5���ʽ�����      int 
					if(PackLength>offset)
				    {
					    unsigned int fiveoutcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString fiveoutcapstr = _T("");fiveoutcapstr.Format(L"%u",fiveoutcap);
					    m_ListCtl.SetItemText(j,43,fiveoutcapstr);
				    }
					//5���ʽ�ɽ���    int 
					if(PackLength>offset)
				    {
					    unsigned int fivecjcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString fivecjcapstr = _T("");fivecjcapstr.Format(L"%u",fivecjcap);
					    m_ListCtl.SetItemText(j,44,fivecjcapstr);
				    }
					//30���ʽ�����     int
					if(PackLength>offset)
				    {
					    unsigned int thirtyincap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString thirtyincapstr = _T("");thirtyincapstr.Format(L"%u",thirtyincap);
					    m_ListCtl.SetItemText(j,45,thirtyincapstr);
				    }
					//30���ʽ�����      int 
					if(PackLength>offset)
				    {
					    unsigned int thirtyoutcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString thirtyoutcapstr = _T("");thirtyoutcapstr.Format(L"%u",thirtyoutcap);
					    m_ListCtl.SetItemText(j,46,thirtyoutcapstr);
				    }
					//30���ʽ�ɽ���    int 
					if(PackLength>offset)
				    {
					    unsigned int thirtycjcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString thirtycjcapstr = _T("");thirtycjcapstr.Format(L"%u",thirtycjcap);
					    m_ListCtl.SetItemText(j,47,thirtycjcapstr);
				    }
				}
				//��Ʒ����������--13λ, 20120307����
				if(listattr!=0&&listattr&(0x2000))
				{
					//���ճֲ���       int
					if(PackLength>offset)
				    {
					    unsigned int zhold = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString zholdstr = _T("");zholdstr.Format(L"%u",zhold);
					    m_ListCtl.SetItemText(j,48,zholdstr);
				    }
					//���ս����       int 
					if(PackLength>offset)
				    {
					    unsigned int zsettle = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString zsettlestr = _T("");zsettlestr.Format(L"%u",zsettle);
					    m_ListCtl.SetItemText(j,49,zsettlestr);
				    }
					//�ֲ���           int
					if(PackLength>offset)
				    {
					    unsigned int hold = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString holdstr = _T("");holdstr.Format(L"%u",hold);
					    m_ListCtl.SetItemText(j,50,holdstr);
				    }
					//�����           int 
					if(PackLength>offset)
				    {
					    unsigned int settlecap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString settlecapstr = _T("");settlecapstr.Format(L"%u",settlecap);
					    m_ListCtl.SetItemText(j,51,settlecapstr);
				    }
				}
				//level2�������---14, 20120508
				if(listattr!=0&&listattr&(0x4000))
				{
					//��������       short  //�޷���
					if(PackLength>offset)
				    {
					    unsigned short jgval = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString jgvalstr = _T("");jgvalstr.Format(L"%u",jgval);
					    m_ListCtl.SetItemText(j,52,jgvalstr);
				    }
					//����������       short  //�޷���
					if(PackLength>offset)
				    {
					    unsigned short jgvalsell = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString jgvalsellstr = _T("");jgvalsellstr.Format(L"%u",jgvalsell);
					    m_ListCtl.SetItemText(j,53,jgvalsellstr);
				    }
					//�����Ի���       short  //�޷���
					if(PackLength>offset)
				    {
					    unsigned short eathold = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString eatholdstr = _T("");eatholdstr.Format(L"%u",eathold);
					    m_ListCtl.SetItemText(j,54,eatholdstr);
				    }
					//�����»���       short  //�޷���
					if(PackLength>offset)
				    {
					    unsigned short jgtcap = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString jgtcapstr = _T("");jgtcapstr.Format(L"%u",jgtcap);
					    m_ListCtl.SetItemText(j,55,jgtcapstr);
				    }
					//�����Ի���       int
					if(PackLength>offset)
				    {
					    unsigned int eatval = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString eatvalstr = _T("");eatvalstr.Format(L"%u",eatval);
					    m_ListCtl.SetItemText(j,56,eatvalstr);
				    }
					//�����»���       int 
					if(PackLength>offset)
				    {
					    unsigned int sellvalue = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString sellvaluestr = _T("");sellvaluestr.Format(L"%u",sellvalue);
					    m_ListCtl.SetItemText(j,57,sellvaluestr);
				    }
				}
				//������ȯ���λ-------------15   20130717 
				if(listattr!=0&&listattr&(0x8000))
				{
					//�ù��Ƿ���������ȯ��� char   1�ǣ�0��
					if(PackLength>offset)
				    {
					    char rzrqtag = *(PackData+offset);
				        offset += 1;
						int rzrqtagval = rzrqtag;
					    CString rzrqtagstr = _T("");rzrqtagstr.Format(L"%u",rzrqtag);
					    m_ListCtl.SetItemText(j,58,rzrqtagstr);
				    }
				}
				//Ͷ�˱��   -------16λ  Ͷ�˱��      short
				if(listattr!=0&&listattr&(0x10000))
				{
					//Ͷ�˱��      short
					if(PackLength>offset)
				    {
					    short tgtype = *(short*)(PackData+offset);
				        offset += 2;
					    CString tgtypestr = _T("");tgtypestr.Format(L"%d",tgtype);
					    m_ListCtl.SetItemText(j,59,tgtypestr);
				    }
				}
				//֤ȯ���   -------17λ
				if(listattr!=0&&listattr&(0x20000))
				{
					//֤ȯ���      short		��16λ��������ʾ֤ȯֻ��2��״̬�ĸ������
					if(PackLength>offset)
				    {
					    unsigned short stocktype = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString stocktypestr = _T("");stocktypestr.Format(L"%d",stocktype);
					    m_ListCtl.SetItemText(j,60,stocktypestr);
				    }
				}
				//����ǵ�����   -------18λ�����ֶν�������г��У�2015-9-29����
				if(listattr!=0&&listattr&(0x40000))
				{
					//���Ǽ���     short
					if(PackLength>offset)
				    {
					    unsigned short szjs = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString szjsstr = _T("");szjsstr.Format(L"%d",szjs);
					    m_ListCtl.SetItemText(j,61,szjsstr);
				    }
					//�µ�����     short
					if(PackLength>offset)
				    {
					    unsigned short xdjs = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString xdjsstr = _T("");xdjsstr.Format(L"%d",xdjs);
					    m_ListCtl.SetItemText(j,62,xdjsstr);
				    }
					//ƽ�̼���     short
					if(PackLength>offset)
				    {
					    unsigned short ppjs = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString ppjsstr = _T("");ppjsstr.Format(L"%d",ppjs);
					    m_ListCtl.SetItemText(j,63,ppjsstr);
				    }
				}
				//����ǵ�����   -------19λ�����ֶν�������г��У�2015-10-9����
				if(listattr!=0&&listattr&(0x80000))
				{
					//��ͨ��ֵ     int  ��λ��Ԫ
					if(PackLength>offset)
				    {
					    int ltsz = *(int*)(PackData+offset);
				        offset += 4;
					    CString ltszstr = _T("");ltszstr.Format(L"%d",ltsz);
					    m_ListCtl.SetItemText(j,64,ltszstr);
				    }
					//����ֵ       int  ��λ��Ԫ
					if(PackLength>offset)
				    {
					    unsigned int zszval = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString zszvalstr = _T("");zszvalstr.Format(L"%u",zszval);
					    m_ListCtl.SetItemText(j,65,zszvalstr);
				    }
				}
				//������Ǹ���   -------20λ�����ֶν�������г��У�2015-10-9����
				if(listattr!=0&&listattr&(0x100000))
				{
					//����           String
					if(PackLength>offset)
					{
						short codelen = *(short*)(PackData+offset);
						offset += 2;
						char *code = new char[codelen+1];
						memset(code,0,codelen+1);
						memcpy(code,PackData+offset,codelen);
						offset += codelen;
						wstring codestr = UTF8_2_UTF16(code);
						m_ListCtl.SetItemText(j,66,codestr.c_str());
						delete []code;
					}
					//����           String
					if(PackLength>offset)
					{
						short namelen = *(short*)(PackData+offset);
						offset += 2;
						char *name = new char[namelen+1];
						memset(name,0,namelen+1);
						memcpy(name,PackData+offset,namelen);
						offset += namelen;
						wstring namestr = UTF8_2_UTF16(name);
						m_ListCtl.SetItemText(j,67,namestr.c_str());
						delete []name;
					}
					//С��λ��       byte	//���ռۡ����¼ۡ����ߵͼ�С��λ
					if(PackLength>offset)
					{
						unsigned char pointpos = *(PackData+offset);
						offset += 1;
					    int pointposval = pointpos;CString pointposstr=_T("");pointposstr.Format(L"%d",pointposval);
						m_ListCtl.SetItemText(j,68,pointposstr);
					}
					//���¼�         int   
					if(PackLength>offset)
				    {
					    unsigned int newprice = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
					    m_ListCtl.SetItemText(j,69,newpricestr);
				    }
					//�Ƿ�           int*10000
					if(PackLength>offset)
				    {
					    int zfvalue = *(int*)(PackData+offset);
				        offset += 4;
					    CString zfvaluestr = _T("");zfvaluestr.Format(L"%u",zfvalue);
					    m_ListCtl.SetItemText(j,70,zfvaluestr);
				    }
				}
				////����������   -------21λ�����ֶν�������г��У�ע���λ��20����ͬʱ���ڣ�2015-10-9����
				if(listattr!=0&&listattr&(0x100000))
				{
					//����           String
					if(PackLength>offset)
					{
						short code1len = *(short*)(PackData+offset);
						offset += 2;
						char *code1 = new char[code1len+1];
						memset(code1,0,code1len+1);
						memcpy(code1,PackData+offset,code1len);
						offset += code1len;
						wstring code1str = UTF8_2_UTF16(code1);
						m_ListCtl.SetItemText(j,71,code1str.c_str());
						delete []code1;
					}
					//����           String
					if(PackLength>offset)
					{
						short name1len = *(short*)(PackData+offset);
						offset += 2;
						char *name1 = new char[name1len+1];
						memset(name1,0,name1len+1);
						memcpy(name1,PackData+offset,name1len);
						offset += name1len;
						wstring name1str = UTF8_2_UTF16(name1);
						m_ListCtl.SetItemText(j,72,name1str.c_str());
						delete []name1;
					}
					//С��λ��       byte	//���ռۡ����¼ۡ����ߵͼ�С��λ
					if(PackLength>offset)
					{
						unsigned char pointpos1 = *(PackData+offset);
						offset += 1;
					    int pointpos1val = pointpos1;CString pointpos1str=_T("");pointpos1str.Format(L"%d",pointpos1val);
						m_ListCtl.SetItemText(j,73,pointpos1str);
					}
					//���¼�         int   
					if(PackLength>offset)
				    {
					    unsigned int newprice1 = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString newprice1str = _T("");newprice1str.Format(L"%d",newprice1);
					    m_ListCtl.SetItemText(j,74,newprice1str);
				    }
					//�Ƿ�           int*10000
					if(PackLength>offset)
				    {
					    int zfvalue1 = *(int*)(PackData+offset);
				        offset += 4;
					    CString zfvalue1str = _T("");zfvalue1str.Format(L"%u",zfvalue1);
					    m_ListCtl.SetItemText(j,75,zfvalue1str);
				    }
				}
			}
			}
		}break;
	case 2991:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
		
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ܼ�¼��	short	�������˸������ܼ�¼��
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumStr);
			}
			//��¼��         short	���м�¼��
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				CString RecdNumStr = _T("");RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
			}
			int i = 0;
			while(PackLength-offset>0)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval = _T("");xhval.Format(L"%d",i+2);
			    m_ListCtl.SetItemText(i+1,0,xhval);
				//����           String
				if(PackLength>offset)
				{
					short codelen = *(short*)(PackData+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackData+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(i,3,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLength>offset)
				{
					short namelen = *(short*)(PackData+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackData+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(i,4,namestr.c_str());
					delete []name;
				}
				//���ٵ�ָ��     String
				if(PackLength>offset)
				{
					short gzindexlen = *(short*)(PackData+offset);
					offset += 2;
					char *gzindex = new char[gzindexlen+1];
					memset(gzindex,0,gzindexlen+1);
					memcpy(gzindex,PackData+offset,gzindexlen);
					offset += gzindexlen;
					wstring gzindexstr = UTF8_2_UTF16(gzindex);
					m_ListCtl.SetItemText(i,5,gzindexstr.c_str());
					delete []gzindex;
				}
				//ĸ������       String
				if(PackLength>offset)
				{
					short fundcodelen = *(short*)(PackData+offset);
					offset += 2;
					char *fundcode = new char[fundcodelen+1];
					memset(fundcode,0,fundcodelen+1);
					memcpy(fundcode,PackData+offset,fundcodelen);
					offset += fundcodelen;
					wstring fundcodestr = UTF8_2_UTF16(fundcode);
					m_ListCtl.SetItemText(i,6,fundcodestr.c_str());
					delete []fundcode;
				}
				//�۸�С����λ�� char
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointposval = pointpos;CString pointstr = _T("");pointstr.Format(L"%d",pointposval);
					m_ListCtl.SetItemText(i,7,pointstr);
				}
				//���ռ�         int
				if(PackLength>offset)
				{
					unsigned int zsprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(i,8,zspricestr);
				}
				//���̼�         int
				if(PackLength>offset)
				{
					unsigned int openprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(i,9,openpricestr);
				}
				//���¼�         int
				if(PackLength>offset)
				{
					unsigned int newprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(i,10,newpricestr);
				}
				//��߼�         int
				if(PackLength>offset)
				{
					unsigned int highprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(i,11,highpricestr);
				}
				//��ͼ�         int
				if(PackLength>offset)
				{
					unsigned int lowprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(i,12,lowpricestr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					unsigned int cjmoney = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(i,13,cjmoneystr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					m_ListCtl.SetItemText(i,14,cjvaluestr);
				}
				//�۸�ܸ�       int*1000
				if(PackLength>offset)
				{
					unsigned int jggg = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString jgggstr = _T("");jgggstr.Format(L"%u",jggg);
					m_ListCtl.SetItemText(i,15,jgggstr);
				}
				//�������       int*1000
				if(PackLength>offset)
				{
					int ztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString ztyjstr = _T("");ztyjstr.Format(L"%u",ztyj);
					m_ListCtl.SetItemText(i,16,ztyjstr);
				}
				//���ڷݶ�       int
				if(PackLength>offset)
				{
					unsigned int cnfe = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cnfestr = _T("");cnfestr.Format(L"%u",cnfe);
					m_ListCtl.SetItemText(i,17,cnfestr);
				}
				//��ʼ�ܸ�       int*1000
				if(PackLength>offset)
				{
					int csgg = *(int*)(PackData+offset);
				    offset += 4;
					CString csggstr = _T("");csggstr.Format(L"%u",csgg);
					m_ListCtl.SetItemText(i,18,csggstr);
				}
				//ָ���Ƿ�       short*100
				if(PackLength>offset)
				{
					short zszf = *(short*)(PackData+offset);
				    offset += 2;
					CString zszfstr = _T("");zszfstr.Format(L"%u",zszf);
					m_ListCtl.SetItemText(i,19,zszfstr);
				}
				//ĸ��ʵʱ��ֵ   int*10000
				if(PackLength>offset)
				{
					int mjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjssjzstr = _T("");mjjssjzstr.Format(L"%d",mjjssjz);
					m_ListCtl.SetItemText(i,20,mjjssjzstr);
				}
				//ĸ�����վ�ֵ   int*10000
				if(PackLength>offset)
				{
					int mjjzrjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjzrjzstr = _T("");mjjzrjzstr.Format(L"%d",mjjzrjz);
					m_ListCtl.SetItemText(i,21,mjjzrjzstr);
				}
				//����ĸ������   int*100
				if(PackLength>offset)
				{
					int szmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString szmjjxzstr = _T("");szmjjxzstr.Format(L"%d",szmjjxz);
					m_ListCtl.SetItemText(i,22,szmjjxzstr);
				}
				//����ĸ�����   int*100
				if(PackLength>offset)
				{
					int xzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString xzmjjxzstr = _T("");xzmjjxzstr.Format(L"%d",xzmjjxz);
					m_ListCtl.SetItemText(i,23,xzmjjxzstr);
				}
				//����           int*100
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
				    offset += 4;
					CString hsvalstr = _T("");hsvalstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(i,24,hsvalstr);
				}
				//����           int*100
				if(PackLength>offset)
				{
					int lbval = *(int*)(PackData+offset);
				    offset += 4;
					CString lbvalstr = _T("");lbvalstr.Format(L"%d",lbval);
					m_ListCtl.SetItemText(i,25,lbvalstr);
				}
				i += 1;
			}
		}break;
	case 2992:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
		
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ܼ�¼��	short	�������˸������ܼ�¼��
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumStr);
			}
			//��Ŀ           short
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				CString RecdNumStr = _T("");RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
			}
			int i = 0;
			while(PackLength-offset>0)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval = _T("");xhval.Format(L"%d",i+2);
			    m_ListCtl.SetItemText(i+1,0,xhval);
				//����           String
				if(PackLength>offset)
				{
					short codelen = *(short*)(PackData+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackData+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(i,3,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLength>offset)
				{
					short namelen = *(short*)(PackData+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackData+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(i,4,namestr.c_str());
					delete []name;
				}
				//ĸ������       String
				if(PackLength>offset)
				{
					short fundcodelen = *(short*)(PackData+offset);
					offset += 2;
					char *fundcode = new char[fundcodelen+1];
					memset(fundcode,0,fundcodelen+1);
					memcpy(fundcode,PackData+offset,fundcodelen);
					offset += fundcodelen;
					wstring fundcodestr = UTF8_2_UTF16(fundcode);
					m_ListCtl.SetItemText(i,5,fundcodestr.c_str());
					delete []fundcode;
				}
				//�۸�С����λ�� char
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointposval = pointpos;CString pointstr = _T("");pointstr.Format(L"%d",pointposval);
					m_ListCtl.SetItemText(i,6,pointstr);
				}
				//���ռ�         int
				if(PackLength>offset)
				{
					unsigned int zsprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(i,7,zspricestr);
				}
				//���̼�         int
				if(PackLength>offset)
				{
					unsigned int openprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(i,8,openpricestr);
				}
				//���¼�         int
				if(PackLength>offset)
				{
					unsigned int newprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(i,9,newpricestr);
				}
				//��߼�         int
				if(PackLength>offset)
				{
					unsigned int highprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(i,10,highpricestr);
				}
				//��ͼ�         int
				if(PackLength>offset)
				{
					unsigned int lowprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(i,11,lowpricestr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					unsigned int cjmoney = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(i,12,cjmoneystr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					m_ListCtl.SetItemText(i,13,cjvaluestr);
				}
				//�������       int*1000
				if(PackLength>offset)
				{
					unsigned int ztyj = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString ztyjstr = _T("");ztyjstr.Format(L"%u",ztyj);
					m_ListCtl.SetItemText(i,14,ztyjstr);
				}
				//���ڷݶ�       int
				if(PackLength>offset)
				{
					unsigned int cnfe = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cnfestr = _T("");cnfestr.Format(L"%u",cnfe);
					m_ListCtl.SetItemText(i,15,cnfestr);
				}
				//����           int*100
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
				    offset += 4;
					CString hsvalstr = _T("");hsvalstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(i,16,hsvalstr);
				}
				//����           int*100
				if(PackLength>offset)
				{
					int lbval = *(int*)(PackData+offset);
				    offset += 4;
					CString lbvalstr = _T("");lbvalstr.Format(L"%d",lbval);
					m_ListCtl.SetItemText(i,17,lbvalstr);
				}
				//��������       int*1000
				if(PackLength>offset)
				{
					int yhsy = *(int*)(PackData+offset);
				    offset += 4;
					CString yhsystr = _T("");yhsystr.Format(L"%d",yhsy);
					m_ListCtl.SetItemText(i,18,yhsystr);
				}
				//Լ������       int*1000
				if(PackLength>offset)
				{
					int ydsy = *(int*)(PackData+offset);
				    offset += 4;
					CString ydsystr = _T("");ydsystr.Format(L"%d",ydsy);
					m_ListCtl.SetItemText(i,19,ydsystr);
				}
				i += 1;
			}
		}break;
	case 2993:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
	
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//�ܼ�¼��	short	�������˸������ܼ�¼��
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumStr);
			}
			//��Ŀ           short
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				CString RecdNumStr = _T("");RecdNumStr.Format(L"%d",RecdNum);
				m_ListCtl.SetItemText(0,2,RecdNumStr);
			}
			int i = 0;
			while(PackLength-offset>0)
			{
				m_ListCtl.InsertItem(i+1,_T(""));
				CString xhval = _T("");xhval.Format(L"%d",i+2);
			    m_ListCtl.SetItemText(i+1,0,xhval);
				//����           String
				if(PackLength>offset)
				{
					short codelen = *(short*)(PackData+offset);
					offset += 2;
					char *code = new char[codelen+1];
					memset(code,0,codelen+1);
					memcpy(code,PackData+offset,codelen);
					offset += codelen;
					wstring codestr = UTF8_2_UTF16(code);
					m_ListCtl.SetItemText(i,3,codestr.c_str());
					delete []code;
				}
				//����           String
				if(PackLength>offset)
				{
					short namelen = *(short*)(PackData+offset);
					offset += 2;
					char *name = new char[namelen+1];
					memset(name,0,namelen+1);
					memcpy(name,PackData+offset,namelen);
					offset += namelen;
					wstring namestr = UTF8_2_UTF16(name);
					m_ListCtl.SetItemText(i,4,namestr.c_str());
					delete []name;
				}
				//���ٵ�ָ��     String
				if(PackLength>offset)
				{
					short gzindexlen = *(short*)(PackData+offset);
					offset += 2;
					char *gzindex = new char[gzindexlen+1];
					memset(gzindex,0,gzindexlen+1);
					memcpy(gzindex,PackData+offset,gzindexlen);
					offset += gzindexlen;
					wstring gzindexstr = UTF8_2_UTF16(gzindex);
					m_ListCtl.SetItemText(i,5,gzindexstr.c_str());
					delete []gzindex;
				}
				//�۸�С����λ�� char
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointposval = pointpos;CString pointstr = _T("");pointstr.Format(L"%d",pointposval);
					m_ListCtl.SetItemText(i,6,pointstr);
				}
				//���ռ�         int
				if(PackLength>offset)
				{
					unsigned int zsprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(i,7,zspricestr);
				}
				//���̼�         int
				if(PackLength>offset)
				{
					unsigned int openprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(i,8,openpricestr);
				}
				//���¼�         int
				if(PackLength>offset)
				{
					unsigned int newprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(i,9,newpricestr);
				}
				//��߼�         int
				if(PackLength>offset)
				{
					unsigned int highprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(i,10,highpricestr);
				}
				//��ͼ�         int
				if(PackLength>offset)
				{
					unsigned int lowprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(i,11,lowpricestr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					unsigned int cjmoney = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(i,12,cjmoneystr);
				}
				//�ɽ���         int
				if(PackLength>offset)
				{
					unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					m_ListCtl.SetItemText(i,13,cjvaluestr);
				}
				//ĸ��ʵʱ��ֵ   int*10000
				if(PackLength>offset)
				{
					int mjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjssjzstr = _T("");mjjssjzstr.Format(L"%d",mjjssjz);
					m_ListCtl.SetItemText(i,14,mjjssjzstr);
				}
				//ĸ�������     int*1000
				if(PackLength>offset)
				{
					int mjjyj = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjyjstr = _T("");mjjyjstr.Format(L"%d",mjjyj);
					m_ListCtl.SetItemText(i,15,mjjyjstr);
				}
				//ָ���Ƿ�       short*100
				if(PackLength>offset)
				{
					short zszf = *(short*)(PackData+offset);
				    offset += 2;
					CString zszfstr = _T("");zszfstr.Format(L"%u",zszf);
					m_ListCtl.SetItemText(i,16,zszfstr);
				}
				//����           int*100
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
				    offset += 4;
					CString hsvalstr = _T("");hsvalstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(i,17,hsvalstr);
				}
				//����           int*100
				if(PackLength>offset)
				{
					int lbval = *(int*)(PackData+offset);
				    offset += 4;
					CString lbvalstr = _T("");lbvalstr.Format(L"%d",lbval);
					m_ListCtl.SetItemText(i,18,lbvalstr);
				}
				//�깺��         int*100
				if(PackLength>offset)
				{
					int sgfy = *(int*)(PackData+offset);
				    offset += 4;
					CString sgfystr = _T("");sgfystr.Format(L"%d",sgfy);
					m_ListCtl.SetItemText(i,19,sgfystr);
				}
				//��ط�         int*100
				if(PackLength>offset)
				{
					int shfy = *(int*)(PackData+offset);
				    offset += 4;
					CString shfystr = _T("");shfystr.Format(L"%d",shfy);
					m_ListCtl.SetItemText(i,20,shfystr);
				}
				i += 1;
			}
		}break;
	case 2994:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
		
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//��������         char		//0����ʾA����1��B����2��ĸ����3������������
			int fundtypeval = 0;
			if(PackLength>offset)
			{
				char fundtype = *(PackData+offset);
				offset += 1;
				fundtypeval = fundtype;CString fundtypestr = _T("");fundtypestr.Format(L"%d",fundtypeval);
				m_ListCtl.SetItemText(0,1,fundtypestr);
			}
			////Ϊ0��A����ʱΪ�����ֶ� 
			if(fundtypeval == 0)   //��ʾ����A������ֶ�
			{
				//�������       int*1000
				if(PackLength>offset)
				{
					int ztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString ztyjstr = _T("");ztyjstr.Format(L"%u",ztyj);
					m_ListCtl.SetItemText(0,2,ztyjstr);
				}
				//ĸ��ʵʱ��ֵ     int*10000
				if(PackLength>offset)
				{
					int mjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjssjzstr = _T("");mjjssjzstr.Format(L"%d",mjjssjz);
					m_ListCtl.SetItemText(0,3,mjjssjzstr);
				}
				//����ĸ������   int*100
				if(PackLength>offset)
				{
					int szmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString szmjjxzstr = _T("");szmjjxzstr.Format(L"%d",szmjjxz);
					m_ListCtl.SetItemText(0,4,szmjjxzstr);
				}
				//����ĸ�����   int*100
				if(PackLength>offset)
				{
					int xzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString xzmjjxzstr = _T("");xzmjjxzstr.Format(L"%d",xzmjjxz);
					m_ListCtl.SetItemText(0,5,xzmjjxzstr);
				}
				//��������       int*1000
				if(PackLength>offset)
				{
					int yhsy = *(int*)(PackData+offset);
				    offset += 4;
					CString yhsystr = _T("");yhsystr.Format(L"%d",yhsy);
					m_ListCtl.SetItemText(0,6,yhsystr);
				}
			}
			//Ϊ1��B����ʱΪ�����ֶ� 
			if(fundtypeval == 1)   //��ʾ����B������ֶ�
			{
				//�������       int*1000
				if(PackLength>offset)
				{
					int bztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString bztyjstr = _T("");bztyjstr.Format(L"%u",bztyj);
					m_ListCtl.SetItemText(0,7,bztyjstr);
				}
				//ĸ��ʵʱ��ֵ     int*10000
				if(PackLength>offset)
				{
					int bmjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString bmjjssjzstr = _T("");bmjjssjzstr.Format(L"%d",bmjjssjz);
					m_ListCtl.SetItemText(0,8,bmjjssjzstr);
				}
				//����ĸ������   int*100
				if(PackLength>offset)
				{
					int bszmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString bszmjjxzstr = _T("");bszmjjxzstr.Format(L"%d",bszmjjxz);
					m_ListCtl.SetItemText(0,9,bszmjjxzstr);
				}
				//����ĸ�����   int*100
				if(PackLength>offset)
				{
					int bxzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString bxzmjjxzstr = _T("");bxzmjjxzstr.Format(L"%d",bxzmjjxz);
					m_ListCtl.SetItemText(0,10,bxzmjjxzstr);
				}
				//�۸�ܸ�         int*1000
				if(PackLength>offset)
				{
					int bjggg = *(int*)(PackData+offset);
				    offset += 4;
					CString bjgggstr = _T("");bjgggstr.Format(L"%d",bjggg);
					m_ListCtl.SetItemText(0,11,bjgggstr);
				}
			}
			//Ϊ2��ĸ����ʱΪ�����ֶ� 
		    if(fundtypeval == 2)   //��ʾ����ĸ������ֶ�
			{
				//�������       int*1000
				if(PackLength>offset)
				{
					int momztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString momztyjstr = _T("");momztyjstr.Format(L"%u",momztyj);
					m_ListCtl.SetItemText(0,12,momztyjstr);
				}
				//ĸ��ʵʱ��ֵ     int*10000
				if(PackLength>offset)
				{
					int mommjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mommjjssjzstr = _T("");mommjjssjzstr.Format(L"%d",mommjjssjz);
					m_ListCtl.SetItemText(0,13,mommjjssjzstr);
				}
				//����ĸ������   int*100
				if(PackLength>offset)
				{
					int momszmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString momszmjjxzstr = _T("");momszmjjxzstr.Format(L"%d",momszmjjxz);
					m_ListCtl.SetItemText(0,14,momszmjjxzstr);
				}
				//����ĸ�����   int*100
				if(PackLength>offset)
				{
					int momxzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString momxzmjjxzstr = _T("");momxzmjjxzstr.Format(L"%d",momxzmjjxz);
					m_ListCtl.SetItemText(0,15,momxzmjjxzstr);
				}
			}
			if(fundtypeval == 3)   //Ϊ3���޺����ֶ�
			{
				//���ֶ�
			}
		}break;
	case 2995:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// crc	       short		//״̬��������crc
			int fundtypeval = 0;
			if(PackLength>offset)
			{
				unsigned short CRCval = *(unsigned short*)(PackData+offset);
				offset += 2;
				CString CRCstr = _T("");CRCstr.Format(L"%d",CRCval);
				m_ListCtl.SetItemText(0,1,CRCstr);
			}
			////Ϊ0��A����ʱΪ�����ֶ� 
			if(PackLength>offset)
			{
				RecdNum = *(short*)(PackData+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			int i = 0;
			while(PackLength-offset>0)
			{
				CString xh = _T("");xh.Format(L"%d",i+2);
				m_ListCtl.InsertItem(i+1,_T(""));
			    m_ListCtl.SetItemText(i+1,0,xh);
				//�ַ������� String[]  
				short strlen = *(short*)(PackData+offset);
				offset+=2;
				char *strval = new char[strlen+1];
				memset(strval,0,strlen+1);
				memcpy(strval,PackData+offset,strlen);
				offset += strlen;
				wstring stringstr = UTF8_2_UTF16(strval);
				m_ListCtl.SetItemText(i,2,stringstr.c_str());
				i += 1;
			}
		}break;
	case 2997:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// int         ��ʼ�۶ϵ�ʱ�䣬��ʽHHMMSS
			int fundtypeval = 0;
			if(PackLength>offset)
			{
				unsigned int ksrdsj = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ksrdsjstr = _T("");ksrdsjstr.Format(L"%u",ksrdsj);
				m_ListCtl.SetItemText(0,1,ksrdsjstr);
			}
			////int         �����۶ϵ�ʱ�䣬��ʽHHMMSS
			if(PackLength>offset)
			{
				unsigned int jsrdsj = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString jsrdsjstr = _T("");jsrdsjstr.Format(L"%u",jsrdsj);
				m_ListCtl.SetItemText(0,2,jsrdsjstr);
			}
			///int         �侲�ڲο��۸�
			if(PackLength>offset)
			{
				unsigned int ljqckjg = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ljqckjgstr = _T("");ljqckjgstr.Format(L"%u",ljqckjg);
				m_ListCtl.SetItemText(0,3,ljqckjgstr);
			}
			///int         �侲�����޼�
			if(PackLength>offset)
			{
				unsigned int ljqxxjg = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ljqxxjgstr = _T("");ljqxxjgstr.Format(L"%u",ljqxxjg);
				m_ListCtl.SetItemText(0,4,ljqxxjgstr);
			}
			///int         �侲�����޼�
			if(PackLength>offset)
			{
				unsigned int ljqsxjg = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ljqsxjgstr = _T("");ljqsxjgstr.Format(L"%u",ljqsxjg);
				m_ListCtl.SetItemText(0,5,ljqsxjgstr);
			}
		}break;
	}
}





void CYunTestDlg::OnBnClickedBtnPress()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�ж��Ƿ��Ƕ�����ܰ�����
	printf("��ʼ����\n");
	bool g_check = m_CheckFun.GetCheck();
	
	//��ȡ��������
	int nindex = m_ComBoxFun.GetCurSel();
	int tmplen=0;
	IODATA tmpio;
	memset(tmpio.strdata,0,10240);
	tmpio.datalen = 0;
	UpdateData();
	//��ʼ��ѹ����������
	m_ConnSNum = 0;
	m_ConnFNum = 0;
	m_SendSNum = 0;
	m_SendFNum = 0;
	m_RecvSNum = 0;
	m_RecvFNum = 0;
	accountcount = 0;
	//��ȡ�����ϵ�������
	CString strnum = _T("");
	GetDlgItem(IDC_EDIT_NUMBER)->GetWindowText(strnum);
	if(strnum.IsEmpty())
	{
		AfxMessageBox(L"������Ϊ�գ�������д������");
		return;
	}
	theApp.socknum = _wtoi(strnum);
	//��ȡ���ͼ��ʱ��
	GetDlgItem(IDC_EDIT_SECOND)->GetWindowText(strnum);
	if(strnum.IsEmpty())
	{
		AfxMessageBox(L"���ͼ��ʱ��Ϊ�գ�������д���ͼ��");
		return;
	}
	CString m_Btnpress = _T("");
	GetDlgItem(IDC_BTN_PRESS)->GetWindowText(m_Btnpress);
//	g_dlg = this;
	if(m_Btnpress.Compare(L"ѹ����ʼ") == 0)
	{
		if(g_check)
		{
			int k=0,h=0,jj = 0;
			for(int f=0;f<50;f++)
			{
				m_pressreqs[f] = "";
			}
			//ȡÿ�����ܵ����ݣ���ȡѡ�еĹ����ֶ�
			for(int t=0;t<m_ComBoxFun.GetCount();t++)
			{
				if(m_ComBoxFun.GetCheck(t))
				{
					k = t;
					m_ComBoxFun.SetCurSel(k);
					OnCbnSelchangeComboFun();

					int idnum = m_ComBoxFun.GetCurSel();
					int FunNo = Xml.XmlFun[idnum].FunID;
					GeneratePack(FunNo);
					memset(tmpio.strdata,0,10240);
	                tmpio.datalen = Datalen;
					
					memcpy(tmpio.strdata,SendBuffer,Datalen);
					iodata.push_back(tmpio);
					mapFun.insert(map<int,vector<IODATA>>::value_type(jj,iodata));
					MapFun.insert(map<int,IODATA>::value_type(jj,tmpio));
					jj++;
					h++;
					iodata.clear();
				}
			}
			if(!k)
			{
				AfxMessageBox(L"���ܺ�ûѡ������ѡ���ܺ�");
				return;
			}
		}
		else
		{
			//��������
			int idno = m_ComBoxFun.GetCurSel();
			int FunId = Xml.XmlFun[idno].FunID;
			GeneratePack(FunId);
			memcpy(tmpio.strdata,SendBuffer,Datalen);
			tmpio.datalen = Datalen;
			MapFun.insert(map<int,IODATA>::value_type(0,tmpio));
			iodata.push_back(tmpio);
			mapFun.insert(map<int,vector<IODATA>>::value_type(0,iodata));
		}
		GetDlgItem(IDC_BTN_PRESS)->SetWindowText(L"ѹ��ֹͣ");
		SYSTEMTIME Iotime;
		GetLocalTime(&Iotime);
		CString IOtm = _T("");
		IOtm.Format(L"%d-%d-%d  %d-%d-%d-%d",Iotime.wYear,Iotime.wMonth,Iotime.wDay,Iotime.wHour,Iotime.wMinute,Iotime.wSecond,Iotime.wMilliseconds);
		GetDlgItem(IDC_EDIT_BEGINTIME)->SetWindowText(IOtm);
		if(m_ioFun.Start() == false)
	    {
		    printf("��ɶ˿ڳ�ʼ��ʧ��");
		    return;
	    }
		SetTimer(STRESS_DATA,1000,NULL);
	}
	else
	{
		m_ioFun.Stop();
		CString IOend = _T("");
		SYSTEMTIME endtime;
		GetLocalTime(&endtime);
		IOend.Format(L"%d-%d-%d  %d-%d-%d",endtime.wYear,endtime.wMonth,endtime.wDay,endtime.wHour,endtime.wMinute,endtime.wSecond);
		GetDlgItem(IDC_EDIT_ENDTIME)->SetWindowText(IOend);
		KillTimer(STRESS_DATA);
		GetDlgItem(IDC_BTN_PRESS)->SetWindowText(L"ѹ����ʼ");
		MapFun.clear();
		return;
	}
}

void CYunTestDlg::GetFunXml(int funid)
{
	CString strData = _T("");
	m_FunList.GetLBText(funid,strData);
	CString Function(strData,4);
	switch(_ttoi(Function))
	{
	case 1000:
		{
            //����xml�ֵ��ļ�
	        bool xmlmk = Xml.DictXml("HQYun.xml");
	        if(xmlmk == false)
	        {
		        AfxMessageBox(L"xml�����ֵ����ʧ��\n");
	        }
		}break;
	case 3001:
		{
			//��������xml�ֵ��ļ�
	        bool xml3001 = Xml.DictXml("tuisong.xml");
	        if(xml3001 == false)
	        {
		        AfxMessageBox(L"xml���͹����ֵ����ʧ��\n");
	        }
		}break;
	}
	//�ֵ������Ϻ󣬵���combox���ݿ�
	for(unsigned int i=0; i<Xml.XmlFun.size(); i++)
	{
		CString xmlstr = _T("");
		xmlstr.Format(_T("%d %s"),Xml.XmlFun[i].FunID,Xml.XmlFun[i].FunName);
		m_ComBoxFun.InsertString(i,xmlstr);
	}
	m_ComBoxFun.SetCurSel(0);
	//����combox������ı༭��Χ  m_ComBoxFun
	CRect SelRect;
	GetDlgItem(IDC_STATIC_FUNC)->GetWindowRect(&SelRect);
	ScreenToClient(&SelRect);

	CRect ComRect;
	GetDlgItem(IDC_COMBO_FUN)->GetWindowRect(&ComRect);
	ScreenToClient(&ComRect);

	for(int i=0; i<50; i++)
	{
		m_Static[i].Create((LPCTSTR)"CStatic"
			, WS_CHILD|SS_LEFT
			, CRect(SelRect.left,ComRect.bottom+8+i*16,SelRect.left+162,SelRect.bottom+8+i*16+ComRect.Height())
			, this
			, 2000+i);
		m_Static[i].SetFont(GetFont());

	//	ES_MULTILINE|ES_WANTRETURN|ES_AUTOVSCROLL    WS_CHILD|WS_VISIBLE|WS_TABSTOP |WS_BORDER|ES_LEFT|ES_AUTOHSCROLL
//|WS_HSCROLL| WS_VSCROLL
		m_Edit[i].Create(ES_MULTILINE|WS_CHILD|WS_EX_CLIENTEDGE|WS_TABSTOP|ES_LEFT|ES_AUTOHSCROLL|WS_BORDER
			, CRect(SelRect.left+100,ComRect.bottom+8+i*16,ComRect.right,ComRect.bottom+8+i*16+SelRect.Height())
			, this
			, 2100+i);
		m_Edit[i].SetFont(GetFont());
	}
	OnCbnSelchangeComboFun();
}

void CYunTestDlg::OnCbnSelchangeFunlist()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nindex = m_FunList.GetCurSel();
	GetFunXml(nindex);
}





LRESULT CYunTestDlg::OnSocketmessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


// ѹ������ֵ
void CYunTestDlg::AddNum(__int64 Type, __int64 CountNum)
{
	if(Type == PressConnSucc)
	{
		InterlockedExchangeAdd64(&m_ConnSNum,CountNum);
	}
	if(Type == PressConnFail)
	{
		InterlockedExchangeAdd64(&m_ConnFNum,CountNum);
	}
	if(Type == PressSendSucc)
	{
		InterlockedExchangeAdd64(&m_SendSNum,CountNum);
	}
	if (Type == PressSendFail)
	{
		InterlockedExchangeAdd64(&m_SendFNum,CountNum);
	}
	if(Type == PressRecvSucc)
	{
		InterlockedExchangeAdd64(&m_RecvSNum,CountNum);
	}
	if(Type == PressRecvFail)
	{
		InterlockedExchangeAdd64(&m_RecvFNum,CountNum);
	}
}


void CYunTestDlg::OnBnClickedBtn300()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ѹ��������ش��봦��

	CString m_add = _T("");
	GetDlgItem(IDC_BTN_300)->GetWindowText(m_add);
	if(m_add.Compare(L"ѹ��") == 0)
	{
		TCHAR filename[80] = {0};
		::GetPrivateProfileString(L"ADDALARM",L"FILENAME",L"",filename,80,L".\\ALARM.ini");
		size_t len = 2*wcslen(filename) + 1;
		size_t converted = 0;
		char *filestr;
		filestr=(char*)malloc(len*sizeof(char));
		wcstombs_s(&converted, filestr, len, filename, _TRUNCATE);

		TCHAR marketcode[10] = {0};
		::GetPrivateProfileString(L"ADDALARM",L"MARKET",L"",marketcode,10,L".\\ALARM.ini");
		size_t mktlen = 2*wcslen(marketcode) + 1;
		size_t mktcodelen = 0;
		char *mktcode;
		mktcode=(char*)malloc(len*sizeof(char));
		wcstombs_s(&mktcodelen, mktcode, mktlen, marketcode, _TRUNCATE);
	    m_ThdData.ReadFile(filestr,mktcode);
		free(filestr);
		free(mktcode);
	    m_ThdData.InitMap();
	    m_ThdData.userid = 0;
	    m_ThdData.stockid = 0;
	    m_ThdData.socketcount = 0;
		m_ThdData.AddUser();
//	    m_ThdData.CreateSocket();
		m_ThdData.GenerateSock();
		
		m_ThdData.g_mutilstop = false;
		SetEvent(m_ThdData.g_Event);
	    m_ThdData.CreateThreadMash();
		GetDlgItem(IDC_BTN_300)->SetWindowText(L"ֹͣ");
	}
	else
	{
		m_ThdData.g_mutilstop = true;
		m_ThdData.Uninit();
		m_ThdData.userid = 0;
		m_ThdData.stockid = 0;
		m_ThdData.socketcount = 0;
	//	m_ThdData->HqData.clear();
		GetDlgItem(IDC_BTN_300)->SetWindowText(L"ѹ��");
	}
}
