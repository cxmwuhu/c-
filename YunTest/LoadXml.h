#include "YunTest.h"
#include "tinyxml\tinyxml.h"

#pragma once
class CLoadXml
{
public:
	CLoadXml(void);
	~CLoadXml(void);
	// 加载xml功能配置文件
	bool DictXml(char * XmlName);
	bool DictTSXml(char * XmlName);
public:
	DictFun g_Dict;
	ComFun g_Fun;
	vector<DictFun> XmlDict;
	vector<ComFun> XmlFun;
	vector<DictFun> AnsDict;
	map<int,vector<DictFun>> answer;
	map<int,map<int,vector<DictFun>>> AnsData;

	//推送信息
	vector<DictFun> TSXmlDict;
	vector<ComFun> TSXmlFun;
	vector<DictFun> TSAnsDict;
	map<int,vector<DictFun>> TSanswer;
	map<int,map<int,vector<DictFun>>> TSAnsData;
};

