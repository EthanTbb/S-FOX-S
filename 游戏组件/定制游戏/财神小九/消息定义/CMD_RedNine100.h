#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						108//109									//游戏 I D
#define GAME_PLAYER					MAX_CHAIR									//游戏人数
#define GAME_NAME					TEXT("财神到")					    //游戏名字

//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//结束状态
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//结束状态

//版本信息
#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//程序版本

//区域索引
#define ID_SHUN_MEN					1									//上门
#define ID_DUI_MEN					2									//天门
#define ID_DAO_MEN					3									//下门
#define ID_JIAO_L					4									//左边角
#define ID_QIAO						5									//桥
#define ID_JIAO_R					6									//右边角

//玩家索引
#define BANKER_INDEX				0									//庄家索引
#define SHUN_MEN_INDEX				1									//顺门索引
#define DUI_MEN_INDEX				2									//对门索引
#define DAO_MEN_INDEX				3									//倒门索引
#define MAX_INDEX					4									//最大索引

#define AREA_COUNT					3									//区域数目
#define AREA_ARRY_COUNT				7									//区域数目

//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率

#define SERVER_LEN					32									//房间长度

//明星人数
#define STAR_COUNT				10

//胜负表示
enum WinFlag
{
	WIN_FLAG_LOST,
	WIN_FLAG_WIN,
	WIN_FLAG_EQUAL,
};	

//机器人信息
struct tagRobotInfo
{
	int nChip[6];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[6] = {1000,10000, 100000, 1000000, 5000000, 10000000};
		int nTmpAreaChance[AREA_COUNT] = {1, 1,1};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("GodInConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							bWinShunMen;						//顺门胜利
	BYTE							bWinDuiMen;							//对门胜利
	BYTE							bWinDaoMen;							//倒门胜利
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_PLACE_JETTON			101									//用户下注
#define SUB_S_GAME_END				102									//游戏结束
#define SUB_S_APPLY_BANKER			103									//申请庄家
#define SUB_S_CHANGE_BANKER			104									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		105									//更新积分
#define SUB_S_SEND_RECORD			106									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		107									//下注失败
#define SUB_S_CANCEL_BANKER			108									//取消申请
#define SUB_S_CANCEL_CAN			109									//取消申请
#define SUB_S_ANDROA_AREA			110									//赢区域
#define SUB_S_AMDIN_COMMAND_RESULT  111									//管理员命令
#define SUB_S_TO_BANKER_MAX         113									//达到庄家上限
#define SUB_S_ROBOT_BANKER			114									//上庄通知 (机器人)
#define SUB_S_NOTICE_TIME           115                                 //通知时间

//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//回复类型
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//接受
#define CR_REFUSAL 3			//拒绝
	BYTE cbExtendData[20];			//附加数据
};

//失败结构
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//下注玩家
	BYTE							lJettonArea;						//下注区域
	LONGLONG						lPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	DOUBLE							lScore;								//玩家积分

	//庄家信息
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	DOUBLE							lCurrentBankerScore;				//庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
	TCHAR							szCancelUser[32];					//取消玩家
};

//是否成功取消申请
struct CMD_S_bCanCancelBanker
{
	WORD							wChariID;
	BOOL							blCancel;
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	LONGLONG						lBankerScore;						//庄家金币
};

//空闲时间通知
struct CMD_S_FreeTime
{
	BYTE                            cbTimeLeave;						//剩余时间
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间

	//玩家信息
	LONGLONG						lUserMaxScore;							//玩家金币

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家成绩
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄
	//明星信息
	float							fStarWinRate[STAR_COUNT];
	LONGLONG						llStarAllCount[STAR_COUNT];
	LONGLONG						llStarWinCount[STAR_COUNT];
	LONGLONG						llAllStarCount;
	int								nRedBack;

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//房间信息
	TCHAR							szServerName[SERVER_LEN];			//房间名称
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	LONGLONG						lAllJettonScore[AREA_ARRY_COUNT];		//全体总注

	//玩家下注
	LONGLONG						lUserJettonScore[AREA_ARRY_COUNT];		//个人总注

	//玩家积分
	LONGLONG						lUserMaxScore;						//最大下注							

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE							cbTableCardArray[4][2];				//桌面扑克

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家赢分
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	LONGLONG						lEndBankerScore;					//庄家成绩
	LONGLONG						lEndUserScore;						//玩家成绩
	LONGLONG						lEndUserReturnScore;				//返回积分
	LONGLONG						lEndRevenue;						//游戏税收

	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameStatus;						//游戏状态
	//明星信息
	BYTE							cbStarSelect[3];
	float							fStarWinRate[STAR_COUNT];
	LONGLONG						llStarAllCount[STAR_COUNT];
	LONGLONG						llStarWinCount[STAR_COUNT];
	LONGLONG						llAllStarCount;
	int								nRedBack;

	//房间信息
	TCHAR							szServerName[SERVER_LEN];			//房间名称
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
	bool                            bApplyBanker;						//申请庄家
	
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家位置
	LONGLONG						lBankerScore;						//庄家金币
	LONGLONG						lUserMaxScore;						//我的金币
	BYTE							cbTimeLeave;						//剩余时间	
	bool							bContiueCard;						//继续发牌
	int								nChipRobotCount;					//人数上限 (下注机器人)
	LONGLONG                        lUserLimitScore;					//限制分数
	BYTE							cbStarSelect[3];
	float							fStarWinRate[STAR_COUNT];
	LONGLONG						llStarAllCount[STAR_COUNT];
	LONGLONG						llStarWinCount[STAR_COUNT];
	LONGLONG						llAllStarCount;
	int								nRedBack;
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
	BYTE							cbAndroidUser;						//机器标识
	BYTE							cbAndroidUserT;						//机器标识
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbTableCardArray[4][2];				//桌面扑克
	BYTE							cbLeftCardCount;					//扑克数目

	BYTE							bcFirstCard;
 
	//庄家信息
	LONGLONG						lBankerScore;						//庄家成绩
	LONGLONG						lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数
	bool							bBankerBomb;

	//玩家成绩
	LONGLONG						lUserScore;							//玩家成绩
	LONGLONG						lUserReturnScore;					//返回积分

	//全局信息
	LONGLONG						lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CONTINUE_CARD			4									//继续发牌
#define SUB_C_MANAGE_CONTROL		5									//取消申请
#define SUB_C_AMDIN_COMMAND			6									//管理员命令

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE						cbJettonArea;						//筹码区域
	LONGLONG					lJettonScore;						//加注数目
};

//////////////////////////////////////////////////////////////////////////

//赢的区域
struct CMD_S_AdminControl
{
	BYTE cbControlType;						//控制类型
};

#define IDM_ADMIN_COMMDN					(WM_USER+3201)
//客户端控制信息
struct CMD_C_AdminControl
{
	BYTE cbControlArea[MAX_INDEX];			//控制区域
};

#endif
