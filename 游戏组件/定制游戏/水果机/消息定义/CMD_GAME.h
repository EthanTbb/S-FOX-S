#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						131									//游戏 I D
#define GAME_PLAYER					MAX_CHAIR									//游戏人数
#define GAME_NAME					TEXT("水果机")						//游戏名字

//版本信息
#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

//状态定义
#define	GS_GAME_FREE				GAME_STATUS_FREE
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY+1					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+2						//结束状态
#define	GS_COMPARE_END				GAME_STATUS_PLAY+3						//结束状态

//区域索引
#define ID_TIAN_MEN					1									//顺门
#define ID_DI_MEN					2									//左边角
#define ID_XUAN_MEN					3									//桥
#define ID_HUANG_MEN				4									//对门

//玩家索引
#define BANKER_INDEX				0									//庄家索引
#define SHUN_MEN_INDEX				1									//顺门索引
#define DUI_MEN_INDEX				2									//对门索引
#define DAO_MEN_INDEX				3									//倒门索引
#define HUAN_MEN_INDEX				4									//倒门索引

#define AREA_COUNT					8									//区域数目

//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率

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
		lRobotBankGetMin = 1000;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;
	}
};

//机器人信息
struct tagRobotInfo
{
	int nChip[8];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	int nAreaOdds[AREA_COUNT];											//区域赔率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[8] = {1,10,100,1000,10000,100000,1000000,5000000};
		int nTmpAreaChance[AREA_COUNT] = {1, 2, 2, 5, 10, 10, 10, 10};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("FruitBattle.ini");
		int nTmpOdds[AREA_COUNT] = {5,10,10,10,20,20,20,50};

		nMaxTime = 10;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
		memcpy(nAreaOdds,nTmpOdds,sizeof(nAreaOdds));

	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							bWinMen[AREA_COUNT+1];						//顺门胜利
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
#define SUB_S_CHECK_IMAGE			109									//取消申请
#define SUB_S_ADMIN_COMMDN			110									//系统控制
#define	SUB_S_CHANGE_GOLD			111									//兑换筹码
#define SUB_S_GAME_COMPARE			112									//游戏猜大小
#define	SUB_S_COMPARE				113									//比牌时间
#define	SUB_S_GUESS					114									//猜大小
#define SUB_S_ADMIN_STORAGE_INFO	115									//刷新控制服务端

//控制服务端信息
struct CMD_S_ADMIN_STORAGE_INFO
{
	LONGLONG						lCurrentStorage;					//当前库存
	LONGLONG						lCurrentDeduct;						//当前衰减
	LONGLONG						lMaxStorage[2];						//库存上限
	WORD							wStorageMul[2];						//系统输分概率
	LONGLONG						lUserTotalScore[GAME_PLAYER];		//玩家总输赢
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
	WORD							wCancelUser;					//取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//当庄玩家
	LONGLONG						lBankerScore;						//庄家金币
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

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制
	int								CheckImage;

	TCHAR							szGameRoomName[32];					//房间名称 
	int								nExchange;							//兑换比例

	LONGLONG						lStartStorage;						//起始库存
	tagCustomAndroid				CustomAndroid;						//机器人配置
};

//游戏状态
struct CMD_S_StatusPlay
{
	//全局下注
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//全体总注

	//玩家下注
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//个人总注

	//玩家积分
	LONGLONG						lUserMaxScore;						//最大下注							

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制

	//扑克信息
	BYTE							cbTableCardArray[5];				//桌面扑克

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
	int								CheckImage;
	TCHAR							szGameRoomName[32];			//房间名称 
	int								nExchange;							//兑换比例

	LONGLONG						lStartStorage;						//起始库存
	tagCustomAndroid				CustomAndroid;						//机器人配置
};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
	INT64                             nListUserCount;						//列表人数
	LONGLONG						lStorageStart;	
	LONGLONG						lBankerWinScore;					//庄家成绩
	LONGLONG						lExchangeGold[GAME_PLAYER];			//兑换金币量
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
	int								nAndroidCount;						//机器人上庄数

};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
	BYTE							cbAndroid;							//机器人
	bool							bAndroid;							//
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbTableCardArray[5];				//桌面扑克
	BYTE							cbLeftCardCount;					//扑克数目

	BYTE							bcFirstCard;
 
	//庄家信息
	LONGLONG						lBankerScore;						//庄家成绩
	LONGLONG						lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	LONGLONG						lUserScore;							//玩家成绩
	LONGLONG						lUserReturnScore[GAME_PLAYER];		//玩家赢的钱
	int								nProbability;						//中奖几率
	int								nRand;								//打枪次数

	//全局信息
	LONGLONG						lRevenue;							//游戏税收
};

//兑换筹码
struct CMD_S_ChangeGold
{	
	LONGLONG						lExchangeGold[GAME_PLAYER];			//兑换金币量
	WORD							wChairID;							//兑换玩家
};

//兑换筹码
struct CMD_S_Compare
{	
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间
};

//猜大小
struct CMD_S_Guess
{	
	int								nGuessNum;							//猜中的数字
	LONGLONG						lUserReturnScore;					//玩家赢的钱
	WORD							wChairID;							//猜大小的玩家
	
	INT								nBankerTime;						//做庄次数
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			1									//用户下注
#define SUB_C_APPLY_BANKER			2									//申请庄家
#define SUB_C_CANCEL_BANKER			3									//取消申请
#define SUB_C_CONTINUE_CARD			4									//继续发牌
#define SUB_C_CHECK_IMAGE			5									//继续发牌
#define SUB_C_ADMIN_COMMDN			6									//系统控制
#define SUB_C_STORAGE				7									//更新库存
#define SUB_C_DEUCT					8									//更新库存
#define	SUB_C_CHANGE_GOLD			9									//兑换筹码
#define	SUB_C_GUESS					10									//猜大小
#define SUB_C_STORAGEINFO			11									//修改吐分条件

//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
};

struct CMD_C_CheckImage
{
	int Index;
};

//兑换筹码
struct CMD_C_ChangeGold
{
	LONGLONG						lChangeGold;						//兑换金币量
};

//猜大小
struct CMD_C_Guess
{
	BOOL							bBig;								//
};


//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN WM_USER+1000
#define IDM_ADMIN_STORAGE			WM_USER+1001
#define IDM_ADMIN_DEUCT				WM_USER+1002
#define IDM_ADMIN_STORAGEINFO		WM_USER+1003
#define IDM_ADMIN_STARTESTORAGE		WM_USER+1004
#define IDM_ADMIN_PLAYERINFO		WM_USER+1005
//控制区域信息
struct tagControlInfo
{
	BYTE cbControlArea;						//控制区域
};

struct CMD_C_FreshStorage
{
	LONGLONG						lStorageStart;						//库存数值
};

struct CMD_C_FreshDeuct
{
	LONGLONG						lStorageDeuct;						//库存数值
};

struct CMD_C_StorageInfo
{
	LONGLONG						lMaxStorage[2];						//库存上限
	WORD							wStorageMul[2];						//吐分概率
};

//服务器控制返回
#define	 S_CR_FAILURE				0		//失败
#define  S_CR_UPDATE_SUCCES			1		//更新成功
#define	 S_CR_SET_SUCCESS			2		//设置成功
#define  S_CR_CANCEL_SUCCESS		3		//取消成功
struct CMD_S_ControlReturns
{
	BYTE cbReturnsType;				//回复类型
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
};


//客户端控制申请
#define  C_CA_UPDATE				1		//更新
#define	 C_CA_SET					2		//设置
#define  C_CA_CANCELS				3		//取消
struct CMD_C_ControlApplication
{
	BYTE cbControlAppType;			//申请类型
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
};

//////////////////////////////////////////////////////////////////////////

#endif
