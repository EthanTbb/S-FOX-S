#pragma once

//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	CUserBetArray					m_UserBetArray;					//�û���ע
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���¿���
	virtual void  OnAllowControl(bool bEnable) = NULL;
	//������
	virtual bool  ReqResult(const void * pBuffer) = NULL;
	//������ע
	virtual void __cdecl UpdateUserBet(bool bReSet) = NULL;
	//���¿��
	virtual bool __cdecl UpdateStorage(const void * pBuffer) = NULL;
	//������Ӯ
	virtual bool __cdecl UpdateControlWin(const void * pBuffer) = NULL;
	//���¿ؼ�
	virtual void __cdecl UpdateControl() = NULL;
};
