#include "StdAfx.h"
#include "UserItemElement.h"

//////////////////////////////////////////////////////////////////////////////////
//常理定义

#define MAX_EXPERIENCE				5									//最大经验

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CUserItemElement * CUserItemElement::m_pUserItemElement=NULL;			//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUserItemElement::CUserItemElement()
{
	//变量定义
	m_wLevelCount=0;
	m_pGrowLevelConfig=NULL;
	m_wMemberCount=0;
	ZeroMemory(&m_MemberParameter, sizeof(m_MemberParameter));

	//设置变量
	m_pIUserOrderParser=NULL;
	m_pIFaceItemControl=NULL;

	//设置对象
	ASSERT(m_pUserItemElement==NULL);
	if (m_pUserItemElement==NULL) m_pUserItemElement=this;

	return;
}

//析构函数
CUserItemElement::~CUserItemElement()
{
	//释放对象
	ASSERT(m_pUserItemElement==this);
	if (m_pUserItemElement==this) 
	{
		//释放资源
		SafeDeleteArray(m_pGrowLevelConfig);

		m_pUserItemElement=NULL;
	}

	return;
}

//设置接口
VOID CUserItemElement::SetUserOrderParser(IUserOrderParser * pIUserOrderParser)
{
	//设置变量
	m_pIUserOrderParser=pIUserOrderParser;

	return;
}

//设置接口
VOID CUserItemElement::SetFaceItemControl(IFaceItemControl * pIFaceItemControl)
{
	//设置变量
	m_pIFaceItemControl=pIFaceItemControl;

	return;
}

//配置等级
VOID CUserItemElement::SetGrowLevelConfig(WORD wLevelCount,tagGrowLevelConfig * pGrowLevelConfig)
{
	//参数校验
	ASSERT(wLevelCount>0 && pGrowLevelConfig!=NULL);
	if(wLevelCount==0 || pGrowLevelConfig==NULL) return;

	try
	{
		//申请资源
		m_pGrowLevelConfig = new tagGrowLevelConfig[wLevelCount];
		if(m_pGrowLevelConfig==NULL) throw TEXT("内存不足,资源申请失败！");

		//设置变量
		m_wLevelCount=wLevelCount;
		CopyMemory(m_pGrowLevelConfig,pGrowLevelConfig,sizeof(tagGrowLevelConfig)*m_wLevelCount);
	}
	catch(...)
	{
		ASSERT(FALSE);

		//释放资源
		SafeDeleteArray(m_pGrowLevelConfig);
	}

	return;
}

VOID CUserItemElement::SetMemberParameter(tagMemberParameterNew* pMemberParameter, WORD wMemberCount)
{
	m_wMemberCount=wMemberCount;
	memcpy(&m_MemberParameter, pMemberParameter, sizeof(tagMemberParameterNew)*wMemberCount);
}

const tagMemberParameterNew* CUserItemElement::GetUserMemberParameter(const WORD wUserMember)
{
	for(int i = 0; i < m_wMemberCount; i++)
	{
		if(m_MemberParameter[i].cbMemberOrder == wUserMember)
		{
			return &m_MemberParameter[i];
		}
	}
	return NULL;
}

//绘画经验
VOID CUserItemElement::DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience)
{
	if (dwExperience>=0L)
	{
		//获取句柄
		HINSTANCE hInstance = GetModuleHandle(SHARE_CONTROL_DLL_NAME);

		//加载资源
		CPngImage ImageLevelBack;
		ImageLevelBack.LoadImage(hInstance,TEXT("IMAGE_LEVEL_BACK"));
		ImageLevelBack.DrawImage(pDC,nXPos,nYPos);

		CPngImage ImageLevelNumber;
		ImageLevelNumber.LoadImage(hInstance,TEXT("IMAGE_LEVEL_NUMBER"));

		//获取经验
		WORD wExperienceLevel=GetExperienceLevel(dwExperience);

		//绘画等级		
		DrawNumber(pDC,&ImageLevelNumber,nXPos+14,nYPos,TEXT("0123456789"),wExperienceLevel,TEXT("%d"));
	}

	return;
}

VOID CUserItemElement::DrawExperience(CDC * pDC, CRect & rcSubItem, DWORD dwExperience)
{
	if (dwExperience>=0L)
	{
		//获取经验
		WORD wExperienceLevel=GetExperienceLevel(dwExperience);

		//获取个数
		int nTempLevel =wExperienceLevel;
		int nCount = 0;
		while (nTempLevel!=0)
		{
			nTempLevel/=10;
			nCount++;
		}
		//获取句柄
		HINSTANCE hInstance = GetModuleHandle(SHARE_CONTROL_DLL_NAME);
		//加载资源
		CPngImage ImageLevelBack;
		ImageLevelBack.LoadImage(hInstance,TEXT("IMAGE_LEVEL_BACK"));
		CPngImage ImageLevelNumber;
		ImageLevelNumber.LoadImage(hInstance,TEXT("IMAGE_LEVEL_NUMBER"));

		//获取位置
		int nYPos = rcSubItem.top + (rcSubItem.Height() - ImageLevelBack.GetHeight()) /2;
		int nXPos = rcSubItem.left + (rcSubItem.Width() - ImageLevelBack.GetWidth() - ImageLevelNumber.GetWidth()* nCount /10) /2;

		//绘画等级	
		ImageLevelBack.DrawImage(pDC,nXPos,nYPos);
		DrawNumber(pDC,&ImageLevelNumber,nXPos+ImageLevelBack.GetWidth(),nYPos,TEXT("0123456789"),wExperienceLevel,TEXT("%d"));
	}

		return;
}

//构造名片
VOID CUserItemElement::ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard)
{
	//创建名片
	CSize ImageSize;
	ImageSize.SetSize(NAME_CARD_CX,NAME_CARD_CY);
	ImageNameCard.Create(ImageSize.cx,ImageSize.cy,32);

	//变量定义
	TCHAR szString[128]=TEXT("");
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

	//设置 DC
	CDC * pDC=CDC::FromHandle(ImageNameCard.GetDC());
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//设置 DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(CSkinMenuKernel::m_SkinAttribute.m_crNormalText);

	//位置定义
	CRect rcNickName(60,5,ImageSize.cx-5,5+12);
	CRect rcGameLevel(60,25,ImageSize.cx-5,25+12);
	CRect rcExperience(60,45,ImageSize.cx-5,45+12);
	CRect rcUserGameID(60,65,ImageSize.cx-5,65+12);
	CRect rcUserUnderWrite(60,85,ImageSize.cx-5,85+12);

	//绘画背景
	pDC->FillSolidRect(0,0,ImageSize.cx,ImageSize.cy,CSkinMenuKernel::m_SkinAttribute.m_crNormalBack);

	//绘画头像
	if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
	{
		m_pIFaceItemControl->DrawFaceItemFrame(pDC,2,16,ITEM_FRAME_NORMAL);
		m_pIFaceItemControl->DrawFaceNormal(pDC,2,16,pCustomFaceInfo->dwCustomFace);
	}
	else
	{
		m_pIFaceItemControl->DrawFaceItemFrame(pDC,2,16,ITEM_FRAME_NORMAL);
		m_pIFaceItemControl->DrawFaceNormal(pDC,2,16,pIClientUserItem->GetFaceID());
	}

	//用户信息
	LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
	LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

	//用户信息
	LPCTSTR pszUserOrder=(pszMaster[0]!=0)?pszMaster:((pszMember[0]!=0)?pszMember:TEXT("普通玩家"));
	_sntprintf(szString,CountArray(szString),TEXT("昵称：%s [ %s ]"),pIClientUserItem->GetNickName(),pszUserOrder);
	pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//积分等级
	LPCTSTR pszKindName=pIGameLevelParser->GetKindName();
	LPCTSTR pszGameLevel=pIGameLevelParser->GetLevelDescribe(pIClientUserItem);
	_sntprintf(szString,CountArray(szString),TEXT("级别：%s [ %s ]"),pszGameLevel,pszKindName);
	pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//QQ号码	
	_sntprintf(szString,CountArray(szString),TEXT("  QQ：%s"),pUserInfo->szQQ);
	pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//手机号码
	_sntprintf(szString,CountArray(szString),TEXT("手机：%s"),pUserInfo->szMobilePhone);
	pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//个性签名
	_sntprintf(szString,CountArray(szString),TEXT("签名：%s"),pUserInfo->szUnderWrite);
	pDC->DrawText(szString,lstrlen(szString),&rcUserUnderWrite,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//释放资源
	ImageNameCard.ReleaseDC();

	return;
}

//密码等级
BYTE CUserItemElement::GetPasswordLevel(LPCTSTR pszPassword)
{
	//变量定义
	BYTE cbPasswordInfo[4]={0,0,0,0};
	UINT uPasswordLength=lstrlen(pszPassword);

	//长度判断
	if (lstrlen(pszPassword)<LEN_LESS_PASSWORD)
	{
		return PASSWORD_LEVEL_0;
	}

	//小写字母
	for (UINT i=0;i<uPasswordLength;i++)
	{
		//小写字母
		if ((pszPassword[i]>=TEXT('a'))&&((pszPassword[i]<=TEXT('z'))))
		{
			cbPasswordInfo[0]=1;
			continue;
		}

		//大写字母
		if ((pszPassword[i]>=TEXT('A'))&&((pszPassword[i]<=TEXT('Z'))))
		{
			cbPasswordInfo[1]=1;
			continue;
		}

		//数字字符
		if ((pszPassword[i]>=TEXT('0'))&&((pszPassword[i]<=TEXT('9'))))
		{
			cbPasswordInfo[2]=1;
			continue;
		}

		//其他字符
		cbPasswordInfo[3]=1;
	}

	//判断等级
	BYTE cbLevelCount=0;
	for (BYTE i=0;i<CountArray(cbPasswordInfo);i++)
	{
		if (cbPasswordInfo[i]>0)
		{
			cbLevelCount++;
		}
	}

	return PASSWORD_LEVEL_0+__min(cbLevelCount,(PASSWORD_LEVEL_3-PASSWORD_LEVEL_0));
}

//获取等级
WORD CUserItemElement::GetExperienceLevel(DWORD dwExperience)
{
	if (dwExperience>=0L && m_pGrowLevelConfig!=NULL)
	{
		//变量定义
		WORD wUserLevel=1;

		//等级计算
		for (WORD wLevelIndex=0;wUserLevel<=m_wLevelCount;wLevelIndex++)
		{
			if(dwExperience<m_pGrowLevelConfig[wLevelIndex].dwExperience) break;			

			//设置变量
			wUserLevel=m_pGrowLevelConfig[wLevelIndex].wLevelID;
		}

		return wUserLevel;
	}

	return 0L;
}

//等级信息
WORD CUserItemElement::GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount)
{
	//变量定义
	WORD wIndexCount=0;
	WORD wLevelVaule[]={1L,5L,10L};

	//设置等级
	for (WORD i=0;i<wMaxCount;i++)
	{
		//获取索引
		for (INT j=0;j<CountArray(wLevelVaule);j++)
		{
			if (wUserLevel>=wLevelVaule[CountArray(wLevelVaule)-j-1])
			{
				//设置变量
				wUserLevel-=wLevelVaule[CountArray(wLevelVaule)-j-1];
				cbOrderIndex[wIndexCount++]=CountArray(wLevelVaule)-j-1;

				break;
			}
		}

		//终止判断
		if (wUserLevel==0L) break;
	}

	return wIndexCount;
}

//等级信息
WORD CUserItemElement::GetExperienceInfo(tagGrowLevelConfig GrowLevelConfig[],WORD wBufferCount)
{
	//变量定义
	WORD wLevelCount=__min(m_wLevelCount,wBufferCount);

	//拷贝数据
	CopyMemory(GrowLevelConfig,m_pGrowLevelConfig,wLevelCount*sizeof(GrowLevelConfig[0]));

	return wLevelCount;
}

bool CUserItemElement::EfficacyIllegal(LPCTSTR pszIllegal)
{
	CStringArray csArrIllegal;
	csArrIllegal.Add(TEXT("select"));
	csArrIllegal.Add(TEXT("insert"));
	csArrIllegal.Add(TEXT("delete"));
	csArrIllegal.Add(TEXT("from"));
	csArrIllegal.Add(TEXT("count"));
	csArrIllegal.Add(TEXT("drop table"));
	csArrIllegal.Add(TEXT("update"));
	csArrIllegal.Add(TEXT("truncate"));
	csArrIllegal.Add(TEXT("asc"));
	csArrIllegal.Add(TEXT("mid"));
	csArrIllegal.Add(TEXT("char"));
	csArrIllegal.Add(TEXT("xp_cmdshell"));
	csArrIllegal.Add(TEXT("exec master"));
	csArrIllegal.Add(TEXT("net localgroup administrators"));
	csArrIllegal.Add(TEXT(":"));
	csArrIllegal.Add(TEXT("net user"));
	csArrIllegal.Add(TEXT("\""));
	csArrIllegal.Add(TEXT("'"));
	csArrIllegal.Add(TEXT("or"));
	csArrIllegal.Add(TEXT("exec"));
	csArrIllegal.Add(TEXT("exectue"));
	csArrIllegal.Add(TEXT("sp_executesql"));

	CString csTest(pszIllegal);

	INT nIllegalCount=(INT)(csArrIllegal.GetCount());

	for (int i = 0;i<nIllegalCount;i++)
	{
		CString csIllegal = csArrIllegal.GetAt(i);

		if (_tcsstr(csTest,csIllegal))
		{
			return false;
		}
	}

	return true;
}

//汉字验证
bool CUserItemElement::EfficacyChineseCharacter(TCHAR chr)
{
	//校验字符
	if (chr != 0x08 && chr <= 0x80) return false;
	if (chr == 0xb7) return false;	
	if ((chr&0xff00) == 0xff00) return false;	
	if ((chr&0xff00) == 0x2000 && ::IsDBCSLeadByte(BYTE(chr&0xFF)) == FALSE) return false;
	if ((chr&0xff00) == 0x3000 && ::IsDBCSLeadByte(BYTE(chr&0xFF)) == FALSE) return false;

	return true;
}

//效验邮箱
bool CUserItemElement::EfficacyEmail(LPCTSTR pszEmail, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	bool bIsValid=false;
	CString strEmail(pszEmail);

	//长度校验
	if(strEmail.GetLength() < LEN_EMAIL)
	{
		//统计变量		
		int nAtPos=0;
		int nAtCount=0;
		int nDotCount=0;

		//变量定义
		TCHAR chEmail = strEmail[0];

		// 首字符判断
		if((chEmail >= 'a' && chEmail <= 'z') || (chEmail >= 'A' && chEmail <= 'Z') || (chEmail >= '0' && chEmail <= '9'))
		{ 
			//设置变量
			bIsValid=true;

			for(int i = 1; i < strEmail.GetLength(); i++)
			{
				chEmail = strEmail[i];

				// 字符校验
				if((chEmail >= 'a' && chEmail <= 'z') || 
				   (chEmail >= 'A' && chEmail <= 'Z') || 
				   (chEmail >= '0' && chEmail <= '9') || 
				   (chEmail == '_') || (chEmail == '-') || (chEmail == '.') || (chEmail == '@'))
				{
					// 统计'@'个数
					if(chEmail == '@')
					{
						nAtPos=i;
						nAtCount++;						
					}
					// 统计'@'之后'.'的个数
					else if((nAtCount > 0) && (chEmail == '.'))
					{
						nDotCount++;
					}
				}
				else
				{
					bIsValid=false;
					break;
				}
			}
		}
		// 结尾字符判断
		if((bIsValid == true) && (chEmail == '.') || (chEmail == '-'))
		{
			bIsValid=false;
		}

		// 字符数量判断
		if((bIsValid == true) && (nAtCount != 1) || (nDotCount < 1) || (nDotCount > 3)) 
		{
			bIsValid=false;
		}

		// 特殊字符判断
		if(bIsValid == true)
		{
			//变量定义
			LPCTSTR pszInvalid[]={TEXT("@."),TEXT(".@"),TEXT(".."),TEXT("--"),TEXT("-@"),TEXT("@-")};

			for(int i = 0; i < CountArray(pszInvalid); i++)
			{
				if(strEmail.Find(pszInvalid[i]) > 0)
				{
					bIsValid=false;
					break;
				}
			}
		}
	}

	//邮箱无效
	if(bIsValid == false)
	{
		CopyMemory(szDescribe, TEXT("邮箱输入有误，请重新输入！"), sizeof(TCHAR)*wMaxCount);
		return false;
	}

	//非法校验
	if (!EfficacyIllegal(pszEmail))
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("邮箱包含非法字符串，请重新输入！"));
		return false;
	}

	return true;
}

//效验帐号
bool CUserItemElement::EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	TCHAR szSpecialChar=TEXT('_');	
	TCHAR szNumber[]={TEXT('0'),TEXT('9')};
	TCHAR szLowerLetter[]={TEXT('a'),TEXT('z')};
	TCHAR szUpperLetter[]={TEXT('A'),TEXT('Z')};

	//变量定义
	INT nChinessCount=0;
	INT nCharLength=lstrlen(pszAccounts);

	//计算字符
	for (INT i=0;i<nCharLength;i++)
	{
		if (EfficacyChineseCharacter(pszAccounts[i])==false)
		{
			//特殊字符
			if(pszAccounts[i]==szSpecialChar) continue;

			//数字字符
            if(pszAccounts[i]>=szNumber[0] && pszAccounts[i]<=szNumber[1]) continue;

			//小写字母
            if(pszAccounts[i]>=szLowerLetter[0] && pszAccounts[i]<=szLowerLetter[1]) continue;

			//大写字母
            if(pszAccounts[i]>=szUpperLetter[0] && pszAccounts[i]<=szUpperLetter[1]) continue;

			//非法字符
			_sntprintf(szDescribe,wMaxCount,TEXT("游戏账号必须由字母、数字、下划线和汉字组成！"));

			return false;
		}
		else
		{
			nChinessCount++;
		}
	}

	//长度判断
	if ((nChinessCount+nCharLength)<LEN_LESS_ACCOUNTS)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("游戏帐号必须大于%ld位以上，请重新输入！"),LEN_LESS_ACCOUNTS);
		return false;
	}

	//非法校验
	if (!EfficacyIllegal(pszAccounts))
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("游戏帐号包含非法字符串，请重新输入！"));
		return false;
	}

	return true;
}

//效验昵称
bool CUserItemElement::EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	INT nCharLength=lstrlen(pszNickName);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//长度判断
	if (nCharLength<LEN_LESS_NICKNAME)
	{
		//变量定义
		WORD nChinessCount=0;

		//计算字符
		for (INT i=0;i<nCharLength;i++)
		{
			if (pszNickName[i]>=0x80)
			{
				nChinessCount++;
			}
		}

		//长度判断
		if ((nChinessCount+nCharLength)<LEN_LESS_NICKNAME)
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("游戏昵称必须大于%ld位以上，请重新输入！"),LEN_LESS_NICKNAME);
			return false;
		}
	}

	//非法校验
	if (!EfficacyIllegal(pszNickName))
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("游戏昵称包含非法字符串，请重新输入！"));
		return false;
	}

	return true;
}

//效验密码
bool CUserItemElement::EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	INT nCharLength=lstrlen(pszPassword);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	if(nCharLength == 0)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("密码不能为空，请重新输入！"),LEN_LESS_PASSWORD);
		return false;
	}

	//长度判断
	if (nCharLength<LEN_LESS_PASSWORD)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("密码必须大于 %ld 位以上，请重新输入！"),LEN_LESS_PASSWORD);
		return false;
	}


	//变量定义
	TCHAR szSpecialChar=TEXT('_');	
	TCHAR szNumber[]={TEXT('0'),TEXT('9')};
	TCHAR szLowerLetter[]={TEXT('a'),TEXT('z')};
	TCHAR szUpperLetter[]={TEXT('A'),TEXT('Z')};

	//计算字符
	for (INT i=0;i<nCharLength;i++)
	{


		//特殊字符
		if(pszPassword[i]==szSpecialChar) continue;

		//数字字符
		if(pszPassword[i]>=szNumber[0] && pszPassword[i]<=szNumber[1]) continue;

		//小写字母
		if(pszPassword[i]>=szLowerLetter[0] && pszPassword[i]<=szLowerLetter[1]) continue;

		//大写字母
		if(pszPassword[i]>=szUpperLetter[0] && pszPassword[i]<=szUpperLetter[1]) continue;

		//非法字符
		_sntprintf(szDescribe,wMaxCount,TEXT("密码必须由字母、数字、下划线组成！"),LEN_LESS_ACCOUNTS);

		return false;
	}

	//非法校验
	if (!EfficacyIllegal(pszPassword))
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("密码包含非法字符串，请重新输入！"));
		return false;
	}

	return true;
}

//效验证件
bool CUserItemElement::EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount)
{
	//长度判断
	UINT nCharLength=lstrlen(pszPassPortID);
	if(nCharLength==0L)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("身份证号不能为空，请重新输入！"),LEN_LESS_PASSWORD);
		return false;
	}

	//变量定义
	bool bNumber=true;

	//数字判断
	for (UINT i=0;i<nCharLength;i++)
	{
		if ((pszPassPortID[i]<TEXT('0'))||(pszPassPortID[i]>TEXT('9')))
		{
			if(pszPassPortID[i] != TEXT('x') && pszPassPortID[i] != TEXT('X'))
			{
				bNumber=false;
				break;
			}
		}
	}

	//合法判断
	if ((bNumber==false)||((nCharLength!=18L)))
	{
		lstrcpyn(szDescribe,TEXT("身份证号码必须为 18 位数字，请重新输入！"),wMaxCount);
		return false;
	}

	//变量定义
	bool bNewPassPortID = (nCharLength==18L);
	bool bYear=true;
	bool bMonth=true;
	bool bDay=true;
	bool bCheck=true;

	//合法判断
	if(bNewPassPortID)
	{
		TCHAR szYear[10]={0};
		for(BYTE i=0; i<4; i++)
		{
			szYear[i]=pszPassPortID[i+6];
		}
		int nYear = StrToInt(szYear);
		if(nYear < 1900 || nYear > 2100)
		{
			bYear=false;
		}
	}

	//合法判断
	BYTE cbIndex=8;
	if(bNewPassPortID)cbIndex=10;
	TCHAR szMonth[10]={0};
	for(BYTE i=0; i<2; i++)
	{
		szMonth[i]=pszPassPortID[i+cbIndex];
	}
	int nMonth = StrToInt(szMonth);
	if(nMonth < 1 || nMonth > 12)
	{
		bMonth=false;
	}

	//合法判断
	cbIndex=10;
	if(bNewPassPortID)cbIndex=12;
	TCHAR szDay[10]={0};
	for(BYTE i=0; i<2; i++)
	{
		szDay[i]=pszPassPortID[i+cbIndex];
	}
	int nDay = StrToInt(szDay);
	if(nDay < 1 || nDay > 31)
	{
		bDay=false;
	}

	//合法判断
	if(bNewPassPortID)
	{
		TCHAR szTemp[10]={0};
		int nSum=0;
		int nWi[]={7,9,10,5,8,4,2,1,6,3,7,9,10,5,8,4,2,1};
		TCHAR cCheckArry[]={TEXT('1'),TEXT('0'),TEXT('X'),TEXT('9'),TEXT('8'),TEXT('7'),TEXT('6'),TEXT('5'),TEXT('4'),TEXT('3'),TEXT('2')};
		for(BYTE i=0; i<17; i++)
		{
			szTemp[0]=pszPassPortID[i];
			int nAi=StrToInt(szTemp);
			nSum+=(nAi*nWi[i]);
		}
		TCHAR cCheck=cCheckArry[nSum%11];
		TCHAR cCheck2=pszPassPortID[17];

		if((cCheck2 != cCheck) && !((cCheck==TEXT('X')) && (cCheck2==TEXT('x'))))
		{
			bCheck=false;
		}
	}

	//合法判断
	if(!bYear||!bMonth||!bDay||!bCheck)
	{
		lstrcpyn(szDescribe,TEXT("您输入的身份证号码不正确，请重新输入正确的身份证号码！"),wMaxCount);
		return false;
	}

	return true;
}

//效验姓名
bool CUserItemElement::EfficacyCompellation(LPCTSTR pszCompellation, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	INT nChinessCount=0;
	INT nCharLength=lstrlen(pszCompellation);

	if(nCharLength == 0)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("真实姓名不能为空，请重新输入！"),LEN_LESS_ACCOUNTS);

		return false;
	}

	//计算字符
	for (INT i=0;i<nCharLength;i++)
	{
		if (EfficacyChineseCharacter(pszCompellation[i])==false)
		{
			//非法字符
			_sntprintf(szDescribe,wMaxCount,TEXT("真实姓名只能由汉字组成，请重新输入！"),LEN_LESS_ACCOUNTS);

			return false;
		}
		else
		{
			nChinessCount++;
		}
	}

	//长度判断
	if ((nChinessCount+nCharLength)>LEN_COMPELLATION)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("真实姓名字符必须小于等于%ld位，请重新输入！"),LEN_COMPELLATION);
		return false;
	}

	//长度判断
	if ((nChinessCount+nCharLength)<LEN_LESS_COMPELLATION)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("真实姓名字符必须大于等于%ld位，请重新输入！"),LEN_LESS_COMPELLATION);
		return false;
	}	

	//非法校验
	if (!EfficacyIllegal(pszCompellation))
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("真实姓名包含非法字符串，请重新输入！"));
		return false;
	}

	return true;
}

//效验手机
bool CUserItemElement::EfficacyMobilePhone(LPCTSTR pszMobilePhone, TCHAR szDescribe[], WORD wMaxCount)
{
	//长度校验
	if(lstrlen(pszMobilePhone) == LEN_MOBILE_PHONE-1)
	{
		//首号校验
		LPCTSTR pszFirstNo=TEXT("1");
		if(StrChr(pszFirstNo,pszMobilePhone[0]) != NULL)
		{
			//号段定义
			LPCTSTR pszDXMobilePhone[]={TEXT("3578"),TEXT("3"),TEXT("3"),TEXT("7"),TEXT("019")};
			LPCTSTR pszLTMobilePhone[]={TEXT("34578"),TEXT("012"),TEXT("5"),TEXT("56"),TEXT("6"),TEXT("56")};
			LPCTSTR pszYDMobilePhone[]={TEXT("34578"),TEXT("456789"),TEXT("7"),TEXT("012789"),TEXT("8"),TEXT("23478")};

			//变量定义
			LPCTSTR pszMobileNo=NULL;

			//电信号段
			if(pszMobileNo = StrChr(pszDXMobilePhone[0],pszMobilePhone[1]))
			{
				INT_PTR nPos = pszMobileNo-pszDXMobilePhone[0];
				if(StrChr(pszDXMobilePhone[nPos+1],pszMobilePhone[2]) != NULL)
				{
					CopyMemory(szDescribe,TEXT("电信号段"),sizeof(TCHAR)*wMaxCount);
					return true;
				}
			}

			//联通号段
			if(pszMobileNo = StrChr(pszLTMobilePhone[0],pszMobilePhone[1]))
			{
				INT_PTR nPos = pszMobileNo-pszLTMobilePhone[0];
				if(StrChr(pszLTMobilePhone[nPos+1],pszMobilePhone[2]) != NULL)
				{
					CopyMemory(szDescribe,TEXT("联通号段"),sizeof(TCHAR)*wMaxCount);
					return true;
				}
			}

			//移动号段
			if(pszMobileNo = StrChr(pszYDMobilePhone[0],pszMobilePhone[1]))
			{
				INT_PTR nPos = pszMobileNo-pszYDMobilePhone[0];
				if(StrChr(pszYDMobilePhone[nPos+1],pszMobilePhone[2]) != NULL)
				{
					CopyMemory(szDescribe,TEXT("移动号段"),sizeof(TCHAR)*wMaxCount);
					return true;
				}
			}
		}
	}

	//非法号码
	CopyMemory(szDescribe,TEXT("您的手机号码输入有误,请重新输入！"),sizeof(TCHAR)*wMaxCount);

	return false;
}

bool CUserItemElement::EfficacyQQ(LPCTSTR pszQQ, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	int nQQCount=lstrlen(pszQQ);

	for (int i=0;i<nQQCount;i++)
	{
		if (!(
			(pszQQ[i]==TEXT('0')) ||
			(pszQQ[i]==TEXT('1')) ||
			(pszQQ[i]==TEXT('2')) ||
			(pszQQ[i]==TEXT('3')) ||
			(pszQQ[i]==TEXT('4')) ||
			(pszQQ[i]==TEXT('5')) ||
			(pszQQ[i]==TEXT('6')) ||
			(pszQQ[i]==TEXT('7')) ||
			(pszQQ[i]==TEXT('8')) ||
			(pszQQ[i]==TEXT('9')) 
			))
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("QQ号码不正确，请重新输入！"));
			return false;
		}
	}

	//长度判断
	if ((nQQCount)<5)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("QQ号码必须大于等于%ld位，请重新输入！"),5);
		return false;
	}	

	if ((nQQCount)>12)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("QQ号码必须小于%ld位，请重新输入！"),12);
		return false;
	}	


	return true;
}

VOID CUserItemElement::ProtectCompellation(LPCTSTR pszCompellation,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//参数校验
	ASSERT(pszCompellation != NULL);
	if(pszCompellation == NULL) return;

	//拷贝数据
	CopyMemory(szResult, pszCompellation, sizeof(TCHAR)*wMaxCount);

	//获取长度
	int nLen = lstrlen(szResult);

	//替换掩码
	for(int i=0;i<nLen;i++)
	{
		if(i >= 1) szResult[i] = MaskChar;
	}

	szResult[i]=0;

}

//证件保护
VOID CUserItemElement::ProtectPassPortID(LPCTSTR pszPassPortID,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//参数校验
	ASSERT(pszPassPortID != NULL);
	if(pszPassPortID == NULL) return;

	//拷贝数据
	CopyMemory(szResult, pszPassPortID, sizeof(TCHAR)*wMaxCount);

	//获取长度
	int nLen = lstrlen(szResult);

	//替换掩码
	for(int i=0;i<nLen;i++)
	{
		if(i > 3 && i < nLen-4) szResult[i] = MaskChar;
	}

	szResult[i]=0;
}

//号码保护
VOID CUserItemElement::ProtectSeatPhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//参数校验
	ASSERT(pszModileMuber!=NULL);
	if(pszModileMuber==NULL) return;

	//拷贝数据
	CopyMemory(szResult, pszModileMuber, sizeof(TCHAR)*wMaxCount);

	//获取长度
	int nLen = lstrlen(szResult);

	//替换掩码
	for(int i = 0; i < nLen; i++)
	{
		if(i > 3 && i < nLen-3) szResult[i] = MaskChar;
	}

	szResult[i] = 0;
}

//号码保护
VOID CUserItemElement::ProtectMobilePhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar)
{
	//参数校验
	ASSERT(pszModileMuber!=NULL);
	if(pszModileMuber==NULL) return;

	//拷贝数据
	CopyMemory(szResult, pszModileMuber, sizeof(TCHAR)*wMaxCount);

	//获取长度
	int nLen = lstrlen(szResult);

	//替换掩码
	for(int i = 0; i < nLen; i++)
	{
		if(i >= 3 && i < nLen-4) szResult[i] = MaskChar;
	}

	szResult[i] = 0;
}


//绘制等级
VOID CUserItemElement::DrawNumber(CDC * pDC,CPngImage * pImageNumber,INT nXPos,INT nYPos,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat)
{
	//参数校验
	if(pImageNumber==NULL) return;

	//变量定义
	CString strNumber(pszNumber);
	CSize SizeNumber(pImageNumber->GetWidth()/strNumber.GetLength(),pImageNumber->GetHeight());

	//变量定义	
	TCHAR szValue[32]=TEXT("");
	_sntprintf(szValue,CountArray(szValue),pszFormat,nNumber);

	//调整横坐标
	if((nFormat&DT_LEFT)!=0) nXPos -= 0;
	if((nFormat&DT_CENTER)!=0) nXPos -= (lstrlen(szValue)*SizeNumber.cx)/2;
	if((nFormat&DT_RIGHT)!=0) nXPos -= lstrlen(szValue)*SizeNumber.cx;
	
	//调整纵坐标
	if((nFormat&DT_TOP)!=0) nYPos -= 0;
	if((nFormat&DT_VCENTER)!=0) nYPos -= SizeNumber.cy/2;
	if((nFormat&DT_BOTTOM)!=0) nYPos -= SizeNumber.cy;

	//绘画数字
	INT nIndex=0;
	CStdString ImageModify;
	for(INT i=0; i<lstrlen(szValue);i++)
	{
		nIndex = strNumber.Find(szValue[i]);
		if(nIndex!=-1)
		{
			pImageNumber->DrawImage(pDC,nXPos,nYPos,SizeNumber.cx,SizeNumber.cy,SizeNumber.cx*nIndex,0,SizeNumber.cx,SizeNumber.cy);
			nXPos += SizeNumber.cx;
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
