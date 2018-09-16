#include "StdAfx.h"
#include "HqSocket.h"


CHqSocket::CHqSocket(void)
{
	WSADATA wsadata;
	m_Socket = INVALID_SOCKET;
	WORD wRequestVersion;
	wRequestVersion = MAKEWORD(2,2);
	int err = WSAStartup(wRequestVersion,&wsadata);
	if(err == 0)
	{
		//		loghq.WriteLog("初始化socket成功");
	}
}


CHqSocket::~CHqSocket(void)
{
	if(m_Socket!=INVALID_SOCKET)
	{
		closesocket(m_Socket);
	}
}

int CHqSocket::ConnectSvr(char * lpszIpAddr, UINT nPort, int nSecond)
{
	m_Socket = ::socket(AF_INET,SOCK_STREAM,0);
	if(m_Socket==0)
	{
	//	loghq.WriteLog("SOCKET创建失败:%d",WSAGetLastError());
		return 0;
	}
	sockaddr_in sockserver;
	sockserver.sin_family = AF_INET;
	sockserver.sin_addr.S_un.S_addr = inet_addr(lpszIpAddr);
	sockserver.sin_port = htons(nPort);

	int ret = connect(m_Socket,(sockaddr*)&sockserver,sizeof(sockaddr));
	if(ret == SOCKET_ERROR)
	{
	//	loghq.WriteLog("socket connect fail:%d",WSAGetLastError());
	}
	fd_set fd;
	//	if(ret == WSAEWOULDBLOCK)
	{
		FD_ZERO(&fd);
		FD_SET(m_Socket,&fd);
		FD_SET(m_Socket,&fd);
		timeval tv;
		tv.tv_sec = nSecond;
		tv.tv_usec = 0;
		int retselect = select(0,0,&fd,0,&tv);
		if(retselect == SOCKET_ERROR)
		{
			closesocket(m_Socket);
		//	loghq.WriteLog("socket连接超时:%d",WSAGetLastError());
			return -1;
		}
	}
	return ret;
}

int CHqSocket::SetNoBlock(bool opflag)
{
	unsigned long int af=opflag?1:0;
	return ioctlsocket(m_Socket,FIONBIO,&af);
}

int CHqSocket::SendPack(const char *psend, int nbytes, int nSecond)
{
	//循环发送，防止一个包太大，发送不完
	int sendbytes=0,num=0;
	while(nbytes>0)
	{
		fd_set fdsend;
		struct timeval timetv;
		FD_ZERO(&fdsend);
		FD_SET(m_Socket,&fdsend);

		timetv.tv_sec = nSecond;
		timetv.tv_usec = 0;

		int sendret = select(0,0,&fdsend,0,&timetv);

		if(sendret<=0)
		{
			num = -1;
			//	break;
		}
		sendbytes = send(m_Socket,psend,nbytes,0);
		if(sendbytes<=0)
		{
		//	loghq.WriteLog("发送失败:%d",WSAGetLastError());
			num=-1;
			break;
		}
		num += sendbytes;
		nbytes -= sendbytes;
		psend = psend+sendbytes;
	}
	return num;
}

int CHqSocket::RecvPack(char *precv, int nbytes, int nSecond)
{
	int recvbytes=0,num=0;
	while(nbytes>0)
	{
		fd_set fdrecv;
		struct timeval recvtime;
		FD_ZERO(&fdrecv);
		FD_SET(m_Socket,&fdrecv);

		recvtime.tv_sec = nSecond;
		recvtime.tv_usec = 0;
		int recvret = select(0,&fdrecv,0,0,&recvtime);
		if(recvret<=0)
		{
			num = -1;
			//	break;
		}
		if ( FD_ISSET(m_Socket,&fdrecv) )
		{
			recvbytes = recv(m_Socket,precv,nbytes,0);
			if(recvbytes<=0)
			{
			//	loghq.WriteLog("收包失败:%d",WSAGetLastError());
				num = -1;
				break;
			}
			num += recvbytes;
			nbytes -= recvbytes;
			precv = precv+recvbytes;
		}
	}
	return num;
}


int CHqSocket::ConnectServer(char * lpszIpAddr, UINT nPort)
{
	m_Socket = ::socket(AF_INET,SOCK_STREAM,0);
	if(m_Socket==0)
	{
		//	loghq.WriteLog("SOCKET创建失败:%d",WSAGetLastError());
		return 0;
	}
	sockaddr_in sockserver;
	sockserver.sin_family = AF_INET;
	sockserver.sin_addr.S_un.S_addr = inet_addr(lpszIpAddr);
	sockserver.sin_port = htons(nPort);

	int ret = connect(m_Socket,(sockaddr*)&sockserver,sizeof(sockaddr));
	if(ret == SOCKET_ERROR)
	{
		int errconn = WSAGetLastError();
		printf("socket连接失败:errconn\n",errconn);
	}
	return ret;
}

int CHqSocket::SendMsg(const char *psend, int nbytes)
{
	//循环发送，防止一个包太大，发送不完
	int sendbytes=0,num=0;
	while(nbytes>0)
	{
		sendbytes = send(m_Socket,psend,nbytes,0);
		if(sendbytes<=0)
		{
			int senderr = WSAGetLastError();
			printf("发送数据失败:d\n",senderr);
			num=-1;
			break;
		}
		num += sendbytes;
		nbytes -= sendbytes;
		psend = psend+sendbytes;
	}
	return num;
}

int CHqSocket::RecvMsg(char *precv, int nbytes)
{
	int recvbytes=0,num=0;
	while(nbytes>0)
	{
		recvbytes = recv(m_Socket,precv,nbytes,0);
		if(recvbytes<=0)
		{
			int recverr = WSAGetLastError();
			printf("接收数据失败:%d\n",recverr);
			num = -1;
			break;
		}
		num += recvbytes;
		nbytes -= recvbytes;
		precv = precv+recvbytes;
	}
	return num;
}


void CHqSocket::Close(SOCKET s)
{
	if(m_Socket!=INVALID_SOCKET)
	{
		closesocket(m_Socket);
		m_Socket = INVALID_SOCKET;
	}
	return;
}
