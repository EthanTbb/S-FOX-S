#include "Stdafx.h"
#include "NetworkManager.h"


//�������

//���캯��
CNetworkManager::CNetworkManager()
{

}

//��������
CNetworkManager::~CNetworkManager()
{

}

//�����ӿ�
//�ӿڲ�ѯ
VOID *  CNetworkManager::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IWHNetwork, Guid, dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IWHNetwork, Guid, dwQueryVer);
	return NULL;
}

DWORD CNetworkManager::ConversionAddr(LPCTSTR pszAddress)
{
	return 0;
}
//��ȡ��ַ
DWORD CNetworkManager::GetLocalNetworkIP(DWORD dwAddrList[], DWORD dwMaxCount)
{
	return 0;
}
//���ܲ�ѯ
//ö�ٴ���
bool  CNetworkManager::EnumProxyServerType(WORD wIndex, tagProxyDescribe & ProxyTypeDescribe)
{
	return false;
}

//�������
BYTE  CNetworkManager::ProxyServerTesting(BYTE ProxyServerType, const tagProxyServer & ProxyServerInfo)
{
	return false;
}

//////////////////////////////////////////////////////////////////////////

////����������
//extern "C" __declspec(dllexport) void *  CreateNetworkManager(const GUID & Guid, DWORD dwInterfaceVer)
//{
//	//��������
//	CNetworkManager * pNetworkManager = NULL;
//	try
//	{
//		pNetworkManager = new CNetworkManager();
//		if (pNetworkManager == NULL) throw TEXT("����ʧ��");
//		void * pObject = pNetworkManager->QueryInterface(Guid, dwInterfaceVer);
//		if (pObject == NULL) throw TEXT("�ӿڲ�ѯʧ��");
//		return pObject;
//	}
//	catch (...) {}
//
//	//�������
//	SafeDelete(pNetworkManager);
//	return NULL;
//}
DECLARE_CREATE_MODULE(NetworkManager)
//////////////////////////////////////////////////////////////////////////
