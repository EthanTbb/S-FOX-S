#ifndef ROBOT_MANAGE_HEAD_FILE
#define ROBOT_MANAGE_HEAD_FILE

//#pragma once

#include "Stdafx.h"
#include <vector>
#include <map>
#include "GameLogic.h"
using std::vector;
using std::map;

//机器人管理
class CRobotConfigManage
{
	//变量定义
protected:
public:
	vector<tagRobotConfig>			m_VectorConfig;						//配置列表
	map<CString, int>				m_MapRobotType;						//机器人类型

	//函数定义
public:
	//构造函数
	CRobotConfigManage();
	//析构函数
	~CRobotConfigManage();
	//读取配置
	VOID ReadRobotConfig(LPCTSTR szFileName, LPCTSTR szRoomName);
	//动态管理
	bool RobotDynamicManage(CString strRobotName[], BYTE bRobotCount);
	//获取信息
	map<CString, int> * GetRobotTypeInfo() { return &m_MapRobotType; }
	//获取类型
	int GetRobotConfigType(LPCTSTR szRobotName, tagRobotConfig * pRobotConfig = NULL);

	//辅助函数
protected:
	//字段转换
	HRESULT StringToRule(CString strRule, tagRobotConfig * pRobotConfig);
	//读取长整
	SCORE GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName);
	//解析字段
	CString GetField(CString & strSrc, CString strSep = _T("~"));
	//解析字段
	int GetFieldToINT(CString & strSrc, CString strSep = _T("~"));
	//解析字段 
	SCORE GetFieldToINT64(CString & strSrc, CString strSep = _T("~"));
};

#endif