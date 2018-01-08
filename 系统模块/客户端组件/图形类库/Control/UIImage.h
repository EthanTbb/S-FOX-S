#ifndef __UIImage_H__
#define __UIImage_H__

#pragma once
#include <atlimage.h>

class FGUI_LIB_CLASS CImageUI : public CControlUI
{
public:
	CImageUI();
	~CImageUI();

	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetSize(SIZE sz);
	SIZE GetSize();
	void SetImage(LPCTSTR pstrImage);
	LPCTSTR GetImage();
	void SetCustomImage(DWORD* dwCustomFace, DWORD dwSize = 48*48);

	virtual void PaintBkImage(HDC hDC);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
private:
	CStdString	m_sImage;
	CImage		m_Image;
	SIZE		m_szImage;
};

#endif