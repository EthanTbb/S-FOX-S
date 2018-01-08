#include "StdAfx.h"
#include "Resource.h"
#include "DlgIssueMessage.h"
#include "DlgInformation.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgIssueMessage, CSkinDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//构造函数
CDlgIssueMessage::CDlgIssueMessage() : CSkinDialog(IDD_DLG_ISSUE_MESSAGE)
{
	m_bSendALLRoom = FALSE;
	m_bSendGame = TRUE;
	m_bSendRoom = TRUE;
	m_bLoop = FALSE;
	m_dwTimeRate=0;
	tConcludeTime=0;
}

//析构函数
CDlgIssueMessage::~CDlgIssueMessage()
{
}

//控件绑定
VOID CDlgIssueMessage::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control( pDX,IDOK,m_btOK );
	DDX_Control( pDX,IDCANCEL,m_btCancel );
	DDX_Check( pDX,IDC_CHECK_ALLROOM,m_bSendALLRoom);
	DDX_Check( pDX,IDC_CHEK_ROOM,m_bSendRoom );
	DDX_Check( pDX,IDC_CHEK_GAME,m_bSendGame );
	DDX_Check( pDX,IDC_CHEK_LOOP,m_bLoop );
	DDX_Text(pDX,IDC_EDT_MESSAGE,m_strMessage);
	DDX_Text(pDX,IDC_ED_TIMERATE,m_dwTimeRate);
}

//初始化函数
BOOL CDlgIssueMessage::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("消息发布"));

	UpdateData(FALSE);

	return TRUE;
}

//确定函数
VOID CDlgIssueMessage::OnOK()
{
	UpdateData();
	
	//效验消息
	if( m_strMessage.IsEmpty() )
	{
		//提示消息
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("抱歉,提示消息不能为空,请重新输入!"),MB_ICONWARNING|MB_OK);
		return;
	}

	if( !m_bSendGame && !m_bSendRoom )
	{
		//提示消息
		CDlgInformation Information(this);
		Information.ShowMessageBox(TEXT("房间消息和游戏消息，请至少选择其中一个！"),MB_ICONWARNING|MB_OK);
		return;
	}

	if(m_bLoop)
	{
		CDateTimeCtrl* pCtrlDate = (CDateTimeCtrl*) GetDlgItem(IDC_CONCLUDEDATE);
		CDateTimeCtrl* pCtrlTime = (CDateTimeCtrl*) GetDlgItem(IDC_CONCLUDETIME);
		ASSERT(pCtrlDate!=NULL && pCtrlTime != NULL);

		CTime timeTime;
		DWORD dwResult = pCtrlDate->GetTime(timeTime);
		if (dwResult == GDT_VALID)
		{
			tConcludeTime=timeTime.GetTime()-timeTime.GetHour()*3600-timeTime.GetMinute()*60-timeTime.GetSecond();
		}
		else
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("请设置消息循环结束日期！"),MB_ICONWARNING|MB_OK);
			return;
		}

		dwResult = pCtrlTime->GetTime(timeTime);
		if (dwResult == GDT_VALID)
		{
			tConcludeTime +=timeTime.GetHour()*3600+timeTime.GetMinute()*60+timeTime.GetSecond();
		}
		else
		{
			//提示消息
			CDlgInformation Information(this);
			Information.ShowMessageBox(TEXT("请设置消息循环结束时间！"),MB_ICONWARNING|MB_OK);
			return;
		}
	}

	return __super::OnOK();
}

//////////////////////////////////////////////////////////////////////////
