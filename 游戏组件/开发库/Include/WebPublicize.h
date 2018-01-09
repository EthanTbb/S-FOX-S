#ifndef WEB_PUBLICIZE_HEAD_FILE
#define WEB_PUBLICIZE_HEAD_FILE

#pragma once

//#include <mshtml.h>	
//#include "mshtmdid.h" 
#include "WebBrowser.h"
#include "ShareControlHead.h"

//#import <mshtml.tlb>

//////////////////////////////////////////////////////////////////////////////////

//���״̬
#define BROWSER_STATUS_LOAD			0									//����״̬
#define BROWSER_STATUS_ERROR		1									//����״̬
#define BROWSER_STATUS_FINISH		2									//���״̬

//////////////////////////////////////////////////////////////////////////////////
//�¼��ӿ�
interface IDocEventHanderSink
{
	//����¼�
	virtual VOID OnEventClicked()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//��������
class CWebPublicize;
class CDocEventHandler;

//////////////////////////////////////////////////////////////////////////////////
//
////�¼�����
//class SHARE_CONTROL_CLASS CDocEventHandler : public CCmdTarget
//{
//	DECLARE_DYNAMIC(CDocEventHandler)
//
//	//�ӿڱ���
//protected:
//	IDocEventHanderSink *					m_pIDocEventHanderSink;				//�ص��ӿ�
//
//	//��������
//public:
//	//���캯��
//	CDocEventHandler();
//	//��������
//	virtual ~CDocEventHandler();
//
//	//�¼�����
//public:
//	//����¼�
//	VOID OnClick(MSHTML::IHTMLEventObjPtr pEvtObj);
//
//	DECLARE_MESSAGE_MAP()
//	DECLARE_DISPATCH_MAP()
//	DECLARE_INTERFACE_MAP()
//};

//////////////////////////////////////////////////////////////////////////////////

//���ؼ�
class SHARE_CONTROL_CLASS CWebPublicize : public CWnd
{
	//��ʶ����
protected:
	DWORD							m_dwDocCookie;						//�ĵ����
	COLORREF						m_crBackGround;						//������ɫ

	//״̬����
protected:
	WORD							m_wIndex;							//��������
	BYTE							m_cbStatus;							//���״̬

	//�ؼ�����
protected:
	CWebBrowser						m_WebBrowser;						//��洰��
	//CDocEventHandler *				m_pEventHandler;					//�¼�����

	//��������
public:
	//���캯��
	CWebPublicize();
	//��������
	virtual ~CWebPublicize();

	//��Ϣ�ӿ�
public:	
	//��ȡ״̬
	BYTE GetBrowserStatus() { return m_cbStatus; }
	//��ȡ����
	CWebBrowser * GetWebBrowser() { return &m_WebBrowser; }

	//��������
public:
	//������ɫ
	VOID SetBackGroundColor(COLORREF crBackGround);

	//�������
public:
	//�������
	VOID Navigate(LPCTSTR pszURL);
	//�������
	VOID Navigate(LPCTSTR pszURL, VARIANT * Flags, VARIANT * TargetFrameName, VARIANT * PostData, VARIANT * Headers);

	//���ú���
protected:
	//״̬֪ͨ
	virtual VOID OnBrowserStatus(BYTE cbStatus);
	//λ�ú���
	virtual VOID GetWebBrowserRect(CRect&rcWebBrowser);
	//�滭����
	virtual VOID DrawPublicizeView(CDC * pDC, INT nWidth, INT nHeight, BYTE cbStatus);

	//��Ϣ����
public:
	//������Ϣ
	VOID OnDestroy();
	//�ػ���Ϣ
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);	
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	//��������
public:
	//װ�ش�����
	VOID InstallEventHandler();
	//ж�ش�����
	VOID UninstallEventHandler();

	//��Ϣ����
protected:
	//�������
	VOID OnDocumentCompleteWeb(LPDISPATCH pDisp, VARIANT FAR * URL);
	//��������
	VOID OnNavigateErrorWeb(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Frame, VARIANT FAR * StatusCode, BOOL FAR * Cancel);
	//������ʼ
	VOID OnBeforeNavigate2Web(LPDISPATCH pDisp, VARIANT FAR * URL, VARIANT FAR * Flags, VARIANT FAR * TargetFrameName, VARIANT FAR * PostData, VARIANT FAR * Headers, BOOL FAR * Cancel);

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif