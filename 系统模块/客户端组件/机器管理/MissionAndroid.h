#ifndef MISSION_ANDROID_HEAD_FILE
#define MISSION_ANDROID_HEAD_FILE
#pragma once

#include "StdAfx.h"

//////////////////////////////////////////////////////////////////////////////////
//动作枚举
enum emAndroidAction
{
	AAction_NULL=0,
	AAction_GetParameter,			//获取参数
	AAction_AddParameter,			//添加参数
	AAction_ModifyParameter,		//修改参数
	AAction_DeleteParameter			//删除参数
};

//////////////////////////////////////////////////////////////////////////////////
//接口定义
interface IAndroidOperateCenter
{
	//控件使能
	virtual VOID OnEventEnableControls(BOOL bEnabled)=NULL;
	//机器参数
	virtual VOID OnEventAndroidParenter(WORD wSubCommdID,WORD wParameterCount,tagAndroidParameter * pAndroidParameter)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//机器任务
class CMissionAndroid : public CWnd, public CMissionItem
{
	//存储变量
protected:
	WORD								m_wDataSize;							//数据大小
	BYTE								m_cbDataBuffer[SOCKET_TCP_PACKET];		//数据缓冲

	//变量定义
protected:
	emAndroidAction						m_AndroidAction;						//机器动作	

	//接口变量
protected:
	IAndroidOperateCenter *				m_pIAndroidOperateCenter;				//机器接口

	//静态变量
protected:
	static CMissionAndroid *			m_pMissionAndroid;						//对象指针

	//函数定义
public:
	//构造函数
	CMissionAndroid();
	//析构函数
	virtual ~CMissionAndroid();

	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//功能函数
public:
	//设置接口
	VOID SetAndroidOperateCenter(IAndroidOperateCenter * pIAndroidOperateCenter);
	//获取参数
	VOID GetAndroidParameter(WORD wServerID);
	//删除参数
	VOID DeleteAndroidParameter(WORD wServerID,DWORD dwBatchID);
	//修改参数
	VOID ModifyAndroidParameter(WORD wServerID,tagAndroidParameter *  pAndroidParameter);
	//添加参数
	VOID AddAndroidParameter(WORD wServerID,tagAndroidParameter *  pAndroidParameter);	

	//静态函数
public:
	//获取对象
	static CMissionAndroid * GetInstance() { return m_pMissionAndroid; }

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif