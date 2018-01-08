#ifndef DLG_SEARCH_TABLE_HEAD_FILE
#define DLG_SEARCH_TABLE_HEAD_FILE
#pragma once

//////////////////////////////////////////////////////////////////////////
//��˵��
class CTableViewFrame;
class CPlazaViewServer;

//////////////////////////////////////////////////////////////////////////

//��������
class CDlgSearchTable : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgSearchTable();
	//��������
	virtual ~CDlgSearchTable();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ������
	virtual VOID OnCancel();

	//FGui����
public:
	//��ʼ����
	virtual void InitControlUI();
	//Ƥ������
	virtual LPCTSTR GetSkinFile();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���ܺ���
public:
	//��������
	VOID SetTableViewFrame(ITableViewFrame * pITableViewFrame);
	//���÷���
	VOID SetServerViewItem(CPlazaViewServer * pServerViewItem);

	//�û�����
protected:
	//���ټ���
	VOID OnBnClickedAutoJoin();

	//�ӿڱ���
protected:
	ITableViewFrame *				m_pITableViewFrame;					//���ӽӿ�
	CPlazaViewServer *				m_pServerViewItem;					//��Ϸ����
	WORD							m_wResultTableID;					//�������
};

//////////////////////////////////////////////////////////////////////////

#endif