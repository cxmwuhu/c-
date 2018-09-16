#include "StdAfx.h"
#include "MutilThread.h"


//自定义ascii到utf8转换函数;
void ASCiiToUtf(LPCSTR srcStr,LPWSTR dstStr)
{
	DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, srcStr, -1, NULL, 0);
	MultiByteToWideChar (CP_UTF8, 0,srcStr, -1, dstStr, dwNum);
}

//定义UTF16到UTF8转换;
char * UTF16_TO_UTF8( const wstring& strUTF16 )  
{  
	int nUTF8Length = ::WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,NULL,0,0,0); 
	char * pStrUtf8=new char[nUTF8Length];
	WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,pStrUtf8,nUTF8Length,0,0);  
	return pStrUtf8;  
}  

//定义UTF8到UTF16转换;
wstring UTF8_UTF16( const string& strUTF8 )  
{  
	int nUTF16Length = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,NULL,0);  
	nUTF16Length += 1;  
	wstring strUTF16(nUTF16Length ,' ');  
	int nRet = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,&strUTF16[0],nUTF16Length);  
	ASSERT(0 != nRet);  
	return strUTF16;  
}  

CMutilThread::CMutilThread(void)
{
	fptext = NULL;
	WSADATA  wsaData;
	int startret = WSAStartup(MAKEWORD(2,2),&wsaData);
	if(startret!=0)
	{
		printf("300初始化socket失败");
	}
}


CMutilThread::~CMutilThread(void)
{
}


// 读取二进制文件
void CMutilThread::ReadFile(char * filename, char *MarketCode)
{
	fptext = fopen(filename,"r+");
	if(fptext == NULL)
	{
		printf("文件打开失败\n");
		return;
	}
	//获取文件中的一行数据
	int gg=0;
	char *szbuff = new char[1024*3];
	while(!feof(fptext))
	{
		HQSTATIC datastruct;
		STATIC_DATA staticdata;
		ZeroMemory(&datastruct,sizeof(HQSTATIC));
		ZeroMemory(&staticdata,sizeof(STATIC_DATA));
		memset(szbuff,0,1024*3);
		fgets(szbuff,3*1024-1,fptext);
		int n = 0,pos = 0,ff=0;
		float lowprice = 0,highprice = 0;
		int len = strlen(szbuff);
		while(*(szbuff+n)!='\0')
		{
			if(*(szbuff+n) == ',')
			{
				switch(ff)
				{
				case 0:     //WORD	m_wStkID;			//本市场内唯一标示,在本市场内的序号
					{
						ff = ff+1;
						char *sztemp = new char[n+1];
						memset(sztemp,0,n+1);
						memcpy(sztemp,szbuff,n);
						staticdata.m_wStkID = atoi(sztemp);
						delete []sztemp;
						n++;
						pos = n;
					}break;
				case 1:      //char	m_strLabel[10];		//代码
					{
						ff = ff+1;
						memcpy(datastruct.m_strLabel,MarketCode,2);
						memcpy(datastruct.m_strLabel+2,szbuff+pos,n-pos);
						n++;
						pos = n;
					}break;
				case 2:    //char	m_strName[32];		//名称
					{
						ff = ff+1;
						memcpy(staticdata.m_strName,szbuff+pos,n-pos);
						n++;
						pos = n;
					}break;
				case 3:   // BYTE	m_cType;			//STK_TYPE
					{
						ff = ff+1;
						staticdata.m_cType = *(szbuff+n-1);
						n++;
						pos = n;
					}break;
				case 4:    //BYTE	m_nPriceDigit;		//价格最小分辨率，非常重要，每一个DWORD类型的价格都要除以10^m_nPriceDigit才是真正的价格
					{
						ff = ff+1;
						staticdata.m_cType = *(szbuff+n-1);
						n++;
						pos = n;
					}break;
				case 5:    //short	m_nVolUnit;			//成交量单位，每一成交量单位表示多少股
					{
						ff = ff+1;
						n++;
						pos = n;
					}break;
				case 6:    //MWORD	m_mFloatIssued;		//流通股本
					{
						ff = ff+1;
						n++;
						pos = n;
					}break;
				case 7:    //MWORD	m_mTotalIssued;		//总股本
					{
						ff = ff+1;
						n++;
						pos = n;
					}break;
				case 8:    //DWORD	m_dwLastClose;		//昨收
					{
						ff = ff+1;
						char *zstemp = new char[n-pos+1];
						memset(zstemp,0,n-pos+1);
						memcpy(zstemp,szbuff+pos,n-pos);
						datastruct.m_dwLastClose = atoi(zstemp);
						delete []zstemp;
						n++;
						pos = n;
					}break;
				case 9:    //DWORD	m_dwAdvStop;		//涨停
					{
						ff = ff+1;
						char buff[10] = {0};
						char *zttemp = new char[n-pos+1];
						memset(zttemp,0,n-pos+1);
						memcpy(zttemp,szbuff+pos,n-pos);
						datastruct.m_dwAdvStop = atoi(zttemp);
						datastruct.m_highstop = (float)(datastruct.m_dwAdvStop)/100;
						delete []zttemp;
						n++;
						pos = n;
						//剩下的就是跌停价了
						char *dttemp = new char[strlen(szbuff)-pos];
						memset(dttemp,0,n-pos+1);
						memcpy(dttemp,szbuff+pos,strlen(szbuff)-pos-1);
						datastruct.m_dwDecStop = atoi(dttemp);
						datastruct.m_lowstop = (float)(datastruct.m_dwDecStop)/100;
						delete []dttemp;
						n = strlen(szbuff)-1;
					}break;
				default:
					break;
				}
			}
			else
			{
				n++;
			}
		}
		if(strcmp(MarketCode,"SH") == 0)
		{
		    HqData.insert(map<int,HQSTATIC>::value_type(gg,datastruct));
		}
		if(strcmp(MarketCode,"SZ") == 0)
		{
			HqData.insert(map<int,HQSTATIC>::value_type(gg,datastruct));
		}
		if(strcmp(MarketCode,"SZ") == 0)
		{
			HqData.insert(map<int,HQSTATIC>::value_type(gg,datastruct));
		}
		gg = gg+1;
	//	printf("%s\n",szbuff);
	}
	delete []szbuff;
	printf("码表数据加载完成\n");
	fclose(fptext);
}


// 创建多线程的函数处理收发数据
void CMutilThread::CreateThreadMash()
{
	SYSTEM_INFO cpuinfo;
	GetSystemInfo(&cpuinfo);
	CPUnum = cpuinfo.dwNumberOfProcessors*2;
//	CPUnum = 1;
	DWORD mutilid;
	socket_thread = new HANDLE[CPUnum];
	for(int i=0; i<CPUnum; i++)
	{
	    socket_thread[i] = ::CreateThread(NULL,0,_NetThread,this,0,&mutilid);
		if(socket_thread[i] == NULL)
		{
		    printf("线程创建失败\n");
		}
		printf("线程创建成功:%d\n",mutilid);
	}
}

DWORD WINAPI CMutilThread::_NetThread(LPVOID lparam)
{
	CMutilThread *pDealMsg = (CMutilThread*)lparam;
	int bigflag = ::GetPrivateProfileInt(L"ADDALARM",L"BIGPACKMARK",0,L".\\ALARM.ini");
	printf("线程函数开始\n");
	while(!(pDealMsg->g_mutilstop))
	{
		int threadid = GetCurrentThreadId();
	//	printf("当前工作线程号:%d\n",threadid);
		//组发送包
		WaitForSingleObject(pDealMsg->g_Event,INFINITE);
//		pDealMsg->g_threadlock.Lock();
		char *GenBuff = new char[1024*5];
		memset(GenBuff,0,1024*5);
		int packlen = 0;
		pDealMsg->Generate300Pack(300,GenBuff,&packlen);
		
		//组包组完后发送数据包
		SOCKET_INFO *pthread = pDealMsg->GetLink();
		if(pthread == NULL)
		{
			continue;
		}
		int nSum = 0, nBytes = 0;
		while(packlen>0&&pthread->m_Sock!=SOCKET_ERROR)
		{
			//  int sendbyte = send(pthread->m_Sock,pDealMsg->GenBuff+nSum,pDealMsg->packoflen,0);
			int sendbyte = send(pthread->m_Sock,GenBuff+nSum,packlen,0);
			if(sendbyte<0)
			{
				printf("发送失败:%d\n",WSAGetLastError());
				pDealMsg->DisConnectSvr(pthread);
				printf("6\n");
				pDealMsg->socketcount = (pDealMsg->socketcount) - 1;
				printf("7\n");
				ReleaseSemaphore(pDealMsg->m_SOCKSemp,1,NULL);
				delete []GenBuff;
				return 0;
			}
			nSum+=sendbyte;
			packlen -= sendbyte;
		}
		delete []GenBuff;
	//	pDealMsg->g_threadlock.Unlock();
		if(bigflag == 1)
		{
			//接受数据包，先接收包头
			char headBuff[20] = {0};
			int hlen = sizeof(BIG_HEAD); 
			int recvlen = 0;
			while(hlen>0)
			{
				int recvret = recv(pthread->m_Sock,headBuff+recvlen,hlen,0);
		//		printf("11,socket值:%d\n",pthread->m_Sock);
				if(recvret<=0)
				{
					int errthread = WSAGetLastError();
					printf("接收失败:d\n",errthread);
					pDealMsg->DisConnectSvr(pthread);
					pDealMsg->socketcount = (pDealMsg->socketcount) - 1;
					ReleaseSemaphore(pDealMsg->m_SOCKSemp,1,NULL);
					return 0;
				}
				recvlen += recvret;
				hlen -= recvret;
			}
			//收包体
			BIG_HEAD *ppData = (BIG_HEAD*)headBuff;
			char *bodypack = new char[ppData->length+1];
			memset(bodypack,0,ppData->length+1);
			int bodybytes = 0;
			while(ppData->length>0)
			{
				int bodyret = recv(pthread->m_Sock,bodypack+bodybytes,ppData->length,0);
				if(bodyret<=0)
				{
					printf("接收包体失败:d\n",WSAGetLastError());
					pDealMsg->DisConnectSvr(pthread);
					pDealMsg->socketcount = (pDealMsg->socketcount) - 1;
					ReleaseSemaphore(pDealMsg->m_SOCKSemp,1,NULL);
					return 0;
				}
				bodybytes += bodyret;
				ppData->length = (ppData->length) - bodyret;
			}
			delete []bodypack;
		}
		else
		{
		    //接受数据包，先接收包头
		    char headBuff[20] = {0};
		    int hlen = sizeof(ACC_CMDHEAD); 
		    int recvlen = 0;
		    while(hlen>0)
		    {
			    int recvret = recv(pthread->m_Sock,headBuff+recvlen,hlen,0);
			    if(recvret<=0)
			    {
				    int errthread = WSAGetLastError();
				    printf("接收失败:d\n",errthread);
				    pDealMsg->DisConnectSvr(pthread);
				    pDealMsg->socketcount = (pDealMsg->socketcount) - 1;
				    ReleaseSemaphore(pDealMsg->m_SOCKSemp,1,NULL);
				    return 0;
			    }
			    recvlen += recvret;
			    hlen -= recvret;
		    }
		    //收包体
		    ACC_CMDHEAD *ppData = (ACC_CMDHEAD*)headBuff;
		    char *bodypack = new char[ppData->nLen+1];
		    memset(bodypack,0,ppData->nLen+1);
		
		    int bodybytes = 0,nretnum=0;
		    while((ppData->nLen)>0)
		    {
			    int bodyret = recv(pthread->m_Sock,bodypack+bodybytes,ppData->nLen,0);
				/*
				push_head *psonhead = (push_head *)bodypack;
				int bodytype = psonhead->sub_type;
				int bodylen = psonhead->sub_length;
				unsigned char szresult = *(bodypack+nretnum+sizeof(push_head));
				int reslt = szresult;
				nretnum += 1;
				if(reslt == 1)    //shibai
				{
					short stringlen = *(short*)(bodypack+nretnum+sizeof(push_head));
					nretnum += 2;
					char *strstring = new char[stringlen+1];
					memset(strstring,0,stringlen+1);
					memcpy(strstring,bodypack+nretnum+sizeof(push_head),stringlen);
					wstring stringval = UTF8_UTF16(strstring);
					delete []strstring;
				}
				if(reslt == 0)
				{

				}
				*/
			    if(bodyret<=0)
			    {
				    printf("接收包体失败:d\n",WSAGetLastError());
				    pDealMsg->DisConnectSvr(pthread);
				    pDealMsg->socketcount = (pDealMsg->socketcount) - 1;
				    ReleaseSemaphore(pDealMsg->m_SOCKSemp,1,NULL);
					delete []bodypack;
				    return 0;
			    }
			    bodybytes += bodyret;
			    ppData->nLen = (ppData->nLen) - bodyret;
			
		    }
		    delete []bodypack;
		}
		pthread->g_busy = false;
	}
	return 1;
}

/*
// 单独开启个线程创建socket线程
void CMutilThread::CreateSocket(void)
{
	DWORD sockid;
	SocketHandle = ::CreateThread(NULL,0,_SocketThread,this,0,&sockid);
	if(SocketHandle == NULL)
	{
		printf("多线程socket创建失败,socket线程号:%d\n",sockid);
		return;
	}
	//获取socket个数
	Socket_Count = ::GetPrivateProfileInt(L"ADDALARM",L"SOCKETNUM",100,L".\\ALARM.ini");
	
}

DWORD WINAPI CMutilThread::_SocketThread(LPVOID lparam)
{
	//获取socket个数
	printf("socket 线程开始\n");
	CMutilThread *pSock = (CMutilThread*)lparam;
	int idx=0;
	while(pSock->socketcount<pSock->Socket_Count)
	{
		DWORD SempID = ::WaitForSingleObject(pSock->m_SOCKSemp,INFINITE);
		if(SempID == WAIT_OBJECT_0)
		{
		SOCKET clientsocket = ::socket(AF_INET,SOCK_STREAM,0);
		if(clientsocket == 0)
		{
			int err = WSAGetLastError();
			printf("SOCKET创建失败:%d\n",err);
		}
		CString ipaddress = _T("");
	//	TCHAR ipaddress[30] = {0};
		::GetPrivateProfileString(L"ADDALARM",L"IPADDR",L"127.0.0.1",ipaddress.GetBuffer(30),30,L".\\ALARM.ini");
		int m_port = ::GetPrivateProfileInt(L"ADDALARM",L"IPPORT",12345,L".\\ALARM.ini");
		char ipvalue[30] = {0};
		::wsprintfA(ipvalue,"%ls",(LPCTSTR)ipaddress);
		//获取连接socket
		for(int i=0; i<ipaddress.GetLength(); i++)
		{
			ipvalue[i] = ipaddress.GetAt(i);
		}
		ipaddress.ReleaseBuffer();
		sockaddr_in serveraddr;
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.S_un.S_addr = inet_addr(ipvalue);
		serveraddr.sin_port = htons(m_port);

		int connret = connect(clientsocket,(sockaddr*)&serveraddr,sizeof(sockaddr));
		if(connret != SOCKET_ERROR)
		{
			//登录之前需要验证下服务器，否则可能连接不成功
			LOGIN_HEAD loginpack;
			memset(&loginpack,0,sizeof(LOGIN_HEAD));
			//获取包头信息
			loginpack.accCmdHead.wCmdType = ACCCMD_SERVERLOGIN;
			loginpack.accCmdHead.nLen = sizeof(ACC_SERVERLOGIN);
			//获取ACC_SERVERLOGIN包体内容
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

			loginpack.accServerLogin.nSerId = idx+1;
			int loginret = send(clientsocket,(char*)&loginpack,sizeof(LOGIN_HEAD),0);
			if(loginret>0)
			{
				printf("登录包发送成功,登录包长度:%d\n",loginret);
			}
	        //把socket保存到数组中
			SOCKET_INFO socketData;
			socketData.g_busy = false;
			socketData.m_Sock = clientsocket;
			socketData.bConnected = true;
			socketData.lidx = idx+1;
			pSock->g_mutil.Lock();
			pSock->Socket_map.insert(map<int,SOCKET_INFO>::value_type(idx,socketData));
			pSock->g_mutil.Unlock();
			//pSock->Socket_Count = pSock->Socket_Count-1;
			idx = idx+1;
			pSock->socketcount = idx;
			if(idx<pSock->Socket_Count)
				ReleaseSemaphore(pSock->m_SOCKSemp,1,NULL);
		}
		else
		{
			int errconn = WSAGetLastError();
			printf("socket连接失败:errconn\n",errconn);
		}
		}
	}
//	SetEvent(pSock->g_Event);
	printf("触发时件成功\n");
	return 1;
}
*/

// 初始化map结构体信息
bool CMutilThread::InitMap(void)
{
	CPUnum = 0;
	Socket_Count = 0;
	//获取socket个数
	Socket_map.clear();
	Socket_Count = ::GetPrivateProfileInt(L"ADDALARM",L"SOCKETNUM",100,L".\\ALARM.ini");
	/*
	for(int i =0; i<Socket_Count; i++)
	{
		Socket_map[i].m_Sock = SOCKET_ERROR;
		Socket_map[i].bConnected = FALSE;
		Socket_map[i].g_busy = false;
		Socket_map[i].lidx = i+1;
	}
	*/
	SocketHandle = NULL;
	m_SOCKSemp = ::CreateSemaphore(NULL,1,100,NULL);
	if(m_SOCKSemp == NULL)
	{
		printf("创建线程信号量失败\n");
		return false;
	}
	g_Event = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	ShutdownEvent = ::CreateEvent(NULL,TRUE,FALSE,NULL);
	return true;
}


// 释放map结构体信息
void CMutilThread::Uninit(void)
{
	for(unsigned int i=0; i<Socket_map.size(); i++)
	{
		closesocket(Socket_map[i].m_Sock);
		Socket_map[i].bConnected = FALSE;
		Socket_map[i].g_busy = false;
		Socket_map[i].lidx = 0;
	}
	Socket_map.clear();
	//退出线程
	for(int j=0; j<CPUnum; j++)
	{
		if((socket_thread[j]!=NULL)&&(socket_thread[j]!=INVALID_HANDLE_VALUE))
		{
			CloseHandle(socket_thread[j]);
		}
	}
	//退出socket线程
	if(SocketHandle!=NULL&&SocketHandle!=INVALID_HANDLE_VALUE)
	{
	    CloseHandle(SocketHandle);
	}
	CPUnum = 0;
	HqData.clear();
}



// 释放某个链接
void CMutilThread::DisConnectSvr(SOCKET_INFO * LINK_INFO)
{
	g_mutil.Lock();
	if(LINK_INFO == NULL)
	{
		g_mutil.Unlock();
		return;
	}
	//关闭相关操作
	closesocket(LINK_INFO->m_Sock);
	//清除这个连接
	map<int,SOCKET_INFO>::iterator sockit;
	sockit = Socket_map.find(LINK_INFO->lidx-1);
	if(sockit!=Socket_map.end())
	{
	//	Socket_map.erase(LINK_INFO->lidx-1);
		Socket_map.erase(sockit);
	}
	g_mutil.Unlock();
}


// 获取空闲socket已发送数据
SOCKET_INFO* CMutilThread::GetLink(void)
{
	g_mutil.Lock();
	SOCKET_INFO *pRet = NULL;
	long i = 0;
	long lsize = Socket_map.size();
	for(i=0; i<lsize; i++)
	{
		if((Socket_map[i].g_busy != TRUE)&&(Socket_map[i].bConnected = TRUE))
		{
			pRet = &(Socket_map[i]);
			Socket_map[i].g_busy = TRUE;
			break;
		}
	}
	if(i>=lsize)
	{
		pRet = NULL;
		printf("与服务器连接用完\n");
	}
	g_mutil.Unlock();
	return pRet;
}


// 用完此连接后释放此次连接
void CMutilThread::SetFreeLink(SOCKET_INFO * SockData)
{
	g_mutil.Lock();
	if(SockData == NULL)
	{
		g_mutil.Unlock();
		return;
	}
	SockData->g_busy = false;
	g_mutil.Unlock();
}


// //300添加预警功能函数
//bool CMutilThread::Generate300Pack(int PackType)
bool CMutilThread::Generate300Pack(int PackType, char *GenBuff, int *buffoflen)
{
	int sendpos = 0;
	memset(GenBuff,0,1024*5);
	int packoflen = 0;
	string tempUTF;
	int bigflag = ::GetPrivateProfileInt(L"ADDALARM",L"BIGPACKMARK",0,L".\\ALARM.ini");
	if(bigflag == 1)   //取行情包头
	{
		struct BIG_HEAD *pHqHead = (struct BIG_HEAD*)GenBuff;
		pHqHead->cSparate = '{';
		pHqHead->attrs = 0;
		pHqHead->type = 3001;
		packoflen += sizeof(struct BIG_HEAD);
		*(GenBuff+packoflen) = 2;
		packoflen += 1;

		//推送包头
		push_head *pphead = (struct push_head*)(GenBuff+packoflen);
		pphead->sub_attrs = 0;
		pphead->sub_extend = 0;
		pphead->sub_type = 300;
		packoflen += sizeof(struct push_head);

		//组包体
		//获取功能配置参数
	    CString Funtype = _T("");
	    Funtype.Format(L"%d",PackType);
	    TCHAR username[50] = {0};
	    ::GetPrivateProfileString(Funtype,L"用户名",L"ccc112",username,50,L".\\ALARM.ini");
		//unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(username);
	    if(tempUTF.length()!=0)
	    {
		    *(short*)(GenBuff+packoflen) = tempUTF.length();
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		    packoflen += tempUTF.length(); 
	    }
	    //用户密码
	    TCHAR userpwd[50] = {0};
	    ::GetPrivateProfileString(Funtype,L"用户密码",L"111111",userpwd,50,L".\\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(userpwd);
	    if(tempUTF.length()!=0)
	    {
		    *(short*)(GenBuff+packoflen) = tempUTF.length();
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		    packoflen += tempUTF.length(); 
	    }
	    //手机号码
	    TCHAR tellphone[12] = {0};
	    ::GetPrivateProfileString(Funtype,L"手机号码",L"11111111111",tellphone,12,L".\\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(tellphone);
	    if(tempUTF.length()!=0)
	    {
		    *(short*)(GenBuff+packoflen) = tempUTF.length();
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		    packoflen += tempUTF.length(); 
	    }
	    //手机id             String
	    TCHAR tellid[20] = {0};
	    ::GetPrivateProfileString(Funtype,L"手机id",L"11111111111",tellid,20,L".\\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(tellid);
	    if(tempUTF.length()!=0)
	    {
		    *(short*)(GenBuff+packoflen) = tempUTF.length();
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		    packoflen += tempUTF.length(); 
	    }
	    //推送标示           String 
	    CString pushmark = _T("");
	    if(userid>=UserID.size())
	    {
		    userid = 0;
		    tempUTF.assign(UserID[userid].c_str());
	    }
	    else
	    {
		    tempUTF.assign(UserID[userid].c_str());
	    }
	    if(tempUTF.length()!=0)
	    {
		    *(short*)(GenBuff+packoflen) = tempUTF.length();
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		    packoflen += tempUTF.length(); 
	    }
	    //平台               byte
	    TCHAR ptype[2] = {0};
	    ::GetPrivateProfileString(Funtype,L"平台",L"3",ptype,2,L".\\ALARM.ini");
	    if(tempUTF.length()!=0)
	    {
		    unsigned char szbyte = _ttoi(ptype);
		    *(GenBuff+packoflen) = szbyte;
		    packoflen += 1;
	    }
	    //本地软件版本       String
	    TCHAR softver[20] = {0};
	    ::GetPrivateProfileString(Funtype,L"本地软件版本",L"",softver,20,L".\\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(softver);
	    if(tempUTF.length()!=0)
	    {
		    *(short*)(GenBuff+packoflen) = tempUTF.length();
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		    packoflen += tempUTF.length(); 
	    }
	    //股票代码           String
	    if(stockid>=HqData.size())
	    {
		    //取股票代码
		    stockid = 0;
		    *(short*)(GenBuff+packoflen) = strlen(HqData[stockid].m_strLabel);
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,HqData[stockid].m_strLabel,strlen(HqData[stockid].m_strLabel));
		    packoflen += strlen(HqData[stockid].m_strLabel);

		    //价格高于           float     (-1表示未设定 以下同)  取跌停价
		    *(float*)(GenBuff+packoflen) = HqData[stockid].m_lowstop;
		    packoflen += sizeof(float);

		    //价格低于           float
		    *(float*)(GenBuff+packoflen) = HqData[stockid].m_highstop;
		    packoflen += sizeof(float);
		    stockid = stockid+1;
	    }
	    else
	    {
		    //取股票代码   string
		    *(short*)(GenBuff+packoflen) = strlen(HqData[stockid].m_strLabel);
		    packoflen += sizeof(unsigned short);
		    memcpy(GenBuff+packoflen,HqData[stockid].m_strLabel,strlen(HqData[stockid].m_strLabel));
		    packoflen += strlen(HqData[stockid].m_strLabel);

		    //价格高于           float     (-1表示未设定 以下同)  取跌停价
		    *(float*)(GenBuff+packoflen) = HqData[stockid].m_lowstop;
		    packoflen += sizeof(float);

		    //价格低于           float
		    *(float*)(GenBuff+packoflen) = HqData[stockid].m_highstop;
		    packoflen += sizeof(float);
		    stockid = stockid+1;
		    if(stockid%20 == 0)
		    {
			    userid = userid+1;
		    }
	    }
	    //日涨幅             float
	    TCHAR uptrip[10] = {0};
	    ::GetPrivateProfileString(Funtype,L"日涨幅",L"0.00",uptrip,10,L".\\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(uptrip);
	    *(float*)(GenBuff+packoflen) = (float)atof(tempUTF.c_str());
	    packoflen += sizeof(float);
	    //日涨幅有效         byte     （0 一次  1 永久）
	    TCHAR upval[2] = {0};
	    ::GetPrivateProfileString(Funtype,L"日涨幅有效",L"3",upval,2,L".\\ALARM.ini");
	    if(tempUTF.length()!=0)
	    {
		    unsigned char upbyte = _ttoi(upval);
		    *(GenBuff+packoflen) = upbyte;
		    packoflen += 1;
	    }
	    //日跌幅             float
	    TCHAR downtrip[10] = {0};
	    ::GetPrivateProfileString(Funtype,L"日跌幅",L"0.00",downtrip,10,L"..\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(downtrip);
	    *(float*)(GenBuff+packoflen) = (float)atof(tempUTF.c_str());
	    packoflen += sizeof(float);
	    //日跌幅有效         byte     （0 一次  1 永久）
	    TCHAR downval[2] = {0};
	    ::GetPrivateProfileString(Funtype,L"日跌幅有效",L"3",downval,2,L".\\ALARM.ini");
	    if(tempUTF.length()!=0)
	    {
		    unsigned char downbyte = _ttoi(downval);
		    *(GenBuff+packoflen) = downbyte;
		    packoflen += 1;
	    }
	    //日换手率             float
	    TCHAR avgtrip[10] = {0};
	    ::GetPrivateProfileString(Funtype,L"日换手率",L"0.00",avgtrip,10,L".\\ALARM.ini");
	    //unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(avgtrip);
	    *(float*)(GenBuff+packoflen) = (float)atof(tempUTF.c_str());
	    packoflen += sizeof(float);
	    //日换手率有效         byte     （0 一次  1 永久）
	    TCHAR avgval[2] = {0};
	    ::GetPrivateProfileString(Funtype,L"日换手率有效",L"3",avgval,2,L".\\ALARM.ini");
	    if(tempUTF.length()!=0)
	    {
		    unsigned char avgbyte = _ttoi(avgval);
		    *(GenBuff+packoflen) = avgbyte;
		    packoflen += 1;
	    }
	    //每日地雷           byte     （0 关闭  1 开启）
	    TCHAR mine[2] = {0};
	    ::GetPrivateProfileString(Funtype,L"日换手率有效",L"3",mine,2,L".\\ALARM.ini");
	    if(tempUTF.length()!=0)
	    {
		    unsigned char minebyte = _ttoi(mine);
		    *(GenBuff+packoflen) = minebyte;
		    packoflen += 1;
	    }
		pHqHead->length = packoflen-sizeof(struct BIG_HEAD);
		pphead->sub_length = pHqHead->length-1-sizeof(struct push_head);
		*buffoflen = packoflen;
	}
	else
	{
        //组大包头
	    struct ACC_CMDHEAD *pBighead = (ACC_CMDHEAD*)GenBuff;
	    pBighead->nExpandInfo = 0;
	    pBighead->wAttr = 0x0200;
	    pBighead->wCmdType = 0xa0a1;
	    packoflen += sizeof(ACC_CMDHEAD);

	    //组小包头
	    push_head *pSmallhead = (push_head*)(GenBuff+sizeof(ACC_CMDHEAD));
	    pSmallhead->sub_attrs = 0;
	    pSmallhead->sub_extend = 0;
	    pSmallhead->sub_type = PackType;

	    packoflen += sizeof(push_head);
	    //组包体部分，对于用户名和密码部分可以
	    //获取功能配置参数
	    CString Funtype = _T("");
	    Funtype.Format(L"%d",PackType);
	    TCHAR username[50] = {0};
	    ::GetPrivateProfileString(Funtype,L"用户名",L"ccc112",username,50,L".\\ALARM.ini");
		//unicode类型转UTF8格式串
	    tempUTF = UTF16_TO_UTF8(username);
	if(tempUTF.length()!=0)
	{
		*(short*)(GenBuff+packoflen) = tempUTF.length();
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		packoflen += tempUTF.length(); 
	}
	//用户密码
	TCHAR userpwd[50] = {0};
	::GetPrivateProfileString(Funtype,L"用户密码",L"111111",userpwd,50,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(userpwd);
	if(tempUTF.length()!=0)
	{
		*(short*)(GenBuff+packoflen) = tempUTF.length();
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		packoflen += tempUTF.length(); 
	}
	//手机号码
	TCHAR tellphone[12] = {0};
	::GetPrivateProfileString(Funtype,L"手机号码",L"11111111111",tellphone,12,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(tellphone);
	if(tempUTF.length()!=0)
	{
		*(short*)(GenBuff+packoflen) = tempUTF.length();
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		packoflen += tempUTF.length(); 
	}
	//手机id             String
	TCHAR tellid[20] = {0};
	::GetPrivateProfileString(Funtype,L"手机id",L"11111111111",tellid,20,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(tellid);
	if(tempUTF.length()!=0)
	{
		*(short*)(GenBuff+packoflen) = tempUTF.length();
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		packoflen += tempUTF.length(); 
	}
	//推送标示           String 
	CString pushmark = _T("");
	if(userid>=UserID.size())
	{
		userid = 0;
		tempUTF.assign(UserID[userid].c_str());
	}
	else
	{
		tempUTF.assign(UserID[userid].c_str());
	}
	if(tempUTF.length()!=0)
	{
		*(short*)(GenBuff+packoflen) = tempUTF.length();
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		packoflen += tempUTF.length(); 
	}
	//平台               byte
	TCHAR ptype[2] = {0};
	::GetPrivateProfileString(Funtype,L"平台",L"3",ptype,2,L".\\ALARM.ini");
	if(tempUTF.length()!=0)
	{
		unsigned char szbyte = _ttoi(ptype);
		*(GenBuff+packoflen) = szbyte;
		packoflen += 1;
	}
	//本地软件版本       String
	TCHAR softver[20] = {0};
	::GetPrivateProfileString(Funtype,L"本地软件版本",L"",softver,20,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(softver);
	if(tempUTF.length()!=0)
	{
		*(short*)(GenBuff+packoflen) = tempUTF.length();
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,tempUTF.c_str(),tempUTF.length());
		packoflen += tempUTF.length(); 
	}
	//股票代码           String
	if(stockid>=HqData.size())
	{
		//取股票代码
		stockid = 0;
		*(short*)(GenBuff+packoflen) = strlen(HqData[stockid].m_strLabel);
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,HqData[stockid].m_strLabel,strlen(HqData[stockid].m_strLabel));
		packoflen += strlen(HqData[stockid].m_strLabel);

		//价格高于           float     (-1表示未设定 以下同)  取跌停价
		*(float*)(GenBuff+packoflen) = HqData[stockid].m_lowstop;
		packoflen += sizeof(float);

		//价格低于           float
		*(float*)(GenBuff+packoflen) = HqData[stockid].m_highstop;
		packoflen += sizeof(float);
		stockid = stockid+1;
	}
	else
	{
		//取股票代码   string
		*(short*)(GenBuff+packoflen) = strlen(HqData[stockid].m_strLabel);
		packoflen += sizeof(unsigned short);
		memcpy(GenBuff+packoflen,HqData[stockid].m_strLabel,strlen(HqData[stockid].m_strLabel));
		packoflen += strlen(HqData[stockid].m_strLabel);

		//价格高于           float     (-1表示未设定 以下同)  取跌停价
		*(float*)(GenBuff+packoflen) = HqData[stockid].m_lowstop;
		packoflen += sizeof(float);

		//价格低于           float
		*(float*)(GenBuff+packoflen) = HqData[stockid].m_highstop;
		packoflen += sizeof(float);
		stockid = stockid+1;
		if(stockid%20 == 0)
		{
			userid = userid+1;
		}
	}
	
	//日涨幅             float
	TCHAR uptrip[10] = {0};
	::GetPrivateProfileString(Funtype,L"日涨幅",L"0.00",uptrip,10,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(uptrip);
	*(float*)(GenBuff+packoflen) = (float)atof(tempUTF.c_str());
	packoflen += sizeof(float);
	//日涨幅有效         byte     （0 一次  1 永久）
	TCHAR upval[2] = {0};
	::GetPrivateProfileString(Funtype,L"日涨幅有效",L"3",upval,2,L".\\ALARM.ini");
	if(tempUTF.length()!=0)
	{
		unsigned char upbyte = _ttoi(upval);
		*(GenBuff+packoflen) = upbyte;
		packoflen += 1;
	}
	//日跌幅             float
	TCHAR downtrip[10] = {0};
	::GetPrivateProfileString(Funtype,L"日跌幅",L"0.00",downtrip,10,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(downtrip);
	*(float*)(GenBuff+packoflen) = (float)atof(tempUTF.c_str());
	packoflen += sizeof(float);
	//日跌幅有效         byte     （0 一次  1 永久）
	TCHAR downval[2] = {0};
	::GetPrivateProfileString(Funtype,L"日跌幅有效",L"3",downval,2,L".\\ALARM.ini");
	if(tempUTF.length()!=0)
	{
		unsigned char downbyte = _ttoi(downval);
		*(GenBuff+packoflen) = downbyte;
		packoflen += 1;
	}
	//日换手率             float
	TCHAR avgtrip[10] = {0};
	::GetPrivateProfileString(Funtype,L"日换手率",L"0.00",avgtrip,10,L".\\ALARM.ini");
	//unicode类型转UTF8格式串
	tempUTF = UTF16_TO_UTF8(avgtrip);
	*(float*)(GenBuff+packoflen) = (float)atof(tempUTF.c_str());
	packoflen += sizeof(float);
	//日换手率有效         byte     （0 一次  1 永久）
	TCHAR avgval[2] = {0};
	::GetPrivateProfileString(Funtype,L"日换手率有效",L"3",avgval,2,L".\\ALARM.ini");
	if(tempUTF.length()!=0)
	{
		unsigned char avgbyte = _ttoi(avgval);
		*(GenBuff+packoflen) = avgbyte;
		packoflen += 1;
	}
	//每日地雷           byte     （0 关闭  1 开启）
	TCHAR mine[2] = {0};
	::GetPrivateProfileString(Funtype,L"日换手率有效",L"3",mine,2,L".\\ALARM.ini");
	if(tempUTF.length()!=0)
	{
		unsigned char minebyte = _ttoi(mine);
		*(GenBuff+packoflen) = minebyte;
		packoflen += 1;
	}
	pBighead->nLen = packoflen - sizeof(ACC_CMDHEAD);
	pSmallhead->sub_length = pBighead->nLen - sizeof(push_head);
	//memcpy(&buffoflen,&packoflen,4);
	*buffoflen = packoflen;
	}
	return true;
}


// 添加用户到map数组中
void CMutilThread::AddUser(void)
{
	int i = 1;
	int accountnum = ::GetPrivateProfileInt(L"ADDALARM",L"USERCOUNT",10,L".\\ALARM.ini");
	UserID.clear();
	char stringstr[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz12345678";
	for(i = 1; i<=accountnum; i++)
	{
		/*
		char *tempstr = new char[17];
		memset(tempstr,0,17);
		
		time_t mt;
		srand((unsigned int)time(&mt));
		int lensize = sizeof(stringstr)-1;
		for(int j = 0; j<16; j++)
		{
			int ff = rand()%lensize+1;
			tempstr[j] = stringstr[ff];
		}
		
		printf("用户名:%s\n",tempstr);
		UserID.insert(map<int,string>::value_type(i-1,tempstr));
		delete []tempstr;
		*/
		string struser;
		char tempstr[10] = {0};
		CString stringtemp = L"";
		::GetPrivateProfileStringW(L"ADDALARM",L"ALARMSTRING",L"DZHGWGWGW",stringtemp.GetBuffer(10),10,L".\\ALARM.ini");
		stringtemp.ReleaseBuffer();
		int stringlen = stringtemp.GetLength();
		for(int hh=0; hh<stringlen; hh++)
		{
			tempstr[hh] = stringtemp.GetAt(hh);
		}
		struser.append(tempstr);
		stringstream inuser;
		inuser<<i;
		string tempuser = inuser.str();
		struser.append(tempuser);
		UserID.insert(map<int,string>::value_type(i-1,struser));
		
	}
}


// 创建socket
int CMutilThread::GenerateSock(void)
{
	int idx = 0;
	int bigflag = ::GetPrivateProfileInt(L"ADDALARM",L"BIGPACKMARK",0,L".\\ALARM.ini");
	for(int h=0; h<Socket_Count; h++)
	{
	SOCKET_INFO socketData;
	SOCKET clientsocket = ::socket(AF_INET,SOCK_STREAM,0);
	if(clientsocket == 0)
	{
		int err = WSAGetLastError();
		printf("SOCKET创建失败:%d\n",err);
	}
	CString ipaddress = _T("");
	//	TCHAR ipaddress[30] = {0};
	::GetPrivateProfileString(L"ADDALARM",L"IPADDR",L"127.0.0.1",ipaddress.GetBuffer(30),30,L".\\ALARM.ini");
	int m_port = ::GetPrivateProfileInt(L"ADDALARM",L"IPPORT",12345,L".\\ALARM.ini");
	char ipvalue[30] = {0};
	::wsprintfA(ipvalue,"%ls",(LPCTSTR)ipaddress);
	//获取连接socket
	for(int i=0; i<ipaddress.GetLength(); i++)
	{
		ipvalue[i] = ipaddress.GetAt(i);
	}
	ipaddress.ReleaseBuffer();
	sockaddr_in serveraddr;
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.S_un.S_addr = inet_addr(ipvalue);
	serveraddr.sin_port = htons(m_port);

	int connret = connect(clientsocket,(sockaddr*)&serveraddr,sizeof(sockaddr));
	if(connret != SOCKET_ERROR)
	{

		if(bigflag==0)
		{
		    //登录之前需要验证下服务器，否则可能连接不成功
		    LOGIN_HEAD loginpack;
		    memset(&loginpack,0,sizeof(LOGIN_HEAD));
		    //获取包头信息
		    loginpack.accCmdHead.wCmdType = ACCCMD_SERVERLOGIN;
		    loginpack.accCmdHead.nLen = sizeof(ACC_SERVERLOGIN);
		    //获取ACC_SERVERLOGIN包体内容
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

		    loginpack.accServerLogin.nSerId = idx+1;
		    int loginret = send(clientsocket,(char*)&loginpack,sizeof(LOGIN_HEAD),0);
		    if(loginret>0)
		    {
			    printf("登录包发送成功,登录包长度:%d\n",loginret);
		    }
			else
			{
				return 0;
			}
		    //把socket保存到数组中
		}
		socketData.g_busy = false;
		socketData.m_Sock = clientsocket;
		socketData.bConnected = true;
		socketData.lidx = idx+1;
	//	pSock->g_mutil.Lock();
		Socket_map.insert(map<int,SOCKET_INFO>::value_type(socketcount,socketData));
		vecSock.push_back(socketData);
	//	pSock->g_mutil.Unlock();
		idx = idx+1;
		socketcount = idx;
	}
	}
	return 1;
}
