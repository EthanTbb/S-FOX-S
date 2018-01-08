#include "StdAfx.h"
#include "Resource.h"
#include "Dlgtaskcenter.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "PlazaViewUserInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//��������
#define ROUND_CX							10
#define ROUND_CY							10

////////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTaskGuide::CDlgTaskGuide(void) : CFGuiDialog(IDD_DLG_TASK_GUIDE)
{
	//��Դ����
	m_btLook=NULL;
	m_btReturn=NULL;

	//��������
	m_FontText.CreateFont(12,0,0,0,100,0,0,0,134,3,2,1,2,TEXT("����"));	
}

//��������
CDlgTaskGuide::~CDlgTaskGuide(void)
{
}

//��ʼ�ؼ�
void CDlgTaskGuide::InitControlUI()
{
	//���ñ���
	CRect rcCaption(0,0,0,75);
	m_PaintManager.SetCaptionRect(rcCaption);

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//�鿴��ť
	m_btLook = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btLook->SetStatusImage(TEXT("file='BT_TASK_LOOK' restype='PNG'"));
	m_btLook->SetPos(CRect(22,82,74,105));

	//���ذ�ť
	m_btReturn = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btReturn->SetStatusImage(TEXT("file='BT_TASK_RETURN' restype='PNG'"));
	m_btReturn->SetPos(CRect(90,82,142,105));
}

//��Ϣ����
void CDlgTaskGuide::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//�˳���ť
		if(m_btReturn==pControlUI) return OnCancel(); 

		//�鿴����
		if(m_btLook==pControlUI)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_TASKVIEW,0L);

			return OnCancel();
		}
	}
}

//��ʼ�滭
void CDlgTaskGuide::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKGUIDE_BACK"));

	//���Ʊ���
	ImageBack.DrawImage(pDC,0,0);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(m_FontText);

	//��������
	TCHAR szGuideText1[32]=TEXT("    �װ�����ң������컹û");
	TCHAR szGuideText2[32]=TEXT("��ȡ����Ŷ����\"�鿴\"�Ͻ���");
	TCHAR szGuideText3[32]=TEXT("ȡ�����ջ����ɣ�");
	CRect rcGuideText(10,23,172,43);
	pDC->DrawText(szGuideText1,rcGuideText,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	rcGuideText.OffsetRect(0,18);
	pDC->DrawText(szGuideText2,rcGuideText,DT_LEFT|DT_WORDBREAK|DT_END_ELLIPSIS);

	rcGuideText.OffsetRect(0,18);
	pDC->DrawText(szGuideText3,rcGuideText,DT_LEFT|DT_WORDBREAK|DT_END_ELLIPSIS);

	return;
}

//��������
BOOL CDlgTaskGuide::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸ķ��
	ModifyStyleEx(0,WS_EX_LAYERED);

	//����͸��
	SetLayeredWindowAttributes(0,225,LWA_ALPHA);

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKGUIDE_BACK"));

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.left=4;
	rcUnLayered.top=16;	
	rcUnLayered.right=rcWindow.Width()-22;
	rcUnLayered.bottom=rcWindow.Height()-10;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,220,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//��Ϣ����
BOOL CDlgTaskGuide::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE) return TRUE;
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȡ���¼�
VOID CDlgTaskGuide::OnCancel()
{
	__super::OnCancel();

	//���ٴ���
	DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////


//���캯��
CDlgTaskNotifyer::CDlgTaskNotifyer(void)/* : CFGuiDialog(IDD_DLG_SIMPLE)*/
{
	//��Ϣ����
	m_szTaskName[0]=0;

	//��Դ����
	m_btClose=NULL;

	//��������
	m_FontText.CreateFont(13,0,0,0,100,0,0,0,134,3,2,1,2,TEXT("����"));	
}

//��������
CDlgTaskNotifyer::~CDlgTaskNotifyer(void)
{
}

//��ʼ�ؼ�
void CDlgTaskNotifyer::InitControlUI()
{
	//��������
	CRect rcCaption(0,0,0,50);
	m_PaintManager.SetCaptionRect(rcCaption);

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//�رհ�ť
	m_btClose = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btClose->SetStatusImage(TEXT("file='BT_NOTIFYER_CLOSE' restype='PNG'"));
	m_btClose->SetFadeOut(false);
	m_btClose->SetPos(CRect(195,5,215,25));

	//������ǩ
	m_TextReward = CTextUI::Create(&m_PaintManager,pParent,TEXT(""));	
	m_TextReward->SetText(TEXT("{a}{f 2}��ȥ��ȡ�����ɣ�{/f}{/a}"));
	m_TextReward->SetFixedXY(CPoint(102,67));
	m_TextReward->SetFixedWidth(120);
	m_TextReward->SetFixedHeight(20);
	m_TextReward->SetShowHtml();	
}

//��Ϣ����
void CDlgTaskNotifyer::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(m_btClose==pControlUI) return OnCancel(); 
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(pControlUI==m_TextReward)
		{
			//�����¼�
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_TASKVIEW,0L);

			return OnCancel();
		}
	}
}

//��������
VOID CDlgTaskNotifyer::SetTaskName(LPCTSTR pszTaskName)
{
	//�����ַ�
	lstrcpyn(m_szTaskName,pszTaskName,CountArray(m_szTaskName));

	return;
}

//��ʼ�滭
void CDlgTaskNotifyer::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKNOTIFYER_BACK"));

	//���Ʊ���
	ImageBack.DrawImage(pDC,0,0);

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(m_FontText);

	//��������
	TCHAR szNotifyText[128]=TEXT("");
	CRect rcNotifyText(9,30,212,73);
	_sntprintf(szNotifyText,CountArray(szNotifyText),TEXT("    ��ϲ���Ѿ����������\"%s\"��"),m_szTaskName);
	pDC->DrawText(szNotifyText,rcNotifyText,DT_LEFT|DT_WORDBREAK|DT_END_ELLIPSIS);

	return;
}

//��������
BOOL CDlgTaskNotifyer::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyleEx(0,WS_EX_TOPMOST);

	//������Դ
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKNOTIFYER_BACK"));

	//����λ��
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//���ô�С
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,rcArce.right-SizeWindow.cx,rcArce.bottom-SizeWindow.cy,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOREDRAW);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.left=4;
	rcUnLayered.top=4;	
	rcUnLayered.right=rcWindow.Width()-4;
	rcUnLayered.bottom=rcWindow.Height()-4;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//��Ϣ����
BOOL CDlgTaskNotifyer::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE) return TRUE;
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//ȡ���¼�
VOID CDlgTaskNotifyer::OnCancel()
{
	__super::OnCancel();

	//���ٴ���
	DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTaskCenter::CDlgTaskCenter(void) : CSkinDialog(IDD_DLG_POPUP)
{
	//��ʶ����
	m_bLableRound=false;

	//�������
	m_wTaskID=0;
	m_pTaskStatus=NULL;
	m_pTaskParameter=NULL;

	//�ؼ�����
	m_pTaskList=NULL;
	m_btTaskTake=NULL;

	//�ڵ����
	m_pNodeSelfTask=NULL;
	m_pNodeAllTask=NULL;

	//���ñ���
	m_rcItemSelected.SetRect(0,0,0,0);	

	//��������
	m_FontText.CreateFont(18,0,0,0,FW_BOLD,0,0,0,134,3,2,1,2,TEXT("������"));	
}

//��������
CDlgTaskCenter::~CDlgTaskCenter(void)
{
	//��������
	WORD wKey=0;
	POSITION Position;

	//ɾ������	
	tagTaskParameter * pTaskParameter=NULL;
	Position=m_TaskParameterMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskParameterMap.GetNextAssoc(Position,wKey,pTaskParameter);
		SafeDelete(pTaskParameter);
	}
	m_TaskParameterMap.RemoveAll();

	//ɾ��״̬
	tagTaskStatus * pTaskStatus=NULL;
	Position=m_TaskStatusMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskStatusMap.GetNextAssoc(Position,wKey,pTaskStatus);
		SafeDelete(pTaskStatus);
	}
	m_TaskStatusMap.RemoveAll();
	
}

//��ʼ�ؼ�
void CDlgTaskCenter::InitControlUI()
{
	__super::InitControlUI();

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//��ȡ����
	m_btTaskTake = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btTaskTake->SetStatusImage(TEXT("file='BT_TASK_TAKE' restype='PNG'"));
	m_btTaskTake->SetPos(CRect(277,285,407,331));
	m_btTaskTake->SetVisible(false);

	//��ȡ����
	m_btTakeReward = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btTakeReward->SetStatusImage(TEXT("file='BT_TASK_REWARD' restype='PNG'"));
	m_btTakeReward->SetPos(CRect(277,285,407,331));
	m_btTakeReward->SetVisible(false);

	//�����б�
	m_pTaskList = new CTaskUI(m_PaintManager); //(&m_PaintManager,pParent,TEXT("ListUI"));	
	m_pTaskList->SetManager(&m_PaintManager,pParent);
	m_pTaskList->SetAlternateBk(false);
	m_pTaskList->SetFixedWidth(161);
	m_pTaskList->SetFixedHeight(320);	
	m_pTaskList->SetFixedXY(CPoint(12,33));	
	m_pTaskList->EnableScrollBar();
	m_pTaskList->SetFloat();
	pParent->Add(m_pTaskList);

	//�б���Ϣ
	TListInfoUI * pListInfoUI = m_pTaskList->GetListInfo();
	pListInfoUI->rcTextPadding=CRect(4,0,0,0);
	pListInfoUI->dwHotBkColor=ARGB(255,128,110,84);
	pListInfoUI->dwSelectedBkColor=0;

	//��������
	tagTaskListItemInfo TaskItem;

	//�ҵ�����
	TaskItem.bIsFolder=true;
	TaskItem.bIsEmpty=false;
	lstrcpyn(TaskItem.szTaskName,TEXT("�ҵ�����"),CountArray(TaskItem.szTaskName));
	m_pNodeSelfTask = m_pTaskList->AddNode(TaskItem,NULL);

	//��������
	TaskItem.bIsFolder=true;
	TaskItem.bIsEmpty=false;
	lstrcpyn(TaskItem.szTaskName,TEXT("��������"),CountArray(TaskItem.szTaskName));
	m_pNodeAllTask = m_pTaskList->AddNode(TaskItem,NULL);
}

//��Ϣ����
void CDlgTaskCenter::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_btTaskTake && m_pTaskParameter!=NULL) 
		{
			//��ȡ����
			return PerformTakeTask();
		}
		else if(pControlUI==m_btTakeReward && m_pTaskParameter!=NULL) 
		{
			//��ȡ����
			return PerformRewardTask();
		}
	}
    else if (lstrcmp(msg.sType, TEXT("itemselect")) == 0 ||
			 lstrcmp(msg.sType, TEXT("itemclick")) == 0)		
	{
		//�ؼ��ж�
		if(m_pTaskList == NULL) return;

		if(lstrcmp(msg.sType, TEXT("itemclick")) == 0 && m_pTaskList->GetItemIndex(msg.pSender) != -1)
		{
			if (lstrcmp(pControlUI->GetClass(), TEXT("ListContainerElementUI")) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();

				if (m_pTaskList->CanExpand(node))
				{
					m_pTaskList->SetChildVisible(node, !node->Data().bChildVisible);

					return;
				}
			}			
		}

		if(lstrcmp(msg.sType, TEXT("itemselect")) == 0 && m_pTaskList->GetCurSel() != -1)
		{
            if (lstrcmp(pControlUI->GetClass(), TEXT("ListUI")) == 0)
			{
				CControlUI * pListItem = m_pTaskList->GetItemAt(m_pTaskList->GetCurSel());
				if(pListItem == NULL) return;

				Node* node = (Node*)pListItem->GetTag();
				if (m_pTaskList->CanExpand(node)==false)
				{
					tagTaskParameter * pTaskParameter = (tagTaskParameter *)pListItem->GetUserData();
					if(pTaskParameter!=NULL)
					{
						//���ñ���
						m_rcItemSelected = pListItem->GetPos();
						m_pTaskParameter=pTaskParameter;

						//���Ҷ���
						m_pTaskStatus=NULL;
						m_TaskStatusMap.Lookup(m_pTaskParameter->wTaskID,m_pTaskStatus);

						//��������
						CRect rcTaskList(7,45,0,359);

						//��������
						if(m_rcItemSelected.top<rcTaskList.top) m_rcItemSelected.OffsetRect(0,rcTaskList.top-m_rcItemSelected.top+6);
						if(m_rcItemSelected.bottom>rcTaskList.bottom) m_rcItemSelected.OffsetRect(0,rcTaskList.bottom-m_rcItemSelected.bottom-6);

						//�����ؼ�
						RectifyControlUI();

						//���½���
						RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
					}
				}
			}
		}		
	}
}

//�滭����
VOID CDlgTaskCenter::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//��Դ���
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//���ر���
	CPngImage ImageLogo;
	ImageLogo.LoadImage(hInstance,TEXT("TASK_TITLE"));

	//���Ʊ���
	ImageLogo.DrawImage(pDC,10,10);
}

//�滭��Ϣ
VOID CDlgTaskCenter::OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight)
{
	//��Դ���
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//������
	CRect rcTaskList(7,45,0,359);
	CRect rcTaskAbout(185,43,0,358);

	//������Դ
	CPngImage ImageTaskListT,ImageTaskListM,ImageTaskListB;
	ImageTaskListT.LoadImage(hInstance,TEXT("TASK_LIST_T"));
	ImageTaskListM.LoadImage(hInstance,TEXT("TASK_LIST_M"));
	ImageTaskListB.LoadImage(hInstance,TEXT("TASK_LIST_B"));

	//������Դ
	CPngImage ImageTaskAboutT,ImageTaskAboutM,ImageTaskAboutB;
	ImageTaskAboutT.LoadImage(hInstance,TEXT("TASK_ABOUT_T"));
	ImageTaskAboutM.LoadImage(hInstance,TEXT("TASK_ABOUT_M"));
	ImageTaskAboutB.LoadImage(hInstance,TEXT("TASK_ABOUT_B"));

	//�����б�
	ImageTaskListT.DrawImage(pDC,rcTaskList.left,rcTaskList.top);
	for(INT nYPos=ImageTaskListT.GetHeight();nYPos<rcTaskList.Height()-ImageTaskListB.GetHeight();nYPos += ImageTaskListM.GetHeight())
	{
		ImageTaskListM.DrawImage(pDC,rcTaskList.left,rcTaskList.top+nYPos);
	}
	ImageTaskListB.DrawImage(pDC,rcTaskList.left,rcTaskList.bottom-ImageTaskListB.GetHeight());

	//���Ƽ��
	ImageTaskAboutT.DrawImage(pDC,rcTaskAbout.left,rcTaskAbout.top);
	for(INT nYPos=ImageTaskAboutT.GetHeight();nYPos<rcTaskAbout.Height()-ImageTaskAboutB.GetHeight();nYPos += ImageTaskAboutM.GetHeight())
	{
		ImageTaskAboutM.DrawImage(pDC,rcTaskAbout.left,rcTaskAbout.top+nYPos);
	}
	ImageTaskAboutB.DrawImage(pDC,rcTaskAbout.left,rcTaskAbout.bottom-ImageTaskAboutB.GetHeight());

	//������Ϣ
	if(m_pTaskParameter!=NULL)
	{
		//������Դ
		CPngImage ImageArrow;
		ImageArrow.LoadImage(hInstance,TEXT("TASK_ARROW"));

		//���Ƽ�ͷ
		CSize SizeArrow(ImageArrow.GetWidth(),ImageArrow.GetHeight());
		ImageArrow.DrawImage(pDC,rcTaskAbout.left-SizeArrow.cx+4,m_rcItemSelected.top+(m_rcItemSelected.Height()-SizeArrow.cy)/2);

		//���û���
		pDC->SetTextColor(RGB(75,0,1));
		pDC->SelectObject(m_FontText);

		//��������
		CRect rcTaskName(rcTaskAbout.left+10,rcTaskAbout.top+10,rcTaskAbout.left+ImageTaskAboutT.GetWidth(),rcTaskAbout.top+35);
		pDC->DrawText(m_pTaskParameter->szTaskName,rcTaskName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);	

		//���û���
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//�������
		CRect rcTaskProgress(rcTaskAbout.left+38,rcTaskAbout.top+61 ,rcTaskAbout.left+92,rcTaskAbout.top+77);			
		pDC->DrawText(TEXT("�������:"),rcTaskProgress,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);	

		//�������
		TCHAR szTaskProgress[32]=TEXT("");
		CRect rcTaskProgress1(rcTaskAbout.left+96,rcTaskAbout.top+61,rcTaskAbout.left+293,rcTaskAbout.top+106);	
		_sntprintf(szTaskProgress,CountArray(szTaskProgress),TEXT("%d/%d"),(m_pTaskStatus?m_pTaskStatus->wTaskProgress:0),m_pTaskParameter->wTaskObject);
		pDC->DrawText(szTaskProgress,rcTaskProgress1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);	

		//ʱ������
		CRect rcTimeLimit(rcTaskAbout.left+38,rcTaskAbout.top+90,rcTaskAbout.left+92,rcTaskAbout.top+106);
		CRect rcTimeLimit1(rcTaskAbout.left+96,rcTaskAbout.top+90,rcTaskAbout.left+293,rcTaskAbout.top+106);
		pDC->DrawText(TEXT("ʱ������:"),rcTimeLimit,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//��ʽ��ʱ��
		TCHAR szTaskTimeLimit[128]=TEXT("");
		WORD wHour = (WORD)(m_pTaskParameter->dwTimeLimit/3600);
		WORD wMinute = (WORD)((m_pTaskParameter->dwTimeLimit-wHour*3600)/60);
		WORD wSecond = (WORD)(m_pTaskParameter->dwTimeLimit-wHour*3600-wMinute*60);
		_sntprintf(szTaskTimeLimit,CountArray(szTaskTimeLimit),TEXT("%02dʱ:%02d��:%02d��"),wHour,wMinute,wSecond);
		pDC->DrawText(szTaskTimeLimit,rcTimeLimit1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//����˵��
		CRect rcTaskDescribe(rcTaskAbout.left+38,rcTaskAbout.top+120,rcTaskAbout.left+92,rcTaskAbout.top+136);
		CRect rcTaskDescribe1(rcTaskAbout.left+96,rcTaskAbout.top+120,rcTaskAbout.left+310,rcTaskAbout.top+150);
		pDC->DrawText(TEXT("����˵��:"),rcTaskDescribe,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//��������
		pDC->DrawText(m_pTaskParameter->szTaskDescribe,rcTaskDescribe1,DT_CALCRECT|DT_WORDBREAK);
    	pDC->DrawText(m_pTaskParameter->szTaskDescribe,rcTaskDescribe1,DT_LEFT|DT_TOP|DT_WORDBREAK|DT_END_ELLIPSIS);

		//��������
		INT nRewardYPos=rcTaskAbout.top+152;
		if(nRewardYPos<rcTaskDescribe1.bottom+8) nRewardYPos=rcTaskDescribe1.bottom+8;

		//������
		CRect rcTaskReward(rcTaskAbout.left+38,nRewardYPos,rcTaskAbout.left+92,nRewardYPos+18);		
		pDC->DrawText(TEXT("������:"),rcTaskReward,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//������
		TCHAR szTaskReward[64]=TEXT("");
		CRect rcTaskReward1(rcTaskAbout.left+96,nRewardYPos,rcTaskAbout.left+300,nRewardYPos+18);		
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��ͨ��ң� ��Ϸ�� +%I64d"),m_pTaskParameter->lStandardAwardGold);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		rcTaskReward1.OffsetRect(0,16);
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��ͨ��ң� Ԫ�� +%I64d "),m_pTaskParameter->lStandardAwardMedal);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		rcTaskReward1.OffsetRect(0,16);
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��Ա��ң� ��Ϸ�� +%I64d"),m_pTaskParameter->lMemberAwardGold);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		rcTaskReward1.OffsetRect(0,16);
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("��Ա��ң� Ԫ�� +%I64d"),m_pTaskParameter->lMemberAwardMedal);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

	}
}

//��ʼ����
VOID CDlgTaskCenter::OnMissionStart()
{
	//ʹ������
	if(m_pTaskList!=NULL) m_pTaskList->SetEnabled(false);
	if(m_btTaskTake!=NULL) m_btTaskTake->SetEnabled(false);
	if(m_btTakeReward!=NULL) m_btTakeReward->SetEnabled(false);
}

//��ֹ����
VOID CDlgTaskCenter::OnMissionConclude()
{
	//ʹ������
	if(m_pTaskList!=NULL) m_pTaskList->SetEnabled(true);
	if(m_btTaskTake!=NULL) m_btTaskTake->SetEnabled(true);
	if(m_btTakeReward!=NULL) m_btTakeReward->SetEnabled(true);	
}

//�����ؼ�
VOID CDlgTaskCenter::RectifyControlUI()
{
	//��������
	if(m_pTaskStatus && m_pTaskStatus->cbTaskStatus==TASK_STATUS_SUCCESS)
	{
		m_btTaskTake->SetVisible(false);
		m_btTakeReward->SetVisible(true);
		m_btTakeReward->SetWantReturn();
	}
	else
	{		
		m_btTaskTake->SetVisible(true);
		m_btTaskTake->SetWantReturn();
		m_btTakeReward->SetVisible(false);
	}

	//ʹ������
	if(m_pTaskParameter!=NULL) m_btTaskTake->SetEnabled(true);
	if(m_pTaskParameter==NULL || m_pTaskStatus!=NULL) m_btTaskTake->SetEnabled(false);

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
}

//Ĭ��ѡ��
VOID CDlgTaskCenter::SelectDefaultItem()
{
	if(m_pTaskParameter == NULL) return;

	//���ҽڵ�
	Node * pSelectNode = m_pTaskList->SearchNode(m_pNodeSelfTask,m_pTaskParameter->wTaskID);			
	if(pSelectNode == NULL)
	{
		m_pTaskList->SearchNode(m_pNodeAllTask,m_pTaskParameter->wTaskID);			
	}

	//ѡ������
	if(pSelectNode!=NULL)
	{
		m_pTaskList->SelectItem(pSelectNode->Data().plistElment->GetIndex());
	}
}

//�������
BOOL CDlgTaskCenter::AddMineTaskItem(tagTaskStatus & TaskStatus)
{
	//��������
	tagTaskStatus * pTaskStatus=NULL;

	//������Դ
	try
	{
		pTaskStatus = new tagTaskStatus;
		if(pTaskStatus==NULL) throw(TEXT("�ڴ治�㣡"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//��������
	CopyMemory(pTaskStatus,&TaskStatus,sizeof(tagTaskStatus));
	m_TaskStatusMap[pTaskStatus->wTaskID]=pTaskStatus;

	//���Ҳ���
	tagTaskParameter * pTaskParameter=NULL;
	if(m_TaskParameterMap.Lookup(pTaskStatus->wTaskID,pTaskParameter)==TRUE)
	{
		//��������ȡ
		if(pTaskStatus->cbTaskStatus != TASK_STATUS_REWARD)
		{
			//��������
			tagTaskListItemInfo TaskItem;

			//��������
			TaskItem.bIsFolder=false;
			TaskItem.bIsEmpty=false;
			TaskItem.bShowStatus=true;
			TaskItem.wTaskID=pTaskParameter->wTaskID;
			TaskItem.cbTaskStatus=pTaskStatus->cbTaskStatus;
			lstrcpyn(TaskItem.szTaskName,pTaskParameter->szTaskName,CountArray(TaskItem.szTaskName));
			Node * pNodeTask = m_pTaskList->AddNode(TaskItem,m_pNodeSelfTask);
			if(pNodeTask) pNodeTask->Data().plistElment->SetUserData(pTaskParameter);

			//Ĭ����ʾ
			if(m_pTaskStatus==NULL)
			{
				m_pTaskStatus=pTaskStatus;
				m_pTaskParameter=pTaskParameter;
			}

			//Ĭ�����
			if(pTaskStatus->cbTaskStatus==TASK_STATUS_SUCCESS)
			{
				m_pTaskStatus=pTaskStatus;
				m_pTaskParameter=pTaskParameter;
			}
		}

		//ɾ������
		Node * pTaskNode=m_pTaskList->SearchNode(m_pNodeAllTask,pTaskParameter->wTaskID);
		if(pTaskNode!=NULL) m_pTaskList->RemoveNode(pTaskNode);
	}		

	return TRUE;
}

//�������
BOOL CDlgTaskCenter::AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize)
{
	//����У��
	ASSERT(pTaskParameterData!=NULL && wParameterSize>0);
	if(pTaskParameterData==NULL || wParameterSize==0) return FALSE;

	//��������
	tagTaskParameter * pTaskParameter=NULL;

	//������Դ
	try
	{
		pTaskParameter = new tagTaskParameter;
		if(pTaskParameter==NULL) throw(TEXT("�ڴ治�㣡"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//��������
	CopyMemory(pTaskParameter,pTaskParameterData,wParameterSize);
	m_TaskParameterMap[pTaskParameter->wTaskID]=pTaskParameter;

	//��������
	tagTaskListItemInfo TaskItem;

	//��������
	TaskItem.bIsEmpty=false;
	TaskItem.bIsFolder=false;
	TaskItem.bShowStatus=false;
	TaskItem.wTaskID=pTaskParameter->wTaskID;
	lstrcpyn(TaskItem.szTaskName,pTaskParameter->szTaskName,CountArray(TaskItem.szTaskName));
	Node * pNodeTask = m_pTaskList->AddNode(TaskItem,m_pNodeAllTask);
	if(pNodeTask) pNodeTask->Data().plistElment->SetUserData(pTaskParameter);

	//Ĭ����ʾ
	if(m_pTaskParameter == NULL) 
	{
		m_pTaskParameter=pTaskParameter;
	}

	return TRUE;
}

//����״̬
VOID CDlgTaskCenter::UpdateTaskStatus(WORD wTaskID,BYTE cbTaskStatus)
{
	//���Ҷ���	
	Node * pTaskNode=m_pTaskList->SearchNode(m_pNodeSelfTask,wTaskID);
	if(pTaskNode)
	{
		if(cbTaskStatus != TASK_STATUS_REWARD)
		{
			//����״̬
			pTaskNode->Data().cbTaskStatus=cbTaskStatus;
			m_pTaskList->UpdateNode(pTaskNode);
		}
		else
		{
			//�Ƴ�����
			m_pTaskList->RemoveNode(pTaskNode);
		}
	}
}

//��������
BOOL CDlgTaskCenter::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	SetWindowPos(NULL,0,0,501,369,SWP_NOZORDER|SWP_NOMOVE);

	//���д���
	CenterWindow(this);

	//��������
	PerformLoadTask();

	return FALSE;
}

//��Ϣ����
BOOL CDlgTaskCenter::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTaskPlazaCenter::CDlgTaskPlazaCenter()
{
	//�����ʶ
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=false;

	//��������
	m_MissionManager.InsertMissionItem(this);
}

//��������
CDlgTaskPlazaCenter::~CDlgTaskPlazaCenter()
{
}

//�����¼�
bool CDlgTaskPlazaCenter::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//��ʾ��ʾ
		CInformation Information;
		Information.ShowMessageBox(TEXT("�޷����ӵ���¼��������������Ϣ����ʧ�ܣ����Ժ����ԣ�"),MB_ICONERROR);
	}
	else
	{
		//��������
		if(m_bLoadTask==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskLoadInfo TaskLoadInfo;
			TaskLoadInfo.dwUserID = pGlobalUserData->dwUserID;
			lstrcpyn(TaskLoadInfo.szPassword,pGlobalUserData->szPassword,CountArray(TaskLoadInfo.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_LOAD,&TaskLoadInfo,sizeof(TaskLoadInfo));

			return true;
		}

		//��ȡǩ��
		if(m_bTakeTask==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskTake TaskTake;
			TaskTake.wTaskID = m_wTaskID;
			TaskTake.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskTake.szMachineID);
			lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_TAKE,&TaskTake,sizeof(TaskTake));

			return true;
		}

		//��ȡ����
		if(m_bTaskReward==true)
		{
			//��ȡ����
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//����ṹ
			CMD_GP_TaskReward TaskReward;
			TaskReward.wTaskID = m_wTaskID;
			TaskReward.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskReward.szMachineID);
			lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

			//��������
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_REWARD,&TaskReward,sizeof(TaskReward));

			return true;
		}
	}
	return true;
}

//�ر��¼�
bool CDlgTaskPlazaCenter::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//��ȡ�¼�
bool CDlgTaskPlazaCenter::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//�������
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//������Ϣ
	if(Command.wSubCmdID==SUB_GP_TASK_INFO)
	{
		//��ȡ����
		CMD_GP_TaskInfo * pTaskInfo = (CMD_GP_TaskInfo *)pData;

		//����У��
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//�������
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;			
		}

		//����Ŀ¼
		m_pTaskList->UpdateNode(m_pNodeAllTask);
		
		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//������ֹ
		OnMissionConclude();

		//�����ؼ�
		RectifyControlUI();			

		//Ĭ��ѡ��
		SelectDefaultItem();

		return true;
	}

    //�����б�
	if(Command.wSubCmdID==SUB_GP_TASK_LIST)
	{
		//����У��
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//��������
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//��������
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//��������
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//�������
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//ƫ��ָ��
			pDataBuffer += wParameterSize;			
		}

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//������
	if(Command.wSubCmdID==SUB_GP_TASK_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GP_TaskResult));
		if(wDataSize>sizeof(CMD_GP_TaskResult)) return false;

		//��ȡ����
		CMD_GP_TaskResult * pTaskResult = (CMD_GP_TaskResult *)pData;

		//��ֹ����
		m_MissionManager.ConcludeMissionItem(this,false);

		//��Ϣ��ʾ
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(TEXT("��ʾ"),pTaskResult->szNotifyContent);
		}

		//ִ�гɹ�
		if(pTaskResult->bSuccessed==true)
		{
			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GP_TASK_TAKE)
			{
				//����״̬	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//�������
				AddMineTaskItem(TaskStatus);				

				//����Ŀ¼
				m_pTaskList->UpdateNode(m_pNodeAllTask);
			}

			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GP_TASK_REWARD)
			{
				//��ѯ����
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);

				//����״̬
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					UpdateTaskStatus(m_wTaskID,pTaskStatus->cbTaskStatus);
				}

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//���²Ƹ�
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}
		}

		//������ֹ
		OnMissionConclude();

		//�����ؼ�
		RectifyControlUI();

		return true;
	}

	return true;
}

//��������
VOID CDlgTaskPlazaCenter::PerformLoadTask()
{
	//���ñ���
	m_bLoadTask=true;
	m_bTakeTask=false;
	m_bTaskReward=false;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//��ȡ����
VOID CDlgTaskPlazaCenter::PerformTakeTask()
{
	//���ñ���
	m_bLoadTask=false;
	m_bTakeTask=true;
	m_bTaskReward=false;

	//���ñ���
	m_wTaskID=m_pTaskParameter->wTaskID;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

//��ȡ����
VOID CDlgTaskPlazaCenter::PerformRewardTask()
{
	//���ñ���
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=true;

	//���ñ���
	m_wTaskID=m_pTaskStatus->wTaskID;

	//��������
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//��ʼ����
	OnMissionStart();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgTaskServerCenter::CDlgTaskServerCenter()
{
	//���ñ���
	m_pITCPSocket=NULL;
}

//��������
CDlgTaskServerCenter::~CDlgTaskServerCenter()
{
}

//��������
VOID CDlgTaskServerCenter::PerformLoadTask()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_C_LoadTaskInfo LoadTaskInfo;
	LoadTaskInfo.dwUserID = pGlobalUserData->dwUserID;
	lstrcpyn(LoadTaskInfo.szPassword,pGlobalUserData->szPassword,CountArray(LoadTaskInfo.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_LOAD_INFO,&LoadTaskInfo,sizeof(LoadTaskInfo));

	//��ʼ����
	OnMissionStart();
}

//��ȡ����
VOID CDlgTaskServerCenter::PerformTakeTask()
{
	//���ñ���
	m_wTaskID=m_pTaskParameter->wTaskID;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GR_C_TakeTask TaskTake;
	TaskTake.wTaskID = m_wTaskID;
	TaskTake.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskTake.szMachineID);
	lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_TAKE,&TaskTake,sizeof(TaskTake));

	//��ʼ����
	OnMissionStart();

	return;
}

//��ȡ����
VOID CDlgTaskServerCenter::PerformRewardTask()
{
	//���ñ���
	m_wTaskID=m_pTaskStatus->wTaskID;

	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//����ṹ
	CMD_GP_TaskReward TaskReward;
	TaskReward.wTaskID = m_wTaskID;
	TaskReward.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskReward.szMachineID);
	lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

	//��������
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_REWARD,&TaskReward,sizeof(TaskReward));

	//��ʼ����
	OnMissionStart();
}

//��������
VOID CDlgTaskServerCenter::Initialization(ITCPSocket * pITCPSocket)
{
	//���ñ���
	m_pITCPSocket = pITCPSocket;
}

//������Ϣ
bool CDlgTaskServerCenter::OnTaskMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//������Ϣ
	if(wSubCmdID==SUB_GR_TASK_INFO)
	{
		//��ȡ����
		CMD_GR_S_TaskInfo * pTaskInfo = (CMD_GR_S_TaskInfo *)pData;

		//����У��
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//�������
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;
		}

		//����Ŀ¼
		m_pTaskList->UpdateNode(m_pNodeAllTask);

		//������ֹ
		OnMissionConclude();

		//�����ؼ�
		RectifyControlUI();

		//Ĭ��ѡ��
		SelectDefaultItem();

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//������Ϣ
	if(wSubCmdID==SUB_GR_TASK_LIST)
	{
		//����У��
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//��������
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//��������
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//��������
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//�������
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//ƫ��ָ��
			pDataBuffer += wParameterSize;		
		}

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//ִ�н��
	if(wSubCmdID==SUB_GR_TASK_RESULT)
	{
		//����У��
		ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskResult));
		if(wDataSize>sizeof(CMD_GR_S_TaskResult)) return false;

		//��ȡ����
		CMD_GR_S_TaskResult * pTaskResult = (CMD_GR_S_TaskResult *)pData;

		//ִ�гɹ�
		if(pTaskResult->bSuccessed==true)
		{
			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GR_TASK_TAKE)
			{
				//����״̬	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//�������
				AddMineTaskItem(TaskStatus);				

				//����Ŀ¼
				m_pTaskList->UpdateNode(m_pNodeAllTask);
			}

			//��ȡ����
			if(pTaskResult->wCommandID==SUB_GR_TASK_REWARD)
			{
				//��ѯ����
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);

				//����״̬
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					UpdateTaskStatus(m_wTaskID,pTaskStatus->cbTaskStatus);
				}

				//��ȡ����
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//���²Ƹ�
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//�����¼�
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}					
		}

		//������ֹ
		OnMissionConclude();

		//��Ϣ��ʾ
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(TEXT("��ʾ"),pTaskResult->szNotifyContent);
		}

		//�����ؼ�
		RectifyControlUI();

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////