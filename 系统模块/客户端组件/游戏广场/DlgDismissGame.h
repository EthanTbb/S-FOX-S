#ifndef DLG_DISMISS_GAME_HEAD
#define DLG_DISMISS_GAME_HEAD
#pragma once

///////////////////////////////////////////////////////////////////////////////////////////
//��ɢ��Ϸ
class CDlgDismissGame : public CSkinDialog
{
	//��������
protected:
	WORD							m_wDismissTable;					//�߳���ʾ

	//���Ʊ���
protected:
	CSkinButton						m_btOK;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť

	//��������
public:
	//���캯��
	CDlgDismissGame(CWnd* pParent = NULL);
	//��������
	virtual ~CDlgDismissGame();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//���ݽ���
	virtual void DoDataExchange(CDataExchange* pDX);
	
	//���ܺ���
public:
	//��ȡ����
	WORD GetDismissTableNum(){ return m_wDismissTable; }

	//��Ϣ����
public:
	//ȷ�ϰ�ť
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()	
};
///////////////////////////////////////////////////////////////////////////////////////////
#endif

