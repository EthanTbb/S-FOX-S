#ifndef DLG_DLGMBCARD_HEAD_FILE
#define DLG_DLGMBCARD_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//�ܱ�����
class CDLGMBCard : public CFGuiDialog
{
public:
	UINT								m_uMBCardID;						//��������
	CHAR								m_szRandLetter[3];
	BYTE								m_szRandNumber[3];
	BYTE								m_cbErrorCount;
	UINT								m_uCoorNumber[6][4];

	//��������
public:
	CDLGMBCard(); 
	virtual ~CDLGMBCard();

	//���غ���
public:
	//������Ϣ
	virtual BOOL OnInitDialog();
	//��ť��Ϣ
	void OnOK();
	//ȡ����Ϣ
	void OnCancel();
	//�ӿں���
public:
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgMBCard.xml"); } 
	//��������
public:
	void RandCoordinate();
	//�����ܱ�
	void SetMBCard(UINT uMBCardID);
};

//////////////////////////////////////////////////////////////////////////////////

#endif