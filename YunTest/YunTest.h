
// YunTest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif
#define HEARTID    3000
#define TUISONGID    3001
#define STRESS_DATA  5000

//��������Ԥ�������¼�ID
#define AlarmID305      6000

//��������Ԥ�������¼�ID
#define NOTICE102      8000

#include "resource.h"		// ������


// CYunTestApp:
// �йش����ʵ�֣������ YunTest.cpp
//

//����ÿ�����ܵ����ͺ�����

//����ÿ�������µ��ֵ�����ṹ��
struct DictFun
{
	TCHAR Dictname[50];   //ÿ�������ֶε�����
	TCHAR Dicttype[50];   //ÿ�������ֶε����ͣ������Ժ�����ͽ��ʹ��
};

//���幦�ܺ��µ���������
struct ComFun
{
	int FunID;       //���幦�ܺ�
	TCHAR FunName[50];       //���幦������
	vector<DictFun> IDList;    //����ÿ��������������������ֶ�
	//����ÿ�������µ�Ans�б�
	map<int,vector<DictFun>> AnsList;  
};

//����Ӧ������ֵ�
struct AnsFun
{
	int flag;                    //Ӧ����
	vector<DictFun> AnsList;     //Ӧ�������������ֵ��б�
};

//����ѹ����ؽṹ
struct IODATA
{
	char strdata[1024*10];
	int datalen;
};

//����ѹ����ؽṹ
struct FUNNO
{
	int Funid;
	CString Funname;
};

//����Ӧ���ܺŶ�ӦӦ�����ֵ�

//����������������������
struct TSHEART
{
	char HeartBuff[10*1024];          //��������������
	int heartlen;                     //��������������
	TSHEART(){ZeroMemory(HeartBuff,10*1024);heartlen=0;}
};

//����102������������������
struct NOTICEHEART
{
	char NoticeBuff[10*1024];          //��������������
	int noticelen;                     //��������������
	NOTICEHEART(){ZeroMemory(NoticeBuff,10*1024);noticelen=0;}
};


class CYunTestApp : public CWinApp
{
public:
	CYunTestApp();
	int socknum;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CYunTestApp theApp;