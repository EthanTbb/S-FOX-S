#include "StdAfx.h"
#include "Resource.h"
#include "Dlgtaskcenter.h"
#include "PlatformFrame.h"
#include "PlatformEvent.h"
#include "PlazaViewUserInfo.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//常量定义
#define ROUND_CX							10
#define ROUND_CY							10

////////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTaskGuide::CDlgTaskGuide(void) : CFGuiDialog(IDD_DLG_TASK_GUIDE)
{
	//资源变量
	m_btLook=NULL;
	m_btReturn=NULL;

	//创建字体
	m_FontText.CreateFont(12,0,0,0,100,0,0,0,134,3,2,1,2,TEXT("宋体"));	
}

//析构函数
CDlgTaskGuide::~CDlgTaskGuide(void)
{
}

//初始控件
void CDlgTaskGuide::InitControlUI()
{
	//设置标题
	CRect rcCaption(0,0,0,75);
	m_PaintManager.SetCaptionRect(rcCaption);

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//查看按钮
	m_btLook = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btLook->SetStatusImage(TEXT("file='BT_TASK_LOOK' restype='PNG'"));
	m_btLook->SetPos(CRect(22,82,74,105));

	//返回按钮
	m_btReturn = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btReturn->SetStatusImage(TEXT("file='BT_TASK_RETURN' restype='PNG'"));
	m_btReturn->SetPos(CRect(90,82,142,105));
}

//消息提醒
void CDlgTaskGuide::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//退出按钮
		if(m_btReturn==pControlUI) return OnCancel(); 

		//查看任务
		if(m_btLook==pControlUI)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_TASKVIEW,0L);

			return OnCancel();
		}
	}
}

//开始绘画
void CDlgTaskGuide::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKGUIDE_BACK"));

	//绘制背景
	ImageBack.DrawImage(pDC,0,0);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(m_FontText);

	//变量定义
	TCHAR szGuideText1[32]=TEXT("    亲爱的玩家，您今天还没");
	TCHAR szGuideText2[32]=TEXT("领取任务哦，点\"查看\"赶紧领");
	TCHAR szGuideText3[32]=TEXT("取任务收获奖励吧！");
	CRect rcGuideText(10,23,172,43);
	pDC->DrawText(szGuideText1,rcGuideText,DT_LEFT|DT_SINGLELINE|DT_END_ELLIPSIS);

	rcGuideText.OffsetRect(0,18);
	pDC->DrawText(szGuideText2,rcGuideText,DT_LEFT|DT_WORDBREAK|DT_END_ELLIPSIS);

	rcGuideText.OffsetRect(0,18);
	pDC->DrawText(szGuideText3,rcGuideText,DT_LEFT|DT_WORDBREAK|DT_END_ELLIPSIS);

	return;
}

//创建函数
BOOL CDlgTaskGuide::OnInitDialog()
{
	__super::OnInitDialog();

	//修改风格
	ModifyStyleEx(0,WS_EX_LAYERED);

	//设置透明
	SetLayeredWindowAttributes(0,225,LWA_ALPHA);

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKGUIDE_BACK"));

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.left=4;
	rcUnLayered.top=16;	
	rcUnLayered.right=rcWindow.Width()-22;
	rcUnLayered.bottom=rcWindow.Height()-10;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,220,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//消息解释
BOOL CDlgTaskGuide::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE) return TRUE;
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//取消事件
VOID CDlgTaskGuide::OnCancel()
{
	__super::OnCancel();

	//销毁窗口
	DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////


//构造函数
CDlgTaskNotifyer::CDlgTaskNotifyer(void)/* : CFGuiDialog(IDD_DLG_SIMPLE)*/
{
	//信息变量
	m_szTaskName[0]=0;

	//资源变量
	m_btClose=NULL;

	//创建字体
	m_FontText.CreateFont(13,0,0,0,100,0,0,0,134,3,2,1,2,TEXT("宋体"));	
}

//析构函数
CDlgTaskNotifyer::~CDlgTaskNotifyer(void)
{
}

//初始控件
void CDlgTaskNotifyer::InitControlUI()
{
	//标题区域
	CRect rcCaption(0,0,0,50);
	m_PaintManager.SetCaptionRect(rcCaption);

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//关闭按钮
	m_btClose = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btClose->SetStatusImage(TEXT("file='BT_NOTIFYER_CLOSE' restype='PNG'"));
	m_btClose->SetFadeOut(false);
	m_btClose->SetPos(CRect(195,5,215,25));

	//奖励标签
	m_TextReward = CTextUI::Create(&m_PaintManager,pParent,TEXT(""));	
	m_TextReward->SetText(TEXT("{a}{f 2}快去领取奖励吧！{/f}{/a}"));
	m_TextReward->SetFixedXY(CPoint(102,67));
	m_TextReward->SetFixedWidth(120);
	m_TextReward->SetFixedHeight(20);
	m_TextReward->SetShowHtml();	
}

//消息提醒
void CDlgTaskNotifyer::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(m_btClose==pControlUI) return OnCancel(); 
	}
	else if(lstrcmp(msg.sType,TEXT("link")) ==0 )
	{
		if(pControlUI==m_TextReward)
		{
			//发送事件
			CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
			if (pPlatformEvent!=NULL) pPlatformEvent->PostPlatformEvent(EVENT_USER_SHOW_TASKVIEW,0L);

			return OnCancel();
		}
	}
}

//任务名称
VOID CDlgTaskNotifyer::SetTaskName(LPCTSTR pszTaskName)
{
	//拷贝字符
	lstrcpyn(m_szTaskName,pszTaskName,CountArray(m_szTaskName));

	return;
}

//开始绘画
void CDlgTaskNotifyer::OnBeginPaintWindow(HDC hDC)
{
	//获取设备
	CDC * pDC = CDC::FromHandle(hDC);

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKNOTIFYER_BACK"));

	//绘制背景
	ImageBack.DrawImage(pDC,0,0);

	//设置环境
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(m_FontText);

	//变量定义
	TCHAR szNotifyText[128]=TEXT("");
	CRect rcNotifyText(9,30,212,73);
	_sntprintf(szNotifyText,CountArray(szNotifyText),TEXT("    恭喜您已经完成了任务\"%s\"！"),m_szTaskName);
	pDC->DrawText(szNotifyText,rcNotifyText,DT_LEFT|DT_WORDBREAK|DT_END_ELLIPSIS);

	return;
}

//创建函数
BOOL CDlgTaskNotifyer::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyleEx(0,WS_EX_TOPMOST);

	//加载资源
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME),TEXT("DLG_TASKNOTIFYER_BACK"));

	//窗口位置
	CRect rcArce;
	SystemParametersInfo(SPI_GETWORKAREA,0,&rcArce,SPIF_SENDCHANGE);

	//设置大小
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());
	SetWindowPos(NULL,rcArce.right-SizeWindow.cx,rcArce.bottom-SizeWindow.cy,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOREDRAW);

	//获取窗口
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//计算位置
	CRect rcUnLayered;
	rcUnLayered.left=4;
	rcUnLayered.top=4;	
	rcUnLayered.right=rcWindow.Width()-4;
	rcUnLayered.bottom=rcWindow.Height()-4;

	//设置区域
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_CX,ROUND_CY);

	//设置区域
	SetWindowRgn(RgnWindow,FALSE);

	//分层窗口
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_CX,ROUND_CY),false);

	return TRUE;
}

//消息解释
BOOL CDlgTaskNotifyer::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if (pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam==VK_ESCAPE) return TRUE;
		if(pMsg->wParam==VK_RETURN) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//取消事件
VOID CDlgTaskNotifyer::OnCancel()
{
	__super::OnCancel();

	//销毁窗口
	DestroyWindow();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTaskCenter::CDlgTaskCenter(void) : CSkinDialog(IDD_DLG_POPUP)
{
	//标识变量
	m_bLableRound=false;

	//任务变量
	m_wTaskID=0;
	m_pTaskStatus=NULL;
	m_pTaskParameter=NULL;

	//控件变量
	m_pTaskList=NULL;
	m_btTaskTake=NULL;

	//节点变量
	m_pNodeSelfTask=NULL;
	m_pNodeAllTask=NULL;

	//设置变量
	m_rcItemSelected.SetRect(0,0,0,0);	

	//创建字体
	m_FontText.CreateFont(18,0,0,0,FW_BOLD,0,0,0,134,3,2,1,2,TEXT("新宋体"));	
}

//析构函数
CDlgTaskCenter::~CDlgTaskCenter(void)
{
	//变量定义
	WORD wKey=0;
	POSITION Position;

	//删除参数	
	tagTaskParameter * pTaskParameter=NULL;
	Position=m_TaskParameterMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskParameterMap.GetNextAssoc(Position,wKey,pTaskParameter);
		SafeDelete(pTaskParameter);
	}
	m_TaskParameterMap.RemoveAll();

	//删除状态
	tagTaskStatus * pTaskStatus=NULL;
	Position=m_TaskStatusMap.GetStartPosition();
	while (Position!=NULL)
	{
		m_TaskStatusMap.GetNextAssoc(Position,wKey,pTaskStatus);
		SafeDelete(pTaskStatus);
	}
	m_TaskStatusMap.RemoveAll();
	
}

//初始控件
void CDlgTaskCenter::InitControlUI()
{
	__super::InitControlUI();

	//获取对象
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//领取任务
	m_btTaskTake = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btTaskTake->SetStatusImage(TEXT("file='BT_TASK_TAKE' restype='PNG'"));
	m_btTaskTake->SetPos(CRect(277,285,407,331));
	m_btTaskTake->SetVisible(false);

	//领取奖励
	m_btTakeReward = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_btTakeReward->SetStatusImage(TEXT("file='BT_TASK_REWARD' restype='PNG'"));
	m_btTakeReward->SetPos(CRect(277,285,407,331));
	m_btTakeReward->SetVisible(false);

	//任务列表
	m_pTaskList = new CTaskUI(m_PaintManager); //(&m_PaintManager,pParent,TEXT("ListUI"));	
	m_pTaskList->SetManager(&m_PaintManager,pParent);
	m_pTaskList->SetAlternateBk(false);
	m_pTaskList->SetFixedWidth(161);
	m_pTaskList->SetFixedHeight(320);	
	m_pTaskList->SetFixedXY(CPoint(12,33));	
	m_pTaskList->EnableScrollBar();
	m_pTaskList->SetFloat();
	pParent->Add(m_pTaskList);

	//列表信息
	TListInfoUI * pListInfoUI = m_pTaskList->GetListInfo();
	pListInfoUI->rcTextPadding=CRect(4,0,0,0);
	pListInfoUI->dwHotBkColor=ARGB(255,128,110,84);
	pListInfoUI->dwSelectedBkColor=0;

	//变量定义
	tagTaskListItemInfo TaskItem;

	//我的任务
	TaskItem.bIsFolder=true;
	TaskItem.bIsEmpty=false;
	lstrcpyn(TaskItem.szTaskName,TEXT("我的任务"),CountArray(TaskItem.szTaskName));
	m_pNodeSelfTask = m_pTaskList->AddNode(TaskItem,NULL);

	//所有任务
	TaskItem.bIsFolder=true;
	TaskItem.bIsEmpty=false;
	lstrcpyn(TaskItem.szTaskName,TEXT("所有任务"),CountArray(TaskItem.szTaskName));
	m_pNodeAllTask = m_pTaskList->AddNode(TaskItem,NULL);
}

//消息提醒
void CDlgTaskCenter::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//获取对象
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI==m_btTaskTake && m_pTaskParameter!=NULL) 
		{
			//领取任务
			return PerformTakeTask();
		}
		else if(pControlUI==m_btTakeReward && m_pTaskParameter!=NULL) 
		{
			//领取奖励
			return PerformRewardTask();
		}
	}
    else if (lstrcmp(msg.sType, TEXT("itemselect")) == 0 ||
			 lstrcmp(msg.sType, TEXT("itemclick")) == 0)		
	{
		//控件判断
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
						//设置变量
						m_rcItemSelected = pListItem->GetPos();
						m_pTaskParameter=pTaskParameter;

						//查找对象
						m_pTaskStatus=NULL;
						m_TaskStatusMap.Lookup(m_pTaskParameter->wTaskID,m_pTaskStatus);

						//变量定义
						CRect rcTaskList(7,45,0,359);

						//调整区域
						if(m_rcItemSelected.top<rcTaskList.top) m_rcItemSelected.OffsetRect(0,rcTaskList.top-m_rcItemSelected.top+6);
						if(m_rcItemSelected.bottom>rcTaskList.bottom) m_rcItemSelected.OffsetRect(0,rcTaskList.bottom-m_rcItemSelected.bottom-6);

						//调整控件
						RectifyControlUI();

						//更新界面
						RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
					}
				}
			}
		}		
	}
}

//绘画标题
VOID CDlgTaskCenter::OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight)
{
	//资源句柄
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//加载标题
	CPngImage ImageLogo;
	ImageLogo.LoadImage(hInstance,TEXT("TASK_TITLE"));

	//绘制标题
	ImageLogo.DrawImage(pDC,10,10);
}

//绘画消息
VOID CDlgTaskCenter::OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight)
{
	//资源句柄
	HINSTANCE hInstance = GetModuleHandle(PLATFORM_RESOURCE_DLL_NAME);

	//区域定义
	CRect rcTaskList(7,45,0,359);
	CRect rcTaskAbout(185,43,0,358);

	//加载资源
	CPngImage ImageTaskListT,ImageTaskListM,ImageTaskListB;
	ImageTaskListT.LoadImage(hInstance,TEXT("TASK_LIST_T"));
	ImageTaskListM.LoadImage(hInstance,TEXT("TASK_LIST_M"));
	ImageTaskListB.LoadImage(hInstance,TEXT("TASK_LIST_B"));

	//加载资源
	CPngImage ImageTaskAboutT,ImageTaskAboutM,ImageTaskAboutB;
	ImageTaskAboutT.LoadImage(hInstance,TEXT("TASK_ABOUT_T"));
	ImageTaskAboutM.LoadImage(hInstance,TEXT("TASK_ABOUT_M"));
	ImageTaskAboutB.LoadImage(hInstance,TEXT("TASK_ABOUT_B"));

	//绘制列表
	ImageTaskListT.DrawImage(pDC,rcTaskList.left,rcTaskList.top);
	for(INT nYPos=ImageTaskListT.GetHeight();nYPos<rcTaskList.Height()-ImageTaskListB.GetHeight();nYPos += ImageTaskListM.GetHeight())
	{
		ImageTaskListM.DrawImage(pDC,rcTaskList.left,rcTaskList.top+nYPos);
	}
	ImageTaskListB.DrawImage(pDC,rcTaskList.left,rcTaskList.bottom-ImageTaskListB.GetHeight());

	//绘制简介
	ImageTaskAboutT.DrawImage(pDC,rcTaskAbout.left,rcTaskAbout.top);
	for(INT nYPos=ImageTaskAboutT.GetHeight();nYPos<rcTaskAbout.Height()-ImageTaskAboutB.GetHeight();nYPos += ImageTaskAboutM.GetHeight())
	{
		ImageTaskAboutM.DrawImage(pDC,rcTaskAbout.left,rcTaskAbout.top+nYPos);
	}
	ImageTaskAboutB.DrawImage(pDC,rcTaskAbout.left,rcTaskAbout.bottom-ImageTaskAboutB.GetHeight());

	//任务信息
	if(m_pTaskParameter!=NULL)
	{
		//加载资源
		CPngImage ImageArrow;
		ImageArrow.LoadImage(hInstance,TEXT("TASK_ARROW"));

		//绘制箭头
		CSize SizeArrow(ImageArrow.GetWidth(),ImageArrow.GetHeight());
		ImageArrow.DrawImage(pDC,rcTaskAbout.left-SizeArrow.cx+4,m_rcItemSelected.top+(m_rcItemSelected.Height()-SizeArrow.cy)/2);

		//设置环境
		pDC->SetTextColor(RGB(75,0,1));
		pDC->SelectObject(m_FontText);

		//任务名称
		CRect rcTaskName(rcTaskAbout.left+10,rcTaskAbout.top+10,rcTaskAbout.left+ImageTaskAboutT.GetWidth(),rcTaskAbout.top+35);
		pDC->DrawText(m_pTaskParameter->szTaskName,rcTaskName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);	

		//设置环境
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//任务进度
		CRect rcTaskProgress(rcTaskAbout.left+38,rcTaskAbout.top+61 ,rcTaskAbout.left+92,rcTaskAbout.top+77);			
		pDC->DrawText(TEXT("任务进度:"),rcTaskProgress,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);	

		//任务进度
		TCHAR szTaskProgress[32]=TEXT("");
		CRect rcTaskProgress1(rcTaskAbout.left+96,rcTaskAbout.top+61,rcTaskAbout.left+293,rcTaskAbout.top+106);	
		_sntprintf(szTaskProgress,CountArray(szTaskProgress),TEXT("%d/%d"),(m_pTaskStatus?m_pTaskStatus->wTaskProgress:0),m_pTaskParameter->wTaskObject);
		pDC->DrawText(szTaskProgress,rcTaskProgress1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);	

		//时间限制
		CRect rcTimeLimit(rcTaskAbout.left+38,rcTaskAbout.top+90,rcTaskAbout.left+92,rcTaskAbout.top+106);
		CRect rcTimeLimit1(rcTaskAbout.left+96,rcTaskAbout.top+90,rcTaskAbout.left+293,rcTaskAbout.top+106);
		pDC->DrawText(TEXT("时间限制:"),rcTimeLimit,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//格式化时间
		TCHAR szTaskTimeLimit[128]=TEXT("");
		WORD wHour = (WORD)(m_pTaskParameter->dwTimeLimit/3600);
		WORD wMinute = (WORD)((m_pTaskParameter->dwTimeLimit-wHour*3600)/60);
		WORD wSecond = (WORD)(m_pTaskParameter->dwTimeLimit-wHour*3600-wMinute*60);
		_sntprintf(szTaskTimeLimit,CountArray(szTaskTimeLimit),TEXT("%02d时:%02d分:%02d秒"),wHour,wMinute,wSecond);
		pDC->DrawText(szTaskTimeLimit,rcTimeLimit1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//任务说明
		CRect rcTaskDescribe(rcTaskAbout.left+38,rcTaskAbout.top+120,rcTaskAbout.left+92,rcTaskAbout.top+136);
		CRect rcTaskDescribe1(rcTaskAbout.left+96,rcTaskAbout.top+120,rcTaskAbout.left+310,rcTaskAbout.top+150);
		pDC->DrawText(TEXT("任务说明:"),rcTaskDescribe,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//计算区域
		pDC->DrawText(m_pTaskParameter->szTaskDescribe,rcTaskDescribe1,DT_CALCRECT|DT_WORDBREAK);
    	pDC->DrawText(m_pTaskParameter->szTaskDescribe,rcTaskDescribe1,DT_LEFT|DT_TOP|DT_WORDBREAK|DT_END_ELLIPSIS);

		//变量定义
		INT nRewardYPos=rcTaskAbout.top+152;
		if(nRewardYPos<rcTaskDescribe1.bottom+8) nRewardYPos=rcTaskDescribe1.bottom+8;

		//任务奖励
		CRect rcTaskReward(rcTaskAbout.left+38,nRewardYPos,rcTaskAbout.left+92,nRewardYPos+18);		
		pDC->DrawText(TEXT("任务奖励:"),rcTaskReward,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//任务奖励
		TCHAR szTaskReward[64]=TEXT("");
		CRect rcTaskReward1(rcTaskAbout.left+96,nRewardYPos,rcTaskAbout.left+300,nRewardYPos+18);		
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("普通玩家： 游戏币 +%I64d"),m_pTaskParameter->lStandardAwardGold);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		rcTaskReward1.OffsetRect(0,16);
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("普通玩家： 元宝 +%I64d "),m_pTaskParameter->lStandardAwardMedal);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		rcTaskReward1.OffsetRect(0,16);
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("会员玩家： 游戏币 +%I64d"),m_pTaskParameter->lMemberAwardGold);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		rcTaskReward1.OffsetRect(0,16);
		_sntprintf(szTaskReward,CountArray(szTaskReward),TEXT("会员玩家： 元宝 +%I64d"),m_pTaskParameter->lMemberAwardMedal);
		pDC->DrawText(szTaskReward,rcTaskReward1,DT_LEFT|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

	}
}

//开始任务
VOID CDlgTaskCenter::OnMissionStart()
{
	//使能设置
	if(m_pTaskList!=NULL) m_pTaskList->SetEnabled(false);
	if(m_btTaskTake!=NULL) m_btTaskTake->SetEnabled(false);
	if(m_btTakeReward!=NULL) m_btTakeReward->SetEnabled(false);
}

//终止任务
VOID CDlgTaskCenter::OnMissionConclude()
{
	//使能设置
	if(m_pTaskList!=NULL) m_pTaskList->SetEnabled(true);
	if(m_btTaskTake!=NULL) m_btTaskTake->SetEnabled(true);
	if(m_btTakeReward!=NULL) m_btTakeReward->SetEnabled(true);	
}

//调整控件
VOID CDlgTaskCenter::RectifyControlUI()
{
	//可视设置
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

	//使能设置
	if(m_pTaskParameter!=NULL) m_btTaskTake->SetEnabled(true);
	if(m_pTaskParameter==NULL || m_pTaskStatus!=NULL) m_btTaskTake->SetEnabled(false);

	//更新界面
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);
}

//默认选择
VOID CDlgTaskCenter::SelectDefaultItem()
{
	if(m_pTaskParameter == NULL) return;

	//查找节点
	Node * pSelectNode = m_pTaskList->SearchNode(m_pNodeSelfTask,m_pTaskParameter->wTaskID);			
	if(pSelectNode == NULL)
	{
		m_pTaskList->SearchNode(m_pNodeAllTask,m_pTaskParameter->wTaskID);			
	}

	//选择子项
	if(pSelectNode!=NULL)
	{
		m_pTaskList->SelectItem(pSelectNode->Data().plistElment->GetIndex());
	}
}

//添加任务
BOOL CDlgTaskCenter::AddMineTaskItem(tagTaskStatus & TaskStatus)
{
	//变量定义
	tagTaskStatus * pTaskStatus=NULL;

	//申请资源
	try
	{
		pTaskStatus = new tagTaskStatus;
		if(pTaskStatus==NULL) throw(TEXT("内存不足！"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//拷贝数据
	CopyMemory(pTaskStatus,&TaskStatus,sizeof(tagTaskStatus));
	m_TaskStatusMap[pTaskStatus->wTaskID]=pTaskStatus;

	//查找参数
	tagTaskParameter * pTaskParameter=NULL;
	if(m_TaskParameterMap.Lookup(pTaskStatus->wTaskID,pTaskParameter)==TRUE)
	{
		//隐藏已领取
		if(pTaskStatus->cbTaskStatus != TASK_STATUS_REWARD)
		{
			//变量定义
			tagTaskListItemInfo TaskItem;

			//插入任务
			TaskItem.bIsFolder=false;
			TaskItem.bIsEmpty=false;
			TaskItem.bShowStatus=true;
			TaskItem.wTaskID=pTaskParameter->wTaskID;
			TaskItem.cbTaskStatus=pTaskStatus->cbTaskStatus;
			lstrcpyn(TaskItem.szTaskName,pTaskParameter->szTaskName,CountArray(TaskItem.szTaskName));
			Node * pNodeTask = m_pTaskList->AddNode(TaskItem,m_pNodeSelfTask);
			if(pNodeTask) pNodeTask->Data().plistElment->SetUserData(pTaskParameter);

			//默认显示
			if(m_pTaskStatus==NULL)
			{
				m_pTaskStatus=pTaskStatus;
				m_pTaskParameter=pTaskParameter;
			}

			//默认完成
			if(pTaskStatus->cbTaskStatus==TASK_STATUS_SUCCESS)
			{
				m_pTaskStatus=pTaskStatus;
				m_pTaskParameter=pTaskParameter;
			}
		}

		//删除任务
		Node * pTaskNode=m_pTaskList->SearchNode(m_pNodeAllTask,pTaskParameter->wTaskID);
		if(pTaskNode!=NULL) m_pTaskList->RemoveNode(pTaskNode);
	}		

	return TRUE;
}

//添加任务
BOOL CDlgTaskCenter::AddAllTaskItem(VOID * pTaskParameterData,WORD wParameterSize)
{
	//参数校验
	ASSERT(pTaskParameterData!=NULL && wParameterSize>0);
	if(pTaskParameterData==NULL || wParameterSize==0) return FALSE;

	//变量定义
	tagTaskParameter * pTaskParameter=NULL;

	//申请资源
	try
	{
		pTaskParameter = new tagTaskParameter;
		if(pTaskParameter==NULL) throw(TEXT("内存不足！"));
	}
	catch(...)
	{
		ASSERT(FALSE);
		return FALSE;
	}

	//拷贝数据
	CopyMemory(pTaskParameter,pTaskParameterData,wParameterSize);
	m_TaskParameterMap[pTaskParameter->wTaskID]=pTaskParameter;

	//变量定义
	tagTaskListItemInfo TaskItem;

	//插入任务
	TaskItem.bIsEmpty=false;
	TaskItem.bIsFolder=false;
	TaskItem.bShowStatus=false;
	TaskItem.wTaskID=pTaskParameter->wTaskID;
	lstrcpyn(TaskItem.szTaskName,pTaskParameter->szTaskName,CountArray(TaskItem.szTaskName));
	Node * pNodeTask = m_pTaskList->AddNode(TaskItem,m_pNodeAllTask);
	if(pNodeTask) pNodeTask->Data().plistElment->SetUserData(pTaskParameter);

	//默认显示
	if(m_pTaskParameter == NULL) 
	{
		m_pTaskParameter=pTaskParameter;
	}

	return TRUE;
}

//更新状态
VOID CDlgTaskCenter::UpdateTaskStatus(WORD wTaskID,BYTE cbTaskStatus)
{
	//查找对象	
	Node * pTaskNode=m_pTaskList->SearchNode(m_pNodeSelfTask,wTaskID);
	if(pTaskNode)
	{
		if(cbTaskStatus != TASK_STATUS_REWARD)
		{
			//更新状态
			pTaskNode->Data().cbTaskStatus=cbTaskStatus;
			m_pTaskList->UpdateNode(pTaskNode);
		}
		else
		{
			//移除任务
			m_pTaskList->RemoveNode(pTaskNode);
		}
	}
}

//创建函数
BOOL CDlgTaskCenter::OnInitDialog()
{
	__super::OnInitDialog();

	//设置窗口
	SetWindowPos(NULL,0,0,501,369,SWP_NOZORDER|SWP_NOMOVE);

	//居中窗口
	CenterWindow(this);

	//加载任务
	PerformLoadTask();

	return FALSE;
}

//消息解释
BOOL CDlgTaskCenter::PreTranslateMessage(MSG * pMsg)
{
	//按键过虑
	if ((pMsg->message==WM_KEYDOWN)&&(pMsg->wParam==VK_ESCAPE))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTaskPlazaCenter::CDlgTaskPlazaCenter()
{
	//任务标识
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=false;

	//设置任务
	m_MissionManager.InsertMissionItem(this);
}

//析构函数
CDlgTaskPlazaCenter::~CDlgTaskPlazaCenter()
{
}

//连接事件
bool CDlgTaskPlazaCenter::OnEventMissionLink(INT nErrorCode)
{
	if (nErrorCode!=0L)
	{
		//显示提示
		CInformation Information;
		Information.ShowMessageBox(TEXT("无法连接到登录服务器，任务信息加载失败，请稍后再试！"),MB_ICONERROR);
	}
	else
	{
		//加载任务
		if(m_bLoadTask==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskLoadInfo TaskLoadInfo;
			TaskLoadInfo.dwUserID = pGlobalUserData->dwUserID;
			lstrcpyn(TaskLoadInfo.szPassword,pGlobalUserData->szPassword,CountArray(TaskLoadInfo.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_LOAD,&TaskLoadInfo,sizeof(TaskLoadInfo));

			return true;
		}

		//领取签到
		if(m_bTakeTask==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskTake TaskTake;
			TaskTake.wTaskID = m_wTaskID;
			TaskTake.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskTake.szMachineID);
			lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_TAKE,&TaskTake,sizeof(TaskTake));

			return true;
		}

		//领取奖励
		if(m_bTaskReward==true)
		{
			//获取对象
			CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

			//构造结构
			CMD_GP_TaskReward TaskReward;
			TaskReward.wTaskID = m_wTaskID;
			TaskReward.dwUserID = pGlobalUserData->dwUserID;			
			CWHService::GetMachineIDEx(TaskReward.szMachineID);
			lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

			//发送数据
			m_MissionManager.SendData(MDM_GP_USER_SERVICE,SUB_GP_TASK_REWARD,&TaskReward,sizeof(TaskReward));

			return true;
		}
	}
	return true;
}

//关闭事件
bool CDlgTaskPlazaCenter::OnEventMissionShut(BYTE cbShutReason)
{
	return true;
}

//读取事件
bool CDlgTaskPlazaCenter::OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize)
{
	//命令过滤
	if(Command.wMainCmdID!=MDM_GP_USER_SERVICE) return true;

	//任务信息
	if(Command.wSubCmdID==SUB_GP_TASK_INFO)
	{
		//提取数据
		CMD_GP_TaskInfo * pTaskInfo = (CMD_GP_TaskInfo *)pData;

		//参数校验
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//添加任务
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;			
		}

		//更新目录
		m_pTaskList->UpdateNode(m_pNodeAllTask);
		
		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//任务终止
		OnMissionConclude();

		//调整控件
		RectifyControlUI();			

		//默认选择
		SelectDefaultItem();

		return true;
	}

    //任务列表
	if(Command.wSubCmdID==SUB_GP_TASK_LIST)
	{
		//参数校验
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//变量定义
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//任务数量
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//解析数据
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//添加任务
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//偏移指针
			pDataBuffer += wParameterSize;			
		}

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//任务结果
	if(Command.wSubCmdID==SUB_GP_TASK_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GP_TaskResult));
		if(wDataSize>sizeof(CMD_GP_TaskResult)) return false;

		//提取数据
		CMD_GP_TaskResult * pTaskResult = (CMD_GP_TaskResult *)pData;

		//终止任务
		m_MissionManager.ConcludeMissionItem(this,false);

		//消息提示
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(TEXT("提示"),pTaskResult->szNotifyContent);
		}

		//执行成功
		if(pTaskResult->bSuccessed==true)
		{
			//领取任务
			if(pTaskResult->wCommandID==SUB_GP_TASK_TAKE)
			{
				//设置状态	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//添加任务
				AddMineTaskItem(TaskStatus);				

				//更新目录
				m_pTaskList->UpdateNode(m_pNodeAllTask);
			}

			//领取奖励
			if(pTaskResult->wCommandID==SUB_GP_TASK_REWARD)
			{
				//查询对象
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);

				//更新状态
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					UpdateTaskStatus(m_wTaskID,pTaskStatus->cbTaskStatus);
				}

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//更新财富
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}
		}

		//任务终止
		OnMissionConclude();

		//调整控件
		RectifyControlUI();

		return true;
	}

	return true;
}

//加载任务
VOID CDlgTaskPlazaCenter::PerformLoadTask()
{
	//设置变量
	m_bLoadTask=true;
	m_bTakeTask=false;
	m_bTaskReward=false;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//领取任务
VOID CDlgTaskPlazaCenter::PerformTakeTask()
{
	//设置变量
	m_bLoadTask=false;
	m_bTakeTask=true;
	m_bTaskReward=false;

	//设置变量
	m_wTaskID=m_pTaskParameter->wTaskID;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

//领取奖励
VOID CDlgTaskPlazaCenter::PerformRewardTask()
{
	//设置变量
	m_bLoadTask=false;
	m_bTakeTask=false;
	m_bTaskReward=true;

	//设置变量
	m_wTaskID=m_pTaskStatus->wTaskID;

	//发起连接
	if (m_MissionManager.AvtiveMissionItem(this,false)==false)  
	{
		return;
	}

	//开始任务
	OnMissionStart();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgTaskServerCenter::CDlgTaskServerCenter()
{
	//设置变量
	m_pITCPSocket=NULL;
}

//析构函数
CDlgTaskServerCenter::~CDlgTaskServerCenter()
{
}

//加载任务
VOID CDlgTaskServerCenter::PerformLoadTask()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_C_LoadTaskInfo LoadTaskInfo;
	LoadTaskInfo.dwUserID = pGlobalUserData->dwUserID;
	lstrcpyn(LoadTaskInfo.szPassword,pGlobalUserData->szPassword,CountArray(LoadTaskInfo.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_LOAD_INFO,&LoadTaskInfo,sizeof(LoadTaskInfo));

	//开始任务
	OnMissionStart();
}

//领取任务
VOID CDlgTaskServerCenter::PerformTakeTask()
{
	//设置变量
	m_wTaskID=m_pTaskParameter->wTaskID;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GR_C_TakeTask TaskTake;
	TaskTake.wTaskID = m_wTaskID;
	TaskTake.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskTake.szMachineID);
	lstrcpyn(TaskTake.szPassword,pGlobalUserData->szPassword,CountArray(TaskTake.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_TAKE,&TaskTake,sizeof(TaskTake));

	//开始任务
	OnMissionStart();

	return;
}

//领取奖励
VOID CDlgTaskServerCenter::PerformRewardTask()
{
	//设置变量
	m_wTaskID=m_pTaskStatus->wTaskID;

	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//构造结构
	CMD_GP_TaskReward TaskReward;
	TaskReward.wTaskID = m_wTaskID;
	TaskReward.dwUserID = pGlobalUserData->dwUserID;	
	CWHService::GetMachineIDEx(TaskReward.szMachineID);
	lstrcpyn(TaskReward.szPassword,pGlobalUserData->szPassword,CountArray(TaskReward.szPassword));

	//发送数据
	m_pITCPSocket->SendData(MDM_GR_TASK,SUB_GR_TASK_REWARD,&TaskReward,sizeof(TaskReward));

	//开始任务
	OnMissionStart();
}

//配置任务
VOID CDlgTaskServerCenter::Initialization(ITCPSocket * pITCPSocket)
{
	//设置变量
	m_pITCPSocket = pITCPSocket;
}

//任务消息
bool CDlgTaskServerCenter::OnTaskMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)
{
	//任务信息
	if(wSubCmdID==SUB_GR_TASK_INFO)
	{
		//提取数据
		CMD_GR_S_TaskInfo * pTaskInfo = (CMD_GR_S_TaskInfo *)pData;

		//参数校验
		ASSERT(wDataSize==sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount));
		if(wDataSize!=sizeof(pTaskInfo->TaskStatus[0])*pTaskInfo->wTaskCount+sizeof(pTaskInfo->wTaskCount)) return false;

		//添加任务
		for(WORD wTaskIndex=0;wTaskIndex<pTaskInfo->wTaskCount;wTaskIndex++)
		{
			if(AddMineTaskItem(pTaskInfo->TaskStatus[wTaskIndex])==FALSE) break;
		}

		//更新目录
		m_pTaskList->UpdateNode(m_pNodeAllTask);

		//任务终止
		OnMissionConclude();

		//调整控件
		RectifyControlUI();

		//默认选择
		SelectDefaultItem();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//任务信息
	if(wSubCmdID==SUB_GR_TASK_LIST)
	{
		//参数校验
		ASSERT(wDataSize>=sizeof(WORD));
		if(wDataSize<sizeof(WORD)) return false;

		//变量定义
		WORD wOffsetSize=0,wParameterSize=0;
		LPBYTE pDataBuffer = (LPBYTE)pData;

		//任务数量
		WORD wTaskCount = *pDataBuffer;
		pDataBuffer += sizeof(wTaskCount);

		//解析数据
		for(WORD i=0;i<wTaskCount;i++)
		{
			wParameterSize = *(WORD *)pDataBuffer;
			pDataBuffer += sizeof(wParameterSize);

			//添加任务
			AddAllTaskItem(pDataBuffer,wParameterSize);

			//偏移指针
			pDataBuffer += wParameterSize;		
		}

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	//执行结果
	if(wSubCmdID==SUB_GR_TASK_RESULT)
	{
		//参数校验
		ASSERT(wDataSize<=sizeof(CMD_GR_S_TaskResult));
		if(wDataSize>sizeof(CMD_GR_S_TaskResult)) return false;

		//提取数据
		CMD_GR_S_TaskResult * pTaskResult = (CMD_GR_S_TaskResult *)pData;

		//执行成功
		if(pTaskResult->bSuccessed==true)
		{
			//领取任务
			if(pTaskResult->wCommandID==SUB_GR_TASK_TAKE)
			{
				//设置状态	
				tagTaskStatus TaskStatus;
				TaskStatus.wTaskProgress=0;
				TaskStatus.wTaskID=m_wTaskID;				
				TaskStatus.cbTaskStatus=TASK_STATUS_UNFINISH;								

				//添加任务
				AddMineTaskItem(TaskStatus);				

				//更新目录
				m_pTaskList->UpdateNode(m_pNodeAllTask);
			}

			//领取奖励
			if(pTaskResult->wCommandID==SUB_GR_TASK_REWARD)
			{
				//查询对象
				tagTaskStatus * pTaskStatus=NULL;
				m_TaskStatusMap.Lookup(m_wTaskID,pTaskStatus);

				//更新状态
				if(pTaskStatus!=NULL)
				{
					pTaskStatus->cbTaskStatus=TASK_STATUS_REWARD;
					UpdateTaskStatus(m_wTaskID,pTaskStatus->cbTaskStatus);
				}

				//获取对象
				CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
				tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

				//更新财富
				pGlobalUserData->lUserScore=pTaskResult->lCurrScore;
				pGlobalUserData->lUserIngot=pTaskResult->lCurrIngot;

				//发送事件
				CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
				if (pPlatformEvent!=NULL) pPlatformEvent->SendPlatformEvent(EVENT_USER_INFO_UPDATE,0L);
			}					
		}

		//任务终止
		OnMissionConclude();

		//消息提示
		if(pTaskResult->szNotifyContent[0]!=0)
		{
			CInformation Information;
			Information.ShowMessageBox(TEXT("提示"),pTaskResult->szNotifyContent);
		}

		//调整控件
		RectifyControlUI();

		//更新界面
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_UPDATENOW);

		return true;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////