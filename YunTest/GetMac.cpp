#include "StdAfx.h"
#include "GetMac.h"


void byte2Hex(unsigned char bData,unsigned char hex[])
{
    int high=bData/16,low =bData %16;
    hex[0] = (high <10)?('0'+high):('A'+high-10);
    hex[1] = (low <10)?('0'+low):('A'+low-10);
}

CGetMac::CGetMac(void)
{
}


CGetMac::~CGetMac(void)
{
}


int CGetMac::getLocalMac(unsigned char * mac)
{
	int ilen = 0;
	
	macaddr tempstr;
	memset(tempstr.macstring,0,sizeof(tempstr));
	ULONG ulSize=0;
    PIP_ADAPTER_INFO pInfo=NULL;
    int temp=0;
    temp = GetAdaptersInfo(pInfo,&ulSize);//第一处调用，获取缓冲区大小
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
	
    int iCount=0;
    while(pInfo)//遍历每一张网卡
    {
        //  pInfo->Address MAC址
		//pInfo->AddressLength mac地址的长度
        for(int i=0;i<(int)pInfo->AddressLength;i++)
        {
            byte2Hex(pInfo->Address[i],&mac[iCount]);
            iCount+=2;
			
            if(i<(int)pInfo->AddressLength-1)
            {
                mac[iCount++] = ':';
            }else
            {
				strncpy(tempstr.macstring,(const char*)(mac+ilen),iCount-ilen);
				ilen = iCount;
				macstring.push_back(tempstr);
            }
        }
        pInfo = pInfo->Next;
    }
	
    if(iCount >0)
    {
        mac[--iCount]='\0';
        return iCount;
    }
    else return -1;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//DEV C++
/*
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Dev C++ -> include libnetapi32.a
BCC 5.5 or VC++ -> #pragma comment(lib,"netapi32.lib")

 
typedef struct _ASTAT_
{
    ADAPTER_STATUS adapt;
    NAME_BUFFER NameBuff [30];
} ASTAT, *PASTAT;
 
void getMac(char * mac)
{
    ASTAT Adapter;
    NCB Ncb;
    UCHAR uRetCode;
    LANA_ENUM lenum;
    int i = 0;
 
    memset(&Ncb, 0, sizeof(Ncb));
    Ncb.ncb_command = NCBENUM;
    Ncb.ncb_buffer = (UCHAR *)&lenum;
    Ncb.ncb_length = sizeof(lenum);
 
    uRetCode = Netbios( &Ncb );
    printf( "The NCBENUM return adapter number is: %d \n ", lenum.length);
    for(i=0; i < lenum.length ; i++)
    {
        memset(&Ncb, 0, sizeof(Ncb));
        Ncb.ncb_command = NCBRESET;
        Ncb.ncb_lana_num = lenum.lana[i];
        uRetCode = Netbios( &Ncb );
 
        memset(&Ncb, 0, sizeof(Ncb));
        Ncb.ncb_command = NCBASTAT;
        Ncb.ncb_lana_num = lenum.lana[i];
        strcpy((char *)Ncb.ncb_callname, "* ");
        Ncb.ncb_buffer = (unsigned char *) &Adapter;
        Ncb.ncb_length = sizeof(Adapter);
        uRetCode = Netbios( &Ncb );
 
        if (uRetCode == 0)
        {
            //sprintf(mac, "%02x-%02x-%02x-%02x-%02x-%02x ",
            sprintf(mac, "%02X%02X%02X%02X%02X%02X ",
                    Adapter.adapt.adapter_address[0],
                    Adapter.adapt.adapter_address[1],
                    Adapter.adapt.adapter_address[2],
                    Adapter.adapt.adapter_address[3],
                    Adapter.adapt.adapter_address[4],
                    Adapter.adapt.adapter_address[5]
                   );
            //printf( "The Ethernet Number on LANA %d is: %s\n ", lenum.lana[i], mac);
        }
    }
}
 
int main(int argc, char *argv[])
{
    char *mac=new char[32];
    getMac(mac);
    printf( "%s\n ", mac);
    delete[]mac;
    system( "PAUSE> NUL ");
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
WINDOWNS获取本机MAC地址
#include <winsock2.h>
#include <Iphlpapi.h>
#include <stdio.h>
 
void byte2Hex(unsigned char bData,unsigned char hex[])
{
    int high=bData/16,low =bData %16;
    hex[0] = (high <10)?('0'+high):('A'+high-10);
    hex[1] = (low <10)?('0'+low):('A'+low-10);
}
 
int getLocalMac(unsigned char *mac) //获取本机MAC址 
{
    ULONG ulSize=0;
    PIP_ADAPTER_INFO pInfo=NULL;
    int temp=0;
    temp = GetAdaptersInfo(pInfo,&ulSize);//第一处调用，获取缓冲区大小
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
 
    int iCount=0;
    while(pInfo)//遍历每一张网卡
    {
        //  pInfo->Address MAC址
        for(int i=0;i<(int)pInfo->AddressLength;i++)
        {
            byte2Hex(pInfo->Address[i],&mac[iCount]);
            iCount+=2;
            if(i<(int)pInfo->AddressLength-1)
            {
                mac[iCount++] = ':';
            }else
            {
                mac[iCount++] = '#';
            }
        }
        pInfo = pInfo->Next;
    }
 
    if(iCount >0)
    {
        mac[--iCount]='\0';
        return iCount;
    }
    else return -1;
}
 
int main(int argc, char* argv[])
{
    unsigned char address[1024];
    if(getLocalMac(address)>0)
    {
        printf("mac-%s\n",address);
    }else
    {
        printf("invoke getMAC error!\n");
    }
    return 0;
}

需要两：iphlpapi.lib  ws2_32.lib 静态库（VC添加工程LINK）

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

windows获取IP地址
#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
 
void CheckIP(void) //定义CheckIP（）函数，用于获取本机IP地址
{
 
    WSADATA wsaData;
    char name[255];//定义用于存放获得的主机名的变量
    char *ip;//定义IP地址变量
    PHOSTENT hostinfo;
 
            //调用MAKEWORD（）获得Winsock版本的正确值，用于加载Winsock库
 
    if (WSAStartup(MAKEWORD(2,0), &wsaData) == 0) {
        //现在是加载Winsock库，如果WSAStartup（）函数返回值为0，说明加载成功，程序可以继续
        if (gethostname(name, sizeof(name)) == 0) {
            //如果成功地将本地主机名存放入由name参数指定的缓冲区中
            if ((hostinfo = gethostbyname(name)) != NULL) {
                //这是获取主机名，如果获得主机名成功的话，将返回一个指针，指向hostinfo，hostinfo
                //为PHOSTENT型的变量，下面即将用到这个结构体
                ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
                //调用inet_ntoa（）函数，将hostinfo结构变量中的h_addr_list转化为标准的点分表示的IP
                //地址（如192.168.0.1）
                printf("%s\n",ip);//输出IP地址
            }
        }
        WSACleanup(); //卸载Winsock库，并释放所有资源
    }
}
int main(void)
{
 
    CheckIP();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Liunx C获取IP、MAC地址  
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
 
 
 
//获取地址
//返回IP地址字符串
//返回：0=成功，-1=失败
int getlocalip(char* outip)
{
    int i=0;
    int sockfd;
    struct ifconf ifconf;
    char buf[512];
    struct ifreq *ifreq;
    char* ip;
    //初始化ifconf
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
 
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        return -1;
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);    //获取所有接口信息
    close(sockfd);
    //接下来一个一个的获取IP地址
    ifreq = (struct ifreq*)buf;
 
    for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)
    {
        ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
        //排除127.0.0.1，继续下一个
        if(strcmp(ip,"127.0.0.1")==0)
        {
            ifreq++;
            continue;
        }
        strcpy(outip,ip);
        return 0;
    }
 
    return -1;
}
 
//获取地址
//返回MAC地址字符串
//返回：0=成功，-1=失败
int get_mac(char* mac)
{
    struct ifreq tmp;
    int sock_mac;
    char mac_addr[30];
    sock_mac = socket(AF_INET, SOCK_STREAM, 0);
    if( sock_mac == -1)
    {
        perror("create socket fail\n");
        return -1;
    }
    memset(&tmp,0,sizeof(tmp));
    strncpy(tmp.ifr_name,"eth0",sizeof(tmp.ifr_name)-1 );
    if( (ioctl( sock_mac, SIOCGIFHWADDR, &tmp)) < 0 )
    {
        printf("mac ioctl error\n");
        return -1;
    }
    sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)tmp.ifr_hwaddr.sa_data[0],
            (unsigned char)tmp.ifr_hwaddr.sa_data[1],
            (unsigned char)tmp.ifr_hwaddr.sa_data[2],
            (unsigned char)tmp.ifr_hwaddr.sa_data[3],
            (unsigned char)tmp.ifr_hwaddr.sa_data[4],
            (unsigned char)tmp.ifr_hwaddr.sa_data[5]
            );
    close(sock_mac);
    memcpy(mac,mac_addr,strlen(mac_addr));
    return 0;
}
 
int main(void)
{
    char ip[20];
    char mac[17];
 
    if ( getlocalip( ip ) == 0 )
    {
        printf("本机IP地址是： %s\n", ip );
    }
    else
    {
        printf("无法获取本机IP地址");
    }
 
    if(get_mac(mac) == 0)
    {
        printf("本机MAC地址是： %s\n", mac);
    }
    else
    {
        printf("无法获取本机MAC地址");
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

C++ 中通过GetAdaptersInfo获取网卡配置和Ip地址信息
一台机器上可能不只有一个网卡，但每一个网卡只有一个MAC地址，而每一个网卡可能配置有多个IP地址；如平常的笔记本电脑中，就会有无线网卡和有线网卡（网线接口）两种；因此，如果要获得本机所有网卡的IP和MAC地址信息，则必须顺序获得每个网卡，再依次获取其信息等；在windows sdk中，用IP_ADAPTER_INFO结构体存储网卡信息，包括网卡名、网卡描述、网卡MAC地址、网卡IP等，该结构体的主要描述如下所示：
typedef struct _IP_ADAPTER_INFO {
　　struct _IP_ADAPTER_INFO* Next;//指向链表中下一个适配器信息的指针
　　DWORD ComboIndex;//预留值
　　char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];//使用ANSI字符串表示的适配器名称
　　char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];//使用ANSI字符串表示的适配器描述
　　UINT AddressLength;//适配器硬件地址以字节计算的长度
　　BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];//硬件地址以BYTE数组所表示
　　DWORD Index;//适配器索引
    UINT Type;//适配器类型,主要有以下几种：
    
  //   *   MIB_IF_TYPE_OTHER     1
 //    *   MIB_IF_TYPE_ETHERNET     6
 //    *   MIB_IF_TYPE_TOKENRING     9
 //    *   MIB_IF_TYPE_FDDI     15
 //    *   MIB_IF_TYPE_PPP     23
 //    *   MIB_IF_TYPE_LOOPBACK      24
 //    *   MIB_IF_TYPE_SLIP      28
     
　　UINT DhcpEnabled;//指定这个适配器是否开启DHCP
　　PIP_ADDR_STRING CurrentIpAddress;//预留值
　　IP_ADDR_STRING IpAddressList;//该适配器的IPv4地址链表
　　IP_ADDR_STRING GatewayList;//该适配器的网关IPv4地址链表
　　IP_ADDR_STRING DhcpServer;//该适配器的DHCP服务器的IPv4 地址链表
　　BOOL HaveWins;
　　IP_ADDR_STRING PrimaryWinsServer;
　　IP_ADDR_STRING SecondaryWinsServer;
　　time_t LeaseObtained;
　　time_t LeaseExpires;
　　} IP_ADAPTER_INFO,*PIP_ADAPTER_INFO;
由于可能有多个网卡，因此struct _IP_ADAPTER_INFO* Next字段为一个链表结构指针，由于一个网卡可能有多个IP，因此IP_ADDR_STRING字段应该也是一个链表结构，其信息如下所示：
typedef struct _IP_ADDR_STRING
{
    struct _IP_ADDR_STRING* Next;  //指向同类型节点，即下一个IP（如果有多IP的话）
    IP_ADDRESS_STRING IpAddress;  //IP地址信息
    IP_MASK_STRING IpMask; //IP子网掩码
    DWORD Context;// 网络表入口。这个值对应着AddIPAddredd和DeleteIPAddress函数中的NTEContext参数
} IP_ADDR_STRING;
 
在基本了解以上信息后，就可以调用GetAdaptersInfo函数来获取相关网卡信息了，其通用的代码如下所示：
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"Iphlpapi.lib") //需要添加Iphlpapi.lib库
 
int main(int argc, char* argv[])
{
    //PIP_ADAPTER_INFO结构体指针存储本机网卡信息
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    //得到结构体大小,用于GetAdaptersInfo参数
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
    int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
    //记录网卡数量
    int netCardNum = 0;
    //记录每张网卡上的IP地址数量
    int IPnumPerNetCard = 0;
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        //如果函数返回的是ERROR_BUFFER_OVERFLOW
        //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //这也是说明为什么stSize既是一个输入量也是一个输出量
        //释放原来的内存空间
        delete pIpAdapterInfo;
        //重新申请内存空间用来存储所有网卡信息
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
        //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
        nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
    }
    if (ERROR_SUCCESS == nRel)
    {
        //输出网卡信息
         //可能有多网卡,因此通过循环去判断
    while (pIpAdapterInfo)
    {
        cout<<"网卡数量："<<++netCardNum<<endl;
        cout<<"网卡名称："<<pIpAdapterInfo->AdapterName<<endl;
        cout<<"网卡描述："<<pIpAdapterInfo->Description<<endl;
        switch(pIpAdapterInfo->Type)
        {
        case MIB_IF_TYPE_OTHER:
            cout<<"网卡类型："<<"OTHER"<<endl;
            break;
        case MIB_IF_TYPE_ETHERNET:
            cout<<"网卡类型："<<"ETHERNET"<<endl;
            break;
        case MIB_IF_TYPE_TOKENRING:
            cout<<"网卡类型："<<"TOKENRING"<<endl;
            break;
        case MIB_IF_TYPE_FDDI:
            cout<<"网卡类型："<<"FDDI"<<endl;
            break;
        case MIB_IF_TYPE_PPP:
            printf("PP\n");
            cout<<"网卡类型："<<"PPP"<<endl;
            break;
        case MIB_IF_TYPE_LOOPBACK:
            cout<<"网卡类型："<<"LOOPBACK"<<endl;
            break;
        case MIB_IF_TYPE_SLIP:
            cout<<"网卡类型："<<"SLIP"<<endl;
            break;
        default:
 
            break;
        }
        cout<<"网卡MAC地址：";
        for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
            if (i < pIpAdapterInfo->AddressLength-1)
            {
                printf("%02X-", pIpAdapterInfo->Address[i]);
            }
            else
            {
                printf("%02X\n", pIpAdapterInfo->Address[i]);
            }
            cout<<"网卡IP地址如下："<<endl;
            //可能网卡有多IP,因此通过循环去判断
            IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
            do
            {
                cout<<"该网卡上的IP数量："<<++IPnumPerNetCard<<endl;
                cout<<"IP 地址："<<pIpAddrString->IpAddress.String<<endl;
                cout<<"子网地址："<<pIpAddrString->IpMask.String<<endl;
                cout<<"网关地址："<<pIpAdapterInfo->GatewayList.IpAddress.String<<endl;
                pIpAddrString=pIpAddrString->Next;
            } while (pIpAddrString);
            pIpAdapterInfo = pIpAdapterInfo->Next;
            cout<<"--------------------------------------------------------------------"<<endl;
    }
     
    }
    //释放内存空间
    if (pIpAdapterInfo)
    {
        delete pIpAdapterInfo;
    }
  
    return 0;

*/