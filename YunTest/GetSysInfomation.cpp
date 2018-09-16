#include "StdAfx.h"
#include "GetSysInfomation.h"


CGetSysInfomation::CGetSysInfomation(void)
{
	GetSystemTimes( &m_preidleTime, &m_prekernelTime, &m_preuserTime);
}


CGetSysInfomation::~CGetSysInfomation(void)
{
}

__int64 CompareFileTime ( FILETIME time1, FILETIME time2 )
{
    __int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;
    __int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;

    return   (b - a);
}

// CPUʹ����
double CGetSysInfomation::CpuUseage()
{
	FILETIME idleTime;
    FILETIME kernelTime;
    FILETIME userTime;
    GetSystemTimes( &idleTime, &kernelTime, &userTime );


    __int64 idle = CompareFileTime( m_preidleTime,idleTime);
    __int64 kernel = CompareFileTime( m_prekernelTime, kernelTime);
    __int64 user = CompareFileTime(m_preuserTime, userTime);


    if(kernel+user == 0)
		return 0.0;
    //���ܵ�ʱ��-����ʱ�䣩/�ܵ�ʱ��=ռ��cpu��ʱ�����ʹ����
    double cpu = (kernel +user - idle) *100/(kernel+user);

    m_preidleTime = idleTime;
    m_prekernelTime = kernelTime;
    m_preuserTime = userTime ;
    return cpu;
}
