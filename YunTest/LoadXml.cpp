#include "StdAfx.h"
#include "LoadXml.h"



//自定义ascii到utf8转换函数;
void ASCToUtf(LPCSTR srcStr,LPWSTR dstStr)
{
	DWORD dwNum = MultiByteToWideChar (CP_UTF8, 0, srcStr, -1, NULL, 0);
	MultiByteToWideChar (CP_UTF8, 0,srcStr, -1, dstStr, dwNum);
}

CLoadXml::CLoadXml(void)
{
}


CLoadXml::~CLoadXml(void)
{
}


// 加载xml功能配置文件
bool CLoadXml::DictXml(char * XmlName)
{
	//读取xml
	TiXmlDocument *xmlfile = new TiXmlDocument;
    bool xmlret = xmlfile->LoadFile(XmlName);
	if(!xmlret)
	{
		printf("xml文件加载失败\n");
		return false;
	}
	//读取根元素hqserver
	TiXmlElement *pRootElement = xmlfile->RootElement();
	if(pRootElement == 0)
	{
		printf("加载hqserver根节点失败\n");
		return false;
	}
	//查找第一个功能节点func
	TiXmlElement *pFunElement = pRootElement->FirstChildElement();
	//读取功能节点属性数据
	TiXmlAttribute *pFunID;
	TiXmlAttribute *pFunName;
	DWORD iNum = 0;
	ComFun g_Fun;
	DictFun g_Dict;
	DictFun g_AnsDict;     //定义应答结构体的字典值
	while(1)
	{
		//获取功能ID的属性
		pFunID = pFunElement->FirstAttribute();
		//获取功能名称的属性
		pFunName = pFunID->Next();
		//获取功能id的值
		g_Fun.FunID = pFunID->IntValue();
		//由于xml取的值是ascii格式，转成多字节
		ASCToUtf(pFunName->Value(),g_Fun.FunName);

		TiXmlElement *pReqElement=pFunElement->FirstChildElement();  //此节点是查找req字段
		//定义功能ID和名字属性;
		TiXmlAttribute *pReqname;
		TiXmlAttribute *pReqtype;
		//定义字段节点;
		TiXmlElement *pFieldElement;
		//循环读取每个功能下的请求字段
		//如果没有请求节点，要进行判断,防止出现死循环异常;
		if(FALSE == pReqElement->NoChildren())
		{
			//获取req节点下面的节点
			pFieldElement = pReqElement->FirstChildElement();
			while(true)
			{
				pReqname = pFieldElement->FirstAttribute();
				pReqtype = pReqname->Next();
				ASCToUtf(pReqname->Value(),g_Dict.Dictname);
				ASCToUtf(pReqtype->Value(),g_Dict.Dicttype);
				//把字典加到功能vector向量里面
				g_Fun.IDList.push_back(g_Dict);
				//把元素值调到功能节点的下面，循环查找下面的功能节点值
				pFieldElement = pFieldElement->NextSiblingElement();
				if(pFieldElement == NULL)
				{
					break;
				}
			}
		}
		
		TiXmlElement *pAnsesElement=pReqElement->NextSiblingElement();  //此节点是查找Anses字段
		//读取功能节点属性数据
	    TiXmlAttribute *pAnsFlag;

		//获取anses节点下面的ans节点,ans子节点一定要在while循环外面，否则NextSligleElement元素后，数值还是原来的数据，导致数据一直重复
		TiXmlElement *pAnsElement = pAnsesElement->FirstChildElement();
		//循环读取每个功能下的请求字段
		//如果没有请求节点，要进行判断,防止出现死循环异常;
		while(true)
		{
			
			//获取Ans flag的属性
		    pAnsFlag = pAnsElement->FirstAttribute();
			int Flag = pAnsFlag->IntValue();      //获取flag标志
			TiXmlElement *pAnsFiledElement = pAnsElement->FirstChildElement();
			//定义ans应答字段类型和名称
			TiXmlAttribute *pAnsName;
			TiXmlAttribute *pAnsType;
			if(pAnsFiledElement != NULL)
			{
			while(true)
			{
				//获取应答字典里面的Name属性
				pAnsName = pAnsFiledElement->FirstAttribute();
				pAnsType = pAnsName->Next();
				ASCToUtf(pAnsName->Value(),g_AnsDict.Dictname);
				ASCToUtf(pAnsType->Value(),g_AnsDict.Dicttype);
				AnsDict.push_back(g_AnsDict);
				
				pAnsFiledElement = pAnsFiledElement->NextSiblingElement();
				if(pAnsFiledElement == NULL)
				{
					break;
				}
			}
			}
			g_Fun.AnsList.insert(map<int,vector<DictFun>>::value_type(Flag,AnsDict));
			answer.insert(map<int,vector<DictFun>>::value_type(Flag,AnsDict));
			
			AnsDict.clear();
			
			pAnsElement = pAnsElement->NextSiblingElement();
			if(pAnsElement == NULL)
			{
				break;
			}
		}
		AnsData.insert(map<int,map<int,vector<DictFun>>>::value_type(g_Fun.FunID,answer));
		answer.clear();
		XmlFun.push_back(g_Fun);
		g_Fun.IDList.clear();
		pFunElement = pFunElement->NextSiblingElement();
		if(pFunElement == NULL)
		{
			break;
		}
	}
	return true;
}