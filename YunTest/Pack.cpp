#include "StdAfx.h"
#include "Pack.h"

//自定义ascii到utf8转换函数;
 void AsciiTUtf(LPCSTR srcStr,LPWSTR dstStr)
 {
	 DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, srcStr, -1, NULL, 0);
	 MultiByteToWideChar (CP_UTF8, 0,srcStr, -1, dstStr, dwNum);
 }

 /*
//定义UTF16到UTF8转换;
 char * UTF16_T_UTF8( const wstring& strUTF16 )  
 {  
	 int nUTF8Length = ::WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,NULL,0,0,0); 
	 char * pStrUtf8=new char[nUTF8Length];
	 WideCharToMultiByte(CP_UTF8,0,strUTF16.c_str(),-1,pStrUtf8,nUTF8Length,0,0);  
	 return pStrUtf8;  
 }  
 */

//定义UTF8到UTF16转换;
 wstring UTF8_TO_UTF16( const string& strUTF8 )  
 {  
	 int nUTF16Length = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,NULL,0);  
	 nUTF16Length += 1;  
	 wstring strUTF16(nUTF16Length ,' ');  
	 int nRet = ::MultiByteToWideChar(CP_UTF8,0,strUTF8.c_str(),-1,&strUTF16[0],nUTF16Length);  
	 ASSERT(0 != nRet);  
	 return strUTF16;  
 }  

 //计算CRC32值
 unsigned int GetCrc32Val(char* InStr,unsigned int len)
 {     
    //生成Crc32的查询表  
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
    //开始计算CRC32校验值  
    Crc=0xffffffff;     
    for(unsigned int i=0; i<len; i++){    
        Crc = (Crc >> 8)^ Crc32Table[(Crc & 0xFF) ^ InStr[i]];     
    }  
      
    Crc ^= 0xFFFFFFFF;  
    return Crc;     
}

 //十进制转二进制-short类型
char *short_to_byte(unsigned short data,char *str)
{
	static int shortlen = sizeof(unsigned short)*8;
	for(int f=shortlen-1;f>=0;f--,data>>=1)
	{
		str[f] = (01&data)+'\0';
	}
	str[shortlen] = '\0';
	return str;
}

//十进制转二进制
char *int_to_byte(int n,char *b)
{
    static int LEN=8*sizeof(int);
    for(int i=LEN-1;i>=0;i--,n>>=1)
        b[i]=(01&n)+'0';
    b[LEN]='\0';
    return b;
}

//十进制转二进制
char *int64_to_byte(__int64 n,char *b)
{
    static int LEN=8*sizeof(__int64);
    for(int i=LEN-1;i>=0;i--,n>>=1)
        b[i]=(01&n)+'0';
    b[LEN]='\0';
    return b;
}

//定义到URLencode的转换;
/*
 inline BYTE_to_Hex(const BYTE &x)
 {
	 return x > 9 ? x + 55: x + 48;
 }
 */

CPack::CPack(void)
{
}


CPack::~CPack(void)
{
}

/*
/////////////////////////////////////////////////////////////////////////////////////////
	//根据解包的字典来解析函数,首先判断flag的个数
	int flagnum = Xml.AnsData[PackType].size();
	map<int,map<int,vector<DictFun>>>::iterator map_it;
	map<int,vector<DictFun>> map_val = map_it->second;
	if(map_it->first == 0)
	{
		//answer字典里面的字典数据大小
		int size = map_val[0].size();
		m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
		for(int h=0; h<size; h++)
		{
			m_ListCtl.InsertColumn(h+1,map_val[0][h].Dictname,LVCFMT_LEFT,100);
			//查找ansstruct的结构个数
			int d = 0;
			for(d =0; d<15; d++)
			{
				if(_tcscmp(map_val[0][h].Dicttype,pAnsStruct[d]) == 0)
					break;
			}
			switch(d)
			{
			case ans_stringarray:
				{
					//解string数组个数
					short strcount = *(short*)(PackBody+offset);
					offset += 2;
					for(int t=0;t<strcount;t++)
					{
						//字符串数组里面每个字符串的长度
						short strlen = *(short*)(PackBody+offset);
						offset += 2;
						char *stringval = new char[strlen+1];
						memset(stringval,0,strlen+1);
						memcpy(stringval,PackBody+offset,strlen);
						offset += strlen;
						wstring stringstr = UTF8_2_UTF16(stringval);
						m_ListCtl.SetItemText(t,h+1,stringstr.c_str());
						delete []stringval;
					}
				}break;
			case ans_unit64:    //8字节无符号整型
				{
					 unsigned __int64 unit64val = *(unsigned __int64*)(PackBody+offset);
			         offset += 8;
			         CString tempunit64 = _T("");tempunit64.Format(L"%u",unit64val);
			         m_ListCtl.SetItemText(0, h+1, tempunit64);//设置其它列数据
				}break;
			case ans_string:   //应答字符串位字符串
				{
					short Noticelength = *(short*)(PackBody+offset);   //公告信息字符串长度
			        offset += 2;
				    if(Noticelength!=0)
				    {
			            char *NoticeMsg = new char[Noticelength+1];
			            memset(NoticeMsg,0,Noticelength+1);
			            memcpy(NoticeMsg,PackBody+offset,Noticelength);
			            offset+=Noticelength;
			            wstring str = UTF8_2_UTF16(NoticeMsg);
				        CString wNotice = _T("");wNotice.Format(L"%ws",str.c_str());
			            m_ListCtl.SetItemText(0, h+1, wNotice);//设置其它列数据
			            delete []NoticeMsg;
				    }
				}break;
			case ans_byte:  //解byte字段
				{
					//解  是否通过       byte       1通过 0没通过
				    unsigned char chPass = *(PackBody+offset);
				    offset += 1;
				    int szch = chPass;
				    CString PassVal=_T("");PassVal.Format(L"%d",szch);
				    m_ListCtl.SetItemText(0,h+1,PassVal);
				}break;
			case ans_char:
				{
					char charval = *(PackBody+offset);
				    offset += 1;
				    int szchar = charval;
				    CString charVal=_T("");charVal.Format(L"%d",szchar);
				    m_ListCtl.SetItemText(0,0,L"1");
				    m_ListCtl.SetItemText(0,h+1,charVal);
				}break;
			case ans_short:
				{
					 short shortval = *(short*)(PackBody+offset);
			         offset += 2;
			         CString tempshort = _T("");tempshort.Format(L"%u",shortval);
			         m_ListCtl.SetItemText(0, h+1, tempshort);//设置其它列数据
				}break;
			case ans_int:
				{
					int intval = *(int*)(PackBody+offset);
			        offset += 4;
			        CString tempint = _T("");tempint.Format(L"%d",intval);
			        m_ListCtl.SetItemText(0, h+1, tempint);//设置其它列数据
				}break;
			case ans_int24:
				{
					char *dataval = new char[4];
					memset(dataval,0,4);
					memcpy(dataval,PackBody+offset,3);
					offset += 3;
					wstring datavalstr = UTF8_2_UTF16(dataval);
					m_ListCtl.SetItemText(0,h+1,datavalstr.c_str());
					delete []dataval;
				}break;
			case ans_bytearray:
				{
					short picdatalen = *(short*)(PackBody+offset);
				    offset += 2;
				    char *picdata = new char[picdatalen+1];
				    memset(picdata,0,picdatalen+1);
				    memcpy(picdata,PackBody+offset,picdatalen);
				    wstring picdatastr = UTF8_2_UTF16(picdata);
				    m_ListCtl.SetItemText(0,h+1,picdatastr.c_str());
				    delete []picdata;
				}break;
			case ans_long:
				{
					unsigned __int64 long8val = *(unsigned __int64*)(PackBody+offset);
			        offset += 8;
			        CString templong8 = _T("");templong8.Format(L"%I64d",long8val);
			        m_ListCtl.SetItemText(0, h+1, templong8);//设置其它列数据
				}break;
			case ans_long64:
				{
					unsigned __int64 long64val = *(unsigned __int64*)(PackBody+offset);
			        offset += 8;
			        CString templong64 = _T("");templong64.Format(L"%I64d",long64val);
			        m_ListCtl.SetItemText(0, h+1, templong64);//设置其它列数据
				}break;
			case ans_intarray:
				{
					short sznum = *(short*)(PackBody+offset);
			        offset += 2;
			        for(int j=0; j<sznum; j++)
			        {
				        int data = *(int*)(PackBody+offset);
				        offset += 4;
				        CString datastr = _T("");datastr.Format(L"%d",data);
				        m_ListCtl.SetItemText(j,h+1,datastr);
			        }
				}break;
			case ans_customdata:
				{
					short customdatalen = *(short*)(PackBody+offset);   //公告信息字符串长度
			        offset += 2;
				    if(customdatalen!=0)
				    {
			            char *customdataMsg = new char[customdatalen+1];
			            memset(customdataMsg,0,customdatalen+1);
			            memcpy(customdataMsg,PackBody+offset,customdatalen);
			            offset+=customdatalen;
			            wstring customstr = UTF8_2_UTF16(customdataMsg);
						m_ListCtl.SetItemText(0, h+1, customstr.c_str());//设置其它列数据
			            delete []customdataMsg;
				    }
				}break;
			case ans_130data:
				{
				}break;
			default:
				break;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	*/

// 解包函数
void CPack::CDataPack(char * &Datapack, unsigned short PackLen, unsigned short PackType)
{
		//单个解包，对每一个包进行解包，配置文档比较混淆，一个个的解包比较清晰，解包比较繁琐
	dlgtest.m_ListCtl.DeleteAllItems();
	int nHeadNum = dlgtest.m_ListCtl.GetHeaderCtrl()->GetItemCount();
	for(int i=nHeadNum-1; i>=0; i--)
	{
        dlgtest.m_ListCtl.DeleteColumn (i);
	}
	for(int n=1;n<=500;n++)
    {
        dlgtest.m_ListCtl.InsertItem(n,_T(""));
		dlgtest.m_ListCtl.SetItemText(n,0,(LPCTSTR)(n+1));
    }
	int offset=0;
	switch(PackType)
	{
	case 1000:
		{
			//返回应答包的字段个数,map数组里面的个数
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			dlgtest.m_ListCtl.InsertColumn(1,L"行情地址",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"委托地址",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"公告信息",LVCFMT_LEFT,150);
			dlgtest.m_ListCtl.InsertColumn(4,L"新版本号",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"下载地址",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"提醒升级",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"强制升级",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"是否提示登录",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"用户运营商ip类",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"统计信息时间间隔",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(11,L"升级提示文字",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(12,L"公告crc",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(13,L"公告提示类型",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(14,L"调度地址",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(15,L"服务ID个数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(16,L"服务id",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(17,L"服务器地址",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			//解应答包里面的行情地址字段，此字段是字符串数组格式string【】
			//解行情数组个数
			short hqaddrnum = *(short*)(Datapack+offset);
			offset += 2;
			//解行情地址长度字段
			int nRow = 0;
			if(PackLen>offset)
			{
			    for(int j=0;j<hqaddrnum;j++)
			    {
				    short hqaddrlength = *(short*)(Datapack+offset);
				    offset += 2;
				    char *hqaddr = new char[hqaddrlength+1];
				    memset(hqaddr,0,hqaddrlength+1);
				    memcpy(hqaddr,Datapack+offset,hqaddrlength);
				    offset += hqaddrlength;
				    wstring str = UTF8_TO_UTF16(hqaddr);
				    CString No = _T("");No.Format(L"%d",j+1);
				    //nRow = dlgtest.m_ListCtl.InsertItem(j,No);// 插入行
				    dlgtest.m_ListCtl.SetItemText(j,0,No);
				    dlgtest.m_ListCtl.SetItemText(j, 1, str.c_str());//设置其它列数据
				    delete []hqaddr;
			    }
			}
			//解析委托地址字段
			if(PackLen>offset)
			{
			    short Entrustaddrnum = *(short*)(Datapack+offset);   //委托地址个数
			    offset += 2;
			    for(int t=0;t<Entrustaddrnum;t++)
			    {
				    short enlength = *(short*)(Datapack+offset);
				    offset += 2;
				    char *Enaddr = new char[enlength+1];
				    memset(Enaddr,0,enlength+1);
				    memcpy(Enaddr,Datapack+offset,enlength);
				    offset += enlength;
				    wstring str = UTF8_TO_UTF16(Enaddr);
				    CString No = _T("");No.Format(L"%d",t+1);
				    dlgtest.m_ListCtl.SetItemText(t,0,No);
				    dlgtest.m_ListCtl.SetItemText(t, 2, str.c_str());//设置其它列数据
				    delete []Enaddr;
			    }
			}
			//解公告信息  string类型
			if(PackLen>offset)
			{
			    short Noticelength = *(short*)(Datapack+offset);   //公告信息字符串长度
			    offset += 2;
				if(Noticelength!=0)
				{
			    char *NoticeMsg = new char[Noticelength+1];
			    memset(NoticeMsg,0,Noticelength+1);
			    memcpy(NoticeMsg,Datapack+offset,Noticelength);
			    offset+=Noticelength;
			    wstring str = UTF8_TO_UTF16(NoticeMsg);
				CString wNotice = _T("");wNotice.Format(L"%ws",str.c_str());
			    dlgtest.m_ListCtl.SetItemText(0, 3, wNotice);//设置其它列数据
			    delete []NoticeMsg;
				}
			}
			//解新版本号       String
			if(PackLen>offset)
			{
			    short Verlength = *(short*)(Datapack+offset);   //公告信息字符串长度
			    offset += 2;
			    char *VerMsg = new char[Verlength+1];
			    memset(VerMsg,0,Verlength+1);
			    memcpy(VerMsg,Datapack+offset,Verlength);
			    offset+=Verlength;
			    wstring Verstr = UTF8_TO_UTF16(VerMsg);
			    dlgtest.m_ListCtl.SetItemText(0, 4, Verstr.c_str());//设置其它列数据
			    delete []VerMsg;
			}
			//解下载地址       String
			if(PackLen>offset)
			{
			    short Addrlength = *(short*)(Datapack+offset);   //公告信息字符串长度
			    offset += 2;
			    char *AddrMsg = new char[Addrlength+1];
			    memset(AddrMsg,0,Addrlength+1);
			    memcpy(AddrMsg,Datapack+offset,Addrlength);
			    offset+=Addrlength;
			    wstring Addrstr = UTF8_TO_UTF16(AddrMsg);
			    dlgtest.m_ListCtl.SetItemText(0, 5, Addrstr.c_str());//设置其它列数据
			    delete []AddrMsg;
			}
			//提醒升级       byte
			if(PackLen>offset)
			{
			    unsigned char UpdateMsg = *(Datapack+offset);
			    offset += 1;
			    int tempch = UpdateMsg;
			    CString tempstr = _T("");tempstr.Format(L"%d",tempch);
			    dlgtest.m_ListCtl.SetItemText(0, 6, tempstr);//设置其它列数据
			}
			//强制升级       byte      1强制 0不强制
			if(PackLen>offset)
			{
			    unsigned char FuMsg = *(Datapack+offset);
			    offset += 1;
			    int tempFU = FuMsg;
			    CString FU = _T("");FU.Format(L"%d",tempFU);
			    dlgtest.m_ListCtl.SetItemText(0, 7, FU);//设置其它列数据
			}
			//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
			if(PackLen>offset)
			{
			    unsigned char NLMsg = *(Datapack+offset);
			    offset += 1;
			    int tempNL = NLMsg;
			    CString NL = _T("");NL.Format(L"%d",tempNL);
			    dlgtest.m_ListCtl.SetItemText(0, 8, NL);//设置其它列数据
			}
			//用户运营商ip类 byte      0表示未知；非0表示有效，
			if(PackLen>offset)
			{
			    unsigned char IPMsg = *(Datapack+offset);
			    offset += 1;
			    int tempIP = IPMsg;
			    CString IPstr = _T("");IPstr.Format(L"%d",tempIP);
			    dlgtest.m_ListCtl.SetItemText(0, 9, IPstr);//设置其它列数据
			}
			
			//统计信息时间间隔 short   单位秒,如果为0表示不统计信息.	//20120307日增加
			if(PackLen>offset)
			{
			    short Timestap = *(short*)(Datapack+offset);
			    offset += 2;
			    CString TStamp = _T("");TStamp.Format(L"%u",Timestap);
			    dlgtest.m_ListCtl.SetItemText(0, 10, TStamp);//设置其它列数据
			}
			//升级提示文字   String
			if(PackLen>offset)
			{
			    short UNlength = *(short*)(Datapack+offset);   //公告信息字符串长度
			    offset += 2;
			    char *UNMsg = new char[UNlength+1];
			    memset(UNMsg,0,UNlength+1);
			    memcpy(UNMsg,Datapack+offset,UNlength);
			    offset+=UNlength;
			    wstring UNstr = UTF8_TO_UTF16(UNMsg);
			    dlgtest.m_ListCtl.SetItemText(0, 11, UNstr.c_str());//设置其它列数据
			    delete []UNMsg;
			}
			//公告crc        short 
			if(PackLen>offset)
			{
			    short CRCValue = *(short*)(Datapack+offset);
			    offset += 2;
			    CString tempCRC = _T("");tempCRC.Format(L"%u",CRCValue);
			    dlgtest.m_ListCtl.SetItemText(0, 12, tempCRC);//设置其它列数据
			}
			//公告提示类型   char	//0或者没有该字段表示强制弹出；1每日弹出一次；2同一公告只弹出一次；3不弹出公告
			if(PackLen>offset)
			{
			    unsigned char NoticeType = *(Datapack+offset);
			    offset += 1;
			    int tempNotice = NoticeType;
			    CString Notice = _T("");Notice.Format(L"%d",tempNotice);
			    dlgtest.m_ListCtl.SetItemText(0, 13, Notice);//设置其它列数据
			}

			//调度地址       String[]  地址格式为 ip:port:id  //id 新增加，id是运营商id 2015-3-23
			if(PackLen>offset)
			{
			    short Addrnum = *(short*)(Datapack+offset);   //委托地址个数
			    offset += 2;
			    for(int f=0;f<Addrnum;f++)
			    {
				    short Addrlength = *(short*)(Datapack+offset);
				    offset += 2;
				    char *Addr = new char[Addrlength+1];
				    memset(Addr,0,Addrlength+1);
				    memcpy(Addr,Datapack+offset,Addrlength);
				    offset += Addrlength;
				    wstring str = UTF8_TO_UTF16(Addr);
				    CString No = _T("");No.Format(L"%d",f+1);
				
				    dlgtest.m_ListCtl.SetItemText(f,0,No);
				    dlgtest.m_ListCtl.SetItemText(f, 14, str.c_str());//设置其它列数据
				    delete []Addr;
			    }
			}
			//下列不同服务地址列表数  short
			if(PackLen>offset)
			{
			    short ServerNum = *(short*)(Datapack+offset);
			    offset += 2;
			    CString tempSvr = _T("");tempSvr.Format(L"%u",ServerNum);
			    dlgtest.m_ListCtl.SetItemText(0, 15, tempSvr);//设置其它列数据
			}
			//服务id             int
			if(PackLen>offset)
			{
			    int ServerID = *(int*)(Datapack+offset);
			    offset += 4;
			    CString tempSvrID = _T("");tempSvrID.Format(L"%d",ServerID);
			    dlgtest.m_ListCtl.SetItemText(0, 16, tempSvrID);//设置其它列数据
			}
			//服务器地址         String[]	//服务器地址格式：ip:port:id
			if(PackLen>offset)
			{
			    short Servernum = *(short*)(Datapack+offset);  
			    offset += 2;
			    for(int m=0;m<Servernum;m++)
			    {
				    short Serlength = *(short*)(Datapack+offset);
				    offset += 2;
				    char *Ser = new char[Serlength+1];
				    memset(Ser,0,Serlength+1);
				    memcpy(Ser,Datapack+offset,Serlength);
				    offset += Serlength;
				    wstring str = UTF8_TO_UTF16(Ser);
				    CString No = _T("");No.Format(L"%d",m+1);
				    dlgtest.m_ListCtl.SetItemText(m,0,No);
				    dlgtest.m_ListCtl.SetItemText(m, 17, str.c_str());//设置其它列数据
				    delete []Ser;
			    }
			}
		}break;
	case 1001:
		{
			//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
			if(PackLen>offset)
			{
			    char nflag = *(Datapack+offset);
			    offset += 1;
				int nMark = nflag;
				if(nMark == 0)
				{
					//返回应答包的字段个数,map数组里面的个数
			        dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			        dlgtest.m_ListCtl.InsertColumn(1,L"类型",LVCFMT_LEFT,100);
			        dlgtest.m_ListCtl.InsertColumn(2,L"券商数",LVCFMT_LEFT,100);
			        dlgtest.m_ListCtl.InsertColumn(3,L"券商名称",LVCFMT_LEFT,sizeof(L"券商名称")*50);
			        CString Flag = _T("");Flag.Format(L"%d",nMark);
					dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			        dlgtest.m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
					//券商数：	short（下列数目）
					short QSnum = *(short*)(Datapack+offset);
					CString QS = _T("");QS.Format(L"%d",QSnum);
					dlgtest.m_ListCtl.SetItemText(0, 2, QS);
					offset += 2;
					for(int i=0; i<QSnum; i++)
					{
						short strlen = *(short*)(Datapack+offset);
						offset += 2;
						char *QSValue = new char[strlen+1];
						memset(QSValue,0,strlen+1);
						memcpy(QSValue,Datapack+offset,strlen);
						wstring cstr = UTF8_TO_UTF16(QSValue);
						CString No = _T("");No.Format(L"%d",i+1);
						dlgtest.m_ListCtl.SetItemText(i,0,No);
						dlgtest.m_ListCtl.SetItemText(i,3,cstr.c_str());
						offset += strlen;
						delete []QSValue;
					}
				}
				if(nMark == 1)
				{
					dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			        dlgtest.m_ListCtl.InsertColumn(1,L"类型",LVCFMT_LEFT,100);
			        dlgtest.m_ListCtl.InsertColumn(2,L"行情地址标记",LVCFMT_LEFT,100);
			        dlgtest.m_ListCtl.InsertColumn(3,L"委托是否支持活字典",LVCFMT_LEFT,sizeof(L"委托是否支持活字典")*10);
					dlgtest.m_ListCtl.InsertColumn(4,L"委托地址数目",LVCFMT_LEFT,100);
					dlgtest.m_ListCtl.InsertColumn(5,L"委托地址",LVCFMT_LEFT,150);
					dlgtest.m_ListCtl.InsertColumn(6,L"委托端口",LVCFMT_LEFT,100);
					dlgtest.m_ListCtl.InsertColumn(7,L"券商行情分配服务器地址数",LVCFMT_LEFT,150);
					dlgtest.m_ListCtl.InsertColumn(8,L"行情分配地址",LVCFMT_LEFT,150);
					dlgtest.m_ListCtl.InsertColumn(9,L"行情分配端口",LVCFMT_LEFT,150);
					CString Flag = _T("");Flag.Format(L"%d",nMark);
					dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			        dlgtest.m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
					//行情地址标记		char	//1表示带券商行情地址；0表示不带券商行情地址
					if(PackLen>offset)
					{
					    char hqmark = *(Datapack+offset);
					    offset += 1;
					    int HQmark = hqmark;
					    CString HQflag = _T("");HQflag.Format(L"%d",HQmark);
					    dlgtest.m_ListCtl.SetItemText(0, 2, HQflag);//设置其它列数据
					}
					//委托是否支持活字典	char	//1表示支持活字典，0表示不支持
					if(PackLen>offset)
					{
					    char ISDict = *(Datapack+offset);
					    offset += 1;
					    int DICT = ISDict;
					    CString isDict = _T("");isDict.Format(L"%d",DICT);
					    dlgtest.m_ListCtl.SetItemText(0, 3, isDict);//设置其它列数据
					}
					//委托地址数目：short（下列数目）
					if(PackLen>offset)
					{
					    short WTnum = *(short*)(Datapack+offset);
					    offset += 2;
						CString wtcount = _T("");wtcount.Format(L"%d",WTnum);
					    dlgtest.m_ListCtl.SetItemText(0, 4, wtcount);//设置其它列数据
						//委托地址	string
						if(PackLen>offset)
						{
							for(int q=0;q<WTnum;q++)
							{
								short WTlen = *(short*)(Datapack+offset);
								offset += 2;
								char *wtaddr = new char[WTlen+1];
								memset(wtaddr,0,WTlen+1);
								memcpy(wtaddr,Datapack+offset,WTlen);
								offset += WTlen;
								wstring addrstr = UTF8_TO_UTF16(wtaddr);
								CString xh = _T("");xh.Format(L"%d",q+1);
								dlgtest.m_ListCtl.SetItemText(q,0,xh);
								dlgtest.m_ListCtl.SetItemText(q,5,addrstr.c_str());
								delete []wtaddr;
								//解委托端口	short
								short WTport = *(short*)(Datapack+offset);
								offset += 2;
								CString wtport = _T("");wtport.Format(L"%d",WTport);
								dlgtest.m_ListCtl.SetItemText(q,6,wtport);
							}
						}
					}
					//券商行情分配服务器地址数：short	//如果行情地址标记为0，没有该及以下字段	
					if(PackLen>offset)
					{
					    short HQnum = *(short*)(Datapack+offset);
					    offset += 2;
						CString hqcount = _T("");hqcount.Format(L"%d",HQnum);
					    dlgtest.m_ListCtl.SetItemText(0, 7, hqcount);//设置其它列数据
						//委托地址	string
						if(PackLen>offset)
						{
							for(int w=0;w<HQnum;w++)
							{
								short HQlen = *(short*)(Datapack+offset);
								offset += 2;
								char *hqaddr = new char[HQlen+1];
								memset(hqaddr,0,HQlen+1);
								memcpy(hqaddr,Datapack+offset,HQlen);
								offset += HQlen;
								wstring hqaddrstr = UTF8_TO_UTF16(hqaddr);
								CString hqxh = _T("");hqxh.Format(L"%d",w+1);
								dlgtest.m_ListCtl.SetItemText(w,0,hqxh);
								dlgtest.m_ListCtl.SetItemText(w,8,hqaddrstr.c_str());
								delete []hqaddr;
								//解委托端口	short
								short HQport = *(short*)(Datapack+offset);
								offset += 2;
								CString wtport = _T("");wtport.Format(L"%d",HQport);
								dlgtest.m_ListCtl.SetItemText(w,9,wtport);
							}
						}
					}
				}
			}
		}break;
	case 1003:
		{
			//定义公共常量变量参数
			short QSnum=0;   //券商个数
			short SXnum = 0;  //扩展属性数目
			short ADDRnum = 0; //地址数目,地址数目和扩展属性是同一级别
			if(PackLen>offset)
			{
				dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(1,L"地址信息crc",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(2,L"券商数目",LVCFMT_LEFT,80);
			    dlgtest.m_ListCtl.InsertColumn(3,L"券商名称",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(4,L"券商名称首拼音",LVCFMT_LEFT,sizeof(L"券商名称首拼音")*5);
				dlgtest.m_ListCtl.InsertColumn(5,L"扩展属性数目",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(6,L"属性序号",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(7,L"属性值",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(8,L"地址数目",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(9,L"地址",LVCFMT_LEFT,100);
				dlgtest.m_ListCtl.InsertColumn(10,L"地址端口",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(11,L"ip地址运营商类别",LVCFMT_LEFT,100);
				dlgtest.m_ListCtl.InsertColumn(12,L"地址类型",LVCFMT_LEFT,80);
				//解地址信息CRC值 short型
				short CRCval = *(short*)(Datapack+offset);
			    offset += 2;
				CString CRCfg = _T("");CRCfg.Format(L"%u",CRCval);
				dlgtest.m_ListCtl.SetItemText(0,1,CRCfg);
				//解short           券商数目
				if(PackLen>offset)
				{
				    QSnum = *(short*)(Datapack+offset);
				    offset += 2;
				    CString QSCount = _T("");QSCount.Format(L"%u",QSnum);
				    dlgtest.m_ListCtl.SetItemText(0,2,QSCount);
				    //解券商名称 string   ,由于下面都是券商个数的子列表，用for循环
					if(PackLen>offset)
					{
				        for(int i=0; i<QSnum; i++)
				        {
					        //解String      券商名称
					        short QSNamelen = *(short*)(Datapack+offset);
					        offset += 2;
					        if(QSNamelen!=0)
					        {
						        char *QSName = new char[QSNamelen+1];
						        memset(QSName,0,QSNamelen+1);
						        memcpy(QSName,Datapack+offset,QSNamelen);
						        wstring QSstr = UTF8_TO_UTF16(QSName);
						        offset += QSNamelen;
								dlgtest.m_ListCtl.SetItemText(i*SXnum,3,QSstr.c_str());
								delete []QSName;
					        }
							if(PackLen>offset)
							{
								//解char        券商名称手拼音
								char QSP = *(Datapack+offset);
								offset += 1;
								CString zmstr = _T("");zmstr.Format(L"%c",QSP);
								dlgtest.m_ListCtl.SetItemText(i*SXnum,4,zmstr);
							}
							if(PackLen>offset)
							{
								//解short	下列扩展属性数目
								SXnum = *(short*)(Datapack+offset);
								offset+=2;
								CString SXnumVal=_T("");SXnumVal.Format(L"%d",SXnum);
								dlgtest.m_ListCtl.SetItemText(i*SXnum,5,SXnumVal);
								
								if(QSnum*SXnum>50)
								{
									for(int w=0; w<QSnum*SXnum;w++)
									{
										CString val=_T("");val.Format(L"%d",w+1);
										dlgtest.m_ListCtl.SetItemText(w,0,val);
									}
								}
								
								for(int r=0; r<SXnum; r++)
								{
									// char     属性序号
									char AttrNo = *(Datapack+offset);
									offset += 1;
									int attrno = AttrNo;
									CString Attr=_T("");Attr.Format(L"%d",attrno);
									dlgtest.m_ListCtl.SetItemText(r+i*SXnum,6,Attr);

									//char	属性值
									char AttrVal = *(Datapack+offset);
									offset += 1;
									int attrval = AttrVal;
									CString Attr2=_T("");Attr2.Format(L"%d",attrno);
									dlgtest.m_ListCtl.SetItemText(r+i*SXnum,7,Attr2);
								}
							}
							if(PackLen>offset)
							{
								//解short	地址数目
								ADDRnum = *(short*)(Datapack+offset);
								offset+=2;
								CString ADDRnumVal=_T("");ADDRnumVal.Format(L"%d",SXnum);
								dlgtest.m_ListCtl.SetItemText(i*SXnum,8,ADDRnumVal);
								
								if(ADDRnum>SXnum)
								{
								    if(QSnum*ADDRnum>50)
								    {
									    for(int l=0; l<QSnum*SXnum;l++)
									    {
										    CString val1=_T("");val1.Format(L"%d",l+1);
										    dlgtest.m_ListCtl.SetItemText(l,0,val1);
									    }
								    }
								}
								
								for(int t=0; t<ADDRnum; t++)
								{
									// string   地址
									if(PackLen>offset)
									{
										short strlen = *(short*)(Datapack+offset);
										offset += 2;
										if(strlen>0)
										{
											char *addrstr = new char[strlen+1];
											memset(addrstr,0,strlen+1);
											memcpy(addrstr,Datapack+offset,strlen);
											wstring tempaddr = UTF8_TO_UTF16(addrstr);
											offset += strlen;
											dlgtest.m_ListCtl.SetItemText(t+i*ADDRnum,9,tempaddr.c_str());
											delete []addrstr;
										}
									}
									//解short    端口
									if(PackLen>offset)
									{
										short port = *(short*)(Datapack+offset);
										offset += 2;
										CString addrport = _T("");addrport.Format(L"",port);
										dlgtest.m_ListCtl.SetItemText(t+i*ADDRnum,10,addrport);
									}
									//ip地址运营商类别  char
									if(PackLen>offset)
									{
									    char iptype = *(Datapack+offset);
									    offset += 1;
									    int IPType = iptype;
									    CString IPstr=_T("");IPstr.Format(L"%d",IPType);
									    dlgtest.m_ListCtl.SetItemText(t+i*ADDRnum,11,IPstr);
									}
									if(PackLen>offset)
									{
									    //char     地址类型	 //1表示委托地址，2表示行情分配地址
									    char addrtype = *(Datapack+offset);
									    offset += 1;
									    int addrval = addrtype;
									    CString Addr=_T("");Addr.Format(L"%d",addrval);
									    dlgtest.m_ListCtl.SetItemText(t+i*ADDRnum,12,Addr);
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
			//先判断请求字段是0还是其他数值
			////城市营业部营业部数目
			short CityCnt = 0;
			int idvalue = *(int*)(dlgtest.SendBuffer+7);
			if(idvalue == 0)     //为0表示城市索引,返回String[]     城市索引
			{
				dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(1,L"城市索引",LVCFMT_LEFT,100);
				short CityNum = *(short*)(Datapack+offset);
				offset += 2;
				for(int i=0; i<CityNum; i++)
				{
					dlgtest.m_ListCtl.InsertItem(i,_T(""));
					CString CityNo=_T("");CityNo.Format(L"%d",i+1);
					dlgtest.m_ListCtl.SetItemText(i,0,CityNo);
					short stringlen = *(short*)(Datapack+offset);
					offset += 2;
					if(stringlen>0)
					{
						char *CityStr = new char[stringlen+1];
						memset(CityStr,0,stringlen+1);
						memcpy(CityStr,Datapack+offset,stringlen);
						offset += stringlen;
						wstring tempstr = UTF8_TO_UTF16(CityStr);
						dlgtest.m_ListCtl.SetItemText(i,1,tempstr.c_str());
						delete []CityStr;
					}
				}
			}
			else
			{
				dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,80);
				dlgtest.m_ListCtl.InsertColumn(1,L"营业部数目",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(2,L"营业部名称",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(3,L"营业部地址",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(4,L"营业部电话",LVCFMT_LEFT,sizeof(L"券商名称首拼音")*5);
				dlgtest.m_ListCtl.InsertColumn(5,L"营业部google搜索地址",LVCFMT_LEFT,100);
				dlgtest.m_ListCtl.InsertColumn(6,L"营业部经度",LVCFMT_LEFT,100);
				dlgtest.m_ListCtl.InsertColumn(7,L"营业部纬度",LVCFMT_LEFT,100);
				if(PackLen>offset)
				{
					//short        下面数组数目
					CityCnt = *(short*)(Datapack+offset);
					CString cicnt = _T("");cicnt.Format(L"%d",CityCnt);
					offset += 2;
					if(CityCnt>0)
					{
						
				        for(int j=0; j<CityCnt; j++)
				        {
					        dlgtest.m_ListCtl.InsertItem(j,_T(""));
					        CString CityGS=_T("");CityGS.Format(L"%d",j+1);
							dlgtest.m_ListCtl.SetItemText(0,1,cicnt);
					        dlgtest.m_ListCtl.SetItemText(j,0,CityGS);
						}
						for(int j=0;j<CityCnt;j++)
						{
						    //String    营业部名称
					        short YYBlen = *(short*)(Datapack+offset);
					        offset += 2;
					        if(YYBlen>0)
					        {
						        char *yybName = new char[YYBlen+1];
						        memset(yybName,0,YYBlen+1);
						        memcpy(yybName,Datapack+offset,YYBlen);
						        offset += YYBlen;
						        wstring YybName = UTF8_TO_UTF16(yybName);
						        dlgtest.m_ListCtl.SetItemText(j,2,YybName.c_str());
						        delete []yybName;
						    }
							 //String    营业部地址
							if(PackLen>offset)
							{
								YYBlen = *(short*)(Datapack+offset);
								offset += 2;
					            if(YYBlen>0)
					            {
						            char *yybaddr = new char[YYBlen+1];
						            memset(yybaddr,0,YYBlen+1);
						            memcpy(yybaddr,Datapack+offset,YYBlen);
						            offset += YYBlen;
						            wstring YybAddr = UTF8_TO_UTF16(yybaddr);
						            dlgtest.m_ListCtl.SetItemText(j,3,YybAddr.c_str());
						            delete []yybaddr;
						        }
							}
							 //String    营业部电话
							if(PackLen>offset)
							{
								YYBlen = *(short*)(Datapack+offset);
								offset += 2;
					            if(YYBlen>0)
					            {
						            char *yybtel = new char[YYBlen+1];
						            memset(yybtel,0,YYBlen+1);
						            memcpy(yybtel,Datapack+offset,YYBlen);
						            offset += YYBlen;
						            wstring YybTel = UTF8_TO_UTF16(yybtel);
						            dlgtest.m_ListCtl.SetItemText(j,4,YybTel.c_str());
						            delete []yybtel;
						        }
							}
							 //String    营业部google搜索地址
							if(PackLen>offset)
							{
								YYBlen = *(short*)(Datapack+offset);
								offset += 2;
					            if(YYBlen>0)
					            {
						            char *goaddr = new char[YYBlen+1];
						            memset(goaddr,0,YYBlen+1);
						            memcpy(goaddr,Datapack+offset,YYBlen);
						            offset += YYBlen;
						            wstring GoAddr = UTF8_TO_UTF16(goaddr);
						            dlgtest.m_ListCtl.SetItemText(j,5,GoAddr.c_str());
						            delete []goaddr;
						        }
							}
							 //String    营业部经度
							if(PackLen>offset)
							{
								YYBlen = *(short*)(Datapack+offset);
								offset += 2;
					            if(YYBlen>0)
					            {
						            char *ltaddr = new char[YYBlen+1];
						            memset(ltaddr,0,YYBlen+1);
						            memcpy(ltaddr,Datapack+offset,YYBlen);
						            offset += YYBlen;
						            wstring LTAddr = UTF8_TO_UTF16(ltaddr);
						            dlgtest.m_ListCtl.SetItemText(j,6,LTAddr.c_str());
						            delete []ltaddr;
						        }
							}
							 //String    营业部weidu
							if(PackLen>offset)
							{
								YYBlen = *(short*)(Datapack+offset);
								offset += 2;
					            if(YYBlen>0)
					            {
						            char *lotude = new char[YYBlen+1];
						            memset(lotude,0,YYBlen+1);
						            memcpy(lotude,Datapack+offset,YYBlen);
						            offset += YYBlen;
						            wstring LoTude = UTF8_TO_UTF16(lotude);
						            dlgtest.m_ListCtl.SetItemText(j,7,LoTude.c_str());
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
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"是否通过",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"提示信息",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			if(PackLen>offset)
			{
				//解  是否通过       byte       1通过 0没通过
				char chPass = *(Datapack+offset);
				offset += 1;
				int szch = chPass;
				CString PassVal=_T("");PassVal.Format(L"%d",szch);
				dlgtest.m_ListCtl.SetItemText(0,0,L"1");
				dlgtest.m_ListCtl.SetItemText(0,1,PassVal);
			}
			if(PackLen>offset)
			{
				//解提示信息       String
				short Msglen = *(short*)(Datapack+offset);
				offset += 2;
				
				char *Msg = new char[Msglen+1];
				memset(Msg,0,Msglen+1);
				memcpy(Msg,Datapack+offset,Msglen);
				offset += Msglen;
				wstring NMsg = UTF8_TO_UTF16(Msg);
				dlgtest.m_ListCtl.SetItemText(0,2,NMsg.c_str());
				delete []Msg;
			}
		}break;
	case 1902:
		{
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"同步结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"自选代码",LVCFMT_LEFT,100);
			//定义收包的字段
			unsigned char nResult = ' ';
			CString byte=_T("");
			if(PackLen>offset)
			{
				//解同步结果       byte       1成功 0失败
				nResult = *(Datapack+offset);
				offset += 1;
				int Rbyte = nResult;
				byte.Format(L"%d",Rbyte);
				dlgtest.m_ListCtl.SetItemText(0,0,L"1");
				dlgtest.m_ListCtl.SetItemText(0,1,byte);
			}
			if(PackLen>offset)
			{
				//解自选代码       String[]
				short CodeNum = *(short*)(Datapack+offset);
				offset += 2;
				for(int i=0;i<CodeNum;i++)
				{
					dlgtest.m_ListCtl.InsertItem(i,_T(""));
					CString Value=_T("");Value.Format(L"%d",i+1);
					dlgtest.m_ListCtl.SetItemText(i,0,Value);
					dlgtest.m_ListCtl.SetItemText(0,1,byte);
					short Codelen = *(short*)(Datapack+offset);
					offset += 2;
					char *CodeVal = new char[Codelen+1];
					memset(CodeVal,0,Codelen+1);
					memcpy(CodeVal,Datapack+offset,Codelen);
					wstring Code = UTF8_TO_UTF16(CodeVal);
					dlgtest.m_ListCtl.SetItemText(i,2,Code.c_str());
					delete []CodeVal;
				}
			}
		}break;
	case 1903:
		{
			//解提示信息       String
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"提示信息",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short Noticelen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				char *Notice = new char[Noticelen+1];
				memset(Notice,0,Noticelen+1);
				memcpy(Notice,Datapack+offset,Noticelen);
				offset += Noticelen;
				wstring NoticeStr = UTF8_TO_UTF16(Notice);
				
				dlgtest.m_ListCtl.SetItemText(0,1,NoticeStr.c_str());
				delete []Notice;
				}
			}
		}break;
	case 1904:
		{
			//是否有效       byte       1有效 0无效
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"是否有效",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"通信密码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				unsigned char IsVal = *(Datapack+offset);
				offset += 1;
				int isval = IsVal;
				CString Val=_T("");Val.Format(L"%d",isval);
				dlgtest.m_ListCtl.SetItemText(0,1,Val);
			}
			if(PackLen>offset)
			{
				short PWlen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				    char *PW = new char[PWlen+1];
				    memset(PW,0,PWlen+1);
				    memcpy(PW,Datapack+offset,PWlen);
				    offset += PWlen;
				    wstring PWStr = UTF8_TO_UTF16(PW);
				    dlgtest.m_ListCtl.SetItemText(0,2,PWStr.c_str());
				    delete []PW;
				}
			}
		}break;
	case 1905:
		{
			//是否有效       byte       1有效 0无效
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"手机号码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"通信密码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short Phonelen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				    char *Phone = new char[Phonelen+1];
				    memset(Phone,0,Phonelen+1);
				    memcpy(Phone,Datapack+offset,Phonelen);
				    offset += Phonelen;
				    wstring PhoneStr = UTF8_TO_UTF16(Phone);
				    dlgtest.m_ListCtl.SetItemText(0,1,PhoneStr.c_str());
				    delete []Phone;
				}
			}
			if(PackLen>offset)
			{
				short PWlen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				    char *PW = new char[PWlen+1];
				    memset(PW,0,PWlen+1);
				    memcpy(PW,Datapack+offset,PWlen);
				    offset += PWlen;
				    wstring PWStr = UTF8_TO_UTF16(PW);
				    dlgtest.m_ListCtl.SetItemText(0,2,PWStr.c_str());
				    delete []PW;
				}
			}
		}break;
	case 2000:
		{
			//记录数         short
			short Num = 0;
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				Num = *(short*)(Datapack+offset);
				CString codecount=_T("");codecount.Format(L"%d",Num);
				offset += 2;
				for(int i=0;i<Num;i++)
				{
					dlgtest.m_ListCtl.InsertItem(i,_T(""));
					CString CodeNum=_T("");CodeNum.Format(L"%d",i+1);
					dlgtest.m_ListCtl.SetItemText(i,0,CodeNum);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,codecount);
			}
			for(int j=0;j<Num;j++)
			{
			    if(PackLen>offset)
			    {
				    short codelen = *(short*)(Datapack+offset);
				    offset += 2;
				    if(PackLen>offset)
				    {
				        char *ProductCode = new char[codelen+1];
				        memset(ProductCode,0,codelen+1);
				        memcpy(ProductCode,Datapack+offset,codelen);
				        offset += codelen;
				        wstring ProductCodeStr = UTF8_TO_UTF16(ProductCode);
				        dlgtest.m_ListCtl.SetItemText(j,2,ProductCodeStr.c_str());
				        delete []ProductCode;
				    }
			    }
			    if(PackLen>offset)
			    {
				    short Namelen = *(short*)(Datapack+offset);
				    offset += 2;
				    if(PackLen>offset)
				    {
				        char *Name = new char[Namelen+1];
				        memset(Name,0,Namelen+1);
				        memcpy(Name,Datapack+offset,Namelen);
				        offset += Namelen;
				        wstring NameStr = UTF8_TO_UTF16(Name);
				        dlgtest.m_ListCtl.SetItemText(j,3,NameStr.c_str());
				        delete []Name;
				    }
			    }
			}
		}break;
	case 2100:
		{
			short Num = 0;     //记录数
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"最新价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"昨收价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"成交额",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"最高价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"最低价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(11,L"有无公告",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(12,L"请求板块指数id",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(13,L"总记录数",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				Num = *(short*)(Datapack+offset);
				CString codecount=_T("");codecount.Format(L"%d",Num);
				offset += 2;
				for(int i=0;i<Num;i++)
				{
					dlgtest.m_ListCtl.InsertItem(i,_T(""));
					CString CodeNum=_T("");CodeNum.Format(L"%d",i+1);
					dlgtest.m_ListCtl.SetItemText(i,0,CodeNum);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,codecount);
			}
			for(int j=0;j<Num;j++)
			{
				//"代码" type="ans_string"
			    if(PackLen>offset)
			    {
				    short codelen = *(short*)(Datapack+offset);
				    offset += 2;
				    if(PackLen>offset)
				    {
				        char *ProductCode = new char[codelen+1];
				        memset(ProductCode,0,codelen+1);
				        memcpy(ProductCode,Datapack+offset,codelen);
				        offset += codelen;
				        wstring ProductCodeStr = UTF8_TO_UTF16(ProductCode);
				        dlgtest.m_ListCtl.SetItemText(j,2,ProductCodeStr.c_str());
				        delete []ProductCode;
				    }
			    }
				//"名称" type="ans_string"
			    if(PackLen>offset)
			    {
				    short Namelen = *(short*)(Datapack+offset);
				    offset += 2;
				    if(PackLen>offset)
				    {
				        char *Name = new char[Namelen+1];
				        memset(Name,0,Namelen+1);
				        memcpy(Name,Datapack+offset,Namelen);
				        offset += Namelen;
				        wstring NameStr = UTF8_TO_UTF16(Name);
				        dlgtest.m_ListCtl.SetItemText(j,3,NameStr.c_str());
				        delete []Name;
				    }
			    }
				if(PackLen>offset)
				{
					//解小数点位数
					unsigned char pointval = *(Datapack+offset);
					offset += 1;
					int PtVal = pointval;
					CString PtStr=_T("");PtStr.Format(L"%d",PtVal);
					dlgtest.m_ListCtl.SetItemText(j,4,PtStr);
				}
				if(PackLen>offset)
				{
					//解最新价
					int newprice = *(int*)(Datapack+offset);
					offset += 4;
					CString NPStr=_T("");NPStr.Format(L"%d",newprice);
					dlgtest.m_ListCtl.SetItemText(j,5,NPStr);
				}
				if(PackLen>offset)
				{
					//解昨收价
					int YTrice = *(int*)(Datapack+offset);
					offset += sizeof(int);
					CString YTriceStr=_T("");YTriceStr.Format(L"%d",YTrice);
					dlgtest.m_ListCtl.SetItemText(j,6,YTriceStr);
				}
				if(PackLen>offset)
				{
					//解成交量
					int MKVal = *(int*)(Datapack+offset);
					offset += 4;
					CString MKValStr=_T("");MKValStr.Format(L"%d",MKVal);
					dlgtest.m_ListCtl.SetItemText(j,7,MKValStr);
				}
				if(PackLen>offset)
				{
					//解成交额
					int MKMoney = *(int*)(Datapack+offset);
					offset += 4;
					CString MKMoneyStr=_T("");MKMoneyStr.Format(L"%d",MKMoney);
					dlgtest.m_ListCtl.SetItemText(j,8,MKMoneyStr);
				}
				if(PackLen>offset)
				{
					//解最高价
					int hprice = *(int*)(Datapack+offset);
					offset += 4;
					CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
					dlgtest.m_ListCtl.SetItemText(j,9,hpriceStr);
				}
				if(PackLen>offset)
				{
					//解最低价
					int lprice = *(int*)(Datapack+offset);
					offset += 4;
					CString LowStr=_T("");LowStr.Format(L"%d",lprice);
					dlgtest.m_ListCtl.SetItemText(j,10,LowStr);
				}
				if(PackLen>offset)
				{
					//有无公告
					unsigned char NoticeVal = *(Datapack+offset);
					offset += 1;
					int IsVal = NoticeVal;
					CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
					dlgtest.m_ListCtl.SetItemText(j,11,IsValStr);
				}
				if(PackLen>offset)
				{
					//请求板块指数id" type="ans_short"
					short IndexID = *(short*)(Datapack+offset);
					offset += 2;
					CString indexstr=_T("");indexstr.Format(L"%d",IndexID);
					dlgtest.m_ListCtl.SetItemText(j,12,indexstr);
				}
			}
			if(PackLen>offset)
			{
					//总记录数" type="ans_short
				short TNum = *(short*)(Datapack+offset);
				offset += 2;
				CString TNumStr=_T("");TNumStr.Format(L"%d",TNum);
				dlgtest.m_ListCtl.SetItemText(0,13,TNumStr);
			}
		}break;
	case 2200:
		{
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"类型",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"AH代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"总股本",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"涨停价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"跌停价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
			    short codelen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				    char *ProductCode = new char[codelen+1];
				    memset(ProductCode,0,codelen+1);
				    memcpy(ProductCode,Datapack+offset,codelen);
				    offset += codelen;
				    wstring ProductCodeStr = UTF8_TO_UTF16(ProductCode);
				    dlgtest.m_ListCtl.SetItemText(0,1,ProductCodeStr.c_str());
				    delete []ProductCode;
				}
			}
		    //"名称" type="ans_string"
			if(PackLen>offset)
			{
				short Namelen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				    char *Name = new char[Namelen+1];
				    memset(Name,0,Namelen+1);
				    memcpy(Name,Datapack+offset,Namelen);
				    offset += Namelen;
				    wstring NameStr = UTF8_TO_UTF16(Name);
				    dlgtest.m_ListCtl.SetItemText(0,2,NameStr.c_str());
				    delete []Name;
				}
			}
			if(PackLen>offset)
			{
				//类型
				unsigned char Type = *(Datapack+offset);
				offset += 1;
				int IsVal = Type;
				CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
				dlgtest.m_ListCtl.SetItemText(0,3,IsValStr);
			}
			if(PackLen>offset)
			{
				//有小数点位数
				unsigned char point = *(Datapack+offset);
				offset += 1;
				int pointval = point;
				CString pointStr=_T("");pointStr.Format(L"%d",pointval);
				dlgtest.m_ListCtl.SetItemText(0,4,pointStr);
			}
			if(PackLen>offset)
			{
				short AHcodelen = *(short*)(Datapack+offset);
				offset += 2;
				if(PackLen>offset)
				{
				    char *AHcode = new char[AHcodelen+1];
				    memset(AHcode,0,AHcodelen+1);
				    memcpy(AHcode,Datapack+offset,AHcodelen);
				    offset += AHcodelen;
				    wstring AHcodeStr = UTF8_TO_UTF16(AHcode);
				    dlgtest.m_ListCtl.SetItemText(0,5,AHcodeStr.c_str());
				    delete []AHcode;
				}
			}
			if(PackLen>offset)
			{
				//解总股本
				int tval = *(int*)(Datapack+offset);
				offset += 4;
				CString LowStr=_T("");LowStr.Format(L"%d",tval);
				dlgtest.m_ListCtl.SetItemText(0,6,LowStr);
			}
			if(PackLen>offset)
			{
				//解涨停价
				int HPrice = *(int*)(Datapack+offset);
				offset += 4;
				CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
				dlgtest.m_ListCtl.SetItemText(0,7,HPriceStr);
			}
			if(PackLen>offset)
			{
				//解最低价
				int Lowprice = *(int*)(Datapack+offset);
				offset += 4;
				CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
				dlgtest.m_ListCtl.SetItemText(0,8,LowpriceStr);
			}
		}break;
	case 2201:
		{
			short RecdNum=0;
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"昨收盘",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"成交额",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"信息地雷",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"五星评级",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"数据位置",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"时间",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"最新价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(11,L"均价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//解昨收盘         int
				int tval = *(int*)(Datapack+offset);
				offset += 4;
				CString LowStr=_T("");LowStr.Format(L"%d",tval);
				dlgtest.m_ListCtl.SetItemText(0,1,LowStr);
			}
			if(PackLen>offset)
			{
				//解成交量         int
				int HPrice = *(int*)(Datapack+offset);
				offset += 4;
				CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
				dlgtest.m_ListCtl.SetItemText(0,2,HPriceStr);
			}
			if(PackLen>offset)
			{
				//解成交额         int
				int Lowprice = *(int*)(Datapack+offset);
				offset += 4;
				CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
				dlgtest.m_ListCtl.SetItemText(0,3,LowpriceStr);
			}
			if(PackLen>offset)
			{
				//信息地雷       byte    1有 0无
				unsigned char Msg = *(Datapack+offset);
				offset += 1;
				int Msgval = Msg;
				CString MsgStr=_T("");MsgStr.Format(L"%d",Msgval);
				dlgtest.m_ListCtl.SetItemText(0,4,MsgStr);
			}
			if(PackLen>offset)
			{
				//五星评级       byte    0-5数值
				unsigned char star = *(Datapack+offset);
				offset += 1;
				int starval = star;
				CString starStr=_T("");starStr.Format(L"%d",starval);
				dlgtest.m_ListCtl.SetItemText(0,5,starStr);
			}
			if(PackLen>offset)
			{
				//数据位置       int         最后一根分时数据需重传
				int datapos = *(int*)(Datapack+offset);
				offset += 4;
				CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
				dlgtest.m_ListCtl.SetItemText(0,6,dataposStr);
			}
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				CString RecdNumStr=_T("");RecdNumStr.Format(L"%d",RecdNum);
				dlgtest.m_ListCtl.SetItemText(0,7,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+2);
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				dlgtest.m_ListCtl.SetItemText(i+1,0,XHno);
			}
			if(PackLen>offset)
			{
				for(int j=0; j<RecdNum; j++)
				{
					//解时间           int
					int timeval = *(int*)(Datapack+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    dlgtest.m_ListCtl.SetItemText(j,8,timevalStr);
					if(PackLen>offset)
					{
						//解最新价         int
					    int NewPrice = *(int*)(Datapack+offset);
				        offset += 4;
				        CString NewPriceStr=_T("");NewPriceStr.Format(L"%d",NewPrice);
				        dlgtest.m_ListCtl.SetItemText(j,9,NewPriceStr);
					}
					if(PackLen>offset)
					{
						//解成交量         int
					    int CJVal = *(int*)(Datapack+offset);
				        offset += 4;
				        CString CJValStr=_T("");CJValStr.Format(L"%d",CJVal);
				        dlgtest.m_ListCtl.SetItemText(j,10,CJValStr);
					}
					if(PackLen>offset)
					{
						//解均价           int
					    int AVprice = *(int*)(Datapack+offset);
				        offset += 4;
				        CString AVpriceStr=_T("");AVpriceStr.Format(L"%d",AVprice);
				        dlgtest.m_ListCtl.SetItemText(j,11,AVpriceStr);
					}
				}
			}
		}break;
	case 2202:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"返回个数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"开盘价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"最高价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"最低价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"收盘价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"成交额",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				dlgtest.m_ListCtl.SetItemText(i,0,XHno);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			if(PackLen>offset)
			{
				for(int j=0; j<RecdNum; j++)
				{
					//解日期           int
					int timeval = *(int*)(Datapack+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    dlgtest.m_ListCtl.SetItemText(j,2,timevalStr);
					if(PackLen>offset)
					{
						//解开盘价         int
					    int OPrice = *(int*)(Datapack+offset);
				        offset += 4;
				        CString OPriceStr=_T("");OPriceStr.Format(L"%d",OPrice);
				        dlgtest.m_ListCtl.SetItemText(j,3,OPriceStr);
					}
					if(PackLen>offset)
					{
						//解最高价         int
					    int mhVal = *(int*)(Datapack+offset);
				        offset += 4;
				        CString mhValStr=_T("");mhValStr.Format(L"%d",mhVal);
				        dlgtest.m_ListCtl.SetItemText(j,4,mhValStr);
					}
					if(PackLen>offset)
					{
						//解最低价         int
					    int mdprice = *(int*)(Datapack+offset);
				        offset += 4;
				        CString mdpriceStr=_T("");mdpriceStr.Format(L"%d",mdprice);
				        dlgtest.m_ListCtl.SetItemText(j,5,mdpriceStr);
					}
					if(PackLen>offset)
					{
						//解收盘价         int
					    int Sprice = *(int*)(Datapack+offset);
				        offset += 4;
				        CString SpriceStr=_T("");SpriceStr.Format(L"%d",Sprice);
				        dlgtest.m_ListCtl.SetItemText(j,6,SpriceStr);
					}
					if(PackLen>offset)
					{
						//解成交量         int
					    int CJValue = *(int*)(Datapack+offset);
				        offset += 4;
				        CString CJValueStr=_T("");CJValueStr.Format(L"%d",CJValue);
				        dlgtest.m_ListCtl.SetItemText(j,7,CJValueStr);
					}
					if(PackLen>offset)
					{
						//解成交额         int
					    int CJMoney = *(int*)(Datapack+offset);
				        offset += 4;
				        CString CJMoneyStr=_T("");CJMoneyStr.Format(L"%d",CJMoney);
				        dlgtest.m_ListCtl.SetItemText(j,8,CJMoneyStr);
					}
				}
			}
		}break;
	case 2203:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"最新价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"成交额",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"最高价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"最低价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"开盘价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"昨收盘",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"流通股本",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"委比",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"量比",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(11,L"上证指数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(12,L"上证指数昨收",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(13,L"深证指数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(14,L"深证数昨收",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(15,L"内盘成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(16,L"现手",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//最新价         int
				int newprice = *(int*)(Datapack+offset);
				offset += 4;
				CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
				dlgtest.m_ListCtl.SetItemText(0,1,newpriceStr);
			}
			if(PackLen>offset)
			{
				//成交量         int
				int cjval = *(int*)(Datapack+offset);
				offset += 4;
				CString cjvalStr=_T("");cjvalStr.Format(L"%d",cjval);
				dlgtest.m_ListCtl.SetItemText(0,2,cjvalStr);
			}
			if(PackLen>offset)
			{
				//成交额         int
				int cjmoney = *(int*)(Datapack+offset);
				offset += 4;
				CString cjmoneyStr=_T("");cjmoneyStr.Format(L"%d",cjmoney);
				dlgtest.m_ListCtl.SetItemText(0,3,cjmoneyStr);
			}
			if(PackLen>offset)
			{
				//最高价         int
				int hprice = *(int*)(Datapack+offset);
				offset += 4;
				CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
				dlgtest.m_ListCtl.SetItemText(0,4,hpriceStr);
			}
			if(PackLen>offset)
			{
				//最低价         int
				int lprice = *(int*)(Datapack+offset);
				offset += 4;
				CString lpriceStr=_T("");lpriceStr.Format(L"%d",lprice);
				dlgtest.m_ListCtl.SetItemText(0,5,lpriceStr);
			}
			if(PackLen>offset)
			{
				//开盘价         int
				int oprice = *(int*)(Datapack+offset);
				offset += 4;
				CString opriceStr=_T("");opriceStr.Format(L"%d",oprice);
				dlgtest.m_ListCtl.SetItemText(0,6,opriceStr);
			}
			if(PackLen>offset)
			{
				//昨收盘         int
				int sprice = *(int*)(Datapack+offset);
				offset += 4;
				CString spriceStr=_T("");spriceStr.Format(L"%d",sprice);
				dlgtest.m_ListCtl.SetItemText(0,7,spriceStr);
			}
			if(PackLen>offset)
			{
				//流通股本       int
				int lgval = *(int*)(Datapack+offset);
				offset += 4;
				CString lgvalStr=_T("");lgvalStr.Format(L"%d",lgval);
				dlgtest.m_ListCtl.SetItemText(0,8,lgvalStr);
			}
			if(PackLen>offset)
			{
				//委比		int×100
				int wb = *(int*)(Datapack+offset);
				offset += 4;
				CString wbStr=_T("");wbStr.Format(L"%d",wb);
				dlgtest.m_ListCtl.SetItemText(0,9,wbStr);
			}
			if(PackLen>offset)
			{
				//量比		int×10000
				int lb = *(int*)(Datapack+offset);
				offset += 4;
				CString lbStr=_T("");lbStr.Format(L"%d",lb);
				dlgtest.m_ListCtl.SetItemText(0,10,lbStr);
			}
			if(PackLen>offset)
			{
				//上证指数	int
				int sindex = *(int*)(Datapack+offset);
				offset += 4;
				CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
				dlgtest.m_ListCtl.SetItemText(0,11,sindexStr);
			}
			if(PackLen>offset)
			{
				//上证指数昨收	int
				int indexspri = *(int*)(Datapack+offset);
				offset += 4;
				CString indexspriStr=_T("");indexspriStr.Format(L"%d",indexspri);
				dlgtest.m_ListCtl.SetItemText(0,12,indexspriStr);
			}
			if(PackLen>offset)
			{
				//深证指数	int
				int sindex = *(int*)(Datapack+offset);
				offset += 4;
				CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
				dlgtest.m_ListCtl.SetItemText(0,13,sindexStr);
			}
			if(PackLen>offset)
			{
				//深证数昨收	int
				int sindexsprice = *(int*)(Datapack+offset);
				offset += 4;
				CString sindexspriceStr=_T("");sindexspriceStr.Format(L"%d",sindexsprice);
				dlgtest.m_ListCtl.SetItemText(0,14,sindexspriceStr);
			}
			if(PackLen>offset)
			{
				//内盘成交量	int	//对指数没有意义
				int incjval = *(int*)(Datapack+offset);
				offset += 4;
				CString incjvalStr=_T("");incjvalStr.Format(L"%d",incjval);
				dlgtest.m_ListCtl.SetItemText(0,15,incjvalStr);
			}
			if(PackLen>offset)
			{
				//现手		int	//对指数没有意义
				int scount = *(int*)(Datapack+offset);
				offset += 4;
				CString scountStr=_T("");scountStr.Format(L"%d",scount);
				dlgtest.m_ListCtl.SetItemText(0,16,scountStr);
			}
		}break;
	case 2204:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"昨收盘",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"买卖价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"买卖量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//昨收盘         int
				int zsprice = *(int*)(Datapack+offset);
				offset += 4;
				CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				dlgtest.m_ListCtl.SetItemText(0,1,zspriceStr);
			}
			if(PackLen>offset)
			{
				//记录数         short       包含买卖盘，五档行情传10，三档行情传6
				RecdNum = *(short*)(Datapack+offset);
				offset += sizeof(short);
				RecdNumStr.Format(L"%d",RecdNum);
				dlgtest.m_ListCtl.SetItemText(0,2,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+2);
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				dlgtest.m_ListCtl.SetItemText(i+1,0,XHno);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//买卖价         int
				if(PackLen>offset)
				{
					int bsprice = *(int*)(Datapack+offset);
				    offset += 4;
				    CString bspriceStr=_T("");bspriceStr.Format(L"%d",bsprice);
				    dlgtest.m_ListCtl.SetItemText(j,3,bspriceStr);
				}
				//买卖量         int
				if(PackLen>offset)
				{
					int bsval = *(int*)(Datapack+offset);
				    offset += 4;
				    CString bsvalStr=_T("");bsvalStr.Format(L"%d",bsval);
				    dlgtest.m_ListCtl.SetItemText(j,4,bsvalStr);
				}
			}
		}break;
	case 2205:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"昨收盘",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"数据位置",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"时间",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"成交价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//昨收盘         int
				int zsprice = *(int*)(Datapack+offset);
				offset += 4;
				CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				dlgtest.m_ListCtl.SetItemText(0,1,zspriceStr);
			}
			if(PackLen>offset)
			{
				//数据位置       int	下面返回的第一条记录记录号
				int datapos = *(int*)(Datapack+offset);
				offset += 4;
				CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
				dlgtest.m_ListCtl.SetItemText(0,2,dataposStr);
			}
			if(PackLen>offset)
			{
				//数据位置       int	下面返回的第一条记录记录号
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				dlgtest.m_ListCtl.SetItemText(0,3,RecdNumStr);
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+2);
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				dlgtest.m_ListCtl.SetItemText(i+1,0,XHno);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//时间           int
				if(PackLen>offset)
				{
					int timeval = *(int*)(Datapack+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    dlgtest.m_ListCtl.SetItemText(j,4,timevalStr);
				}
				//成交价         int
				if(PackLen>offset)
				{
					int cjprice = *(int*)(Datapack+offset);
				    offset += 4;
				    CString cjpriceStr=_T("");cjpriceStr.Format(L"%d",cjprice);
				    dlgtest.m_ListCtl.SetItemText(j,5,cjpriceStr);
				}
				//成交量         int
				if(PackLen>offset)
				{
					int cjvalue = *(int*)(Datapack+offset);
				    offset += 4;
				    CString cjvalueStr=_T("");cjvalueStr.Format(L"%d",cjvalue);
				    dlgtest.m_ListCtl.SetItemText(j,6,cjvalueStr);
				}
			}
		}break;
	case 2206:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"上涨家数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"平盘家数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"下跌家数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"均价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"加权均价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"总流通市值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"总市值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//上涨家数       short
				short szcount = *(short*)(Datapack+offset);
				offset += 2;
				CString szcountStr=_T("");szcountStr.Format(L"%d",szcount);
				dlgtest.m_ListCtl.SetItemText(0,1,szcountStr);
			}
			if(PackLen>offset)
			{
				//平盘家数       short
				short ppcount = *(short*)(Datapack+offset);
				offset += 2;
				CString ppcountStr=_T("");ppcountStr.Format(L"%d",ppcount);
				dlgtest.m_ListCtl.SetItemText(0,2,ppcountStr);
			}
			if(PackLen>offset)
			{
				//下跌家数       short
				short xdcount = *(short*)(Datapack+offset);
				offset += 2;
				CString xdcountStr=_T("");xdcountStr.Format(L"%d",xdcount);
				dlgtest.m_ListCtl.SetItemText(0,3,xdcountStr);
			}
			if(PackLen>offset)
			{
				//均价		int
				int avprice = *(int*)(Datapack+offset);
				offset += 4;
				CString avpriceStr=_T("");avpriceStr.Format(L"%d",avprice);
				dlgtest.m_ListCtl.SetItemText(0,4,avpriceStr);
			}
			if(PackLen>offset)
			{
				//加权均价	int
				int jqprice = *(int*)(Datapack+offset);
				offset += 4;
				CString jqpriceStr=_T("");jqpriceStr.Format(L"%d",jqprice);
				dlgtest.m_ListCtl.SetItemText(0,5,jqpriceStr);
			}
			if(PackLen>offset)
			{
				//总流通市值	int
				int zltval = *(int*)(Datapack+offset);
				offset += 4;
				CString zltvalStr=_T("");zltvalStr.Format(L"%d",zltval);
				dlgtest.m_ListCtl.SetItemText(0,6,zltvalStr);
			}
			if(PackLen>offset)
			{
				//总市值	int
				int totalval = *(int*)(Datapack+offset);
				offset += 4;
				CString totalvalStr=_T("");totalvalStr.Format(L"%d",totalval);
				dlgtest.m_ListCtl.SetItemText(0,7,totalvalStr);
			}
		}break;
	case 2207:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"最新价格",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"昨收盘",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"成交额",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//数据位置       int	下面返回的第一条记录记录号
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				dlgtest.m_ListCtl.SetItemText(i,0,XHno);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				if(PackLen>offset)
				{
					//名称	String	//名字
					short namelen = *(short*)(Datapack+offset);
					offset += 2;
					char *nameval = new char[namelen+1];
					memset(nameval,0,namelen+1);
					memcpy(nameval,Datapack+offset,namelen);
					wstring NameStr = UTF8_TO_UTF16(nameval);
					offset += namelen;
					dlgtest.m_ListCtl.SetItemText(j,2,NameStr.c_str());
					delete []nameval;
				}
				if(PackLen>offset)
			    {
				    //小数位数   byte
				    unsigned char pointval = *(Datapack+offset);
				    offset += 1;
					int pointvalue = pointval;
				    CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				    dlgtest.m_ListCtl.SetItemText(j,3,pointvalStr);
			    }
				if(PackLen>offset)
			    {
				    //最新价格	int
				    int newprice = *(int*)(Datapack+offset);
				    offset += 4;
				    CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
				    dlgtest.m_ListCtl.SetItemText(j,4,newpriceStr);
			    }
				if(PackLen>offset)
			    {
				    //昨收盘	int
				    int zsprice = *(int*)(Datapack+offset);
				    offset += 4;
				    CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				    dlgtest.m_ListCtl.SetItemText(j,5,zspriceStr);
			    }
				if(PackLen>offset)
			    {
				    //成交额	int	//值为0就不显示，成交额为万，客户端根据屏幕大小显示为万或者带0～2为小数的亿
				    int cjmoney = *(int*)(Datapack+offset);
				    offset += 4;
				    CString cjmoneyStr=_T("");cjmoneyStr.Format(L"%d",cjmoney);
				    dlgtest.m_ListCtl.SetItemText(j,6,cjmoneyStr);
			    }
			}
		}break;
	case 2300:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"净值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"上期净值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"累计净值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"截止日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"总记录数",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				dlgtest.m_ListCtl.SetItemText(i,0,XHno);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				if(PackLen>offset)
				{
					//代码           String
					short codelen = *(short*)(Datapack+offset);
					offset += 2;
					char *codeval = new char[codelen+1];
					memset(codeval,0,codelen+1);
					memcpy(codeval,Datapack+offset,codelen);
					wstring codevalStr = UTF8_TO_UTF16(codeval);
					offset += codelen;
					dlgtest.m_ListCtl.SetItemText(j,2,codevalStr.c_str());
					delete []codeval;
				}
				if(PackLen>offset)
				{
					//名称	String	//名字
					short namelen = *(short*)(Datapack+offset);
					offset += 2;
					char *nameval = new char[namelen+1];
					memset(nameval,0,namelen+1);
					memcpy(nameval,Datapack+offset,namelen);
					wstring NameStr = UTF8_TO_UTF16(nameval);
					offset += namelen;
					dlgtest.m_ListCtl.SetItemText(j,3,NameStr.c_str());
					delete []nameval;
				}
				if(PackLen>offset)
			    {
				    //小数位数   byte
				    unsigned char pointval = *(Datapack+offset);
				    offset += 1;
					int pointvalue = pointval;
				    CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				    dlgtest.m_ListCtl.SetItemText(j,4,pointvalStr);
			    }
				if(PackLen>offset)
			    {
				    //净值           int
				    int jzvalue = *(int*)(Datapack+offset);
				    offset += 4;
				    CString jzvalueStr=_T("");jzvalueStr.Format(L"%d",jzvalue);
				    dlgtest.m_ListCtl.SetItemText(j,5,jzvalueStr);
			    }
				if(PackLen>offset)
			    {
				    //上期净值       int
				    int sqjz = *(int*)(Datapack+offset);
				    offset += 4;
				    CString sqjzStr=_T("");sqjzStr.Format(L"%d",sqjz);
				    dlgtest.m_ListCtl.SetItemText(j,6,sqjzStr);
			    }
				if(PackLen>offset)
			    {
				    //累计净值       int
				    int ljjz = *(int*)(Datapack+offset);
				    offset += 4;
				    CString ljjzStr=_T("");ljjzStr.Format(L"%d",ljjz);
				    dlgtest.m_ListCtl.SetItemText(j,7,ljjzStr);
			    }
				if(PackLen>offset)
			    {
				    //截止日期       int
				    int enddate = *(int*)(Datapack+offset);
				    offset += 4;
				    CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
				    dlgtest.m_ListCtl.SetItemText(j,8,enddateStr);
			    }
			}
			if(PackLen>offset)
			{
				//总记录数       short
				short tolcnt = *(short*)(Datapack+offset);
				offset += 2;
				CString tolcntStr=_T("");tolcntStr.Format(L"%d",tolcnt);
				dlgtest.m_ListCtl.SetItemText(0,9,tolcntStr);
			}
		}break;
	case 2301:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"万份收益",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"7日年化收益率",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"上期万份收益",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"上期年化收益率",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"截止日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"总记录数",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				
			}
			for(int i=0; i<RecdNum; i++)
			{
				CString XHno=_T("");XHno.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				dlgtest.m_ListCtl.SetItemText(i,0,XHno);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				if(PackLen>offset)
				{
					//代码           String
					short codelen = *(short*)(Datapack+offset);
					offset += 2;
					char *codeval = new char[codelen+1];
					memset(codeval,0,codelen+1);
					memcpy(codeval,Datapack+offset,codelen);
					wstring codevalStr = UTF8_TO_UTF16(codeval);
					offset += codelen;
					dlgtest.m_ListCtl.SetItemText(j,2,codevalStr.c_str());
					delete []codeval;
				}
				if(PackLen>offset)
				{
					//名称	String	//名字
					short namelen = *(short*)(Datapack+offset);
					offset += 2;
					char *nameval = new char[namelen+1];
					memset(nameval,0,namelen+1);
					memcpy(nameval,Datapack+offset,namelen);
					wstring NameStr = UTF8_TO_UTF16(nameval);
					offset += namelen;
					dlgtest.m_ListCtl.SetItemText(j,3,NameStr.c_str());
					delete []nameval;
				}
				if(PackLen>offset)
			    {
				    //小数位数   byte
				    unsigned char pointval = *(Datapack+offset);
				    offset += 1;
					int pointvalue = pointval;
				    CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				    dlgtest.m_ListCtl.SetItemText(j,4,pointvalStr);
			    }
				if(PackLen>offset)
			    {
				    //万份收益       int
				    int wfvalue = *(int*)(Datapack+offset);
				    offset += 4;
				    CString wfvalueStr=_T("");wfvalueStr.Format(L"%d",wfvalue);
				    dlgtest.m_ListCtl.SetItemText(j,5,wfvalueStr);
			    }
				if(PackLen>offset)
			    {
				    //7日年化收益率  int
				    int Sevenpoint = *(int*)(Datapack+offset);
				    offset += 4;
				    CString SevenpointStr=_T("");SevenpointStr.Format(L"%d",Sevenpoint);
				    dlgtest.m_ListCtl.SetItemText(j,6,SevenpointStr);
			    }
				if(PackLen>offset)
			    {
				    //上期万份收益   int
				    int sqwfsy = *(int*)(Datapack+offset);
				    offset += 4;
				    CString sqwfsyStr=_T("");sqwfsyStr.Format(L"%d",sqwfsy);
				    dlgtest.m_ListCtl.SetItemText(j,7,sqwfsyStr);
			    }
				if(PackLen>offset)
			    {
				    //上期年化收益率 int
				    int sqnhsy = *(int*)(Datapack+offset);
				    offset += 4;
				    CString sqnhsyStr=_T("");sqnhsyStr.Format(L"%d",sqnhsy);
				    dlgtest.m_ListCtl.SetItemText(j,8,sqnhsyStr);
			    }
				if(PackLen>offset)
			    {
				    //截止日期       int
				    int enddate = *(int*)(Datapack+offset);
				    offset += 4;
				    CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
				    dlgtest.m_ListCtl.SetItemText(j,9,enddateStr);
			    }
			}
			if(PackLen>offset)
			{
				//总记录数       short
				short tolcnt = *(short*)(Datapack+offset);
				offset += 2;
				CString tolcntStr=_T("");tolcntStr.Format(L"%d",tolcnt);
				dlgtest.m_ListCtl.SetItemText(0,10,tolcntStr);
			}
		}break;
	case 2310:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"类型",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//代码           String
				short codelen = *(short*)(Datapack+offset);
				offset += 2;
				char *codeval = new char[codelen+1];
				memset(codeval,0,codelen+1);
				memcpy(codeval,Datapack+offset,codelen);
				wstring codevalStr = UTF8_TO_UTF16(codeval);
				offset += codelen;
				dlgtest.m_ListCtl.SetItemText(0,1,codevalStr.c_str());
				delete []codeval;
			}
			if(PackLen>offset)
			{
				//名称	String	//名字
				short namelen = *(short*)(Datapack+offset);
				offset += 2;
				char *nameval = new char[namelen+1];
				memset(nameval,0,namelen+1);
				memcpy(nameval,Datapack+offset,namelen);
				wstring NameStr = UTF8_TO_UTF16(nameval);
				offset += namelen;
				dlgtest.m_ListCtl.SetItemText(0,2,NameStr.c_str());
				delete []nameval;
			}
			if(PackLen>offset)
			{
				//类型           byte
				unsigned char tYPE = *(Datapack+offset);
				offset += 1;
			    int tYPEvalue = tYPE;
				CString tYPEStr=_T("");tYPEStr.Format(L"%d",tYPEvalue);
				dlgtest.m_ListCtl.SetItemText(0,3,tYPEStr);
			}
			if(PackLen>offset)
			{
				//小数位数   byte
				unsigned char pointval = *(Datapack+offset);
				offset += 1;
				int pointvalue = pointval;
				CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				dlgtest.m_ListCtl.SetItemText(0,4,pointvalStr);
			}
		}break;
	case 2311:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"净值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"累计净值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
				    if(PackLen>offset)
				    {
					    //日期           int
					    int datetime = *(int*)(Datapack+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    dlgtest.m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //净值           int
					    int jzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString jzvalStr=_T("");jzvalStr.Format(L"%d",jzval);
					    dlgtest.m_ListCtl.SetItemText(j,3,jzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //累计净值       int
					    int ljjzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
					    dlgtest.m_ListCtl.SetItemText(j,4,ljjzvalStr);
				    }
				}
			}
		}break;
	case 2312:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"股票代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"股票名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"上期持股",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"本期持股",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"占基金%",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"占流通股本%",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
			        {
				        //代码           String
				        short codelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *codeval = new char[codelen+1];
				        memset(codeval,0,codelen+1);
				        memcpy(codeval,Datapack+offset,codelen);
				        wstring codevalStr = UTF8_TO_UTF16(codeval);
				        offset += codelen;
				        dlgtest.m_ListCtl.SetItemText(j,2,codevalStr.c_str());
				        delete []codeval;
			        }
			        if(PackLen>offset)
			        {
				        //名称	String	//名字
				        short namelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *nameval = new char[namelen+1];
				        memset(nameval,0,namelen+1);
				        memcpy(nameval,Datapack+offset,namelen);
				        wstring NameStr = UTF8_TO_UTF16(nameval);
				        offset += namelen;
				        dlgtest.m_ListCtl.SetItemText(j,3,NameStr.c_str());
				        delete []nameval;
			        }
				    if(PackLen>offset)
				    {
					    //上期持股       int
					    int sqcgval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString sqcgvalStr=_T("");sqcgvalStr.Format(L"%d",sqcgval);
					    dlgtest.m_ListCtl.SetItemText(j,4,sqcgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //本期持股       int
					    int bqcgval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString bqcgvalStr=_T("");bqcgvalStr.Format(L"%d",bqcgval);
					    dlgtest.m_ListCtl.SetItemText(j,5,bqcgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //占基金%        int       保留两位小数，×100转成整数
					    int zjjval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString zjjvalStr=_T("");zjjvalStr.Format(L"%d",zjjval);
					    dlgtest.m_ListCtl.SetItemText(j,6,zjjvalStr);
				    }
					if(PackLen>offset)
				    {
					    //占流通股本%    int       保留两位小数，×100转成整数
					    int zltgbval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString zltgbvalStr=_T("");zltgbvalStr.Format(L"%d",zltgbval);
					    dlgtest.m_ListCtl.SetItemText(j,7,zltgbvalStr);
				    }
				}
			}
		}break;
	case 2313:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"截止日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"期初总份额",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"本期总申购",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"本期总赎回",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
				    if(PackLen>offset)
				    {
					    //截止日期       int       格式如“20080331”，最新的排在最前
					    int datetime = *(int*)(Datapack+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    dlgtest.m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //期初总份额     int       单位：万
					    int qczgbval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
					    dlgtest.m_ListCtl.SetItemText(j,3,qczgbvalStr);
				    }
					if(PackLen>offset)
				    {
					    //本期总申购     int       单位：万
					    int bqzsgval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
					    dlgtest.m_ListCtl.SetItemText(j,4,bqzsgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //本期总赎回     int       单位：万
					    int bqzshval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString bqzshvalStr=_T("");bqzshvalStr.Format(L"%d",bqzshval);
					    dlgtest.m_ListCtl.SetItemText(j,5,bqzshvalStr);
				    }
				}
			}
		}break;
	case 2314:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"截止日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"持有人户数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"户均份额",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"机构持有份额(万)",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"个人持有份额(万)",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
				    if(PackLen>offset)
				    {
					    //截止日期       int       格式如“20080331”，最新的排在最前
					    int datetime = *(int*)(Datapack+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    dlgtest.m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //持有人户数     int
					    int qczgbval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
					    dlgtest.m_ListCtl.SetItemText(j,3,qczgbvalStr);
				    }
					if(PackLen>offset)
				    {
					    //户均份额       int
					    int bqzsgval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
					    dlgtest.m_ListCtl.SetItemText(j,4,bqzsgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //机构持有份额   int       单位：万
					    int bqzshval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString bqzshvalStr=_T("");bqzshvalStr.Format(L"%d",bqzshval);
					    dlgtest.m_ListCtl.SetItemText(j,5,bqzshvalStr);
				    }
					if(PackLen>offset)
				    {
					    //个人持有份额   int       单位：万
					    int grcyfeval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString grcyfevalStr=_T("");grcyfevalStr.Format(L"%d",grcyfeval);
					    dlgtest.m_ListCtl.SetItemText(j,6,grcyfevalStr);
				    }
				}
			}
		}break;
	case 2315:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"概况信息",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				char *Msg = new char[RecdNum+1];
				memset(Msg,0,RecdNum+1);
				memcpy(Msg,Datapack+offset,RecdNum);
				offset += RecdNum;
				wstring contMsg = UTF8_TO_UTF16(Msg);
				dlgtest.m_ListCtl.SetItemText(0,1,contMsg.c_str());
				delete []Msg;
			}
		}break;
	case 2316:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"累计净值增长率",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
				    if(PackLen>offset)
				    {
					    //日期           int
					    int datetime = *(int*)(Datapack+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    dlgtest.m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //累计净值增长率     int
					    int qczgbval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
					    dlgtest.m_ListCtl.SetItemText(j,3,qczgbvalStr);
				    }
				}
			}
		}break;
	case 2320:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"标题",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
				    if(PackLen>offset)
				    {
					    //ID             int
					    int nId = *(int*)(Datapack+offset);
					    offset += 4;
					    CString nIdStr=_T("");nIdStr.Format(L"%d",nId);
					    dlgtest.m_ListCtl.SetItemText(j,2,nIdStr);
				    }
					if(PackLen>offset)
				    {
					    //标题           String
					    short ntiocelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *Msg = new char[ntiocelen+1];
				        memset(Msg,0,ntiocelen+1);
				        memcpy(Msg,Datapack+offset,ntiocelen);
				        offset += ntiocelen;
				        wstring contMsg = UTF8_TO_UTF16(Msg);
				        dlgtest.m_ListCtl.SetItemText(j,3,contMsg.c_str());
						delete []Msg;
				    }
				}
			}
		}break;
	case 2321:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"标题",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//标题           String
			    short ntiocelen = *(short*)(Datapack+offset);
				offset += 2;
				char *Msg = new char[ntiocelen+1];
				memset(Msg,0,ntiocelen+1);
				memcpy(Msg,Datapack+offset,ntiocelen);
				offset += ntiocelen;
				wstring contMsg = UTF8_TO_UTF16(Msg);
				dlgtest.m_ListCtl.SetItemText(0,1,contMsg.c_str());
			    delete []Msg;
			}
			if(PackLen>offset)
			{
				//内容           String
			    short contextlen = *(short*)(Datapack+offset);
				offset += 2;
				char *textMsg = new char[contextlen+1];
				memset(textMsg,0,contextlen+1);
				memcpy(textMsg,Datapack+offset,contextlen);
				offset += contextlen;
				wstring contMsgStr = UTF8_TO_UTF16(textMsg);
				dlgtest.m_ListCtl.SetItemText(0,2,contMsgStr.c_str());
			    delete []textMsg;
			}
		}break;
	case 2322:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
				    if(PackLen>offset)
				    {
					    //代码           String
					    short codelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *codeMsg = new char[codelen+1];
				        memset(codeMsg,0,codelen+1);
				        memcpy(codeMsg,Datapack+offset,codelen);
				        offset += codelen;
				        wstring codeMsgStr = UTF8_TO_UTF16(codeMsg);
				        dlgtest.m_ListCtl.SetItemText(j,2,codeMsgStr.c_str());
						delete []codeMsg;
				    }
					if(PackLen>offset)
				    {
					    //名称           String
					    short namelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *nameMsg = new char[namelen+1];
				        memset(nameMsg,0,namelen+1);
				        memcpy(nameMsg,Datapack+offset,namelen);
				        offset += namelen;
				        wstring nameMsgstr = UTF8_TO_UTF16(nameMsg);
				        dlgtest.m_ListCtl.SetItemText(j,3,nameMsgstr.c_str());
						delete []nameMsg;
				    }
				}
			}
		}break;
	case 2330:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"标题",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				short crcval = *(short*)(Datapack+offset);
				offset += 2;
				CString CRCval=_T("");CRCval.Format(L"%d",crcval);
				dlgtest.m_ListCtl.SetItemText(0,1,CRCval);
			}
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
				    {
					    //名称	 String
					    short ntiocelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *Msg = new char[ntiocelen+1];
				        memset(Msg,0,ntiocelen+1);
				        memcpy(Msg,Datapack+offset,ntiocelen);
				        offset += ntiocelen;
				        wstring contMsg = UTF8_TO_UTF16(Msg);
				        dlgtest.m_ListCtl.SetItemText(j,3,contMsg.c_str());
						delete []Msg;
				    }
				}
			}
		}break;
	case 2331:
		{
			char FundType = *(Datapack+offset);
			offset += 2;
			int Fundint = FundType;
			CString FundTypeStr = _T("");FundTypeStr.Format(L"%d",Fundint);

			//总记录数       short
			short tolNum = *(short*)(Datapack+offset);
			offset += 2;
			CString tolStr = _T("");tolStr.Format(L"%d",tolNum);

			//记录数		short
			short RecdNum=*(short*)(Datapack+offset);
			offset += 2;
			CString RecdNumStr=_T("");RecdNumStr.Format(L"%d",RecdNum);
			if(Fundint!=5)
			{  
			    dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			    dlgtest.m_ListCtl.InsertColumn(1,L"总记录数",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(5,L"当期日期",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(6,L"最新净值",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(7,L"累计净值",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(8,L"上期日期",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(9,L"上期净值",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(10,L"上期累计",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(11,L"日增长率",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(12,L"周增长率",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(13,L"月增长率",LVCFMT_LEFT,100);
		 	    dlgtest.m_ListCtl.InsertColumn(14,L"年增长率",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertItem(0,_T(""));
			    dlgtest.m_ListCtl.SetItemText(0,0,L"1");
				dlgtest.m_ListCtl.SetItemText(0,1,tolStr);
				dlgtest.m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
				    {
					    //代码           String
					    short codelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *codeMsg = new char[codelen+1];
				        memset(codeMsg,0,codelen+1);
				        memcpy(codeMsg,Datapack+offset,codelen);
				        offset += codelen;
				        wstring codeMsgStr = UTF8_TO_UTF16(codeMsg);
				        dlgtest.m_ListCtl.SetItemText(j,3,codeMsgStr.c_str());
						delete []codeMsg;
				    }
					if(PackLen>offset)
				    {
					    //名称           String
					    short namelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *nameMsg = new char[namelen+1];
				        memset(nameMsg,0,namelen+1);
				        memcpy(nameMsg,Datapack+offset,namelen);
				        offset += namelen;
				        wstring nameMsgStr = UTF8_TO_UTF16(nameMsg);
				        dlgtest.m_ListCtl.SetItemText(j,4,nameMsgStr.c_str());
						delete []nameMsg;
				    }
					if(PackLen>offset)
				    {
					    //当期日期       int
					    int TodayDate = *(int*)(Datapack+offset);
					    offset += 4;
					    CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
					    dlgtest.m_ListCtl.SetItemText(j,5,TodayDateStr);
				    }
					if(PackLen>offset)
				    {
					    //最新净值       int*10000
					    int zxjzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
					    dlgtest.m_ListCtl.SetItemText(j,6,zxjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //累计净值       int*10000 
					    int ljjzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
					    dlgtest.m_ListCtl.SetItemText(j,7,ljjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期日期       int  
					    int sqdateval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
					    dlgtest.m_ListCtl.SetItemText(j,8,sqdatevalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期净值       int*10000  
					    int sqval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString sqvalStr=_T("");sqvalStr.Format(L"%d",sqval);
					    dlgtest.m_ListCtl.SetItemText(j,9,sqvalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期累计       int*10000  
					    int sqljval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
					    dlgtest.m_ListCtl.SetItemText(j,10,sqljvalStr);
				    }
					if(PackLen>offset)
				    {
					    //日增长率       int*10000  
					    int rzzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString rzzvalStr=_T("");rzzvalStr.Format(L"%d",rzzval);
					    dlgtest.m_ListCtl.SetItemText(j,11,rzzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //周增长率       int*10000  
					    int zzzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString zzzvalStr=_T("");zzzvalStr.Format(L"%d",zzzval);
					    dlgtest.m_ListCtl.SetItemText(j,12,zzzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //月增长率       int*10000  
					    int mzzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString mzzvalStr=_T("");mzzvalStr.Format(L"%d",mzzval);
					    dlgtest.m_ListCtl.SetItemText(j,13,mzzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //周增长率       int*10000  
					    int yzzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString yzzvalStr=_T("");yzzvalStr.Format(L"%d",yzzval);
					    dlgtest.m_ListCtl.SetItemText(j,14,yzzvalStr);
				    }
				}
			}
			if(Fundint==5)
			{  
			    dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			    dlgtest.m_ListCtl.InsertColumn(1,L"总记录数",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(5,L"当期日期",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(6,L"万份收益",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(7,L"七日收益率",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(8,L"上期日期",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(9,L"万份收益",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertColumn(10,L"七日收益率",LVCFMT_LEFT,100);
			    dlgtest.m_ListCtl.InsertItem(0,_T(""));
			    dlgtest.m_ListCtl.SetItemText(0,0,L"1");
				dlgtest.m_ListCtl.SetItemText(0,1,tolStr);
				dlgtest.m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
				    {
					    //代码           String
					    short codelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *codeMsg = new char[codelen+1];
				        memset(codeMsg,0,codelen+1);
				        memcpy(codeMsg,Datapack+offset,codelen);
				        offset += codelen;
				        wstring codeMsgStr = UTF8_TO_UTF16(codeMsg);
				        dlgtest.m_ListCtl.SetItemText(j,3,codeMsgStr.c_str());
						delete []codeMsg;
				    }
					if(PackLen>offset)
				    {
					    //名称           String
					    short namelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *nameMsg = new char[namelen+1];
				        memset(nameMsg,0,namelen+1);
				        memcpy(nameMsg,Datapack+offset,namelen);
				        offset += namelen;
				        wstring nameMsgStr = UTF8_TO_UTF16(nameMsg);
				        dlgtest.m_ListCtl.SetItemText(j,4,nameMsgStr.c_str());
						delete []nameMsg;
				    }
					if(PackLen>offset)
				    {
					    //当期日期       int
					    int TodayDate = *(int*)(Datapack+offset);
					    offset += 4;
					    CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
					    dlgtest.m_ListCtl.SetItemText(j,5,TodayDateStr);
				    }
					if(PackLen>offset)
				    {
					    //万份收益       int*10000
					    int zxjzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
					    dlgtest.m_ListCtl.SetItemText(j,6,zxjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //七日收益率     int*10000 
					    int ljjzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
					    dlgtest.m_ListCtl.SetItemText(j,7,ljjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期日期       int  
					    int sqdateval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
					    dlgtest.m_ListCtl.SetItemText(j,8,sqdatevalStr);
				    }
					if(PackLen>offset)
				    {
					    //万份收益       int*10000  
					    int sqljval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
					    dlgtest.m_ListCtl.SetItemText(j,9,sqljvalStr);
				    }
					if(PackLen>offset)
				    {
					    //七日收益率     int*10000  
					    int rzzval = *(int*)(Datapack+offset);
					    offset += 4;
					    CString rzzvalStr=_T("");rzzvalStr.Format(L"%d",rzzval);
					    dlgtest.m_ListCtl.SetItemText(j,10,rzzvalStr);
				    }
				}
			}
		}break;
	case 2500:
		{
			short RecdNum=0;
			CString RecdNumStr=_T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"汇率名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"汇率数值",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//日期           int
			int dateval = *(int*)(Datapack+offset);
			offset += 4;
			CString dateStr = _T("");dateStr.Format(L"%d",dateval);
			dlgtest.m_ListCtl.SetItemText(0,1,dateStr);
			if(PackLen>offset)
			{
				//记录数         short
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
				for(int i=0; i<RecdNum; i++)
				{
					CString GS = _T("");GS.Format(L"%d",i+2);
					dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
					dlgtest.m_ListCtl.SetItemText(i+1,0,GS);
				}
				dlgtest.m_ListCtl.SetItemText(0,2,RecdNumStr);
				for(int j=0; j<RecdNum; j++)
				{
					if(PackLen>offset)
				    {
					    //汇率名称       String
					    short ntiocelen = *(short*)(Datapack+offset);
				        offset += 2;
				        char *Msg = new char[ntiocelen+1];
				        memset(Msg,0,ntiocelen+1);
				        memcpy(Msg,Datapack+offset,ntiocelen);
				        offset += ntiocelen;
				        wstring contMsg = UTF8_TO_UTF16(Msg);
				        dlgtest.m_ListCtl.SetItemText(j,3,contMsg.c_str());
						delete []Msg;
				    }
					if(PackLen>offset)
				    {
					    //小数位数       byte
					    unsigned char pointpos = *(Datapack+offset);
						offset += 1;
						int pointwz = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointwz);
						dlgtest.m_ListCtl.SetItemText(j,4,pointstr);
				    }
					if(PackLen>offset)
				    {
					    //汇率数值       int
					    int hlval = *(int*)(Datapack+offset);
						offset += 4;
						CString hlvalstr = _T("");hlvalstr.Format(L"%d",hlval);
						dlgtest.m_ListCtl.SetItemText(j,5,hlvalstr);
				    }
				}
			}
		}break;
	case 2600:
		{
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"下一ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"Url地址",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//下一ID         int         下一条的ID，没有下一条返回0

			if(PackLen>offset)
			{
				int nIdval = *(int*)(Datapack+offset);
				offset += 4;
				CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
				dlgtest.m_ListCtl.SetItemText(0,1,IdStr);
			}
			//内容           String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(Datapack+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,Datapack+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_TO_UTF16(context);
				dlgtest.m_ListCtl.SetItemText(0,2,textStr.c_str());
				delete []context;
			}
			// Url地址        String, 格式: ip:port:id,客户端根据平台生成url;该接口仅仅用于广发证券
			if(PackLen>offset)
			{
				short urladdrlen = *(short*)(Datapack+offset);
				offset += 2;
				char *urladdr = new char[urladdrlen+1];
				memset(urladdr,0,urladdrlen+1);
				memcpy(urladdr,Datapack+offset,urladdrlen);
				offset += urladdrlen;
				wstring urladdrStr = UTF8_TO_UTF16(urladdr);
				dlgtest.m_ListCtl.SetItemText(0,2,urladdrStr.c_str());
				delete []urladdr;
			}
		}break;
	case 2601:
		{
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"下一ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//ID            int         服务器端最新信息的id，如果没有返回0
			if(PackLen>offset)
			{
				int nIdval = *(int*)(Datapack+offset);
				offset += 4;
				CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
				dlgtest.m_ListCtl.SetItemText(0,1,IdStr);
			}
			//内容          String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(Datapack+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,Datapack+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_TO_UTF16(context);
				dlgtest.m_ListCtl.SetItemText(0,2,textStr.c_str());
				delete []context;
			}
		}break;
	case 2602:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"该类信息总数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"开始位置",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"内容",LVCFMT_LEFT,100);
			//该类信息总数  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
			    dlgtest.m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			//开始位置      short	//下列资讯第一条编号
			if(PackLen>offset)
			{
				short beginpos = *(short*)(Datapack+offset);
				offset += 2;
				CString beginStr = _T("");beginStr.Format(L"%d",beginpos);
				dlgtest.m_ListCtl.SetItemText(0,2,beginStr);
			}
			for(int j=0; j<RecdNum; j++)
			{
			    //内容          String
			    if(PackLen>offset)
			    {
				    short contextlen = *(short*)(Datapack+offset);
				    offset += 2;
				    char *context = new char[contextlen+1];
				    memset(context,0,contextlen+1);
				    memcpy(context,Datapack+offset,contextlen);
				    offset += contextlen;
				    wstring textStr = UTF8_TO_UTF16(context);
				    dlgtest.m_ListCtl.SetItemText(j,3,textStr.c_str());
				    delete []context;
			    }
			}
		}break;
	case 2700:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"分类ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"分类名称",LVCFMT_LEFT,100);
			//该类信息总数  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
			    dlgtest.m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//分类ID         int
				if(PackLen>offset)
				{
				    int TypeId = *(int*)(Datapack+offset);
				    offset += 2;
				    CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
				    dlgtest.m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
			    //内容          String
			    if(PackLen>offset)
			    {
				    short contextlen = *(short*)(Datapack+offset);
				    offset += 2;
				    char *context = new char[contextlen+1];
				    memset(context,0,contextlen+1);
				    memcpy(context,Datapack+offset,contextlen);
				    offset += contextlen;
				    wstring textStr = UTF8_TO_UTF16(context);
				    dlgtest.m_ListCtl.SetItemText(j,3,textStr.c_str());
				    delete []context;
			    }
			}
		}break;
	case 2701:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"标题",LVCFMT_LEFT,100);
			//该类信息总数  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
			    dlgtest.m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//ID             int
				if(PackLen>offset)
				{
				    int TypeId = *(int*)(Datapack+offset);
				    offset += 2;
				    CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
				    dlgtest.m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
			    //标题           String
			    if(PackLen>offset)
			    {
				    short contextlen = *(short*)(Datapack+offset);
				    offset += 2;
				    char *context = new char[contextlen+1];
				    memset(context,0,contextlen+1);
				    memcpy(context,Datapack+offset,contextlen);
				    offset += contextlen;
				    wstring textStr = UTF8_TO_UTF16(context);
				    dlgtest.m_ListCtl.SetItemText(j,3,textStr.c_str());
				    delete []context;
			    }
			}
		}break;
	case 2702:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"分类ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"分类名称",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"是否有子类",LVCFMT_LEFT,100);
			//该类信息总数  short
			if(PackLen>offset)
			{
				RecdNum = *(short*)(Datapack+offset);
				offset += 2;
				RecdNumStr.Format(L"%d",RecdNum);
			}
			for(int i=0; i<RecdNum; )
			{
				CString xhval=_T("");xhval.Format(L"%d",i+1);
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
			    dlgtest.m_ListCtl.SetItemText(i,0,xhval);
				i++;
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//ID             int
				if(PackLen>offset)
				{
				    int TypeId = *(int*)(Datapack+offset);
				    offset += 2;
				    CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
				    dlgtest.m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
			    //标题           String
			    if(PackLen>offset)
			    {
				    short contextlen = *(short*)(Datapack+offset);
				    offset += 2;
				    char *context = new char[contextlen+1];
				    memset(context,0,contextlen+1);
				    memcpy(context,Datapack+offset,contextlen);
				    offset += contextlen;
				    wstring textStr = UTF8_TO_UTF16(context);
				    dlgtest.m_ListCtl.SetItemText(j,3,textStr.c_str());
				    delete []context;
			    }
				//是否有子类     byte    0没有子菜单，1有;
				if(PackLen>offset)
				{
				    unsigned char Type = *(Datapack+offset);
					int bytetype = Type;
				    offset += 1;
				    CString bytetypestr = _T("");bytetypestr.Format(L"%d",Type);
				    dlgtest.m_ListCtl.SetItemText(j,4,bytetypestr);
				}
			}
		}break;
	case 2710:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"标题",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//标题           String
			if(PackLen>offset)
			{
				short topiclen = *(short*)(Datapack+offset);
				offset += 2;
				char *topic = new char[topiclen+1];
				memset(topic,0,topiclen+1);
				memcpy(topic,Datapack+offset,topiclen);
				offset += topiclen;
				wstring topicStr = UTF8_TO_UTF16(topic);
				dlgtest.m_ListCtl.SetItemText(0,1,topicStr.c_str());
				delete []topic;
			}
			//内容           String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(Datapack+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,Datapack+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_TO_UTF16(context);
				dlgtest.m_ListCtl.SetItemText(0,3,textStr.c_str());
				delete []context;
			}
		}break;
	case 2900:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"新弹出ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"消息内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//新弹出ID       int     为 0 时表示没有新消息
			if(PackLen>offset)
			{
				int idlen = *(int*)(Datapack+offset);
				offset += 4;
				CString idstr=_T("");idstr.Format(L"%d",idlen);
				dlgtest.m_ListCtl.SetItemText(0,1,idstr);
			}
			//内容           String
			if(PackLen>offset)
			{
				short contextlen = *(short*)(Datapack+offset);
				offset += 2;
				char *context = new char[contextlen+1];
				memset(context,0,contextlen+1);
				memcpy(context,Datapack+offset,contextlen);
				offset += contextlen;
				wstring textStr = UTF8_TO_UTF16(context);
				dlgtest.m_ListCtl.SetItemText(0,3,textStr.c_str());
				delete []context;
			}
		}break;
	case 2901:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"新弹出ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"消息内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//新广告ID       int     为 0 时表示没有新广告
			if(PackLen>offset)
			{
				int newsidlen = *(int*)(Datapack+offset);
				offset += 4;
				CString newsidlenstr=_T("");newsidlenstr.Format(L"%d",newsidlen);
				dlgtest.m_ListCtl.SetItemText(0,1,newsidlenstr);
			}
			//广告内容       String
			if(PackLen>offset)
			{
				short newslen = *(short*)(Datapack+offset);
				offset += 2;
				char *news = new char[newslen+1];
				memset(news,0,newslen+1);
				memcpy(news,Datapack+offset,newslen);
				offset += newslen;
				wstring newsStr = UTF8_TO_UTF16(news);
				dlgtest.m_ListCtl.SetItemText(0,3,newsStr.c_str());
				delete []news;
			}
		}break;
	case 2902:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"新图片ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"图片格式",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"图片数据",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//新图片ID       int      为 0 时表示没有新图片
			if(PackLen>offset)
			{
				int piclen = *(int*)(Datapack+offset);
				offset += 4;
				CString picstr=_T("");picstr.Format(L"%d",piclen);
				dlgtest.m_ListCtl.SetItemText(0,1,picstr);
			}
			//图片格式       char
			if(PackLen>offset)
			{
				char pictype = *(Datapack+offset);
				offset+=1;
				int picval = pictype;
				CString picstr = _T("");picstr.Format(L"%d",picval);
				dlgtest.m_ListCtl.SetItemText(0,2,picstr);
			}
		    //	图片数据       byte[]   如果没有新图片，可传回零字节长度的数组, 图片数据需按数组方式序列化(包含2字节标记字节数长度)
			if(PackLen>offset)
			{
				short picdatalen = *(short*)(Datapack+offset);
				offset += 2;
				char *picdata = new char[picdatalen+1];
				memset(picdata,0,picdatalen+1);
				memcpy(picdata,Datapack+offset,picdatalen);
				wstring picdatastr = UTF8_TO_UTF16(picdata);
				dlgtest.m_ListCtl.SetItemText(0,3,picdatastr.c_str());
				delete []picdata;
			}
		}break;
	case 2903:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"注册结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//注册结果	byte     0 表示正确，用户将会手机短信验证码；
			unsigned char nResult = *(Datapack+offset);
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2904:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"验证结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"用户id",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"right权限位码",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"权限日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//验证结果       byte     验证结果：
			unsigned char nResult = *(Datapack+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
			//[用户id	int]	 	0.验证成功，这个时候有用户id(收到该id后客户端要保存该id，以后登陆时候用），如果验证失败没有该字段
			int userid = *(int*)(Datapack+offset);
			offset += 4;
			CString useridstr = _T("");useridstr.Format(L"%d",userid);
			dlgtest.m_ListCtl.SetItemText(0,2,useridstr);
			//[long		right权限位码]  //验证成功返回的用户默认权限和日期，失败没有返
			__int64 qxval = *(__int64*)(Datapack+offset);
			offset += 8;
			char *b = new char[65];
			memset(b,0,65);
			for(int i=0;i<64;i++,qxval<<=1)
			{
				b[i] = (01&qxval)+'0';
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				int qxdata = b[i];
				CString qxdatastr=_T("");qxdatastr.Format(L"%d",qxdata);
				dlgtest.m_ListCtl.SetItemText(i,3,qxdatastr);
			}
			delete []b;
			//[int[]		权限日期，和right从0位到63对应]
			short sznum = *(short*)(Datapack+offset);
			offset += 2;
			for(int j=0; j<sznum; j++)
			{
				int data = *(int*)(Datapack+offset);
				offset += 4;
				CString datastr = _T("");datastr.Format(L"%d",data);
				dlgtest.m_ListCtl.SetItemText(j,4,datastr);
			}
		}break;
	case 2907:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"异常原因",LVCFMT_LEFT,100);
			int causelen = *(int*)(Datapack+offset);
			offset += 4;
			CString Datastr=_T("");Datastr.Format(L"%d",causelen);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			dlgtest.m_ListCtl.SetItemText(0,1,Datastr);
		}break;
	case 2908:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			unsigned char nResult = *(Datapack+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
		}
	case 2909:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"修改结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			unsigned char nResult = *(Datapack+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
		}
	case 2910:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"修改结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			unsigned char nResult = *(Datapack+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
			//等级		int
			int gradeval = *(int*)(Datapack+offset);
			offset += 4;
			CString gradestr = _T("");gradestr.Format(L"%d",gradeval);
			dlgtest.m_ListCtl.SetItemText(0,2,gradestr);
		}
	case 2911:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"操作结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"股票代码列表",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			char nResult = *(Datapack+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			
			//股票代码列表： string[] //取股票代码才有该字段，如果第一次取自选股可能没有该字段
			short codenum = *(short*)(Datapack+offset);
			offset += 2;
			for(int i=0; i<codenum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
				short codelen = *(short*)(Datapack+offset);
				offset += 2;
				char *code = new char[codelen+1];
				memset(code,0,codelen+1);
				memcpy(code,Datapack+offset,codelen);
				wstring codestr = UTF8_TO_UTF16(code);
				dlgtest.m_ListCtl.SetItemText(i,2,codestr.c_str());
				delete []code;
			}
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2912:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"操作结果",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"股票代码列表",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			char nResult = *(Datapack+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			
			//股票代码列表： string[] //取股票代码才有该字段，如果第一次取自选股可能没有该字段
			short codenum = *(short*)(Datapack+offset);
			offset += 2;
			for(int i=0; i<codenum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
				short codelen = *(short*)(Datapack+offset);
				offset += 2;
				char *code = new char[codelen+1];
				memset(code,0,codelen+1);
				memcpy(code,Datapack+offset,codelen);
				wstring codestr = UTF8_TO_UTF16(code);
				dlgtest.m_ListCtl.SetItemText(i,2,codestr.c_str());
				delete []code;
			}
			dlgtest.m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2913:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"滚动资讯类数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"id",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"滚动文本",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//滚动资讯类数	short	//表示下面有多少（id，string对数目）
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				
				dlgtest.m_ListCtl.InsertItem(i,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
				
			}
			for(int j=0; j<RecdNum; j++)
			{
				//id		char	//滚动类型，见下面定义  1(分时图滚动） 2(k线滚动）3(行情滚动) 4(首页资讯滚动)，注意相同类型的文本可能有多条
				if(PackLen>offset)
				{
					char idtype = *(Datapack+offset);
					offset += 1;
				    int idval = idtype;
				    CString idstr = _T("");idstr.Format(L"%d",idval);
					dlgtest.m_ListCtl.SetItemText(j,2,idstr);
				}
				//滚动文本	string	//id类型的滚动文本
				if(PackLen>offset)
				{
					short contextlen = *(short*)(PackLen+offset);
					offset += 2;
					char *text = new char[contextlen+1];
					memset(text,0,contextlen+1);
					memcpy(text,Datapack+offset,contextlen);
					offset += contextlen;
					wstring textstr = UTF8_TO_UTF16(text);
					dlgtest.m_ListCtl.SetItemText(j,3,textstr.c_str());
					delete []text;
				}
			}
		}break;
	case 2914:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"wml文件的内容",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			char *wmltext = new char[PackLen+1];
			memset(wmltext,0,PackLen+1);
			memcpy(wmltext,Datapack+offset,PackLen);
			offset += PackLen;
			wstring wmlstr = UTF8_TO_UTF16(wmltext);
			dlgtest.m_ListCtl.SetItemText(0,1,wmlstr.c_str());
			delete []wmltext;
		}break;
	case 2915:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"加权平均委买价格",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"委买总量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"加权平均委卖价格",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"委卖总量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"买卖价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"买卖量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//加权平均委买价格	int
			int avgprice = *(int*)(PackLen+offset);
			offset += 4;
			CString avgpricestr = _T("");avgpricestr.Format(L"%d",avgprice);
			dlgtest.m_ListCtl.SetItemText(0,1,avgpricestr);
			//委买总量		int
			int tolbuyval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolbuyvalstr = _T("");tolbuyvalstr.Format(L"%d",tolbuyval);
			dlgtest.m_ListCtl.SetItemText(0,2,tolbuyvalstr);
			//加权平均委卖价格	int
			int sellval = *(int*)(PackLen+offset);
			offset += 4;
			CString sellvalstr = _T("");sellvalstr.Format(L"%d",sellval);
			dlgtest.m_ListCtl.SetItemText(0,3,sellvalstr);
			//委卖总量		int
			int tolsellval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolsellvalstr = _T("");tolsellvalstr.Format(L"%d",tolsellval);
			dlgtest.m_ListCtl.SetItemText(0,4,tolsellvalstr);
			//记录数         	short       //6～10的买卖盘
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			dlgtest.m_ListCtl.SetItemText(0,5,RecdNumStr);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				dlgtest.m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//买卖价         	int
				if(PackLen>offset)
				{
					int buysell = *(int*)(Datapack+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					dlgtest.m_ListCtl.SetItemText(j,6,buysellstr);
				}
				//买卖量         	int
				if(PackLen>offset)
				{
					int buysellval = *(int*)(Datapack+offset);
					offset += 4;
					CString buysellvalstr = _T("");buysellvalstr.Format(L"%d",buysellval);
					dlgtest.m_ListCtl.SetItemText(j,7,buysellvalstr);
				}
			}
		}break;
	case 2916:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"数据位置",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"时间",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"成交价",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"成交量",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//数据位置       int
			int datapos = *(int*)(PackLen+offset);
			offset += 4;
			CString dataposstr = _T("");dataposstr.Format(L"%d",datapos);
			dlgtest.m_ListCtl.SetItemText(0,1,dataposstr);
			//记录数         	short       //6～10的买卖盘
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			dlgtest.m_ListCtl.SetItemText(0,2,RecdNumStr);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				dlgtest.m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//买卖价         	int
				if(PackLen>offset)
				{
					int datetime = *(int*)(Datapack+offset);
					offset += 4;
					CString datetimestr = _T("");datetimestr.Format(L"%d",datetime);
					dlgtest.m_ListCtl.SetItemText(j,3,datetimestr);
				}
				//买卖价         	int
				if(PackLen>offset)
				{
					int buysell = *(int*)(Datapack+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					dlgtest.m_ListCtl.SetItemText(j,4,buysellstr);
				}
				//买卖量         	int
				if(PackLen>offset)
				{
					int buysellval = *(int*)(Datapack+offset);
					offset += 4;
					CString buysellvalstr = _T("");buysellvalstr.Format(L"%d",buysellval);
					dlgtest.m_ListCtl.SetItemText(j,5,buysellvalstr);
				}
			}
		}break;
	case 2917:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"队列数据ID",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"买卖队列类型数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"买卖队列类型",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"买卖价格",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"总买（或卖）单数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"委托队列数组(股)",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"买卖队列类型",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"买卖价格",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"总买（或卖）单数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"委托队列数组(股)",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertItem(0,_T(""));
			dlgtest.m_ListCtl.SetItemText(0,0,L"1");
			//队列数据ID	int
			if(PackLen>offset)
			{
			int dataid = *(int*)(PackLen+offset);
			offset += 4;
			CString dataidstr = _T("");dataidstr.Format(L"%d",dataid);
			dlgtest.m_ListCtl.SetItemText(0,1,dataidstr);
			}
			if(PackLen>offset)
			{
			//买卖队列类型数	char	    1表示只有一种买或者卖，2表示买卖都有
			char buyselltype = *(Datapack+offset);
			offset += 1;
			int buysellval = buyselltype;
			CString buysellstr = _T("");buysellstr.Format(L"%d",buysellval);
			dlgtest.m_ListCtl.SetItemText(0,2,buysellstr);
			}
			//买卖队列类型	char	    0表示买, 1表示卖
			if(PackLen>offset)
			{
			char buysell = *(Datapack+offset);
			offset += 1;
			int buysellvalue = buysell;
			CString buysellvaluestr = _T("");buysellvaluestr.Format(L"%d",buysellvalue);
			dlgtest.m_ListCtl.SetItemText(0,3,buysellvaluestr);
			}
			//买卖价格		int	 
			if(PackLen>offset)
			{
			int priceval = *(int*)(PackLen+offset);
			offset += 4;
			CString pricevalstr = _T("");pricevalstr.Format(L"%d",priceval);
			dlgtest.m_ListCtl.SetItemText(0,4,pricevalstr);
			}
			//总买（或卖）单数  int
			if(PackLen>offset)
			{
			int tolalval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolalvalstr = _T("");tolalvalstr.Format(L"%d",tolalval);
			dlgtest.m_ListCtl.SetItemText(0,5,tolalvalstr);
			}
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+2);
				dlgtest.m_ListCtl.SetItemText(i+1,0,xh);
			}
			for(int j=0; j<RecdNum; j++)
			{
				//委托队列数组	int[]	    委托队列数组，该单位是股
				if(PackLen>offset)
				{
					int dateval = *(int*)(Datapack+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					dlgtest.m_ListCtl.SetItemText(j,6,datevalstr);
				}
			}
			//买卖队列类型	char	    0表示买, 1表示卖
			if(PackLen>offset)
			{
			char buysell2 = *(Datapack+offset);
			offset += 1;
			int buysell2value = buysell2;
			CString buysell2valuestr = _T("");buysell2valuestr.Format(L"%d",buysell2value);
			dlgtest.m_ListCtl.SetItemText(0,7,buysell2valuestr);
			}
			//买卖价格		int	 
			if(PackLen>offset)
			{
			int priceval2 = *(int*)(PackLen+offset);
			offset += 4;
			CString priceval2str = _T("");priceval2str.Format(L"%d",priceval2);
			dlgtest.m_ListCtl.SetItemText(0,8,priceval2str);
			}
			//总买（或卖）单数  int
			if(PackLen>offset)
			{
			int tolalval2 = *(int*)(PackLen+offset);
			offset += 4;
			CString tolalval2str = _T("");tolalval2str.Format(L"%d",tolalval2);
			dlgtest.m_ListCtl.SetItemText(0,9,tolalval2str);
			}
			//委托队列数组	int[]	    委托队列数组，该单位是股
			short RecdNum2 = *(short*)(Datapack+offset);
			offset += 2;
			if(RecdNum2>RecdNum)
			{
			    for(int m=0; m<RecdNum; m++)
			    {
				    dlgtest.m_ListCtl.InsertItem(m+1,_T(""));
				    CString xhval = _T("");xhval.Format(L"%d",m+2);
				    dlgtest.m_ListCtl.SetItemText(m+1,0,xhval);
			    }
			}
			for(int n=0; n<RecdNum2; n++)
			{
				//委托队列数组	int[]	    委托队列数组，该单位是股
				if(PackLen>offset)
				{
					int dateval2 = *(int*)(Datapack+offset);
					offset += 4;
					CString dateval2str = _T("");dateval2str.Format(L"%d",dateval2);
					dlgtest.m_ListCtl.SetItemText(n,10,dateval2str);
				}
			}
		}break;
	case 2918:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"ddx",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(Datapack+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					dlgtest.m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddx			short			显示3位小数
				if(PackLen>offset)
				{
					short ddxval = *(short*)(Datapack+offset);
					offset += 2;
					CString ddxvalstr = _T("");ddxvalstr.Format(L"%d",ddxval);
					dlgtest.m_ListCtl.SetItemText(j,3,ddxvalstr);
				}
			}
			
		}break;
	case 2919:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"ddy",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(Datapack+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					dlgtest.m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddy			short			显示3位小数
				if(PackLen>offset)
				{
					short ddyval = *(short*)(Datapack+offset);
					offset += 2;
					CString ddyvalstr = _T("");ddyvalstr.Format(L"%d",ddyval);
					dlgtest.m_ListCtl.SetItemText(j,3,ddyvalstr);
				}
			}
		}break;
	case 2920:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"ddy",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(Datapack+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					dlgtest.m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddz			int24			显示3位小数
				if(PackLen>offset)
				{
					//dlgtest.m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24自定义格式，用3个char类型的数组
					char *byte = new char[4];
					memset(byte,0,4);
					memcpy(byte,Datapack+offset,3);
					offset += 3;
					wstring bytestr = UTF8_TO_UTF16(byte);
					dlgtest.m_ListCtl.SetItemText(j,3,bytestr.c_str());
					delete []byte;
					char byteval[4]={0};
					byteval[0] = *(Datapack+offset-3);
					byteval[0] = byteval[0]&0x7F;
					byteval[1] = *(Datapack+offset-2);
					byteval[2] = *(Datapack+offset-1);
					string str;str.append(byteval[0],1);str.append(byteval[1],1);str.append(byteval[3],1);
					str.append(byteval[4],3);
					wstring strstr = UTF8_TO_UTF16(str);
					CString tempstr = _T("");tempstr.Format(L"%s-%s",bytestr.c_str(),strstr.c_str());
				}
				//ddzwidth		char			ddz线的宽度
				if(PackLen>offset)
				{
					char ddzwidth = *(Datapack+offset);
					offset += 1;
					int ddzwidthval = ddzwidth;
					CString ddzwidthvalstr = _T("");ddzwidthvalstr.Format(L"%d",ddzwidthval);
					dlgtest.m_ListCtl.SetItemText(j,4,ddzwidthvalstr);
				}
			}
		}break;
	case 2921:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"买入单数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(4,L"卖出单数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(5,L"买入中单以上比例",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(6,L"买入大单以上比例",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(7,L"买入特大单比例",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(8,L"卖出中单以上比例",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(9,L"卖出大单以上比例",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(10,L"卖出特大单比例",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(Datapack+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					dlgtest.m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//买入单数	uint24		//uint24是无符号的24位整数
				if(PackLen>offset)
				{
					//dlgtest.m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24自定义格式，用3个char类型的数组
					char *buybyte = new char[4];
					memset(buybyte,0,4);
					memcpy(buybyte,Datapack+offset,3);
					offset += 3;
					wstring buybytestr = UTF8_TO_UTF16(buybyte);
					dlgtest.m_ListCtl.SetItemText(j,3,buybytestr.c_str());
					delete []buybyte;
				}
				//卖出单数	uint24		//uint24是无符号的24位整数
				if(PackLen>offset)
				{
					//dlgtest.m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24自定义格式，用3个char类型的数组
					char *sellbyte = new char[4];
					memset(sellbyte,0,4);
					memcpy(sellbyte,Datapack+offset,3);
					offset += 3;
					wstring sellbytestr = UTF8_TO_UTF16(sellbyte);
					dlgtest.m_ListCtl.SetItemText(j,4,sellbytestr.c_str());
					delete []sellbyte;
				}
				//买入中单以上比例	short	×1000		包含中、大和特大单
				if(PackLen>offset)
				{
					short buybl = *(short*)(Datapack+offset);
					offset += 2;
					CString buyblstr = _T("");buyblstr.Format(L"%d",buybl);
					dlgtest.m_ListCtl.SetItemText(j,5,buyblstr);
				}
				//买入大单以上比例	short	×1000 		包含大和特大单
				if(PackLen>offset)
				{
					short buybig = *(short*)(Datapack+offset);
					offset += 2;
					CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
					dlgtest.m_ListCtl.SetItemText(j,6,buybigstr);
				}
				//买入特大单比例		short	×1000 		特大单
				if(PackLen>offset)
				{
					short buybiggest = *(short*)(Datapack+offset);
					offset += 2;
					CString buybiggeststr = _T("");buybiggeststr.Format(L"%d",buybiggest);
					dlgtest.m_ListCtl.SetItemText(j,7,buybiggeststr);
				}
				//卖出中单以上比例	short	×1000		包含中、大和特大单
				if(PackLen>offset)
				{
					short sellmid = *(short*)(Datapack+offset);
					offset += 2;
					CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
					dlgtest.m_ListCtl.SetItemText(j,8,sellmidstr);
				}
				//卖出大单以上比例	short	×1000 		包含大和特大单
				if(PackLen>offset)
				{
					short sellbig = *(short*)(Datapack+offset);
					offset += 2;
					CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
					dlgtest.m_ListCtl.SetItemText(j,9,sellbigstr);
				}
				//卖出特大单比例		short	×1000 		特大单
				if(PackLen>offset)
				{
					short sellbiggest = *(short*)(Datapack+offset);
					offset += 2;
					CString sellbiggeststr = _T("");sellbiggeststr.Format(L"%d",sellbiggest);
					dlgtest.m_ListCtl.SetItemText(j,8,sellbiggeststr);
				}
			}
		}break;
	case 2922:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"ddx数值",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//ddx数值	short	×1000	
				if(PackLen>offset)
				{
					short dateval = *(short*)(Datapack+offset);
					offset += 2;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					dlgtest.m_ListCtl.SetItemText(j,2,datevalstr);
				}
			}
		}break;
	case 2923:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"单数差值",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//单数差值	int24		//int24是24位整数	
				if(PackLen>offset)
				{
					char *dataval = new char[4];
					memset(dataval,0,4);
					memcpy(dataval,Datapack+offset,3);
					offset += 3;
					wstring datavalstr = UTF8_TO_UTF16(dataval);
					dlgtest.m_ListCtl.SetItemText(j,2,datavalstr.c_str());
					delete []dataval;
				}
			}
		}break;
	case 2924:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			dlgtest.m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			dlgtest.m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(2,L"总买",LVCFMT_LEFT,100);
			dlgtest.m_ListCtl.InsertColumn(3,L"总卖",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
			RecdNum = *(short*)(Datapack+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			for(int i=0; i<RecdNum; i++)
			{
				dlgtest.m_ListCtl.InsertItem(i+1,_T(""));
				CString xh = _T("");xh.Format(L"%d",i+1);
				dlgtest.m_ListCtl.SetItemText(i,0,xh);
			}
			dlgtest.m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				//总买		int	
				if(PackLen>offset)
				{
					int tolbuy = *(int*)(Datapack+offset);
					offset += 2;
					CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);
					dlgtest.m_ListCtl.SetItemText(j,2,tolbuystr);
				}
				//总卖		int
				if(PackLen>offset)
				{
					int tolsell = *(int*)(Datapack+offset);
					offset += 2;
					CString tolsellstr = _T("");tolsellstr.Format(L"%d",tolsell);
					dlgtest.m_ListCtl.SetItemText(j,3,tolsellstr);
				}
			}
		}break;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
// 解包函数
void CPack::AyncPack(char * &PackBody, unsigned short PackLen, unsigned short PackType)
{
	//单个解包，对每一个包进行解包，配置文档比较混淆，一个个的解包比较清晰，解包比较繁琐
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
	//根据应答字典来插入listctrl列名
	//定义功能号所对应的combox组合框里的序号位置
	int funpos = 0;
	for(int qq = 0; qq<Xml.XmlFun.size(); qq++)
	{
		if(PackType == Xml.XmlFun[qq].FunID)
		{
			funpos = qq;
			break;
		}
	}
	m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
	printf("解listctrl控件列表，功能号位置:%d\n",funpos);
	switch(PackType)
	{
	case 1001:
		{
			char nflag = *(PackBody+offset);
			int nMark = nflag;
			if(nMark == 0)
			{
				for(int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			else
			{
				for(int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
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
				for(int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			else
			{
				for(int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
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
				for(int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			if(Fundint==5)
			{
				for(int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
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
				for(int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
			if(nMark!=0)
			{
				for(int yy = 0; yy<Xml.AnsData[PackType][1].size(); yy++)
	            {
		            m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][1][yy].Dictname,LVCFMT_LEFT,100);
	            }
			}
		}break;
	default:
		{
			for(int yy = 0; yy<Xml.AnsData[PackType][0].size(); yy++)
	        {
		        m_ListCtl.InsertColumn(yy+1,Xml.AnsData[PackType][0][yy].Dictname,LVCFMT_LEFT,100);
	        }
		}break;
	}
	printf("解listctrl控件列表结束\n");
	switch(PackType)
	{
	case 1000:
		{
			//返回应答包的字段个数,map数组里面的个数
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			m_ListCtl.InsertColumn(1,L"行情地址",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"委托地址",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"公告信息",LVCFMT_LEFT,150);
			m_ListCtl.InsertColumn(4,L"新版本号",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"下载地址",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"提醒升级",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"强制升级",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"是否提示登录",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"用户运营商ip类",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"统计信息时间间隔",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"升级提示文字",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"公告crc",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"公告提示类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"调度地址",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"服务ID个数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"服务id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"服务器地址",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			//解应答包里面的行情地址字段，此字段是字符串数组格式string【】
			//解行情数组个数
			short hqaddrnum = *(short*)(PackBody+offset);
			offset += 2;
			//解行情地址长度字段
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
				    //nRow = m_ListCtl.InsertItem(j,No);// 插入行
				    m_ListCtl.SetItemText(j,0,No);
				    m_ListCtl.SetItemText(j, 1, str.c_str());//设置其它列数据
				    delete []hqaddr;
			    }
			}
			//解析委托地址字段
			if(PackLen>offset)
			{
			    short Entrustaddrnum = *(short*)(PackBody+offset);   //委托地址个数
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
				    m_ListCtl.SetItemText(t, 2, str.c_str());//设置其它列数据
				    delete []Enaddr;
			    }
			}
			//解公告信息  string类型
			if(PackLen>offset)
			{
			    short Noticelength = *(short*)(PackBody+offset);   //公告信息字符串长度
			    offset += 2;
				if(Noticelength!=0)
				{
			    char *NoticeMsg = new char[Noticelength+1];
			    memset(NoticeMsg,0,Noticelength+1);
			    memcpy(NoticeMsg,PackBody+offset,Noticelength);
			    offset+=Noticelength;
			    wstring str = UTF8_2_UTF16(NoticeMsg);
				CString wNotice = _T("");wNotice.Format(L"%ws",str.c_str());
			    m_ListCtl.SetItemText(0, 3, wNotice);//设置其它列数据
			    delete []NoticeMsg;
				}
			}
			//解新版本号       String
			if(PackLen>offset)
			{
			    short Verlength = *(short*)(PackBody+offset);   //公告信息字符串长度
			    offset += 2;
			    char *VerMsg = new char[Verlength+1];
			    memset(VerMsg,0,Verlength+1);
			    memcpy(VerMsg,PackBody+offset,Verlength);
			    offset+=Verlength;
			    wstring Verstr = UTF8_2_UTF16(VerMsg);
			    m_ListCtl.SetItemText(0, 4, Verstr.c_str());//设置其它列数据
			    delete []VerMsg;
			}
			//解下载地址       String
			if(PackLen>offset)
			{
			    short Addrlength = *(short*)(PackBody+offset);   //公告信息字符串长度
			    offset += 2;
			    char *AddrMsg = new char[Addrlength+1];
			    memset(AddrMsg,0,Addrlength+1);
			    memcpy(AddrMsg,PackBody+offset,Addrlength);
			    offset+=Addrlength;
			    wstring Addrstr = UTF8_2_UTF16(AddrMsg);
			    m_ListCtl.SetItemText(0, 5, Addrstr.c_str());//设置其它列数据
			    delete []AddrMsg;
			}
			//提醒升级       byte
			if(PackLen>offset)
			{
			    unsigned char UpdateMsg = *(PackBody+offset);
			    offset += 1;
			    int tempch = UpdateMsg;
			    CString tempstr = _T("");tempstr.Format(L"%d",tempch);
			    m_ListCtl.SetItemText(0, 6, tempstr);//设置其它列数据
			}
			//强制升级       byte      1强制 0不强制
			if(PackLen>offset)
			{
			    unsigned char FuMsg = *(PackBody+offset);
			    offset += 1;
			    int tempFU = FuMsg;
			    CString FU = _T("");FU.Format(L"%d",tempFU);
			    m_ListCtl.SetItemText(0, 7, FU);//设置其它列数据
			}
			//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
			if(PackLen>offset)
			{
			    unsigned char NLMsg = *(PackBody+offset);
			    offset += 1;
			    int tempNL = NLMsg;
			    CString NL = _T("");NL.Format(L"%d",tempNL);
			    m_ListCtl.SetItemText(0, 8, NL);//设置其它列数据
			}
			//用户运营商ip类 byte      0表示未知；非0表示有效，
			if(PackLen>offset)
			{
			    unsigned char IPMsg = *(PackBody+offset);
			    offset += 1;
			    int tempIP = IPMsg;
			    CString IPstr = _T("");IPstr.Format(L"%d",tempIP);
			    m_ListCtl.SetItemText(0, 9, IPstr);//设置其它列数据
			}
			
			//统计信息时间间隔 short   单位秒,如果为0表示不统计信息.	//20120307日增加
			if(PackLen>offset)
			{
			    short Timestap = *(short*)(PackBody+offset);
			    offset += 2;
			    CString TStamp = _T("");TStamp.Format(L"%u",Timestap);
			    m_ListCtl.SetItemText(0, 10, TStamp);//设置其它列数据
			}
			//升级提示文字   String
			if(PackLen>offset)
			{
			    short UNlength = *(short*)(PackBody+offset);   //公告信息字符串长度
			    offset += 2;
			    char *UNMsg = new char[UNlength+1];
			    memset(UNMsg,0,UNlength+1);
			    memcpy(UNMsg,PackBody+offset,UNlength);
			    offset+=UNlength;
			    wstring UNstr = UTF8_2_UTF16(UNMsg);
			    m_ListCtl.SetItemText(0, 11, UNstr.c_str());//设置其它列数据
			    delete []UNMsg;
			}
			//公告crc        short 
			if(PackLen>offset)
			{
			    short CRCValue = *(short*)(PackBody+offset);
			    offset += 2;
			    CString tempCRC = _T("");tempCRC.Format(L"%u",CRCValue);
			    m_ListCtl.SetItemText(0, 12, tempCRC);//设置其它列数据
			}
			//公告提示类型   char	//0或者没有该字段表示强制弹出；1每日弹出一次；2同一公告只弹出一次；3不弹出公告
			if(PackLen>offset)
			{
			    unsigned char NoticeType = *(PackBody+offset);
			    offset += 1;
			    int tempNotice = NoticeType;
			    CString Notice = _T("");Notice.Format(L"%d",tempNotice);
			    m_ListCtl.SetItemText(0, 13, Notice);//设置其它列数据
			}

			//调度地址       String[]  地址格式为 ip:port:id  //id 新增加，id是运营商id 2015-3-23
			if(PackLen>offset)
			{
			    short Addrnum = *(short*)(PackBody+offset);   //委托地址个数
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
				    m_ListCtl.SetItemText(f, 14, str.c_str());//设置其它列数据
				    delete []Addr;
			    }
			}
			//下列不同服务地址列表数  short
			if(PackLen>offset)
			{
			    short ServerNum = *(short*)(PackBody+offset);
			    offset += 2;
			    CString tempSvr = _T("");tempSvr.Format(L"%u",ServerNum);
			    m_ListCtl.SetItemText(0, 15, tempSvr);//设置其它列数据
			}
			//服务id             int
			if(PackLen>offset)
			{
			    int ServerID = *(int*)(PackBody+offset);
			    offset += 4;
			    CString tempSvrID = _T("");tempSvrID.Format(L"%d",ServerID);
			    m_ListCtl.SetItemText(0, 16, tempSvrID);//设置其它列数据
			}
			//服务器地址         String[]	//服务器地址格式：ip:port:id
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
				    m_ListCtl.SetItemText(m, 17, str.c_str());//设置其它列数据
				    delete []Ser;
			    }
			}
		}break;
	case 1001:
		{
			//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
			if(PackLen>offset)
			{
			    char nflag = *(PackBody+offset);
			    offset += 1;
				int nMark = nflag;
				if(nMark == 0)
				{
					//返回应答包的字段个数,map数组里面的个数
			        m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			        m_ListCtl.InsertColumn(1,L"类型",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(2,L"券商数",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(3,L"券商名称",LVCFMT_LEFT,sizeof(L"券商名称")*50);
			        CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
			        m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
					//券商数：	short（下列数目）
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
					m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			        m_ListCtl.InsertColumn(1,L"类型",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(2,L"行情地址标记",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(3,L"委托是否支持活字典",LVCFMT_LEFT,sizeof(L"委托是否支持活字典")*10);
					m_ListCtl.InsertColumn(4,L"委托地址数目",LVCFMT_LEFT,100);
					m_ListCtl.InsertColumn(5,L"委托地址",LVCFMT_LEFT,150);
					m_ListCtl.InsertColumn(6,L"委托端口",LVCFMT_LEFT,100);
					m_ListCtl.InsertColumn(7,L"券商行情分配服务器地址数",LVCFMT_LEFT,150);
					m_ListCtl.InsertColumn(8,L"行情分配地址",LVCFMT_LEFT,150);
					m_ListCtl.InsertColumn(9,L"行情分配端口",LVCFMT_LEFT,150);
					CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
			        m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
					//行情地址标记		char	//1表示带券商行情地址；0表示不带券商行情地址
					if(PackLen>offset)
					{
					    char hqmark = *(PackBody+offset);
					    offset += 1;
					    int HQmark = hqmark;
					    CString HQflag = _T("");HQflag.Format(L"%d",HQmark);
					    m_ListCtl.SetItemText(0, 2, HQflag);//设置其它列数据
					}
					//委托是否支持活字典	char	//1表示支持活字典，0表示不支持
					if(PackLen>offset)
					{
					    char ISDict = *(PackBody+offset);
					    offset += 1;
					    int DICT = ISDict;
					    CString isDict = _T("");isDict.Format(L"%d",DICT);
					    m_ListCtl.SetItemText(0, 3, isDict);//设置其它列数据
					}
					//委托地址数目：short（下列数目）
					if(PackLen>offset)
					{
					    short WTnum = *(short*)(PackBody+offset);
					    offset += 2;
						CString wtcount = _T("");wtcount.Format(L"%d",WTnum);
					    m_ListCtl.SetItemText(0, 4, wtcount);//设置其它列数据
						//委托地址	string
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
								//解委托端口	short
								short WTport = *(short*)(PackBody+offset);
								offset += 2;
								CString wtport = _T("");wtport.Format(L"%d",WTport);
								m_ListCtl.SetItemText(q,6,wtport);
							}
						}
					}
					//券商行情分配服务器地址数：short	//如果行情地址标记为0，没有该及以下字段	
					if(PackLen>offset)
					{
					    short HQnum = *(short*)(PackBody+offset);
					    offset += 2;
						CString hqcount = _T("");hqcount.Format(L"%d",HQnum);
					    m_ListCtl.SetItemText(0, 7, hqcount);//设置其它列数据
						//委托地址	string
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
								//解委托端口	short
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
			//定义公共常量变量参数
			short QSnum=0;   //券商个数
			short SXnum = 0;  //扩展属性数目
			short ADDRnum = 0; //地址数目,地址数目和扩展属性是同一级别
			if(PackLen>offset)
			{
				m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(1,L"地址信息crc",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(2,L"券商数目",LVCFMT_LEFT,80);
			    m_ListCtl.InsertColumn(3,L"券商名称",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(4,L"券商名称首拼音",LVCFMT_LEFT,sizeof(L"券商名称首拼音")*5);
				m_ListCtl.InsertColumn(5,L"扩展属性数目",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(6,L"属性序号",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(7,L"属性值",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(8,L"地址数目",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(9,L"地址",LVCFMT_LEFT,100);
				m_ListCtl.InsertColumn(10,L"地址端口",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(11,L"ip地址运营商类别",LVCFMT_LEFT,100);
				m_ListCtl.InsertColumn(12,L"地址类型",LVCFMT_LEFT,80);
				//解地址信息CRC值 short型
				short CRCval = *(short*)(PackBody+offset);
			    offset += 2;
				CString CRCfg = _T("");CRCfg.Format(L"%u",CRCval);
				m_ListCtl.SetItemText(0,1,CRCfg);
				//解short           券商数目
				if(PackLen>offset)
				{
				    QSnum = *(short*)(PackBody+offset);
				    offset += 2;
				    CString QSCount = _T("");QSCount.Format(L"%u",QSnum);
				    m_ListCtl.SetItemText(0,2,QSCount);
				    //解券商名称 string   ,由于下面都是券商个数的子列表，用for循环
					if(PackLen>offset)
					{
				        for(int i=0; i<QSnum; i++)
				        {
					        //解String      券商名称
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
								//解char        券商名称手拼音
								char QSP = *(PackBody+offset);
								offset += 1;
								CString zmstr = _T("");zmstr.Format(L"%c",QSP);
								m_ListCtl.SetItemText(i*SXnum,4,zmstr);
							}
							if(PackLen>offset)
							{
								//解short	下列扩展属性数目
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
									// char     属性序号
									char AttrNo = *(PackBody+offset);
									offset += 1;
									int attrno = AttrNo;
									CString Attr=_T("");Attr.Format(L"%d",attrno);
									m_ListCtl.SetItemText(r+i*SXnum,6,Attr);

									//char	属性值
									char AttrVal = *(PackBody+offset);
									offset += 1;
									int attrval = AttrVal;
									CString Attr2=_T("");Attr2.Format(L"%d",attrno);
									m_ListCtl.SetItemText(r+i*SXnum,7,Attr2);
								}
							}
							if(PackLen>offset)
							{
								//解short	地址数目
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
									// string   地址
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
									//解short    端口
									if(PackLen>offset)
									{
										short port = *(short*)(PackBody+offset);
										offset += 2;
										CString addrport = _T("");addrport.Format(L"",port);
										m_ListCtl.SetItemText(t+i*ADDRnum,10,addrport);
									}
									//ip地址运营商类别  char
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
									    //char     地址类型	 //1表示委托地址，2表示行情分配地址
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
			//先判断请求字段是0还是其他数值
			////城市营业部营业部数目
			short CityCnt = 0;
			int idvalue = *(int*)(SendBuffer+sizeof(HQ_HEAD));
			if(idvalue == 0)     //为0表示城市索引,返回String[]     城市索引
			{
				m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(1,L"城市索引",LVCFMT_LEFT,100);
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
				m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,80);
				m_ListCtl.InsertColumn(1,L"营业部数目",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(2,L"营业部名称",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(3,L"营业部地址",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(4,L"营业部电话",LVCFMT_LEFT,sizeof(L"券商名称首拼音")*5);
				m_ListCtl.InsertColumn(5,L"营业部google搜索地址",LVCFMT_LEFT,100);
				m_ListCtl.InsertColumn(6,L"营业部经度",LVCFMT_LEFT,100);
				m_ListCtl.InsertColumn(7,L"营业部纬度",LVCFMT_LEFT,100);
				if(PackLen>offset)
				{
					//short        下面数组数目
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
						    //String    营业部名称
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
							 //String    营业部地址
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
							 //String    营业部电话
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
							 //String    营业部google搜索地址
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
							 //String    营业部经度
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
							 //String    营业部weidu
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"是否通过",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"提示信息",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			if(PackLen>offset)
			{
				//解  是否通过       byte       1通过 0没通过
				char chPass = *(PackBody+offset);
				offset += 1;
				int szch = chPass;
				CString PassVal=_T("");PassVal.Format(L"%d",szch);
				m_ListCtl.SetItemText(0,0,L"1");
				m_ListCtl.SetItemText(0,1,PassVal);
			}
			if(PackLen>offset)
			{
				//解提示信息       String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"同步结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"自选代码",LVCFMT_LEFT,100);
			//定义收包的字段
			unsigned char nResult = ' ';
			CString byte=_T("");
			if(PackLen>offset)
			{
				//解同步结果       byte       1成功 0失败
				nResult = *(PackBody+offset);
				offset += 1;
				int Rbyte = nResult;
				byte.Format(L"%d",Rbyte);
				m_ListCtl.SetItemText(0,0,L"1");
				m_ListCtl.SetItemText(0,1,byte);
			}
			if(PackLen>offset)
			{
				//解自选代码       String[]
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
			//解提示信息       String
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"提示信息",LVCFMT_LEFT,100);
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
			//是否有效       byte       1有效 0无效
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"是否有效",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"通信密码",LVCFMT_LEFT,100);
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
			//是否有效       byte       1有效 0无效
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"手机号码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"通信密码",LVCFMT_LEFT,100);
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
			//记录数         short
			short Num = 0;
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
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
			short Num = 0;     //记录数
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"有无公告",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"请求板块指数id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"总记录数",LVCFMT_LEFT,100);
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
				//"代码" type="ans_string"
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
				//"名称" type="ans_string"
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
					//解小数点位数
					unsigned char pointval = *(PackBody+offset);
					offset += 1;
					int PtVal = pointval;
					CString PtStr=_T("");PtStr.Format(L"%d",PtVal);
					m_ListCtl.SetItemText(j,4,PtStr);
				}
				if(PackLen>offset)
				{
					//解最新价
					int newprice = *(int*)(PackBody+offset);
					offset += 4;
					CString NPStr=_T("");NPStr.Format(L"%d",newprice);
					m_ListCtl.SetItemText(j,5,NPStr);
				}
				if(PackLen>offset)
				{
					//解昨收价
					int YTrice = *(int*)(PackBody+offset);
					offset += sizeof(int);
					CString YTriceStr=_T("");YTriceStr.Format(L"%d",YTrice);
					m_ListCtl.SetItemText(j,6,YTriceStr);
				}
				if(PackLen>offset)
				{
					//解成交量
					int MKVal = *(int*)(PackBody+offset);
					offset += 4;
					CString MKValStr=_T("");MKValStr.Format(L"%d",MKVal);
					m_ListCtl.SetItemText(j,7,MKValStr);
				}
				if(PackLen>offset)
				{
					//解成交额
					int MKMoney = *(int*)(PackBody+offset);
					offset += 4;
					CString MKMoneyStr=_T("");MKMoneyStr.Format(L"%d",MKMoney);
					m_ListCtl.SetItemText(j,8,MKMoneyStr);
				}
				if(PackLen>offset)
				{
					//解最高价
					int hprice = *(int*)(PackBody+offset);
					offset += 4;
					CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
					m_ListCtl.SetItemText(j,9,hpriceStr);
				}
				if(PackLen>offset)
				{
					//解最低价
					int lprice = *(int*)(PackBody+offset);
					offset += 4;
					CString LowStr=_T("");LowStr.Format(L"%d",lprice);
					m_ListCtl.SetItemText(j,10,LowStr);
				}
				if(PackLen>offset)
				{
					//有无公告
					unsigned char NoticeVal = *(PackBody+offset);
					offset += 1;
					int IsVal = NoticeVal;
					CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
					m_ListCtl.SetItemText(j,11,IsValStr);
				}
				if(PackLen>offset)
				{
					//请求板块指数id" type="ans_short"
					short IndexID = *(short*)(PackBody+offset);
					offset += 2;
					CString indexstr=_T("");indexstr.Format(L"%d",IndexID);
					m_ListCtl.SetItemText(j,12,indexstr);
				}
			}
			if(PackLen>offset)
			{
					//总记录数" type="ans_short
				short TNum = *(short*)(PackBody+offset);
				offset += 2;
				CString TNumStr=_T("");TNumStr.Format(L"%d",TNum);
				m_ListCtl.SetItemText(0,13,TNumStr);
			}
		}break;
	case 2200:
		{
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"AH代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"总股本",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"涨停价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"跌停价",LVCFMT_LEFT,100);
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
		    //"名称" type="ans_string"
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
				//类型
				unsigned char Type = *(PackBody+offset);
				offset += 1;
				int IsVal = Type;
				CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
				m_ListCtl.SetItemText(0,3,IsValStr);
			}
			if(PackLen>offset)
			{
				//有小数点位数
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
				//解总股本
				int tval = *(int*)(PackBody+offset);
				offset += 4;
				CString LowStr=_T("");LowStr.Format(L"%d",tval);
				m_ListCtl.SetItemText(0,6,LowStr);
			}
			if(PackLen>offset)
			{
				//解涨停价
				int HPrice = *(int*)(PackBody+offset);
				offset += 4;
				CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
				m_ListCtl.SetItemText(0,7,HPriceStr);
			}
			if(PackLen>offset)
			{
				//解最低价
				int Lowprice = *(int*)(PackBody+offset);
				offset += 4;
				CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
				m_ListCtl.SetItemText(0,8,LowpriceStr);
			}
		}break;
	case 2201:
		{
			short RecdNum=0;
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"昨收盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"信息地雷",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"五星评级",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//解昨收盘         int
				int tval = *(int*)(PackBody+offset);
				offset += 4;
				CString LowStr=_T("");LowStr.Format(L"%d",tval);
				m_ListCtl.SetItemText(0,1,LowStr);
			}
			if(PackLen>offset)
			{
				//解成交量         int
				int HPrice = *(int*)(PackBody+offset);
				offset += 4;
				CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
				m_ListCtl.SetItemText(0,2,HPriceStr);
			}
			if(PackLen>offset)
			{
				//解成交额         int
				int Lowprice = *(int*)(PackBody+offset);
				offset += 4;
				CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
				m_ListCtl.SetItemText(0,3,LowpriceStr);
			}
			if(PackLen>offset)
			{
				//信息地雷       byte    1有 0无
				unsigned char Msg = *(PackBody+offset);
				offset += 1;
				int Msgval = Msg;
				CString MsgStr=_T("");MsgStr.Format(L"%d",Msgval);
				m_ListCtl.SetItemText(0,4,MsgStr);
			}
			if(PackLen>offset)
			{
				//五星评级       byte    0-5数值
				unsigned char star = *(PackBody+offset);
				offset += 1;
				int starval = star;
				CString starStr=_T("");starStr.Format(L"%d",starval);
				m_ListCtl.SetItemText(0,5,starStr);
			}
			if(PackLen>offset)
			{
				//数据位置       int         最后一根分时数据需重传
				int datapos = *(int*)(PackBody+offset);
				offset += 4;
				CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
				m_ListCtl.SetItemText(0,6,dataposStr);
			}
			if(PackLen>offset)
			{
				//记录数         short
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
					//解时间           int
					int timeval = *(int*)(PackBody+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    m_ListCtl.SetItemText(j,8,timevalStr);
					if(PackLen>offset)
					{
						//解最新价         int
					    int NewPrice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString NewPriceStr=_T("");NewPriceStr.Format(L"%d",NewPrice);
				        m_ListCtl.SetItemText(j,9,NewPriceStr);
					}
					if(PackLen>offset)
					{
						//解成交量         int
					    int CJVal = *(int*)(PackBody+offset);
				        offset += 4;
				        CString CJValStr=_T("");CJValStr.Format(L"%d",CJVal);
				        m_ListCtl.SetItemText(j,10,CJValStr);
					}
					if(PackLen>offset)
					{
						//解均价           int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"返回个数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"收盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交额",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
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
					//解日期           int
					int timeval = *(int*)(PackBody+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    m_ListCtl.SetItemText(j,2,timevalStr);
					if(PackLen>offset)
					{
						//解开盘价         int
					    int OPrice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString OPriceStr=_T("");OPriceStr.Format(L"%d",OPrice);
				        m_ListCtl.SetItemText(j,3,OPriceStr);
					}
					if(PackLen>offset)
					{
						//解最高价         int
					    int mhVal = *(int*)(PackBody+offset);
				        offset += 4;
				        CString mhValStr=_T("");mhValStr.Format(L"%d",mhVal);
				        m_ListCtl.SetItemText(j,4,mhValStr);
					}
					if(PackLen>offset)
					{
						//解最低价         int
					    int mdprice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString mdpriceStr=_T("");mdpriceStr.Format(L"%d",mdprice);
				        m_ListCtl.SetItemText(j,5,mdpriceStr);
					}
					if(PackLen>offset)
					{
						//解收盘价         int
					    int Sprice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString SpriceStr=_T("");SpriceStr.Format(L"%d",Sprice);
				        m_ListCtl.SetItemText(j,6,SpriceStr);
					}
					if(PackLen>offset)
					{
						//解成交量         int
					    int CJValue = *(int*)(PackBody+offset);
				        offset += 4;
				        CString CJValueStr=_T("");CJValueStr.Format(L"%d",CJValue);
				        m_ListCtl.SetItemText(j,7,CJValueStr);
					}
					if(PackLen>offset)
					{
						//解成交额         int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"昨收盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"流通股本",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"委比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"上证指数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"上证指数昨收",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"深证指数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"深证数昨收",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"内盘成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"现手",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//最新价         int
				int newprice = *(int*)(PackBody+offset);
				offset += 4;
				CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
				m_ListCtl.SetItemText(0,1,newpriceStr);
			}
			if(PackLen>offset)
			{
				//成交量         int
				int cjval = *(int*)(PackBody+offset);
				offset += 4;
				CString cjvalStr=_T("");cjvalStr.Format(L"%d",cjval);
				m_ListCtl.SetItemText(0,2,cjvalStr);
			}
			if(PackLen>offset)
			{
				//成交额         int
				int cjmoney = *(int*)(PackBody+offset);
				offset += 4;
				CString cjmoneyStr=_T("");cjmoneyStr.Format(L"%d",cjmoney);
				m_ListCtl.SetItemText(0,3,cjmoneyStr);
			}
			if(PackLen>offset)
			{
				//最高价         int
				int hprice = *(int*)(PackBody+offset);
				offset += 4;
				CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
				m_ListCtl.SetItemText(0,4,hpriceStr);
			}
			if(PackLen>offset)
			{
				//最低价         int
				int lprice = *(int*)(PackBody+offset);
				offset += 4;
				CString lpriceStr=_T("");lpriceStr.Format(L"%d",lprice);
				m_ListCtl.SetItemText(0,5,lpriceStr);
			}
			if(PackLen>offset)
			{
				//开盘价         int
				int oprice = *(int*)(PackBody+offset);
				offset += 4;
				CString opriceStr=_T("");opriceStr.Format(L"%d",oprice);
				m_ListCtl.SetItemText(0,6,opriceStr);
			}
			if(PackLen>offset)
			{
				//昨收盘         int
				int sprice = *(int*)(PackBody+offset);
				offset += 4;
				CString spriceStr=_T("");spriceStr.Format(L"%d",sprice);
				m_ListCtl.SetItemText(0,7,spriceStr);
			}
			if(PackLen>offset)
			{
				//流通股本       int
				int lgval = *(int*)(PackBody+offset);
				offset += 4;
				CString lgvalStr=_T("");lgvalStr.Format(L"%d",lgval);
				m_ListCtl.SetItemText(0,8,lgvalStr);
			}
			if(PackLen>offset)
			{
				//委比		int×100
				int wb = *(int*)(PackBody+offset);
				offset += 4;
				CString wbStr=_T("");wbStr.Format(L"%d",wb);
				m_ListCtl.SetItemText(0,9,wbStr);
			}
			if(PackLen>offset)
			{
				//量比		int×10000
				int lb = *(int*)(PackBody+offset);
				offset += 4;
				CString lbStr=_T("");lbStr.Format(L"%d",lb);
				m_ListCtl.SetItemText(0,10,lbStr);
			}
			if(PackLen>offset)
			{
				//上证指数	int
				int sindex = *(int*)(PackBody+offset);
				offset += 4;
				CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
				m_ListCtl.SetItemText(0,11,sindexStr);
			}
			if(PackLen>offset)
			{
				//上证指数昨收	int
				int indexspri = *(int*)(PackBody+offset);
				offset += 4;
				CString indexspriStr=_T("");indexspriStr.Format(L"%d",indexspri);
				m_ListCtl.SetItemText(0,12,indexspriStr);
			}
			if(PackLen>offset)
			{
				//深证指数	int
				int sindex = *(int*)(PackBody+offset);
				offset += 4;
				CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
				m_ListCtl.SetItemText(0,13,sindexStr);
			}
			if(PackLen>offset)
			{
				//深证数昨收	int
				int sindexsprice = *(int*)(PackBody+offset);
				offset += 4;
				CString sindexspriceStr=_T("");sindexspriceStr.Format(L"%d",sindexsprice);
				m_ListCtl.SetItemText(0,14,sindexspriceStr);
			}
			if(PackLen>offset)
			{
				//内盘成交量	int	//对指数没有意义
				int incjval = *(int*)(PackBody+offset);
				offset += 4;
				CString incjvalStr=_T("");incjvalStr.Format(L"%d",incjval);
				m_ListCtl.SetItemText(0,15,incjvalStr);
			}
			if(PackLen>offset)
			{
				//现手		int	//对指数没有意义
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"昨收盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"买卖价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"买卖量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//昨收盘         int
				int zsprice = *(int*)(PackBody+offset);
				offset += 4;
				CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				m_ListCtl.SetItemText(0,1,zspriceStr);
			}
			if(PackLen>offset)
			{
				//记录数         short       包含买卖盘，五档行情传10，三档行情传6
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
				//买卖价         int
				if(PackLen>offset)
				{
					int bsprice = *(int*)(PackBody+offset);
				    offset += 4;
				    CString bspriceStr=_T("");bspriceStr.Format(L"%d",bsprice);
				    m_ListCtl.SetItemText(j,3,bspriceStr);
				}
				//买卖量         int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"昨收盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"成交价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//昨收盘         int
				int zsprice = *(int*)(PackBody+offset);
				offset += 4;
				CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				m_ListCtl.SetItemText(0,1,zspriceStr);
			}
			if(PackLen>offset)
			{
				//数据位置       int	下面返回的第一条记录记录号
				int datapos = *(int*)(PackBody+offset);
				offset += 4;
				CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
				m_ListCtl.SetItemText(0,2,dataposStr);
			}
			if(PackLen>offset)
			{
				//数据位置       int	下面返回的第一条记录记录号
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
				//时间           int
				if(PackLen>offset)
				{
					int timeval = *(int*)(PackBody+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    m_ListCtl.SetItemText(j,4,timevalStr);
				}
				//成交价         int
				if(PackLen>offset)
				{
					int cjprice = *(int*)(PackBody+offset);
				    offset += 4;
				    CString cjpriceStr=_T("");cjpriceStr.Format(L"%d",cjprice);
				    m_ListCtl.SetItemText(j,5,cjpriceStr);
				}
				//成交量         int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"上涨家数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"平盘家数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"下跌家数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"加权均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"总流通市值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"总市值",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//上涨家数       short
				short szcount = *(short*)(PackBody+offset);
				offset += 2;
				CString szcountStr=_T("");szcountStr.Format(L"%d",szcount);
				m_ListCtl.SetItemText(0,1,szcountStr);
			}
			if(PackLen>offset)
			{
				//平盘家数       short
				short ppcount = *(short*)(PackBody+offset);
				offset += 2;
				CString ppcountStr=_T("");ppcountStr.Format(L"%d",ppcount);
				m_ListCtl.SetItemText(0,2,ppcountStr);
			}
			if(PackLen>offset)
			{
				//下跌家数       short
				short xdcount = *(short*)(PackBody+offset);
				offset += 2;
				CString xdcountStr=_T("");xdcountStr.Format(L"%d",xdcount);
				m_ListCtl.SetItemText(0,3,xdcountStr);
			}
			if(PackLen>offset)
			{
				//均价		int
				int avprice = *(int*)(PackBody+offset);
				offset += 4;
				CString avpriceStr=_T("");avpriceStr.Format(L"%d",avprice);
				m_ListCtl.SetItemText(0,4,avpriceStr);
			}
			if(PackLen>offset)
			{
				//加权均价	int
				int jqprice = *(int*)(PackBody+offset);
				offset += 4;
				CString jqpriceStr=_T("");jqpriceStr.Format(L"%d",jqprice);
				m_ListCtl.SetItemText(0,5,jqpriceStr);
			}
			if(PackLen>offset)
			{
				//总流通市值	int
				int zltval = *(int*)(PackBody+offset);
				offset += 4;
				CString zltvalStr=_T("");zltvalStr.Format(L"%d",zltval);
				m_ListCtl.SetItemText(0,6,zltvalStr);
			}
			if(PackLen>offset)
			{
				//总市值	int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"最新价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"昨收盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"成交额",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//数据位置       int	下面返回的第一条记录记录号
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
					//名称	String	//名字
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
				    //小数位数   byte
				    unsigned char pointval = *(PackBody+offset);
				    offset += 1;
					int pointvalue = pointval;
				    CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				    m_ListCtl.SetItemText(j,3,pointvalStr);
			    }
				if(PackLen>offset)
			    {
				    //最新价格	int
				    int newprice = *(int*)(PackBody+offset);
				    offset += 4;
				    CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
				    m_ListCtl.SetItemText(j,4,newpriceStr);
			    }
				if(PackLen>offset)
			    {
				    //昨收盘	int
				    int zsprice = *(int*)(PackBody+offset);
				    offset += 4;
				    CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
				    m_ListCtl.SetItemText(j,5,zspriceStr);
			    }
				if(PackLen>offset)
			    {
				    //成交额	int	//值为0就不显示，成交额为万，客户端根据屏幕大小显示为万或者带0～2为小数的亿
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"上期净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"累计净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"截止日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"总记录数",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
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
					//代码           String
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
					//名称	String	//名字
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
				    //小数位数   byte
				    unsigned char pointval = *(PackBody+offset);
				    offset += 1;
					int pointvalue = pointval;
				    CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				    m_ListCtl.SetItemText(j,4,pointvalStr);
			    }
				if(PackLen>offset)
			    {
				    //净值           int
				    int jzvalue = *(int*)(PackBody+offset);
				    offset += 4;
				    CString jzvalueStr=_T("");jzvalueStr.Format(L"%d",jzvalue);
				    m_ListCtl.SetItemText(j,5,jzvalueStr);
			    }
				if(PackLen>offset)
			    {
				    //上期净值       int
				    int sqjz = *(int*)(PackBody+offset);
				    offset += 4;
				    CString sqjzStr=_T("");sqjzStr.Format(L"%d",sqjz);
				    m_ListCtl.SetItemText(j,6,sqjzStr);
			    }
				if(PackLen>offset)
			    {
				    //累计净值       int
				    int ljjz = *(int*)(PackBody+offset);
				    offset += 4;
				    CString ljjzStr=_T("");ljjzStr.Format(L"%d",ljjz);
				    m_ListCtl.SetItemText(j,7,ljjzStr);
			    }
				if(PackLen>offset)
			    {
				    //截止日期       int
				    int enddate = *(int*)(PackBody+offset);
				    offset += 4;
				    CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
				    m_ListCtl.SetItemText(j,8,enddateStr);
			    }
			}
			if(PackLen>offset)
			{
				//总记录数       short
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"万份收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"7日年化收益率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"上期万份收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"上期年化收益率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"截止日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"总记录数",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
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
					//代码           String
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
					//名称	String	//名字
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
				    //小数位数   byte
				    unsigned char pointval = *(PackBody+offset);
				    offset += 1;
					int pointvalue = pointval;
				    CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
				    m_ListCtl.SetItemText(j,4,pointvalStr);
			    }
				if(PackLen>offset)
			    {
				    //万份收益       int
				    int wfvalue = *(int*)(PackBody+offset);
				    offset += 4;
				    CString wfvalueStr=_T("");wfvalueStr.Format(L"%d",wfvalue);
				    m_ListCtl.SetItemText(j,5,wfvalueStr);
			    }
				if(PackLen>offset)
			    {
				    //7日年化收益率  int
				    int Sevenpoint = *(int*)(PackBody+offset);
				    offset += 4;
				    CString SevenpointStr=_T("");SevenpointStr.Format(L"%d",Sevenpoint);
				    m_ListCtl.SetItemText(j,6,SevenpointStr);
			    }
				if(PackLen>offset)
			    {
				    //上期万份收益   int
				    int sqwfsy = *(int*)(PackBody+offset);
				    offset += 4;
				    CString sqwfsyStr=_T("");sqwfsyStr.Format(L"%d",sqwfsy);
				    m_ListCtl.SetItemText(j,7,sqwfsyStr);
			    }
				if(PackLen>offset)
			    {
				    //上期年化收益率 int
				    int sqnhsy = *(int*)(PackBody+offset);
				    offset += 4;
				    CString sqnhsyStr=_T("");sqnhsyStr.Format(L"%d",sqnhsy);
				    m_ListCtl.SetItemText(j,8,sqnhsyStr);
			    }
				if(PackLen>offset)
			    {
				    //截止日期       int
				    int enddate = *(int*)(PackBody+offset);
				    offset += 4;
				    CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
				    m_ListCtl.SetItemText(j,9,enddateStr);
			    }
			}
			if(PackLen>offset)
			{
				//总记录数       short
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//代码           String
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
				//名称	String	//名字
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
				//类型           byte
				unsigned char tYPE = *(PackBody+offset);
				offset += 1;
			    int tYPEvalue = tYPE;
				CString tYPEStr=_T("");tYPEStr.Format(L"%d",tYPEvalue);
				m_ListCtl.SetItemText(0,3,tYPEStr);
			}
			if(PackLen>offset)
			{
				//小数位数   byte
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"累计净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
					    //日期           int
					    int datetime = *(int*)(PackBody+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //净值           int
					    int jzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString jzvalStr=_T("");jzvalStr.Format(L"%d",jzval);
					    m_ListCtl.SetItemText(j,3,jzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //累计净值       int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"股票代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"上期持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"本期持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"占基金%",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"占流通股本%",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
				        //代码           String
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
				        //名称	String	//名字
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
					    //上期持股       int
					    int sqcgval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString sqcgvalStr=_T("");sqcgvalStr.Format(L"%d",sqcgval);
					    m_ListCtl.SetItemText(j,4,sqcgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //本期持股       int
					    int bqcgval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString bqcgvalStr=_T("");bqcgvalStr.Format(L"%d",bqcgval);
					    m_ListCtl.SetItemText(j,5,bqcgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //占基金%        int       保留两位小数，×100转成整数
					    int zjjval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString zjjvalStr=_T("");zjjvalStr.Format(L"%d",zjjval);
					    m_ListCtl.SetItemText(j,6,zjjvalStr);
				    }
					if(PackLen>offset)
				    {
					    //占流通股本%    int       保留两位小数，×100转成整数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"截止日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"期初总份额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"本期总申购",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"本期总赎回",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
					    //截止日期       int       格式如“20080331”，最新的排在最前
					    int datetime = *(int*)(PackBody+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //期初总份额     int       单位：万
					    int qczgbval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
					    m_ListCtl.SetItemText(j,3,qczgbvalStr);
				    }
					if(PackLen>offset)
				    {
					    //本期总申购     int       单位：万
					    int bqzsgval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
					    m_ListCtl.SetItemText(j,4,bqzsgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //本期总赎回     int       单位：万
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"截止日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"持有人户数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"户均份额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"机构持有份额(万)",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"个人持有份额(万)",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
					    //截止日期       int       格式如“20080331”，最新的排在最前
					    int datetime = *(int*)(PackBody+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //持有人户数     int
					    int qczgbval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
					    m_ListCtl.SetItemText(j,3,qczgbvalStr);
				    }
					if(PackLen>offset)
				    {
					    //户均份额       int
					    int bqzsgval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
					    m_ListCtl.SetItemText(j,4,bqzsgvalStr);
				    }
					if(PackLen>offset)
				    {
					    //机构持有份额   int       单位：万
					    int bqzshval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString bqzshvalStr=_T("");bqzshvalStr.Format(L"%d",bqzshval);
					    m_ListCtl.SetItemText(j,5,bqzshvalStr);
				    }
					if(PackLen>offset)
				    {
					    //个人持有份额   int       单位：万
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"概况信息",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"累计净值增长率",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
					    //日期           int
					    int datetime = *(int*)(PackBody+offset);
					    offset += 4;
					    CString DateStr=_T("");DateStr.Format(L"%d",datetime);
					    m_ListCtl.SetItemText(j,2,DateStr);
				    }
					if(PackLen>offset)
				    {
					    //累计净值增长率     int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"标题",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
					    //标题           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"标题",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//标题           String
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
				//内容           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			if(PackLen>offset)
			{
				//记录数         short
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
					    //代码           String
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
					    //名称           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"标题",LVCFMT_LEFT,100);
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
				//记录数         short
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
					    //名称	 String
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

			//总记录数       short
			short tolNum = *(short*)(PackBody+offset);
			offset += 2;
			CString tolStr = _T("");tolStr.Format(L"%d",tolNum);

			//记录数		short
			short RecdNum=*(short*)(PackBody+offset);
			offset += 2;
			CString RecdNumStr=_T("");RecdNumStr.Format(L"%d",RecdNum);
			if(Fundint!=5)
			{  
			    m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			    m_ListCtl.InsertColumn(1,L"总记录数",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(5,L"当期日期",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(6,L"最新净值",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(7,L"累计净值",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(8,L"上期日期",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(9,L"上期净值",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(10,L"上期累计",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(11,L"日增长率",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(12,L"周增长率",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(13,L"月增长率",LVCFMT_LEFT,100);
		 	    m_ListCtl.InsertColumn(14,L"年增长率",LVCFMT_LEFT,100);
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
					    //代码           String
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
					    //名称           String
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
					    //当期日期       int
					    int TodayDate = *(int*)(PackBody+offset);
					    offset += 4;
					    CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
					    m_ListCtl.SetItemText(j,5,TodayDateStr);
				    }
					if(PackLen>offset)
				    {
					    //最新净值       int*10000
					    int zxjzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
					    m_ListCtl.SetItemText(j,6,zxjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //累计净值       int*10000 
					    int ljjzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
					    m_ListCtl.SetItemText(j,7,ljjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期日期       int  
					    int sqdateval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
					    m_ListCtl.SetItemText(j,8,sqdatevalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期净值       int*10000  
					    int sqval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString sqvalStr=_T("");sqvalStr.Format(L"%d",sqval);
					    m_ListCtl.SetItemText(j,9,sqvalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期累计       int*10000  
					    int sqljval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
					    m_ListCtl.SetItemText(j,10,sqljvalStr);
				    }
					if(PackLen>offset)
				    {
					    //日增长率       int*10000  
					    int rzzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString rzzvalStr=_T("");rzzvalStr.Format(L"%d",rzzval);
					    m_ListCtl.SetItemText(j,11,rzzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //周增长率       int*10000  
					    int zzzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString zzzvalStr=_T("");zzzvalStr.Format(L"%d",zzzval);
					    m_ListCtl.SetItemText(j,12,zzzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //月增长率       int*10000  
					    int mzzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString mzzvalStr=_T("");mzzvalStr.Format(L"%d",mzzval);
					    m_ListCtl.SetItemText(j,13,mzzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //周增长率       int*10000  
					    int yzzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString yzzvalStr=_T("");yzzvalStr.Format(L"%d",yzzval);
					    m_ListCtl.SetItemText(j,14,yzzvalStr);
				    }
				}
			}
			if(Fundint==5)
			{  
			    m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			    m_ListCtl.InsertColumn(1,L"总记录数",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(5,L"当期日期",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(6,L"万份收益",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(7,L"七日收益率",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(8,L"上期日期",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(9,L"万份收益",LVCFMT_LEFT,100);
			    m_ListCtl.InsertColumn(10,L"七日收益率",LVCFMT_LEFT,100);
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
					    //代码           String
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
					    //名称           String
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
					    //当期日期       int
					    int TodayDate = *(int*)(PackBody+offset);
					    offset += 4;
					    CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
					    m_ListCtl.SetItemText(j,5,TodayDateStr);
				    }
					if(PackLen>offset)
				    {
					    //万份收益       int*10000
					    int zxjzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
					    m_ListCtl.SetItemText(j,6,zxjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //七日收益率     int*10000 
					    int ljjzval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
					    m_ListCtl.SetItemText(j,7,ljjzvalStr);
				    }
					if(PackLen>offset)
				    {
					    //上期日期       int  
					    int sqdateval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
					    m_ListCtl.SetItemText(j,8,sqdatevalStr);
				    }
					if(PackLen>offset)
				    {
					    //万份收益       int*10000  
					    int sqljval = *(int*)(PackBody+offset);
					    offset += 4;
					    CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
					    m_ListCtl.SetItemText(j,9,sqljvalStr);
				    }
					if(PackLen>offset)
				    {
					    //七日收益率     int*10000  
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"汇率名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"汇率数值",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//日期           int
			int dateval = *(int*)(PackBody+offset);
			offset += 4;
			CString dateStr = _T("");dateStr.Format(L"%d",dateval);
			m_ListCtl.SetItemText(0,1,dateStr);
			if(PackLen>offset)
			{
				//记录数         short
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
					    //汇率名称       String
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
					    //小数位数       byte
					    unsigned char pointpos = *(PackBody+offset);
						offset += 1;
						int pointwz = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointwz);
						m_ListCtl.SetItemText(j,4,pointstr);
				    }
					if(PackLen>offset)
				    {
					    //汇率数值       int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"下一ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"Url地址",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//下一ID         int         下一条的ID，没有下一条返回0

			if(PackLen>offset)
			{
				int nIdval = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
				m_ListCtl.SetItemText(0,1,IdStr);
			}
			//内容           String
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
			// Url地址        String, 格式: ip:port:id,客户端根据平台生成url;该接口仅仅用于广发证券
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"下一ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//ID            int         服务器端最新信息的id，如果没有返回0
			if(PackLen>offset)
			{
				int nIdval = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
				m_ListCtl.SetItemText(0,1,IdStr);
			}
			//内容          String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"该类信息总数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"开始位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"内容",LVCFMT_LEFT,100);
			//该类信息总数  short
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
			//开始位置      short	//下列资讯第一条编号
			if(PackLen>offset)
			{
				short beginpos = *(short*)(PackBody+offset);
				offset += 2;
				CString beginStr = _T("");beginStr.Format(L"%d",beginpos);
				m_ListCtl.SetItemText(0,2,beginStr);
			}
			for(int j=0; j<RecdNum; j++)
			{
			    //内容          String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"分类ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"分类名称",LVCFMT_LEFT,100);
			//该类信息总数  short
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
				//分类ID         int
				if(PackLen>offset)
				{
				    int TypeId = *(int*)(PackBody+offset);
				    offset += 2;
				    CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
				    m_ListCtl.SetItemText(j,2,TypeIdstr);
				}
			    //内容          String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"标题",LVCFMT_LEFT,100);
			//该类信息总数  short
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
			    //标题           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"分类ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"分类名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"是否有子类",LVCFMT_LEFT,100);
			//该类信息总数  short
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
			    //标题           String
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
				//是否有子类     byte    0没有子菜单，1有;
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"标题",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"内容",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//标题           String
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
			//内容           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"新弹出ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"消息内容",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//新弹出ID       int     为 0 时表示没有新消息
			if(PackLen>offset)
			{
				int idlen = *(int*)(PackBody+offset);
				offset += 4;
				CString idstr=_T("");idstr.Format(L"%d",idlen);
				m_ListCtl.SetItemText(0,1,idstr);
			}
			//内容           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"新弹出ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"消息内容",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//新广告ID       int     为 0 时表示没有新广告
			if(PackLen>offset)
			{
				int newsidlen = *(int*)(PackBody+offset);
				offset += 4;
				CString newsidlenstr=_T("");newsidlenstr.Format(L"%d",newsidlen);
				m_ListCtl.SetItemText(0,1,newsidlenstr);
			}
			//广告内容       String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"新图片ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"图片格式",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"图片数据",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//新图片ID       int      为 0 时表示没有新图片
			if(PackLen>offset)
			{
				int piclen = *(int*)(PackBody+offset);
				offset += 4;
				CString picstr=_T("");picstr.Format(L"%d",piclen);
				m_ListCtl.SetItemText(0,1,picstr);
			}
			//图片格式       char
			if(PackLen>offset)
			{
				char pictype = *(PackBody+offset);
				offset+=1;
				int picval = pictype;
				CString picstr = _T("");picstr.Format(L"%d",picval);
				m_ListCtl.SetItemText(0,2,picstr);
			}
		    //	图片数据       byte[]   如果没有新图片，可传回零字节长度的数组, 图片数据需按数组方式序列化(包含2字节标记字节数长度)
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"注册结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//注册结果	byte     0 表示正确，用户将会手机短信验证码；
			unsigned char nResult = *(PackBody+offset);
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
		}break;
	case 2904:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"验证结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"用户id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"right权限位码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"权限日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//验证结果       byte     验证结果：
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
			//[用户id	int]	 	0.验证成功，这个时候有用户id(收到该id后客户端要保存该id，以后登陆时候用），如果验证失败没有该字段
			int userid = *(int*)(PackBody+offset);
			offset += 4;
			CString useridstr = _T("");useridstr.Format(L"%d",userid);
			m_ListCtl.SetItemText(0,2,useridstr);
			//[long		right权限位码]  //验证成功返回的用户默认权限和日期，失败没有返
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
			//[int[]		权限日期，和right从0位到63对应]
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"异常原因",LVCFMT_LEFT,100);
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
		}
	case 2909:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"修改结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
		}
	case 2910:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"修改结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
			//等级		int
			int gradeval = *(int*)(PackBody+offset);
			offset += 4;
			CString gradestr = _T("");gradestr.Format(L"%d",gradeval);
			m_ListCtl.SetItemText(0,2,gradestr);
		}
	case 2911:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"操作结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"股票代码列表",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			
			//股票代码列表： string[] //取股票代码才有该字段，如果第一次取自选股可能没有该字段
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"操作结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"股票代码列表",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//响应：结果		byte	0，//表示请求已经提交，会短信通知用户
			char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			
			//股票代码列表： string[] //取股票代码才有该字段，如果第一次取自选股可能没有该字段
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"滚动资讯类数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"滚动文本",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//滚动资讯类数	short	//表示下面有多少（id，string对数目）
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
				//id		char	//滚动类型，见下面定义  1(分时图滚动） 2(k线滚动）3(行情滚动) 4(首页资讯滚动)，注意相同类型的文本可能有多条
				if(PackLen>offset)
				{
					char idtype = *(PackBody+offset);
					offset += 1;
				    int idval = idtype;
				    CString idstr = _T("");idstr.Format(L"%d",idval);
					m_ListCtl.SetItemText(j,2,idstr);
				}
				//滚动文本	string	//id类型的滚动文本
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"wml文件的内容",LVCFMT_LEFT,100);
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"加权平均委买价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"委买总量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"加权平均委卖价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"委卖总量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"买卖价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"买卖量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//加权平均委买价格	int
			int avgprice = *(int*)(PackLen+offset);
			offset += 4;
			CString avgpricestr = _T("");avgpricestr.Format(L"%d",avgprice);
			m_ListCtl.SetItemText(0,1,avgpricestr);
			//委买总量		int
			int tolbuyval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolbuyvalstr = _T("");tolbuyvalstr.Format(L"%d",tolbuyval);
			m_ListCtl.SetItemText(0,2,tolbuyvalstr);
			//加权平均委卖价格	int
			int sellval = *(int*)(PackLen+offset);
			offset += 4;
			CString sellvalstr = _T("");sellvalstr.Format(L"%d",sellval);
			m_ListCtl.SetItemText(0,3,sellvalstr);
			//委卖总量		int
			int tolsellval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolsellvalstr = _T("");tolsellvalstr.Format(L"%d",tolsellval);
			m_ListCtl.SetItemText(0,4,tolsellvalstr);
			//记录数         	short       //6～10的买卖盘
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
				//买卖价         	int
				if(PackLen>offset)
				{
					int buysell = *(int*)(PackBody+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					m_ListCtl.SetItemText(j,6,buysellstr);
				}
				//买卖量         	int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"成交价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//数据位置       int
			int datapos = *(int*)(PackBody+offset);
			offset += 4;
			CString dataposstr = _T("");dataposstr.Format(L"%d",datapos);
			m_ListCtl.SetItemText(0,1,dataposstr);
			//记录数         	short       //6～10的买卖盘
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
				//买卖价         	int
				if(PackLen>offset)
				{
					int datetime = *(int*)(PackBody+offset);
					offset += 4;
					CString datetimestr = _T("");datetimestr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,3,datetimestr);
				}
				//买卖价         	int
				if(PackLen>offset)
				{
					int buysell = *(int*)(PackBody+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					m_ListCtl.SetItemText(j,4,buysellstr);
				}
				//买卖量         	int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"队列数据ID",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"买卖队列类型数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"买卖队列类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"买卖价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"总买（或卖）单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"委托队列数组(股)",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"买卖队列类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"买卖价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"总买（或卖）单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"委托队列数组(股)",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//队列数据ID	int
			if(PackLen>offset)
			{
			int dataid = *(int*)(PackLen+offset);
			offset += 4;
			CString dataidstr = _T("");dataidstr.Format(L"%d",dataid);
			m_ListCtl.SetItemText(0,1,dataidstr);
			}
			if(PackLen>offset)
			{
			//买卖队列类型数	char	    1表示只有一种买或者卖，2表示买卖都有
			char buyselltype = *(PackBody+offset);
			offset += 1;
			int buysellval = buyselltype;
			CString buysellstr = _T("");buysellstr.Format(L"%d",buysellval);
			m_ListCtl.SetItemText(0,2,buysellstr);
			}
			//买卖队列类型	char	    0表示买, 1表示卖
			if(PackLen>offset)
			{
			char buysell = *(PackBody+offset);
			offset += 1;
			int buysellvalue = buysell;
			CString buysellvaluestr = _T("");buysellvaluestr.Format(L"%d",buysellvalue);
			m_ListCtl.SetItemText(0,3,buysellvaluestr);
			}
			//买卖价格		int	 
			if(PackLen>offset)
			{
			int priceval = *(int*)(PackLen+offset);
			offset += 4;
			CString pricevalstr = _T("");pricevalstr.Format(L"%d",priceval);
			m_ListCtl.SetItemText(0,4,pricevalstr);
			}
			//总买（或卖）单数  int
			if(PackLen>offset)
			{
			int tolalval = *(int*)(PackLen+offset);
			offset += 4;
			CString tolalvalstr = _T("");tolalvalstr.Format(L"%d",tolalval);
			m_ListCtl.SetItemText(0,5,tolalvalstr);
			}
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//委托队列数组	int[]	    委托队列数组，该单位是股
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,6,datevalstr);
				}
			}
			//买卖队列类型	char	    0表示买, 1表示卖
			if(PackLen>offset)
			{
			char buysell2 = *(PackBody+offset);
			offset += 1;
			int buysell2value = buysell2;
			CString buysell2valuestr = _T("");buysell2valuestr.Format(L"%d",buysell2value);
			m_ListCtl.SetItemText(0,7,buysell2valuestr);
			}
			//买卖价格		int	 
			if(PackLen>offset)
			{
			int priceval2 = *(int*)(PackLen+offset);
			offset += 4;
			CString priceval2str = _T("");priceval2str.Format(L"%d",priceval2);
			m_ListCtl.SetItemText(0,8,priceval2str);
			}
			//总买（或卖）单数  int
			if(PackLen>offset)
			{
			int tolalval2 = *(int*)(PackLen+offset);
			offset += 4;
			CString tolalval2str = _T("");tolalval2str.Format(L"%d",tolalval2);
			m_ListCtl.SetItemText(0,9,tolalval2str);
			}
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//委托队列数组	int[]	    委托队列数组，该单位是股
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"ddx",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddx			short			显示3位小数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"ddy",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddy			short			显示3位小数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"ddy",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//ddz			int24			显示3位小数
				if(PackLen>offset)
				{
					//m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24自定义格式，用3个char类型的数组
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
				//ddzwidth		char			ddz线的宽度
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"买入单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"卖出单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"买入中单以上比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"买入大单以上比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"买入特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"卖出中单以上比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"卖出大单以上比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"卖出特大单比例",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//买入单数	uint24		//uint24是无符号的24位整数
				if(PackLen>offset)
				{
					//m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24自定义格式，用3个char类型的数组
					char *buybyte = new char[4];
					memset(buybyte,0,4);
					memcpy(buybyte,PackBody+offset,3);
					offset += 3;
					wstring buybytestr = UTF8_2_UTF16(buybyte);
					m_ListCtl.SetItemText(j,3,buybytestr.c_str());
					delete []buybyte;
				}
				//卖出单数	uint24		//uint24是无符号的24位整数
				if(PackLen>offset)
				{
					//m_ListCtl.SetItemText(j,3,ddyvalstr);
					//int24自定义格式，用3个char类型的数组
					char *sellbyte = new char[4];
					memset(sellbyte,0,4);
					memcpy(sellbyte,PackBody+offset,3);
					offset += 3;
					wstring sellbytestr = UTF8_2_UTF16(sellbyte);
					m_ListCtl.SetItemText(j,4,sellbytestr.c_str());
					delete []sellbyte;
				}
				//买入中单以上比例	short	×1000		包含中、大和特大单
				if(PackLen>offset)
				{
					short buybl = *(short*)(PackBody+offset);
					offset += 2;
					CString buyblstr = _T("");buyblstr.Format(L"%d",buybl);
					m_ListCtl.SetItemText(j,5,buyblstr);
				}
				//买入大单以上比例	short	×1000 		包含大和特大单
				if(PackLen>offset)
				{
					short buybig = *(short*)(PackBody+offset);
					offset += 2;
					CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
					m_ListCtl.SetItemText(j,6,buybigstr);
				}
				//买入特大单比例		short	×1000 		特大单
				if(PackLen>offset)
				{
					short buybiggest = *(short*)(PackBody+offset);
					offset += 2;
					CString buybiggeststr = _T("");buybiggeststr.Format(L"%d",buybiggest);
					m_ListCtl.SetItemText(j,7,buybiggeststr);
				}
				//卖出中单以上比例	short	×1000		包含中、大和特大单
				if(PackLen>offset)
				{
					short sellmid = *(short*)(PackBody+offset);
					offset += 2;
					CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
					m_ListCtl.SetItemText(j,8,sellmidstr);
				}
				//卖出大单以上比例	short	×1000 		包含大和特大单
				if(PackLen>offset)
				{
					short sellbig = *(short*)(PackBody+offset);
					offset += 2;
					CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
					m_ListCtl.SetItemText(j,9,sellbigstr);
				}
				//卖出特大单比例		short	×1000 		特大单
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"ddx数值",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//ddx数值	short	×1000	
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"单数差值",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//单数差值	int24		//int24是24位整数	
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"总买",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"总卖",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//总买		int	
				if(PackLen>offset)
				{
					int tolbuy = *(int*)(PackBody+offset);
					offset += 2;
					CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);
					m_ListCtl.SetItemText(j,2,tolbuystr);
				}
				//总卖		int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"买入中单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"买入大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"买入特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"卖出中单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"卖出大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"卖出特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// 买入中单比例		short	×10000	
			short buymid = *(short*)(PackBody+offset);
			offset += 2;
			CString buymidstr = _T("");buymidstr.Format(L"%d",buymid);
			m_ListCtl.SetItemText(0,1,buymidstr);
			// 买入大单比例		short	×10000
			short buybig = *(short*)(PackBody+offset);
			offset += 2;
			CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
			m_ListCtl.SetItemText(0,2,buybigstr);
			// 买入特大单比例		short	×10000	
			short buybigger = *(short*)(PackBody+offset);
			offset += 2;
			CString buybiggerstr = _T("");buybiggerstr.Format(L"%d",buybigger);
			m_ListCtl.SetItemText(0,3,buybiggerstr);
			// 卖出中单比例		short	×10000		
			short sellmid = *(short*)(PackBody+offset);
			offset += 2;
			CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
			m_ListCtl.SetItemText(0,4,sellmidstr);
			// 卖出大单比例		short	×10000
			short sellbig = *(short*)(PackBody+offset);
			offset += 2;
			CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
			m_ListCtl.SetItemText(0,5,sellbigstr);
			// 卖出特大单比例		short	×10000
			short sellbigger = *(short*)(PackBody+offset);
			offset += 2;
			CString sellbiggerstr = _T("");sellbiggerstr.Format(L"%d",sellbigger);
			m_ListCtl.SetItemText(0,6,sellbiggerstr);
		}break;
	case 2928:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"主力资金值",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//日期			int
				if(PackLen>offset)
				{
					int dateval = *(int*)(PackBody+offset);
					offset += 4;
					CString datevalstr = _T("");datevalstr.Format(L"%d",dateval);
					m_ListCtl.SetItemText(j,2,datevalstr);
				}
				//主力资金值		short×10000			显示2位小数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"贡献点数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"涨跌",LVCFMT_LEFT,100);
			//委托队列数组	int[]	    委托队列数组，该单位是股
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
				//代码           String
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
				//贡献点数	  int  *100  
				if(PackLen>offset)
				{
					int gxds = *(short*)(PackBody+offset);
					offset += 4;
					CString gxdsstr = _T("");gxdsstr.Format(L"%d",gxds);
					m_ListCtl.SetItemText(j,4,gxdsstr);
				}
				//涨跌		  short*10000 
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总卖",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"(卖)均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"总买",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"(买)均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"单数差",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"特大买单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"特大卖单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"大买单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"大卖单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"中买单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"中卖单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"小买单",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"小卖单",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总卖		int
			int tolsell = *(int*)(PackBody+offset);
			offset += 4;
			CString tolsellstr = _T("");tolsellstr.Format(L"%d",tolsell);m_ListCtl.SetItemText(0,1,tolsellstr);
			//(卖)均价	int	//价格位数有静态数据中位数确定
			int sellprice = *(int*)(PackBody+offset);
			offset += 4;
			CString sellpricestr = _T("");sellpricestr.Format(L"%d",sellprice);m_ListCtl.SetItemText(0,2,sellpricestr);
			//总买		int
			int tolbuy = *(int*)(PackBody+offset);
			offset += 4;
			CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);m_ListCtl.SetItemText(0,3,tolbuystr);
			//(买)均价	int
			int buyprice = *(int*)(PackBody+offset);
			offset += 4;
			CString buypricestr = _T("");buypricestr.Format(L"%d",buyprice);m_ListCtl.SetItemText(0,4,buypricestr);
			//ddx		short×1000
			short ddxval = *(short*)(PackBody+offset);
			offset += 2;
			CString ddxvalstr = _T("");ddxvalstr.Format(L"%d",ddxval);
			m_ListCtl.SetItemText(0,5,ddxvalstr);
			// 单数差	int
			int datanum = *(int*)(PackBody+offset);
			offset += 4;
			CString datanumstr = _T("");datanumstr.Format(L"%d",datanum);m_ListCtl.SetItemText(0,6,datanumstr);
			//特大买单	int
			int buymid = *(int*)(PackBody+offset);
			offset += 4;
			CString buymidstr = _T("");buymidstr.Format(L"%d",buymid);
			m_ListCtl.SetItemText(0,7,buymidstr);
			// 特大卖单	int
			int buybig = *(int*)(PackBody+offset);
			offset += 4;
			CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
			m_ListCtl.SetItemText(0,8,buybigstr);
			// 大买单	int	
			int buybigger = *(int*)(PackBody+offset);
			offset += 4;
			CString buybiggerstr = _T("");buybiggerstr.Format(L"%d",buybigger);
			m_ListCtl.SetItemText(0,9,buybiggerstr);
			// 大卖单	int		
			int sellmid = *(int*)(PackBody+offset);
			offset += 4;
			CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
			m_ListCtl.SetItemText(0,10,sellmidstr);
			// 中买单	int
			int sellbig = *(int*)(PackBody+offset);
			offset += 4;
			CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
			m_ListCtl.SetItemText(0,11,sellbigstr);
			// 中卖单	int	
			int sellbigger = *(int*)(PackBody+offset);
			offset += 4;
			CString sellbiggerstr = _T("");sellbiggerstr.Format(L"%d",sellbigger);
			m_ListCtl.SetItemText(0,12,sellbiggerstr);
			// 小买单	int
			int smallbuy = *(int*)(PackBody+offset);
			offset += 4;
			CString smallbuystr = _T("");smallbuystr.Format(L"%d",smallbuy);
			m_ListCtl.SetItemText(0,13,smallbuystr);
			// 小卖单	int	
			int smallsell = *(int*)(PackBody+offset);
			offset += 4;
			CString smallsellstr = _T("");smallsellstr.Format(L"%d",smallsell);
			m_ListCtl.SetItemText(0,14,smallsellstr);
		}break;
	case 2931:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"买入中单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"买入大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"买入特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"卖出中单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"卖出大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"卖出特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"大笔买入笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"大笔卖出笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"机构吃货笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"机构吐货笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"相关的量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//买入中单比例		short	×10000
			short tolsell = *(short*)(PackBody+offset);
			offset += 2;
			CString tolsellstr = _T("");tolsellstr.Format(L"%d",tolsell);m_ListCtl.SetItemText(0,1,tolsellstr);
			//买入大单比例		short	×10000
			short sellprice = *(short*)(PackBody+offset);
			offset += 2;
			CString sellpricestr = _T("");sellpricestr.Format(L"%d",sellprice);m_ListCtl.SetItemText(0,2,sellpricestr);
			//买入特大单比例	short	×10000
			short tolbuy = *(short*)(PackBody+offset);
			offset += 2;
			CString tolbuystr = _T("");tolbuystr.Format(L"%d",tolbuy);m_ListCtl.SetItemText(0,3,tolbuystr);
			//卖出中单比例		short	×10000
			short buyprice = *(short*)(PackBody+offset);
			offset += 2;
			CString buypricestr = _T("");buypricestr.Format(L"%d",buyprice);m_ListCtl.SetItemText(0,4,buypricestr);
			//卖出大单比例		short	×10000
			short ddxval = *(short*)(PackBody+offset);
			offset += 2;
			CString ddxvalstr = _T("");ddxvalstr.Format(L"%d",ddxval);
			m_ListCtl.SetItemText(0,5,ddxvalstr);
			// 卖出特大单比例	short	×10000	
			short datanum = *(short*)(PackBody+offset);
			offset += 2;
			CString datanumstr = _T("");datanumstr.Format(L"%d",datanum);m_ListCtl.SetItemText(0,6,datanumstr);
			//大笔买入笔数		short
			short buymid = *(short*)(PackBody+offset);
			offset += 2;
			CString buymidstr = _T("");buymidstr.Format(L"%d",buymid);
			m_ListCtl.SetItemText(0,7,buymidstr);
			// 大笔卖出笔数		short
			short buybig = *(short*)(PackBody+offset);
			offset += 2;
			CString buybigstr = _T("");buybigstr.Format(L"%d",buybig);
			m_ListCtl.SetItemText(0,8,buybigstr);
			//机构吃货笔数		short
			short buybigger = *(short*)(PackBody+offset);
			offset += 2;
			CString buybiggerstr = _T("");buybiggerstr.Format(L"%d",buybigger);
			m_ListCtl.SetItemText(0,9,buybiggerstr);
			//机构吐货笔数		short		
			short sellmid = *(short*)(PackBody+offset);
			offset += 2;
			CString sellmidstr = _T("");sellmidstr.Format(L"%d",sellmid);
			m_ListCtl.SetItemText(0,10,sellmidstr);
			// 位置			short	    返回的数据的第一个记录的序号
			short sellbig = *(short*)(PackBody+offset);
			offset += 2;
			CString sellbigstr = _T("");sellbigstr.Format(L"%d",sellbig);
			m_ListCtl.SetItemText(0,11,sellbigstr);
			// 记录数		short
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
				//类型		char	    //0：大笔买入；1：大笔卖出；2：机构吃货；3：机构吐货
				if(PackLen>offset)
				{
					char type = *(PackBody+offset);
					offset += 1;
					int typeval = type;
					CString typestr = _T("");typestr.Format(L"%d",typeval);m_ListCtl.SetItemText(j,13,typestr);
				}
				//时间		short	    //时间HHMM
				if(PackLen>offset)
				{
					short timedata = *(short*)(PackBody+offset);
			        offset += 2;
			        CString timedatastr = _T("");timedatastr.Format(L"%d",timedata);
			        m_ListCtl.SetItemText(j,14,timedatastr);
				}
				//相关的量		int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"股票代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"相关的量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// 位置			short	    返回的数据的第一个记录的序号
			short position = *(short*)(PackBody+offset);
			offset += 2;
			CString positionstr = _T("");positionstr.Format(L"%d",position);
			m_ListCtl.SetItemText(0,1,positionstr);
			// 记录数		short
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
				//股票代码	string
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
				//类型		char	    //0：大笔买入；1：大笔卖出；2：机构吃货；3：机构吐货
				if(PackLen>offset)
				{
					char type = *(PackBody+offset);
					offset += 1;
					int typeval = type;
					CString typestr = _T("");typestr.Format(L"%d",typeval);m_ListCtl.SetItemText(j,5,typestr);
				}
				//时间		short	    //时间HHMM
				if(PackLen>offset)
				{
					short timedata = *(short*)(PackBody+offset);
			        offset += 2;
			        CString timedatastr = _T("");timedatastr.Format(L"%d",timedata);
			        m_ListCtl.SetItemText(j,6,timedatastr);
				}
				//相关的量		int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"标记tag",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"udd",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"upp",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"ls",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"hs",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"d3",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// 记录数		short
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
				//标记tag	char	//标记，该标记定义见下面说明
				if(PackLen>offset)
				{
					char type = *(PackBody+offset);
					offset += 1;
					int typeval = type;
					CString typestr = _T("");typestr.Format(L"%d",typeval);m_ListCtl.SetItemText(j,2,typestr);
				}
				//udd		int	//价格数值，小数点位数2200中的小数位数定义，下面价格位数和该值相同
				if(PackLen>offset)
				{
					int udd = *(short*)(PackBody+offset);
			        offset += 4;
			        CString uddstr = _T("");uddstr.Format(L"%d",udd);
			        m_ListCtl.SetItemText(j,3,uddstr);
				}
				// upp		int	//价格数值
				if(PackLen>offset)
				{
					int upp = *(short*)(PackBody+offset);
			        offset += 4;
			        CString uppstr = _T("");uppstr.Format(L"%d",upp);
			        m_ListCtl.SetItemText(j,4,uppstr);
				}
				//ls		int	//价格数值
				if(PackLen>offset)
				{
					int ls = *(short*)(PackBody+offset);
					offset += 4;
					CString lsstr = _T("");lsstr.Format(L"%d",ls);
					m_ListCtl.SetItemText(j,5,lsstr);
				}
				//hs		int	//价格数值
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"返回个数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"收盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交额",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				//记录数         short
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
					//解日期           int
					int timeval = *(int*)(PackBody+offset);
				    offset += 4;
				    CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
				    m_ListCtl.SetItemText(j,2,timevalStr);
					if(PackLen>offset)
					{
						//解开盘价         int
					    int OPrice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString OPriceStr=_T("");OPriceStr.Format(L"%d",OPrice);
				        m_ListCtl.SetItemText(j,3,OPriceStr);
					}
					if(PackLen>offset)
					{
						//解最高价         int
					    int mhVal = *(int*)(PackBody+offset);
				        offset += 4;
				        CString mhValStr=_T("");mhValStr.Format(L"%d",mhVal);
				        m_ListCtl.SetItemText(j,4,mhValStr);
					}
					if(PackLen>offset)
					{
						//解最低价         int
					    int mdprice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString mdpriceStr=_T("");mdpriceStr.Format(L"%d",mdprice);
				        m_ListCtl.SetItemText(j,5,mdpriceStr);
					}
					if(PackLen>offset)
					{
						//解收盘价         int
					    int Sprice = *(int*)(PackBody+offset);
				        offset += 4;
				        CString SpriceStr=_T("");SpriceStr.Format(L"%d",Sprice);
				        m_ListCtl.SetItemText(j,6,SpriceStr);
					}
					if(PackLen>offset)
					{
						//解成交量         int
					    int CJValue = *(int*)(PackBody+offset);
				        offset += 4;
				        CString CJValueStr=_T("");CJValueStr.Format(L"%d",CJValue);
				        m_ListCtl.SetItemText(j,7,CJValueStr);
					}
					if(PackLen>offset)
					{
						//解成交额         int
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
			//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
			if(PackLen>offset)
			{
			    char nflag = *(PackBody+offset);
			    offset += 1;
				int nMark = nflag;
				if(nMark == 0)
				{
					//返回应答包的字段个数,map数组里面的个数
			        m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			        m_ListCtl.InsertColumn(1,L"类型",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(2,L"券商数",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(3,L"券商名称",LVCFMT_LEFT,sizeof(L"券商名称")*50);
			        CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
			        m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
					//券商数：	short（下列数目）
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
					m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,50);
			        m_ListCtl.InsertColumn(1,L"类型",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(2,L"请求的券商wap或者电话数目",LVCFMT_LEFT,100);
			        m_ListCtl.InsertColumn(3,L"类型",LVCFMT_LEFT,sizeof(L"委托是否支持活字典")*10);
					m_ListCtl.InsertColumn(4,L"wap/电话提示串",LVCFMT_LEFT,100);
					m_ListCtl.InsertColumn(5,L"wap/电话",LVCFMT_LEFT,150);
					CString Flag = _T("");Flag.Format(L"%d",nMark);
					m_ListCtl.SetItemText(0,0,L"1");
			        m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
					//请求的券商wap或者电话数目：short（下列数目）
					if(PackLen>offset)
					{
					    short WTnum = *(short*)(PackBody+offset);
					    offset += 2;
						CString wtcount = _T("");wtcount.Format(L"%d",WTnum);
					    m_ListCtl.SetItemText(0, 2, wtcount);//设置其它列数据
						//委托地址	string
						if(PackLen>offset)
						{
							for(int q=0;q<WTnum;q++)
							{
								//类型			char	//1是wap，2电话
								char type = *(PackBody+offset);
								offset += 1;
								int typeval = type;CString typevalstr = _T("");typevalstr.Format(L"%d",typeval);
								//wap/电话提示串	string
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
								//wap/电话		string
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"登陆结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"本次会话标识",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"right权限位码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"权限日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"提示串数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"权限提示位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"提示串",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//验证结果       byte     验证结果：
			unsigned char nResult = *(PackBody+offset);
			offset+=1;
			int resultval = nResult;CString resultStr = _T("");resultStr.Format(L"%d",resultval);
			m_ListCtl.SetItemText(0,1,resultStr);
			// 本次会话标识	long     对登陆成功的连接同时会返回8个字节的long会话标识信息，客户端在本次登陆会话中要保存该标识；
			__int64 userid = *(__int64*)(PackBody+offset);
			offset += 8;
			CString useridstr = _T("");useridstr.Format(L"%I64d",userid);
			m_ListCtl.SetItemText(0,2,useridstr);
			
			//[long		right权限位码]  //验证成功返回的用户默认权限和日期，失败没有返
			__int64 qxval = *(__int64*)(PackBody+offset);
			offset += 8;
			char *b = new char[65];
			memset(b,0,65);
			char *qxdata = ok64(qxval,b);
			wstring qxdatastr = UTF8_2_UTF16(qxdata);
			m_ListCtl.SetItemText(0,3,qxdatastr.c_str());
			delete []b;
			//[int[]		权限日期，和right从0位到63对应]
			short sznum = *(short*)(PackBody+offset);
			offset += 2;
			for(int j=0; j<sznum; j++)
			{
				int data = *(int*)(PackBody+offset);
				offset += 4;
				CString datastr = _T("");datastr.Format(L"%d",data);
				m_ListCtl.SetItemText(j,4,datastr);
			}
			//short	  提示串数目
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
				//	byte	  权限提示位，从0开始编号
					if(PackLen>offset)
					{
					unsigned char bytenotic = *(PackBody+offset);
					offset += 1;
					int ddzwidthval = bytenotic;
					CString ddzwidthvalstr = _T("");ddzwidthvalstr.Format(L"%d",ddzwidthval);
					m_ListCtl.SetItemText(n,6,ddzwidthvalstr);
					}
					//Strint 提示串
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"结点数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"类名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"该结点显示方式",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"该类品种",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"该结点类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"子类数目或者类id",LVCFMT_LEFT,100);
			//结点数目	short		//指最近一级的数目
			RecdNum = *(short*)(PackBody+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			int j=0;
			while(PackLen-offset>0)
			{
				//类名称		string
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
				//该结点显示方式	char	//该结点在界面上显示方式，0表示列表方式，1表示在菜单方式	 
				if(PackLen>offset)
				{
					char distype = *(PackBody+offset);
			        offset+=1;
			        int distypeval = distype;CString distypeStr = _T("");distypeStr.Format(L"%d",distypeval);
			        m_ListCtl.SetItemText(j,3,distypeStr);
				}
				//该类品种		char	//该字段仅仅对叶结点有效，为叶结点表示股票类型（见2200接口）；对股指期货定义一个特殊的类型：127，这个类型仅仅用于客户端区分是股指期货类型，内部分类仍然按照股指期货（8）类型处理	 
				if(PackLen>offset)
				{
					char classtype = *(PackBody+offset);
			        offset+=1;
			        int classtypeval = classtype;CString classtypeStr = _T("");classtypeStr.Format(L"%d",classtypeval);
			        m_ListCtl.SetItemText(j,4,classtypeStr);
				}
				//该结点类型		char	//0表示该节点是叶结点，下面的short是该结点的id；1表示该结点是非叶结点，下面的short记录它子结点数目
				if(PackLen>offset)
				{
					char jdtype = *(PackBody+offset);
			        offset+=1;
			        int jdtypeval = jdtype;CString jdtypeStr = _T("");jdtypeStr.Format(L"%d",jdtypeval);
			        m_ListCtl.SetItemText(j,5,jdtypeStr);
				}
				//子类数目或者类id	short
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
			//判断买卖标志，可以获得以下字段
			char bsflag = *(SendBuffer+Datalen-1);
			int nflag = bsflag;
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"价格位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"成交量单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"最新",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"昨收",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(9,L"总手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"最高",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"最低",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"开盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"昨结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"持仓",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"昨日持仓",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"买卖盘数据个数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"买卖价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"买卖数量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总数目	short	//该分类证券总数目
			short tolnum = *(short*)(PackBody+offset);
			offset+=2;
			CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
			m_ListCtl.SetItemText(0,1,tolnumStr);
			// 数目	short	//下列结构的数目
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
				//代码		 string
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
				//名称		 string	
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
				//价格位数	 char
				if(PackLen>offset)
				{
					char pricepos = *(PackBody+offset);
			        offset+=1;
			        int priceposval = pricepos;CString priceposStr = _T("");priceposStr.Format(L"%d",priceposval);
			        m_ListCtl.SetItemText(m,5,priceposStr);
				}
				//成交量单位	 short 
				if(PackLen>offset)
				{
					unsigned short cjlval = *(unsigned short*)(PackBody+offset);
			        offset+=2;
			        CString cjlStr = _T("");cjlStr.Format(L"%d",cjlval);
			        m_ListCtl.SetItemText(m,6,cjlStr);
				}
				//最新		 int 
				if(PackLen>offset)
				{
					int newval = *(int*)(PackBody+offset);
			        offset+=4;
			        CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			        m_ListCtl.SetItemText(m,7,newvalStr);
				}
				//昨收		 int
				if(PackLen>offset)
				{
					int zsval = *(int*)(PackBody+offset);
			        offset+=4;
			        CString zsvalStr = _T("");zsvalStr.Format(L"%d",zsval);
			        m_ListCtl.SetItemText(m,8,zsvalStr);
				}
				//总手		 int	  //成交量
				if(PackLen>offset)
				{
					int tolval = *(int*)(PackBody+offset);
			        offset+=4;
			        CString tolvalStr = _T("");tolvalStr.Format(L"%d",tolval);
			        m_ListCtl.SetItemText(m,9,tolvalStr);
				}
				//最高		 int
				if(PackLen>offset)
				{
					unsigned int highval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			        m_ListCtl.SetItemText(m,10,highvalStr);
				}
				//最低		 int  
				if(PackLen>offset)
				{
					unsigned int lowval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			        m_ListCtl.SetItemText(m,11,lowvalStr);
				}
				//开盘		 int
				if(PackLen>offset)
				{
					unsigned int openval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString openvalStr = _T("");openvalStr.Format(L"%d",openval);
			        m_ListCtl.SetItemText(m,12,openvalStr);
				}
				//成交额	 int
				if(PackLen>offset)
				{
					unsigned int cjval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString cjvalStr = _T("");cjvalStr.Format(L"%d",cjval);
			        m_ListCtl.SetItemText(m,13,cjvalStr);
				}
	            //结算价	 int
				if(PackLen>offset)
				{
					unsigned int settleval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString settlevalStr = _T("");settlevalStr.Format(L"%d",settleval);
			        m_ListCtl.SetItemText(m,14,settlevalStr);
				}
	            //昨结算价	 int	//前结价
				if(PackLen>offset)
				{
					unsigned int zsettleval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString zsettlevalStr = _T("");zsettlevalStr.Format(L"%d",zsettleval);
			        m_ListCtl.SetItemText(m,15,zsettlevalStr);
				}
	            //持仓		 int	//对期货或期指才有意义
				if(PackLen>offset)
				{
					unsigned int holdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString holdvalStr = _T("");holdvalStr.Format(L"%d",holdval);
			        m_ListCtl.SetItemText(m,16,holdvalStr);
				}
	            //昨日持仓	 int	//对期货或期指才有意义
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
				    //买卖盘数据个数   short
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
					    //买卖价格      int
					    if(PackLen>offset)
				        {
					        unsigned int bsprival = *(unsigned int*)(PackBody+offset);
			                offset+=4;
			                CString bsprivalStr = _T("");bsprivalStr.Format(L"%d",bsprival);
			                m_ListCtl.SetItemText(m+f,19,bsprivalStr);
				        }
					    //买卖数量      int  
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"价格位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"成交量单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"昨收",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"涨停",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"跌停",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(9,L"流通盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"总股本",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"融资融券标记",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"交易量单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"证券扩展分类",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//代码		 string
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
			//名称		 string	
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
			//类型		 char	//见2200接口的股票类型
			if(PackLen>offset)
			{
				char type = *(PackBody+offset);
			    offset+=1;
			    int typeval = type;CString typeStr = _T("");typeStr.Format(L"%d",typeval);
			    m_ListCtl.SetItemText(0,3,typeStr);
			}
			//价格位数	 char
			if(PackLen>offset)
			{
				char pricepos = *(PackBody+offset);
			    offset+=1;
			    int priceposval = pricepos;CString priceposStr = _T("");priceposStr.Format(L"%d",priceposval);
			    m_ListCtl.SetItemText(0,4,priceposStr);
			}
			//成交量单位	 short 
			if(PackLen>offset)
			{
				unsigned short cjlval = *(unsigned short*)(PackBody+offset);
			    offset+=2;
			    CString cjlStr = _T("");cjlStr.Format(L"%d",cjlval);
			    m_ListCtl.SetItemText(0,5,cjlStr);
			}
			//昨收		 int
			if(PackLen>offset)
			{
				int zsval = *(int*)(PackBody+offset);
			    offset+=4;
			    CString zsvalStr = _T("");zsvalStr.Format(L"%d",zsval);
			    m_ListCtl.SetItemText(0,6,zsvalStr);
		    }
			//涨停		 int
			if(PackLen>offset)
			{
				unsigned int highval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			    m_ListCtl.SetItemText(0,7,highvalStr);
			}
			//跌停		 int  
			if(PackLen>offset)
			{
				unsigned int lowval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			    m_ListCtl.SetItemText(0,8,lowvalStr);
			}
			//流通盘	 int	//对期货或期指是昨日持仓，单位是手
			if(PackLen>offset)
			{
				unsigned int lypval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString lypvalStr = _T("");lypvalStr.Format(L"%d",lypval);
			    m_ListCtl.SetItemText(0,9,lypvalStr);
			}
			//总股本	 int	//对商品是昨结算价
			if(PackLen>offset)
			{
				unsigned int cjval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString cjvalStr = _T("");cjvalStr.Format(L"%d",cjval);
			    m_ListCtl.SetItemText(0,10,cjvalStr);
			}
			//融资融券标记   char   //1是融资融券标的，0不是  	  2013-9-23
			if(PackLen>offset)
			{
				char rzrqflag = *(PackBody+offset);
				offset += 1;
				int rzrqval = rzrqflag;CString rzrqstr=_T("");rzrqstr.Format(L"%d",rzrqval);
				m_ListCtl.SetItemText(0,11,rzrqstr);
			}
	        //交易量单位     int    //这个主要用于港股的委托使用的交易量，对其它市场该数值和成交量单位相同     2015-8-29
		    if(PackLen>offset)
			{
				unsigned int tradeunit = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString tradeunitStr = _T("");tradeunitStr.Format(L"%d",tradeunit);
			    m_ListCtl.SetItemText(0,12,tradeunitStr);
			}
			//证券扩展分类	 char	//0无效，1基础三板，2创新三板	2016-4-8
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数据标记(tag)",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"最新",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"今开",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"最高",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最低",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"总额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"内盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"现手",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(10,L"均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"持仓",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"增仓",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"买卖盘记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"买卖价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"买卖量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//数据标记(tag)	 char	//结算价、持仓和增仓字段标记：为0没有这3个数据，1有这3个数据 
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//最新		 int
			if(PackLen>offset)
			{
				unsigned int newval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			    m_ListCtl.SetItemText(0,2,newvalStr);
			}
			//今开		 int
			if(PackLen>offset)
			{
				int openval = *(int*)(PackBody+offset);
			    offset+=4;
			    CString openvalStr = _T("");openvalStr.Format(L"%d",openval);
			    m_ListCtl.SetItemText(0,3,openvalStr);
		    }
			//最高		 int
			if(PackLen>offset)
			{
				unsigned int highval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			    m_ListCtl.SetItemText(0,4,highvalStr);
			}
			//最低		 int
			if(PackLen>offset)
			{
				unsigned int lowval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			    m_ListCtl.SetItemText(0,5,lowvalStr);
			}
			//成交量	 int	//也叫总手
			if(PackLen>offset)
			{
				unsigned int cjlval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString cjlvalStr = _T("");cjlvalStr.Format(L"%d",cjlval);
			    m_ListCtl.SetItemText(0,6,cjlvalStr);
			}
			//总额		 int
			if(PackLen>offset)
			{
				unsigned int cjval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString cjvalStr = _T("");cjvalStr.Format(L"%d",cjval);
			    m_ListCtl.SetItemText(0,7,cjvalStr);
			}
			//内盘		 int
			if(PackLen>offset)
			{
				unsigned int innerval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString innervalStr = _T("");innervalStr.Format(L"%d",innerval);
			    m_ListCtl.SetItemText(0,8,innervalStr);
			}
	        //现手		 int
		    if(PackLen>offset)
			{
				unsigned int tradeunit = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString tradeunitStr = _T("");tradeunitStr.Format(L"%d",tradeunit);
			    m_ListCtl.SetItemText(0,9,tradeunitStr);
			}
			//均价		 int
			if(PackLen>offset)
			{
				unsigned int avgpri = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString avgpriStr = _T("");avgpriStr.Format(L"%d",avgpri);
			    m_ListCtl.SetItemText(0,10,avgpriStr);
			}
			//tag为1有下面3个字段	  
	 // 结算价	 int	//对商品、期货或期指类才有意义
			if(TAGval == 1)
			{
				if(PackLen>offset)
			    {
				    unsigned int settlepri = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString settlepriStr = _T("");settlepriStr.Format(L"%d",settlepri);
			        m_ListCtl.SetItemText(0,11,settlepriStr);
			    }
				//持仓	 	 int	//期货或期指类才有意义
				if(PackLen>offset)
			    {
				    unsigned int holdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString holdvalStr = _T("");holdvalStr.Format(L"%d",holdval);
			        m_ListCtl.SetItemText(0,12,holdvalStr);
			    }
				//增仓		 int	//期货或期指类才有意义
				if(PackLen>offset)
			    {
				    unsigned int addholdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString addholdvalStr = _T("");addholdvalStr.Format(L"%d",addholdval);
			        m_ListCtl.SetItemText(0,13,addholdvalStr);
			    }
			}
			//量比		 short
			if(PackLen>offset)
			{
				unsigned short val = *(unsigned short*)(PackBody+offset);
			    offset+=2;
			    CString valStr = _T("");valStr.Format(L"%d",val);
			    m_ListCtl.SetItemText(0,14,valStr);
			}
			//买卖盘记录数   short 
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
				//买卖价         int
				if(PackLen>offset)
			    {
				    unsigned int bspridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bspridvalStr = _T("");bspridvalStr.Format(L"%d",bspridval);
			        m_ListCtl.SetItemText(j,16,bspridvalStr);
			    }
				//买卖量         int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"持仓标记(tag)",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//持仓标记(tag)	char	    //1包含持仓，0不包含持仓
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//数据位置	 int	     第一个分时数据的位置
			if(PackLen>offset)
			{
				unsigned int newval = *(unsigned int*)(PackBody+offset);
			    offset+=4;
			    CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			    m_ListCtl.SetItemText(0,2,newvalStr);
			}
			//数目		 short	     分时数据数目
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
				//时间	 int
				if(PackLen>offset)
			    {
				    unsigned int TIME = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString TIMEStr = _T("");TIMEStr.Format(L"%d",TIME);
			        m_ListCtl.SetItemText(j,4,TIMEStr);
			    }
				//买卖价         int
				if(PackLen>offset)
			    {
				    unsigned int bspridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bspridvalStr = _T("");bspridvalStr.Format(L"%d",bspridval);
			        m_ListCtl.SetItemText(j,5,bspridvalStr);
			    }
				//成交量	 int	     成交量表示是当前成交量，即现手
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,6,bsdvalStr);
			    }
				//持仓量	 int	     总的持仓量，tag为0没有该字段
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"持仓标记(tag)",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"信息地雷数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"五星评级",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//持仓标记(tag)	char	    //1包含持仓，0不包含持仓
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//信息地雷数     char   
			if(PackLen>offset)
			{
				char msgnum = *(PackBody+offset);
			    offset+=1;
			    int msgnumval = msgnum;CString msgnumStr = _T("");msgnumStr.Format(L"%d",msgnumval);
			    m_ListCtl.SetItemText(0,2,msgnumStr);
			}
			//五星评级       char        0-5数值 
			if(PackLen>offset)
			{
				char STAR = *(PackBody+offset);
			    offset+=1;
			    int STARval = STAR;CString STARStr = _T("");STARStr.Format(L"%d",STARval);
			    m_ListCtl.SetItemText(0,3,STARStr);
			}
			//数据位置	 SHORT	     第一个分时数据的位置
			if(PackLen>offset)
			{
				unsigned short newval = *(unsigned short*)(PackBody+offset);
			    offset+=2;
			    CString newvalStr = _T("");newvalStr.Format(L"%d",newval);
			    m_ListCtl.SetItemText(0,4,newvalStr);
			}
			//数目		 short	     分时数据数目
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
				//时间	 int
				if(PackLen>offset)
			    {
				    unsigned int TIME = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString TIMEStr = _T("");TIMEStr.Format(L"%d",TIME);
			        m_ListCtl.SetItemText(j,6,TIMEStr);
			    }
				//买卖价         int
				if(PackLen>offset)
			    {
				    unsigned int bspridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bspridvalStr = _T("");bspridvalStr.Format(L"%d",bspridval);
			        m_ListCtl.SetItemText(j,7,bspridvalStr);
			    }
				//成交量	 int	     成交量表示是当前成交量，即现手
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,8,bsdvalStr);
			    }
				//均价           int 
				if(PackLen>offset)
			    {
				    unsigned int avgval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString avgvalStr = _T("");avgvalStr.Format(L"%d",avgval);
			        m_ListCtl.SetItemText(j,9,avgvalStr);
			    }
				if(TAGval!=0)
				{
				//持仓量	 int	     总的持仓量，tag为0没有该字段
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"股票类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//记录数         short
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
				//代码		 string
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
			    //名称		 string	
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
				//股票类型	  char 
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"持仓标记(tag)",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"收盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//持仓标记(tag)	char	    //1包含持仓，0不包含持仓
			int TAGval = 0;
			if(PackLen>offset)
			{
				char TAG = *(PackBody+offset);
			    offset+=1;
			    TAGval = TAG;CString TAGStr = _T("");TAGStr.Format(L"%d",TAGval);
			    m_ListCtl.SetItemText(0,1,TAGStr);
			}
			//记录数         short
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
				//日期           int
				if(PackLen>offset)
			    {
				    unsigned int TIME = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString TIMEStr = _T("");TIMEStr.Format(L"%d",TIME);
			        m_ListCtl.SetItemText(j,2,TIMEStr);
			    }
				//开盘价         int
				if(PackLen>offset)
			    {
				    unsigned int openpridval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString openpridvalStr = _T("");openpridvalStr.Format(L"%d",openpridval);
			        m_ListCtl.SetItemText(j,3,openpridvalStr);
			    }
				//最高价         int
				if(PackLen>offset)
			    {
				    unsigned int highval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString highvalStr = _T("");highvalStr.Format(L"%d",highval);
			        m_ListCtl.SetItemText(j,4,highvalStr);
			    }
				//最低价         int
				if(PackLen>offset)
			    {
				    unsigned int lowval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString lowvalStr = _T("");lowvalStr.Format(L"%d",lowval);
			        m_ListCtl.SetItemText(j,5,lowvalStr);
			    }
				//收盘价         int 
				if(PackLen>offset)
			    {
				    unsigned int closeval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString closevalStr = _T("");closevalStr.Format(L"%d",closeval);
			        m_ListCtl.SetItemText(j,6,closevalStr);
			    }
				//成交量         int
				if(PackLen>offset)
			    {
				    unsigned int bsdval = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString bsdvalStr = _T("");bsdvalStr.Format(L"%d",bsdval);
			        m_ListCtl.SetItemText(j,7,bsdvalStr);
			    }
				//成交额         int
				if(PackLen>offset)
			    {
				    unsigned int cjmoney = *(unsigned int*)(PackBody+offset);
			        offset+=4;
			        CString cjmoneyStr = _T("");cjmoneyStr.Format(L"%d",cjmoney);
			        m_ListCtl.SetItemText(j,8,cjmoneyStr);
			    }
				//持仓量	 int	     总的持仓量，tag为0没有该字段
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"滚动wml内容",LVCFMT_LEFT,60);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//持仓标记(tag)	char	    //1包含持仓，0不包含持仓
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"新公告id",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"公告内容",LVCFMT_LEFT,60);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//int	新公告id
			if(PackLen>offset)
			{
				int noticeid = *(int*)(PackBody+offset);
				offset += 4;
				CString IdStr = _T("");IdStr.Format(L"%d",noticeid);
				m_ListCtl.SetItemText(0,1,IdStr);
			}
			//String 	公告内容
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"提示信息数目",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"提示信息id",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(3,L"String",LVCFMT_LEFT,60);
			//short	提示信息数目
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
			//int	新公告id
			for(int j=0; j<RecdNum; j++)
			{
			    if(PackLen>offset)
			    {
				    int noticeid = *(int*)(PackBody+offset);
				    offset += 4;
				    CString IdStr = _T("");IdStr.Format(L"%d",noticeid);
				    m_ListCtl.SetItemText(j,2,IdStr);
			    }
			    //String 	公告内容
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"新公告id",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(2,L"公告内容",LVCFMT_LEFT,60);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//String 	公告内容
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
			//int	用户id
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"列表范围",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"列表字段属性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"总记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"股票类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"请求板块指数成分股的id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"成交单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"内盘成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"现手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"涨速",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"委比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"公告数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"市盈率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(26,L"市净率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(27,L"卖一",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(28,L"买一",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(29,L"7日涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(30,L"7日换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(31,L"30日涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(32,L"30日换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(33,L"当日ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(34,L"当日ddy",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(35,L"当日ddz",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(36,L"60日ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(37,L"60日ddy",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(38,L"10日ddx红色的天数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(39,L"10日ddx连续红色数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(40,L"当日资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(41,L"当日资金流出",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(42,L"5日资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(43,L"5日资金流出",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(44,L"5日资金成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(45,L"30日资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(46,L"30日资金流出",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(47,L"30日资金成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(48,L"昨日持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(49,L"昨日结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(50,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(51,L"结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(52,L"机构买单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(53,L"机构卖单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(54,L"机构吃货数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(55,L"机构吐货数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(56,L"机构吃货量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(57,L"机构吐货量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(58,L"该股是否是融资融券标的",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//列表范围       short
			short listrange = 0;
			if(PackLen>offset)
			{
				listrange = *(short*)(PackBody+offset);
				offset += 2;
				CString range = _T("");range.Format(L"%d",listrange);m_ListCtl.SetItemText(0,1,range);
			}
			//列表字段属性	short	该字段定义见后面说明
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
			printf("列表字段属性:%s",tempbyte);
			
			//总记录数	short	服务器端该类型总记录数
			if(PackLen>offset)
			{
				short tolnum = *(short*)(PackBody+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,3,tolnumStr);
			}
			//记录数         short	下列记录数
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
				//代码           String
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
				//名称           String
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
				//小数位数       byte
				if(PackLen>offset)
				{
					unsigned char pointpos = *(PackBody+offset);
					offset += 1;
					int pointval = pointpos;CString pointstr=_T("");pointstr.Format(L"%d",pointval);
					m_ListCtl.SetItemText(j,7,pointstr);
				}
				//股票类型       byte
				if(PackLen>offset)
				{
					unsigned char stocktype = *(PackBody+offset);
					offset += 1;
					int stocktypeval = stocktype;CString stocktypestr=_T("");stocktypestr.Format(L"%d",stocktypeval);
					m_ListCtl.SetItemText(j,8,stocktypestr);
				}
				//昨收价         int
				if(PackLen>offset)
				{
					int zsprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(j,9,zspricestr);
				}
				//开盘价         int
				if(PackLen>offset)
				{
					int openprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(j,10,openpricestr);
				}
				//最新价         int
				if(PackLen>offset)
				{
					int newprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(j,11,newpricestr);
				}
				//最高价         int
				if(PackLen>offset)
				{
					int highprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(j,12,highpricestr);
				}
				//最低价         int
				if(PackLen>offset)
				{
					int lowprice = *(int*)(PackBody+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(j,13,lowpricestr);
				}
				//成交额         int
				if(PackLen>offset)
				{
					int cjmoney = *(int*)(PackBody+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(j,14,cjmoneystr);
				}
				//请求板块指数成分股的id    short
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
					//成交单位	  short
				    if(PackLen>offset)
				    {
					    short cjpos = *(short*)(PackBody+offset);
				        offset += 2;
					    CString cjposstr = _T("");cjposstr.Format(L"%u",cjpos);
					    m_ListCtl.SetItemText(j,16,cjposstr);
				    }
					//成交量         int
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
					//内盘成交量     int       ------1位
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
					//现手           int       ------2位
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
					//量比           short×100------3位
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
					//换手           short×10000----4位
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
					//涨速           short×10000----5位  有正负号
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
					//委比           short×10000----6位  有正负号
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
					//公告数目       byte 0表示无----7位
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
					//市盈率	  int×100  有正负号
					if(PackLen>offset)
				    {
					    int sylval = *(int*)(PackBody+offset);
				        offset += 4;
					    CString sylvalstr = _T("");sylvalstr.Format(L"%d",sylval/100);
					    m_ListCtl.SetItemText(j,25,sylvalstr);
				    }
					//市净率	  int×100  有正负号
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
					//卖一		  int
					if(PackLen>offset)
				    {
					    unsigned int sell = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString sellstr = _T("");sellstr.Format(L"%u",sell);
					    m_ListCtl.SetItemText(j,27,sellstr);
				    }
					//买一		  int
					if(PackLen>offset)
				    {
					    int buy = *(int*)(PackBody+offset);
				        offset += 4;
					    CString buystr = _T("");buystr.Format(L"%u",buy);
					    m_ListCtl.SetItemText(j,28,buystr);
				    }
				}
				//统计字段----------10位
				if(listattr!=0&&listattr&(0x0400))
				{
					//7日涨幅	  int×10000  有正负号
					if(PackLen>offset)
				    {
					    int zindex = *(int*)(PackBody+offset);
				        offset += 4;
						double zindexval = zindex/10000;
					    CString zindexstr = _T("");zindexstr.Format(L"%lf",zindexval);
					    m_ListCtl.SetItemText(j,29,zindexstr);
				    }
					//7日换手	  int×10000
					if(PackLen>offset)
				    {
					    unsigned int sevenval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
						double sevenvalue = sevenval/10000;
					    CString sevenvalstr = _T("");sevenvalstr.Format(L"%lf",sevenvalue);
					    m_ListCtl.SetItemText(j,30,sevenvalstr);
				    }
					//30日涨幅	  int×10000  有正负号
					if(PackLen>offset)
				    {
					    int z30index = *(int*)(PackBody+offset);
				        offset += 4;
						double z30indexval = z30index/10000;
					    CString z30indexstr = _T("");z30indexstr.Format(L"%lf",z30indexval);
					    m_ListCtl.SetItemText(j,31,z30indexstr);
				    }
					//30日换手	  int×10000
					if(PackLen>offset)
				    {
					    unsigned int thirtyval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
						double thirtyvalval = thirtyval/10000;
					    CString thirtyvalstr = _T("");thirtyvalstr.Format(L"%lf",thirtyvalval);
					    m_ListCtl.SetItemText(j,32,thirtyvalstr);
				    }
				}
				//level2统计字段--11位
				if(listattr!=0&&listattr&(0x0800))
				{
					//当日ddx	  short×1000  有正负号
					if(PackLen>offset)
				    {
					    short dddx = *(short*)(PackBody+offset);
				        offset += 2;
						double dddxval = dddx/1000;
					    CString dddxstr = _T("");dddxstr.Format(L"%lf",dddxval);
					    m_ListCtl.SetItemText(j,33,dddxstr);
				    }
					//当日ddy	  short×1000  有正负号
					if(PackLen>offset)
				    {
					    short dddy = *(short*)(PackBody+offset);
				        offset += 2;
						double dddyval = dddy/1000;
					    CString dddystr = _T("");dddystr.Format(L"%lf",dddyval);
					    m_ListCtl.SetItemText(j,34,dddystr);
				    }
					//当日ddz	  int×1000  有正负号
					if(PackLen>offset)
				    {
					    int dddz = *(int*)(PackBody+offset);
				        offset += 4;
						double dddzval = dddz/1000;
					    CString dddzstr = _T("");dddzstr.Format(L"%lf",dddzval);
					    m_ListCtl.SetItemText(j,35,dddzstr);
				    }
					//60日ddx	  int×1000  有正负号
					if(PackLen>offset)
				    {
					    int sixddxval = *(int*)(PackBody+offset);
				        offset += 4;
						double sixddxvalue = sixddxval/1000;
					    CString sixddxvalstr = _T("");sixddxvalstr.Format(L"%lf",sixddxvalue);
					    m_ListCtl.SetItemText(j,36,sixddxvalstr);
				    }
					//60日ddy	  int×1000  有正负号
					if(PackLen>offset)
				    {
					    int sixddyval = *(int*)(PackBody+offset);
				        offset += 4;
						double sixddyvalue = sixddyval/1000;
					    CString sixddyvalstr = _T("");sixddyvalstr.Format(L"%lf",sixddyvalue);
					    m_ListCtl.SetItemText(j,37,sixddyvalstr);
				    }
					//10日ddx红色的天数 char
					if(PackLen>offset)
				    {
					    char ddxredval = *(PackBody+offset);
				        offset += 1;
					    int ddxred = ddxredval;CString ddxredvalstr = _T("");ddxredvalstr.Format(L"%u",ddxred);
					    m_ListCtl.SetItemText(j,38,ddxredvalstr);
				    }
					//10日ddx连续红色数 char
					if(PackLen>offset)
				    {
					    char ddxredcval = *(PackBody+offset);
				        offset += 1;
					    int ddxredc = ddxredcval;CString ddxredcvalstr = _T("");ddxredcvalstr.Format(L"%u",ddxredc);
					    m_ListCtl.SetItemText(j,39,ddxredcvalstr);
				    }
				}
				////level2统计字段--12位, 20111020增加
				if(listattr!=0&&listattr&(0x1000))
				{
					//当日资金流入     int         //资金净额=资金流入-资金流出
					if(PackLen>offset)
				    {
					    unsigned int incap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString incapstr = _T("");incapstr.Format(L"%u",incap);
					    m_ListCtl.SetItemText(j,40,incapstr);
				    }
					//当日资金流出     int 
					if(PackLen>offset)
				    {
					    unsigned int outcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString outcapstr = _T("");outcapstr.Format(L"%u",outcap);
					    m_ListCtl.SetItemText(j,41,outcapstr);
				    }
					//5日资金流入      int
					if(PackLen>offset)
				    {
					    unsigned int fiveincap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString fiveincapstr = _T("");fiveincapstr.Format(L"%u",fiveincap);
					    m_ListCtl.SetItemText(j,42,fiveincapstr);
				    }
					//5日资金流出      int 
					if(PackLen>offset)
				    {
					    unsigned int fiveoutcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString fiveoutcapstr = _T("");fiveoutcapstr.Format(L"%u",fiveoutcap);
					    m_ListCtl.SetItemText(j,43,fiveoutcapstr);
				    }
					//5日资金成交额    int 
					if(PackLen>offset)
				    {
					    unsigned int fivecjcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString fivecjcapstr = _T("");fivecjcapstr.Format(L"%u",fivecjcap);
					    m_ListCtl.SetItemText(j,44,fivecjcapstr);
				    }
					//30日资金流入     int
					if(PackLen>offset)
				    {
					    unsigned int thirtyincap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString thirtyincapstr = _T("");thirtyincapstr.Format(L"%u",thirtyincap);
					    m_ListCtl.SetItemText(j,45,thirtyincapstr);
				    }
					//30日资金流出      int 
					if(PackLen>offset)
				    {
					    unsigned int thirtyoutcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString thirtyoutcapstr = _T("");thirtyoutcapstr.Format(L"%u",thirtyoutcap);
					    m_ListCtl.SetItemText(j,46,thirtyoutcapstr);
				    }
					//30日资金成交额    int 
					if(PackLen>offset)
				    {
					    unsigned int thirtycjcap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString thirtycjcapstr = _T("");thirtycjcapstr.Format(L"%u",thirtycjcap);
					    m_ListCtl.SetItemText(j,47,thirtycjcapstr);
				    }
				}
				//商品类特有数据--13位, 20120307增加
				if(listattr!=0&&listattr&(0x2000))
				{
					//昨日持仓量       int
					if(PackLen>offset)
				    {
					    unsigned int zhold = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString zholdstr = _T("");zholdstr.Format(L"%u",zhold);
					    m_ListCtl.SetItemText(j,48,zholdstr);
				    }
					//昨日结算价       int 
					if(PackLen>offset)
				    {
					    unsigned int zsettle = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString zsettlestr = _T("");zsettlestr.Format(L"%u",zsettle);
					    m_ListCtl.SetItemText(j,49,zsettlestr);
				    }
					//持仓量           int
					if(PackLen>offset)
				    {
					    unsigned int hold = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString holdstr = _T("");holdstr.Format(L"%u",hold);
					    m_ListCtl.SetItemText(j,50,holdstr);
				    }
					//结算价           int 
					if(PackLen>offset)
				    {
					    unsigned int settlecap = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString settlecapstr = _T("");settlecapstr.Format(L"%u",settlecap);
					    m_ListCtl.SetItemText(j,51,settlecapstr);
				    }
				}
				//level2监控数据---14, 20120508
				if(listattr!=0&&listattr&(0x4000))
				{
					//机构买单数       short  //无符号
					if(PackLen>offset)
				    {
					    unsigned short jgval = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString jgvalstr = _T("");jgvalstr.Format(L"%u",jgval);
					    m_ListCtl.SetItemText(j,52,jgvalstr);
				    }
					//机构卖单数       short  //无符号
					if(PackLen>offset)
				    {
					    unsigned short jgvalsell = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString jgvalsellstr = _T("");jgvalsellstr.Format(L"%u",jgvalsell);
					    m_ListCtl.SetItemText(j,53,jgvalsellstr);
				    }
					//机构吃货数       short  //无符号
					if(PackLen>offset)
				    {
					    unsigned short eathold = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString eatholdstr = _T("");eatholdstr.Format(L"%u",eathold);
					    m_ListCtl.SetItemText(j,54,eatholdstr);
				    }
					//机构吐货数       short  //无符号
					if(PackLen>offset)
				    {
					    unsigned short jgtcap = *(unsigned short*)(PackBody+offset);
				        offset += 2;
					    CString jgtcapstr = _T("");jgtcapstr.Format(L"%u",jgtcap);
					    m_ListCtl.SetItemText(j,55,jgtcapstr);
				    }
					//机构吃货量       int
					if(PackLen>offset)
				    {
					    unsigned int eatval = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString eatvalstr = _T("");eatvalstr.Format(L"%u",eatval);
					    m_ListCtl.SetItemText(j,56,eatvalstr);
				    }
					//机构吐货量       int 
					if(PackLen>offset)
				    {
					    unsigned int sellvalue = *(unsigned int*)(PackBody+offset);
				        offset += 4;
					    CString sellvaluestr = _T("");sellvaluestr.Format(L"%u",sellvalue);
					    m_ListCtl.SetItemText(j,57,sellvaluestr);
				    }
				}
				//融资融券标的位-------------15   20130717 
				if(listattr!=0&&listattr&(0x8000))
				{
					//该股是否是融资融券标的 char   1是，0否
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"长度的高位2字节",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"压缩前长度",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"码表crc",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"拼音",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//长度的高位2字节  short	//该2字节和正文长度short合并成int来表示长度，长度属性位置位
			short highzj = *(short*)(PackBody+offset);
			offset += 2;
			CString highzjStr = _T("");highzjStr.Format(L"%d",highzj);
			m_ListCtl.SetItemText(0,1,highzjStr);
			//压缩前长度	    int		//未压缩时数据长度，该长度指压缩部分在未压缩前的数据长度
			int compresslen = *(int*)(PackBody+offset);
			offset += 4;
			CString CompStr = _T("");CompStr.Format(L"%d",compresslen);
			m_ListCtl.SetItemText(0,2,CompStr);
			//码表crc	    int
			int staticCRC = *(int*)(PackBody+offset);
			offset+=4;
			CString codeCRC = _T("");codeCRC.Format(L"%d",codeCRC);
			m_ListCtl.SetItemText(0,3,codeCRC);
			//数目		    short	
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
				//代码		String	不包括市场代码
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
				//名称           String
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
				//拼音		String	//如果是多音字则用‘`’字符分割
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"最新时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//最新时间 	    int
			int newtime = *(int*)(PackBody+offset);
			CString newtimeStr = _T("");newtimeStr.Format(L"%d",newtime);
			m_ListCtl.SetItemText(0,1,newtimeStr);
		}break;
	case 2958:
		{
			unsigned short RecdNum=0;
			CString RecdNumStr=_T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"乘数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"加数",LVCFMT_LEFT,100);
			//数目		    short	
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
				//日期	    int        //YYYYMMDD
				if(PackLen>offset)
				{
					unsigned int datetime = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString DateStr = _T("");DateStr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,2,DateStr);
				}
				//乘数       int*10000  //有正负号数
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,3,csdataStr);
				}
				//加数       int*10000  //有正负号数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"关注度",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总数             short              //20120216改动
			short tolnum = *(short*)(PackBody+offset);
			offset += 2;
			CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
			m_ListCtl.SetItemText(0,1,tolnumStr);
			//位置             short			
			short position = *(short*)(PackBody+offset);
			offset += 2;
			CString posStr = _T("");posStr.Format(L"%d",position);
			m_ListCtl.SetItemText(0,2,posStr);
			//数目		    short	
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
				//代码		String	不包括市场代码
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
				//名称           String
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
				//关注度     int	
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,6,csdataStr);
				}
				//涨幅       int*10000  //有正负号数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"价格小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"关注度排名",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"关注度",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//价格小数位数	    byte
			unsigned char pricepos = *(PackBody+offset);
			offset += 1;
			int priceposval = pricepos;
			CString priceposvalStr = _T("");priceposvalStr.Format(L"%d",priceposval);
			m_ListCtl.SetItemText(0,1,priceposvalStr);
			//关注度排名       short		
			short position = *(short*)(PackBody+offset);
			offset += 2;
			CString posStr = _T("");posStr.Format(L"%d",position);
			m_ListCtl.SetItemText(0,2,posStr);
			//数目		    short	
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
				//日期	    int        //YYYYMMDD
				if(PackLen>offset)
				{
					unsigned int datetime = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString dateStr = _T("");dateStr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,4,dateStr);
				}
				//关注度     int	
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,5,csdataStr);
				}
				//价格       int        // 
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"板块代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"板块名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"价格小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"关注度",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//板块代码         String
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
			//板块名称         String		
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
			//价格小数位数	    byte
			//价格小数位数	    byte
			if(PackLen>offset)
			{
			    unsigned char pricepos = *(PackBody+offset);
			    offset += 1;
			    int priceposval = pricepos;
			    CString priceposvalStr = _T("");priceposvalStr.Format(L"%d",priceposval);
			    m_ListCtl.SetItemText(0,3,priceposvalStr);
			}
			//数目		    short	
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
				//日期	    int        //YYYYMMDD
				if(PackLen>offset)
				{
					unsigned int datetime = *(unsigned int*)(PackBody+offset);
					offset += 4;
					CString dateStr = _T("");dateStr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,5,dateStr);
				}
				//关注度     int	
				if(PackLen>offset)
				{
					int csdata = *(int*)(PackBody+offset);
					offset += 4;
					CString csdataStr = _T("");csdataStr.Format(L"%d",csdata);
					m_ListCtl.SetItemText(j,6,csdataStr);
				}
				//价格       int        // 
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"年",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"月",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"日",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"时",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"分",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"秒",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//short	年
			if(PackLen>offset)
			{
			    short year = *(short*)(PackBody+offset);
			    offset += 2;
			    CString yearStr=_T("");yearStr.Format(L"%d",year);
			    m_ListCtl.SetItemText(0,1,yearStr);
			}
			// char     月
			if(PackLen>offset)
			{
			    char month = *(PackBody+offset);
			    offset += 1;
			    int monthval = month;CString monthStr=_T("");monthStr.Format(L"%d",monthval);
			    m_ListCtl.SetItemText(0,2,monthStr);
			}
			// char     日
			if(PackLen>offset)
			{
			    char day = *(PackBody+offset);
			    offset += 1;
			    int dayval = day;CString dayStr=_T("");dayStr.Format(L"%d",dayval);
			    m_ListCtl.SetItemText(0,3,dayStr);
			}
			//char     时
			if(PackLen>offset)
			{
			    char hour = *(PackBody+offset);
			    offset += 1;
			    int hourval = hour;CString hourStr=_T("");hourStr.Format(L"%d",hourval);
			    m_ListCtl.SetItemText(0,4,hourStr);
			}
			//char     分
			if(PackLen>offset)
			{
			    char minute = *(PackBody+offset);
			    offset += 1;
			    int minuteval = minute;CString minuteStr=_T("");minuteStr.Format(L"%d",minuteval);
			    m_ListCtl.SetItemText(0,5,minuteStr);
			}
			//char     秒	
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"下列记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"股票代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"样本股数量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"收盘价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"样本股价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"成交金额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"平均股价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"总市值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"指数当前样本的总市值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"静态市盈率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"指数级别",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//short    总数目
			short tolnum = *(short*)(PackBody+offset);
			offset += 2;
			CString tolnumStr=_T("");tolnumStr.Format(L"%d",tolnum);
			m_ListCtl.SetItemText(0,1,tolnumStr);
			if(PackLen>offset)
			{
				////short    下列记录数
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
					// String 股票代码
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
					//short  样本股数量
					if(PackLen>offset)
				    {
						short ybnum = *(short*)(PackBody+offset);
						offset += 2;
						CString ybnumstr = _T("");ybnumstr.Format(L"%d",ybnum);
						m_ListCtl.SetItemText(j,4,ybnumstr);
					}
					if(PackLen>offset)
				    {
					    //int    收盘价格，可能是昨收盘价格也可能是今天收市后的价格
					    unsigned int spvalue = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString spvaluestr = _T("");spvaluestr.Format(L"%u",spvalue);
						m_ListCtl.SetItemText(j,5,spvaluestr);
				    }
					if(PackLen>offset)
				    {
					    //int    样本股价
					    unsigned int ybpricevalue = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString ybpricevaluestr = _T("");ybpricevaluestr.Format(L"%u",ybpricevalue);
						m_ListCtl.SetItemText(j,6,ybpricevaluestr);
				    }
					if(PackLen>offset)
				    {
					    //int    成交金额，单位百万元，客户端可以显示带2位小数的亿元
					    unsigned int cjvalue = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
						m_ListCtl.SetItemText(j,6,cjvaluestr);
				    }
					if(PackLen>offset)
				    {
					    //int    平均股价
					    unsigned int avgprice = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString avgpricestr = _T("");avgpricestr.Format(L"%u",avgprice);
						m_ListCtl.SetItemText(j,7,avgpricestr);
				    }
					if(PackLen>offset)
				    {
					    //int    总市值，单位百亿元，客户端可以显示带2位小数的万亿元
					    unsigned int tolmoney = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString tolmoneystr = _T("");tolmoneystr.Format(L"%u",tolmoney);
						m_ListCtl.SetItemText(j,8,tolmoneystr);
				    }
					if(PackLen>offset)
				    {
					    //int    指数当前样本的总市值占上证综指全样本的总市值百分比，2位小数
					    int index = *(int*)(PackBody+offset);
						offset += 4;
						CString indexstr = _T("");indexstr.Format(L"%u",index);
						m_ListCtl.SetItemText(j,9,indexstr);
				    }
					if(PackLen>offset)
				    {
					    //int    静态市盈率
					    int staticdata = *(int*)(PackBody+offset);
						offset += 4;
						CString staticdatastr = _T("");staticdatastr.Format(L"%u",staticdata);
						m_ListCtl.SetItemText(j,10,staticdatastr);
				    }
					if(PackLen>offset)
				    {
					    //short  指数级别,前n位为指数排序数值，最后一位即指数级别信息：1为重点指数,2为全貌指数
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"下列数据数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"上涨和下跌家数比值",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				////short    下列记录数
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
					// char             上涨和下跌家数比值,范围-100~100
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"每股送",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"每股配",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"配股价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"每股红利",LVCFMT_LEFT,100);
			if(PackLen>offset)
			{
				////short    下列记录数
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
					// 日期	    int        //YYYYMMDD
					if(PackLen>offset)
				    {
						unsigned int datetime = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString datestr = _T("");datestr.Format(L"%u",datetime);
						m_ListCtl.SetItemText(j,2,datestr);
					}
					// 每股送     int*10000  //	
					if(PackLen>offset)
				    {
						unsigned int pergu = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString pergustr = _T("");pergustr.Format(L"%u",pergu);
						m_ListCtl.SetItemText(j,3,pergustr);
					}
					// 每股配     int*10000  //
					if(PackLen>offset)
				    {
						unsigned int pergup = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString pergupstr = _T("");pergupstr.Format(L"%u",pergup);
						m_ListCtl.SetItemText(j,4,pergupstr);
					}
					//配股价     int*10000  //
					if(PackLen>offset)
				    {
						unsigned int pgprice = *(unsigned int*)(PackBody+offset);
						offset += 4;
						CString pgpricestr = _T("");pgpricestr.Format(L"%u",pgprice);
						m_ListCtl.SetItemText(j,5,pgpricestr);
					}
					//每股红利   int*10000  //
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"结果",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//结果:                    char
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


// 解包函数
void CYunTestDlg::DealPack(char * & PackData, unsigned int PackLength, unsigned int PackType)
{
		//单个解包，对每一个包进行解包，配置文档比较混淆，一个个的解包比较清晰，解包比较繁琐
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
			short tjnum = 0; //途径数目
			CString RecdNumStr=_T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"运营商名",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"途径数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"方式",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"wap/短信提示串",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"wap/短信码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"指令码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"提示串",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//数目		    short	
			RecdNum = *(unsigned short*)(PackData+offset);
			offset += 2;
			RecdNumStr.Format(L"%d",RecdNum);
			m_ListCtl.SetItemText(0,1,RecdNumStr);
			for(int j=0; j<RecdNum; j++)
			{
				wstring YYSstr;
				//运营商名    String
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
				//途径数目    short   //下列结构的数目
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
					//方式      char    //1表示wap方式，2表示短信方式
				    if(PackLength>offset)
				    {
					    char type = *(PackData+offset);
					    offset += 1;typeval = type;
					    CString typevalStr = _T("");typevalStr.Format(L"%d",typeval);
					    m_ListCtl.SetItemText(n+k,4,typevalStr);
				    }
					if(typeval == 1)
					{
					//wap/短信提示串   String
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
					//wap/短信码       String
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
					//指令码           String
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
					//提示串           String
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"股票代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"股票类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"最新价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"涨幅",LVCFMT_LEFT,100);
			if(PackLength>offset)
			{
				////short    下列记录数
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
					// 股票代码      String
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
					//股票名称	   String
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
					// 股票类型      char
					if(PackLength>offset)
				    {
						char codetype = *(PackData+offset);
						offset += 1;
						int codeval = codetype;
						CString codetypestr = _T("");codetypestr.Format(L"%d",codeval);
						m_ListCtl.SetItemText(j,4,codetypestr);
					}
					//小数位数      char
					if(PackLength>offset)
				    {
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointval = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointval);
						m_ListCtl.SetItemText(j,5,pointstr);
					}
					//最新价格      int
					if(PackLength>offset)
				    {
						int newprice = *(int*)(PackData+offset);
						offset += 4;
						CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
						m_ListCtl.SetItemText(j,6,newpricestr);
					}
					//涨幅          int*10000
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"记录数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"股票代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"最新价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"板块id",LVCFMT_LEFT,100);
			if(PackLength>offset)
			{
				////short    下列记录数
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
					// 股票代码      String
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
					//股票名称	   String
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
					//小数位数      char
					if(PackLength>offset)
				    {
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointval = pointpos;
						CString pointstr = _T("");pointstr.Format(L"%d",pointval);
						m_ListCtl.SetItemText(j,4,pointstr);
					}
					//最新价格      int
					if(PackLength>offset)
				    {
						int newprice = *(int*)(PackData+offset);
						offset += 4;
						CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
						m_ListCtl.SetItemText(j,5,newpricestr);
					}
					//涨幅          int*10000
					if(PackLength>offset)
				    {
						int zfval = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalstr = _T("");zfvalstr.Format(L"%d",zfval);
						m_ListCtl.SetItemText(j,6,zfvalstr);
					}
					//板块id        short	//该板块id,可以用这个id请求对应成分股
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"持仓标记(tag)",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//持仓标记(tag)	char	    //1包含持仓，0不包含持仓
			char holdtag = *(PackData+offset);
			offset += 1;
			int holdval = holdtag;
			CString HoldStr=_T("");HoldStr.Format(L"%d",holdval);
			m_ListCtl.SetItemText(0,1,HoldStr);
			//数据位置	 int	     第一个分时数据的位置
			int dateval = *(int*)(PackData+offset);
			offset += 4;
			CString datevalStr = _T("");datevalStr.Format(L"%d",dateval);
			m_ListCtl.SetItemText(0,2,datevalStr);
			if(PackLength>offset)
			{
				//记录数         short
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
					    //时间	 int	     格式：hhmmss
					    unsigned int datevalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString datevaluestr = _T("");datevaluestr.Format(L"%u",datevalue);
						m_ListCtl.SetItemText(j,4,datevaluestr);
				    }
					if(PackLength>offset)
				    {
					    //价格	 int	     价格最高1位表示该笔交易是否是外盘
					    unsigned int pricevalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString pricevaluestr = _T("");pricevaluestr.Format(L"%u",pricevalue);
						m_ListCtl.SetItemText(j,5,pricevaluestr);
				    }
					if(PackLength>offset)
				    {
					    //成交量	 int	     成交量表示是当前成交量，即现手
					    unsigned int cjvalue = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
						m_ListCtl.SetItemText(j,6,cjvaluestr);
				    }
					if(holdval != 0)
					{
					    if(PackLength>offset)
				        {
					        //持仓量	 int	     总的持仓量，tag为0没有该字段
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"成交价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//数据位置       int
			int datapos = *(int*)(PackData+offset);
			offset += 4;
			CString dataposstr = _T("");dataposstr.Format(L"%d",datapos);
			m_ListCtl.SetItemText(0,1,dataposstr);
			//记录数         	short       //6～10的买卖盘
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
				//买卖价         	int
				if(PackLength>offset)
				{
					int datetime = *(int*)(PackData+offset);
					offset += 4;
					CString datetimestr = _T("");datetimestr.Format(L"%d",datetime);
					m_ListCtl.SetItemText(j,3,datetimestr);
				}
				//买卖价         	int
				if(PackLength>offset)
				{
					int buysell = *(int*)(PackData+offset);
					offset += 4;
					CString buysellstr = _T("");buysellstr.Format(L"%d",buysell);
					m_ListCtl.SetItemText(j,4,buysellstr);
				}
				//买卖量         	int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"标记",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数据",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//标记          char   //0:行情服务器返回的成功标记
                                //1:行情服务器返回的失败标记
                                //2:账户返回的数据
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
				// 数据          char*N	//该数据只有在标记为2的时候才有,表示账户返回的结果
				if(PackLength>offset)
				{
					//数据中通常有个sub_head结构,如果需要服务器填ip字段,该字段就位于sub_head字段后的4个字节,真正数据在sub_head的后面

				}
			}
		}break;
	case 2973:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"属性字段",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"标记tag",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"udd",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"upp",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"ls",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"hs",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"d3",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"板块数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"板块代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"板块名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"小数点位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"最新价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"板块涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"板块id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"当日大单资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"5日大单资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"30日大单资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"当日ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"60日ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"最近10天连续流入天数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"最近10天流入的天数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"7日涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"7日换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"30日涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(26,L"30日换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//属性字段      short	//定义见说明
			short attrval = 0;
			if(PackLength>offset)
			{
			    attrval = *(short*)(PackData+offset);
			    offset += 2;
			    CString attrstr = _T("");attrstr.Format(L"%d",attrval);
			    m_ListCtl.SetItemText(0,1,attrstr);
			}
			//第0位, 买卖决策信息, 下列各个字段意义见接口2933说明
			if(attrval&0x0001)
			{
				if(PackLength>offset)
				{
				    //标记tag	char	//标记
				    char tag = *(PackData+offset);
				    offset += 1; 
					int tagval = tag;
				    CString tagstr = _T("");tagstr.Format(L"%d",tagval);
				    m_ListCtl.SetItemText(0,2,tagstr);
				}
				//udd		int	//价格数值，小数点位数2200中的小数位数定义，下面价格位数和该值相同
				if(PackLength>offset)
				{
					int uddval = *(int*)(PackData+offset);
					offset += 4;
					CString uddstr = _T("");uddstr.Format(L"%d",uddval);
					m_ListCtl.SetItemText(0,3,uddstr);
				}
				//upp		int	//价格数值
				if(PackLength>offset)
				{
					int uppval = *(int*)(PackData+offset);
					offset += 4;
					CString uppstr = _T("");uppstr.Format(L"%d",uppval);
					m_ListCtl.SetItemText(0,4,uppstr);
				}
				//ls		int	//价格数值
				if(PackLength>offset)
				{
					int lsval = *(int*)(PackData+offset);
					offset += 4;
					CString lsstr = _T("");lsstr.Format(L"%d",lsval);
					m_ListCtl.SetItemText(0,5,lsstr);
				}
				// hs		int	//价格数值	   
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
			////第1位,所属板块信息
			if(attrval&0x0002)
			{
				//板块数目     short
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
				      //板块代码     String
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
						//板块名称     String
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
						//小数点位数   char
						if(PackLength>offset)
				        {
							char pointpos = *(PackData+offset);
							offset += 1;
							int pointval = pointpos;
							CString pointstr = _T("");pointstr.Format(L"%d",pointval);
							m_ListCtl.SetItemText(j,11,pointstr);
						}
						//最新价格     int 
						if(PackLength>offset)
				        {
							int newprice = *(int*)(PackData+offset);
							offset += 4;
							CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
							m_ListCtl.SetItemText(j,12,newpricestr);
						}
						//板块涨幅     int*10000 
						if(PackLength>offset)
				        {
							int zfbk = *(int*)(PackData+offset);
							offset += 4;
							CString zfbkstr = _T("");zfbkstr.Format(L"%d",zfbk);
							m_ListCtl.SetItemText(j,13,zfbkstr);
						}
						//板块id       short  //可以利用该id请求板块成分股
						if(PackLength>offset)
				        {
							short bkid = *(short*)(PackData+offset);
							offset += 2;
							CString bkidstr = _T("");bkidstr.Format(L"%d",bkid);
							m_ListCtl.SetItemText(j,14,bkidstr);
						}
						//资金流入     int   //单位:万元
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
			////第2位,个股资金流向信息
			if(attrval&0x0004)
			{
				//当日大单资金流入     int  //单位:万元,      可为负值，20150331
				if(PackLength>offset)
				{
					int largeinput = *(int*)(PackData+offset);
					offset += 4;
					CString largeinputstr = _T("");largeinputstr.Format(L"%d",largeinput);
					m_ListCtl.SetItemText(0,16,largeinputstr);
				}
				//5日大单资金流入      int  //单位:万元,      可为负值，20150331
				if(PackLength>offset)
				{
					int fivelargeinput = *(int*)(PackData+offset);
					offset += 4;
					CString fivelargeinputstr = _T("");fivelargeinputstr.Format(L"%d",fivelargeinput);
					m_ListCtl.SetItemText(0,17,fivelargeinputstr);
				}
				//30日大单资金流入     int  //单位:万元,      可为负值，20150331
				if(PackLength>offset)
				{
					int thirtylargeinput = *(int*)(PackData+offset);
					offset += 4;
					CString thirtylargeinputstr = _T("");thirtylargeinputstr.Format(L"%d",thirtylargeinput);
					m_ListCtl.SetItemText(0,18,thirtylargeinputstr);
				}
				//当日ddx              short*1000 
				if(PackLength>offset)
				{
					short todayddx = *(short*)(PackData+offset);
					offset += 2;
					CString todayddxstr = _T("");todayddxstr.Format(L"%d",todayddx);
					m_ListCtl.SetItemText(0,19,todayddxstr);
				}
				//60日ddx              short*1000
				if(PackLength>offset)
				{
					short sixtyddx = *(short*)(PackData+offset);
					offset += 2;
					CString sixtyddxstr = _T("");sixtyddxstr.Format(L"%d",sixtyddx);
					m_ListCtl.SetItemText(0,20,sixtyddxstr);
				}
				//最近10天连续流入天数 char
				if(PackLength>offset)
				{
					char inputnum = *(PackData+offset);
					offset += 1;
					int inputnumval = inputnum;
					CString inputnumstr = _T("");inputnumstr.Format(L"%d",inputnumval);
					m_ListCtl.SetItemText(0,21,inputnumstr);
				}
				//最近10天流入的天数   char
				if(PackLength>offset)
				{
					char inputday = *(PackData+offset);
					offset += 1;
					int inputdayval = inputday;
					CString inputdaystr = _T("");inputdaystr.Format(L"%d",inputdayval);
					m_ListCtl.SetItemText(0,22,inputdaystr);
				}
			}
			////第3位,阶段统计信息
			if(attrval&0x0008)
			{
				//7日涨幅	  int×10000  有正负号
				if(PackLength>offset)
				{
					int servezf = *(int*)(PackData+offset);
					offset += 4;
					CString servenstr = _T("");servenstr.Format(L"%d",servezf);
					m_ListCtl.SetItemText(0,23,servenstr);
				}
				//7日换手	  int×10000
				if(PackLength>offset)
				{
					int servehs = *(int*)(PackData+offset);
					offset += 4;
					CString servehsstr = _T("");servehsstr.Format(L"%d",servehs);
					m_ListCtl.SetItemText(0,24,servehsstr);
				}
				//30日涨幅	  int×10000  有正负号
				if(PackLength>offset)
				{
					int thirtyzf = *(int*)(PackData+offset);
					offset += 4;
					CString thirtyzfstr = _T("");thirtyzfstr.Format(L"%d",thirtyzf);
					m_ListCtl.SetItemText(0,25,thirtyzfstr);
				}
				//30日换手	  int×10000
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"发布日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"报告期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"上市日期",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"每股收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"每股净资产",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"净资产收益率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"每股经营现金",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"每股公积金",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"每股未分配",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"股东权益比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"净利润同比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"主营收入同比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"销售毛利率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"调整每股净资",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"总资产",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"流动资产",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"固定资产",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"无形资产",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"流动负债",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"长期负债",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"总负债",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"股东权益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"资本公积金",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"经营现金流量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"投资现金流量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(26,L"筹资现金流量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(27,L"现金增加额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(28,L"主营收入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(29,L"主营利润",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(30,L"营业利润",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(31,L"投资收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(32,L"营业外收支",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(33,L"利润总额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(34,L"净利润",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(35,L"未分配利润",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(36,L"总股本",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(37,L"无限售股份合计",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(38,L"A股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(39,L"B股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(40,L"境外上市外资股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(41,L"其他流通股份",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(42,L"限售股份合计",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(43,L"国家持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(44,L"国有法人持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(45,L"境内法人持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(46,L"境内自然人持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(47,L"其他发起人股份",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(48,L"募集法人股份",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(49,L"境外法人持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(50,L"境外自然人持股",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(51,L"优先股或其他",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//int	发布日期	YYYYMMDD
			if(PackLength>offset)
			{
				unsigned int fbdate = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString fbdatestr = _T("");fbdatestr.Format(L"%u",fbdate);
				m_ListCtl.SetItemText(0,1,fbdatestr);
			}
			//int	报告期		YYYYMMDD
			if(PackLength>offset)
			{
				unsigned int reportdate = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString reportdatestr = _T("");reportdatestr.Format(L"%u",reportdate);
				m_ListCtl.SetItemText(0,2,reportdatestr);
			}
			//int	上市日期	YYYYMMDD
			if(PackLength>offset)
			{
				unsigned int ssdate = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ssdatestr = _T("");ssdatestr.Format(L"%u",ssdate);
				m_ListCtl.SetItemText(0,3,ssdatestr);
			}
			//int	每股收益*10000
			if(PackLength>offset)
			{
				int mgsy = *(int*)(PackData+offset);
				offset += 4;
				CString mgsystr = _T("");mgsystr.Format(L"%d",mgsy);
				m_ListCtl.SetItemText(0,4,mgsystr);
			}
			//int	每股净资产*10000
			if(PackLength>offset)
			{
				int mgjzc = *(int*)(PackData+offset);
				offset += 4;
				CString mgjzcstr = _T("");mgjzcstr.Format(L"%d",mgjzc);
				m_ListCtl.SetItemText(0,5,mgjzcstr);
			}
			//int	净资产收益率*10000	
			if(PackLength>offset)
			{
				int jzcsy = *(int*)(PackData+offset);
				offset += 4;
				CString jzcsystr = _T("");jzcsystr.Format(L"%d",jzcsy);
				m_ListCtl.SetItemText(0,6,jzcsystr);
			}
			//int	每股经营现金*10000	
			if(PackLength>offset)
			{
				int mgjyxj = *(int*)(PackData+offset);
				offset += 4;
				CString mgjyxjstr = _T("");mgjyxjstr.Format(L"%d",mgjyxj);
				m_ListCtl.SetItemText(0,7,mgjyxjstr);
			}
			//int	每股公积金*10000	
			if(PackLength>offset)
			{
				int mggjj = *(int*)(PackData+offset);
				offset += 4;
				CString mggjjstr = _T("");mggjjstr.Format(L"%d",mggjj);
				m_ListCtl.SetItemText(0,8,mggjjstr);
			}
			//int	每股未分配*10000	
			if(PackLength>offset)
			{
				int mgwfp = *(int*)(PackData+offset);
				offset += 4;
				CString mgwfpstr = _T("");mgwfpstr.Format(L"%d",mgwfp);
				m_ListCtl.SetItemText(0,9,mgwfpstr);
			}
			//int	股东权益比*10000	
			if(PackLength>offset)
			{
				int gdqyb = *(int*)(PackData+offset);
				offset += 4;
				CString gdqybstr = _T("");gdqybstr.Format(L"%d",gdqyb);
				m_ListCtl.SetItemText(0,10,gdqybstr);
			}
			//int	净利润同比*10000
			if(PackLength>offset)
			{
				int jlrtb = *(int*)(PackData+offset);
				offset += 4;
				CString jlrtbstr = _T("");jlrtbstr.Format(L"%d",jlrtb);
				m_ListCtl.SetItemText(0,11,jlrtbstr);
			}
			//int	主营收入同比*10000
			if(PackLength>offset)
			{
				int zysr = *(int*)(PackData+offset);
				offset += 4;
				CString zysrstr = _T("");zysrstr.Format(L"%d",zysr);
				m_ListCtl.SetItemText(0,12,zysrstr);
			}
			//int	销售毛利率*10000
			if(PackLength>offset)
			{
				int xsmll = *(int*)(PackData+offset);
				offset += 4;
				CString xsmllstr = _T("");xsmllstr.Format(L"%d",xsmll);
				m_ListCtl.SetItemText(0,13,xsmllstr);
			}
			//int	调整每股净资*10000	
			if(PackLength>offset)
			{
				int tzmgjz = *(int*)(PackData+offset);
				offset += 4;
				CString tzmgjzstr = _T("");tzmgjzstr.Format(L"%d",tzmgjz);
				m_ListCtl.SetItemText(0,14,tzmgjzstr);
			}
			//int	总资产  //单位:千元
			if(PackLength>offset)
			{
				int tolmoney = *(int*)(PackData+offset);
				offset += 4;
				CString tolmoneystr = _T("");tolmoneystr.Format(L"%d",tolmoney);
				m_ListCtl.SetItemText(0,15,tolmoneystr);
			}
			//int	流动资产  //单位:千元
			if(PackLength>offset)
			{
				int ldzc = *(int*)(PackData+offset);
				offset += 4;
				CString ldzcstr = _T("");ldzcstr.Format(L"%d",ldzc);
				m_ListCtl.SetItemText(0,16,ldzcstr);
			}
			//int	固定资产  //单位:千元	
			if(PackLength>offset)
			{
				int gdzc = *(int*)(PackData+offset);
				offset += 4;
				CString gdzcstr = _T("");gdzcstr.Format(L"%d",gdzc);
				m_ListCtl.SetItemText(0,17,gdzcstr);
			}
			//int	无形资产  //单位:千元	
			if(PackLength>offset)
			{
				int wxzc = *(int*)(PackData+offset);
				offset += 4;
				CString wxzcstr = _T("");wxzcstr.Format(L"%d",wxzc);
				m_ListCtl.SetItemText(0,18,wxzcstr);
			}
			//int	流动负债  //单位:千元
			if(PackLength>offset)
			{
				int ldfz = *(int*)(PackData+offset);
				offset += 4;
				CString ldfzstr = _T("");ldfzstr.Format(L"%d",ldfz);
				m_ListCtl.SetItemText(0,19,ldfzstr);
			}
			//int	长期负债  //单位:千元
			if(PackLength>offset)
			{
				int cqfz = *(int*)(PackData+offset);
				offset += 4;
				CString cqfzstr = _T("");cqfzstr.Format(L"%d",cqfz);
				m_ListCtl.SetItemText(0,20,cqfzstr);
			}
			//int	总负债  //单位:千元
			if(PackLength>offset)
			{
				int tolfz = *(int*)(PackData+offset);
				offset += 4;
				CString tolfzstr = _T("");tolfzstr.Format(L"%d",tolfz);
				m_ListCtl.SetItemText(0,21,tolfzstr);
			}
			//int	股东权益  //单位:千元
			if(PackLength>offset)
			{
				int gdqy = *(int*)(PackData+offset);
				offset += 4;
				CString gdqystr = _T("");gdqystr.Format(L"%d",gdqy);
				m_ListCtl.SetItemText(0,22,gdqystr);
			}
			//int	资本公积金  //单位:千元
			if(PackLength>offset)
			{
				int zbgjj = *(int*)(PackData+offset);
				offset += 4;
				CString zbgjjstr = _T("");zbgjjstr.Format(L"%d",zbgjj);
				m_ListCtl.SetItemText(0,23,zbgjjstr);
			}
			//int	经营现金流量  //单位:千元
			if(PackLength>offset)
			{
				int jyzjll = *(int*)(PackData+offset);
				offset += 4;
				CString jyzjllstr = _T("");jyzjllstr.Format(L"%d",jyzjll);
				m_ListCtl.SetItemText(0,24,jyzjllstr);
			}
			//int	投资现金流量  //单位:千元
			if(PackLength>offset)
			{
				int tzxjll = *(int*)(PackData+offset);
				offset += 4;
				CString tzxjllstr = _T("");tzxjllstr.Format(L"%d",tzxjll);
				m_ListCtl.SetItemText(0,25,tzxjllstr);
			}
			//int	筹资现金流量  //单位:千元
			if(PackLength>offset)
			{
				int czxjll = *(int*)(PackData+offset);
				offset += 4;
				CString czxjllstr = _T("");czxjllstr.Format(L"%d",czxjll);
				m_ListCtl.SetItemText(0,26,czxjllstr);
			}
			//int	现金增加额  //单位:千元
			if(PackLength>offset)
			{
				int xjzje = *(int*)(PackData+offset);
				offset += 4;
				CString xjzjestr = _T("");xjzjestr.Format(L"%d",xjzje);
				m_ListCtl.SetItemText(0,27,xjzjestr);
			}
			//int	主营收入  //单位:千元
			if(PackLength>offset)
			{
				int zysr = *(int*)(PackData+offset);
				offset += 4;
				CString zysrstr = _T("");zysrstr.Format(L"%d",zysr);
				m_ListCtl.SetItemText(0,28,zysrstr);
			}
			//int	主营利润  //单位:千元
			if(PackLength>offset)
			{
				int zylr = *(int*)(PackData+offset);
				offset += 4;
				CString zylrstr = _T("");zylrstr.Format(L"%d",zylr);
				m_ListCtl.SetItemText(0,29,zylrstr);
			}
			//int	营业利润  //单位:千元
			if(PackLength>offset)
			{
				int yylr = *(int*)(PackData+offset);
				offset += 4;
				CString yylrstr = _T("");yylrstr.Format(L"%d",yylr);
				m_ListCtl.SetItemText(0,30,yylrstr);
			}
			//int	投资收益  //单位:千元
			if(PackLength>offset)
			{
				int tzsy = *(int*)(PackData+offset);
				offset += 4;
				CString tzsystr = _T("");tzsystr.Format(L"%d",tzsy);
				m_ListCtl.SetItemText(0,31,tzsystr);
			}
			//int	营业外收支  //单位:千元
			if(PackLength>offset)
			{
				int yywsr = *(int*)(PackData+offset);
				offset += 4;
				CString yywsrstr = _T("");yywsrstr.Format(L"%d",yywsr);
				m_ListCtl.SetItemText(0,32,yywsrstr);
			}
			//int	利润总额  //单位:千元
			if(PackLength>offset)
			{
				int lrze = *(int*)(PackData+offset);
				offset += 4;
				CString lrzestr = _T("");lrzestr.Format(L"%d",lrze);
				m_ListCtl.SetItemText(0,33,lrzestr);
			}
			//int	净利润  //单位:千元
			if(PackLength>offset)
			{
				int jlr = *(int*)(PackData+offset);
				offset += 4;
				CString jlrstr = _T("");jlrstr.Format(L"%d",jlr);
				m_ListCtl.SetItemText(0,34,jlrstr);
			}
			//int	未分配利润  //单位:千元
			if(PackLength>offset)
			{
				int wfplr = *(int*)(PackData+offset);
				offset += 4;
				CString wfplrstr = _T("");wfplrstr.Format(L"%d",wfplr);
				m_ListCtl.SetItemText(0,35,wfplrstr);
			}
			//int	总股本	//单位:万股
			if(PackLength>offset)
			{
				int zgb = *(int*)(PackData+offset);
				offset += 4;
				CString zgbstr = _T("");zgbstr.Format(L"%d",zgb);
				m_ListCtl.SetItemText(0,36,zgbstr);
			}
			//int	无限售股份合计	//单位:万股
			if(PackLength>offset)
			{
				int wxsgb = *(int*)(PackData+offset);
				offset += 4;
				CString wxsgbstr = _T("");wxsgbstr.Format(L"%d",wxsgb);
				m_ListCtl.SetItemText(0,37,wxsgbstr);
			}
			//int	A股	//单位:万股
			if(PackLength>offset)
			{
				int Ag = *(int*)(PackData+offset);
				offset += 4;
				CString Agstr = _T("");Agstr.Format(L"%d",Ag);
				m_ListCtl.SetItemText(0,38,Agstr);
			}
			//int	B股	//单位:万股
			if(PackLength>offset)
			{
				int Bg = *(int*)(PackData+offset);
				offset += 4;
				CString Bgstr = _T("");Bgstr.Format(L"%d",Bg);
				m_ListCtl.SetItemText(0,39,Bgstr);
			}
			//int	境外上市外资股	//单位:万股
			if(PackLength>offset)
			{
				int jwsswzg = *(int*)(PackData+offset);
				offset += 4;
				CString jwsswzgstr = _T("");jwsswzgstr.Format(L"%d",jwsswzg);
				m_ListCtl.SetItemText(0,40,jwsswzgstr);
			}
			//int	其他流通股份	//单位:万股
			if(PackLength>offset)
			{
				int qtltgf = *(int*)(PackData+offset);
				offset += 4;
				CString qtltgfstr = _T("");qtltgfstr.Format(L"%d",qtltgf);
				m_ListCtl.SetItemText(0,41,qtltgfstr);
			}
			//int	限售股份合计	//单位:万股
			if(PackLength>offset)
			{
				int xsgfhj = *(int*)(PackData+offset);
				offset += 4;
				CString xsgfhjstr = _T("");xsgfhjstr.Format(L"%d",xsgfhj);
				m_ListCtl.SetItemText(0,42,xsgfhjstr);
			}
			//int	国家持股	//单位:万股
			if(PackLength>offset)
			{
				int gjcg = *(int*)(PackData+offset);
				offset += 4;
				CString gjcgstr = _T("");gjcgstr.Format(L"%d",gjcg);
				m_ListCtl.SetItemText(0,43,gjcgstr);
			}
			//int	国有法人持股	//单位:万股
			if(PackLength>offset)
			{
				int gjfrcg = *(int*)(PackData+offset);
				offset += 4;
				CString gjfrcgstr = _T("");gjfrcgstr.Format(L"%d",gjfrcg);
				m_ListCtl.SetItemText(0,44,gjfrcgstr);
			}
			//int	境内法人持股	//单位:万股
			if(PackLength>offset)
			{
				int jlfrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jlfrcgstr = _T("");jlfrcgstr.Format(L"%d",jlfrcg);
				m_ListCtl.SetItemText(0,45,jlfrcgstr);
			}
			//int	境内自然人持股	//单位:万股
			if(PackLength>offset)
			{
				int jlzrrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jlzrrcgstr = _T("");jlzrrcgstr.Format(L"%d",jlzrrcg);
				m_ListCtl.SetItemText(0,46,jlzrrcgstr);
			}
			//int	其他发起人股份	//单位:万股
			if(PackLength>offset)
			{
				int qtfqrgf = *(int*)(PackData+offset);
				offset += 4;
				CString qtfqrgfstr = _T("");qtfqrgfstr.Format(L"%d",qtfqrgf);
				m_ListCtl.SetItemText(0,47,qtfqrgfstr);
			}
			//int	募集法人股份
			if(PackLength>offset)
			{
				int mjfrgf = *(int*)(PackData+offset);
				offset += 4;
				CString mjfrgfstr = _T("");mjfrgfstr.Format(L"%d",mjfrgf);
				m_ListCtl.SetItemText(0,48,mjfrgfstr);
			}
			//int	境外法人持股	//单位:万股
			if(PackLength>offset)
			{
				int jwfrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jwfrcgstr = _T("");jwfrcgstr.Format(L"%d",jwfrcg);
				m_ListCtl.SetItemText(0,49,jwfrcgstr);
			}
			//int	境外自然人持股	//单位:万股
			if(PackLength>offset)
			{
				int jwzrrcg = *(int*)(PackData+offset);
				offset += 4;
				CString jwzrrcgstr = _T("");jwzrrcgstr.Format(L"%d",jwzrrcg);
				m_ListCtl.SetItemText(0,50,jwzrrcgstr);
			}
			//int	优先股或其他	//单位:万股
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"买入中单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"买入大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"买入特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"卖出中单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"卖出大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"卖出特大单比例",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"机构吃货笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"机构吐货笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"大笔买入笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"大笔卖出笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"中买单笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"中卖单笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"小买单笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"小卖单笔数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"相关的量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
		}break;
	case 2977:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"股票代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"相关的量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//位置		short	    返回的数据的第一个记录的序号
			if(PackLength>offset)
			{
				short position = *(short*)(PackData+offset);
				offset += 2;
				CString posstr = _T("");posstr.Format(L"%d",position);
				m_ListCtl.SetItemText(0,1,posstr);
			}
			//位置		short	    返回的数据的第一个记录的序号
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
					//股票代码	string
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
					//股票名称	string
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
					//类型	char	    //类型和2976一致
					if(PackLength>offset)
					{
						char type = *(PackData+offset);
						offset += 1;
						int typeval = type;CString typeSTR = _T("");typeSTR.Format(L"%d",typeval);
						m_ListCtl.SetItemText(j,5,typeSTR);
					}
					//时间	short	    //时间HHMM
					if(PackLength>offset)
					{
						short datetime = *(short*)(PackData+offset);
						offset += 2;
						CString datestr = _T("");datestr.Format(L"%d",datetime);
						m_ListCtl.SetItemText(j,6,datestr);
					}
					//相关的量	int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"字段属性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"总记录数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"下列数据数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"价格位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交量单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"流通股本",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"总股本",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"昨收",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"涨停价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"跌停价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"昨日结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"昨日持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"动态数据时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"开盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"最高",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"最低",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"最新",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"现手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"内盘",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(26,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(27,L"结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(28,L"卖一价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(29,L"买一价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(30,L"卖一量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(31,L"买一量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(32,L"卖二价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(33,L"买二价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(34,L"卖二量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(35,L"买二量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(36,L"卖三价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(37,L"买三价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(38,L"卖三量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(39,L"买三量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(40,L"卖四价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(41,L"买四价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(42,L"卖四量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(43,L"买四量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(44,L"卖五价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(45,L"买五价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(46,L"卖五量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(47,L"买五量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(48,L"融资融券标记",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(49,L"信息地雷时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//字段属性        int   推送返回的字段，见后续说明，和设置对应；
			unsigned int attr = 0;
			if(PackLength>offset)
			{
				attr = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString attrstr = _T("");attrstr.Format(L"%u",attr);
				m_ListCtl.SetItemText(0,1,attrstr);
			}
			//总记录数目      short   对个股列表推送返回的是设置的个数；对商品列表返回的是该id分类总数目
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,2,tolnumstr);
			}
			//位置		short	    返回的数据的第一个记录的序号
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
					//代码		 string，该字段必有，下列字段是否有根据字段属性；
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
					    //股票名称	string
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
					    //类型                 char,1位：类型和价格位数   
					    if(PackLength>offset)
					    {
							char type = *(PackData+offset);
							offset += 1;
							int typeval = type;CString typestr = _T("");typestr.Format(L"%d",typeval);
							m_ListCtl.SetItemText(j,6,typestr);
						}
						//价格位数             char
						if(PackLength>offset)
					    {
							char pricepos = *(PackData+offset);
							offset += 1;
							int priceposval = pricepos;CString priceposstr = _T("");priceposstr.Format(L"%d",priceposval);
							m_ListCtl.SetItemText(j,7,priceposstr);
						}
					}
					//2位：成交量单位
					if(attr&0x0004)
					{
					    //成交量单位           short   
					    if(PackLength>offset)
					    {
							short cjldw = *(short*)(PackData+offset);
							offset += 2;
							CString cjldwstr = _T("");cjldwstr.Format(L"%d",cjldw);
							m_ListCtl.SetItemText(j,8,cjldwstr);
						}
					}
					//3位：流通股本
					if(attr&0x0008)
					{
					    //流通股本             int   
					    if(PackLength>offset)
					    {
							int ltgb = *(int*)(PackData+offset);
							offset += 4;
							CString ltgbstr = _T("");ltgbstr.Format(L"%d",ltgb);
							m_ListCtl.SetItemText(j,9,ltgbstr);
						}
					}
					//4位：总股本
					if(attr&0x0010)
					{
					    //总股本               int 
					    if(PackLength>offset)
					    {
							unsigned int zgb = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zgbstr = _T("");zgbstr.Format(L"%u",zgb);
							m_ListCtl.SetItemText(j,10,zgbstr);
						}
					}
					//5位：昨收
					if(attr&0x0020)
					{
					    //昨收                 int 
					    if(PackLength>offset)
					    {
							unsigned int zsprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
							m_ListCtl.SetItemText(j,11,zspricestr);
						}
					}
					//6位：涨停价格和跌停价格
					if(attr&0x0040)
					{
					    //涨停价格             int
					    if(PackLength>offset)
					    {
							unsigned int highprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
							m_ListCtl.SetItemText(j,12,highpricestr);
						}
						//跌停价格             int
					    if(PackLength>offset)
					    {
							unsigned int lowprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
							m_ListCtl.SetItemText(j,13,lowpricestr);
						}
					}
					//7位：昨日结算价
					if(attr&0x0080)
					{
					    //昨日结算价           int
					    if(PackLength>offset)
					    {
							unsigned int ysettleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString ysettlepricestr = _T("");ysettlepricestr.Format(L"%u",ysettleprice);
							m_ListCtl.SetItemText(j,14,ysettlepricestr);
						}
					}
					//8位：昨日持仓量
					if(attr&0x0100)
					{
					    //昨日持仓量           int 
					    if(PackLength>offset)
					    {
							unsigned int yholdprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString yholdpricestr = _T("");yholdpricestr.Format(L"%u",yholdprice);
							m_ListCtl.SetItemText(j,15,yholdpricestr);
						}
					}
					//9位：动态数据时间，HHMMSS
					if(attr&0x0200)
					{
					    //动态数据时间，       int  HHMMSS 
					    if(PackLength>offset)
					    {
							unsigned int Dynctime = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString Dynctimestr = _T("");Dynctimestr.Format(L"%u",Dynctime);
							m_ListCtl.SetItemText(j,16,Dynctimestr);
						}
					}
					//10位：开盘
					if(attr&0x0200)
					{
					    //开盘                 int
					    if(PackLength>offset)
					    {
							unsigned int openprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
							m_ListCtl.SetItemText(j,17,openpricestr);
						}
					}
					//11位：最高和最低
					if(attr&0x0800)
					{
					    //最高                 int
					    if(PackLength>offset)
					    {
							unsigned int zgprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zgpricestr = _T("");zgpricestr.Format(L"%u",zgprice);
							m_ListCtl.SetItemText(j,18,zgpricestr);
						}
						//最低                 int
					    if(PackLength>offset)
					    {
							unsigned int zdprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString zdpricestr = _T("");zdpricestr.Format(L"%u",zdprice);
							m_ListCtl.SetItemText(j,19,zdpricestr);
						}
					}
					//12位：最新
					if(attr&0x1000)
					{
					    //最新                 int
					    if(PackLength>offset)
					    {
							unsigned int newprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
							m_ListCtl.SetItemText(j,20,newpricestr);
						}
					}
					//13位：均价
					if(attr&0x2000)
					{
					    //均价                 int
					    if(PackLength>offset)
					    {
							unsigned int avgprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString avgpricestr = _T("");avgpricestr.Format(L"%u",avgprice);
							m_ListCtl.SetItemText(j,21,avgpricestr);
						}
					}
					//14位：成交量 
					if(attr&0x4000)
					{
					    //成交量               int
					    if(PackLength>offset)
					    {
							unsigned int cjlval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString cjlvalstr = _T("");cjlvalstr.Format(L"%u",cjlval);
							m_ListCtl.SetItemText(j,22,cjlvalstr);
						}
					}
					//15位：现手 
					if(attr&0x8000)
					{
					    //现手                 int
					    if(PackLength>offset)
					    {
							unsigned int xsval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString xsvalstr = _T("");xsvalstr.Format(L"%u",xsval);
							m_ListCtl.SetItemText(j,23,xsvalstr);
						}
					}
					//16位：成交额 
					if(attr&0x10000)
					{
					    //成交额               int
					    if(PackLength>offset)
					    {
							unsigned int cjeval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString cjevalstr = _T("");cjevalstr.Format(L"%u",cjeval);
							m_ListCtl.SetItemText(j,24,cjevalstr);
						}
					}
					//17位：内盘 
					if(attr&0x20000)
					{
					    //内盘                 int
					    if(PackLength>offset)
					    {
							unsigned int innerval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString innervalstr = _T("");innervalstr.Format(L"%u",innerval);
							m_ListCtl.SetItemText(j,25,innervalstr);
						}
					}
					//18位：持仓量 
					if(attr&0x40000)
					{
					    //持仓量               int
					    if(PackLength>offset)
					    {
							unsigned int holdval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString holdvalstr = _T("");holdvalstr.Format(L"%u",holdval);
							m_ListCtl.SetItemText(j,26,holdvalstr);
						}
					}
					//19位：结算价 
					if(attr&0x80000)
					{
					    //结算价               int
					    if(PackLength>offset)
					    {
							unsigned int settleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString settlepricestr = _T("");settlepricestr.Format(L"%u",settleprice);
							m_ListCtl.SetItemText(j,27,settlepricestr);
						}
					}
					//20位：卖一价和买一价 
					if(attr&0x100000)
					{
					    //卖一价                int
					    if(PackLength>offset)
					    {
							unsigned int sell1price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell1pricestr = _T("");sell1pricestr.Format(L"%u",sell1price);
							m_ListCtl.SetItemText(j,28,sell1pricestr);
						}
						//买一价                int
					    if(PackLength>offset)
					    {
							unsigned int buy1price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy1pricestr = _T("");buy1pricestr.Format(L"%u",buy1price);
							m_ListCtl.SetItemText(j,29,buy1pricestr);
						}
					}
					//21位：卖一量和买一量 
					if(attr&0x200000)
					{
					    //卖一量                int
					    if(PackLength>offset)
					    {
							unsigned int sell1val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell1valstr = _T("");sell1valstr.Format(L"%u",sell1val);
							m_ListCtl.SetItemText(j,30,sell1valstr);
						}
						//买一量                int
					    if(PackLength>offset)
					    {
							unsigned int buy1val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy1valstr = _T("");buy1valstr.Format(L"%u",buy1val);
							m_ListCtl.SetItemText(j,31,buy1valstr);
						}
					}
					//22位：卖二价和买二价 
					if(attr&0x400000)
					{
					    //卖二价                int
					    if(PackLength>offset)
					    {
							unsigned int sell2price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell2pricestr = _T("");sell2pricestr.Format(L"%u",sell2price);
							m_ListCtl.SetItemText(j,32,sell2pricestr);
						}
						//买二价                int
					    if(PackLength>offset)
					    {
							unsigned int buy2price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy2pricestr = _T("");buy2pricestr.Format(L"%u",buy2price);
							m_ListCtl.SetItemText(j,33,buy2pricestr);
						}
					}
					//23位：卖二量和买二量 
					if(attr&0x800000)
					{
					    //卖二量               int
					    if(PackLength>offset)
					    {
							unsigned int sell2val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell2valstr = _T("");sell2valstr.Format(L"%u",sell2val);
							m_ListCtl.SetItemText(j,34,sell2valstr);
						}
						//买二量                int
					    if(PackLength>offset)
					    {
							unsigned int buy2val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy2valstr = _T("");buy2valstr.Format(L"%u",buy2val);
							m_ListCtl.SetItemText(j,35,buy2valstr);
						}
					}
					//24位：卖三价和买三价 
					if(attr&0x1000000)
					{
					    //卖三价                int
					    if(PackLength>offset)
					    {
							unsigned int sell3price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell3pricestr = _T("");sell3pricestr.Format(L"%u",sell3price);
							m_ListCtl.SetItemText(j,36,sell3pricestr);
						}
						//买三价                int
					    if(PackLength>offset)
					    {
							unsigned int buy3price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy3pricestr = _T("");buy3pricestr.Format(L"%u",buy3price);
							m_ListCtl.SetItemText(j,37,buy3pricestr);
						}
					}
					//25位：卖三量和买三量 
					if(attr&0x2000000)
					{
					    //卖三量               int
					    if(PackLength>offset)
					    {
							unsigned int sell3val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell3valstr = _T("");sell3valstr.Format(L"%u",sell3val);
							m_ListCtl.SetItemText(j,38,sell3valstr);
						}
						//买三量                int
					    if(PackLength>offset)
					    {
							unsigned int buy3val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy3valstr = _T("");buy3valstr.Format(L"%u",buy3val);
							m_ListCtl.SetItemText(j,39,buy3valstr);
						}
					}
					//26位：卖四价和买四价 
					if(attr&0x4000000)
					{
					    //卖四价                int
					    if(PackLength>offset)
					    {
							unsigned int sell4price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell4pricestr = _T("");sell4pricestr.Format(L"%u",sell4price);
							m_ListCtl.SetItemText(j,40,sell4pricestr);
						}
						//买四价                int
					    if(PackLength>offset)
					    {
							unsigned int buy4price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy4pricestr = _T("");buy4pricestr.Format(L"%u",buy4price);
							m_ListCtl.SetItemText(j,41,buy4pricestr);
						}
					}
					//27位：卖四量和买四量 
					if(attr&0x8000000)
					{
					    //卖四量               int
					    if(PackLength>offset)
					    {
							unsigned int sell4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell4valstr = _T("");sell4valstr.Format(L"%u",sell4val);
							m_ListCtl.SetItemText(j,42,sell4valstr);
						}
						//买四量                int
					    if(PackLength>offset)
					    {
							unsigned int buy4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy4valstr = _T("");buy4valstr.Format(L"%u",buy4val);
							m_ListCtl.SetItemText(j,43,buy4valstr);
						}
					}
					//28位：卖五价和买五价 
					if(attr&0x10000000)
					{
					    //卖五价                int
					    if(PackLength>offset)
					    {
							unsigned int sell5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell5pricestr = _T("");sell5pricestr.Format(L"%u",sell5price);
							m_ListCtl.SetItemText(j,44,sell5pricestr);
						}
						//买五价                int
					    if(PackLength>offset)
					    {
							unsigned int buy5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy5pricestr = _T("");buy5pricestr.Format(L"%u",buy5price);
							m_ListCtl.SetItemText(j,45,buy5pricestr);
						}
					}
					//29位：卖五量和买五量 
					if(attr&0x20000000)
					{
					    //卖五量               int
					    if(PackLength>offset)
					    {
							unsigned int sell5val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell5valstr = _T("");sell5valstr.Format(L"%u",sell5val);
							m_ListCtl.SetItemText(j,46,sell5valstr);
						}
						//买五量                int
					    if(PackLength>offset)
					    {
							unsigned int buy5val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy5valstr = _T("");buy5valstr.Format(L"%u",buy5val);
							m_ListCtl.SetItemText(j,47,buy5valstr);
						}
					}
					//30位：融资融券标记  //2015-3-13增加 
					if(attr&0x40000000)
					{
					    //融资融券标记         char //2015-3-13增加
					    if(PackLength>offset)
					    {
							char rzrqflag = *(PackData+offset);
							offset += 1;
							int flagval = rzrqflag;CString rzrqflagstr = _T("");rzrqflagstr.Format(L"%u",flagval);
							m_ListCtl.SetItemText(j,48,rzrqflagstr);
						}
					}
					//31位：信息地雷标记  //2015-3-13增加
					if(attr&0x80000000)
					{
						//信息地雷时间         int  //2015-3-13增加
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"结点数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"类名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"该结点显示方式",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"该类品种",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"分类的首字符",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"该结点类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"子类数目或者类id",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//结点数目	short		//指最近一级的数目
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
					
					//类名称		string
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
					//该结点显示方式	char	//该结点在界面上显示方式，0表示列表方式，1表示在菜单方式
					if(PackLength>offset)
					{
						char type = *(PackData+offset);
						offset += 1;
						int typeval = type;CString typeSTR = _T("");typeSTR.Format(L"%d",typeval);
						m_ListCtl.SetItemText(i,3,typeSTR);
					}
					//该类品种		char
					if(PackLength>offset)
					{
						char typevalue = *(PackData+offset);
						offset += 1;
						int typevalueval = typevalue;CString typevalueSTR = _T("");typevalueSTR.Format(L"%d",typevalueval);
						m_ListCtl.SetItemText(i,4,typevalueSTR);
					}
					//分类的首字符          char    //为0表示无效字符
					if(PackLength>offset)
					{
						char typeszm = *(PackData+offset);
						offset += 1;
						CString typeszmSTR = _T("");typeszmSTR.Format(L"%c",typeszm);
						m_ListCtl.SetItemText(i,5,typeszmSTR);
					}
					//该结点类型		char	//0表示该节点是叶结点，下面的short是该结点的id；1表示该结点是非叶结点，下面的short记录它子结点数目
					if(PackLength>offset)
					{
						char jdtype = *(PackData+offset);
						offset += 1;
						int jdtypeval = jdtype;CString jdtypeSTR = _T("");jdtypeSTR.Format(L"%d",jdtypeval);
						m_ListCtl.SetItemText(i,6,jdtypeSTR);
					}
					//子类数目或者类id	short
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"股票名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"关注度",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"小数点位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总数             short              //20120216改动
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset+=2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumstr);
			}
			//位置             short		
			if(PackLength>offset)
			{
				short position = *(short*)(PackData+offset);
				offset+=2;
				CString positionstr = _T("");positionstr.Format(L"%d",position);
				m_ListCtl.SetItemText(0,2,positionstr);
			}
			//数目		    short
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
					
					//代码	    String
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
					//股票名称   String
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
					//关注度     int	
					if(PackLength>offset)
					{
						unsigned int gzdval = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString gzdSTR = _T("");gzdSTR.Format(L"%u",gzdval);
						m_ListCtl.SetItemText(i,6,gzdSTR);
					}
					//涨幅       int*10000  //有正负号数
					if(PackLength>offset)
					{
						int zfvalue = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalueSTR = _T("");zfvalueSTR.Format(L"%d",zfvalue);
						m_ListCtl.SetItemText(i,7,zfvalueSTR);
					}
					//小数点位数 char
					if(PackLength>offset)
					{
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointposval = pointpos;CString pointposSTR = _T("");pointposSTR.Format(L"%d",pointposval);
						m_ListCtl.SetItemText(i,8,pointposSTR);
					}
					//最新价     int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"板块的id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总记录数	short	服务器端该类型总记录数
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset+=2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumstr);
			}
			//记录数         short	下列记录数
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
					//代码           String
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
					//名称           String
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
					//小数位数       byte
					if(PackLength>offset)
					{
						char pointpos = *(PackData+offset);
						offset += 1;
						int pointposval = pointpos;CString pointposSTR = _T("");pointposSTR.Format(L"%d",pointposval);
						m_ListCtl.SetItemText(j,5,pointposSTR);
					}
					//最新价         int	
					if(PackLength>offset)
					{
						unsigned int newprice = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newpriceSTR = _T("");newpriceSTR.Format(L"%u",newprice);
						m_ListCtl.SetItemText(j,6,newpriceSTR);
					}
					//涨幅           int*10000
					if(PackLength>offset)
					{
						int zfvalue = *(int*)(PackData+offset);
						offset += 4;
						CString zfvalueSTR = _T("");zfvalueSTR.Format(L"%d",zfvalue);
						m_ListCtl.SetItemText(j,7,zfvalueSTR);
					}
					
					//板块的id       short
					if(PackLength>offset)
					{
						unsigned short bkid = *(unsigned short*)(PackData+offset);
						offset += 2;
						CString bkidSTR = _T("");bkidSTR.Format(L"%u",bkid);
						m_ListCtl.SetItemText(j,8,bkidSTR);
					}
					//代码           String
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
					//名称           String
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
					//小数位数       byte
					if(PackLength>offset)
					{
						char pointpos1 = *(PackData+offset);
						offset += 1;
						int pointpos1val = pointpos1;CString pointpos1STR = _T("");pointpos1STR.Format(L"%d",pointpos1val);
						m_ListCtl.SetItemText(j,11,pointpos1STR);
					}
					//最新价         int	
					if(PackLength>offset)
					{
						unsigned int newprice1 = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newprice1STR = _T("");newprice1STR.Format(L"%u",newprice1);
						m_ListCtl.SetItemText(j,12,newprice1STR);
					}
					//涨幅           int*10000
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"数据间隔",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"走势数据掩码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"数据位置",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"均价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// 数据间隔       char
			if(PackLength>offset)
			{
				char datepos = *(PackData+offset);
				offset+=1;
				int dateposval = datepos;CString dateposstr = _T("");dateposstr.Format(L"%d",dateposval);
				m_ListCtl.SetItemText(0,1,dateposstr);
			}
			//走势数据掩码   char	    //
			if(PackLength>offset)
			{
				char zsdatepos = *(PackData+offset);
				offset+=1;
				int zsdateposval = zsdatepos;CString zsdateposstr = _T("");zsdateposstr.Format(L"%d",zsdateposval);
				m_ListCtl.SetItemText(0,2,zsdateposstr);
			}
			//数据位置       short		
			if(PackLength>offset)
			{
				short position = *(short*)(PackData+offset);
				offset+=2;
				CString positionstr = _T("");positionstr.Format(L"%d",position);
				m_ListCtl.SetItemText(0,3,positionstr);
			}
			//记录数         short
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
					
					//时间           int
					if(PackLength>offset)
					{
						unsigned int datetime = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString datetimestr = _T("");datetimestr.Format(L"%u",datetime);
						 m_ListCtl.SetItemText(i,5,datetimestr);
					}
					//最新价     int
					if(PackLength>offset)
					{
						unsigned int newprice = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString newpriceSTR = _T("");newpriceSTR.Format(L"%u",newprice);
						m_ListCtl.SetItemText(i,6,newpriceSTR);
					}
					//成交量         int	   //总成交量
					if(PackLength>offset)
					{
						unsigned int cjval = *(unsigned int*)(PackData+offset);
						offset += 4;
						CString cjvalSTR = _T("");cjvalSTR.Format(L"%u",cjval);
						m_ListCtl.SetItemText(i,7,cjvalSTR);
					}
					//均价           int  
					if(PackLength>offset)
					{
						int avgvalue = *(int*)(PackData+offset);
						offset += 4;
						CString avgvalueSTR = _T("");avgvalueSTR.Format(L"%d",avgvalue);
						m_ListCtl.SetItemText(i,8,avgvalueSTR);
					}
					//持仓量         int      //总持仓量
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"字段属性掩码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"总记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"股票类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"成交单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"美式定价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"美式定价小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"隐含波动率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"隐含波动率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"标的价格敏感性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"标的价格敏感性小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"标的价格二次敏感性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"标的价格二次敏感性小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"时间敏感性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"时间敏感性小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(26,L"波动率敏感性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(27,L"波动率敏感性小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(28,L"无风险利率敏感性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(29,L"无风险利率敏感性小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(30,L"杠杆率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(31,L"杠杆率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(32,L"实际(有效)杠杆率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(33,L"实际(有效)杠杆率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(34,L"溢价率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(35,L"溢价率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(36,L"时间价值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(37,L"时间价值小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(38,L"内在价值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(39,L"内在价值小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(40,L"时间值耗损",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(41,L"时间值耗损小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(42,L"值类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(43,L"实虚具体值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(44,L"实虚具体值小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(45,L"年化到期时间率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(46,L"年化到期时间率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(47,L"行权价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(48,L"行权价格小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(49,L"无风险利率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(50,L"无风险利率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(51,L"历史波动率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(52,L"历史波动率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(53,L"风险收益率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(54,L"风险收益率小数位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(55,L"步长",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(56,L"权类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(57,L"行权日",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(58,L"买一价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(59,L"卖一价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(60,L"买一量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(61,L"卖一量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(62,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(63,L"结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(64,L"昨结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//字段属性掩码   int
			unsigned int attr = 0;
			if(PackLength>offset)
			{
				attr = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString attrstr = _T("");attrstr.Format(L"%u",attr);
				m_ListCtl.SetItemText(0,1,attrstr);
			}
			//总记录数       short
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumstr = _T("");tolnumstr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,2,tolnumstr);
			}
			//记录数         short
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
					//代码           String
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
					//名称           String
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
					////0位
					if(attr&0x0001)
					{
					    //小数位数       byte	//昨收价、最新价、开高低价小数位
					    if(PackLength>offset)
					    {
						    unsigned char pointpos = *(PackData+offset);
						    offset += 1;
							int pointposval = pointpos;CString pointposstr=_T("");pointposstr.Format(L"%d",pointposval);
						    m_ListCtl.SetItemText(j,6,pointposstr);
						}
						//股票类型       byte
						if(PackLength>offset)
					    {
						    unsigned char stocktype = *(PackData+offset);
						    offset += 1;
							int stocktypeval = stocktype;CString stocktypestr=_T("");stocktypestr.Format(L"%d",stocktypeval);
						    m_ListCtl.SetItemText(j,7,stocktypestr);
						}
						//成交单位	  short
						if(PackLength>offset)
			            {
				            short cjtype = *(short*)(PackData+offset);
				            offset += 2;
				            CString cjtypestr = _T("");cjtypestr.Format(L"%d",cjtype);
				            m_ListCtl.SetItemText(0,8,cjtypestr);
			            }
						//昨收价         int
						if(PackLength>offset)
			            {
				            unsigned int zsprice = *(unsigned int*)(PackData+offset);
				            offset += 4;
				            CString zspricestr = _T("");zspricestr.Format(L"%d",zsprice);
				            m_ListCtl.SetItemText(0,9,zspricestr);
			            }
					}
					//1位
					if(attr&0x0002)
					{
					    //最新价         int   
					    if(PackLength>offset)
					    {
							unsigned int newprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
							m_ListCtl.SetItemText(j,10,newpricestr);
						}
					}
					// //2位
					if(attr&0x0004)
					{
					    //开盘价         int
					    if(PackLength>offset)
					    {
							unsigned int openprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
							m_ListCtl.SetItemText(j,11,openpricestr);
						}
						//最高价         int
						if(PackLength>offset)
					    {
							unsigned int highprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
							m_ListCtl.SetItemText(j,12,highpricestr);
						}
						//最低价         int
						if(PackLength>offset)
					    {
							unsigned int lowprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
							m_ListCtl.SetItemText(j,13,lowpricestr);
						}
					}
					//3位
					if(attr&0x0008)
					{
					    //成交额         int      //总成交额  
					    if(PackLength>offset)
					    {
							int cje = *(int*)(PackData+offset);
							offset += 4;
							CString cjestr = _T("");cjestr.Format(L"%d",cje);
							m_ListCtl.SetItemText(j,14,cjestr);
						}
					}
					//4位
					if(attr&0x0010)
					{
					   // 成交量         int	   //总成交量 
					    if(PackLength>offset)
					    {
							unsigned int cjval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString cjvalstr = _T("");cjvalstr.Format(L"%u",cjval);
							m_ListCtl.SetItemText(j,15,cjvalstr);
						}
					}
					//5位
					if(attr&0x0020)
					{
					    //美式定价       int
					    if(PackLength>offset)
					    {
							unsigned int msprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString mspricestr = _T("");mspricestr.Format(L"%u",msprice);
							m_ListCtl.SetItemText(j,16,mspricestr);
						}
						//美式定价小数位 byte
						if(PackLength>offset)
					    {
						    unsigned char mspointpos = *(PackData+offset);
						    offset += 1;
							int mspointposval = mspointpos;CString mspointposstr=_T("");mspointposstr.Format(L"%d",mspointposval);
						    m_ListCtl.SetItemText(j,17,mspointposstr);
						}
					}
					//6位
					if(attr&0x0040)
					{
					    //隐含波动率     int
					    if(PackLength>offset)
					    {
							int yhbdl = *(int*)(PackData+offset);
							offset += 4;
							CString yhbdlstr = _T("");yhbdlstr.Format(L"%d",yhbdl);
							m_ListCtl.SetItemText(j,18,yhbdlstr);
						}
						//隐含波动率小数位 byte
						if(PackLength>offset)
					    {
						    unsigned char bdpointpos = *(PackData+offset);
						    offset += 1;
							int bdpointposval = bdpointpos;CString bdpointposstr=_T("");bdpointposstr.Format(L"%d",bdpointposval);
						    m_ListCtl.SetItemText(j,19,bdpointposstr);
						}
					}
					//7位：昨日结算价
					if(attr&0x0080)
					{
					     //标的价格敏感性  int 
					    if(PackLength>offset)
					    {
							int bpricemgx = *(int*)(PackData+offset);
							offset += 4;
							CString bpricemgxstr = _T("");bpricemgxstr.Format(L"%d",bpricemgx);
							m_ListCtl.SetItemText(j,20,bpricemgxstr);
						}
						//标的价格敏感性小数位 byte
						if(PackLength>offset)
					    {
						    unsigned char bpricemgxpos = *(PackData+offset);
						    offset += 1;
							int bpricemgxposval = bpricemgxpos;CString bpricemgxposstr=_T("");bpricemgxposstr.Format(L"%d",bpricemgxposval);
						    m_ListCtl.SetItemText(j,21,bpricemgxposstr);
						}
					}
					//8位
					if(attr&0x0100)
					{
					    //标的价格二次敏感性 int 
					    if(PackLength>offset)
					    {
							int bprice2mgx = *(int*)(PackData+offset);
							offset += 4;
							CString bprice2mgxstr = _T("");bprice2mgxstr.Format(L"%d",bprice2mgx);
							m_ListCtl.SetItemText(j,22,bprice2mgxstr);
						}
						//标的价格二次敏感性小数位 byte
						if(PackLength>offset)
					    {
						    unsigned char bprice2mgxpos = *(PackData+offset);
						    offset += 1;
							int bprice2mgxposval = bprice2mgxpos;CString bprice2mgxposstr=_T("");bprice2mgxposstr.Format(L"%d",bprice2mgxposval);
						    m_ListCtl.SetItemText(j,23,bprice2mgxposstr);
						}
					}
					//9位：动态数据时间，HHMMSS
					if(attr&0x0200)
					{
					    //时间敏感性       int
					    if(PackLength>offset)
					    {
							int timemgx = *(int*)(PackData+offset);
							offset += 4;
							CString timemgxstr = _T("");timemgxstr.Format(L"%d",timemgx);
							m_ListCtl.SetItemText(j,24,timemgxstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char timemgxpos = *(PackData+offset);
						    offset += 1;
							int timemgxposval = timemgxpos;CString timemgxposstr=_T("");timemgxposstr.Format(L"%d",timemgxposval);
						    m_ListCtl.SetItemText(j,25,timemgxposstr);
						}
					}
					//10位
					if(attr&0x0200)
					{
					   //波动率敏感性       int
					    if(PackLength>offset)
					    {
							int posmgx = *(int*)(PackData+offset);
							offset += 4;
							CString posmgxstr = _T("");posmgxstr.Format(L"%d",posmgx);
							m_ListCtl.SetItemText(j,26,posmgxstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char posmgxpos = *(PackData+offset);
						    offset += 1;
							int posmgxval = posmgxpos;CString posmgxposstr=_T("");posmgxposstr.Format(L"%d",posmgxval);
						    m_ListCtl.SetItemText(j,27,posmgxposstr);
						}
					}
					//11位：最高和最低
					if(attr&0x0800)
					{
					    //最高                 int
					     //无风险利率敏感性       int
					    if(PackLength>offset)
					    {
							int wfxmgx = *(int*)(PackData+offset);
							offset += 4;
							CString wfxmgxstr = _T("");wfxmgxstr.Format(L"%d",wfxmgx);
							m_ListCtl.SetItemText(j,28,wfxmgxstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char wfxmgxpos = *(PackData+offset);
						    offset += 1;
							int wfxmgxval = wfxmgxpos;CString wfxmgxposstr=_T("");wfxmgxposstr.Format(L"%d",wfxmgxval);
						    m_ListCtl.SetItemText(j,29,wfxmgxposstr);
						}
					}
					//12位：最新
					if(attr&0x1000)
					{
					    //杠杆率                 int
					    if(PackLength>offset)
					    {
							int ggl = *(int*)(PackData+offset);
							offset += 4;
							CString gglstr = _T("");gglstr.Format(L"%d",ggl);
							m_ListCtl.SetItemText(j,30,gglstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char gglpos = *(PackData+offset);
						    offset += 1;
							int gglposval = gglpos;CString gglposstr=_T("");gglposstr.Format(L"%d",gglposval);
						    m_ListCtl.SetItemText(j,31,gglposstr);
						}
					}
					//13位：均价
					if(attr&0x2000)
					{
					    //实际(有效)杠杆率       int
					    if(PackLength>offset)
					    {
							int sjggl = *(int*)(PackData+offset);
							offset += 4;
							CString sjgglstr = _T("");sjgglstr.Format(L"%d",sjggl);
							m_ListCtl.SetItemText(j,32,sjgglstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char sjgglpos = *(PackData+offset);
						    offset += 1;
							int sjgglposval = sjgglpos;CString sjgglposstr=_T("");sjgglposstr.Format(L"%d",sjgglposval);
						    m_ListCtl.SetItemText(j,33,sjgglposstr);
						}
					}
					//14位：成交量 
					if(attr&0x4000)
					{
					     //溢价率                  int
					    if(PackLength>offset)
					    {
							int yjl = *(int*)(PackData+offset);
							offset += 4;
							CString yjlstr = _T("");yjlstr.Format(L"%d",yjl);
							m_ListCtl.SetItemText(j,34,yjlstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char yjlpos = *(PackData+offset);
						    offset += 1;
							int yjlposval = yjlpos;CString yjlposstr=_T("");yjlposstr.Format(L"%d",yjlposval);
						    m_ListCtl.SetItemText(j,35,yjlposstr);
						}
					}
					//15位：现手 
					if(attr&0x8000)
					{
					     //时间价值                int
					    if(PackLength>offset)
					    {
							int sjjz = *(int*)(PackData+offset);
							offset += 4;
							CString sjjzstr = _T("");sjjzstr.Format(L"%d",sjjz);
							m_ListCtl.SetItemText(j,36,sjjzstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char sjjzpos = *(PackData+offset);
						    offset += 1;
							int sjjzposval = sjjzpos;CString sjjzposstr=_T("");sjjzposstr.Format(L"%d",sjjzposval);
						    m_ListCtl.SetItemText(j,37,sjjzposstr);
						}
					}
					//16位：成交额 
					if(attr&0x10000)
					{
					     //内在价值                int
					    if(PackLength>offset)
					    {
							int nzjz = *(int*)(PackData+offset);
							offset += 4;
							CString nzjzstr = _T("");nzjzstr.Format(L"%d",nzjz);
							m_ListCtl.SetItemText(j,38,nzjzstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char nzjzpos = *(PackData+offset);
						    offset += 1;
							int nzjzposval = nzjzpos;CString nzjzposstr=_T("");nzjzposstr.Format(L"%d",nzjzposval);
						    m_ListCtl.SetItemText(j,39,nzjzposstr);
						}
					}
					//17位：内盘 
					if(attr&0x20000)
					{
					     //时间值耗损              int
					    if(PackLength>offset)
					    {
							int sjzhs = *(int*)(PackData+offset);
							offset += 4;
							CString sjzhsstr = _T("");sjzhsstr.Format(L"%d",sjzhs);
							m_ListCtl.SetItemText(j,40,sjzhsstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char sjzhspos = *(PackData+offset);
						    offset += 1;
							int sjzhsposval = sjzhspos;CString sjzhsposstr=_T("");sjzhsposstr.Format(L"%d",sjzhsposval);
						    m_ListCtl.SetItemText(j,41,sjzhsposstr);
						}
					}
					//18位：持仓量 
					if(attr&0x40000)
					{
						//值类型                  byte  //实值:1 虚值:2 平值:3
						if(PackLength>offset)
					    {
						    unsigned char valtype = *(PackData+offset);
						    offset += 1;
							int valtypeval = valtype;CString valtypestr=_T("");valtypestr.Format(L"%d",valtypeval);
						    m_ListCtl.SetItemText(j,42,valtypestr);
						}
					     //实虚具体值               int
					    if(PackLength>offset)
					    {
							int sxjtz = *(int*)(PackData+offset);
							offset += 4;
							CString sxjtzstr = _T("");sxjtzstr.Format(L"%d",sxjtz);
							m_ListCtl.SetItemText(j,43,sxjtzstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char sxjtzpos = *(PackData+offset);
						    offset += 1;
							int sxjtzposval = sxjtzpos;CString sxjtzposstr=_T("");sxjtzposstr.Format(L"%d",sxjtzposval);
						    m_ListCtl.SetItemText(j,44,sxjtzposstr);
						}
					}
					//19位：结算价 
					if(attr&0x80000)
					{
					     //年化到期时间率           int
					    if(PackLength>offset)
					    {
							int nhdq = *(int*)(PackData+offset);
							offset += 4;
							CString nhdqstr = _T("");nhdqstr.Format(L"%d",nhdq);
							m_ListCtl.SetItemText(j,45,nhdqstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char nhdqpos = *(PackData+offset);
						    offset += 1;
							int nhdqposval = nhdqpos;CString nhdqposstr=_T("");nhdqposstr.Format(L"%d",nhdqposval);
						    m_ListCtl.SetItemText(j,46,nhdqposstr);
						}
					}
					//20位：卖一价和买一价 
					if(attr&0x100000)
					{
					     //行权价格                 int
					    if(PackLength>offset)
					    {
							int xqjg = *(int*)(PackData+offset);
							offset += 4;
							CString xqjgstr = _T("");xqjgstr.Format(L"%d",xqjg);
							m_ListCtl.SetItemText(j,47,xqjgstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char xqjgpos = *(PackData+offset);
						    offset += 1;
							int xqjgposval = xqjgpos;CString xqjgposstr=_T("");xqjgposstr.Format(L"%d",xqjgposval);
						    m_ListCtl.SetItemText(j,48,xqjgposstr);
						}
					}
					//21位：卖一量和买一量 
					if(attr&0x200000)
					{
					    //无风险利率              int
					    if(PackLength>offset)
					    {
							int wfxllval = *(int*)(PackData+offset);
							offset += 4;
							CString wfxllvalstr = _T("");wfxllvalstr.Format(L"%d",wfxllval);
							m_ListCtl.SetItemText(j,49,wfxllvalstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char wfxllvalpos = *(PackData+offset);
						    offset += 1;
							int wfxllvalval = wfxllvalpos;CString wfxllvalposstr=_T("");wfxllvalposstr.Format(L"%d",wfxllvalval);
						    m_ListCtl.SetItemText(j,50,wfxllvalposstr);
						}
					}
					//22位：卖二价和买二价 
					if(attr&0x400000)
					{
					    //历史波动率              int
					    if(PackLength>offset)
					    {
							int lsbdl = *(int*)(PackData+offset);
							offset += 4;
							CString lsbdlstr = _T("");lsbdlstr.Format(L"%d",lsbdl);
							m_ListCtl.SetItemText(j,51,lsbdlstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char lsbdlpos = *(PackData+offset);
						    offset += 1;
							int lsbdlval = lsbdlpos;CString lsbdlposstr=_T("");lsbdlposstr.Format(L"%d",lsbdlval);
						    m_ListCtl.SetItemText(j,52,lsbdlposstr);
						}
					}
					//23位：卖二量和买二量 
					if(attr&0x800000)
					{
					     //风险收益率              int
					    if(PackLength>offset)
					    {
							int fxsyl = *(int*)(PackData+offset);
							offset += 4;
							CString fxsylstr = _T("");fxsylstr.Format(L"%d",fxsyl);
							m_ListCtl.SetItemText(j,53,fxsylstr);
						}
						//时间敏感性小数位  byte
						if(PackLength>offset)
					    {
						    unsigned char fxsylpos = *(PackData+offset);
						    offset += 1;
							int fxsylposval = fxsylpos;CString fxsylposstr=_T("");fxsylposstr.Format(L"%d",fxsylposval);
						    m_ListCtl.SetItemText(j,54,fxsylposstr);
						}
					}
					//24位：卖三价和买三价 
					if(attr&0x1000000)
					{
					    //步长                    byte
					    if(PackLength>offset)
					    {
							unsigned char bcval = *(PackData+offset);
						    offset += 1;
							int bcvalval = bcval;CString bcvalstr=_T("");bcvalstr.Format(L"%d",bcvalval);
						    m_ListCtl.SetItemText(j,55,bcvalstr);
						}
					}
					//25位：卖三量和买三量 
					if(attr&0x2000000)
					{
					    //权类型                  byte  //买权1/卖权-1
					    if(PackLength>offset)
					    {
							char qtype = *(PackData+offset);
						    offset += 1;
							int qtypeval = qtype;CString qtypestr=_T("");qtypestr.Format(L"%d",qtypeval);
						    m_ListCtl.SetItemText(j,56,qtypestr);
						}
					}
					//26位：卖四价和买四价 
					if(attr&0x4000000)
					{
					    //行权日                   int
					    if(PackLength>offset)
					    {
							unsigned int xqdate = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString xqdatestr = _T("");xqdatestr.Format(L"%u",xqdate);
							m_ListCtl.SetItemText(j,57,xqdatestr);
						}
					}
					//27位：卖四量和买四量 
					if(attr&0x8000000)
					{
					    //买一价                   int
					    if(PackLength>offset)
					    {
							unsigned int sell4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell4valstr = _T("");sell4valstr.Format(L"%u",sell4val);
							m_ListCtl.SetItemText(j,58,sell4valstr);
						}
						//卖一价                   int
					    if(PackLength>offset)
					    {
							unsigned int buy4val = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy4valstr = _T("");buy4valstr.Format(L"%u",buy4val);
							m_ListCtl.SetItemText(j,59,buy4valstr);
						}
					}
					//28位：卖五价和买五价 
					if(attr&0x10000000)
					{
					    //买一量                   int
					    if(PackLength>offset)
					    {
							unsigned int sell5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString sell5pricestr = _T("");sell5pricestr.Format(L"%u",sell5price);
							m_ListCtl.SetItemText(j,60,sell5pricestr);
						}
						//卖一量                   int
					    if(PackLength>offset)
					    {
							unsigned int buy5price = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString buy5pricestr = _T("");buy5pricestr.Format(L"%u",buy5price);
							m_ListCtl.SetItemText(j,61,buy5pricestr);
						}
					}
					//29位：卖五量和买五量 
					if(attr&0x20000000)
					{
					    //持仓量                   int
					    if(PackLength>offset)
					    {
							unsigned int holdval = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString holdvalstr = _T("");holdvalstr.Format(L"%u",holdval);
							m_ListCtl.SetItemText(j,62,holdvalstr);
						}
					}
					//30位：融资融券标记  //2015-3-13增加 
					if(attr&0x40000000)
					{
					    //结算价                   int
					    if(PackLength>offset)
					    {
							unsigned int settleprice = *(unsigned int*)(PackData+offset);
							offset += 4;
							CString settlepricestr = _T("");settlepricestr.Format(L"%u",settleprice);
							m_ListCtl.SetItemText(j,63,settlepricestr);
						}
					}
					//31位：信息地雷标记  //2015-3-13增加
					if(attr&0x80000000)
					{
						//昨结算价                 int
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"列表范围",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"列表字段属性",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"总记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"记录数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"股票类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"请求板块指数成分股的id",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"成交单位",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"内盘成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"现手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"涨速",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"委比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"公告数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"市盈率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(26,L"市净率",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(27,L"卖一",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(28,L"买一",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(29,L"7日涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(30,L"7日换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(31,L"30日涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(32,L"30日换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(33,L"当日ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(34,L"当日ddy",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(35,L"当日ddz",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(36,L"60日ddx",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(37,L"60日ddy",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(38,L"10日ddx红色的天数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(39,L"10日ddx连续红色数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(40,L"当日资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(41,L"当日资金流出",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(42,L"5日资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(43,L"5日资金流出",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(44,L"5日资金成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(45,L"30日资金流入",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(46,L"30日资金流出",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(47,L"30日资金成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(48,L"昨日持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(49,L"昨日结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(50,L"持仓量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(51,L"结算价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(52,L"机构买单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(53,L"机构卖单数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(54,L"机构吃货数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(55,L"机构吐货数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(56,L"机构吃货量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(57,L"机构吐货量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(58,L"该股是否是融资融券标的",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(59,L"投顾标记",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(60,L"证券标记",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(61,L"上涨家数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(62,L"下跌家数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(63,L"平盘家数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(64,L"流通市值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(65,L"总市值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(66,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(67,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(68,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(69,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(70,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(71,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(72,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(73,L"小数位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(74,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(75,L"涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//列表范围       short
			short listrange = 0;
			if(PackLength>offset)
			{
				listrange = *(short*)(PackData+offset);
				offset += 2;
				CString range = _T("");range.Format(L"%d",listrange);m_ListCtl.SetItemText(0,1,range);
			}
			//列表字段属性	short	该字段定义见后面说明
			unsigned int listattr = 0;
			if(PackLength>offset)
			{
				listattr = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString listattrStr = _T("");listattrStr.Format(L"%d",listattr);
				m_ListCtl.SetItemText(0,2,listattrStr);
			}
			//总记录数	short	服务器端该类型总记录数
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,3,tolnumStr);
			}
			//记录数         short	下列记录数
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
				//代码           String
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
				//名称           String
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
				//小数位数       byte
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointval = pointpos;CString pointstr=_T("");pointstr.Format(L"%d",pointval);
					m_ListCtl.SetItemText(j,7,pointstr);
				}
				//股票类型       byte
				if(PackLength>offset)
				{
					unsigned char stocktype = *(PackData+offset);
					offset += 1;
					int stocktypeval = stocktype;CString stocktypestr=_T("");stocktypestr.Format(L"%d",stocktypeval);
					m_ListCtl.SetItemText(j,8,stocktypestr);
				}
				//昨收价         int
				if(PackLength>offset)
				{
					int zsprice = *(int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(j,9,zspricestr);
				}
				//开盘价         int
				if(PackLength>offset)
				{
					int openprice = *(int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(j,10,openpricestr);
				}
				//最新价         int
				if(PackLength>offset)
				{
					int newprice = *(int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(j,11,newpricestr);
				}
				//最高价         int
				if(PackLength>offset)
				{
					int highprice = *(int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(j,12,highpricestr);
				}
				//最低价         int
				if(PackLength>offset)
				{
					int lowprice = *(int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(j,13,lowpricestr);
				}
				//成交额         int
				if(PackLength>offset)
				{
					int cjmoney = *(int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(j,14,cjmoneystr);
				}
				//请求板块指数成分股的id    short
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
					//成交单位	  short
				    if(PackLength>offset)
				    {
					    short cjpos = *(short*)(PackData+offset);
				        offset += 2;
					    CString cjposstr = _T("");cjposstr.Format(L"%u",cjpos);
					    m_ListCtl.SetItemText(j,16,cjposstr);
				    }
					//成交量         int
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
					//内盘成交量     int       ------1位
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
					//现手           int       ------2位
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
					//量比           short×100------3位
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
					//换手           short×10000----4位
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
					//涨速           short×10000----5位  有正负号
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
					//委比           short×10000----6位  有正负号
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
					//公告数目       byte 0表示无----7位
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
					//市盈率	  int×100  有正负号
					if(PackLength>offset)
				    {
					    int sylval = *(int*)(PackData+offset);
				        offset += 4;
					    CString sylvalstr = _T("");sylvalstr.Format(L"%d",sylval/100);
					    m_ListCtl.SetItemText(j,25,sylvalstr);
				    }
					//市净率	  int×100  有正负号
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
					//卖一		  int
					if(PackLength>offset)
				    {
					    unsigned int sell = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString sellstr = _T("");sellstr.Format(L"%u",sell);
					    m_ListCtl.SetItemText(j,27,sellstr);
				    }
					//买一		  int
					if(PackLength>offset)
				    {
					    int buy = *(int*)(PackData+offset);
				        offset += 4;
					    CString buystr = _T("");buystr.Format(L"%u",buy);
					    m_ListCtl.SetItemText(j,28,buystr);
				    }
				}
				//统计字段----------10位
				if(listattr!=0&&listattr&(0x0400))
				{
					//7日涨幅	  int×10000  有正负号
					if(PackLength>offset)
				    {
					    int zindex = *(int*)(PackData+offset);
				        offset += 4;
						double zindexval = zindex/10000;
					    CString zindexstr = _T("");zindexstr.Format(L"%lf",zindexval);
					    m_ListCtl.SetItemText(j,29,zindexstr);
				    }
					//7日换手	  int×10000
					if(PackLength>offset)
				    {
					    unsigned int sevenval = *(unsigned int*)(PackData+offset);
				        offset += 4;
						double sevenvalue = sevenval/10000;
					    CString sevenvalstr = _T("");sevenvalstr.Format(L"%lf",sevenvalue);
					    m_ListCtl.SetItemText(j,30,sevenvalstr);
				    }
					//30日涨幅	  int×10000  有正负号
					if(PackLength>offset)
				    {
					    int z30index = *(int*)(PackData+offset);
				        offset += 4;
						double z30indexval = z30index/10000;
					    CString z30indexstr = _T("");z30indexstr.Format(L"%lf",z30indexval);
					    m_ListCtl.SetItemText(j,31,z30indexstr);
				    }
					//30日换手	  int×10000
					if(PackLength>offset)
				    {
					    unsigned int thirtyval = *(unsigned int*)(PackData+offset);
				        offset += 4;
						double thirtyvalval = thirtyval/10000;
					    CString thirtyvalstr = _T("");thirtyvalstr.Format(L"%lf",thirtyvalval);
					    m_ListCtl.SetItemText(j,32,thirtyvalstr);
				    }
				}
				//level2统计字段--11位
				if(listattr!=0&&listattr&(0x0800))
				{
					//当日ddx	  short×1000  有正负号
					if(PackLength>offset)
				    {
					    short dddx = *(short*)(PackData+offset);
				        offset += 2;
						double dddxval = dddx/1000;
					    CString dddxstr = _T("");dddxstr.Format(L"%lf",dddxval);
					    m_ListCtl.SetItemText(j,33,dddxstr);
				    }
					//当日ddy	  short×1000  有正负号
					if(PackLength>offset)
				    {
					    short dddy = *(short*)(PackData+offset);
				        offset += 2;
						double dddyval = dddy/1000;
					    CString dddystr = _T("");dddystr.Format(L"%lf",dddyval);
					    m_ListCtl.SetItemText(j,34,dddystr);
				    }
					//当日ddz	  int×1000  有正负号
					if(PackLength>offset)
				    {
					    int dddz = *(int*)(PackData+offset);
				        offset += 4;
						double dddzval = dddz/1000;
					    CString dddzstr = _T("");dddzstr.Format(L"%lf",dddzval);
					    m_ListCtl.SetItemText(j,35,dddzstr);
				    }
					//60日ddx	  int×1000  有正负号
					if(PackLength>offset)
				    {
					    int sixddxval = *(int*)(PackData+offset);
				        offset += 4;
						double sixddxvalue = sixddxval/1000;
					    CString sixddxvalstr = _T("");sixddxvalstr.Format(L"%lf",sixddxvalue);
					    m_ListCtl.SetItemText(j,36,sixddxvalstr);
				    }
					//60日ddy	  int×1000  有正负号
					if(PackLength>offset)
				    {
					    int sixddyval = *(int*)(PackData+offset);
				        offset += 4;
						double sixddyvalue = sixddyval/1000;
					    CString sixddyvalstr = _T("");sixddyvalstr.Format(L"%lf",sixddyvalue);
					    m_ListCtl.SetItemText(j,37,sixddyvalstr);
				    }
					//10日ddx红色的天数 char
					if(PackLength>offset)
				    {
					    char ddxredval = *(PackData+offset);
				        offset += 1;
					    int ddxred = ddxredval;CString ddxredvalstr = _T("");ddxredvalstr.Format(L"%u",ddxred);
					    m_ListCtl.SetItemText(j,38,ddxredvalstr);
				    }
					//10日ddx连续红色数 char
					if(PackLength>offset)
				    {
					    char ddxredcval = *(PackData+offset);
				        offset += 1;
					    int ddxredc = ddxredcval;CString ddxredcvalstr = _T("");ddxredcvalstr.Format(L"%u",ddxredc);
					    m_ListCtl.SetItemText(j,39,ddxredcvalstr);
				    }
				}
				////level2统计字段--12位, 20111020增加
				if(listattr!=0&&listattr&(0x1000))
				{
					//当日资金流入     int         //资金净额=资金流入-资金流出
					if(PackLength>offset)
				    {
					    unsigned int incap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString incapstr = _T("");incapstr.Format(L"%u",incap);
					    m_ListCtl.SetItemText(j,40,incapstr);
				    }
					//当日资金流出     int 
					if(PackLength>offset)
				    {
					    unsigned int outcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString outcapstr = _T("");outcapstr.Format(L"%u",outcap);
					    m_ListCtl.SetItemText(j,41,outcapstr);
				    }
					//5日资金流入      int
					if(PackLength>offset)
				    {
					    unsigned int fiveincap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString fiveincapstr = _T("");fiveincapstr.Format(L"%u",fiveincap);
					    m_ListCtl.SetItemText(j,42,fiveincapstr);
				    }
					//5日资金流出      int 
					if(PackLength>offset)
				    {
					    unsigned int fiveoutcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString fiveoutcapstr = _T("");fiveoutcapstr.Format(L"%u",fiveoutcap);
					    m_ListCtl.SetItemText(j,43,fiveoutcapstr);
				    }
					//5日资金成交额    int 
					if(PackLength>offset)
				    {
					    unsigned int fivecjcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString fivecjcapstr = _T("");fivecjcapstr.Format(L"%u",fivecjcap);
					    m_ListCtl.SetItemText(j,44,fivecjcapstr);
				    }
					//30日资金流入     int
					if(PackLength>offset)
				    {
					    unsigned int thirtyincap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString thirtyincapstr = _T("");thirtyincapstr.Format(L"%u",thirtyincap);
					    m_ListCtl.SetItemText(j,45,thirtyincapstr);
				    }
					//30日资金流出      int 
					if(PackLength>offset)
				    {
					    unsigned int thirtyoutcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString thirtyoutcapstr = _T("");thirtyoutcapstr.Format(L"%u",thirtyoutcap);
					    m_ListCtl.SetItemText(j,46,thirtyoutcapstr);
				    }
					//30日资金成交额    int 
					if(PackLength>offset)
				    {
					    unsigned int thirtycjcap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString thirtycjcapstr = _T("");thirtycjcapstr.Format(L"%u",thirtycjcap);
					    m_ListCtl.SetItemText(j,47,thirtycjcapstr);
				    }
				}
				//商品类特有数据--13位, 20120307增加
				if(listattr!=0&&listattr&(0x2000))
				{
					//昨日持仓量       int
					if(PackLength>offset)
				    {
					    unsigned int zhold = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString zholdstr = _T("");zholdstr.Format(L"%u",zhold);
					    m_ListCtl.SetItemText(j,48,zholdstr);
				    }
					//昨日结算价       int 
					if(PackLength>offset)
				    {
					    unsigned int zsettle = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString zsettlestr = _T("");zsettlestr.Format(L"%u",zsettle);
					    m_ListCtl.SetItemText(j,49,zsettlestr);
				    }
					//持仓量           int
					if(PackLength>offset)
				    {
					    unsigned int hold = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString holdstr = _T("");holdstr.Format(L"%u",hold);
					    m_ListCtl.SetItemText(j,50,holdstr);
				    }
					//结算价           int 
					if(PackLength>offset)
				    {
					    unsigned int settlecap = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString settlecapstr = _T("");settlecapstr.Format(L"%u",settlecap);
					    m_ListCtl.SetItemText(j,51,settlecapstr);
				    }
				}
				//level2监控数据---14, 20120508
				if(listattr!=0&&listattr&(0x4000))
				{
					//机构买单数       short  //无符号
					if(PackLength>offset)
				    {
					    unsigned short jgval = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString jgvalstr = _T("");jgvalstr.Format(L"%u",jgval);
					    m_ListCtl.SetItemText(j,52,jgvalstr);
				    }
					//机构卖单数       short  //无符号
					if(PackLength>offset)
				    {
					    unsigned short jgvalsell = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString jgvalsellstr = _T("");jgvalsellstr.Format(L"%u",jgvalsell);
					    m_ListCtl.SetItemText(j,53,jgvalsellstr);
				    }
					//机构吃货数       short  //无符号
					if(PackLength>offset)
				    {
					    unsigned short eathold = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString eatholdstr = _T("");eatholdstr.Format(L"%u",eathold);
					    m_ListCtl.SetItemText(j,54,eatholdstr);
				    }
					//机构吐货数       short  //无符号
					if(PackLength>offset)
				    {
					    unsigned short jgtcap = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString jgtcapstr = _T("");jgtcapstr.Format(L"%u",jgtcap);
					    m_ListCtl.SetItemText(j,55,jgtcapstr);
				    }
					//机构吃货量       int
					if(PackLength>offset)
				    {
					    unsigned int eatval = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString eatvalstr = _T("");eatvalstr.Format(L"%u",eatval);
					    m_ListCtl.SetItemText(j,56,eatvalstr);
				    }
					//机构吐货量       int 
					if(PackLength>offset)
				    {
					    unsigned int sellvalue = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString sellvaluestr = _T("");sellvaluestr.Format(L"%u",sellvalue);
					    m_ListCtl.SetItemText(j,57,sellvaluestr);
				    }
				}
				//融资融券标的位-------------15   20130717 
				if(listattr!=0&&listattr&(0x8000))
				{
					//该股是否是融资融券标的 char   1是，0否
					if(PackLength>offset)
				    {
					    char rzrqtag = *(PackData+offset);
				        offset += 1;
						int rzrqtagval = rzrqtag;
					    CString rzrqtagstr = _T("");rzrqtagstr.Format(L"%u",rzrqtag);
					    m_ListCtl.SetItemText(j,58,rzrqtagstr);
				    }
				}
				//投顾标记   -------16位  投顾标记      short
				if(listattr!=0&&listattr&(0x10000))
				{
					//投顾标记      short
					if(PackLength>offset)
				    {
					    short tgtype = *(short*)(PackData+offset);
				        offset += 2;
					    CString tgtypestr = _T("");tgtypestr.Format(L"%d",tgtype);
					    m_ListCtl.SetItemText(j,59,tgtypestr);
				    }
				}
				//证券标记   -------17位
				if(listattr!=0&&listattr&(0x20000))
				{
					//证券标记      short		共16位，用来表示证券只有2个状态的各个标记
					if(PackLength>offset)
				    {
					    unsigned short stocktype = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString stocktypestr = _T("");stocktypestr.Format(L"%d",stocktype);
					    m_ListCtl.SetItemText(j,60,stocktypestr);
				    }
				}
				//板块涨跌家数   -------18位，该字段仅仅板块市场有；2015-9-29增加
				if(listattr!=0&&listattr&(0x40000))
				{
					//上涨家数     short
					if(PackLength>offset)
				    {
					    unsigned short szjs = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString szjsstr = _T("");szjsstr.Format(L"%d",szjs);
					    m_ListCtl.SetItemText(j,61,szjsstr);
				    }
					//下跌家数     short
					if(PackLength>offset)
				    {
					    unsigned short xdjs = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString xdjsstr = _T("");xdjsstr.Format(L"%d",xdjs);
					    m_ListCtl.SetItemText(j,62,xdjsstr);
				    }
					//平盘家数     short
					if(PackLength>offset)
				    {
					    unsigned short ppjs = *(unsigned short*)(PackData+offset);
				        offset += 2;
					    CString ppjsstr = _T("");ppjsstr.Format(L"%d",ppjs);
					    m_ListCtl.SetItemText(j,63,ppjsstr);
				    }
				}
				//板块涨跌家数   -------19位，该字段仅仅板块市场有；2015-10-9增加
				if(listattr!=0&&listattr&(0x80000))
				{
					//流通市值     int  单位万元
					if(PackLength>offset)
				    {
					    int ltsz = *(int*)(PackData+offset);
				        offset += 4;
					    CString ltszstr = _T("");ltszstr.Format(L"%d",ltsz);
					    m_ListCtl.SetItemText(j,64,ltszstr);
				    }
					//总市值       int  单位万元
					if(PackLength>offset)
				    {
					    unsigned int zszval = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString zszvalstr = _T("");zszvalstr.Format(L"%u",zszval);
					    m_ListCtl.SetItemText(j,65,zszvalstr);
				    }
				}
				//板块领涨个股   -------20位，该字段仅仅板块市场有；2015-10-9增加
				if(listattr!=0&&listattr&(0x100000))
				{
					//代码           String
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
					//名称           String
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
					//小数位数       byte	//昨收价、最新价、开高低价小数位
					if(PackLength>offset)
					{
						unsigned char pointpos = *(PackData+offset);
						offset += 1;
					    int pointposval = pointpos;CString pointposstr=_T("");pointposstr.Format(L"%d",pointposval);
						m_ListCtl.SetItemText(j,68,pointposstr);
					}
					//最新价         int   
					if(PackLength>offset)
				    {
					    unsigned int newprice = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString newpricestr = _T("");newpricestr.Format(L"%d",newprice);
					    m_ListCtl.SetItemText(j,69,newpricestr);
				    }
					//涨幅           int*10000
					if(PackLength>offset)
				    {
					    int zfvalue = *(int*)(PackData+offset);
				        offset += 4;
					    CString zfvaluestr = _T("");zfvaluestr.Format(L"%u",zfvalue);
					    m_ListCtl.SetItemText(j,70,zfvaluestr);
				    }
				}
				////板块领跌个股   -------21位，该字段仅仅板块市场有；注意该位和20不能同时存在；2015-10-9增加
				if(listattr!=0&&listattr&(0x100000))
				{
					//代码           String
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
					//名称           String
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
					//小数位数       byte	//昨收价、最新价、开高低价小数位
					if(PackLength>offset)
					{
						unsigned char pointpos1 = *(PackData+offset);
						offset += 1;
					    int pointpos1val = pointpos1;CString pointpos1str=_T("");pointpos1str.Format(L"%d",pointpos1val);
						m_ListCtl.SetItemText(j,73,pointpos1str);
					}
					//最新价         int   
					if(PackLength>offset)
				    {
					    unsigned int newprice1 = *(unsigned int*)(PackData+offset);
				        offset += 4;
					    CString newprice1str = _T("");newprice1str.Format(L"%d",newprice1);
					    m_ListCtl.SetItemText(j,74,newprice1str);
				    }
					//涨幅           int*10000
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"跟踪的指数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"母基代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"价格小数点位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"价格杠杆",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"整体溢价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"场内份额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"初始杠杆",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"指数涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"母基实时净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(21,L"母基昨日净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(22,L"上折母基需涨",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(23,L"下折母基需跌",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(24,L"换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(25,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总记录数	short	服务器端该类型总记录数
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumStr);
			}
			//记录数         short	下列记录数
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
				//代码           String
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
				//名称           String
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
				//跟踪的指数     String
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
				//母基代码       String
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
				//价格小数点位数 char
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointposval = pointpos;CString pointstr = _T("");pointstr.Format(L"%d",pointposval);
					m_ListCtl.SetItemText(i,7,pointstr);
				}
				//昨收价         int
				if(PackLength>offset)
				{
					unsigned int zsprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(i,8,zspricestr);
				}
				//开盘价         int
				if(PackLength>offset)
				{
					unsigned int openprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(i,9,openpricestr);
				}
				//最新价         int
				if(PackLength>offset)
				{
					unsigned int newprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(i,10,newpricestr);
				}
				//最高价         int
				if(PackLength>offset)
				{
					unsigned int highprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(i,11,highpricestr);
				}
				//最低价         int
				if(PackLength>offset)
				{
					unsigned int lowprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(i,12,lowpricestr);
				}
				//成交额         int
				if(PackLength>offset)
				{
					unsigned int cjmoney = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(i,13,cjmoneystr);
				}
				//成交量         int
				if(PackLength>offset)
				{
					unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					m_ListCtl.SetItemText(i,14,cjvaluestr);
				}
				//价格杠杆       int*1000
				if(PackLength>offset)
				{
					unsigned int jggg = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString jgggstr = _T("");jgggstr.Format(L"%u",jggg);
					m_ListCtl.SetItemText(i,15,jgggstr);
				}
				//整体溢价       int*1000
				if(PackLength>offset)
				{
					int ztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString ztyjstr = _T("");ztyjstr.Format(L"%u",ztyj);
					m_ListCtl.SetItemText(i,16,ztyjstr);
				}
				//场内份额       int
				if(PackLength>offset)
				{
					unsigned int cnfe = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cnfestr = _T("");cnfestr.Format(L"%u",cnfe);
					m_ListCtl.SetItemText(i,17,cnfestr);
				}
				//初始杠杆       int*1000
				if(PackLength>offset)
				{
					int csgg = *(int*)(PackData+offset);
				    offset += 4;
					CString csggstr = _T("");csggstr.Format(L"%u",csgg);
					m_ListCtl.SetItemText(i,18,csggstr);
				}
				//指数涨幅       short*100
				if(PackLength>offset)
				{
					short zszf = *(short*)(PackData+offset);
				    offset += 2;
					CString zszfstr = _T("");zszfstr.Format(L"%u",zszf);
					m_ListCtl.SetItemText(i,19,zszfstr);
				}
				//母基实时净值   int*10000
				if(PackLength>offset)
				{
					int mjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjssjzstr = _T("");mjjssjzstr.Format(L"%d",mjjssjz);
					m_ListCtl.SetItemText(i,20,mjjssjzstr);
				}
				//母基昨日净值   int*10000
				if(PackLength>offset)
				{
					int mjjzrjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjzrjzstr = _T("");mjjzrjzstr.Format(L"%d",mjjzrjz);
					m_ListCtl.SetItemText(i,21,mjjzrjzstr);
				}
				//上折母基需涨   int*100
				if(PackLength>offset)
				{
					int szmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString szmjjxzstr = _T("");szmjjxzstr.Format(L"%d",szmjjxz);
					m_ListCtl.SetItemText(i,22,szmjjxzstr);
				}
				//下折母基需跌   int*100
				if(PackLength>offset)
				{
					int xzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString xzmjjxzstr = _T("");xzmjjxzstr.Format(L"%d",xzmjjxz);
					m_ListCtl.SetItemText(i,23,xzmjjxzstr);
				}
				//换手           int*100
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
				    offset += 4;
					CString hsvalstr = _T("");hsvalstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(i,24,hsvalstr);
				}
				//量比           int*100
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"母基代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"价格小数点位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"整体溢价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"场内份额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"隐含收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"约定收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总记录数	short	服务器端该类型总记录数
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumStr);
			}
			//数目           short
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
				//代码           String
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
				//名称           String
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
				//母基代码       String
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
				//价格小数点位数 char
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointposval = pointpos;CString pointstr = _T("");pointstr.Format(L"%d",pointposval);
					m_ListCtl.SetItemText(i,6,pointstr);
				}
				//昨收价         int
				if(PackLength>offset)
				{
					unsigned int zsprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(i,7,zspricestr);
				}
				//开盘价         int
				if(PackLength>offset)
				{
					unsigned int openprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(i,8,openpricestr);
				}
				//最新价         int
				if(PackLength>offset)
				{
					unsigned int newprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(i,9,newpricestr);
				}
				//最高价         int
				if(PackLength>offset)
				{
					unsigned int highprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(i,10,highpricestr);
				}
				//最低价         int
				if(PackLength>offset)
				{
					unsigned int lowprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(i,11,lowpricestr);
				}
				//成交额         int
				if(PackLength>offset)
				{
					unsigned int cjmoney = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(i,12,cjmoneystr);
				}
				//成交量         int
				if(PackLength>offset)
				{
					unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					m_ListCtl.SetItemText(i,13,cjvaluestr);
				}
				//整体溢价       int*1000
				if(PackLength>offset)
				{
					unsigned int ztyj = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString ztyjstr = _T("");ztyjstr.Format(L"%u",ztyj);
					m_ListCtl.SetItemText(i,14,ztyjstr);
				}
				//场内份额       int
				if(PackLength>offset)
				{
					unsigned int cnfe = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cnfestr = _T("");cnfestr.Format(L"%u",cnfe);
					m_ListCtl.SetItemText(i,15,cnfestr);
				}
				//换手           int*100
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
				    offset += 4;
					CString hsvalstr = _T("");hsvalstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(i,16,hsvalstr);
				}
				//量比           int*100
				if(PackLength>offset)
				{
					int lbval = *(int*)(PackData+offset);
				    offset += 4;
					CString lbvalstr = _T("");lbvalstr.Format(L"%d",lbval);
					m_ListCtl.SetItemText(i,17,lbvalstr);
				}
				//隐含收益       int*1000
				if(PackLength>offset)
				{
					int yhsy = *(int*)(PackData+offset);
				    offset += 4;
					CString yhsystr = _T("");yhsystr.Format(L"%d",yhsy);
					m_ListCtl.SetItemText(i,18,yhsystr);
				}
				//约定收益       int*1000
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"总数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"数目",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"代码",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"名称",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"跟踪的指数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"价格小数点位数",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"昨收价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"开盘价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"最新价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"最高价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"最低价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"成交额",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"成交量",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"母基实时净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"母基金溢价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(16,L"指数涨幅",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(17,L"换手",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(18,L"量比",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(19,L"申购费",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(20,L"赎回费",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//总记录数	short	服务器端该类型总记录数
			if(PackLength>offset)
			{
				short tolnum = *(short*)(PackData+offset);
				offset += 2;
				CString tolnumStr = _T("");tolnumStr.Format(L"%d",tolnum);
				m_ListCtl.SetItemText(0,1,tolnumStr);
			}
			//数目           short
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
				//代码           String
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
				//名称           String
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
				//跟踪的指数     String
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
				//价格小数点位数 char
				if(PackLength>offset)
				{
					unsigned char pointpos = *(PackData+offset);
					offset += 1;
					int pointposval = pointpos;CString pointstr = _T("");pointstr.Format(L"%d",pointposval);
					m_ListCtl.SetItemText(i,6,pointstr);
				}
				//昨收价         int
				if(PackLength>offset)
				{
					unsigned int zsprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString zspricestr = _T("");zspricestr.Format(L"%u",zsprice);
					m_ListCtl.SetItemText(i,7,zspricestr);
				}
				//开盘价         int
				if(PackLength>offset)
				{
					unsigned int openprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString openpricestr = _T("");openpricestr.Format(L"%u",openprice);
					m_ListCtl.SetItemText(i,8,openpricestr);
				}
				//最新价         int
				if(PackLength>offset)
				{
					unsigned int newprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString newpricestr = _T("");newpricestr.Format(L"%u",newprice);
					m_ListCtl.SetItemText(i,9,newpricestr);
				}
				//最高价         int
				if(PackLength>offset)
				{
					unsigned int highprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString highpricestr = _T("");highpricestr.Format(L"%u",highprice);
					m_ListCtl.SetItemText(i,10,highpricestr);
				}
				//最低价         int
				if(PackLength>offset)
				{
					unsigned int lowprice = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString lowpricestr = _T("");lowpricestr.Format(L"%u",lowprice);
					m_ListCtl.SetItemText(i,11,lowpricestr);
				}
				//成交额         int
				if(PackLength>offset)
				{
					unsigned int cjmoney = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjmoneystr = _T("");cjmoneystr.Format(L"%u",cjmoney);
					m_ListCtl.SetItemText(i,12,cjmoneystr);
				}
				//成交量         int
				if(PackLength>offset)
				{
					unsigned int cjvalue = *(unsigned int*)(PackData+offset);
				    offset += 4;
					CString cjvaluestr = _T("");cjvaluestr.Format(L"%u",cjvalue);
					m_ListCtl.SetItemText(i,13,cjvaluestr);
				}
				//母基实时净值   int*10000
				if(PackLength>offset)
				{
					int mjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjssjzstr = _T("");mjjssjzstr.Format(L"%d",mjjssjz);
					m_ListCtl.SetItemText(i,14,mjjssjzstr);
				}
				//母基金溢价     int*1000
				if(PackLength>offset)
				{
					int mjjyj = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjyjstr = _T("");mjjyjstr.Format(L"%d",mjjyj);
					m_ListCtl.SetItemText(i,15,mjjyjstr);
				}
				//指数涨幅       short*100
				if(PackLength>offset)
				{
					short zszf = *(short*)(PackData+offset);
				    offset += 2;
					CString zszfstr = _T("");zszfstr.Format(L"%u",zszf);
					m_ListCtl.SetItemText(i,16,zszfstr);
				}
				//换手           int*100
				if(PackLength>offset)
				{
					int hsval = *(int*)(PackData+offset);
				    offset += 4;
					CString hsvalstr = _T("");hsvalstr.Format(L"%d",hsval);
					m_ListCtl.SetItemText(i,17,hsvalstr);
				}
				//量比           int*100
				if(PackLength>offset)
				{
					int lbval = *(int*)(PackData+offset);
				    offset += 4;
					CString lbvalstr = _T("");lbvalstr.Format(L"%d",lbval);
					m_ListCtl.SetItemText(i,18,lbvalstr);
				}
				//申购费         int*100
				if(PackLength>offset)
				{
					int sgfy = *(int*)(PackData+offset);
				    offset += 4;
					CString sgfystr = _T("");sgfystr.Format(L"%d",sgfy);
					m_ListCtl.SetItemText(i,19,sgfystr);
				}
				//赎回费         int*100
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"基金类型",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"整体溢价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"母基实时净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"上折母基需涨",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"下折母基需跌",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(6,L"隐含收益",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(7,L"整体溢价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(8,L"母基实时净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(9,L"上折母基需涨",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(10,L"下折母基需跌",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(11,L"价格杠杆",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(12,L"整体溢价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(13,L"母基实时净值",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(14,L"上折母基需涨",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(15,L"下折母基需跌",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			//基金类型         char		//0、表示A基金；1、B基金；2、母基金；3、非上述类型
			int fundtypeval = 0;
			if(PackLength>offset)
			{
				char fundtype = *(PackData+offset);
				offset += 1;
				fundtypeval = fundtype;CString fundtypestr = _T("");fundtypestr.Format(L"%d",fundtypeval);
				m_ListCtl.SetItemText(0,1,fundtypestr);
			}
			////为0、A基金时为下列字段 
			if(fundtypeval == 0)   //表示的是A基金的字段
			{
				//整体溢价       int*1000
				if(PackLength>offset)
				{
					int ztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString ztyjstr = _T("");ztyjstr.Format(L"%u",ztyj);
					m_ListCtl.SetItemText(0,2,ztyjstr);
				}
				//母基实时净值     int*10000
				if(PackLength>offset)
				{
					int mjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mjjssjzstr = _T("");mjjssjzstr.Format(L"%d",mjjssjz);
					m_ListCtl.SetItemText(0,3,mjjssjzstr);
				}
				//上折母基需涨   int*100
				if(PackLength>offset)
				{
					int szmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString szmjjxzstr = _T("");szmjjxzstr.Format(L"%d",szmjjxz);
					m_ListCtl.SetItemText(0,4,szmjjxzstr);
				}
				//下折母基需跌   int*100
				if(PackLength>offset)
				{
					int xzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString xzmjjxzstr = _T("");xzmjjxzstr.Format(L"%d",xzmjjxz);
					m_ListCtl.SetItemText(0,5,xzmjjxzstr);
				}
				//隐含收益       int*1000
				if(PackLength>offset)
				{
					int yhsy = *(int*)(PackData+offset);
				    offset += 4;
					CString yhsystr = _T("");yhsystr.Format(L"%d",yhsy);
					m_ListCtl.SetItemText(0,6,yhsystr);
				}
			}
			//为1、B基金时为下列字段 
			if(fundtypeval == 1)   //表示的是B基金的字段
			{
				//整体溢价       int*1000
				if(PackLength>offset)
				{
					int bztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString bztyjstr = _T("");bztyjstr.Format(L"%u",bztyj);
					m_ListCtl.SetItemText(0,7,bztyjstr);
				}
				//母基实时净值     int*10000
				if(PackLength>offset)
				{
					int bmjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString bmjjssjzstr = _T("");bmjjssjzstr.Format(L"%d",bmjjssjz);
					m_ListCtl.SetItemText(0,8,bmjjssjzstr);
				}
				//上折母基需涨   int*100
				if(PackLength>offset)
				{
					int bszmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString bszmjjxzstr = _T("");bszmjjxzstr.Format(L"%d",bszmjjxz);
					m_ListCtl.SetItemText(0,9,bszmjjxzstr);
				}
				//下折母基需跌   int*100
				if(PackLength>offset)
				{
					int bxzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString bxzmjjxzstr = _T("");bxzmjjxzstr.Format(L"%d",bxzmjjxz);
					m_ListCtl.SetItemText(0,10,bxzmjjxzstr);
				}
				//价格杠杆         int*1000
				if(PackLength>offset)
				{
					int bjggg = *(int*)(PackData+offset);
				    offset += 4;
					CString bjgggstr = _T("");bjgggstr.Format(L"%d",bjggg);
					m_ListCtl.SetItemText(0,11,bjgggstr);
				}
			}
			//为2、母基金时为下列字段 
		    if(fundtypeval == 2)   //表示的是母基金的字段
			{
				//整体溢价       int*1000
				if(PackLength>offset)
				{
					int momztyj = *(int*)(PackData+offset);
				    offset += 4;
					CString momztyjstr = _T("");momztyjstr.Format(L"%u",momztyj);
					m_ListCtl.SetItemText(0,12,momztyjstr);
				}
				//母基实时净值     int*10000
				if(PackLength>offset)
				{
					int mommjjssjz = *(int*)(PackData+offset);
				    offset += 4;
					CString mommjjssjzstr = _T("");mommjjssjzstr.Format(L"%d",mommjjssjz);
					m_ListCtl.SetItemText(0,13,mommjjssjzstr);
				}
				//上折母基需涨   int*100
				if(PackLength>offset)
				{
					int momszmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString momszmjjxzstr = _T("");momszmjjxzstr.Format(L"%d",momszmjjxz);
					m_ListCtl.SetItemText(0,14,momszmjjxzstr);
				}
				//下折母基需跌   int*100
				if(PackLength>offset)
				{
					int momxzmjjxz = *(int*)(PackData+offset);
				    offset += 4;
					CString momxzmjjxzstr = _T("");momxzmjjxzstr.Format(L"%d",momxzmjjxz);
					m_ListCtl.SetItemText(0,15,momxzmjjxzstr);
				}
			}
			if(fundtypeval == 3)   //为3，无后续字段
			{
				//无字段
			}
		}break;
	case 2995:
		{
			short RecdNum = 0;
			CString RecdNumStr = _T("");
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"状态描述串的crc",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"状态表示字符串数组",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// crc	       short		//状态描述串的crc
			int fundtypeval = 0;
			if(PackLength>offset)
			{
				unsigned short CRCval = *(unsigned short*)(PackData+offset);
				offset += 2;
				CString CRCstr = _T("");CRCstr.Format(L"%d",CRCval);
				m_ListCtl.SetItemText(0,1,CRCstr);
			}
			////为0、A基金时为下列字段 
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
				//字符串数组 String[]  
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
			m_ListCtl.InsertColumn(0,L"序号",LVCFMT_LEFT,60);
			m_ListCtl.InsertColumn(1,L"开始熔断的时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(2,L"结束熔断的时间",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(3,L"冷静期参考价格",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(4,L"冷静期下限价",LVCFMT_LEFT,100);
			m_ListCtl.InsertColumn(5,L"冷静期上限价",LVCFMT_LEFT,100);
			m_ListCtl.InsertItem(0,_T(""));
			m_ListCtl.SetItemText(0,0,L"1");
			// int         开始熔断的时间，格式HHMMSS
			int fundtypeval = 0;
			if(PackLength>offset)
			{
				unsigned int ksrdsj = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ksrdsjstr = _T("");ksrdsjstr.Format(L"%u",ksrdsj);
				m_ListCtl.SetItemText(0,1,ksrdsjstr);
			}
			////int         结束熔断的时间，格式HHMMSS
			if(PackLength>offset)
			{
				unsigned int jsrdsj = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString jsrdsjstr = _T("");jsrdsjstr.Format(L"%u",jsrdsj);
				m_ListCtl.SetItemText(0,2,jsrdsjstr);
			}
			///int         冷静期参考价格
			if(PackLength>offset)
			{
				unsigned int ljqckjg = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ljqckjgstr = _T("");ljqckjgstr.Format(L"%u",ljqckjg);
				m_ListCtl.SetItemText(0,3,ljqckjgstr);
			}
			///int         冷静期下限价
			if(PackLength>offset)
			{
				unsigned int ljqxxjg = *(unsigned int*)(PackData+offset);
				offset += 4;
				CString ljqxxjgstr = _T("");ljqxxjgstr.Format(L"%u",ljqxxjg);
				m_ListCtl.SetItemText(0,4,ljqxxjgstr);
			}
			///int         冷静期上限价
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
*/


/*
case 1000:
{
//返回应答包的字段个数,map数组里面的个数
m_ListCtl.InsertItem(0,_T(""));
//解应答包里面的行情地址字段，此字段是字符串数组格式string【】
//解行情数组个数
short hqaddrnum = *(short*)(PackBody+offset);
offset += 2;
//解行情地址长度字段
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
//nRow = m_ListCtl.InsertItem(j,No);// 插入行
m_ListCtl.SetItemText(j,0,No);
m_ListCtl.SetItemText(j, 1, str.c_str());//设置其它列数据
delete []hqaddr;
}
}
//解析委托地址字段
if(PackLen>offset)
{
short Entrustaddrnum = *(short*)(PackBody+offset);   //委托地址个数
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
m_ListCtl.SetItemText(t, 2, str.c_str());//设置其它列数据
delete []Enaddr;
}
}
//解公告信息  string类型
if(PackLen>offset)
{
short Noticelength = *(short*)(PackBody+offset);   //公告信息字符串长度
offset += 2;
if(Noticelength!=0)
{
char *NoticeMsg = new char[Noticelength+1];
memset(NoticeMsg,0,Noticelength+1);
memcpy(NoticeMsg,PackBody+offset,Noticelength);
offset+=Noticelength;
wstring str = UTF8_2_UTF16(NoticeMsg);
CString wNotice = _T("");wNotice.Format(L"%ws",str.c_str());
m_ListCtl.SetItemText(0, 3, wNotice);//设置其它列数据
delete []NoticeMsg;
}
}
//解新版本号       String
if(PackLen>offset)
{
short Verlength = *(short*)(PackBody+offset);   //公告信息字符串长度
offset += 2;
char *VerMsg = new char[Verlength+1];
memset(VerMsg,0,Verlength+1);
memcpy(VerMsg,PackBody+offset,Verlength);
offset+=Verlength;
wstring Verstr = UTF8_2_UTF16(VerMsg);
m_ListCtl.SetItemText(0, 4, Verstr.c_str());//设置其它列数据
delete []VerMsg;
}
//解下载地址       String
if(PackLen>offset)
{
short Addrlength = *(short*)(PackBody+offset);   //公告信息字符串长度
offset += 2;
char *AddrMsg = new char[Addrlength+1];
memset(AddrMsg,0,Addrlength+1);
memcpy(AddrMsg,PackBody+offset,Addrlength);
offset+=Addrlength;
wstring Addrstr = UTF8_2_UTF16(AddrMsg);
m_ListCtl.SetItemText(0, 5, Addrstr.c_str());//设置其它列数据
delete []AddrMsg;
}
//提醒升级       byte
if(PackLen>offset)
{
unsigned char UpdateMsg = *(PackBody+offset);
offset += 1;
int tempch = UpdateMsg;
CString tempstr = _T("");tempstr.Format(L"%d",tempch);
m_ListCtl.SetItemText(0, 6, tempstr);//设置其它列数据
}
//强制升级       byte      1强制 0不强制
if(PackLen>offset)
{
unsigned char FuMsg = *(PackBody+offset);
offset += 1;
int tempFU = FuMsg;
CString FU = _T("");FU.Format(L"%d",tempFU);
m_ListCtl.SetItemText(0, 7, FU);//设置其它列数据
}
//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
if(PackLen>offset)
{
unsigned char NLMsg = *(PackBody+offset);
offset += 1;
int tempNL = NLMsg;
CString NL = _T("");NL.Format(L"%d",tempNL);
m_ListCtl.SetItemText(0, 8, NL);//设置其它列数据
}
//用户运营商ip类 byte      0表示未知；非0表示有效，
if(PackLen>offset)
{
unsigned char IPMsg = *(PackBody+offset);
offset += 1;
int tempIP = IPMsg;
CString IPstr = _T("");IPstr.Format(L"%d",tempIP);
m_ListCtl.SetItemText(0, 9, IPstr);//设置其它列数据
}

//统计信息时间间隔 short   单位秒,如果为0表示不统计信息.	//20120307日增加
if(PackLen>offset)
{
short Timestap = *(short*)(PackBody+offset);
offset += 2;
CString TStamp = _T("");TStamp.Format(L"%u",Timestap);
m_ListCtl.SetItemText(0, 10, TStamp);//设置其它列数据
}
//升级提示文字   String
if(PackLen>offset)
{
short UNlength = *(short*)(PackBody+offset);   //公告信息字符串长度
offset += 2;
char *UNMsg = new char[UNlength+1];
memset(UNMsg,0,UNlength+1);
memcpy(UNMsg,PackBody+offset,UNlength);
offset+=UNlength;
wstring UNstr = UTF8_2_UTF16(UNMsg);
m_ListCtl.SetItemText(0, 11, UNstr.c_str());//设置其它列数据
delete []UNMsg;
}
//公告crc        short 
if(PackLen>offset)
{
short CRCValue = *(short*)(PackBody+offset);
offset += 2;
CString tempCRC = _T("");tempCRC.Format(L"%u",CRCValue);
m_ListCtl.SetItemText(0, 12, tempCRC);//设置其它列数据
}
//公告提示类型   char	//0或者没有该字段表示强制弹出；1每日弹出一次；2同一公告只弹出一次；3不弹出公告
if(PackLen>offset)
{
unsigned char NoticeType = *(PackBody+offset);
offset += 1;
int tempNotice = NoticeType;
CString Notice = _T("");Notice.Format(L"%d",tempNotice);
m_ListCtl.SetItemText(0, 13, Notice);//设置其它列数据
}

//调度地址       String[]  地址格式为 ip:port:id  //id 新增加，id是运营商id 2015-3-23
if(PackLen>offset)
{
short Addrnum = *(short*)(PackBody+offset);   //委托地址个数
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
m_ListCtl.SetItemText(f, 14, str.c_str());//设置其它列数据
delete []Addr;
}
}
//下列不同服务地址列表数  short
if(PackLen>offset)
{
short ServerNum = *(short*)(PackBody+offset);
offset += 2;
CString tempSvr = _T("");tempSvr.Format(L"%u",ServerNum);
m_ListCtl.SetItemText(0, 15, tempSvr);//设置其它列数据
}
//服务id             int
if(PackLen>offset)
{
int ServerID = *(int*)(PackBody+offset);
offset += 4;
CString tempSvrID = _T("");tempSvrID.Format(L"%d",ServerID);
m_ListCtl.SetItemText(0, 16, tempSvrID);//设置其它列数据
}
//服务器地址         String[]	//服务器地址格式：ip:port:id
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
m_ListCtl.SetItemText(m, 17, str.c_str());//设置其它列数据
delete []Ser;
}
}
}break;
case 1001:
{
//是否提示登录   byte      1提示 0不提示，如果没有该位表示不提示
if(PackLen>offset)
{
char nflag = *(PackBody+offset);
offset += 1;
int nMark = nflag;
if(nMark == 0)
{
//返回应答包的字段个数,map数组里面的个数
CString Flag = _T("");Flag.Format(L"%d",nMark);
m_ListCtl.SetItemText(0,0,L"1");
m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
//券商数：	short（下列数目）
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
m_ListCtl.SetItemText(0, 1, Flag);//设置其它列数据
//行情地址标记		char	//1表示带券商行情地址；0表示不带券商行情地址
if(PackLen>offset)
{
char hqmark = *(PackBody+offset);
offset += 1;
int HQmark = hqmark;
CString HQflag = _T("");HQflag.Format(L"%d",HQmark);
m_ListCtl.SetItemText(0, 2, HQflag);//设置其它列数据
}
//委托是否支持活字典	char	//1表示支持活字典，0表示不支持
if(PackLen>offset)
{
char ISDict = *(PackBody+offset);
offset += 1;
int DICT = ISDict;
CString isDict = _T("");isDict.Format(L"%d",DICT);
m_ListCtl.SetItemText(0, 3, isDict);//设置其它列数据
}
//委托地址数目：short（下列数目）
if(PackLen>offset)
{
short WTnum = *(short*)(PackBody+offset);
offset += 2;
CString wtcount = _T("");wtcount.Format(L"%d",WTnum);
m_ListCtl.SetItemText(0, 4, wtcount);//设置其它列数据
//委托地址	string
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
//解委托端口	short
short WTport = *(short*)(PackBody+offset);
offset += 2;
CString wtport = _T("");wtport.Format(L"%d",WTport);
m_ListCtl.SetItemText(q,6,wtport);
}
}
}
//券商行情分配服务器地址数：short	//如果行情地址标记为0，没有该及以下字段	
if(PackLen>offset)
{
short HQnum = *(short*)(PackBody+offset);
offset += 2;
CString hqcount = _T("");hqcount.Format(L"%d",HQnum);
m_ListCtl.SetItemText(0, 7, hqcount);//设置其它列数据
//委托地址	string
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
//解委托端口	short
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
//定义公共常量变量参数
short QSnum=0;   //券商个数
short SXnum = 0;  //扩展属性数目
short ADDRnum = 0; //地址数目,地址数目和扩展属性是同一级别
if(PackLen>offset)
{
//解地址信息CRC值 short型
short CRCval = *(short*)(PackBody+offset);
offset += 2;
CString CRCfg = _T("");CRCfg.Format(L"%u",CRCval);
m_ListCtl.SetItemText(0,1,CRCfg);
//解short           券商数目
if(PackLen>offset)
{
QSnum = *(short*)(PackBody+offset);
offset += 2;
CString QSCount = _T("");QSCount.Format(L"%u",QSnum);
m_ListCtl.SetItemText(0,2,QSCount);
//解券商名称 string   ,由于下面都是券商个数的子列表，用for循环
if(PackLen>offset)
{
for(int i=0; i<QSnum; i++)
{
//解String      券商名称
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
//解char        券商名称手拼音
char QSP = *(PackBody+offset);
offset += 1;
CString zmstr = _T("");zmstr.Format(L"%c",QSP);
m_ListCtl.SetItemText(i*SXnum,4,zmstr);
}
if(PackLen>offset)
{
//解short	下列扩展属性数目
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
// char     属性序号
char AttrNo = *(PackBody+offset);
offset += 1;
int attrno = AttrNo;
CString Attr=_T("");Attr.Format(L"%d",attrno);
m_ListCtl.SetItemText(r+i*SXnum,6,Attr);

//char	属性值
char AttrVal = *(PackBody+offset);
offset += 1;
int attrval = AttrVal;
CString Attr2=_T("");Attr2.Format(L"%d",attrno);
m_ListCtl.SetItemText(r+i*SXnum,7,Attr2);
}
}
if(PackLen>offset)
{
//解short	地址数目
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
// string   地址
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
//解short    端口
if(PackLen>offset)
{
short port = *(short*)(PackBody+offset);
offset += 2;
CString addrport = _T("");addrport.Format(L"",port);
m_ListCtl.SetItemText(t+i*ADDRnum,10,addrport);
}
//ip地址运营商类别  char
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
//char     地址类型	 //1表示委托地址，2表示行情分配地址
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
//先判断请求字段是0还是其他数值
////城市营业部营业部数目
short CityCnt = 0;
int idvalue = *(int*)(SendBuffer+sizeof(HQ_HEAD));
if(idvalue == 0)     //为0表示城市索引,返回String[]     城市索引
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
//short        下面数组数目
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
//String    营业部名称
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
//String    营业部地址
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
//String    营业部电话
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
//String    营业部google搜索地址
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
//String    营业部经度
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
//String    营业部weidu
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
//解  是否通过       byte       1通过 0没通过
char chPass = *(PackBody+offset);
offset += 1;
int szch = chPass;
CString PassVal=_T("");PassVal.Format(L"%d",szch);
m_ListCtl.SetItemText(0,0,L"1");
m_ListCtl.SetItemText(0,1,PassVal);
}
if(PackLen>offset)
{
//解提示信息       String
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
//定义收包的字段
unsigned char nResult = ' ';
CString byte=_T("");
if(PackLen>offset)
{
//解同步结果       byte       1成功 0失败
nResult = *(PackBody+offset);
offset += 1;
int Rbyte = nResult;
byte.Format(L"%d",Rbyte);
m_ListCtl.SetItemText(0,0,L"1");
m_ListCtl.SetItemText(0,1,byte);
}
if(PackLen>offset)
{
//解自选代码       String[]
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
//解提示信息       String
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
//是否有效       byte       1有效 0无效
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
//是否有效       byte       1有效 0无效
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
//记录数         short
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
short Num = 0;     //记录数
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
//"代码" type="ans_string"
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
//"名称" type="ans_string"
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
//解小数点位数
unsigned char pointval = *(PackBody+offset);
offset += 1;
int PtVal = pointval;
CString PtStr=_T("");PtStr.Format(L"%d",PtVal);
m_ListCtl.SetItemText(j,4,PtStr);
}
if(PackLen>offset)
{
//解最新价
int newprice = *(int*)(PackBody+offset);
offset += 4;
CString NPStr=_T("");NPStr.Format(L"%d",newprice);
m_ListCtl.SetItemText(j,5,NPStr);
}
if(PackLen>offset)
{
//解昨收价
int YTrice = *(int*)(PackBody+offset);
offset += sizeof(int);
CString YTriceStr=_T("");YTriceStr.Format(L"%d",YTrice);
m_ListCtl.SetItemText(j,6,YTriceStr);
}
if(PackLen>offset)
{
//解成交量
int MKVal = *(int*)(PackBody+offset);
offset += 4;
CString MKValStr=_T("");MKValStr.Format(L"%d",MKVal);
m_ListCtl.SetItemText(j,7,MKValStr);
}
if(PackLen>offset)
{
//解成交额
int MKMoney = *(int*)(PackBody+offset);
offset += 4;
CString MKMoneyStr=_T("");MKMoneyStr.Format(L"%d",MKMoney);
m_ListCtl.SetItemText(j,8,MKMoneyStr);
}
if(PackLen>offset)
{
//解最高价
int hprice = *(int*)(PackBody+offset);
offset += 4;
CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
m_ListCtl.SetItemText(j,9,hpriceStr);
}
if(PackLen>offset)
{
//解最低价
int lprice = *(int*)(PackBody+offset);
offset += 4;
CString LowStr=_T("");LowStr.Format(L"%d",lprice);
m_ListCtl.SetItemText(j,10,LowStr);
}
if(PackLen>offset)
{
//有无公告
unsigned char NoticeVal = *(PackBody+offset);
offset += 1;
int IsVal = NoticeVal;
CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
m_ListCtl.SetItemText(j,11,IsValStr);
}
if(PackLen>offset)
{
//请求板块指数id" type="ans_short"
short IndexID = *(short*)(PackBody+offset);
offset += 2;
CString indexstr=_T("");indexstr.Format(L"%d",IndexID);
m_ListCtl.SetItemText(j,12,indexstr);
}
}
if(PackLen>offset)
{
//总记录数" type="ans_short
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
//"名称" type="ans_string"
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
//类型
unsigned char Type = *(PackBody+offset);
offset += 1;
int IsVal = Type;
CString IsValStr=_T("");IsValStr.Format(L"%d",IsVal);
m_ListCtl.SetItemText(0,3,IsValStr);
}
if(PackLen>offset)
{
//有小数点位数
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
//解总股本
int tval = *(int*)(PackBody+offset);
offset += 4;
CString LowStr=_T("");LowStr.Format(L"%d",tval);
m_ListCtl.SetItemText(0,6,LowStr);
}
if(PackLen>offset)
{
//解涨停价
int HPrice = *(int*)(PackBody+offset);
offset += 4;
CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
m_ListCtl.SetItemText(0,7,HPriceStr);
}
if(PackLen>offset)
{
//解最低价
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
//解昨收盘         int
int tval = *(int*)(PackBody+offset);
offset += 4;
CString LowStr=_T("");LowStr.Format(L"%d",tval);
m_ListCtl.SetItemText(0,1,LowStr);
}
if(PackLen>offset)
{
//解成交量         int
int HPrice = *(int*)(PackBody+offset);
offset += 4;
CString HPriceStr=_T("");HPriceStr.Format(L"%d",HPrice);
m_ListCtl.SetItemText(0,2,HPriceStr);
}
if(PackLen>offset)
{
//解成交额         int
int Lowprice = *(int*)(PackBody+offset);
offset += 4;
CString LowpriceStr=_T("");LowpriceStr.Format(L"%d",Lowprice);
m_ListCtl.SetItemText(0,3,LowpriceStr);
}
if(PackLen>offset)
{
//信息地雷       byte    1有 0无
unsigned char Msg = *(PackBody+offset);
offset += 1;
int Msgval = Msg;
CString MsgStr=_T("");MsgStr.Format(L"%d",Msgval);
m_ListCtl.SetItemText(0,4,MsgStr);
}
if(PackLen>offset)
{
//五星评级       byte    0-5数值
unsigned char star = *(PackBody+offset);
offset += 1;
int starval = star;
CString starStr=_T("");starStr.Format(L"%d",starval);
m_ListCtl.SetItemText(0,5,starStr);
}
if(PackLen>offset)
{
//数据位置       int         最后一根分时数据需重传
int datapos = *(int*)(PackBody+offset);
offset += 4;
CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
m_ListCtl.SetItemText(0,6,dataposStr);
}
if(PackLen>offset)
{
//记录数         short
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
//解时间           int
int timeval = *(int*)(PackBody+offset);
offset += 4;
CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
m_ListCtl.SetItemText(j,8,timevalStr);
if(PackLen>offset)
{
//解最新价         int
int NewPrice = *(int*)(PackBody+offset);
offset += 4;
CString NewPriceStr=_T("");NewPriceStr.Format(L"%d",NewPrice);
m_ListCtl.SetItemText(j,9,NewPriceStr);
}
if(PackLen>offset)
{
//解成交量         int
int CJVal = *(int*)(PackBody+offset);
offset += 4;
CString CJValStr=_T("");CJValStr.Format(L"%d",CJVal);
m_ListCtl.SetItemText(j,10,CJValStr);
}
if(PackLen>offset)
{
//解均价           int
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
//记录数         short
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
//解日期           int
int timeval = *(int*)(PackBody+offset);
offset += 4;
CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
m_ListCtl.SetItemText(j,2,timevalStr);
if(PackLen>offset)
{
//解开盘价         int
int OPrice = *(int*)(PackBody+offset);
offset += 4;
CString OPriceStr=_T("");OPriceStr.Format(L"%d",OPrice);
m_ListCtl.SetItemText(j,3,OPriceStr);
}
if(PackLen>offset)
{
//解最高价         int
int mhVal = *(int*)(PackBody+offset);
offset += 4;
CString mhValStr=_T("");mhValStr.Format(L"%d",mhVal);
m_ListCtl.SetItemText(j,4,mhValStr);
}
if(PackLen>offset)
{
//解最低价         int
int mdprice = *(int*)(PackBody+offset);
offset += 4;
CString mdpriceStr=_T("");mdpriceStr.Format(L"%d",mdprice);
m_ListCtl.SetItemText(j,5,mdpriceStr);
}
if(PackLen>offset)
{
//解收盘价         int
int Sprice = *(int*)(PackBody+offset);
offset += 4;
CString SpriceStr=_T("");SpriceStr.Format(L"%d",Sprice);
m_ListCtl.SetItemText(j,6,SpriceStr);
}
if(PackLen>offset)
{
//解成交量         int
int CJValue = *(int*)(PackBody+offset);
offset += 4;
CString CJValueStr=_T("");CJValueStr.Format(L"%d",CJValue);
m_ListCtl.SetItemText(j,7,CJValueStr);
}
if(PackLen>offset)
{
//解成交额         int
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
//最新价         int
int newprice = *(int*)(PackBody+offset);
offset += 4;
CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
m_ListCtl.SetItemText(0,1,newpriceStr);
}
if(PackLen>offset)
{
//成交量         int
int cjval = *(int*)(PackBody+offset);
offset += 4;
CString cjvalStr=_T("");cjvalStr.Format(L"%d",cjval);
m_ListCtl.SetItemText(0,2,cjvalStr);
}
if(PackLen>offset)
{
//成交额         int
int cjmoney = *(int*)(PackBody+offset);
offset += 4;
CString cjmoneyStr=_T("");cjmoneyStr.Format(L"%d",cjmoney);
m_ListCtl.SetItemText(0,3,cjmoneyStr);
}
if(PackLen>offset)
{
//最高价         int
int hprice = *(int*)(PackBody+offset);
offset += 4;
CString hpriceStr=_T("");hpriceStr.Format(L"%d",hprice);
m_ListCtl.SetItemText(0,4,hpriceStr);
}
if(PackLen>offset)
{
//最低价         int
int lprice = *(int*)(PackBody+offset);
offset += 4;
CString lpriceStr=_T("");lpriceStr.Format(L"%d",lprice);
m_ListCtl.SetItemText(0,5,lpriceStr);
}
if(PackLen>offset)
{
//开盘价         int
int oprice = *(int*)(PackBody+offset);
offset += 4;
CString opriceStr=_T("");opriceStr.Format(L"%d",oprice);
m_ListCtl.SetItemText(0,6,opriceStr);
}
if(PackLen>offset)
{
//昨收盘         int
int sprice = *(int*)(PackBody+offset);
offset += 4;
CString spriceStr=_T("");spriceStr.Format(L"%d",sprice);
m_ListCtl.SetItemText(0,7,spriceStr);
}
if(PackLen>offset)
{
//流通股本       int
int lgval = *(int*)(PackBody+offset);
offset += 4;
CString lgvalStr=_T("");lgvalStr.Format(L"%d",lgval);
m_ListCtl.SetItemText(0,8,lgvalStr);
}
if(PackLen>offset)
{
//委比		int×100
int wb = *(int*)(PackBody+offset);
offset += 4;
CString wbStr=_T("");wbStr.Format(L"%d",wb);
m_ListCtl.SetItemText(0,9,wbStr);
}
if(PackLen>offset)
{
//量比		int×10000
int lb = *(int*)(PackBody+offset);
offset += 4;
CString lbStr=_T("");lbStr.Format(L"%d",lb);
m_ListCtl.SetItemText(0,10,lbStr);
}
if(PackLen>offset)
{
//上证指数	int
int sindex = *(int*)(PackBody+offset);
offset += 4;
CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
m_ListCtl.SetItemText(0,11,sindexStr);
}
if(PackLen>offset)
{
//上证指数昨收	int
int indexspri = *(int*)(PackBody+offset);
offset += 4;
CString indexspriStr=_T("");indexspriStr.Format(L"%d",indexspri);
m_ListCtl.SetItemText(0,12,indexspriStr);
}
if(PackLen>offset)
{
//深证指数	int
int sindex = *(int*)(PackBody+offset);
offset += 4;
CString sindexStr=_T("");sindexStr.Format(L"%d",sindex);
m_ListCtl.SetItemText(0,13,sindexStr);
}
if(PackLen>offset)
{
//深证数昨收	int
int sindexsprice = *(int*)(PackBody+offset);
offset += 4;
CString sindexspriceStr=_T("");sindexspriceStr.Format(L"%d",sindexsprice);
m_ListCtl.SetItemText(0,14,sindexspriceStr);
}
if(PackLen>offset)
{
//内盘成交量	int	//对指数没有意义
int incjval = *(int*)(PackBody+offset);
offset += 4;
CString incjvalStr=_T("");incjvalStr.Format(L"%d",incjval);
m_ListCtl.SetItemText(0,15,incjvalStr);
}
if(PackLen>offset)
{
//现手		int	//对指数没有意义
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
//昨收盘         int
int zsprice = *(int*)(PackBody+offset);
offset += 4;
CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
m_ListCtl.SetItemText(0,1,zspriceStr);
}
if(PackLen>offset)
{
//记录数         short       包含买卖盘，五档行情传10，三档行情传6
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
//买卖价         int
if(PackLen>offset)
{
int bsprice = *(int*)(PackBody+offset);
offset += 4;
CString bspriceStr=_T("");bspriceStr.Format(L"%d",bsprice);
m_ListCtl.SetItemText(j,3,bspriceStr);
}
//买卖量         int
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
//昨收盘         int
int zsprice = *(int*)(PackBody+offset);
offset += 4;
CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
m_ListCtl.SetItemText(0,1,zspriceStr);
}
if(PackLen>offset)
{
//数据位置       int	下面返回的第一条记录记录号
int datapos = *(int*)(PackBody+offset);
offset += 4;
CString dataposStr=_T("");dataposStr.Format(L"%d",datapos);
m_ListCtl.SetItemText(0,2,dataposStr);
}
if(PackLen>offset)
{
//数据位置       int	下面返回的第一条记录记录号
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
//时间           int
if(PackLen>offset)
{
int timeval = *(int*)(PackBody+offset);
offset += 4;
CString timevalStr=_T("");timevalStr.Format(L"%d",timeval);
m_ListCtl.SetItemText(j,4,timevalStr);
}
//成交价         int
if(PackLen>offset)
{
int cjprice = *(int*)(PackBody+offset);
offset += 4;
CString cjpriceStr=_T("");cjpriceStr.Format(L"%d",cjprice);
m_ListCtl.SetItemText(j,5,cjpriceStr);
}
//成交量         int
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
//上涨家数       short
short szcount = *(short*)(PackBody+offset);
offset += 2;
CString szcountStr=_T("");szcountStr.Format(L"%d",szcount);
m_ListCtl.SetItemText(0,1,szcountStr);
}
if(PackLen>offset)
{
//平盘家数       short
short ppcount = *(short*)(PackBody+offset);
offset += 2;
CString ppcountStr=_T("");ppcountStr.Format(L"%d",ppcount);
m_ListCtl.SetItemText(0,2,ppcountStr);
}
if(PackLen>offset)
{
//下跌家数       short
short xdcount = *(short*)(PackBody+offset);
offset += 2;
CString xdcountStr=_T("");xdcountStr.Format(L"%d",xdcount);
m_ListCtl.SetItemText(0,3,xdcountStr);
}
if(PackLen>offset)
{
//均价		int
int avprice = *(int*)(PackBody+offset);
offset += 4;
CString avpriceStr=_T("");avpriceStr.Format(L"%d",avprice);
m_ListCtl.SetItemText(0,4,avpriceStr);
}
if(PackLen>offset)
{
//加权均价	int
int jqprice = *(int*)(PackBody+offset);
offset += 4;
CString jqpriceStr=_T("");jqpriceStr.Format(L"%d",jqprice);
m_ListCtl.SetItemText(0,5,jqpriceStr);
}
if(PackLen>offset)
{
//总流通市值	int
int zltval = *(int*)(PackBody+offset);
offset += 4;
CString zltvalStr=_T("");zltvalStr.Format(L"%d",zltval);
m_ListCtl.SetItemText(0,6,zltvalStr);
}
if(PackLen>offset)
{
//总市值	int
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
//数据位置       int	下面返回的第一条记录记录号
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
//名称	String	//名字
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
//小数位数   byte
unsigned char pointval = *(PackBody+offset);
offset += 1;
int pointvalue = pointval;
CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
m_ListCtl.SetItemText(j,3,pointvalStr);
}
if(PackLen>offset)
{
//最新价格	int
int newprice = *(int*)(PackBody+offset);
offset += 4;
CString newpriceStr=_T("");newpriceStr.Format(L"%d",newprice);
m_ListCtl.SetItemText(j,4,newpriceStr);
}
if(PackLen>offset)
{
//昨收盘	int
int zsprice = *(int*)(PackBody+offset);
offset += 4;
CString zspriceStr=_T("");zspriceStr.Format(L"%d",zsprice);
m_ListCtl.SetItemText(j,5,zspriceStr);
}
if(PackLen>offset)
{
//成交额	int	//值为0就不显示，成交额为万，客户端根据屏幕大小显示为万或者带0～2为小数的亿
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
//记录数         short
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
//代码           String
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
//名称	String	//名字
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
//小数位数   byte
unsigned char pointval = *(PackBody+offset);
offset += 1;
int pointvalue = pointval;
CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
m_ListCtl.SetItemText(j,4,pointvalStr);
}
if(PackLen>offset)
{
//净值           int
int jzvalue = *(int*)(PackBody+offset);
offset += 4;
CString jzvalueStr=_T("");jzvalueStr.Format(L"%d",jzvalue);
m_ListCtl.SetItemText(j,5,jzvalueStr);
}
if(PackLen>offset)
{
//上期净值       int
int sqjz = *(int*)(PackBody+offset);
offset += 4;
CString sqjzStr=_T("");sqjzStr.Format(L"%d",sqjz);
m_ListCtl.SetItemText(j,6,sqjzStr);
}
if(PackLen>offset)
{
//累计净值       int
int ljjz = *(int*)(PackBody+offset);
offset += 4;
CString ljjzStr=_T("");ljjzStr.Format(L"%d",ljjz);
m_ListCtl.SetItemText(j,7,ljjzStr);
}
if(PackLen>offset)
{
//截止日期       int
int enddate = *(int*)(PackBody+offset);
offset += 4;
CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
m_ListCtl.SetItemText(j,8,enddateStr);
}
}
if(PackLen>offset)
{
//总记录数       short
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
//记录数         short
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
//代码           String
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
//名称	String	//名字
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
//小数位数   byte
unsigned char pointval = *(PackBody+offset);
offset += 1;
int pointvalue = pointval;
CString pointvalStr=_T("");pointvalStr.Format(L"%d",pointvalue);
m_ListCtl.SetItemText(j,4,pointvalStr);
}
if(PackLen>offset)
{
//万份收益       int
int wfvalue = *(int*)(PackBody+offset);
offset += 4;
CString wfvalueStr=_T("");wfvalueStr.Format(L"%d",wfvalue);
m_ListCtl.SetItemText(j,5,wfvalueStr);
}
if(PackLen>offset)
{
//7日年化收益率  int
int Sevenpoint = *(int*)(PackBody+offset);
offset += 4;
CString SevenpointStr=_T("");SevenpointStr.Format(L"%d",Sevenpoint);
m_ListCtl.SetItemText(j,6,SevenpointStr);
}
if(PackLen>offset)
{
//上期万份收益   int
int sqwfsy = *(int*)(PackBody+offset);
offset += 4;
CString sqwfsyStr=_T("");sqwfsyStr.Format(L"%d",sqwfsy);
m_ListCtl.SetItemText(j,7,sqwfsyStr);
}
if(PackLen>offset)
{
//上期年化收益率 int
int sqnhsy = *(int*)(PackBody+offset);
offset += 4;
CString sqnhsyStr=_T("");sqnhsyStr.Format(L"%d",sqnhsy);
m_ListCtl.SetItemText(j,8,sqnhsyStr);
}
if(PackLen>offset)
{
//截止日期       int
int enddate = *(int*)(PackBody+offset);
offset += 4;
CString enddateStr=_T("");enddateStr.Format(L"%d",enddate);
m_ListCtl.SetItemText(j,9,enddateStr);
}
}
if(PackLen>offset)
{
//总记录数       short
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
//代码           String
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
//名称	String	//名字
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
//类型           byte
unsigned char tYPE = *(PackBody+offset);
offset += 1;
int tYPEvalue = tYPE;
CString tYPEStr=_T("");tYPEStr.Format(L"%d",tYPEvalue);
m_ListCtl.SetItemText(0,3,tYPEStr);
}
if(PackLen>offset)
{
//小数位数   byte
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
//记录数         short
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
//日期           int
int datetime = *(int*)(PackBody+offset);
offset += 4;
CString DateStr=_T("");DateStr.Format(L"%d",datetime);
m_ListCtl.SetItemText(j,2,DateStr);
}
if(PackLen>offset)
{
//净值           int
int jzval = *(int*)(PackBody+offset);
offset += 4;
CString jzvalStr=_T("");jzvalStr.Format(L"%d",jzval);
m_ListCtl.SetItemText(j,3,jzvalStr);
}
if(PackLen>offset)
{
//累计净值       int
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
//记录数         short
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
//代码           String
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
//名称	String	//名字
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
//上期持股       int
int sqcgval = *(int*)(PackBody+offset);
offset += 4;
CString sqcgvalStr=_T("");sqcgvalStr.Format(L"%d",sqcgval);
m_ListCtl.SetItemText(j,4,sqcgvalStr);
}
if(PackLen>offset)
{
//本期持股       int
int bqcgval = *(int*)(PackBody+offset);
offset += 4;
CString bqcgvalStr=_T("");bqcgvalStr.Format(L"%d",bqcgval);
m_ListCtl.SetItemText(j,5,bqcgvalStr);
}
if(PackLen>offset)
{
//占基金%        int       保留两位小数，×100转成整数
int zjjval = *(int*)(PackBody+offset);
offset += 4;
CString zjjvalStr=_T("");zjjvalStr.Format(L"%d",zjjval);
m_ListCtl.SetItemText(j,6,zjjvalStr);
}
if(PackLen>offset)
{
//占流通股本%    int       保留两位小数，×100转成整数
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
//记录数         short
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
//截止日期       int       格式如“20080331”，最新的排在最前
int datetime = *(int*)(PackBody+offset);
offset += 4;
CString DateStr=_T("");DateStr.Format(L"%d",datetime);
m_ListCtl.SetItemText(j,2,DateStr);
}
if(PackLen>offset)
{
//期初总份额     int       单位：万
int qczgbval = *(int*)(PackBody+offset);
offset += 4;
CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
m_ListCtl.SetItemText(j,3,qczgbvalStr);
}
if(PackLen>offset)
{
//本期总申购     int       单位：万
int bqzsgval = *(int*)(PackBody+offset);
offset += 4;
CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
m_ListCtl.SetItemText(j,4,bqzsgvalStr);
}
if(PackLen>offset)
{
//本期总赎回     int       单位：万
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
//记录数         short
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
//截止日期       int       格式如“20080331”，最新的排在最前
int datetime = *(int*)(PackBody+offset);
offset += 4;
CString DateStr=_T("");DateStr.Format(L"%d",datetime);
m_ListCtl.SetItemText(j,2,DateStr);
}
if(PackLen>offset)
{
//持有人户数     int
int qczgbval = *(int*)(PackBody+offset);
offset += 4;
CString qczgbvalStr=_T("");qczgbvalStr.Format(L"%d",qczgbval);
m_ListCtl.SetItemText(j,3,qczgbvalStr);
}
if(PackLen>offset)
{
//户均份额       int
int bqzsgval = *(int*)(PackBody+offset);
offset += 4;
CString bqzsgvalStr=_T("");bqzsgvalStr.Format(L"%d",bqzsgval);
m_ListCtl.SetItemText(j,4,bqzsgvalStr);
}
if(PackLen>offset)
{
//机构持有份额   int       单位：万
int bqzshval = *(int*)(PackBody+offset);
offset += 4;
CString bqzshvalStr=_T("");bqzshvalStr.Format(L"%d",bqzshval);
m_ListCtl.SetItemText(j,5,bqzshvalStr);
}
if(PackLen>offset)
{
//个人持有份额   int       单位：万
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
//记录数         short
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
//记录数         short
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
//日期           int
int datetime = *(int*)(PackBody+offset);
offset += 4;
CString DateStr=_T("");DateStr.Format(L"%d",datetime);
m_ListCtl.SetItemText(j,2,DateStr);
}
if(PackLen>offset)
{
//累计净值增长率     int
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
//记录数         short
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
//标题           String
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
//标题           String
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
//内容           String
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
//记录数         short
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
//代码           String
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
//名称           String
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
//记录数         short
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
//名称	 String
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

//总记录数       short
short tolNum = *(short*)(PackBody+offset);
offset += 2;
CString tolStr = _T("");tolStr.Format(L"%d",tolNum);

//记录数		short
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
//代码           String
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
//名称           String
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
//当期日期       int
int TodayDate = *(int*)(PackBody+offset);
offset += 4;
CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
m_ListCtl.SetItemText(j,5,TodayDateStr);
}
if(PackLen>offset)
{
//最新净值       int*10000
int zxjzval = *(int*)(PackBody+offset);
offset += 4;
CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
m_ListCtl.SetItemText(j,6,zxjzvalStr);
}
if(PackLen>offset)
{
//累计净值       int*10000 
int ljjzval = *(int*)(PackBody+offset);
offset += 4;
CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
m_ListCtl.SetItemText(j,7,ljjzvalStr);
}
if(PackLen>offset)
{
//上期日期       int  
int sqdateval = *(int*)(PackBody+offset);
offset += 4;
CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
m_ListCtl.SetItemText(j,8,sqdatevalStr);
}
if(PackLen>offset)
{
//上期净值       int*10000  
int sqval = *(int*)(PackBody+offset);
offset += 4;
CString sqvalStr=_T("");sqvalStr.Format(L"%d",sqval);
m_ListCtl.SetItemText(j,9,sqvalStr);
}
if(PackLen>offset)
{
//上期累计       int*10000  
int sqljval = *(int*)(PackBody+offset);
offset += 4;
CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
m_ListCtl.SetItemText(j,10,sqljvalStr);
}
if(PackLen>offset)
{
//日增长率       int*10000  
int rzzval = *(int*)(PackBody+offset);
offset += 4;
CString rzzvalStr=_T("");rzzvalStr.Format(L"%d",rzzval);
m_ListCtl.SetItemText(j,11,rzzvalStr);
}
if(PackLen>offset)
{
//周增长率       int*10000  
int zzzval = *(int*)(PackBody+offset);
offset += 4;
CString zzzvalStr=_T("");zzzvalStr.Format(L"%d",zzzval);
m_ListCtl.SetItemText(j,12,zzzvalStr);
}
if(PackLen>offset)
{
//月增长率       int*10000  
int mzzval = *(int*)(PackBody+offset);
offset += 4;
CString mzzvalStr=_T("");mzzvalStr.Format(L"%d",mzzval);
m_ListCtl.SetItemText(j,13,mzzvalStr);
}
if(PackLen>offset)
{
//周增长率       int*10000  
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
//代码           String
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
//名称           String
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
//当期日期       int
int TodayDate = *(int*)(PackBody+offset);
offset += 4;
CString TodayDateStr=_T("");TodayDateStr.Format(L"%d",TodayDate);
m_ListCtl.SetItemText(j,5,TodayDateStr);
}
if(PackLen>offset)
{
//万份收益       int*10000
int zxjzval = *(int*)(PackBody+offset);
offset += 4;
CString zxjzvalStr=_T("");zxjzvalStr.Format(L"%d",zxjzval);
m_ListCtl.SetItemText(j,6,zxjzvalStr);
}
if(PackLen>offset)
{
//七日收益率     int*10000 
int ljjzval = *(int*)(PackBody+offset);
offset += 4;
CString ljjzvalStr=_T("");ljjzvalStr.Format(L"%d",ljjzval);
m_ListCtl.SetItemText(j,7,ljjzvalStr);
}
if(PackLen>offset)
{
//上期日期       int  
int sqdateval = *(int*)(PackBody+offset);
offset += 4;
CString sqdatevalStr=_T("");sqdatevalStr.Format(L"%d",sqdateval);
m_ListCtl.SetItemText(j,8,sqdatevalStr);
}
if(PackLen>offset)
{
//万份收益       int*10000  
int sqljval = *(int*)(PackBody+offset);
offset += 4;
CString sqljvalStr=_T("");sqljvalStr.Format(L"%d",sqljval);
m_ListCtl.SetItemText(j,9,sqljvalStr);
}
if(PackLen>offset)
{
//七日收益率     int*10000  
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
//日期           int
int dateval = *(int*)(PackBody+offset);
offset += 4;
CString dateStr = _T("");dateStr.Format(L"%d",dateval);
m_ListCtl.SetItemText(0,1,dateStr);
if(PackLen>offset)
{
//记录数         short
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
//汇率名称       String
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
//小数位数       byte
unsigned char pointpos = *(PackBody+offset);
offset += 1;
int pointwz = pointpos;
CString pointstr = _T("");pointstr.Format(L"%d",pointwz);
m_ListCtl.SetItemText(j,4,pointstr);
}
if(PackLen>offset)
{
//汇率数值       int
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
//下一ID         int         下一条的ID，没有下一条返回0

if(PackLen>offset)
{
int nIdval = *(int*)(PackBody+offset);
offset += 4;
CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
m_ListCtl.SetItemText(0,1,IdStr);
}
//内容           String
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
// Url地址        String, 格式: ip:port:id,客户端根据平台生成url;该接口仅仅用于广发证券
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
//ID            int         服务器端最新信息的id，如果没有返回0
if(PackLen>offset)
{
int nIdval = *(int*)(PackBody+offset);
offset += 4;
CString IdStr=_T("");IdStr.Format(L"%d",nIdval);
m_ListCtl.SetItemText(0,1,IdStr);
}
//内容          String
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
//该类信息总数  short
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
//开始位置      short	//下列资讯第一条编号
if(PackLen>offset)
{
short beginpos = *(short*)(PackBody+offset);
offset += 2;
CString beginStr = _T("");beginStr.Format(L"%d",beginpos);
m_ListCtl.SetItemText(0,2,beginStr);
}
for(int j=0; j<RecdNum; j++)
{
//内容          String
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

//该类信息总数  short
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
//分类ID         int
if(PackLen>offset)
{
int TypeId = *(int*)(PackBody+offset);
offset += 2;
CString TypeIdstr = _T("");TypeIdstr.Format(L"%d",TypeId);
m_ListCtl.SetItemText(j,2,TypeIdstr);
}
//内容          String
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
//该类信息总数  short
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
//标题           String
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
//该类信息总数  short
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
//标题           String
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
//是否有子类     byte    0没有子菜单，1有;
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
//标题           String
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
//内容           String
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
*/