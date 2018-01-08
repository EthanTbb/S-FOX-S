#ifndef DLG_GAME_OPTION_HEAD_FILE
#define DLG_GAME_OPTION_HEAD_FILE

#pragma once

#include "GameOptionItem.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////


//��Ϸ����
class GAME_FRAME_CLASS CDlgGameOption : public CFGuiDialog
{
	//��������
public:
	//���캯��
	CDlgGameOption();
	//��������
	virtual ~CDlgGameOption();

	//���ܺ���
public:
	//��Ϸ����
	bool CreateGameOption(IUnknownEx * pIUnknownEx[], WORD wItemCount);

	//MFC����
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGUI����
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//�û�����
public:

	//��������
	VOID InitData();

	VOID LoadDataFromMemory();

	VOID LoadDataFromUI();

	VOID UpdateData();

	//����ٿ�
	VOID InitUI();

	VOID UpdateUI();

	//�����߼�
	VOID OnDefault();

	VOID DoApply();

public:
	//��������
	bool                            m_bMuteStatuts;                     //����״̬
	bool							m_bAllowSound;						//��������
	bool							m_bAllowBackGroundSound;			//����������
	bool							m_bFullGameView;					//ȫ��ģʽ
	bool							m_bNotifyUserInOut;					//������Ϣ
	bool							m_bDebarDetestChat;					//������Ϣ
	bool							m_bAllowLookon;						//�����Թ�
	bool							m_bAllowFriendLookon;				//�����Թ�
	//Ĭ������
	bool                            m_bDefaultMuteStatuts;                     //����״̬
	bool							m_bDefaultAllowSound;						//��������
	bool							m_bDefaultAllowBackGroundSound;			//����������
	bool							m_bDefaultFullGameView;					//ȫ��ģʽ
	bool							m_bDefaultNotifyUserInOut;					//������Ϣ
	bool							m_bDefaultDebarDetestChat;					//������Ϣ
	bool							m_bDefaultAllowLookon;						//�����Թ�
	bool							m_bDefaultAllowFriendLookon;				//�����Թ�

};

//////////////////////////////////////////////////////////////////////////

#endif