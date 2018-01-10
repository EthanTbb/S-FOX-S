#ifndef _CLOG4CFLIE_H_
#define _CLOG4CFLIE_H_

#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <windows.h>


class CLog4CFile
{
public:
	CLog4CFile(void);
	~CLog4CFile(void);
public:
	static void WriteLog(LPTSTR lpszFlieName, LPTSTR lpszFormat, ...);
};
#endif