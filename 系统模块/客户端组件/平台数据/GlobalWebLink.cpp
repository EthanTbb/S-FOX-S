#include "StdAfx.h"
#include "GlobalWebLink.h"
#include "GlobalUserInfo.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����
CGlobalWebLink * CGlobalWebLink::m_pGlobalWebLink=NULL;

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGlobalWebLink::CGlobalWebLink()
{
	//���ñ���
	if(m_pGlobalWebLink==NULL) m_pGlobalWebLink=this;

	//���ñ���
	m_szSpreadLink[0]=0;
	m_szUpdateLink[0]=0;
	m_szPlatformLink[0]=0;
	m_szValidateLink[0]=0;
}

//��������
CGlobalWebLink::~CGlobalWebLink()
{
	//���ñ���
	if(m_pGlobalWebLink==this) m_pGlobalWebLink=NULL;
}

//��֤����
LPCTSTR CGlobalWebLink::GetValidateLink()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//��������
	DWORD dwUserID = pGlobalUserData?pGlobalUserData->dwUserID:0L;
	DWORD dwTickCount = GetTickCount()-pGlobalUserData->dwLogonTickCount;

	//����ǩ��
	TCHAR szSignature[MAX_PATH]=TEXT("");
	_sntprintf(szSignature,CountArray(szSignature),TEXT("%d%s%d%s"),dwUserID,pGlobalUserData->szDynamicPass,dwTickCount,szValidateKey);

	//����ǩ��
	TCHAR szMD5Signature[LEN_MD5] = TEXT("");
	CWHEncrypt::MD5Encrypt(szSignature,szMD5Signature);

	//��������
	_sntprintf(m_szValidateLink,CountArray(m_szValidateLink),szValidateLink,dwUserID,dwTickCount,szMD5Signature);

	return m_szValidateLink;
}

//��ȡ��ֵ
UINT CGlobalWebLink::ReadFileValue(LPCTSTR pszKeyName, LPCTSTR pszItemName, UINT nDefault)
{
	//��ȡĿ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\PlatformLink.INI"),szDirectory);

	//��ȡ����
	UINT nValue=GetPrivateProfileInt(pszKeyName,pszItemName,nDefault,szServerInfoPath);

	return nValue;
}

//��ȡ����
VOID CGlobalWebLink::ReadFileString(LPCTSTR pszKeyName, LPCTSTR pszItemName, TCHAR szResult[], WORD wMaxCount)
{
	//��ȡĿ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	//����·��
	TCHAR szServerInfoPath[MAX_PATH]=TEXT("");
	_sntprintf(szServerInfoPath,CountArray(szServerInfoPath),TEXT("%s\\PlatformLink.INI"),szDirectory);

	//��ȡ����
	TCHAR szResultEncrypt[MAX_ENCRYPT_LEN];
	GetPrivateProfileString(pszKeyName,pszItemName,TEXT(""),szResultEncrypt,CountArray(szResultEncrypt),szServerInfoPath);

	//��������
	szResult[0]=0;
	CWHEncrypt::XorCrevasse(szResultEncrypt,szResult,wMaxCount);

	return;
}

//��ȡ����
VOID CGlobalWebLink::LoadPaltformLink()
{
	//ƽ̨����
	ReadFileString(TEXT("PLATFORMLINK"),TEXT("PlatformLink"),m_szPlatformLink,MAX_PATH);	
	if(m_szPlatformLink[0]==0)
	{
		lstrcpyn(m_szPlatformLink,szPlatformLink,CountArray(m_szPlatformLink));
	}

	//�ƹ�����
	ReadFileString(TEXT("PLATFORMLINK"),TEXT("SpreadLink"),m_szSpreadLink,MAX_PATH);	
	if(m_szSpreadLink[0]==0)
	{
		lstrcpyn(m_szSpreadLink,m_szPlatformLink,CountArray(m_szSpreadLink));
	}

	//��������
	ReadFileString(TEXT("PLATFORMLINK"),TEXT("UpdateLink"),m_szUpdateLink,MAX_PATH);	
	if(m_szUpdateLink[0]==0)
	{
		lstrcpyn(m_szUpdateLink,m_szPlatformLink,CountArray(m_szUpdateLink));
	}
	
	return;
}

//////////////////////////////////////////////////////////////////////////////////
