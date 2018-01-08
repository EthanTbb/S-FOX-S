#ifndef DLG_WHISPER_HEAD_FILE
#define DLG_WHISPER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CDlgWhisper;
class CPlazaViewServer;

//数组定义
typedef CWHArray<CDlgWhisper *> CDlgWhisperItemArray;					//私聊数组
typedef CWHArray<IClientUserItem *> CClientUserItemArray;				//用户数组

//////////////////////////////////////////////////////////////////////////////////

//配置结构
struct tagWhisper
{
	//数据变量
	DWORD							dwServerRule;						//房间规则
	TCHAR							szServerName[LEN_SERVER];			//房间名字

	//接口变量
	ITCPSocket *					pITCPSocket;						//网络组件
	IClientUserItem *				pIMySelfUserItem;					//自己指针
	IUserOrderParser *				pIUserOrderParser;					//等级接口
	IGameLevelParser *				pIGameLevelParser;					//等级接口
};

//////////////////////////////////////////////////////////////////////////////////

//私聊窗口
class CDlgWhisper : public CFGuiDialog, public IExpressionSink
{
	//状态变量
protected:
	bool							m_bCreateFlag;						//创建标志
	bool							m_bShowHistory;						//聊天记录标志
	bool							m_bLableRound;

	//数据变量
protected:
	DWORD							m_dwUserRight;						//用户权限
	DWORD							m_dwMasterRight;					//管理权限

	//接口变量
protected:
	ITCPSocket *					m_pITCPSocket;						//网络组件
	IClientUserItem *				m_pIMySelfUserItem;					//自己指针
	IUserOrderParser *				m_pIUserOrderParser;				//等级组件
	IGameLevelParser *				m_pIGameLevelParser;				//等级接口
	IPlazaUserManager *				m_pIPlazaUserManager;				//用户接口

	//变量定义
protected:
	DWORD							m_dwSendUserID;						//用户标识
	DWORD							m_dwConversationID;					//会话标识
	DWORD							m_dwChatHistroyID;					//记录标识
	CClientUserItemArray			m_ClientUserItemArray;				//用户数组

	//界面控件
protected:
	CLabelUI *						m_pLabelToolBar;					//工具面板
	CLabelUI *						m_pLabelRound;						//背景环绕
	CLabelUI *						m_pLabelChatArea;					//聊天环绕
	CLabelUI *						m_pLabelUserInfoArea;				//信息环绕
	CLabelUI *						m_pLabelFunctionArea;				//功能环绕
	CLabelUI *						m_pLabelChatHistory;				//聊天记录
	CLabelUI *						m_pLabelHistoryArea;				//记录区域

	//界面控件
protected:
	CButtonUI *						m_pbtCancel;						//关闭按钮
	CButtonUI *						m_pbtSendChat;						//发送聊天
	CButtonUI *						m_pbtSendControl;					//发送控制
	CButtonUI *						m_pbtUserMenu;						//用户菜单

	//界面控件
protected:
	CButtonUI *						m_pbtChatButton1;					//颜色设置
	CButtonUI *						m_pbtChatButton2;					//表情按钮
	CButtonUI *						m_pbtChatButton3;					//喇叭按钮
	CButtonUI *						m_pbtChatHistory;					//聊天记录	

	//界面控件
protected:	
	CButtonUI *						m_pbtAddFriend;						//加为好友
	CButtonUI *						m_pbtSetDetest;						//设为厌恶
	CButtonUI *						m_pbtMessageSetting;				//消息设置
	CButtonUI *						m_pbtShieldThisChat;				//屏蔽聊天
	
	//控制按钮
protected:	
	CSkinComboBox					m_ComboAndroidList;					//机器列表		

	//聊天控件
protected:
	CEdit							m_ChatInput;						//聊天输入
	CRichEditMessage				m_ChatMessage;						//聊天信息
	CRichEditMessage				m_HistoryEdit;						//聊天记录

	//记录控件
protected:
	IChatHistory*					m_pIChatHistory;					//聊天记录
	IChatHistoryManager*			m_pIChatHistroyManager;				//记录管理接口

	//组件变量
protected:
	CPlazaViewServer *				m_pServerViewItem;					//房间接口
	CExpressionControl *			m_pExpressionControl;				//表情窗口

	//函数定义
public:
	//构造函数
	CDlgWhisper();
	//析构函数
	virtual ~CDlgWhisper();

	//重载函数
protected:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgWhisper.xml"); } 
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//配置函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();
	//命令函数
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//重置函数
protected:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	

	//重置函数
protected:
	//绘画标题
	//virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//绘画消息
	virtual VOID DrawClientAreaEnd(CDC * pDC, INT nWidth, INT nHeight);
	//表情事件
	virtual VOID OnExpressionSelect(CExpression * pExpression, tagExpressionInfo * pExpressionInfo);

	virtual void OnEndPaintWindow(HDC hDC);

	//配置函数
public:
	//设置权限
	VOID SetUserRight(DWORD dwUserRight) { m_dwUserRight = dwUserRight; }
	//设置权限
	VOID SetMasterRight(DWORD dwMasterRight) { m_dwMasterRight = dwMasterRight; }
	//设置接口
	VOID SetTCPSocket(ITCPSocket * pITCPSocket) { m_pITCPSocket=pITCPSocket; }
	//设置接口
	VOID SetServerViewItem(CPlazaViewServer * pServerViewItem) { m_pServerViewItem=pServerViewItem; }
	//设置接口
	VOID SetMySelfUserItem(IClientUserItem * pIMySelfUserItem) { m_pIMySelfUserItem=pIMySelfUserItem; }
	//设置接口
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser) { m_pIUserOrderParser=pIUserOrderParser; }
	//设置接口
	VOID SetGameLevelParser(IGameLevelParser * pIGameLevelParser) { m_pIGameLevelParser=pIGameLevelParser; }
	//设置接口
	VOID SetPlazaUserManager(IPlazaUserManager* pIPlazaUserManager) { m_pIPlazaUserManager=pIPlazaUserManager; }
	//设置聊天记录管理接口
	VOID SetChatHistoryManager(IChatHistoryManager *pIChatHistoryManager) { m_pIChatHistroyManager = pIChatHistoryManager; }

	//功能函数
public:
	//废弃判断
	bool DisuseEstimate();
	//用户比较
	bool CompareUserItem(DWORD dwUserID[], WORD wUserCount);
	//聊天效验
	bool EfficacyUserChat(LPCTSTR pszChatString, WORD wExpressionIndex);
	//创建窗口
	bool CreateWhisperWnd(DWORD dwConversationID,DWORD dwSendUserID, IClientUserItem * pIClientUserItem[], WORD wUserCount, CWnd * pParentWnd);

	//事件函数
public:
	//用户进入
	VOID OnEventUserEnter(IClientUserItem * pIClientUserItem);
	//用户离开
	VOID OnEventUserLeave(IClientUserItem * pIClientUserItem);
	//用户状态
	VOID OnEventUserStatus(IClientUserItem * pIClientUserItem);
	//用户表情
	VOID OnEventExpression(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//用户私聊
	VOID OnEventWisperChat(IClientUserItem * pIClientUserItem, LPCTSTR pszString, COLORREF rcColor);

	//内部函数
protected:
	//更新标题
	VOID RectifyTitle();
	//调整控件
	VOID RectifyControl(INT nWidth, INT nHeight);
	//排序函数
	VOID SortUserIDData(DWORD dwUserID[], WORD wItemCount);
	//绘画控制
	VOID DrawFunctionArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);
	//绘画用户
	VOID DrawUserInfoArea(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight);

	//网络命令
protected:
	//发送表情
	bool SendExpressionPacket(IClientUserItem * pIClientUserItem, WORD wItemIndex);
	//发送聊天
	bool SendWhisperChatPacket(IClientUserItem * pIClientUserItem, LPCTSTR pszChatString);
	//发送会话
	bool SendColloquyChatPacket(IClientUserItem * pIClientUserItem[], WORD wUserCount, LPCTSTR pszChatString);

	//辅助函数
protected:
	//加载聊天记录
	VOID LoadChatHistory();
	//加载机器
	VOID LoadAndroidList();

	//按钮消息
protected:
	//用户菜单
	VOID OnBnClickedUserMenu();
	//发送消息
	VOID OnBnClickedSendChat();
	//发送控制
	VOID OnBnClickedSendControl();
	//聊天按钮
	VOID OnBnClickedChatButton1();
	//聊天按钮
	VOID OnBnClickedChatButton2();
	//聊天按钮
	VOID OnBnClickedChatButton3();
	//聊天记录
	VOID OnBnClickedChatHistory();
	//屏蔽私聊
	VOID OnBnClickedShieldThisChat();
	//加为好友
	VOID OnBnClickAddFriend();
	//设为厌恶
	VOID OnBnClickSetDetest();
	//屏蔽私聊
	VOID OnBnClickMessageSetting();

	//菜单命令
protected:
	//更多颜色
	VOID OnSelectMoreColor();
	//选择颜色
	VOID OnSelectChatColor(UINT nCommandID);

	//消息函数
protected:
	//销毁消息
	VOID OnNcDestroy();	
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//获取大小
	VOID OnGetMinMaxInfo(MINMAXINFO* lpMMI);	

	//控件消息
protected:
	//选择变更
	VOID OnCbnSelchangeComboxAndroid();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif