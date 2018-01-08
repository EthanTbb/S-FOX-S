#pragma once

////////////////////////////////////////////////////////////////////////////////////////
//房间设置
class CDlgCollocateRoom : public CFGuiDialog
{
	//配置变量
protected:
	CParameterGame *				m_pParameterGame;					//游戏配置
	CParameterServer *				m_pParameterServer;					//房间配置

	//胜率限制
public:
	WORD							m_wMinWinRate;						//最低胜率
	bool							m_bLimitWinRate;					//限制胜率

	//逃率限制
public:
	WORD							m_wMaxFleeRate;						//最高逃跑
	bool							m_bLimitFleeRate;					//限制断线

	//积分限制
public:
	DWORD							m_lMaxGameScore;					//最高分数 
	DWORD							m_lMinGameScore;					//最低分数
	bool							m_bLimitGameScore;					//限制分数

	//其他配置
public:
	bool							m_bLimitDetest;						//限制玩家
	bool							m_bLimitSameIP;						//效验地址

public:
	bool							m_bTakePassword;					//密码标志
	TCHAR							m_szTablePassword[LEN_PASSWORD];    //桌子密码

	//函数定义
public:
	//构造函数
	CDlgCollocateRoom();
	//析构函数
	virtual ~CDlgCollocateRoom();

	//功能函数
public:
	//配置参数
	bool InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer);

	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile() { return TEXT("DlgCollocateRoom.xml"); } 
	//初始控件
	virtual VOID InitControlUI();
	//消息提醒
    virtual VOID Notify(TNotifyUI &  msg);	

	//重载函数
public:
	//保存参数
	virtual bool SaveParameter();
	//默认参数
	virtual bool DefaultParameter();
	//更新控制
	virtual bool UpdateControlStatus();

	//重载函数
protected:
	//初始化窗口
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();

	//辅助函数
protected:
	//获取状态
	bool GetButtonCheckStatus(LPCTSTR pszButtonName);
	//获取字符
	VOID GetControlText(LPCTSTR pszControlName,TCHAR szString[], WORD wMaxCount);	
};

////////////////////////////////////////////////////////////////////////////////////////
