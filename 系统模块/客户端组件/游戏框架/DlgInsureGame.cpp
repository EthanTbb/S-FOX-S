#include "stdafx.h"
#include "resource.h"
#include "DlgInsureGame.h"

//��Ϸ�Ҵ�ȡ
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");

//���ֿؼ�
const TCHAR* const szNumberUserScoreControlName = TEXT("NumberUserScore");
const TCHAR* const szNumberAcessUserInsrueControlName = TEXT("NumberAcessUserInsrue");
const TCHAR* const szNumberGameScoreControlName = TEXT("NumberGameScore");
const TCHAR* const szNumberInsureGameScoreControlName = TEXT("NumberInsureGameScore");
const TCHAR* const szNumberGameBeanControlName = TEXT("NumberGameBean");
const TCHAR* const szNumberGameIngotControlName = TEXT("NumberGameIngot");
const TCHAR* const szNumberGlamourValueControlName = TEXT("NumberGlamourValue");

//��ť�ؼ�
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

//�༭�ؼ�
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
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//���붺��
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}


SCORE CNumberConvert::GetUserControlScore(LPCTSTR pszScore)
{
	tString tStrScore(TEXT(" "));
	//ȥ�����еģ���
	tString::size_type pos = -1;
	while( (pos=tStrScore.find(TEXT(","))) != -1 ){
		tStrScore.erase(pos, 1);
	}

	//��������
	WORD wIndex=0;
	SCORE lScore=0L;

	//��ȡ��ֵ
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszScore,CountArray(szBuffer));

	//������ֵ
	while (szBuffer[wIndex]!=0)
	{
		//��������
		WORD wCurrentIndex=wIndex++;

		//������ֵ
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//ת���ַ�
VOID CNumberConvert::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//��������
	LPCTSTR pszNumber[]={TEXT("��"),TEXT("Ҽ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("½"),TEXT("��"),TEXT("��"),TEXT("��")};
	LPCTSTR pszWeiName[]={TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��")};
	LPCTSTR pszWeiName1[]={TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("��")};

	//ת����ֵ
	TCHAR szSwitchScore[32]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//�ַ�����
	UINT uItemLength=lstrlen(pszNumber[0]);
	int uSwitchLength=lstrlen(szSwitchScore);

	//ת������
	for (int i=0;i<uSwitchLength;i++)
	{
		//��������
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//�������
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//��������
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//�����ж�
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//��λ�ж�
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//�λ��
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//����λ��
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//���ñ���
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//ת���ַ�
VOID CNumberConvert::NumberConvert(TCHAR* szNum, TCHAR szResult[], int nLen)
{
#ifndef _UNICODE
#define SZ_LEN	10
#else
#define SZ_LEN	5
#endif
	const TCHAR szBigUnits[][SZ_LEN] = { _T("�f"), _T("�|"), _T("��"), _T("��"), _T("��"), _T("��"), _T("�"), 
		_T("��"), _T("��"), _T("��"), _T("�d"),_T("�O"), _T("���ɳ"), _T("��ɮ�o"), 
		_T("������"), _T("����˼�h"), _T("�o����") };

	const TCHAR szSmallUnits[][SZ_LEN] = { _T("ʮ"), _T("��"), _T("ǧ") };
	const TCHAR _szNum[][SZ_LEN] = { TEXT("��"),TEXT("Ҽ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("½"),TEXT("��"),TEXT("��"),TEXT("��") };

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

		//ȫ������
		if (_ttoi(szPart) == 0)
		{
			if (uEmptyBigUnitCount == 0)
				_tcsnccat(szResult, _szNum[0], SZ_LEN);
			nIndex += nPartLen;
			uEmptyBigUnitCount++;
			continue;
		}

		bool bFillZero = false;
		//����ʮ��λ
		for (int i = 0; i < nPartLen; ++i)
		{
			UINT uNum = szPart[i] - '0';
			if (nIndex > 0 && bFillZero == false && uNum == 0)
			{
				bFillZero = true;
			}
			else if (uNum != 0)
			{
				//��λ���㲹��
				if (bFillZero == true && uNum != 0 && uEmptyBigUnitCount == 0)
					_tcsnccat(szResult, _szNum[0], SZ_LEN);
				_tcsnccat(szResult, _szNum[uNum], SZ_LEN);
				if (nPartLen - i - 2 >= 0)
					_tcsnccat(szResult, szSmallUnits[nPartLen - i - 2], SZ_LEN);
				bFillZero = false;
			}
		}

		//����λ
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

	// ����
	tString tStrScore(szResult);
	//ȥ�����еģ���
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
//���캯��
CDlgInsureGame::CDlgInsureGame() : CFGuiDialog(IDD_GAME_RULE)
{
	//��ʶ����
	m_bSwitchIng=false;
	m_blCanStore=false;
	m_blCanGetOnlyFree=false;

	//������Ϣ
	m_wRevenueTake=0;
	m_wRevenueTransfer=0;
	m_lUserScore=0;
	m_lUserInsure=0;

	//�ӿڱ���
	m_pIMeUserData=NULL;
	m_pIClientKernel=NULL;	

	//����ؼ�
	m_pTextForget=NULL;
	m_pEditPassword=NULL;	

	//����ؼ�
	m_pButtonSave=NULL;
	m_pButtonTake=NULL;
	m_pButtonRefresh=NULL;

}

//��������
CDlgInsureGame::~CDlgInsureGame()
{
}

//�س�
VOID CDlgInsureGame::OnOK()
{
	DestroyWindow();
}

//ȡ��
VOID CDlgInsureGame::OnCancel()
{
	DestroyWindow();
}

//��ʼ������
BOOL CDlgInsureGame::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�ҵ�����"));

	//���д���
	CenterWindow(NULL);

	return TRUE;
}


//��ʼ�ؼ�
void CDlgInsureGame::InitControlUI()
{
	__super::InitControlUI();

	m_pButtonSave = static_cast<CButtonUI*>(GetControlByName(szButtonInControlName));
	m_pButtonTake = static_cast<CButtonUI*>(GetControlByName(szButtonOutControlName));
	m_pEditInOut = static_cast<CEditUI*>(GetControlByName(szEditInOutControlName));
	m_pEditPassword = static_cast<CEditUI*>(GetControlByName(szEditPassWordControlName));
	m_LabelInOut = static_cast<CLabelUI*>(GetControlByName(szLabelInOutControlName));

}

//��Ϣ����
void CDlgInsureGame::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);
	
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if((pControlUI->GetName() == szButtonCloseControlName))
		{
			return OnCancel();
		}
		if(m_pButtonSave==pControlUI)				//����¼�
		{
			return OnBnClickedSaveScore();
		}
		else if(m_pButtonTake==pControlUI)			//ȡ���¼�
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

	//Я����Ϸ��
	CNumberUI* pNumberUserScore = static_cast<CNumberUI*>(GetControlByName(szNumberUserScoreControlName));
	float fUserScore = (float)m_lUserScore;
	if(pNumberUserScore != NULL) pNumberUserScore->SetNumber(fUserScore);

	//��Ϸ���
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


//ת���ַ�
VOID CDlgInsureGame::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//���붺��
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}

//��ť
VOID CDlgInsureGame::OnBnClickedSaveScore()
{
	//��ȡUI����
	LoadDataFromUI();
	SCORE lInputScore = m_InOutScore;

	//������֤
	if (lInputScore <= 0 || lInputScore > m_lUserScore )
	{
		//��������
		TCHAR szMessage[128]=TEXT("");

		//������Ϣ
		if(m_lUserScore>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���������Ϸ�ҽ�������1��%I64d֮��"), m_lUserScore);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("������Я������Ϸ�Ҳ���,���ܽ��д�����!"));
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,10);

		////���ÿؼ�
		//m_pEditAmount->SetFocus();
		//m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//״̬�ж�
	if (US_PLAYING==m_pIMeUserData->GetUserStatus())
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������Ϸ���ٴ�"),MB_ICONERROR,30);
		return;	
	}

	//��������
	CMD_GR_C_SaveScoreRequest SaveScoreRequest;
	SaveScoreRequest.cbActivityGame=TRUE;
	SaveScoreRequest.lSaveScore=lInputScore;

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_SAVE_SCORE_REQUEST,&SaveScoreRequest,sizeof(SaveScoreRequest));

	return;
}

//ȡ�ť
VOID CDlgInsureGame::OnBnClickedTakeScore()
{
	//��ȡUI����
	LoadDataFromUI();
	SCORE lInputScore = m_InOutScore;

	//������֤
	if (lInputScore <= 0 || lInputScore > m_lUserInsure )
	{
		//��������
		TCHAR szMessage[128]=TEXT("");

		//������Ϣ
		if(m_lUserInsure>0)
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("���������Ϸ�ҽ�������1��%I64d֮��"), m_lUserInsure);
		}
		else 
		{
			_sntprintf(szMessage,CountArray(szMessage),TEXT("�����е���Ϸ������,���ܽ���������!"));
		}

		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(szMessage,MB_ICONERROR,30);

		////���ÿؼ�
		//m_pEditAmount->SetFocus();
		//m_pEditAmount->SetText(TEXT(""));

		return;
	}

	//������֤
	TCHAR szPassword[LEN_PASSWORD]=TEXT("");
	lstrcpyn(szPassword,m_pEditPassword->GetText(),CountArray(szPassword));
	if (szPassword[0]==0)
	{
		//��ʾ��Ϣ
		CInformation Information(this);
		Information.ShowMessageBox(TEXT("�������벻��Ϊ�գ����������룡"),MB_ICONERROR,30);

		//���ÿؼ�
		m_pEditPassword->SetText(TEXT(""));
		m_pEditPassword->SetFocus();

		return;
	}

	//�������
	TCHAR szTempPassword[LEN_PASSWORD]=TEXT("");
	CopyMemory(szTempPassword,szPassword,sizeof(szTempPassword));
	CWHEncrypt::MD5Encrypt(szTempPassword,szPassword);

	//��������
	CMD_GR_C_TakeScoreRequest TakeScoreRequest;
	ZeroMemory(&TakeScoreRequest,sizeof(TakeScoreRequest));

	//����ṹ
	TakeScoreRequest.cbActivityGame=TRUE;
	TakeScoreRequest.lTakeScore=lInputScore;
	CopyMemory(TakeScoreRequest.szInsurePass, szPassword, sizeof(TakeScoreRequest.szInsurePass));

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_TAKE_SCORE_REQUEST,&TakeScoreRequest,sizeof(TakeScoreRequest));

	//�������
	m_pEditPassword->SetText(TEXT(""));

	return;
}

//ˢ�°�ť
VOID CDlgInsureGame::OnBnClickedRefreshScore()
{
	//��������
	CMD_GR_C_QueryInsureInfoRequest QueryInsureInfo;
	QueryInsureInfo.cbActivityGame=TRUE;

	//��������
	SendSocketData(MDM_GR_INSURE,SUB_GR_QUERY_INSURE_INFO,&QueryInsureInfo,sizeof(QueryInsureInfo));
}

//������Ϣ
void CDlgInsureGame::SetSendInfo(IClientKernel *pClientKernel,IClientUserItem const*pMeUserData)
{
	//����Ч��
	ASSERT(pClientKernel!=NULL);
	if(pClientKernel==NULL) return;

	//������Ϣ
	m_pIClientKernel = pClientKernel;
	m_pIMeUserData=const_cast<IClientUserItem *>(pMeUserData);

	//ˢ������
	OnBnClickedRefreshScore();

	return;
}

//����λ��
void CDlgInsureGame::SetPostPoint(CPoint Point)
{
	//CRect Rect;
	//GetWindowRect(&Rect);
	//SetWindowPos(NULL,Point.x,Point.y-Rect.bottom+Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,/*SWP_NOMOVE|*/SWP_NOZORDER);
}

//������Ϣ
void CDlgInsureGame::SendSocketData(WORD wMainCmdID, WORD wSubCmdID, void * pBuffer, WORD wDataSize)
{
	//����Ч��
	ASSERT(m_pIClientKernel!=NULL);

	//������Ϣ
	if(m_pIClientKernel) m_pIClientKernel->SendSocketData(wMainCmdID, wSubCmdID, pBuffer, wDataSize);

	return ;
}

//��ʾ����
void CDlgInsureGame::ShowItem()
{
	//��������
	bool bShow = true;
	if(m_blCanGetOnlyFree && m_pIMeUserData->GetUserStatus() == US_PLAYING) bShow = false;

	//���ÿؼ�
	m_pButtonSave->SetEnabled(m_blCanStore);
	m_pButtonTake->SetEnabled(bShow);
}


///////////////////////////////////////////////////////////////////////////////////////////////////