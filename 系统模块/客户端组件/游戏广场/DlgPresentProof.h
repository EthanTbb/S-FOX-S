#ifndef DLG_DLGPRESENT_PROOF_HEAD_FILE
#define DLG_DLGPRESENT_PROOF_HEAD_FILE


#pragma once

//#include "InsureCommon.h"

struct tagTransferScoreInfo
{
	DWORD							dwTargetID;							//�û� I D
	SCORE							lTransferScore;						//ת�˽��
	TCHAR							szTargetAccounts[LEN_NICKNAME];		//Ŀ���û�
	TCHAR							szTransRemark[LEN_TRANS_REMARK];	//ת�˱�ע
};

class CDlgPresentProof: public CFGuiDialog
{
public:
	//��������
public:
	//���캯��
	CDlgPresentProof();
	//��������
	virtual ~CDlgPresentProof();

	//���غ���
protected:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);

	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//��������
public:

	//���ܺ���
public:
	//ת����Ϣ
	void SetTransferScoreInfo(tagTransferScoreInfo TransferScoreInfo);
	BOOL SaveBmp(HBITMAP hBitmap, CString FileName);
	//ת����Ϣ
protected:
	tagTransferScoreInfo						m_tagTransferScoreInfo;
};

#endif