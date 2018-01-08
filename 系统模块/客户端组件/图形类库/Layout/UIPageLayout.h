#ifndef UISCROLL_PAGE_HEAD_FILE
#define UISCROLL_PAGE_HEAD_FILE

#pragma once

class CPageLayoutUI;
class CButtonUI;

class FGUI_LIB_CLASS CPageBarUI : public CHorizontalLayoutUI
{
public:
	CPageBarUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetOwner(CPageLayoutUI* pOwner);

	void SetButtonLeftStatusImage(LPCTSTR pStrImage);
	LPCTSTR GetButtonLeftStatusImage();
	void SetButtonRightStatusImage(LPCTSTR pStrImage);
	LPCTSTR GetButtonRightStatusImage();
	void SetButtonMidStatusImage(LPCTSTR pStrImage);
	LPCTSTR GetButtonMidStatusImage();
	void SetButtonMidSelectImage(LPCTSTR pStrImage);
	LPCTSTR GetButtonMidSelectImage();

	void SetButtonLeftVisible(bool bVisible);
	bool GetButtonLeftVisible();
	void SetButtonRightVisible(bool bVisible);
	bool GetButtonRightVisible();
	void SetButtonMidVisible(bool bVisible);
	bool GetButtonMidVisible();

	bool IsPageOption(CControlUI* pControl);
	int GetPageIndexByControlUI(CControlUI* pControl);
	bool Select(const int nPage);
	void UpdatePageBar();

	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DoInit();
	void SetPos(RECT rc);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

private:
	virtual void EnableScrollBar(bool bEnableVertical = true, bool bEnableHorizontal = false){};
	bool OnPageEvent(void* param);

private:
	CPageLayoutUI* m_pOwner;
	CButtonUI* m_pPageLeft;
	CButtonUI* m_pPageRight;
	CStdPtrArray m_ArrayPages;

	CStdString m_sButtonLeftStatusImage;
	CStdString m_sButtonRightStatusImage;
	CStdString m_sButtonMidStatusImage;
	CStdString m_sButtonMidSelectImage;
	CStdString m_sGroup;

	bool	   m_bButtonLeftVisible;
	bool	   m_bButtonRightVisible;
	bool	   m_bButtonMidVisible;
};



typedef std::vector<CControlUI*> VControlRow;	//一个控件行
typedef std::vector<VControlRow> AllControlRow;	//所有控件行

struct PageInfo
{
	int nyChildPadding;
	AllControlRow allRow;
};

typedef std::vector<PageInfo> VPage;		//一页


class FGUI_LIB_CLASS CPageLayoutUI : public CTileLayoutUI
{
public:
	CPageLayoutUI();
	virtual ~CPageLayoutUI();
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	int GetRows() const;
	void SetRows(int nRows);
	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);
	void EnablePageBar(bool bEnable, LPCTSTR pstrAttributeList = NULL);
	void VisiblePageBar(bool bVisible);
	bool IsVisiblePageBar() const;
	void ProcessPageBar(RECT rc, int cxRequired, int cyRequired);
	
	void SetCurrPage(int index);
	int GetCurPage();
	int SelectPrevPage();
	int SelectNextPage();
	int SelectPage(const int nPageIndex);
	int GetMaxPage();

	void SetXMinChildPadding(const int xMinChildPadding);
	int  GetXMinChildPadding();
	void SetXMaxChildPadding(const int xMaxChildPadding);
	int  GetXMaxChildPadding();
	void SetYMinChildPadding(const int yMinChildPadding);
	int  GetYMinChildPadding();
	void SetYMaxChildPadding(const int yMaxChildPadding);
	int  GetYMaxChildPadding();

	void SetVisible(bool bVisible);
	void SetPos(RECT rc);
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DoInit();
	bool Activate();

	CPageBarUI* GetPageBar();
	LPCTSTR GetPageBarStyle();

	CControlUI* FindControl(FINDCONTROLPROC Proc,const CPaintManagerUI * pManager, LPVOID pData, UINT uFlags);
	CControlUI* FindControl(LPCTSTR pstrName);
	void SetManager(CPaintManagerUI* pManager, CControlUI* pParent, bool bInit = true);

	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

private:
	void AnalysisRows(const RECT& rc);
	void AnalysisPages(const RECT& rc, const AllControlRow& allRows);
	void SetPagePos(const RECT& rc, const PageInfo& page, const bool bVisible);
	int GetControlColumnMaxHeight(const VControlRow& vControlRow);
	int GetControlColumnPadding(const RECT& rc, const VControlRow& vControlRow);
	int GetControlRowPadding(const RECT& rc, const PageInfo& page);

private:
	int m_nRows;
	int m_nMaxPages;
	int m_nCurPage; //当前页
	CPageBarUI* m_pPageBar;
	bool m_bVisiblePageBar;
	CStdString m_sPageBarStyle;

	int	m_nxMinChildPadding;
	int	m_nxMaxChildPadding;
	int	m_nyMinChildPadding;
	int	m_nyMaxChildPadding;

	AllControlRow m_allRows;
	VPage		  m_allPage;
};

#endif