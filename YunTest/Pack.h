#include "YunTestDlg.h"
#pragma once
class CPack
{
public:
	CPack(void);
	~CPack(void);
	CYunTestDlg dlgtest;
	// ½â°üº¯Êý
	void CDataPack(char * &Datapack, unsigned short PackLen, unsigned short PackType);
};

