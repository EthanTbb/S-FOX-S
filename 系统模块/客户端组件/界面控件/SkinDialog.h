#ifndef SKIN_DIALOG_HEAD_FILE
#define SKIN_DIALOG_HEAD_FILE

#pragma once

#include "SkinEncircle.h"
#include "SkinControlHead.h"
#include "SkinRenderManager.h"
#include "SkinResourceManager.h"
#include "SkinDpi.h"

//////////////////////////////////////////////////////////////////////////////////
//��������
class CSkinDialogAttribute;
class CSkinLayered;
class CSkinDialog;

//��������
typedef CMap<HWND,HWND,CSkinLayered*,CSkinLayered*>		CSkinLayeredMap;

//////////////////////////////////////////////////////////////////////////////////

//������Դ
class SKIN_CONTROL_CLASS CSkinDialogAttribute
{
	//��ɫ����
public:
	COLORREF						m_crTitleText;						//������ɫ
	COLORREF						m_crTitleFrame;						//������ɫ
	COLORREF						m_crBackGround;						//������ɫ
	COLORREF						m_crControlText;					//������ɫ
	COLORREF						m_crLabelRound;						//��ǩ����

	//��Դ����
public:
	CFont                           m_Font;                             //�ؼ�����
	CSize							m_SizeButton;						//��ť��С
	CBrush							m_brBackGround;						//������ˢ
	CBrush							m_brLabelRound;						//���ƻ�ˢ
	CEncirclePNG					m_PNGEncircleFrame;					//�����Դ

	//λ�ñ���
public:
	tagEncircleInfo					m_EncircleInfoView;					//��ͼ����
	tagEncircleInfo					m_EncircleInfoFrame;				//��ܻ���

	//��������
public:
	//���캯��
	CSkinDialogAttribute();
	//��������
	virtual ~CSkinDialogAttribute();

	//������
public:
	//������Դ
	bool Initialization(CSkinRenderManager * pSkinRenderManager);
};

//////////////////////////////////////////////////////////////////////////////////

//�ֲ㴰��
class SKIN_CONTROL_CLASS CSkinLayered : public CWnd
{
	//��������
protected:	
	HWND							m_hwndControl;						//���ھ��
	LONG							m_OriParentProc;					//���ڹ���

	//��̬����
protected:
	static CSkinLayeredMap			m_SkinLayeredMap;					//�ֲ��б�

	//��������
public:
	//���캯��
	CSkinLayered();
	//��������
	virtual ~CSkinLayered();

	//���ܺ���
public:
	//��������
	bool CreateLayered(HWND hWnd);
	//��������
	VOID InitLayeredArea(CDC * pDCImage, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);
	//��������
	VOID InitLayeredArea(CPngImage & Image, BYTE cbAlpha, CRect & rcUnLayered, CPoint & PointRound, bool bUnLayeredChild);

	//��Ϣ����
public:
	//�ر���Ϣ
	VOID OnClose();
	//���ý���
	VOID OnSetFocus(CWnd* pOldWnd);

	//��̬����
protected:
	//ö�ٺ���
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);
	//���ڹ���
	static LRESULT CALLBACK ParentProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�Ի�����
class SKIN_CONTROL_CLASS CSkinDialog : public CFGuiDialog
{
	//��ʶ����
protected:
	bool							m_bInitDialog;						//��ʼ��ʶ
	bool							m_bLableRound;						//���Ʊ�ʶ

	//��������
protected:
	BYTE							m_cbButtonState[3];					//��ť״̬
	CRect							m_rcRoundSpace;						//���Ƽ��

	//����ؼ�
protected:
	CLabelUI *						m_pLabelRound;						//���Ʊ�ǩ
	CButtonUI *						m_pButton[3];						//��ťλ��

	//���ڱ���
private:
	UINT                            m_nIDTemplate;                      //��Դ����	
	CSkintDPI                       m_SkinDPI;                          //����DPI
	CSkinLayered					m_SkinLayered;						//�ֲ㴰��

	//��Դ����
public:
	static CSkinDialogAttribute		m_SkinAttribute;					//��Դ����

	//��������
public:
	//���캯��
	CSkinDialog(UINT nIDTemplate, CWnd * pParentWnd=NULL);
	//��������
	virtual ~CSkinDialog();

    //�ӿں���
public:
	//��ʼ�ؼ�
	virtual void InitControlUI();
	//��Ϣ����
	virtual void Notify(TNotifyUI &  msg);	

	//�滭�ӿ�
public:
	//�����滭
	virtual void OnEndPaintWindow(HDC hDC);
	//��ʼ�滭
	virtual void OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//������Ϣ
	virtual BOOL OnInitDialog();
	//�滭����
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight);
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);

	//��������
private:
	//������Դ
	VOID RectifyResource(INT nWidth, INT nHeight);
	//������ť
	VOID CaleTitleButton(INT nWidth, INT nHeight);

	//�滭����
private:
	//�滭�ؼ�
	VOID DrawControlView(CDC * pDC, INT nWdith, INT nHeight);

	//��Ϣӳ��
protected:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�ؼ���ɫ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	//�Զ���Ϣ
protected:
	//������Ϣ
	LRESULT	OnSetTextMesage(WPARAM wParam, LPARAM lParam);	

	//��������
public:
	//ö�ٺ���
	static BOOL CALLBACK EnumChildProc(HWND hWndChild, LPARAM lParam);
	//��������
	static VOID CALLBACK SetWndFont(CWnd *pWnd, CFont *pFont);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif