#include "stdafx.h"
#include "resource.h"
#include "DlgInsureGame.h"

//游戏币存取
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");

//数字控件
const TCHAR* const szNumberUserScoreControlName = TEXT("NumberUserScore");
const TCHAR* const szNumberAcessUserInsrueControlName = TEXT("NumberAcessUserInsrue");
const TCHAR* const szNumberGameScoreControlName = TEXT("NumberGameScore");
const TCHAR* const szNumberInsureGameScoreControlName = TEXT("NumberInsureGameScore");
const TCHAR* const szNumberGameBeanControlName = TEXT("NumberGameBean");
const TCHAR* const szNumberGameIngotControlName = TEXT("NumberGameIngot");
const TCHAR* const szNumberGlamourValueControlName = TEXT("NumberGlamourValue");

//按钮控件
const TCHAR* const szButtonW1ControlName = TEXT("ButtonW1");
const TCHAR* const szButtonW5ControlName = TEXT("ButtonW5");
const TCHAR* const szButtonW10ControlName = TEXT("ButtonW10");
const TCHAR* const szButtonW50ControlName = TEXT("ButtonW50");
const TCHAR* const szButtonW100ControlName = TEXT("ButtonW100");
const TCHAR* const szButtonW1000ControlName = TEXT("ButtonW1000");
const TCHAR* const szButtonW10000ControlName = TEXT("ButtonW10000");
const TCHAR* const szButtonAllInControlName = TEXT("ButtonAllIn");
const TCHAR* const szButtonAllOutControlName = TEXT("ButtonAllOut");
const TCHAR* const szButtonInControlName = TEXT("ButtonIn");
const TCHAR* const szButtonOutControlName = TEXT("ButtonOut");

//编辑控件
const TCHAR* const szEditInOutControlName = TEXT("EditInOut");
const TCHAR* const szEditPassWordControlName = TEXT("EditPassWord");
const TCHAR* const szLabelInOutControlName = TEXT("LabelInOut");

//////////////////////////////////////////////////////////////////////////

CNumberConvert::CNumberConvert()
{

}
CNumberConvert::~CNumberConvert()
{

}

VOID CNumberConvert::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//插入逗号
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}


SCORE CNumberConvert::GetUserControlScore(LPCTSTR pszScore)
{
	tString tStrScore(TEXT(" "));
	//去掉所有的，号
	tString::size_type pos = -1;
	while( (pos=tStrScore.find(TEXT(","))) != -1 ){
		tStrScore.erase(pos, 1);
	}

	//变量定义
	WORD wIndex=0;
	SCORE lScore=0L;

	//获取数值
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszScore,CountArray(szBuffer));

	//构造数值
	while (szBuffer[wIndex]!=0)
	{
		//变量定义
		WORD wCurrentIndex=wIndex++;

		//构造数值
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//转换字符
VOID CNumberConvert::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//变量定义
	LPCTSTR pszNumber[]={TEXT("零"),TEXT("壹"),TEXT("贰"),TEXT("叁"),TEXT("肆"),TEXT("伍"),TEXT("陆"),TEXT("柒"),TEXT("捌"),TEXT("玖")};
	LPCTSTR pszWeiName[]={TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("亿"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万")};
	LPCTSTR pszWeiName1[]={TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万"),TEXT("亿")};

	//转换数值
	TCHAR szSwitchScore[32]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//字符长度
	UINT uItemLength=lstrlen(pszNumber[0]);
	int uSwitchLength=lstrlen(szSwitchScore);

	//转换操作
	for (int i=0;i<uSwitchLength;i++)
	{
		//变量定义
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//补零操作
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//拷贝数字
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//补零判断
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//补位判断
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//填补位名
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//拷贝位名
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//设置变量
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//转换字符
VOID CNumberConvert::NumberConvert(TCHAR* szNum, TCHAR szResult[], int nLen)
{
#ifndef _UNICODE
#define SZ_LEN	10
#else
#define SZ_LEN	5
#endif
	const TCHAR szBigUnits[][SZ_LEN] = { _T("萬"), _T("億"), _T("兆"), _T("京"), _T("垓"), _T("秭"), _T("穰"), 
		_T("溝"), _T("澗"), _T("正"), _T("載"),_T("極"), _T("恒河沙"), _T("阿僧祇"), 
		_T("那由他"), _T("不可思議"), _T("無量大數") };

	const TCHAR szSmallUnits[][SZ_LEN] = { _T("十"), _T("百"), _T("千") };
	const TCHAR _szNum[][SZ_LEN] = { TEXT("零"),TEXT("壹"),TEXT("贰"),TEXT("叁"),TEXT("肆"),TEXT("伍"),TEXT("陆"),TEXT("柒"),TEXT("捌"),TEXT("玖") };

	int nIndex = 0;
	int nPartLen = 0;
	UINT uEmptyBigUnitCount = 0;
	TCHAR szPart[32] = { 0 };

	while (nLen > nIndex)
	{
		if ((nLen - nIndex) % 4 == 0)
		{
			nPartLen = 4;
		}
		else
		{
			nPartLen = (nLen - nIndex) % 4;
		}
		lstrcpyn(szPart, szNum + nIndex, nPartLen+1);

		//全零跳过
		if (_ttoi(szPart) == 0)
		{
			if (uEmptyBigUnitCount == 0)
				_tcsnccat(szResult, _szNum[0], SZ_LEN);
			nIndex += nPartLen;
			uEmptyBigUnitCount++;
			continue;
		}

		bool bFillZero = false;
		//计算十单位
		for (int i = 0; i < nPartLen; ++i)
		{
			UINT uNum = szPart[i] - '0';
			if (nIndex > 0 && bFillZero == false && uNum == 0)
			{
				bFillZero = true;
			}
			else if (uNum != 0)
			{
				//首位遇零补零
				if (bFillZero == true && uNum != 0 && uEmptyBigUnitCount == 0)
					_tcsnccat(szResult, _szNum[0], SZ_LEN);
				_tcsnccat(szResult, _szNum[uNum], SZ_LEN);
				if (nPartLen - i - 2 >= 0)
					_tcsnccat(szResult, szSmallUnits[nPartLen - i - 2], SZ_LEN);
				bFillZero = false;
			}
		}

		//补万单位
		if ((nLen - nIndex) % 4 == 0)
		{
			if ((nLen - nIndex) / 4 - 2 >= 0)
				_tcsnccat(szResult, szBigUnits[(nLen - nIndex) / 4 - 2], SZ_LEN);
		}
		else
		{
			if ((nLen - nIndex) / 4 - 1 >= 0)
				_tcsnccat(szResult, szBigUnits[(nLen - nIndex) / 4 - 1], SZ_LEN);
		}

		uEmptyBigUnitCount = 0;
		nIndex += nPartLen;
	}

	// 清零
	tString tStrScore(szResult);
	//去掉所有的，号
	tString::size_type pos = -1;
	if((pos=tStrScore.rfind(_szNum[0])) != -1 )
	{
		if(pos == tStrScore.length()-1)
		{
			tStrScore.erase(pos);
		}
	}

	int nScoreLength = (int)(tStrScore.length()+1);

	lstrcpyn(szResult,tStrScore.c_str(),nScoreLength);

	return;
}

//////////////////////////////////////////////////////////////////////////
//构造函数
CDlgInsureGame::CDlgInsureGame() : CFGuiDialog(IDD_GAME_RULE)
{
	//标识变量
	m_bSwitchIng=false;
	m_blCanStore=false;
	m_blCanGetOnlyFree=false;

	//银行信息
	m_wRevenueTake=0;
	m_wRevenueTransfer=0;
	m_lUserScore=0;
	m_lUserInsure=0;

	//接口变量
	m_pIMeUserData=NULL;
	m_pIClientKernel=NULL;	

	//界面控件
	m_pTextForget=NULL;
	m_pEditPassword=NULL;	

	//界面控件
	m_pButtonSave=NULL;
	m_pButtonTake=NULL;
	m_pButtonRefresh=NULL;

}

//析构函数
CDlgInsureGame::~CDlgInsureGame()
{
}

//回车
VOID CDlgInsureGame::OnOK()
{
	DestroyWindow();
}

//取消
VOID CDlgInsureGame::OnCancel()
{
	DestroyWindow();
}

//初始化函数
BOOL CDlgInsureGame::OnInitDialog()
{
	__super::OnInitDialog();

	//设置标题
	SetWindowText(TEXT("我的银行"));

	//居中窗口
	CenterWindow(NULL);

	return TRUE;
}


//初始控件
void CDlgInsureGame::InitControlUI()
{
	__super::InitControlUI();

	m_pButtonSave = static_cast<CButtonUI*>(GetControlByName(szButtonInControlName));
	m_pButtonTake = static_cast<CButtonUI*>(GetControlByName(szButtonOutControlName));
	m_pEditInOut = static_cast<CEditUI*>(GetControlByName(szEditInOutControlName));
	m_pEditPassword = static_cast<CEditUI*>(GetControlByName(szEditPassWordControlName));
	m_LabelInOut = static_cast<CLabelUI*>(GetControlByName(szLabelInOutControlName));

}

//消息提醒
void CDlgInsureGame::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);
	
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if((pControlUI->GetName() == szButtonCloseControlName))
		{
			return OnCancel();
		}
		if(m_pButtonSave==pControlUI)				//存款事件
		{
			return OnBnClickedSaveScore();
		}
		else if(m_pButtonTake==pControlUI)			//取款事件
		{
			return OnBnClickedTakeScore();
		}
		else
		{
			OnBnClickAccess(msg);
		}
	}
	else if( msg.sType == TEXT("textchanged") )
	{
		if( pControlUI->GetName() == szEditInOutControlName )
		{
			ShowCapital();
		}
	}

}

LPCTSTR CDlgInsureGame::GetSkinFile()
{
    return TEXT("GameDlgInsure.xml"); 
}


VOID CDlgInsureGame::LoadDataFromUI()
{
	CNumberConvert nc;
	m_InOutScore = nc.GetUserControlScore(m_pEditInOut->GetText());
}


VOID CDlgInsureGame::ShowCapital()
{
	LoadDataFromUI();
	TCHAR szBuffer[256]={0};
	TCHAR szSource[256]={0};
	lstrcpyn(szSource,m_pEditInOut->GetText(),256);
	CString str(szSource);
	str.Replace(TEXT(","),TEXT(""));
	CNumberConvert nc;
	nc.NumberConvert(str.GetBuffer(),szBuffer,str.GetLength());
	if(m_LabelInOut != NULL) m_LabelInOut->SetText(szBuffer);

}


VOID CDlgInsureGame::UpdateUI()
{

	//携带游戏币
	CNumberUI* pNumberUserScore = static_cast<CNumberUI*>(GetControlByName(szNumberUserScoreControlName));
	float fUserScore = (float)m_lUserScore;
	if(pNumberUserScore != NULL) pNumberUserScore->SetNumber(fUserScore);

	//游戏存款
	CNumberUI* pUserInsrueScore = static_cast<CNumberUI*>(GetControlByName(szNumberAcessUserInsrueControlName));
	float fUserInsure = (float)m_lUserInsure;
	if(pUserInsrueScore != NULL) pUserInsrueScore->SetNumber(fUserInsure);
}

VOID CDlgInsureGame::SetAccessEditNum( SCORE  dwNum)
{
	TCHAR szBuffer[64]=TEXT("");
	wsprintf(szBuffer, TEXT("%ld"), dwNum);
	CEditUI* pEditUI = (CEditUI*)(GetControlByName(szEditInOutControlName));
	if(pEditUI != NULL) pEditUI->SetText(szBuffer);
}

VOID CDlgInsureGame::OnBnClickAccess(TNotifyUI& msg)
{

	const TCHAR* const szAccessButtonName[7]={szButtonW1ControlName, szButtonW5ControlName, szButtonW10ControlName, 
		szButtonW50ControlName, szButtonW100ControlName, szButtonW1000ControlName, 
		szButtonW10000ControlName};
	const SCORE dwAccessScore[7]={10000, 5*10000, 10*10000, 50*10000, 100*10000, 1000*10000, 10000*10000};

	tagGlobalUserData* pGlobalUserData = CGlobalUserInfo::GetInstance()->GetGlobalUserData();
	CNumberConvert nc;
	m_InOutScore = nc.GetUserControlScore(m_pEditInOut->GetText());
	SCORE nAccessScore = m_InOutScore;
	for(int i = 0; i < CountArray(szAccessButtonName); i++)
	{
		if( msg.pSender->GetName() == szAccessButtonName[i] ){
			nAccessScore += dwAccessScore[i];
			SetAccessEditNum(nAccessScore);
			ShowCapital();
			return;
		}
	}

	if( msg.pSender->GetName() == szButtonAllInControlName){
		SetAccessEditNum( m_lUserScore);
	}
	else if( msg.pSender->GetName() == szButtonAllOutControlName){
		SetAccessEditNum(  m_lUserInsure);
	}

	ShowCapital();
}


//转换字符
VOID CDlgInsureGame::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//插入逗号
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}

//存款按钮
VOID CDlgInsureGame::OnBnClickedSaveScore()
{
	//获取UI数据
	LoadDataFromUI();
	SCORE lInputScore = m_InOutScore;

	//参数验证
	if (lInputScore <= 0 || lInputScore > m_lUserScore )
	{
		//变量定义
		TCHAR szMessage[128]=TEXT("");

		//构造消息
		if(m_lUserScore>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("你输入的游戏币金额必须在1和%I64d之间"), m_lUserScore);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("你身上携带的游戏币不足,不能进行存款操作!"));
		}

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,10);

		////设置控件
		//m_pEditAmount->SetFocus();
		//m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//状态判断
	if (US_PLAYING==m_pIMeUserData->GetUserStatus())
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("请结束游戏后再存款！"),MB_ICONERROR,30);
		return;	
	}

	//构造数据
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	SaveScoreRequest.cbActivityGame=TRUE;
	SaveScoreRequest.lSaveScore=lInputScore;

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//取款按钮
VOID CDlgInsureGame::OnBnClickedTakeScore()
{
	//获取UI数据
	LoadDataFromUI();
	SCORE lInputScore = m_InOutScore;

	//参数验证
	if (lInputScore <= 0 || lInputScore > m_lUserInsure )
	{
		//变量定义
		TCHAR szMessage[128]=TEXT("");

		//构造消息
		if(m_lUserInsure>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("您输入的游戏币金额必须在1和%I64d之间"), m_lUserInsure);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("您银行的游戏币余额不足,不能进行提款操作!"));
		}

		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,30);

		////设置控件
		//m_pEditAmount->SetFocus();
		//m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//密码验证
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	lstrcpyn(szPassword,m_pEditPassword->GetText(),CountArray(szPassword));
	if (szPassword[0]==0)
	{
		//提示消息
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("银行密码不能为空，请重新输入！"),MB_ICONERROR,30);

		//设置控件
		m_pEditPassword->SetText(TEXT(""));
		m_pEditPassword->SetFocus();

		return;
	}

	//密码加密
	TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);

	//变量定义
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//构造结构
	TakeScoreRequest.cbActivityGame=TRUE;
	TakeScoreRequest.lTakeScore=lInputScore;
	CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	//清空密码
	m_pEditPassword->SetText(TEXT(""));

	return;
}

//刷新按钮
VOID CDlgInsureGame::OnBnClickedRefreshScore()
{
	//变量定义
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	QueryInsureInfo.cbActivityGame=TRUE;

	//发送数据
	SendSocketData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
}

//设置信息
void CDlgInsureGame::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserData)
{
	//参数效验
	ASSERT(pClientKernel!=NULL);
	if(pClientKernel==NULL) return;

	//设置信息
	m_pIClientKernel = pClientKernel;
	m_pIMeUserData=const_cast<IClientUserItem *>(pMeUserData);

	//刷新银行
	OnBnClickedRefreshScore();

	return;
}

//设置位置
void CDlgInsureGame::SetPostPoint(CPoint Point)
{
	//CRect Rect;
	//GetWindowRect(&Rect);
	//SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,/*SWP_NOMOVE|*/SWP_NOZORDER);
}

//发送信息
void CDlgInsureGame::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//参数效验
	ASSERT(m_pIClientKernel!=NULL);

	//发送信息
	if(m_pIClientKernel) m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//显示银行
void CDlgInsureGame::ShowItem()
{
	//变量定义
	bool bShow = true;
	if(m_blCanGetOnlyFree && m_pIMeUserData->GetUserStatus() == US_PLAYING) bShow = false;

	//设置控件
	m_pButtonSave->SetEnabled(m_blCanStore);
	m_pButtonTake->SetEnabled(bShow);
}


///////////////////////////////////////////////////////////////////////////////////////////////////