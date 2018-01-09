#ifndef DLG_MATCH_AWARDS_HEAD_FILE
#define DLG_MATCH_AWARDS_HEAD_FILE

#pragma once

#include "afxwin.h"
#include "GameFrameHead.h"

//////////////////////////////////////////////////////////////////////////

//������״
class GAME_FRAME_CLASS CDlgMatchAwards : public CFGuiDialog
{
	//���Ʊ���
public:
	CMD_GR_MatchResult *				m_pMatchResult;					//�������

	//��Դ����
protected:
	CFont								m_RewardFont;					//��������
	CFont								m_AccountsFont;					//�˺�����	
	CFont								m_WatermarkFont;				//ˮӡ����	

	//����ӿ�
protected:
	IClientKernel *						m_pIClientKernel;				//�ں˽ӿ�
	IStringMessage *					m_pIStringMessage;				//��Ϣ�ӿ�

	//����ؼ�
protected:
	CButtonUI *							m_pbtClose;						//�رհ�ť
	CButtonUI *							m_pbtTakePhoto;					//���հ�ť
	CSkinLayered						m_SkinLayered;					//�ֲ㴰��

	//��������
public:
	//���캯��
	CDlgMatchAwards();
	//��������
	virtual ~CDlgMatchAwards();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//��Ϣ����
public:
	//�������
	VOID OnLButtonDown(UINT nFlags, CPoint Point);

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);
	
	//���ܺ���
public:
	//���ò���
	VOID SetMatchResult(CMD_GR_MatchResult * pMatchResult);
	
	//��������
protected:
	//������Ļ
	HBITMAP CopyScreenToBitmap(LPRECT lpRect);
	//��Ϸ����
	bool CaptureGameScreen(LPCTSTR pszSavePath);
	//����λͼ
	bool CopyBitmapToClipBoard(HBITMAP hBitmap);

	//��Ϣ����
public:
	//�ر���Ϣ
	VOID OnBnClickedClose();
	//������Ϣ
	VOID OnBnClickedTakePhoto();
};

#endif