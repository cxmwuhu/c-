// dehexport.h: interface for the dehexport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEHEXPORT_H__E8ADE811_6DA9_44A9_82F9_5672E81157B3__INCLUDED_)
#define AFX_DEHEXPORT_H__E8ADE811_6DA9_44A9_82F9_5672E81157B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



/*
 Debugging and Error Handling Helper Function
*/

#ifdef DEH_EXPORT
#define DEH_API extern "C" __declspec(dllexport)
#else
#define DEH_API extern "C" __declspec(dllimport)
#endif


struct DEH_BUFFER
{
	char* buf;
	int len;
};



DEH_API bool DEH_GetCallStack(DEH_BUFFER* outbuf);
DEH_API bool DEH_GetModuleList(DEH_BUFFER* outbuf);
DEH_API void DEH_EnableCrashLog();
DEH_API void DEH_Free(DEH_BUFFER* inbuf);






















#endif // !defined(AFX_DEHEXPORT_H__E8ADE811_6DA9_44A9_82F9_5672E81157B3__INCLUDED_)
