#include "YunTestDlg.h"
#pragma once
class CPack
{
public:
	CPack(void);
	~CPack(void);
	CYunTestDlg dlgtest;
	// �������
	void CDataPack(char * &Datapack, unsigned short PackLen, unsigned short PackType);
};

