#ifndef PLAZA_USER_INFO_HEAD_FILE
#define PLAZA_USER_INFO_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
#define IDI_CHECK_USER_BUFF			11				//检查玩家Buff时间
#define IDI_CHECK_SEND_PRESENT		12				//检测玩家有没有被赠送道具

//类型视图
class CPlazaUserInfo : public CMissionItem
{
	//函数定义
public:
	//构造函数
	CPlazaUserInfo(CPaintManagerUI*	pPaintManager);
	//析构函数
	virtual ~CPlazaUserInfo();
	//清除状态
	void ClearBuff();
	//重载函数
public:
	//连接事件
	virtual bool OnEventMissionLink(INT nErrorCode);
	//关闭事件
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//读取事件
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//FGUI函数
public:
	//消息提醒
	bool Notify(TNotifyUI &  msg);
	//初始控件
	bool InitControlUI();
	//判断消息
	bool IsNotify(TNotifyUI &  msg);
	//获取控件
	CControlUI * GetControlByName(LPCTSTR pszControlName);
	
	//事件消息
public:
	//事件消息
	bool OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	VOID OnTimer(UINT_PTR nIDEvent);
	//用户函数
public:
	//激活任务
	VOID AvtiveMission();

	//刷新界面
	VOID UpdateUI();

	//刷新等级
	VOID UpdateLevelUI();

	//更新Buff数据
	VOID UpdateBuffData(const tagPropertyBuff& Buff);

	//查询等级
	VOID QueryLevel();

	//查询玩家道具状态
	VOID QueryPropBuff();

	//查询实名
	VOID QueryIndividual();

	//查询道具赠送
	VOID QuerySendPresent();

	//激活查询赠送任务
	VOID AvtiveQuerySendPresentMission();

	//刷新等级
	VOID UpdateLevel();

	//刷新BUFF
	VOID UpdateBuffUI();
	bool VisibleBuff(const tagPropertyBuff& buff, CControlUI* pControl, LPCTSTR lpImage);
	
	//刷新头像
	VOID UpdateUserFace(const WORD wFaceID);
	VOID UpdateUserFace(DWORD dwCustomFace[48*48]);

	//获取头像区域
	RECT GetUserFacePos();

	//设置奖励
	void SetRealTipAward();
	
	//显示实名
	void ShowRealTip();

	//取消实名
	void CancleRealTip();

	//隐藏实名
	void HideRealTip();

	//存储变量
protected:
	tagGrowLevelParameter			m_GrowLevelParameter;				//成长参数
	CMissionManager					m_MissionManager;					//任务管理
	CPaintManagerUI*				m_pPaintManager;
	CImageUI*						m_pPlazaHallAvtatr;					//头像
	CControlUI*						m_pUserDoubleBuff;					//双倍
	CControlUI*						m_pUserAmuletBuff;					//防身
	CControlUI*						m_pUserGuardKickBuff;				//防踢
	WORD							m_wFaceID;							
	bool							m_bQueryLevel;						//查询等级
	bool							m_bQueryPropBuff;					//道具BUff
	bool                            m_bQueryIndividual;                 //查询资料
	bool							m_bQuerySendPresent;				//查询赠送
	vector<tagPropertyBuff>			m_vecPropertyBuff;					//道具BUff
	CStdString						m_sGuardKickBuffImage;
	CStdString						m_sAmuletBuffImage;
	CStdString						m_sDoubleBuffImage;
	int								m_nRealNum;							//实名次数
};

//////////////////////////////////////////////////////////////////////////////////

#endif