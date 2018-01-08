#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "GameFrameUserView.h"

//////////////////////////////////////////////////////////////////////////////////

//���ƫ��
#define	BILL_T						10									//����ϸ�
#define	BILL_L						10									//������
#define	BILL_R						10									//����ҿ�
#define	BILL_B						0									//����¸�
#define	BILL_H						82									//����¸�
//�ؼ���ʾ
#define IDC_WEB_PUBLICIZE			100									//����ؼ�
#define IDC_BT_USER_MEDAL			101									//�һ�����
#define IDC_BT_LOVE_LINESS			102									//�һ�����

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameFrameUserView, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_BT_USER_MEDAL, OnBnClickedUserMedal)
	ON_BN_CLICKED(IDC_BT_LOVE_LINESS, OnBnClickedLoveLiness)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameUserView::CGameFrameUserView()
{
	//���ñ���
	m_pIClientKernel=NULL;
	m_pICurrentUserItem=NULL;

	return;
}

//��������
CGameFrameUserView::~CGameFrameUserView()
{
}

//���ù��
VOID CGameFrameUserView::SetGameBillInfo(WORD wKindID, WORD wServerID)
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szBillUrl[256]=TEXT("");
	_sntprintf(szBillUrl,CountArray(szBillUrl),TEXT("%s/Ads/GameFrame.aspx?KindID=%ld&ServerID=%ld"),pGlobalWebLink->GetPlatformLink(),wKindID,wServerID);

	//�򿪵�ַ
	m_GameFramePublicize.Navigate(szBillUrl);

	return;
}

//�����û�
VOID CGameFrameUserView::SetUserViewInfo(IClientUserItem * pIClientUserItem)
{
	//�����û�
	if (m_pICurrentUserItem!=pIClientUserItem)
	{
		//���ñ���
		m_pICurrentUserItem=pIClientUserItem;

		//���½���
		if (m_hWnd!=NULL)
		{
			RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
		}
	}

	return;
}

//�����ؼ�
VOID CGameFrameUserView::RectifyControl(INT nWidth, INT nHeight)
{
	////��ȡ��С
	CRect rcButton;
	//m_btUserMedal.GetWindowRect(&rcButton);

	//����λ��
	INT nXBorad=12L;
	INT nXButtonPos=(nWidth-rcButton.Width()*2L-nXBorad*2L)/3L;

	////������ť
	//m_btUserMedal.SetWindowPos(NULL,nXBorad+nXButtonPos,nHeight-rcButton.Height()-6,0,0,SWP_NOZORDER|SWP_NOSIZE);
	//m_btLoveLiness.SetWindowPos(NULL,nXBorad+nXButtonPos*2+rcButton.Width(),nHeight-rcButton.Height()-6,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//�������
	if (m_GameFramePublicize.m_hWnd!=NULL)
	{
		m_GameFramePublicize.SetWindowPos(NULL,BILL_L,BILL_T,nWidth-BILL_L-BILL_R,BILL_H,SWP_NOZORDER);
	}

	return;
}

//���ƶһ�
VOID CGameFrameUserView::OnBnClickedUserMedal()
{
	//��ȡ����
	CGlobalWebLink * pGlobalWebLink=CGlobalWebLink::GetInstance();

	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("%s/UserMedal.aspx"),pGlobalWebLink->GetPlatformLink());

	//��ҳ��
	ShellExecute(NULL,TEXT("OPEN"),szLogonLink,NULL,NULL,SW_NORMAL);

	return;
}

//�����һ�
VOID CGameFrameUserView::OnBnClickedLoveLiness()
{
	//�����ַ
	TCHAR szLogonLink[256]=TEXT("");
	_sntprintf(szLogonLink,CountArray(szLogonLink),TEXT("LoveLiness.aspx"));

	//��ҳ��
	CDlgBrowser DlgBrowser;
	DlgBrowser.CreateBrowserControl(TEXT("�����һ�"),szLogonLink,CSize(730,450));

	return;
}

//�滭����
BOOL CGameFrameUserView::OnEraseBkgnd(CDC * pDC)
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CImage ImageBuffer;
	ImageBuffer.Create(rcClient.Width(),rcClient.Height(),32);

	//���� DC
	CImageDC BufferDC(ImageBuffer);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//���û���
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextAlign(TA_LEFT|TA_TOP);
	pBufferDC->SetTextColor(RGB(124,77,36));
	pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//������Դ
	CImage ImageUserInfoL;
	CImage ImageUserInfoM;
	CImage ImageUserInfoR;
	ImageUserInfoL.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_INFO_L);
	ImageUserInfoM.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_INFO_M);
	ImageUserInfoR.LoadFromResource(GetModuleHandle(GAME_FRAME_DLL_NAME),IDB_USER_INFO_R);

	//��������
	INT nXStartPos=ImageUserInfoL.GetWidth();
	INT nXConcludePos=rcClient.Width()-ImageUserInfoR.GetWidth();

	//�滭���
	ImageUserInfoL.BitBlt(BufferDC,0,0);
	ImageUserInfoR.BitBlt(BufferDC,rcClient.Width()-ImageUserInfoR.GetWidth(),0);

	//�滭�м�
	for (INT nXPos=nXStartPos;nXPos<nXConcludePos;nXPos+=ImageUserInfoM.GetWidth())
	{
		INT nCXDraw=__min(ImageUserInfoM.GetWidth(),nXConcludePos-nXPos);
		ImageUserInfoM.BitBlt(BufferDC,nXPos,0,nCXDraw,ImageUserInfoM.GetHeight(),0,0);
	}

	//�滭�û�
	if (m_pICurrentUserItem!=NULL)
	{
		//��������
		INT nYExcursion=106L;
		INT nXExcursion = 19L;
		TCHAR szString[128]=TEXT("");

		//��������
		tagUserInfo * pUserInfo=m_pICurrentUserItem->GetUserInfo();
		tagGameAttribute * pGameAttribute=m_pIClientKernel->GetGameAttribute();
		tagCustomFaceInfo * pCustomFaceInfo=m_pICurrentUserItem->GetCustomFaceInfo();

		//��������
		CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();
		CFaceItemControl * pFaceItemControl=CFaceItemControl::GetInstance();
		CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

		//�ӿڱ���
		IUserOrderParser * pIUserOrderParser=pGlobalUnits->m_UserOrderParserModule.GetInterface();
		IGameLevelParser * pIGameLevelParser=pGlobalUnits->m_GameLevelParserModule.GetInterface();

		//�滭ͷ��
		if ((pCustomFaceInfo->dwDataSize!=0L)&&(m_pICurrentUserItem->GetCustomID()!=0L))
		{
			pFaceItemControl->DrawFaceNormal(pBufferDC,nXExcursion,nYExcursion+14,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pFaceItemControl->DrawFaceNormal(pBufferDC,nXExcursion,nYExcursion+14,m_pICurrentUserItem->GetFaceID());
		}

		//λ�ö���
		int nVStart = 6;
		int nVSpace = 18;
		int nVHight = 14;
		int nHLSpace = 54;
		int nHRSpace = 12;

		int x = nXExcursion +nHLSpace;
		int y = nYExcursion+nVStart+nVSpace*0;
		int w = rcClient.Width()-nHRSpace;
		int h = y+nVHight;
		CRect rcNickName (x,y,w,h);
		CRect rcGameLevel(x,y+nVSpace,w,h+nVSpace);
		CRect rcQQ       (x,y+nVSpace*2,w,h+nVSpace*2);
		CRect rcMobilePhone(x,y+nVSpace*3,w,h+nVSpace*3);
		CRect rcUserUnderWrite(x,y+nVSpace*4,w,h+nVSpace*4);
		

		//λ�ö���
		CRect rcGameSorce(nXExcursion,nYExcursion+102,rcClient.Width()-12,nYExcursion+114);
		CRect rcGameDrawInfo(nXExcursion,nYExcursion+122,rcClient.Width()-12,nYExcursion+134);
		CRect rcGameDrawInfo2(nXExcursion,nYExcursion+142,rcClient.Width()-12,nYExcursion+154);

		//������Ϣ
		DWORD dwExperience=m_pICurrentUserItem->GetUserExperience();
		WORD wExperienceLevel=pUserItemElement->GetExperienceLevel(dwExperience);

		//�û���Ϣ
		LPCTSTR pszMember=pIUserOrderParser->GetMemberDescribe(m_pICurrentUserItem->GetMemberOrder());
		LPCTSTR pszMaster=pIUserOrderParser->GetMasterDescribe(m_pICurrentUserItem->GetMasterOrder());

		//�û���Ϣ
		LPCTSTR pszUserOrder=(pszMaster[0]!=0)?pszMaster:((pszMember[0]!=0)?pszMember:TEXT("��ͨ���"));
		_sntprintf(szString,CountArray(szString),TEXT("�ǳƣ�%s [ %s ]"),m_pICurrentUserItem->GetNickName(),pszUserOrder);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//���ֵȼ�
		LPCTSTR pszGameLevel=pIGameLevelParser->GetLevelDescribe(m_pICurrentUserItem);
		_sntprintf(szString,CountArray(szString),TEXT("����%s [ %s ]"),pszGameLevel,pGameAttribute->szGameName);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//QQ����	
		_sntprintf(szString,CountArray(szString),TEXT("  QQ��%s"),pUserInfo->szQQ);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcQQ,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�ֻ�����
		_sntprintf(szString,CountArray(szString),TEXT("�ֻ���%s"),pUserInfo->szMobilePhone);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcMobilePhone,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//����ǩ��
		_sntprintf(szString,CountArray(szString),TEXT("ǩ����%s"),pUserInfo->szUnderWrite);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcUserUnderWrite,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�ɼ�
		_sntprintf(szString,CountArray(szString),TEXT("�ɼ���%I64d"),pUserInfo->lScore);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcGameSorce,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		//��Ϸ��Ϣ
		float fUserWinRate=m_pICurrentUserItem->GetUserWinRate();
		_sntprintf(szString,CountArray(szString),TEXT("ʤ�ʣ�%.2f%%"),fUserWinRate);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcGameDrawInfo,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

		
		float fUserFleeRate=m_pICurrentUserItem->GetUserFleeRate();
		_sntprintf(szString,CountArray(szString),TEXT("�����ʣ�%.2f%%"),fUserFleeRate);
		pBufferDC->DrawText(szString,lstrlen(szString),&rcGameDrawInfo2,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
	}

	//�滭����
	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),pBufferDC,0,0,SRCCOPY);

	return TRUE;
}

//λ����Ϣ
VOID CGameFrameUserView::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	RectifyControl(cx,cy);

	return;
}

//������Ϣ
INT CGameFrameUserView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��ѯ�ӿ�
	ASSERT(CGlobalUnits::GetInstance()->m_ClientKernelModule.GetInterface()!=NULL);
	m_pIClientKernel=CGlobalUnits::GetInstance()->m_ClientKernelModule.GetInterface();

	//�������
	CRect rcCreate(0,0,0,0);
	/*m_GameFramePublicize.SetBoradColor(RGB(0,0,0));*/
	m_GameFramePublicize.Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN,rcCreate,this,IDC_WEB_PUBLICIZE);

	return 0;
}

//////////////////////////////////////////////////////////////////////////////////
