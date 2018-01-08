#ifndef DLG_CUSTOM_FACE_HEAD_FILE
#define DLG_CUSTOM_FACE_HEAD_FILE

#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////

//ͷ��ѡ��
class CDlgCustomFace : public CFGuiDialog, public CMissionItem, public IImageEditorEvent, public IFaceItemSelectEvent
{
	//��������
public:
	//���캯��
	CDlgCustomFace();
	//��������
	virtual ~CDlgCustomFace();

	//�༭�ӿ�
public:
	//ͼ���¼�
	virtual VOID OnEventFaceSelect(WORD wFaceID);
	//ͼ���¼�
	virtual VOID OnEventImageSelect(bool bLoadImage, bool bOperateing);

	//���ú���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//�ӿں���
public:
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgModifyAvatar.xml"); } 
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//���ں���
protected:
	//�滭����
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);

	virtual void OnEndPaintWindow(HDC hDC);
	//���غ���
protected:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//���ܺ���
public:
	//���ýӿ�
	bool SetCustomFaceEvent(IUnknownEx * pIUnknownEx);

	//�ڲ�����
private:
	//�ؼ�����
	VOID EnableControl(bool bEnableControl);

private:
	bool LoadSysAvatar();
	bool UpdateAvatarPreview(const WORD wFaceID);
	bool UpdateAvatarPreview(DWORD* dwCustomFace, DWORD dwSize);

	DECLARE_MESSAGE_MAP()

	//״̬����
protected:
	BYTE							m_cbMode;							//�༭ģʽ
	WORD							m_wFaceID;							//ѡ��ͷ��
	tagCustomFaceInfo				m_CustomFaceInfo;					//�Զ�ͷ��
	tagCustomFaceInfo96				m_CustomFaceInfo96;					//�����ߴ�

	//�ӿڱ���
protected:
	ICustomFaceEvent *				m_pICustomFaceEvent;				//ͷ���¼�

	//���ܰ�ť
protected:
	CButtonUI *						m_pbtOk;							//�洢�˳�
	CButtonUI *						m_pbtCancel;						//�洢�˳�
	CTileLayoutUI*					m_pTileLayoutSysAvatar;				//ϵͳͷ�񲼾�
	CImageUI*						m_pAvatarPreview;					//ͷ��Ԥ��
	//�ؼ�����
protected:
	//CSkinTabCtrl		 			m_TabControl;						//ѡ������
	CMissionManager					m_MissionManager;					//�������
	CFaceItemCustomWnd				m_FaceItemCustomWnd;				//ͷ���Զ�
	//CFaceItemSelectWnd	 			m_FaceItemSelectWnd;				//ͷ��ѡ��

};

//////////////////////////////////////////////////////////////////////////////////

#endif