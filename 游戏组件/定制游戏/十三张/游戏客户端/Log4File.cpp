#include "StdAfx.h"
#include ".\log4file.h"

CLog4CFile::CLog4CFile(void)
{
}

CLog4CFile::~CLog4CFile(void)
{
}

void CLog4CFile::WriteLog( LPTSTR lpszFlieName, LPTSTR lpszFormat, ... )
{

	TCHAR szExePath[MAX_PATH] = {};
	GetModuleFileName( NULL,szExePath,CountArray(szExePath) );

	CString szTemp;
	szTemp = szExePath;
	int nPos = szTemp.ReverseFind(TEXT('\\'));
	_sntprintf(szExePath, CountArray(szExePath), TEXT("%s%s"), szTemp.Left(nPos + 1), lpszFlieName);
	
	CFile file;
	file.Open(szExePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeReadWrite);
	file.SeekToEnd();

	TCHAR szTimerBuftter[20];
	time_t now;
	time(&now);
	_tcsftime(szTimerBuftter, 20, TEXT("%Y-%m-%d %H:%M:%S"), localtime(&now));
	
	va_list args;
	int		nBuf;
	WCHAR	szBuffer[1024];
	WCHAR	szFormat[1024];
	va_start(args, lpszFormat);
	  
	nBuf = _vsnwprintf(szBuffer, sizeof(szBuffer)-sizeof(LPWSTR), CT2CW(lpszFormat), args);
	swprintf(szFormat, L"[Log4c] [%s] :%s \r\n", szTimerBuftter, szBuffer);
	
	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, szFormat, wcslen(szFormat), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + 1];
	::WideCharToMultiByte(CP_ACP, NULL, szFormat, wcslen(szFormat), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = '\0';

	file.Write(szAnsi, strlen(szAnsi));
	
	va_end(args);

	file.Flush();
	file.Close();
	
	delete[] szAnsi;
	szAnsi = NULL;

}

