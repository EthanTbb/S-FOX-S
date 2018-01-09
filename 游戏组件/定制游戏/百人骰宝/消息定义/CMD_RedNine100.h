#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						118									//游戏 I D
#define GAME_PLAYER					MAX_CHAIR									//游戏人数
#define GAME_NAME					TEXT("百人骰宝")					//游戏名字

//状态定义
#define GS_GAME_START				GAME_STATUS_PLAY					//游戏开始
#define	GS_PLAYER_BET				GAME_STATUS_PLAY+1					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+2					//结束状态
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+3					//结束状态


//版本信息
#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

//区域索引
#define ID_AREA_DRAGON				0									//龙
#define ID_AREA_TIGER				1									//虎
#define ID_AREA_LEOPARD				2									//豹

//玩家索引
#define DRAGON_INDEX				0									//龙
#define TIGER_INDEX					1									//虎
#define LEOPARD_INDEX				2									//豹
#define BANKER_INDEX				3									//庄

//其它
#define AREA_COUNT					52								//区域数目
#define CARD_COUNT					2									//抓牌数目
#define DIRECT_COUNT				4									//方位数目
#define ALL_CARD_COUNT				40									//牌堆数目
#define MAX_ODDS					1									//最大赔率
#define JETTON_COUNT				6									//筹码种类
#define  DICE_COUNT					3									//骰子个数
//银行模块

static const BYTE g_cbAreaOdds[AREA_COUNT]={
											1,1,
											8,8,8,8,8,8,
											150,150,150,150,150,150,
											24,
											50,18,14,12,8,6,6,6,6,8,12,14,18,50,
											5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
											3,3,3,3,3,3,
											1,1
											};

//筹码面值
static const LONG g_lScoreType[JETTON_COUNT]={500L,1000L,10000L,100000L,1000000L,5000000L};
//筹码光标
static const UINT g_nJettonIDI[JETTON_COUNT]={3000,3001,3002,3003,3004,3005};
//筹码位图
static const UINT g_nJettonIDB[JETTON_COUNT]={3100,3101,3102,3103,3104,3105};
//筹码IDC
static const UINT g_nJettonIDC[JETTON_COUNT]={3200,3201,3202,3203,3204,3205};

//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率
#define SERVER_LEN					32									//房间长度

//机器人信息
struct tagRobotInfo
{
	int nChip[JETTON_COUNT];											//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[JETTON_COUNT] = {500L,1000L,10000L,100000L,1000000L,5000000L};
		//int nTmpAreaChance[AREA_COUNT] = {
		//		288,288,288,288,
		//		8,8,8,8,8,8,
		//		0,0,0,0,0,0,
		//		3,
		//		0,4,4,6,9,12,12,12,12,9,6,4,4,0,
		//		12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
		//		24,24,24,24,24,24
		//};
		int nTmpAreaChance[AREA_COUNT] = {
			24,24,24,24,
				24,24,24,24,24,24,
				12,12,12,12,12,12,
				12,
				12,24,24,24,24,12,12,12,12,12,12,12,12,12,
				12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
				24,24,24,24,24,24
		};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("SicBoBattleConfig.ini");

		nMaxTime = 2;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//机器人配置
struct tagCustomAndroid
{
	//坐庄
	BOOL							nEnableRobotBanker;				//是否做庄
	LONGLONG						lRobotBankerCountMin;			//坐庄次数
	LONGLONG						lRobotBankerCountMax;			//坐庄次数
	LONGLONG						lRobotListMinCount;				//列表人数
	LONGLONG						lRobotListMaxCount;				//列表人数
	LONGLONG						lRobotApplyBanker;				//最多申请个数
	LONGLONG						lRobotWaitBanker;				//空盘重申

	//下注
	LONGLONG						lRobotMinBetTime;				//下注筹码个数
	LONGLONG						lRobotMaxBetTime;				//下注筹码个数
	LONGLONG						lRobotMinJetton;				//下注筹码金额
	LONGLONG						lRobotMaxJetton;				//下注筹码金额
	LONGLONG						lRobotBetMinCount;				//下注机器人数
	LONGLONG						lRobotBetMaxCount;				//下注机器人数
	LONGLONG						lRobotAreaLimit;				//区域限制

	//存取款
	LONGLONG						lRobotScoreMin;					//金币下限
	LONGLONG						lRobotScoreMax;					//金币上限
	LONGLONG						lRobotBankGetMin;				//取款最小值(非庄)
	LONGLONG						lRobotBankGetMax;				//取款最大值(非庄)
	LONGLONG						lRobotBankGetBankerMin;			//取款最小值(坐庄)
	LONGLONG						lRobotBankGetBankerMax;			//取款最大值(坐庄)
	LONGLONG						lRobotBankStoMul;				//存款百分比

	//构造函数
	tagCustomAndroid()
	{
		DefaultCustomRule();
	}

	void DefaultCustomRule()
	{
		nEnableRobotBanker = TRUE;
		lRobotBankerCountMin = 5;
		lRobotBankerCountMax = 10;
		lRobotListMinCount = 2;
		lRobotListMaxCount = 5;
		lRobotApplyBanker = 5;
		lRobotWaitBanker = 3;

		lRobotMinBetTime = 6;
		lRobotMaxBetTime = 8;
		lRobotMinJetton = 100;
		lRobotMaxJetton = 5000000;
		lRobotBetMinCount = 4;
		lRobotBetMaxCount = 8;
		lRobotAreaLimit = 10000000;

		lRobotScoreMin = 1000000;
		lRobotScoreMax = 100000000;
		lRobotBankGetMin = 100;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;
	}
};

//机器人配置
struct tagRobotConfig
{
	//上庄操作
	SCORE lBankerCondition;											//上庄条件

	//银行操作
	SCORE lBankDrawCondition;										//取款条件
	SCORE lBankDrawScore;											//取款数额
	SCORE lBankSaveCondition;										//存款条件
	SCORE lBankSaveScore;											//存款数额

	//庄家操作
	SCORE lBankerDrawCount;											//取款次数 (庄家)
	SCORE lBankerWinGiveUp;											//赢分下庄 (庄家)
	SCORE lBankerLoseGiveUp;											//输分下庄 (庄家)

	//下注操作
	SCORE lJettonLimit[2];											//筹码范围
	SCORE lBetTimeLimit;												//下注次数

	//配置人数
	int		 nCfgRobotCount;											//配置人数
};

//记录信息
struct tagGameRecord
{
	BYTE cbDiceValue[DICE_COUNT];
};

//下注信息
struct tagUserBet
{
	TCHAR							szNickName[32];						//用户昵称
	DWORD							dwUserGameID;						//用户ID
	LONGLONG						lUserStartScore;					//用户金币
	LONGLONG						lUserWinLost;						//用户金币
	LONGLONG						lUserBet[AREA_COUNT];				//用户下注
};

//下注信息数组
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#endif

//库存控制
#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2
//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE				99									//游戏空闲
#define SUB_S_GAME_START			100									//游戏开始
#define SUB_S_START_BET				101
#define SUB_S_PLACE_JETTON			102									//用户下注
#define SUB_S_GAME_END				103									//游戏结束
#define SUB_S_APPLY_BANKER			104									//申请庄家
#define SUB_S_CHANGE_BANKER			105									//切换庄家
#define SUB_S_CHANGE_USER_SCORE		106									//更新积分
#define SUB_S_SEND_RECORD			107									//游戏记录
#define SUB_S_PLACE_JETTON_FAIL		108									//下注失败
#define SUB_S_CANCEL_BANKER			109									//取消申请
#define SUB_S_ANDROA_AREA			110									//WinArea	
#define SUB_S_REVOCAT_BET			111									//撤销押注
#define SUB_S_ROBOT_BANKER			112									//上庄通知 (机器人)
#define SUB_S_ROBOT_CONFIG			113									//配置通知 (机器人)
#define SUB_S_AMDIN_COMMAND			120									//管理员命令
#define SUB_S_SEND_USER_BET_INFO    121									//发送下注
#define SUB_S_UPDATE_STORAGE        122									//更新库存
#define SUB_S_CONTROL_WIN			123									//控制单个玩家输赢


//控制区域信息
struct tagControlInfo
{
	BYTE cbControlArea[5];			//控制区域
};

struct CMD_C_FreshStorage
{
	BYTE                            cbReqType;						//请求类型
	LONGLONG						lStorageDeduct;					//库存衰减
	LONGLONG						lStorageCurrent;				//当前库存
	LONGLONG						lStorageMax1;					//库存上限1
	LONGLONG						lStorageMul1;					//系统输分概率1
	LONGLONG						lStorageMax2;					//库存上限2
	LONGLONG						lStorageMul2;					//系统输分概率2
};

//更新库存
struct CMD_S_UpdateStorage
{
	BYTE                            cbReqType;						//请求类型
	LONGLONG						lStorageStart;				//起始库存
	LONGLONG						lStorageDeduct;				//库存衰减
	LONGLONG						lStorageCurrent;				//当前库存
	LONGLONG						lStorageMax1;					//库存上限1
	LONGLONG						lStorageMul1;					//系统输分概率1
	LONGLONG						lStorageMax2;					//库存上限2
	LONGLONG						lStorageMul2;					//系统输分概率2
};

struct CMD_C_ControlWinLose
{
	BYTE							cbWinLose;						//0为无效值，1为赢，2为输
	LONGLONG						lQueryGameID;					//玩家ID
};

struct CMD_S_ControlWinLose
{
	BYTE							cbWinLose;						//0为无效值，1为赢，2为输
	LONGLONG						lQueryGameID;					//玩家ID
};

//请求回复
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//回复类型
#define ACK_SET_WIN_AREA  1
#define ACK_RESET_CONTROL 2
#define ACK_PRINT_SYN     3
	BYTE cbResult;
#define CR_ACCEPT  2			//接受
#define CR_REFUSAL 3			//拒绝
#define CR_INVALID 4			//失效
	BYTE cbSicbo1;
	BYTE cbSicbo2;
	BYTE cbSicbo3;
	BYTE cbTotalCount;
	BYTE cbLastType;			//上一次的控制类型
};

struct CMD_S_RevocatBet
{
	SCORE   lUserBet[AREA_COUNT];
};

struct CMD_S_AndroidArea
{
	BYTE bWinArea[AREA_COUNT];
	CMD_S_AndroidArea()
	{
		memset(this,0,sizeof(*this));
	}
};

//发送下注
struct CMD_S_SendUserBetInfo
{
	BYTE							cbAreaCount;							//下注区域
	LONGLONG						lUserStartScore[GAME_PLAYER];				//起始分数
	LONGLONG						lUserJettonScore[GAME_PLAYER];//个人总注
};

//失败结构
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//下注玩家
	BYTE							lJettonArea;						//下注区域
	SCORE						lPlaceScore;						//当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//椅子号码
	WORD							wCurrentBankerChairID;				//当前庄家
	BYTE							cbBankerTime;						//庄家局数
	SCORE						lScore;								//玩家积分
	SCORE						lCurrentBankerScore;				//庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;					//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	SCORE						lBankerScore;						//庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	bool							bEnableSysBanker;					//系统做庄
	//玩家信息
	SCORE						lUserMaxScore;						//玩家金币
	SCORE						lBankerWinScore;					//庄家成绩
	SCORE						lBankerScore;						//庄家分数
	//控制信息
	SCORE						lApplyBankerCondition;				//申请条件
	SCORE						lAreaLimitScore;					//区域限制
	//房间信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称};
	tagCustomAndroid				CustomAndroid;						//机器人配置
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	SCORE							lAllJettonScore[AREA_COUNT];		//全体总注

	//玩家下注
	SCORE							lUserJettonScore[AREA_COUNT];		//个人总注

	//玩家积分
	SCORE							lUserMaxScore;						//最大下注							

	//控制信息
	SCORE							lApplyBankerCondition;				//申请条件
	SCORE							lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE								cbDiceValue[DICE_COUNT];				//桌面扑克

	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	SCORE						lBankerWinScore;					//庄家赢分
	SCORE						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//结束信息
	SCORE						lEndBankerScore;					//庄家成绩
	SCORE						lEndUserScore;						//玩家成绩
	SCORE						lEndUserReturnScore;				//返回积分
	LONG							lEndRevenue;						//游戏税收

	//全局信息
	BYTE							cbTimeLeave;						//剩余时间
	BYTE							cbGameStatus;						//游戏状态

	//房间信息
	TCHAR							szGameRoomName[SERVER_LEN];			//房间名称};
	tagCustomAndroid				CustomAndroid;						//机器人配置
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
	INT64                           nListUserCount;						//列表人数
};

//游戏开始
struct CMD_S_GameStart
{
	WORD							wBankerUser;									//庄家位置
	BYTE							cbTimeLeave;									//剩余时间	
	SCORE						lBankerScore;									//庄家金币
	SCORE						lUserMaxScore;									//我的金币
	int								nChipRobotCount;								//人数上限 (下注机器人)
	int								nAndriodApplyCount;					//机器人列表人数
	BYTE							bWinFlag[AREA_COUNT];							//输赢信息 (机器人)
};

//开始下注
struct CMD_S_StartBet
{
	BYTE							cbTimeLeave;									//剩余时间	
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	SCORE						lJettonScore;						//加注数目
	bool							bIsAndroid;							//是否机器人
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbDiceValue[DICE_COUNT];			//骰子点数
	BYTE							cbLeftCardCount;					//扑克数目

	BYTE							bcFirstCard;
 
	//庄家信息
	INT								nBankerTime;						//做庄次数
	SCORE						lBankerScore;						//庄家成绩
	SCORE						lBankerTotallScore;					//庄家成绩
	//玩家成绩
	SCORE						lUserScore;							//玩家成绩
	SCORE						lUserReturnScore;					//返回积分

	//全局信息
	LONG							lRevenue;							//游戏税收
};

//配置通知
struct CMD_S_RobotConfig
{
	tagRobotConfig					robotConfig;						//机器配置
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CONTINUE_CARD			4									//继续发牌
#define SUB_C_AMDIN_COMMAND			5									//管理员命令
#define SUB_C_STORAGE				6									//更新库存
#define SUB_C_CONTROL_WIN			7									//控制单个玩家输赢
#define IDM_ADMIN_STORAGE			WM_USER+1001
#define IDM_CONTROL_WIN				WM_USER+1002

struct tagAdminReq
{
	BYTE		m_cbControlStyle;					//控制方式
				#define		CS_BANKER_LOSE    1
				#define		CS_BANKER_WIN	  2
				#define		CS_BET_AREA		  3
	BYTE cbSicbo1;
	BYTE cbSicbo2;
	BYTE cbSicbo3;
	BYTE cbTotalCount;
};


struct CMD_C_AdminReq
{
	BYTE cbReqType;
	#define RQ_SET_COUNT		1
	#define RQ_RESET_CONTROL	2
	#define RQ_PRINT_SYN		3
	BYTE cbSicbo1;
	BYTE cbSicbo2;
	BYTE cbSicbo3;
	BYTE cbTotalCount;
};

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	SCORE						lJettonScore;						//加注数目
};

#pragma pack(pop)
//////////////////////////////////////////////////////////////////////////

#endif
