#ifndef DLG_INSURE_GAME_HEAD_FILE
#define DLG_INSURE_GAME_HEAD_FILE

#pragma once

#include "GameFrameHead.h"


class CNumberConvert
{
public:
	CNumberConvert();
	~CNumberConvert();

public:
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);
	SCORE GetUserControlScore(LPCTSTR pszScore);
	//ת���ַ�
	VOID SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize);
	//ת���ַ�
	VOID NumberConvert(TCHAR* szNum, TCHAR szResult[], int nLen);

};

///////////////////////////////////////////////////////////////////////////////////////////////////
//��Ϸ����
class CDlgInsureGame : public CFGuiDialog
{
	//��ʶ����
public:
	bool							m_bSwitchIng;					//ת����ʶ	
	bool							m_blCanStore;                   //����ʶ
	bool                            m_blCanGetOnlyFree;             //ȡ���ʶ

	//������Ϣ
public:
	WORD							m_wRevenueTake;					//˰�ձ���
	WORD							m_wRevenueTransfer;				//˰�ձ���
	SCORE							m_lUserScore;					//�û���Ϸ��
	SCORE							m_lUserInsure;					//������Ϸ��
	
	//����ؼ�
protected:
	CEditUI *						m_pEditAmount;					//�����༭	
	CEditUI *						m_pEditPassword;				//����༭
	CTextUI *						m_pTextForget;					//��������

	//����ؼ�
protected:
	CButtonUI *						m_pButtonSave;					//��ť
	CButtonUI *						m_pButtonTake;					//ȡ�ť
	CButtonUI *						m_pButtonRefresh;				//ˢ�°�ť
	CEditUI*						m_pEditInOut;
	CEditUI*						m_pEditPassWord;
	CLabelUI*						m_LabelInOut;

	SCORE			m_InOutScore;		//����ķ���
	//�ӿڱ���
protected:	
	IClientUserItem	*				m_pIMeUserData;					//�û���Ϣ
	IClientKernel *					m_pIClientKernel;				//�ں˽ӿ�

	//��������
public:
	//���캯��
	CDlgInsureGame();
	//��������
	virtual ~CDlgInsureGame();
	//���غ���
public:		
	//ȷ����Ϣ
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();	
	//��ʼ������
	virtual BOOL OnInitDialog();
    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//��������
public:

	//��������
	VOID LoadDataFromUI();

	//����ٿ�
	VOID UpdateUI();

	//��ʾ��д
	VOID ShowCapital();
	VOID SetAccessEditNum( SCORE  dwNum);
	VOID OnBnClickAccess(TNotifyUI& msg);
	//��������
public:

	//ת���ַ�
	VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);

	//�ؼ�����
protected:

	//��ť
	VOID OnBnClickedSaveScore();
	//ȡ�ť
	VOID OnBnClickedTakeScore();
	//ˢ�°�ť
	VOID OnBnClickedRefreshScore();
	//��������
	VOID OnBnClickedForgetInsure();

	//�ӿں���
public:
	//��ʾ����
	void ShowItem();
	//����λ��
	void SetPostPoint(CPoint Point);

	//���ܺ���
public:
	//�������
	void SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserDatas);
	//������Ϣ
	void SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize);

};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif