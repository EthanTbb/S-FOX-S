#ifndef DLG_CAMERA_HEAD_FILE
#define DLG_CAMERA_HEAD_FILE

#pragma once

#include "AvatarControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//����ͷ��
class AVATAR_CONTROL_CLASS CCameraControl : public CFGuiDialog
{
	//��Ԫ����
	friend class CFaceItemCustomWnd;

	//��������
protected:
	bool							m_bInit;							//���ñ�ʶ
	HWND							m_hWndCamera;						//��ʾ����
	CImage							m_ImageResult;						//���ͼ��
	
	//�豸��Ϣ
protected:
	CAPSTATUS						m_CapStatus;						//��ǰ״̬
	CAPTUREPARMS					m_CapTureparms;						//��׽����
	CAPDRIVERCAPS					m_CapDrivercaps;					//��������

	//����ؼ�
protected:
	CLabelUI *						m_pLabelText;						//�ı���ǩ

	//����ؼ�
protected:
	CButtonUI *						m_pbtOk;							//����༭
	CButtonUI *						m_pbtRepeat;						//��������
	CButtonUI *						m_pbtCameraDict;					//����֡

	//�����ؼ�
protected:
	CStatic							m_CameraControl;					//��Ƶ����

	//��������
public:
	//���캯��
	CCameraControl(CWnd * pParent);
	//��������
	virtual ~CCameraControl();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//���ú���
	virtual BOOL OnInitDialog();

	//�ӿں���
public:
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgCamera.xml"); } 
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	
	//��Ϣ����
protected:
	//��������
	VOID OnBnClickedRepeat();
	//����ͼƬ
	VOID OnBnClickedCameraDict();
};

//////////////////////////////////////////////////////////////////////////////////

#endif