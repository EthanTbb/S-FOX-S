#ifndef PLAZA_VIEW_SERVER_CTRL_HEAD_FILE
#define PLAZA_VIEW_SERVER_CTRL_HEAD_FILE

#pragma once
#include "PlatformPublicize.h"
#include "DlgTrumpetItem.h"

//////////////////////////////////////////////////////////////////////////////////
//房间控制
class CPlazaViewServerCtrl : public CFGuiDialog,public IGamePropertySink,public IGamePropertyUseSink,public IChatControlSink,public IExpressionSink 
{
	//界面变量
protected:
	bool							m_bCreateFlag;						//创建标志

	//房间属性
protected:
	WORD							m_wChairCount;						//椅子数目
	WORD							m_wServerType;						//房间类型
	DWORD							m_dwServerRule;						//房间规则

	//用户属性
protected:
	DWORD							m_dwUserRight;						//用户权限
	DWORD							m_dwMasterRight;					//管理权限

	//聊天变量
private:
	DWORD							m_dwChatTime;						//聊天时间
	CString							m_strChatString;					//发送信息

	//控件变量
private:
	CRichEditMessage				m_ChatMessage;						//聊天信息

	//控件变量
protected:
	CWndChatControl					m_ChatControl;						//聊天控制
	CDlgTrumpet *               m_pTrumpetItem;                     //喇叭窗口 
	CExpressionControl *			m_pExpressionControl;				//表情窗口
	//喇叭数量
public:
	DWORD							m_dwTrumpetCount;					//小喇叭数
	DWORD							m_dwTyphonCount;					//大喇叭数	

	//数组变量
protected:
	CClientUserItemArray			m_MenuUserItemArray;				//用户数组

	//控件变量
protected:
	CToolTipCtrl					m_ToolTipCtrl;						//提示控件
	CUserListControl				m_UserListControl;					//用户列表
	CPlatformPublicize				m_BillPublicize;					//浏览控件

	//接口变量
protected:
	ITCPSocket *					m_pITCPSocket;						//TCP连接
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
	IClientUserItem *				m_pISelectedUserItem;				//选择指针	
	IGameLevelParser *				m_pIGameLevelParser;				//等级接口
	IPlazaUserManager *				m_pIPlazaUserManager;				//用户管理

	//资源变量
protected:
	CEncircleBMP					m_ChatEncircle;						//聊天资源
	CEncircleBMP					m_UserListEncircle;					//列表资源

	//函数定义
public:
	//构造函数
	CPlazaViewServerCtrl();
	//析构函数
	virtual ~CPlazaViewServerCtrl();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { delete this; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//重载函数
public:	
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
	virtual VOID Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("ServerCtrl.xml"); } 

	//绘画接口
public:
	//开始绘画
	virtual void OnBeginPaintWindow(HDC hDC);
	
	//函数定义
public:
	//进入事件
	VOID SetServerInfo(WORD wChairCount,WORD wServerType,DWORD dwServerRule,DWORD dwUserRight,DWORD dwMasterRight);

	//重载函数
protected:	
	//配置函数
	virtual BOOL OnInitDialog();
	//消息过虑
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//接口函数
public:
	//使用道具
	virtual bool OnEventUseProperty(WORD wPropertyIndex);
	virtual bool PerformShowBag();
	virtual bool PerformShowShop();
	//列表配置
	virtual VOID SetColumnDescribe(tagColumnItem ColumnItem[], BYTE cbColumnCount);	
	
	//喇叭数量
public:
	int  GetTrumpetCount();
	VOID SetTrumpetCount(const DWORD dwCount);
	int  GetTyphonCount();
	VOID SetTyphonCount(const DWORD dwCount);

	//用户菜单
public:
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem);
	//显示菜单
	virtual VOID TrackUserItemMenu(IClientUserItem * pIClientUserItem[], WORD wItemCount);

	//控件接口
public:
	//消息接口
	IStringMessage * GetStringMessage() { return QUERY_OBJECT_INTERFACE(m_ChatMessage,IStringMessage);}

	//用户事件
public:
	//用户进入
	virtual VOID OnEventUserEnter(IClientUserItem * pIClientUserItem);
	//用户离开
	virtual VOID OnEventUserLeave(IClientUserItem * pIClientUserItem);
	//用户状态
	virtual VOID OnEventUserUpdate(IClientUserItem * pIClientUserItem);

	//道具接口
protected:
	//购前检查
	virtual bool OnEventBuyPropertyPrep(LPCTSTR pszNickName,WORD wPropertyIndex,LPTSTR pszMessage);
	//购买道具
	virtual bool OnEventBuyProperty(LPCTSTR pszNickName, WORD wItemCount, WORD wPropertyIndex);
	//查询用户
	virtual IClientUserItem * OnQueryClientUserItem() { return m_pIMySelfUserItem; };

	//控件事件
protected:
	//用户选择
	virtual VOID OnChangeChatTarget(IClientUserItem * pIClientUserItem);
	//表情事件
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	//道具事件
public:
	//喇叭事件
	VOID OnEventPropertyTrumpet();

	//界面函数
private:
	//设置列表
	VOID InitSilderControl();
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);	

	//内部函数
protected:
	//聊天效验
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//发送聊天
	bool SendUserChatPacket(DWORD dwTargetUserID, LPCTSTR pszChatString, COLORREF crColor);
	//发送表情
	bool SendExpressionPacket(DWORD dwTargetUserID, WORD wItemIndex);

	//管理命令
protected:
	//禁止用户大厅聊天
	VOID OnLimitUserRoomChat();
	//允许用户大厅聊天
	VOID OnAllowUserRoomChat();
	//禁止用户游戏聊天
	VOID OnLimitUserGameChat();
	//允许用户游戏聊天
	VOID OnAllowUserGameChat();
	//禁止用户私聊
	VOID OnLimitUserWhispChat();
	//允许用户私聊
	VOID OnAllowUserWhispChat();

	//聊天命令
protected:
	//更多颜色
	VOID OnSelectMoreColor();
	//选择颜色
	VOID OnSelectChatColor(UINT nCommandID);
	//选择短语
	VOID OnSelectChatShort(UINT nCommandID);

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//右键列表
	VOID OnNMRclickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//单击列表
	VOID OnNMClickUserList(NMHDR * pNMHDR, LRESULT * pResult);
	//双击列表
	VOID OnNMDblclkUserList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif