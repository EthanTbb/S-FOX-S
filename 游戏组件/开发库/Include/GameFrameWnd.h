#ifndef GAME_FRAME_WND_HEAD_FILE
#define GAME_FRAME_WND_HEAD_FILE

#pragma once

#include "GameFrameHead.h"
#include "GameFrameControl.h"
#include "DlgGameRule.h"

//////////////////////////////////////////////////////////////////////////////////

//�����Դ
class CFrameEncircle : public CEncircleBMP
{
	//��������
public:
	//���캯��
	CFrameEncircle();
	//��������
	virtual ~CFrameEncircle();

	//���غ���
protected:
	//�滭����
	virtual bool PreDrawEncircleImage(tagEncircleBMP & EncircleImage);
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸ����
class GAME_FRAME_CLASS CGameFrameWnd : public CFGuiFrameWnd, public IGameFrameWnd,public IFlashControlSink,public IClientPropertySink
{
	//״̬����
protected:
	bool							m_bMaxShow;							//����־
	bool							m_bShowControl;						//��ʾ��־
	CRect							m_rcNormalSize;						//����λ��

	//��������
protected:
	INT								m_nScrollXPos;						//����λ��
	INT								m_nScrollYPos;						//����λ��
	INT								m_nScrollXMax;						//���λ��
	INT								m_nScrollYMax;						//���λ��

	//����ӿ�
protected:
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IGameFrameView *				m_pIGameFrameView;					//��ͼ�ӿ�

	//�ؼ�ָ��
protected:
	CDlgGameRule *					m_pDlgGameRule;						//��Ϸ����

	//���ܿؼ�
public:
	CFlashControl					m_FlashControl;						//�����ؼ�
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CSkinSplitter					m_SkinSplitter;						//��ֿؼ�
	CGameFrameControl				m_GameFrameControl;					//���ƿ��

	//��Դ����
protected:
	CFrameEncircle					m_FrameEncircle;					//�����Դ

	//��������
public:
	//���캯��
	CGameFrameWnd();
	//��������
	virtual ~CGameFrameWnd();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	
	//Ƥ������
	virtual LPCTSTR GetSkinFile() { return TEXT("GameFrame.xml"); }

	//���ڿ���
public:
	//��Ϸ����
	virtual bool ShowGameRule();
	//��󴰿�
	virtual bool MaxSizeWindow();
	//��ԭ����
	virtual bool RestoreWindow();

	//���ƽӿ�
public:
	//��������
	virtual bool AllowGameSound(bool bAllowSound);
	//�Թۿ���
	virtual bool AllowGameLookon(DWORD dwUserID, bool bAllowLookon);
	//���ƽӿ�
	virtual bool OnGameOptionChange();

	//���߽ӿ�
public:
	//�������
	virtual VOID OnEventFlashFinish();
	//���߳ɹ�
	virtual VOID OnPropertySuccess(LPCTSTR szTargerNickName,DWORD dwSourceUserID,DWORD dwTargerUserID,WORD wPropertyIndex,WORD wPropertyCount);
	//����ʧ��
	virtual VOID OnPropertyFailure( LONG lErrorCode,LPCTSTR szDescribeString);
	//��������
	virtual bool SetTrumpetNum(DWORD dwTrumpetCount,DWORD dwTyphonCount);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//�������
protected:
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);

	//ϵͳ��Ϣ
public:
	//��������
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////////////

#endif