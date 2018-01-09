#ifndef __UIGAMELIST_H__
#define __UIGAMELIST_H__

#pragma once
#include <vector>
#include <math.h>

inline double _CalculateDelay(double state) {
	return pow(state, 2);
}

struct FGUI_LIB_CLASS CStdNodeData
{
	int _level;
	bool _child_visible;
	CStdString _text;
	CListLabelElementUI* _pListElement;
};

class FGUI_LIB_CLASS CStdNode
{
public:
	CStdNode();
	explicit CStdNode(CStdNodeData t);
	CStdNode(CStdNodeData t, CStdNode* parent);
	~CStdNode();
	CStdNodeData& data();
	int num_children()const;
	CStdNode* child(int i);
	CStdNode* parent();
	bool has_children()const;
	void add_child(CStdNode* child);
	void remove_child(CStdNode* child);
	CStdNode* get_last_child();
private:
	void set_parent(CStdNode* parent);

private:
	typedef std::vector<CStdNode*>	Children;
	Children		_children;
	CStdNode*		_parent;
	CStdNodeData	_data;
};	


class CNodeElementUI;
// category(0)->game(1)->server(2)->room(3)
class FGUI_LIB_CLASS CGameListUI : public CListUI
{
public:
	enum { SCROLL_TIMERID = 10 };
	CGameListUI();
	virtual ~CGameListUI();
	LPCTSTR GetClass() const;
	UINT GetControlFlags() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	bool Add(CControlUI* pControl);
	bool Add(CControlUI* pControl, CControlUI* pParent);
	bool AddAt(CControlUI* pControl, int iIndex);
	bool Remove(CControlUI* pControl);
	bool RemoveAt(int iIndex);
	void RemoveAll();
	void DoEvent(TEventUI& event);
	CNodeElementUI* GetRoot();
	bool AddNode(CNodeElementUI* pNodeElementUI, CNodeElementUI* pParent = NULL);
	void RemoveNode(CNodeElementUI* pNodeElementUI);
	void SetChildVisible(CNodeElementUI* pNodeElement, bool visible);
	SIZE GetExpanderSizeX(CStdNode* node) const;
	bool Expand(CNodeElementUI* pNodeElementUI);

private:
	CStdNode* _root;
	CNodeElementUI* _RootElement;

	LONG m_dwDelayDeltaY;
	DWORD m_dwDelayNum;
	DWORD m_dwDelayLeft;
};


//CNodeElementUI
class FGUI_LIB_CLASS CNodeElementUI : public CListContainerElementUI
{
public:
	CNodeElementUI();
	virtual ~CNodeElementUI();

	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);

	int GetCount() const;
	CControlUI* GetItemAt(int iIndex) const;
	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);
	bool Remove(CControlUI* pControl);
	bool RemoveAt(int iIndex);

	//针对m_ArrayUINodes操作
	int GetNodeCount() const;
	CNodeElementUI* GetNodeAt(int iIndex) const;

	void SetText(LPCTSTR pstrText);

	void DrawItemText(HDC hDC, const RECT& rcItem);
	void DrawItemContorl(HDC hDC, const RECT& rcItem);
	SIZE EstimateSize(SIZE szAvailable);
	
	void MakeHtmlText();
	LPCTSTR GetNormalExpandImage();
	void SetNormalExpandImage(LPCTSTR pstrImage);
	LPCTSTR GetActiveExpandImage();
	void SetActiveExpandImage(LPCTSTR pstrImage);
	int GetLeftSpace() const;
	void SetLeftSpace(const int nSpace);
	void SetIcon(LPCTSTR pstrImage);
	LPCTSTR GetIcon();

	//不同定制(Expand展开 Collapse收缩)
	void SetExpandNormalControlName(LPCTSTR pstrName);
	LPCTSTR GetExpandNormalControlName();
	void SetExpandHotControlName(LPCTSTR pstrName);
	LPCTSTR GetExpandHotControlName();
	void SetExpandSelectdControlName(LPCTSTR pstrName);
	LPCTSTR GetExpandSelectdControlName();
	void SetExpandDisableControlName(LPCTSTR pstrName);
	LPCTSTR GetExpandDisableControlName();

	void SetCollapseNormalControlName(LPCTSTR pstrName);
	LPCTSTR GetCollapseNormalControlName();
	void SetCollapseHotControlName(LPCTSTR pstrName);
	LPCTSTR GetCollapseHotControlName();
	void SetCollapseSelectdControlName(LPCTSTR pstrName);
	LPCTSTR GetCollapseSelectdControlName();
	void SetCollapseDisableControlName(LPCTSTR pstrName);
	LPCTSTR GetCollapseDisableControlName();

	void Invalidate();
	void DoPaint(HDC hDC, const RECT& rcPaint);
	void DoEvent(TEventUI& event);
	void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);

	//node 
	CStdNodeData& data() { return _data;}
	int num_children()const;
	CNodeElementUI* child(int i);
	CNodeElementUI* parent();
	bool has_children()const;
	void add_child(CNodeElementUI* child);
	void remove_child(CNodeElementUI* child);
	CNodeElementUI* get_last_child();
private:
	void set_parent(CNodeElementUI* parent);

private:
	typedef std::vector<CNodeElementUI*>	Children;
	Children			_children;
	CNodeElementUI*		_parent;
	CStdNodeData		_data;
	CStdPtrArray		m_ArrayUINodes;
	CStdString			m_sNormalExpandImage;
	CStdString			m_sActiveExpandImage;
	int					m_nLeftSpace;
	TListInfoUI			m_ListInfo;
	CStdString			m_sIconImage;

	CStdString			m_sExpandNormalControlName;
	CStdString			m_sExpandHotControlName;
	CStdString			m_sExpandSelectdControlName;
	CStdString			m_sExpandDisableControlName;
	CStdString			m_sCollapseNormalControlName;
	CStdString			m_sCollapseHotControlName;
	CStdString			m_sCollapseSelectdControlName;
	CStdString			m_sCollapseDisableControlName;
};

#endif