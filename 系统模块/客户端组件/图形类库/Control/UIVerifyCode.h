#ifndef __UIVERIFYCODE_H__
#define __UIVERIFYCODE_H__

#pragma once

class FGUI_LIB_CLASS CVerifyCodeUI : public CLabelUI
{
public:
	CVerifyCodeUI();
	~CVerifyCodeUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetNumberFomart(LPCTSTR pstrFomart);
	LPCTSTR GetNumberFomart();
	void SetNumberImage(LPCTSTR pstrImage);
	LPCTSTR GetNumberImage();

	LPCTSTR GetCurrCode() const;
	void Refresh();
	void DoInit();
	void PaintText(HDC hDC);
    void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	virtual void SetText(LPCTSTR pstrText);
protected:
	CStdString m_Code;
	CStdString m_sNumberImage;
	CStdString m_sNumberFomart;
	CStdString m_sAnalysisText;
	SIZE m_AutoSize;
	std::map<TCHAR, int> m_mapFormat;

};

#endif