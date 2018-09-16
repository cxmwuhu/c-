#include "StdAfx.h"
#include "LoadXml.h"



//�Զ���ascii��utf8ת������;
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


// ����xml���������ļ�
bool CLoadXml::DictXml(char * XmlName)
{
	//��ȡxml
	TiXmlDocument *xmlfile = new TiXmlDocument;
    bool xmlret = xmlfile->LoadFile(XmlName);
	if(!xmlret)
	{
		printf("xml�ļ�����ʧ��\n");
		return false;
	}
	//��ȡ��Ԫ��hqserver
	TiXmlElement *pRootElement = xmlfile->RootElement();
	if(pRootElement == 0)
	{
		printf("����hqserver���ڵ�ʧ��\n");
		return false;
	}
	//���ҵ�һ�����ܽڵ�func
	TiXmlElement *pFunElement = pRootElement->FirstChildElement();
	//��ȡ���ܽڵ���������
	TiXmlAttribute *pFunID;
	TiXmlAttribute *pFunName;
	DWORD iNum = 0;
	ComFun g_Fun;
	DictFun g_Dict;
	DictFun g_AnsDict;     //����Ӧ��ṹ����ֵ�ֵ
	while(1)
	{
		//��ȡ����ID������
		pFunID = pFunElement->FirstAttribute();
		//��ȡ�������Ƶ�����
		pFunName = pFunID->Next();
		//��ȡ����id��ֵ
		g_Fun.FunID = pFunID->IntValue();
		//����xmlȡ��ֵ��ascii��ʽ��ת�ɶ��ֽ�
		ASCToUtf(pFunName->Value(),g_Fun.FunName);

		TiXmlElement *pReqElement=pFunElement->FirstChildElement();  //�˽ڵ��ǲ���req�ֶ�
		//���幦��ID����������;
		TiXmlAttribute *pReqname;
		TiXmlAttribute *pReqtype;
		//�����ֶνڵ�;
		TiXmlElement *pFieldElement;
		//ѭ����ȡÿ�������µ������ֶ�
		//���û������ڵ㣬Ҫ�����ж�,��ֹ������ѭ���쳣;
		if(FALSE == pReqElement->NoChildren())
		{
			//��ȡreq�ڵ�����Ľڵ�
			pFieldElement = pReqElement->FirstChildElement();
			while(true)
			{
				pReqname = pFieldElement->FirstAttribute();
				pReqtype = pReqname->Next();
				ASCToUtf(pReqname->Value(),g_Dict.Dictname);
				ASCToUtf(pReqtype->Value(),g_Dict.Dicttype);
				//���ֵ�ӵ�����vector��������
				g_Fun.IDList.push_back(g_Dict);
				//��Ԫ��ֵ�������ܽڵ�����棬ѭ����������Ĺ��ܽڵ�ֵ
				pFieldElement = pFieldElement->NextSiblingElement();
				if(pFieldElement == NULL)
				{
					break;
				}
			}
		}
		
		TiXmlElement *pAnsesElement=pReqElement->NextSiblingElement();  //�˽ڵ��ǲ���Anses�ֶ�
		//��ȡ���ܽڵ���������
	    TiXmlAttribute *pAnsFlag;

		//��ȡanses�ڵ������ans�ڵ�,ans�ӽڵ�һ��Ҫ��whileѭ�����棬����NextSligleElementԪ�غ���ֵ����ԭ�������ݣ���������һֱ�ظ�
		TiXmlElement *pAnsElement = pAnsesElement->FirstChildElement();
		//ѭ����ȡÿ�������µ������ֶ�
		//���û������ڵ㣬Ҫ�����ж�,��ֹ������ѭ���쳣;
		while(true)
		{
			
			//��ȡAns flag������
		    pAnsFlag = pAnsElement->FirstAttribute();
			int Flag = pAnsFlag->IntValue();      //��ȡflag��־
			TiXmlElement *pAnsFiledElement = pAnsElement->FirstChildElement();
			//����ansӦ���ֶ����ͺ�����
			TiXmlAttribute *pAnsName;
			TiXmlAttribute *pAnsType;
			if(pAnsFiledElement != NULL)
			{
			while(true)
			{
				//��ȡӦ���ֵ������Name����
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