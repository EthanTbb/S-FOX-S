#ifndef ROBOT_MANAGE_HEAD_FILE
#define ROBOT_MANAGE_HEAD_FILE

//#pragma once

#include "Stdafx.h"
#include <vector>
#include <map>
#include "GameLogic.h"
using std::vector;
using std::map;

//�����˹���
class CRobotConfigManage
{
	//��������
protected:
public:
	vector<tagRobotConfig>			m_VectorConfig;						//�����б�
	map<CString, int>				m_MapRobotType;						//����������

	//��������
public:
	//���캯��
	CRobotConfigManage();
	//��������
	~CRobotConfigManage();
	//��ȡ����
	VOID ReadRobotConfig(LPCTSTR szFileName, LPCTSTR szRoomName);
	//��̬����
	bool RobotDynamicManage(CString strRobotName[], BYTE bRobotCount);
	//��ȡ��Ϣ
	map<CString, int> * GetRobotTypeInfo() { return &m_MapRobotType; }
	//��ȡ����
	int GetRobotConfigType(LPCTSTR szRobotName, tagRobotConfig * pRobotConfig = NULL);

	//��������
protected:
	//�ֶ�ת��
	HRESULT StringToRule(CString strRule, tagRobotConfig * pRobotConfig);
	//��ȡ����
	SCORE GetPrivateProfileInt64(LPCTSTR lpAppName, LPCTSTR lpKeyName, SCORE lDefault, LPCTSTR lpFileName);
	//�����ֶ�
	CString GetField(CString & strSrc, CString strSep = _T("~"));
	//�����ֶ�
	int GetFieldToINT(CString & strSrc, CString strSep = _T("~"));
	//�����ֶ� 
	SCORE GetFieldToINT64(CString & strSrc, CString strSep = _T("~"));
};

#endif