// Duilib.h : Duilib DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CDuilibApp
// �йش���ʵ�ֵ���Ϣ������� Duilib.cpp
//

class CDuilibApp : public CWinApp
{
public:
	CDuilibApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
