#include "Stdafx.h"
#include ".\RobotManage.h"

//���캯��
CRobotConfigManage::CRobotConfigManage()
{
	//������ʼ��
	m_VectorConfig.clear();
	m_MapRobotType.clear();
}

//��������
CRobotConfigManage::~CRobotConfigManage()
{

}

//��ȡ����
VOID CRobotConfigManage::ReadRobotConfig(LPCTSTR szFileName, LPCTSTR szRoomName)
{
	//ѭ����ȡ����
	CString strRuleName, strRule;
	int nCount = 0;
	TCHAR szPath[MAX_PATH] = TEXT("");
	TCHAR szConfigFileName[MAX_PATH] = TEXT("");
	TCHAR OutBuf[255] = TEXT("");
	GetCurrentDirectory(sizeof(szPath), szPath);
	//myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s\\%s"), szPath, szFileName);
	myprintf(szConfigFileName, sizeof(szConfigFileName), _T("%s"), szFileName);

	while (nCount <= 50)		
	{
		//��������
		strRuleName.Format(_T("RobotRule%d"), nCount);

		//��ȡ����
		ZeroMemory(OutBuf, sizeof(OutBuf));
		GetPrivateProfileString(szRoomName, strRuleName, _T(""), OutBuf, sizeof(OutBuf), szConfigFileName);
		strRule.Format((_T("%s"), OutBuf));

		//�������
		tagRobotConfig robotConfig = {};
		if (0 != StringToRule(strRule, &robotConfig))
			break;

		//��ӹ���
		m_VectorConfig.push_back(robotConfig);

		nCount++;
	}

	//Ĭ������
	if (nCount == 0)
	{
		tagRobotConfig defaultConfig = {10000, 1000000, 2000000, 100000000, 90000000, 3, 50000000, 10000000, 100, 1000, 3, 5};

		//��ӹ���
		m_VectorConfig.push_back(defaultConfig);
	}
}

//��̬����
/************************************************************************************** 
 * 1 ����Ƿ����»�����
 * 2 ����Ƿ�������Ѿ��ﵽ���Ҫ������,û����ǿ�Ʋ���������������
 * 3 ����2�Ľ��ѡ������»����˵ķ�ʽ
 **************************************************************************************/
bool CRobotConfigManage::RobotDynamicManage(CString strRobotName[], BYTE bRobotCount)
{
	//��������
	int nRandNum = 0, nCfgCount = int(m_VectorConfig.size());
	vector<CString> vectNewRobot, vectOldRobot;
	vector<int> vectRobotCount(nCfgCount), vectRobotNeed(nCfgCount);
	map<CString, int>::iterator iter;

	//JJTest
	if (nCfgCount == 0)
	{
		ASSERT(0);
		return false;
	}

	//��������
	for (int i = 0; i < bRobotCount; i++)
	{
		iter = m_MapRobotType.find(strRobotName[i]);
		
		//�Ƿ��»�����
		if (iter == m_MapRobotType.end())
		{
			vectNewRobot.push_back(strRobotName[i]);
		}
		else
		{
			vectOldRobot.push_back(strRobotName[i]);

			//�Ƿ�����
			int nRobotType = iter->second;
			if (nRobotType < 0 || nRobotType >= nCfgCount)
			{
				//ת��Ϊ�»�����
				vectOldRobot.pop_back();
				vectNewRobot.push_back(strRobotName[i]);
			}
			else
			{
				//ͳ����Ŀ
				vectRobotCount[nRobotType]++;
			}
		}
	}

	//����ӳ�� (����Ѿ���ʧ�Ļ�����)
	map<CString, int> mapTmp;
	for (int i = 0; i < int(vectOldRobot.size()); i++)
	{
		iter = m_MapRobotType.find(vectOldRobot[i]);
		if (iter != m_MapRobotType.end())
			mapTmp.insert(map<CString, int>::value_type(iter->first, iter->second));
	}
	m_MapRobotType = mapTmp;
	
	//�Ƿ����»�����
	if (vectNewRobot.size() == 0)
		return false;

	//�����»�����
	for (int i = 0; i < int(vectNewRobot.size()); i++)
	{
		//�Ƿ���Ҫǿ�Ʋ��� (���ܻ���������δ��������������������֮��)
		bool bForceManage = false;
		int nRobotType = 255;
		for (int k = 0; k < nCfgCount; k++)
			vectRobotNeed[k] = 0;

		for (int j = 0; j < nCfgCount; j++)
		{
			if (vectRobotCount[j] >= m_VectorConfig[j].nCfgRobotCount)
				vectRobotNeed[j] = 0;
			else
			{
				bForceManage = true;
				vectRobotNeed[j] = m_VectorConfig[j].nCfgRobotCount - vectRobotCount[j];
			}
		}

		//��ǿ�Ʋ���
		if (!bForceManage)
		{
			for (int j = 0; j < nCfgCount; j++)
				vectRobotNeed[j] =  m_VectorConfig[j].nCfgRobotCount;
		}

		//ʣ������
		int nRNTotal = 0;
		for (int j = 0; j < nCfgCount; j++)
			nRNTotal += vectRobotNeed[j];

		//�������
		ASSERT(nRNTotal!=0);
		if (nRNTotal == 0)	continue;
		nRandNum = rand() % nRNTotal;
		for (int j = 0; j < nCfgCount; j++)
		{
			nRandNum -= vectRobotNeed[j];
			if (nRandNum < 0)
			{
				nRobotType = j;
				break;
			}
		}
		
		//��ӻ����˺͹���ӳ��
		m_MapRobotType.insert(map<CString, int>::value_type(vectNewRobot[i], nRobotType));
		vectRobotCount[nRobotType]++;
	}

	return true;
}

//��ȡ����
int CRobotConfigManage::GetRobotConfigType(LPCTSTR szRobotName, tagRobotConfig * pRobotConfig/* = NULL*/)
{
	//�����˺�
	CString strRobotName;
	strRobotName.Format(_T("%s"), szRobotName);

	//Ѱ������
	map<CString, int>::iterator iter = m_MapRobotType.find(strRobotName);
	if (iter == m_MapRobotType.end())
		return -1;

	//�������
	int nCfgCount = int(m_VectorConfig.size());
	ASSERT(nCfgCount>0);
	if (iter->second < 0 || iter->second >= nCfgCount)
		return -1;

	//�������
	if (pRobotConfig != NULL)
		*pRobotConfig = m_VectorConfig[iter->second];
	
	return int(iter->second);
}

//�ֶ�ת��
HRESULT CRobotConfigManage::StringToRule(CString strRule, tagRobotConfig * pRobotConfig)
{
	if (strRule == _T(""))
		return -1;

	pRobotConfig->lBankerCondition		= GetFieldToINT64(strRule);

	pRobotConfig->lBankDrawCondition	= GetFieldToINT64(strRule);
	pRobotConfig->lBankDrawScore		= GetFieldToINT64(strRule);
	pRobotConfig->lBankSaveCondition    = GetFieldToINT64(strRule);
	pRobotConfig->lBankSaveScore		= GetFieldToINT64(strRule);

	pRobotConfig->lBankerDrawCount		= GetFieldToINT64(strRule);
	pRobotConfig->lBankerWinGiveUp		= GetFieldToINT64(strRule);
	pRobotConfig->lBankerLoseGiveUp		= GetFieldToINT64(strRule);

	pRobotConfig->lJettonLimit[0]		= GetFieldToINT64(strRule);
	pRobotConfig->lJettonLimit[1]		= GetFieldToINT64(strRule);
	pRobotConfig->lBetTimeLimit  		= GetFieldToINT64(strRule);

	pRobotConfig->nCfgRobotCount		= GetFieldToINT(strRule);

	return 0;
}

//��ȡ����
SCORE CRobotConfigManage::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName)
{
	//��������
	TCHAR OutBuf[255] = _T("");
	TCHAR DefaultBuf[255] = {};
	SCORE lNumber = 0;

	GetPrivateProfileString(lpAppName, lpKeyName, DefaultBuf, OutBuf, 255, lpFileName);

	if (OutBuf[0] != 0)
		myscanf(OutBuf, mystrlen(OutBuf), _T("%I64d"), &lNumber);
	else
		lNumber = lDefault;

	return lNumber;
}

//�����ֶ�
CString CRobotConfigManage::GetField(CString & strSrc, CString strSep/* = _T("~")*/)
{
	//��������
	CString strResult;
	int nPos = 0;

	//Ѱ�ҷָ���
	nPos = strSrc.Find(strSep);

	//�ַ�������
	if (nPos == -1)
	{
		strResult = strSrc;
		strSrc.Empty();
	}
	else
	{
		strResult = strSrc.Left(nPos);
		strSrc.Delete(0, nPos+1);
	}

	return strResult;
}

//�����ֶ�
int CRobotConfigManage::GetFieldToINT(CString & strSrc, CString strSep/* = _T("~")*/)
{
	//��������
	CString strResult;
	int nNum = 0;

	//�����ֶ�
	strResult = GetField(strSrc, strSep);

	myscanf(strResult, strResult.GetLength(), _T("%d"), &nNum);

	return nNum;
}

//�����ֶ�
SCORE CRobotConfigManage::GetFieldToINT64(CString & strSrc, CString strSep/* = _T("~")*/)
{
	//��������
	CString strResult;
	SCORE lNum = 0;

	//�����ֶ�
	strResult = GetField(strSrc, strSep);

	myscanf(strResult, strResult.GetLength(), _T("%I64d"), &lNum);

	return lNum;
}
