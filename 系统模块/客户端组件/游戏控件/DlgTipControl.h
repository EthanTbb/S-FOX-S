#ifndef DLG_TIP_CONTROL_HEAD_FILE
#define DLG_TIP_CONTROL_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������
class CWndTipControl;

//////////////////////////////////////////////////////////////////////////////////
//���Ͷ���
typedef CMap<HWND,HWND,CWndTipControl *,CWndTipControl *>				CTipControlMap;

//���Ͷ���
typedef BOOL (WINAPI * LPUpdateLayeredWindow)(HWND,HDC,POINT*,SIZE*,HDC,POINT*,COLORREF,BLENDFUNCTION*,DWORD);          

//////////////////////////////////////////////////////////////////////////////////

//�Ի�����
class SHARE_CONTROL_CLASS CWndTipControl
{
	//���ı���
protected:
	HWND							m_hWnd;								//���ھ��

	//��Դ����
protected:
	HINSTANCE						m_hInstance;						//��Դ���
	TCHAR							m_szImageGround[64];				//����ͼƬ

	//��������
protected:
	bool							m_bTrackMouse;						//���ٱ�ʶ
	INT								m_nRelayTime;						//��ʱʱ��
	CRect							m_rcInvalidArea;					//��Ч����
	
	//��������
protected:
	BLENDFUNCTION					m_Blend;							//��Ͻṹ	
	HINSTANCE						m_hFuncInst;						//�������							

	//��������
protected:
	LPUpdateLayeredWindow			m_pUpdateLayeredWindow;				//���º���

	//��Դ����
public:	
	static CTipControlMap			m_TipControlMap;					//�ؼ�ӳ��

	//��������
public:
	//���캯��
	CWndTipControl();
	//��������
	virtual ~CWndTipControl();

	//���ܺ���
public:
	//���ش���
	VOID QuickHidwWindow();
	//������ʱ
	VOID SetRelayTime(INT nRelayTime);	
	//�����ؼ�
	VOID CreateTipControl(INT nWidth,INT nHeight,INT nAlpha);	
	//��ʾ����
	VOID RelayShowWindow(INT nXPos,INT YPos,CRect rcInvalidArea);
	//������Դ
	VOID SetControlResource(HINSTANCE hInstanceRes,LPCTSTR pszImageGround);

	//���غ���
public:
	//��ʾ̽��
	virtual BOOL DetectIsShowWindow();
	//����̽��
	virtual BOOL DetectIsHideWindow();
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//�滭����
private:
	//���¿ؼ�
	BOOL UpdateTipControl();
	//�滭�ؼ�
	VOID DrawControlView(HDC hDC, INT nWdith, INT nHeight);
	
	//��Ϣӳ��
protected:	
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//�����Ϣ
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	//��̬����
protected:
	//���ڹ���
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////////////

#endif