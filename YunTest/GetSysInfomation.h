#include <Windows.h>

#pragma once
class CGetSysInfomation
{
public:
	CGetSysInfomation(void);
	~CGetSysInfomation(void);


private:
    FILETIME m_preidleTime;
    FILETIME m_prekernelTime;
    FILETIME m_preuserTime;
public:
	// CPU π”√¬ 
	double CpuUseage();
};

