#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//系统文件
#include "Stdafx.h"
#include "Resource.h"

//任务文件
#include "MissionList.h"
#include "MissionLogon.h"
#include "MissionAndroid.h"

//
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//平台框架
class CPlatformFrame : public CSkinDialog,public IServerListDataSink,IAndroidOperateCenter
{
	//变量定义
protected:
	bool							m_bLogonSuccess;					//登录成功
	int								m_nSelectItem;						//选择索引

	//资源变量
protected:
	CBrush							m_brBackGround;						//背景画刷

	//游戏列表
protected:
	CServerListData					m_ServerListData;					//游戏数据

	//控件变量
protected:
	CSkinComboBox					m_cbGameList;						//游戏列表
	CSkinComboBox					m_cbServerList;						//房间列表
	CSkinComboBox					m_cbEnterHour;						//进入时间
	CSkinComboBox					m_cbEnterMinute;					//进入时间
	CSkinComboBox					m_cbEnterSecond;					//进入时间
	CSkinComboBox					m_cbLeaveHour;						//离开时间
	CSkinComboBox					m_cbLeaveMinute;					//离开时间
	CSkinComboBox					m_cbLeaveSecond;					//离开时间

	//控件变量
protected:
	CSkinEdit						m_edtAndroidCount;					//机器数目
	CSkinEdit						m_edtTableScoreMin;					//携带分数
	CSkinEdit						m_edtTableScoreMax;					//携带分数
	CSkinEdit						m_edtEnterIntervalMin;				//进入间隔
	CSkinEdit						m_edtEnterIntervalMax;				//进入间隔
	CSkinEdit						m_edtLeaveIntervalMin;				//离开间隔
	CSkinEdit						m_edtLeaveIntervalMax;				//离开间隔
	CSkinEdit						m_edtSwitchTableMin;				//换桌局数
	CSkinEdit						m_edtSwitchTableMax;				//换桌局数
	CSkinEdit						m_edtCountMember0;					//普通会员
	CSkinEdit						m_edtCountMember1;					//一级会员
	CSkinEdit						m_edtCountMember2;					//二级会员
	CSkinEdit						m_edtCountMember3;					//三级会员
	CSkinEdit						m_edtCountMember4;					//四级会员
	CSkinEdit						m_edtCountMember5;					//五级会员
	//按钮控件
protected:
	CSkinButton						m_btQuery;							//查询按钮
	CSkinButton						m_btAddItem;						//添加子项
	CSkinButton						m_btModifyItem;						//修改子项
	CSkinButton						m_btDeleteItem;						//删除子项

	//控件变量
protected:
	CSkinListCtrl					m_ParemeterList;					//参数列表
	CSystemTrayIcon					m_SystemTrayIcon;					//任务图标

	//任务组件
protected:
	CMissionList					m_MissionList;						//列表任务
	CMissionLogon					m_MissionLogon;						//登录任务
	CMissionAndroid					m_MissionAndroid;					//机器任务
	CMissionManager					m_MissionManager;					//任务管理

	//静态变量
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//框架指针

	//函数定义
public:
	//构造函数
	CPlatformFrame();
	//析构函数
	virtual ~CPlatformFrame();

	//重载函数
protected:
	//初始窗口
	virtual BOOL OnInitDialog();
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//交换数据
	virtual VOID DoDataExchange(CDataExchange* pDX);

	//状态通知
public:
	//完成通知
	virtual VOID OnGameItemFinish();
	//完成通知
	virtual VOID OnGameKindFinish(WORD wKindID);
	//更新通知
	virtual VOID OnGameItemUpdateFinish();

	//更新通知
public:
	//插入通知
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//更新通知
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//删除通知
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//机器接口
public:
	//控件使能
	virtual VOID OnEventEnableControls(BOOL bEnabled);
	//机器参数
	virtual VOID OnEventAndroidParenter(WORD wSubCommdID,WORD wParameterCount,tagAndroidParameter * pAndroidParameter);

	//辅助函数
private:
	//初始化控件
	VOID InitControls();
	//添加修改
	VOID AddModifyParameter(bool bModify);
	//选中子项
	VOID SelectItem(int nIndex);
	//设置子项
	VOID SetItemToParameterList(int nItemIndex,tagAndroidParameter * pAndroidParameter);
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);

	//消息函数
protected:
	//关闭消息
	VOID OnClose();	
	//销毁窗口
	VOID OnDestroy();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//时间消息
	VOID OnTimer(UINT_PTR nIDEvent);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//位置消息
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//鼠标消息
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//改变消息
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//选择改变
	VOID OnCbnComboGameSelchange();
	//单击子项
	VOID OnHdnListParameterItemclick(NMHDR *pNMHDR, LRESULT *pResult);
	//删除子项
	VOID OnLvnListParameterDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	//键盘按下
	VOID OnLvnKeydownListParameter(NMHDR *pNMHDR, LRESULT *pResult);

	//自定消息
protected:
	//事件消息
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	//事件函数
protected:
	//添加子项
	VOID OnBnClickedBtAdd();
	//编辑子项
	VOID OnBnClickedBtModify();
	//删除子项
	VOID OnBnClickedBtDelete();
	//查询参数
	VOID OnBnClickedBtQuery();

	//功能函数
public:
	//获取实例
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()			
};

//////////////////////////////////////////////////////////////////////////////////

#endif