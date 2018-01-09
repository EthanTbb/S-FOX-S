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

	LPCTSTR GetCurrCode() const;

	void Refresh();

	void DoInit();

	void PaintText(HDC hDC);

protected:
	CStdString m_Code;

};

#endif