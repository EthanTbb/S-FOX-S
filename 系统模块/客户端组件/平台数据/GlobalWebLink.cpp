#include "StdAfx.h"
#include "GlobalWebLink.h"
#include "GlobalUserInfo.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量
CGlobalWebLink * CGlobalWebLink::m_pGlobalWebLink=NULL;

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGlobalWebLink::CGlobalWebLink()
{
	//设置变量
	if(m_pGlobalWebLink==NULL) m_pGlobalWebLink=this;

	//设置变量
	m_szSpreadLink[0]=0;
	m_szUpdateLink[0]=0;
	m_szPlatformLink[0]=0;
	m_szValidateLink[0]=0;
}

//析构函数
CGlobalWebLink::~CGlobalWebLink()
{
	//设置变量
	if(m_pGlobalWebLink==this) m_pGlobalWebLink=NULL;
}

//验证链接
LPCTSTR CGlobalWebLink::GetValidateLink()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	DWORD dwUserID = pGlobalUserData?pGlobalUserData->dwUserID:0L;
	DWORD dwTickCount = GetTickCount()-pGlobalUserData->dwLogonTickCount;

	//生成签名
	TCHAR szSignature[MAX_PATH]=TEXT("");
	_sntprintf(szSignature,CountArray(szSignature),TEXT("%d%s%d%s"),dwUserID,pGlobalUserData->szDynamicPass,dwTickCount,szValidateKey);

	//加密签名
	TCHAR szMD5Signature[LEN_MD5] = TEXT("");
	CWHEncrypt::MD5Encrypt(szSignature,szMD5Signature);

	//生成链接
	_sntprintf(m_szValidateLink,CountArray(m_szValidateLink),szValidateLink,dwUserID,dwTickCount,szMD5Signature);

	return m_szValidateLink;
}

//读取数值
UINT CGlobalWebLink::ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault)
{
	//获取目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\PlatformLink.INI"),szDirectory);

	//读取数据
	UINT nValue=GetPrivateProfileInt(pszKeyName,pszItemName,nDefault,szServerInfoPath);

	return nValue;
}

//读取数据
VOID CGlobalWebLink::ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount)
{
	//获取目录
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//构造路径
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\PlatformLink.INI"),szDirectory);

	//读取数据
	TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
	GetPrivateProfileString(pszKeyName,pszItemName,TEXT(""),szResultEncrypt,CountArray(szResultEncrypt),szServerInfoPath);

	//解密数据
	szResult[0]=0;
	CWHEncrypt::XorCrevasse(szResultEncrypt,szResult,wMaxCount);

	return;
}

//读取链接
VOID CGlobalWebLink::LoadPaltformLink()
{
	//平台链接
	ReadFileString(TEXT("PLATFORMLINK"),TEXT("PlatformLink"),m_szPlatformLink,MAX_PATH);	
	if(m_szPlatformLink[0]==0)
	{
		lstrcpyn(m_szPlatformLink,szPlatformLink,CountArray(m_szPlatformLink));
	}

	//推广链接
	ReadFileString(TEXT("PLATFORMLINK"),TEXT("SpreadLink"),m_szSpreadLink,MAX_PATH);	
	if(m_szSpreadLink[0]==0)
	{
		lstrcpyn(m_szSpreadLink,m_szPlatformLink,CountArray(m_szSpreadLink));
	}

	//更新链接
	ReadFileString(TEXT("PLATFORMLINK"),TEXT("UpdateLink"),m_szUpdateLink,MAX_PATH);	
	if(m_szUpdateLink[0]==0)
	{
		lstrcpyn(m_szUpdateLink,m_szPlatformLink,CountArray(m_szUpdateLink));
	}
	
	return;
}

//////////////////////////////////////////////////////////////////////////////////
