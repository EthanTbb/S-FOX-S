#ifndef DLG_ADD_GAME_HEAD_FILE
#define DLG_ADD_GAME_HEAD_FILE

#pragma once
//////////////////////////////////////////////////////////////////////////////////
#include "ServerListData.h"

typedef vector<CCheckButtonUI*> VECheckButtonList;	//һ���ؼ���

//////////////////////////////////////////////////////////////////////////
//�����Ϸ
class CDlgAddGame : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgAddGame();
	//��������
	virtual ~CDlgAddGame();

	//MFC����
protected:
	//��ʼ����
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGui����
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//��Ϣ����
public:
	//ȫѡ��Ϣ
	void OnNotifyAllSelect(TNotifyUI &  msg);
	//ѡ����Ϣ
	void OnNotifySelect(TNotifyUI &  msg);
	//�û�����
public:
	//��������
	void LoadDataFromExternal(vector<CGameKindItem *> & vMyGameKind);

	//ˢ������
	void UpdateDataToExternal(vector<CGameKindItem *> & vMyGameKind);

	//��������
	void LoadDataFromMemory();

	//����ҳ��
	void CreateUI();

	//��������
	void LoadDataFromUI();

	//ˢ�½���
	void UpdateUI();

	//��������
private:
	//��������
	inline void GetProcessName(CGameKindItem *vMyGameKind,TCHAR szProcessName[LEN_PROCESS]);

protected:
	vector<CGameTypeItem *> m_vGameType;
	vector<CGameKindItem *> m_vGameKind;
    vector<CGameKindItem *> m_vMyGameKind;
	vector<VECheckButtonList> m_vecCheckbutton;
	vector<CCheckButtonUI*> m_vecAllGameCheckList;
};

//////////////////////////////////////////////////////////////////////////////////

#endif