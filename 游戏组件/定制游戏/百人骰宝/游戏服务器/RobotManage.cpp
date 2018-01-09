#include "Stdafx.h"
#include ".\RobotManage.h"

//构造函数
CRobotConfigManage::CRobotConfigManage()
{
	//变量初始化
	m_VectorConfig.clear();
	m_MapRobotType.clear();
}

//析构函数
CRobotConfigManage::~CRobotConfigManage()
{

}

//读取配置
VOID CRobotConfigManage::ReadRobotConfig(LPCTSTR szFileName, LPCTSTR szRoomName)
{
	//循环读取规则
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
		//规则名称
		strRuleName.Format(_T("RobotRule%d"), nCount);

		//读取配置
		ZeroMemory(OutBuf, sizeof(OutBuf));
		GetPrivateProfileString(szRoomName, strRuleName, _T(""), OutBuf, sizeof(OutBuf), szConfigFileName);
		strRule.Format((_T("%s"), OutBuf));

		//检查内容
		tagRobotConfig robotConfig = {};
		if (0 != StringToRule(strRule, &robotConfig))
			break;

		//添加规则
		m_VectorConfig.push_back(robotConfig);

		nCount++;
	}

	//默认配置
	if (nCount == 0)
	{
		tagRobotConfig defaultConfig = {10000, 1000000, 2000000, 100000000, 90000000, 3, 50000000, 10000000, 100, 1000, 3, 5};

		//添加规则
		m_VectorConfig.push_back(defaultConfig);
	}
}

//动态管理
/************************************************************************************** 
 * 1 检查是否有新机器人
 * 2 检查是否各配置已经达到最低要求人数,没有则强制补满，满则随机添加
 * 3 根据2的结果选择分配新机器人的方式
 **************************************************************************************/
bool CRobotConfigManage::RobotDynamicManage(CString strRobotName[], BYTE bRobotCount)
{
	//变量定义
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

	//类型整理
	for (int i = 0; i < bRobotCount; i++)
	{
		iter = m_MapRobotType.find(strRobotName[i]);
		
		//是否新机器人
		if (iter == m_MapRobotType.end())
		{
			vectNewRobot.push_back(strRobotName[i]);
		}
		else
		{
			vectOldRobot.push_back(strRobotName[i]);

			//非法类型
			int nRobotType = iter->second;
			if (nRobotType < 0 || nRobotType >= nCfgCount)
			{
				//转化为新机器人
				vectOldRobot.pop_back();
				vectNewRobot.push_back(strRobotName[i]);
			}
			else
			{
				//统计数目
				vectRobotCount[nRobotType]++;
			}
		}
	}

	//管理映射 (清除已经消失的机器人)
	map<CString, int> mapTmp;
	for (int i = 0; i < int(vectOldRobot.size()); i++)
	{
		iter = m_MapRobotType.find(vectOldRobot[i]);
		if (iter != m_MapRobotType.end())
			mapTmp.insert(map<CString, int>::value_type(iter->first, iter->second));
	}
	m_MapRobotType = mapTmp;
	
	//是否有新机器人
	if (vectNewRobot.size() == 0)
		return false;

	//分配新机器人
	for (int i = 0; i < int(vectNewRobot.size()); i++)
	{
		//是否需要强制补满 (即总机器人人数未超过各种配置配置人数之和)
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

		//非强制补满
		if (!bForceManage)
		{
			for (int j = 0; j < nCfgCount; j++)
				vectRobotNeed[j] =  m_VectorConfig[j].nCfgRobotCount;
		}

		//剩余总数
		int nRNTotal = 0;
		for (int j = 0; j < nCfgCount; j++)
			nRNTotal += vectRobotNeed[j];

		//随机规则
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
		
		//添加机器人和规则映射
		m_MapRobotType.insert(map<CString, int>::value_type(vectNewRobot[i], nRobotType));
		vectRobotCount[nRobotType]++;
	}

	return true;
}

//获取类型
int CRobotConfigManage::GetRobotConfigType(LPCTSTR szRobotName, tagRobotConfig * pRobotConfig/* = NULL*/)
{
	//机器账号
	CString strRobotName;
	strRobotName.Format(_T("%s"), szRobotName);

	//寻找类型
	map<CString, int>::iterator iter = m_MapRobotType.find(strRobotName);
	if (iter == m_MapRobotType.end())
		return -1;

	//检查类型
	int nCfgCount = int(m_VectorConfig.size());
	ASSERT(nCfgCount>0);
	if (iter->second < 0 || iter->second >= nCfgCount)
		return -1;

	//填充配置
	if (pRobotConfig != NULL)
		*pRobotConfig = m_VectorConfig[iter->second];
	
	return int(iter->second);
}

//字段转换
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

//读取长整
SCORE CRobotConfigManage::GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName)
{
	//变量定义
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

//解析字段
CString CRobotConfigManage::GetField(CString & strSrc, CString strSep/* = _T("~")*/)
{
	//变量定义
	CString strResult;
	int nPos = 0;

	//寻找分隔符
	nPos = strSrc.Find(strSep);

	//字符串处理
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

//解析字段
int CRobotConfigManage::GetFieldToINT(CString & strSrc, CString strSep/* = _T("~")*/)
{
	//变量定义
	CString strResult;
	int nNum = 0;

	//解析字段
	strResult = GetField(strSrc, strSep);

	myscanf(strResult, strResult.GetLength(), _T("%d"), &nNum);

	return nNum;
}

//解析字段
SCORE CRobotConfigManage::GetFieldToINT64(CString & strSrc, CString strSep/* = _T("~")*/)
{
	//变量定义
	CString strResult;
	SCORE lNum = 0;

	//解析字段
	strResult = GetField(strSrc, strSep);

	myscanf(strResult, strResult.GetLength(), _T("%I64d"), &lNum);

	return lNum;
}
