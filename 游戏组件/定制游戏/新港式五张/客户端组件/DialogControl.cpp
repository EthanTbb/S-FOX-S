// DialogControl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "resource.h"
#include "DialogControl.h"


// CDialogControl �Ի���

IMPLEMENT_DYNAMIC(CDialogControl, CDialog)
CDialogControl::CDialogControl(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogControl::IDD, pParent)
{
}

CDialogControl::~CDialogControl()
{
}

void CDialgControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogControl, CDialog)
END_MESSAGE_MAP()


// CDialogControl ��Ϣ�������
