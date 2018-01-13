#pragma once

#include "Stdafx.h"
#include "SkinVerticleScrollbar.h"

class CMyEdit : public CSkinRichEdit
{
DECLARE_DYNAMIC(CMyEdit)


public:
CMyEdit();
virtual ~CMyEdit();

//�������
protected:		
	CSkinVerticleScrollbar			m_SkinVerticleScrollbar;	//��������
protected:
DECLARE_MESSAGE_MAP()

public:
//�����ı��򱳾�ɫ
void			SetBackColor(COLORREF rgb);
//�����ı����������ɫ
void			SetTextColor(COLORREF rgb);
//��������
void			SetTextFont(const LOGFONT &lf);
//��ȡ��ǰ����ɫ
COLORREF		GetBackColor(void){return m_crBackGnd;}
//��ȡ��ǰ�ı�ɫ
COLORREF		GetTextColor(void){return m_crText;}
//��ȡ��ǰ����
BOOL			GetTextFont(LOGFONT &lf);
//��ʼ��
void			Init();
//����λ��
void			ReSetLoc( int cx, int cy);
bool InsertString(LPCTSTR pszString, CHARFORMAT2 & CharFormat);
private:
COLORREF		m_crText;
COLORREF		m_crBackGnd;
CFont			m_font;
CBrush			m_brBackGnd;

afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
// protected:
// 	virtual void PreSubclassWindow();
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};

