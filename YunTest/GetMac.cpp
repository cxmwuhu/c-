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
    temp = GetAdaptersInfo(pInfo,&ulSize);//��һ�����ã���ȡ��������С
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
	
    int iCount=0;
    while(pInfo)//����ÿһ������
    {
        //  pInfo->Address MACַ
		//pInfo->AddressLength mac��ַ�ĳ���
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
WINDOWNS��ȡ����MAC��ַ
#include <winsock2.h>
#include <Iphlpapi.h>
#include <stdio.h>
 
void byte2Hex(unsigned char bData,unsigned char hex[])
{
    int high=bData/16,low =bData %16;
    hex[0] = (high <10)?('0'+high):('A'+high-10);
    hex[1] = (low <10)?('0'+low):('A'+low-10);
}
 
int getLocalMac(unsigned char *mac) //��ȡ����MACַ 
{
    ULONG ulSize=0;
    PIP_ADAPTER_INFO pInfo=NULL;
    int temp=0;
    temp = GetAdaptersInfo(pInfo,&ulSize);//��һ�����ã���ȡ��������С
    pInfo=(PIP_ADAPTER_INFO)malloc(ulSize);
    temp = GetAdaptersInfo(pInfo,&ulSize);
 
    int iCount=0;
    while(pInfo)//����ÿһ������
    {
        //  pInfo->Address MACַ
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

��Ҫ����iphlpapi.lib  ws2_32.lib ��̬�⣨VC��ӹ���LINK��

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

windows��ȡIP��ַ
#include<stdio.h>
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
 
void CheckIP(void) //����CheckIP�������������ڻ�ȡ����IP��ַ
{
 
    WSADATA wsaData;
    char name[255];//�������ڴ�Ż�õ��������ı���
    char *ip;//����IP��ַ����
    PHOSTENT hostinfo;
 
            //����MAKEWORD�������Winsock�汾����ȷֵ�����ڼ���Winsock��
 
    if (WSAStartup(MAKEWORD(2,0), &wsaData) == 0) {
        //�����Ǽ���Winsock�⣬���WSAStartup������������ֵΪ0��˵�����سɹ���������Լ���
        if (gethostname(name, sizeof(name)) == 0) {
            //����ɹ��ؽ������������������name����ָ���Ļ�������
            if ((hostinfo = gethostbyname(name)) != NULL) {
                //���ǻ�ȡ���������������������ɹ��Ļ���������һ��ָ�룬ָ��hostinfo��hostinfo
                //ΪPHOSTENT�͵ı��������漴���õ�����ṹ��
                ip = inet_ntoa(*(struct in_addr *)*hostinfo->h_addr_list);
                //����inet_ntoa������������hostinfo�ṹ�����е�h_addr_listת��Ϊ��׼�ĵ�ֱ�ʾ��IP
                //��ַ����192.168.0.1��
                printf("%s\n",ip);//���IP��ַ
            }
        }
        WSACleanup(); //ж��Winsock�⣬���ͷ�������Դ
    }
}
int main(void)
{
 
    CheckIP();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Liunx C��ȡIP��MAC��ַ  
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
 
 
 
//��ȡ��ַ
//����IP��ַ�ַ���
//���أ�0=�ɹ���-1=ʧ��
int getlocalip(char* outip)
{
    int i=0;
    int sockfd;
    struct ifconf ifconf;
    char buf[512];
    struct ifreq *ifreq;
    char* ip;
    //��ʼ��ifconf
    ifconf.ifc_len = 512;
    ifconf.ifc_buf = buf;
 
    if((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
    {
        return -1;
    }
    ioctl(sockfd, SIOCGIFCONF, &ifconf);    //��ȡ���нӿ���Ϣ
    close(sockfd);
    //������һ��һ���Ļ�ȡIP��ַ
    ifreq = (struct ifreq*)buf;
 
    for(i=(ifconf.ifc_len/sizeof(struct ifreq)); i>0; i--)
    {
        ip = inet_ntoa(((struct sockaddr_in*)&(ifreq->ifr_addr))->sin_addr);
        //�ų�127.0.0.1��������һ��
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
 
//��ȡ��ַ
//����MAC��ַ�ַ���
//���أ�0=�ɹ���-1=ʧ��
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
        printf("����IP��ַ�ǣ� %s\n", ip );
    }
    else
    {
        printf("�޷���ȡ����IP��ַ");
    }
 
    if(get_mac(mac) == 0)
    {
        printf("����MAC��ַ�ǣ� %s\n", mac);
    }
    else
    {
        printf("�޷���ȡ����MAC��ַ");
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

C++ ��ͨ��GetAdaptersInfo��ȡ�������ú�Ip��ַ��Ϣ
һ̨�����Ͽ��ܲ�ֻ��һ����������ÿһ������ֻ��һ��MAC��ַ����ÿһ���������������ж��IP��ַ����ƽ���ıʼǱ������У��ͻ��������������������������߽ӿڣ����֣���ˣ����Ҫ��ñ�������������IP��MAC��ַ��Ϣ�������˳����ÿ�������������λ�ȡ����Ϣ�ȣ���windows sdk�У���IP_ADAPTER_INFO�ṹ��洢������Ϣ����������������������������MAC��ַ������IP�ȣ��ýṹ�����Ҫ����������ʾ��
typedef struct _IP_ADAPTER_INFO {
����struct _IP_ADAPTER_INFO* Next;//ָ����������һ����������Ϣ��ָ��
����DWORD ComboIndex;//Ԥ��ֵ
����char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];//ʹ��ANSI�ַ�����ʾ������������
����char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];//ʹ��ANSI�ַ�����ʾ������������
����UINT AddressLength;//������Ӳ����ַ���ֽڼ���ĳ���
����BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];//Ӳ����ַ��BYTE��������ʾ
����DWORD Index;//����������
    UINT Type;//����������,��Ҫ�����¼��֣�
    
  //   *   MIB_IF_TYPE_OTHER     1
 //    *   MIB_IF_TYPE_ETHERNET     6
 //    *   MIB_IF_TYPE_TOKENRING     9
 //    *   MIB_IF_TYPE_FDDI     15
 //    *   MIB_IF_TYPE_PPP     23
 //    *   MIB_IF_TYPE_LOOPBACK      24
 //    *   MIB_IF_TYPE_SLIP      28
     
����UINT DhcpEnabled;//ָ������������Ƿ���DHCP
����PIP_ADDR_STRING CurrentIpAddress;//Ԥ��ֵ
����IP_ADDR_STRING IpAddressList;//����������IPv4��ַ����
����IP_ADDR_STRING GatewayList;//��������������IPv4��ַ����
����IP_ADDR_STRING DhcpServer;//����������DHCP��������IPv4 ��ַ����
����BOOL HaveWins;
����IP_ADDR_STRING PrimaryWinsServer;
����IP_ADDR_STRING SecondaryWinsServer;
����time_t LeaseObtained;
����time_t LeaseExpires;
����} IP_ADAPTER_INFO,*PIP_ADAPTER_INFO;
���ڿ����ж�����������struct _IP_ADAPTER_INFO* Next�ֶ�Ϊһ������ṹָ�룬����һ�����������ж��IP�����IP_ADDR_STRING�ֶ�Ӧ��Ҳ��һ������ṹ������Ϣ������ʾ��
typedef struct _IP_ADDR_STRING
{
    struct _IP_ADDR_STRING* Next;  //ָ��ͬ���ͽڵ㣬����һ��IP������ж�IP�Ļ���
    IP_ADDRESS_STRING IpAddress;  //IP��ַ��Ϣ
    IP_MASK_STRING IpMask; //IP��������
    DWORD Context;// �������ڡ����ֵ��Ӧ��AddIPAddredd��DeleteIPAddress�����е�NTEContext����
} IP_ADDR_STRING;
 
�ڻ����˽�������Ϣ�󣬾Ϳ��Ե���GetAdaptersInfo��������ȡ���������Ϣ�ˣ���ͨ�õĴ���������ʾ��
#include <WinSock2.h>
#include <Iphlpapi.h>
#include <iostream>
using namespace std;
#pragma comment(lib,"Iphlpapi.lib") //��Ҫ���Iphlpapi.lib��
 
int main(int argc, char* argv[])
{
    //PIP_ADAPTER_INFO�ṹ��ָ��洢����������Ϣ
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    //�õ��ṹ���С,����GetAdaptersInfo����
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    //����GetAdaptersInfo����,���pIpAdapterInfoָ�����;����stSize��������һ��������Ҳ��һ�������
    int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
    //��¼��������
    int netCardNum = 0;
    //��¼ÿ�������ϵ�IP��ַ����
    int IPnumPerNetCard = 0;
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        //����������ص���ERROR_BUFFER_OVERFLOW
        //��˵��GetAdaptersInfo�������ݵ��ڴ�ռ䲻��,ͬʱ�䴫��stSize,��ʾ��Ҫ�Ŀռ��С
        //��Ҳ��˵��ΪʲôstSize����һ��������Ҳ��һ�������
        //�ͷ�ԭ�����ڴ�ռ�
        delete pIpAdapterInfo;
        //���������ڴ�ռ������洢����������Ϣ
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
        //�ٴε���GetAdaptersInfo����,���pIpAdapterInfoָ�����
        nRel=GetAdaptersInfo(pIpAdapterInfo,&stSize);    
    }
    if (ERROR_SUCCESS == nRel)
    {
        //���������Ϣ
         //�����ж�����,���ͨ��ѭ��ȥ�ж�
    while (pIpAdapterInfo)
    {
        cout<<"����������"<<++netCardNum<<endl;
        cout<<"�������ƣ�"<<pIpAdapterInfo->AdapterName<<endl;
        cout<<"����������"<<pIpAdapterInfo->Description<<endl;
        switch(pIpAdapterInfo->Type)
        {
        case MIB_IF_TYPE_OTHER:
            cout<<"�������ͣ�"<<"OTHER"<<endl;
            break;
        case MIB_IF_TYPE_ETHERNET:
            cout<<"�������ͣ�"<<"ETHERNET"<<endl;
            break;
        case MIB_IF_TYPE_TOKENRING:
            cout<<"�������ͣ�"<<"TOKENRING"<<endl;
            break;
        case MIB_IF_TYPE_FDDI:
            cout<<"�������ͣ�"<<"FDDI"<<endl;
            break;
        case MIB_IF_TYPE_PPP:
            printf("PP\n");
            cout<<"�������ͣ�"<<"PPP"<<endl;
            break;
        case MIB_IF_TYPE_LOOPBACK:
            cout<<"�������ͣ�"<<"LOOPBACK"<<endl;
            break;
        case MIB_IF_TYPE_SLIP:
            cout<<"�������ͣ�"<<"SLIP"<<endl;
            break;
        default:
 
            break;
        }
        cout<<"����MAC��ַ��";
        for (DWORD i = 0; i < pIpAdapterInfo->AddressLength; i++)
            if (i < pIpAdapterInfo->AddressLength-1)
            {
                printf("%02X-", pIpAdapterInfo->Address[i]);
            }
            else
            {
                printf("%02X\n", pIpAdapterInfo->Address[i]);
            }
            cout<<"����IP��ַ���£�"<<endl;
            //���������ж�IP,���ͨ��ѭ��ȥ�ж�
            IP_ADDR_STRING *pIpAddrString =&(pIpAdapterInfo->IpAddressList);
            do
            {
                cout<<"�������ϵ�IP������"<<++IPnumPerNetCard<<endl;
                cout<<"IP ��ַ��"<<pIpAddrString->IpAddress.String<<endl;
                cout<<"������ַ��"<<pIpAddrString->IpMask.String<<endl;
                cout<<"���ص�ַ��"<<pIpAdapterInfo->GatewayList.IpAddress.String<<endl;
                pIpAddrString=pIpAddrString->Next;
            } while (pIpAddrString);
            pIpAdapterInfo = pIpAdapterInfo->Next;
            cout<<"--------------------------------------------------------------------"<<endl;
    }
     
    }
    //�ͷ��ڴ�ռ�
    if (pIpAdapterInfo)
    {
        delete pIpAdapterInfo;
    }
  
    return 0;

*/