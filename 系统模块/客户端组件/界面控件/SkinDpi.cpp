
#include <stdafx.h>

#include <windows.h>
#include "SkinDpi.h"

//辅助类
class helper
{
public:
	enum
	{
		ATL_WM_OCC_LOADFROMSTREAM = 0x0376,
		ATL_WM_OCC_LOADFROMSTORAGE = 0x0377,
		ATL_WM_OCC_INITNEW = 0x0378,
		ATL_WM_OCC_LOADFROMSTREAM_EX = 0x037A,
		ATL_WM_OCC_LOADFROMSTORAGE_EX = 0x037B,
		ATL_DISPID_DATASOURCE = 0x80010001,
		ATL_DISPID_DATAFIELD = 0x80010002,
	};

//local struct used for implementation
#pragma pack(push, 1)
	struct DLGINITSTRUCT
	{
		WORD nIDC;
		WORD message;
		DWORD dwSize;
	};
	struct DLGTEMPLATEEX
	{
		WORD dlgVer;
		WORD signature;
		DWORD helpID;
		DWORD exStyle;
		DWORD style;
		WORD cDlgItems;
		short x;
		short y;
		short cx;
		short cy;
	};
	struct DLGITEMTEMPLATEEX
	{
		DWORD helpID;
		DWORD exStyle;
		DWORD style;
		short x;
		short y;
		short cx;
		short cy;
		DWORD id;
	};
#pragma pack(pop)

	static BOOL IsDialogEx(const DLGTEMPLATE* pTemplate)
	{
		return ((DLGTEMPLATEEX*)pTemplate)->signature == 0xFFFF;
	}

	inline static WORD& DlgTemplateItemCount(DLGTEMPLATE* pTemplate)
	{
		if (IsDialogEx(pTemplate))
			return reinterpret_cast<DLGTEMPLATEEX*>(pTemplate)->cDlgItems;
		else
			return pTemplate->cdit;
	}

	inline static const WORD& DlgTemplateItemCount(const DLGTEMPLATE* pTemplate)
	{
		if (IsDialogEx(pTemplate))
			return reinterpret_cast<const DLGTEMPLATEEX*>(pTemplate)->cDlgItems;
		else
			return pTemplate->cdit;
	}

	static DLGITEMTEMPLATE* FindFirstDlgItem(const DLGTEMPLATE* pTemplate)
	{
		BOOL bDialogEx = IsDialogEx(pTemplate);

		WORD* pw;
		DWORD dwStyle;
		if (bDialogEx)
		{
			pw = (WORD*)((DLGTEMPLATEEX*)pTemplate + 1);
			dwStyle = ((DLGTEMPLATEEX*)pTemplate)->style;
		}
		else
		{
			pw = (WORD*)(pTemplate + 1);
			dwStyle = pTemplate->style;
		}

		//跳过菜单
		if (*pw == 0xFFFF)
			pw += 2;				// Has menu ID, so skip 2 words
		else
			while (*pw++);			// Either No menu, or string, skip past terminating NULL

		if (*pw == 0xFFFF)
			pw += 2;				// Has class atom, so skip 2 words
		else
			while (*pw++);			// Either No class, or string, skip past terminating NULL

		//跳过标题
		while (*pw++);

		// If we have DS_SETFONT, there is extra font information which we must now skip
		if (dwStyle & DS_SETFONT)
		{
			if (bDialogEx)
				pw += 3;			// Skip font size, weight, (italic, charset)
			else
				pw += 1;			// Skip font size
			while (*pw++);			// Skip typeface name
		}

		// Dword-align and return
		return (DLGITEMTEMPLATE*)(((DWORD_PTR)pw + 3) & ~3);
	}

	// Given the current dialog item and whether this is an extended dialog
	// return a pointer to the next DLGITEMTEMPLATE*
	static DLGITEMTEMPLATE* FindNextDlgItem(DLGITEMTEMPLATE* pItem, BOOL bDialogEx)
	{
		WORD* pw;

		// First skip fixed size header information, size of which depends
		// if this is a DLGITEMTEMPLATE or DLGITEMTEMPLATEEX
		if (bDialogEx)
			pw = (WORD*)((DLGITEMTEMPLATEEX*)pItem + 1);
		else
			pw = (WORD*)(pItem + 1);

		if (*pw == 0xFFFF)			// Skip class name ordinal or string
			pw += 2; // (WORDs)
		else
			while (*pw++);

		if (*pw == 0xFFFF)			// Skip title ordinal or string
			pw += 2; // (WORDs)
		else
			while (*pw++);

		WORD cbExtra = *pw++;		// Skip extra data

		// cbExtra includes the size WORD in DIALOG resource.
		if (cbExtra != 0 && !bDialogEx)
			cbExtra -= 2;

		// Dword-align and return
		return (DLGITEMTEMPLATE*)(((DWORD_PTR)pw + cbExtra + 3) & ~3);
	}

};


struct dialogdata_t
{
    int       pt,has_menu;
    unsigned  weight;    
    BOOL      italic;
    LPCWSTR    faceName;    
};

static void getptsize( HDC &dc, HFONT &font, SIZE *pSize )
{
    HFONT oldfont = 0;
    static char *sym = "abcdefghijklmnopqrstuvwxyz"
                       "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    SIZE sz;
    TEXTMETRICA t;      
    oldfont = (HFONT)SelectObject(dc,font);
    GetTextMetricsA(dc,&t);
    GetTextExtentPointA(dc,sym, 52, &sz);
    pSize->cy = t.tmHeight;
    pSize->cx = (sz.cx / 26 + 1) / 2;
    SelectObject(dc,oldfont);    
}

#define word_at(ptr)  (*(WORD *)(ptr))
#define dword_at(ptr) (*(DWORD *)(ptr))

static void querydialogdata( LPCSTR data, dialogdata_t * result )
{
    WORD *p = (WORD *)data; 
    unsigned long rstyle = dword_at(p); 
    int dialogex=0;

    p += 2;

    if (rstyle == 0xffff0001)
    {
        p+=4;
        rstyle=dword_at(p);
        p+=2;
        dialogex=1;
    }else
        p+=2;

    p+=5;

    // Skip menu
    switch(word_at(p))
    {
    case 0x0000: // no menu name
        result->has_menu=0;
        p++;
        break;
    case 0xffff: // has menu        
        result->has_menu=1;
        p += 2; 
        break;
    default:     // has menu
        result->has_menu=1;     
        p+=wcslen((LPCWSTR)p)+1;
        break;
    }    

    // skip class name
    switch(word_at(p))
    {
    case 0x0000:
        p++;
        break;
    case 0xffff:        
        p += 2;
        break;
    default:
        p+=wcslen((LPCWSTR)p)+1;
        break;
    }

    // skip the caption
    p+=wcslen((LPCWSTR)p)+1;

    // get the font name 

    if (rstyle & DS_SETFONT)
    {
        result->pt = word_at(p); p++;
        if (dialogex)
        {
            result->weight = word_at(p); p++;
            result->italic = LOBYTE(word_at(p)); p++;
        }else
        {
            result->weight = FW_DONTCARE;
            result->italic = FALSE;
        }
        result->faceName = (LPCWSTR)p;              
        p += wcslen( result->faceName ) + 1;
    }else
    {
        result->faceName=L"Tahoma"; // TODO: put system font name here
        result->pt=8; // TODO
        result->weight=FW_NORMAL; // TODO
        result->italic=FALSE; // TODO
    }
}

//构造函数
CSkintDPI::CSkintDPI()
{
    m_Hwnd=0;
    m_Font=0;
    m_Oldfont=0;
    m_Inst=0;
}

//析构函数
CSkintDPI::~CSkintDPI()
{
    Detach();
}

//附加函数
void CSkintDPI::Attach(HINSTANCE hInst,HWND _hwnd,LPCTSTR lpszTemplateName,double dpi)
{
	//定义变量
    int t;
	helper::DLGTEMPLATEEX *lpDialogTemplate;

    if (m_Hwnd) Detach();

	//设置变量
    m_Inst = hInst;
    m_Hwnd=_hwnd;
	m_lpszTemplateName=lpszTemplateName;

    if (!hInst)
	    { } // hInst = AfxFindResourceHandle(IMAKEINTRESOURCE(IDD), RT_DIALOG);

	HRSRC hResource = ::FindResource(hInst, m_lpszTemplateName, RT_DIALOG);
	HANDLE hDialogTemplate = LoadResource(hInst, hResource);
	lpDialogTemplate = (helper::DLGTEMPLATEEX *)LockResource(hDialogTemplate);    


	dialogdata_t dd;
    querydialogdata((LPCSTR)lpDialogTemplate,&dd);

    if ((dd.pt<0)||(dd.pt>32767))
    {
        // I don't know what to do if it happens this way
        // this is a compromise solution:
        dd.pt=-dd.pt;
        dd.pt&=0xFFFF;
    }

  //  m_Font=CreateFontW(
		//-(int)(dd.pt*dpi/72.0 + 0.5), // negative makes it use "char size"
  //  0,              // logical average character width 
  //  0,              // angle of escapement 
  //  0,              // base-line orientation angle 
  //  dd.weight,  // weight
  //  dd.italic,  // italic
  //  FALSE,          // underline attribute flag 
  //  FALSE,          // strikeout attribute flag 
  //  DEFAULT_CHARSET,    // character set identifier 
  //  OUT_DEFAULT_PRECIS, // output precision 
  //  CLIP_DEFAULT_PRECIS,// clipping precision 
  //  DEFAULT_QUALITY,    // output quality 
  //  DEFAULT_PITCH,  // pitch and family 
  //  dd.faceName  // pointer to typeface name string 
  //  );

	m_Font=CreateFont(-12, 0, 0, 0, 400, FALSE, FALSE, FALSE, GB2312_CHARSET,  
		OUT_DEFAULT_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,  
		DEFAULT_PITCH, TEXT("宋体"));

    m_Oldfont=(HFONT)::SendMessage(m_Hwnd, WM_GETFONT, 0, 0);
    SendMessage(m_Hwnd, WM_SETFONT, (LPARAM)m_Font, TRUE);
  
	SIZE szf;

	
    PAINTSTRUCT ps;
    HDC dc=BeginPaint(m_Hwnd, &ps);
    getptsize(dc,m_Font,&szf);
    EndPaint(m_Hwnd, &ps);
	

    double x_n=szf.cx,
           x_d=4,
           y_n=szf.cy,
           y_d=8;

	RECT rect;
	GetClientRect(m_Hwnd,&rect);	
	
	rect.right=rect.left+(int)(lpDialogTemplate->cx*x_n/x_d+0.5);
	rect.bottom=rect.top+(int)(lpDialogTemplate->cy*y_n/y_d+0.5);

	ClientToScreen(m_Hwnd, (LPPOINT)&rect);
	ClientToScreen(m_Hwnd, ((LPPOINT)&rect)+1);

    AdjustWindowRectEx(&rect, lpDialogTemplate->style, dd.has_menu, lpDialogTemplate->exStyle );
	MoveWindow(m_Hwnd,rect.left,rect.top,rect.right-rect.left,rect.bottom-rect.top,TRUE);


	helper::DLGITEMTEMPLATEEX *item=
		(helper::DLGITEMTEMPLATEEX *)helper::FindFirstDlgItem((DLGTEMPLATE *)lpDialogTemplate);

	HWND wnd;

	for(t=0;t<helper::DlgTemplateItemCount((DLGTEMPLATE *)lpDialogTemplate);t++)
	{
		// OLD IMPLEMENTATION: wnd=GetDlgItem(hwnd,item->id);

	    // TODO: check if it is the best implementation
	    //       are we sure that controls will preserve the order?

		if (!t) 
			wnd=GetWindow(m_Hwnd,GW_CHILD);
		else
			wnd=GetWindow(wnd,GW_HWNDNEXT);
		
		while ((wnd)&&(GetDlgCtrlID(wnd) != item->id)) 
			wnd=GetWindow(wnd,GW_HWNDNEXT);

		if (!wnd) break;
	    
		MoveWindow(wnd,(int)(item->x*x_n/x_d+0.5),
		               (int)(item->y*y_n/y_d+0.5),
		               (int)(item->cx*x_n/x_d+0.5),
		               (int)(item->cy*y_n/y_d+0.5), TRUE);

        SendMessage(wnd, WM_SETFONT, (LPARAM)m_Font, TRUE);
		
		item=(helper::DLGITEMTEMPLATEEX *)helper::FindNextDlgItem((DLGITEMTEMPLATE *)item,TRUE);
	}

    UnlockResource(hDialogTemplate);    
    FreeResource(hDialogTemplate);
}

//附加函数
void CSkintDPI::Attach(HINSTANCE inst,HWND dlg,UINT IDD,double dpi)
{
	Attach( inst,  dlg,  MAKEINTRESOURCE(IDD), dpi);
}

//分离函数
void CSkintDPI::Detach()
{
    if (!m_Hwnd) return;
    SendMessage(m_Hwnd, WM_SETFONT, (LPARAM)m_Oldfont, TRUE);


	HRSRC hResource = ::FindResource(m_Inst, m_lpszTemplateName, RT_DIALOG);
	HANDLE hDialogTemplate = LoadResource(m_Inst, hResource);
	helper::DLGTEMPLATEEX *lpDialogTemplate = 
		(helper::DLGTEMPLATEEX *)LockResource(hDialogTemplate);    


	helper::DLGITEMTEMPLATEEX *item=
		(helper::DLGITEMTEMPLATEEX *)helper::FindFirstDlgItem((DLGTEMPLATE *)lpDialogTemplate);

	int t;
	HWND wnd;
	for(t=0;t<helper::DlgTemplateItemCount((DLGTEMPLATE *)lpDialogTemplate);t++)
	{
		// OLD IMPLEMENTATION: wnd=GetDlgItem(hwnd,item->id);

	    // TODO: check if it is the best implementation
	    //       are we sure that controls will preserve the order?

		if (!t) 
			wnd=GetWindow(m_Hwnd,GW_CHILD);
		else
			wnd=GetWindow(wnd,GW_HWNDNEXT);
		
		while ((wnd)&&(GetDlgCtrlID(wnd) != item->id)) 
			wnd=GetWindow(wnd,GW_HWNDNEXT);

		if (!wnd) break;

        SendMessage(wnd, WM_SETFONT, (LPARAM)m_Oldfont, TRUE);		
		item=(helper::DLGITEMTEMPLATEEX *)helper::FindNextDlgItem((DLGITEMTEMPLATE *)item,TRUE);
	}

    UnlockResource(hDialogTemplate);    
    FreeResource(hDialogTemplate);


    DeleteObject(m_Font);
    m_Hwnd=0;
    m_Font=0;
    m_Oldfont=0;
}

