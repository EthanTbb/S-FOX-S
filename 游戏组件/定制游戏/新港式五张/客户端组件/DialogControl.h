#pragma once


// ��ҿ��ƶԻ��� �Ի���

class CDialogControl : public CDialog
{
	DECLARE_DYNAMIC(CDialogControl)

public:
	CDialogControl(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogControl();

// �Ի�������
	enum { IDD = IDD_DIALOG_CONRTOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
