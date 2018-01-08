#ifndef DLG_TASKDETAILS_HEAD_FILE
#define DLG_TASKDETAILS_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//��������
class CDlgTaskDetails  : public CFGuiDialog
{
protected:
	tagTaskStatus *							m_pTaskStatus;						//����״̬	
	tagTaskParameter *						m_pTaskParameter;					//�������	
	//��������
public:
	//���캯��
	CDlgTaskDetails ();
	//��������
	virtual ~CDlgTaskDetails ();

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
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	
	//���غ���
public:
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

public:
	//���ò���
	void SetTaskParameter(tagTaskParameter * pTaskParameter,tagTaskStatus * pTaskStatus);

	//ˢ�¿ؼ�
	void UpdateDetails();

};

//////////////////////////////////////////////////////////////////////////////////

#endif