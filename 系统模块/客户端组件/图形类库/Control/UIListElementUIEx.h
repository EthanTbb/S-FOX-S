#include "UIList.h"

class FGUI_LIB_CLASS CListComboElementUI : public CListContainerElementUI
{
public:
	CListComboElementUI();
	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	bool HoverItem(bool bHover = true);
	void SetPos(RECT rc);
	void SetFixedHeight(int cy);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI& event);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DrawItemBk(HDC hDC, const RECT& rcItem);
	void SetText2(LPCTSTR pstrText);
	CStdString GetText2();
	void SetFont(int index);
	int  GetFont() const;

	void SetTextColor(DWORD dwTextColor);
	DWORD GetTextColor() const;
	DWORD GetText2Color() const;
	void SetText2Color(DWORD dwTextColor);

	void SetFaceID(const int uFaceID, bool light=true);
	void SetFaceImage(LPCTSTR pstrImage);
	LPCTSTR GetFaceImage();
	void SetDeleteStatusImage(LPCTSTR pstrImage);
	LPCTSTR GetDeleteStatusImage();

	bool OnButtonEvent(void* param);
private:
	bool HoverItemText(bool bHover);
private:
	CVerticalLayoutUI* m_pHeadVerticalLayout;	//头像垂直布局
	CVerticalLayoutUI* m_pDeleteBtnVerticalLayout;
	CButtonUI*	m_pButton;						//删除按钮
	CControlUI* m_pFaceImage;					//头像显示
	CLabelUI*	m_pLabelText;					//昵称显示
	CLabelUI*	m_pLabelText2;					//账号显示
	bool		m_bBigerHeight;
	CStdString	m_szText2;						//昵称文字
	DWORD		m_dwText2Color;
	DWORD		m_dwTextColor;
	int			m_CurItemHeight;				//当前项的高度
	int			m_NormalHeight;		
	int			m_iFont;
};