#ifndef FACE_CUSTOM_CONTROL_HEAD_FILE
#define FACE_CUSTOM_CONTROL_HEAD_FILE

#pragma once

#include "FaceItemControl.h"
#include "AvatarControlHead.h"
#include "ImageEditorControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�Զ�ͷ��
class AVATAR_CONTROL_CLASS CFaceItemCustomWnd : public CFGuiWnd
{

	//��������
public:
	//���캯��
	CFaceItemCustomWnd();
	//��������
	virtual ~CFaceItemCustomWnd();

	//�ӿں���
public:
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgCustomAvatar.xml"); }
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	

	//���غ���
protected:
	//�滭��Ϣ
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//���ú���
public:
	//���ýӿ�
	VOID SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent);

	//ͼƬ����
public:
	//�����ж�
	bool IsNull();
	//����ͼƬ
	VOID DestoryImage();
	//��ȡͼƬ
	VOID DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//��Ϣӳ��
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//��ť��Ϣ
protected:
	//��תͼ��
	VOID OnBnClickedTurnLeft();
	//��תͼ��
	VOID OnBnClickedTurnRight();
	//��ʾ����
	VOID OnBnClickedShowVLine();
	//����ͼ��
	VOID OnBnClickedLoadImage();
	//����ͼ��
	VOID OnBnClickedLoadCamera();

	DECLARE_MESSAGE_MAP()

	//���ư�ť
protected:
	CButtonUI *			 			m_pbtTurnLeft;						//��ת��ť		
	CButtonUI *			 			m_pbtTurnRight;						//��ת��ť
	CButtonUI *			 			m_pbtShowVLine;						//���߰�ť

	//���ư�ť
protected:
	CButtonUI *			 			m_pbtLoadImage;						//����ͼƬ
	CButtonUI *			 			m_pbtLoadCamera;					//��������
	CTabLayoutUI*					m_pTabLayoutCustom;					//��ͼ�л�


	//�ؼ�����
protected:
	CImageEditorControl    			m_ImageEditorControl;				//�༭�ؼ�
public:
	bool							m_bLoadCustom;						//�ɹ�����
};

//////////////////////////////////////////////////////////////////////////////////

#endif