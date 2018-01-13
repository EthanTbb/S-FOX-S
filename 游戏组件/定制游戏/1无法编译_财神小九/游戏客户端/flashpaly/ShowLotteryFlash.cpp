#include "../Stdafx.h"
#include ".\ShowLotteryFlash.h"
//#include "../GlobalUnits.h"



const CString GET_GOLD = TEXT("Get_Gold");
const WCHAR FLASH_PATH_VAR[] = L"ShowInfo";

const DWORD IDM_MAIN_SHOWLOTERYFLASH = 10;
const DWORD IDM_SUB_EXIT = 0;


#ifdef _DEBUG
#define  new DEBUG_NEW
#endif



BEGIN_MESSAGE_MAP(CShowLotteryFlash2, CFlashShowWnd2)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETCURSOR()

	ON_MESSAGE(CM_FLASH_PLAY_END, OnPlayFlashEnd)
	//ON_MESSAGE(CM_CLICK_MENUITEM, OnClickMenuItem)
	ON_WM_CLOSE()
END_MESSAGE_MAP()





CShowLotteryFlash2::CShowLotteryFlash2(void)
{
	m_isSending = false;
	m_pIRoomViewItem = NULL;
}

CShowLotteryFlash2::~CShowLotteryFlash2(void)
{
}


void CShowLotteryFlash2::OnLButtonDown(UINT nFlags, CPoint point)
{
	//FLASH动画窗口不响应点、拖
	return;
	POINT	ptold, ptnow;

	GetCursorPos(&ptold);
	SendMessage(WM_SYSCOMMAND, 0xf012, 0);
	GetCursorPos(&ptnow);

	ptold.x -= ptnow.x;
	ptold.y -= ptnow.y;

	if (ptold.x>=-2 && ptold.x<=2 && 
		ptold.y>=-2 && ptold.y<=2)
	{
		m_pPlayer->MouseDown(point.x, point.y);
		m_pPlayer->MouseUp(point.x, point.y);
	}
	//CFlashShowWnd2::OnLButtonDown(nFlags, point);
}

void CShowLotteryFlash2::OnMouseMove(UINT nFlags, CPoint point)
{
	//FLASH动画窗口不响应点、拖
	return;
	m_pPlayer->MouseMove(point.x, point.y);
	CFlashShowWnd2::OnMouseMove(nFlags, point);
}

void CShowLotteryFlash2::OnLButtonUp(UINT nFlags, CPoint point)
{
	//FLASH动画窗口不响应点、拖
	return;
	CFlashShowWnd2::OnLButtonUp(nFlags, point);
}

void CShowLotteryFlash2::OnTimer(UINT nIDEvent)
{
	CFlashShowWnd2::OnTimer(nIDEvent);
}


void CShowLotteryFlash2::GiveShowInfo(BYTE Card[])
{
	if (m_pPlayer)
    {
        CString sCardDate;

        //if (m_nDesicType == 4)
        //{
        //    WCHAR FLASH_PATH_VAR6[] = L"go";
        //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR6, _bstr_t("1"));
        //    WCHAR FLASH_PATH_VAR7[] = L"bfirst";
        //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR7, _bstr_t("1"));
        //    WCHAR FLASH_PATH_VAR8[] = L"sound";
        //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR8, _bstr_t("0"));
        //} 
        //else
        //{
        //    WCHAR FLASH_PATH_VAR6[] = L"go";
        //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR6, _bstr_t("1"));
        //    WCHAR FLASH_PATH_VAR8[] = L"sound";
        //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR8, _bstr_t("0"));
        //}
        //

        //WCHAR FLASH_PATH_VAR1[] = L"dice1";
        //sCardDate.Format(TEXT("%d"),Card[0]);
        //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR1, _bstr_t(sCardDate));
        //WCHAR FLASH_PATH_VAR2[] = L"dice2";
        //sCardDate.Format(TEXT("%d"),Card[1]);
        //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR2, _bstr_t(sCardDate));
        //WCHAR FLASH_PATH_VAR3[] = L"dice3";
        //sCardDate.Format(TEXT("%d"),Card[2]);
        //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR3, _bstr_t(sCardDate));
        //WCHAR FLASH_PATH_VAR4[] = L"dice4";
        //sCardDate.Format(TEXT("%d"),Card[3]);
        //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR4, _bstr_t(sCardDate));
        //WCHAR FLASH_PATH_VAR5[] = L"dice5";
        //sCardDate.Format(TEXT("%d"),Card[4]);
        //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR5, _bstr_t(sCardDate));

        //m_pPlayer->m_pFlashInterface->Forward();
    }

	m_isSending = false;
}
void   CShowLotteryFlash2::StopFlash()
{
	if(m_pPlayer==NULL)
		return;
    m_pPlayer->m_pFlashInterface->Rewind();
    //m_pPlayer->m_pFlashInterface->GotoFrame(0);
    // m_pPlayer->m_pFlashInterface->Stop();
     m_pPlayer->m_pFlashInterface->StopPlay();
    // m_pPlayer->m_pFlashInterface->Play();
    // m_pPlayer->m_pFlashInterface->GotoFrame(0);

    //if (m_nDesicType == 4)
    //{
    //    WCHAR FLASH_PATH_VAR6[] = L"go";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR6, _bstr_t("0"));
    //    WCHAR FLASH_PATH_VAR7[] = L"bfirst";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR7, _bstr_t("0"));
    //    WCHAR FLASH_PATH_VAR8[] = L"sound";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR8, _bstr_t("0"));
    //} 
    //else
    //{
    //    WCHAR FLASH_PATH_VAR6[] = L"go";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR6, _bstr_t("0"));
    //    WCHAR FLASH_PATH_VAR8[] = L"sound";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR8, _bstr_t("0"));
    //}

    //WCHAR FLASH_PATH_VAR1[] = L"dice1";
    //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR1, _bstr_t("0"));
    //WCHAR FLASH_PATH_VAR2[] = L"dice2";
    //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR2, _bstr_t("0"));
    //WCHAR FLASH_PATH_VAR3[] = L"dice3";
    //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR3, _bstr_t("0"));
    //WCHAR FLASH_PATH_VAR4[] = L"dice4";
    //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR4, _bstr_t("0"));
    //WCHAR FLASH_PATH_VAR5[] = L"dice5";
    //m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR5, _bstr_t("0"));

     
    return;
}
void CShowLotteryFlash2::ShowFlash(CWnd *pIRoomViewItem, int showx, int showy,bool bShow)
{
	RECT	rc;
	PlayFlashAnimation playdat;


	rc.left = showx;
	rc.top = showy;
    if (m_nDesicType == 0)
    {
        rc.right = rc.left + SHOWLOTTERYFLASH_W;
        rc.bottom = rc.top + SHOWLOTTERYFLASH_H;
    } 
    else
    {
        rc.right = rc.left + SHOWLOTTERYFLASH1_W;
        rc.bottom = rc.top + SHOWLOTTERYFLASH1_H;
    }
	

	m_pIRoomViewItem = pIRoomViewItem;
	memset(&playdat, 0, sizeof(playdat));

	//sprintf(playdat.theFileName, "%s\\ShowLottery.swf", g_GlobalUnits.GetWorkDirectory());
    TCHAR szFilePath[MAX_PATH +1]={0};
    GetModuleFileName(NULL,szFilePath,MAX_PATH);
    (_tcsrchr(szFilePath,_T('\\')))[1] =0;
    CString str_url ;
    for(int n=0;szFilePath[n];n++)
    {
        if(szFilePath[n] !=_T('\\'))
        {
            str_url +=szFilePath[n];
        }
        else
        {
            str_url +=_T("\\\\");
        }
    }
    if (m_nDesicType == 0)
    {
        str_url  +=_T("GodIn\\\\flash\\\\GodIn.swf");
    } 
    else //if(m_nDesicType == 1)
    {
        str_url  +=_T("GodIn\\\\flash\\\\GodShow.swf");
    }
    _sntprintf(playdat.theFileName,CountArray(playdat.theFileName), str_url);


	playdat.ppWnd = pIRoomViewItem->m_hWnd;
	playdat.msgID = IDM_FALSH_FINISH;//CM_FLASH_PLAY_END;
	playdat.lprcshow = &rc;

	StartAnimation(&playdat,bShow);

    //if (m_nDesicType == 4)
    //{
    //    WCHAR FLASH_PATH_VAR6[] = L"go";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR6, _bstr_t("1"));
    //    WCHAR FLASH_PATH_VAR7[] = L"bfirst";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR7, _bstr_t("1"));
    //    WCHAR FLASH_PATH_VAR8[] = L"sound";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR8, _bstr_t("0"));
    //} 
    //else
    //{
    //    WCHAR FLASH_PATH_VAR6[] = L"go";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR6, _bstr_t("1"));
    //    WCHAR FLASH_PATH_VAR8[] = L"sound";
    //    m_pPlayer->m_pFlashInterface->SetVariable((WCHAR*)FLASH_PATH_VAR8, _bstr_t("0"));
    //}

   
    
}

void CShowLotteryFlash2::MoveFlash(CWnd *pIRoomViewItem, int showx, int showy)
{
	RECT	rc;
	rc.left = showx;
	rc.top = showy;
	if (m_nDesicType == 0)
	{
		rc.right = rc.left + SHOWLOTTERYFLASH_W;
		rc.bottom = rc.top + SHOWLOTTERYFLASH_H;
	} 
	else
	{
		rc.right = rc.left + SHOWLOTTERYFLASH1_W;
		rc.bottom = rc.top + SHOWLOTTERYFLASH1_H;
	}
	SetWindowPos(NULL, rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top, SWP_NOREDRAW);
}

LRESULT CShowLotteryFlash2::OnPlayFlashEnd(WPARAM wParam, LPARAM lParam)
{
	DestroyWindow();
	return S_OK;
}

bool CShowLotteryFlash2::DealFlashCommand(LPCSTR cmd, LPCSTR lparam)
{
	//if (stricmp(cmd, GET_GOLD) == 0 && m_isSending == false)
	//{
	//	m_isSending = true;
	//	//m_pIRoomViewItem->SendData(MDM_GF_TOOLS, SUB_TL_LOTTERY, NULL, 0);
	//	return true;
	//}
	return __super::DealFlashCommand(cmd, lparam);
}


void CShowLotteryFlash2::OnRButtonDown(UINT nFlags, CPoint point)
{
	//m_SkinMenu.DestroySkinMenu();
	//m_SkinMenu.CreateSkinMenu(this, IDM_MAIN_SHOWLOTERYFLASH);
	//
	//m_SkinMenu.AddMenuItem(IDM_SUB_EXIT, "退出");

	//ClientToScreen(&point);
	//m_SkinMenu.PopupMenu(point.x, point.y);
}


BOOL CShowLotteryFlash2::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	return FALSE;//CFlashShowWnd2::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CShowLotteryFlash2::OnClickMenuItem(WPARAM wParam, LPARAM lParam)
{
	PostMessage(WM_CLOSE);
	return 0;
}

void CShowLotteryFlash2::OnClose()
{
	DestroyWindow();
}




BOOL CShowLotteryFlash2::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message!=WM_TIMER)
		return TRUE;

	return CFlashShowWnd2::PreTranslateMessage(pMsg);
}
