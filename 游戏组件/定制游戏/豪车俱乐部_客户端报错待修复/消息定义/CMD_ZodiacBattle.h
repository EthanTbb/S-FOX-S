#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define TEST_INT_OUT(num)  CString str;str.Format(L"this is:%d",num);AfxMessageBox(str);
#define KIND_ID						140									//游戏 I D
#define GAME_PLAYER					MAX_CHAIR							//游戏人数
#define GAME_NAME					TEXT("豪车俱乐部")					//游戏名字

//客户端消息
#define IDM_ADMIN_COMMDN			WM_USER+1000
#define IDM_UPDATE_STORAGE			WM_USER+1001
//状态定义
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//下注状态
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//结束状态
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//结束状态

#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//程序版本

#define COLOR_TEXT					RGB(193,167,108)

//区域索引
#define ID_MASERATI					1									//玛莎拉蒂	
#define ID_FERRARI					2									//法拉利		
#define ID_LAMBORGHINI				3									//兰博基尼		
#define ID_PORSCHE					4									//保时捷		
#define ID_LANDROVER				5									//路虎	
#define ID_BMW						6									//宝马	
#define ID_JAGUAR					7									//捷豹	
#define ID_BENZ						8									//奔驰	
		
#define AREA_MASERATI               0
#define AREA_FERRARI			    1
#define AREA_LAMBORGHINI            2
#define AREA_PORSCHE                3
#define AREA_LANDROVER		        4
#define AREA_BMW				    5
#define AREA_JAGUAR					6
#define AREA_BENZ					7


 //玛莎拉蒂 //法拉利 //兰博基尼 //保时捷 //路虎 //宝马 //捷豹 //奔驰 


#define AREA_COUNT					8									//区域数目

#define AREA_ALL					8									//动物种类

#define ANIMAL_COUNT                16                                  //动物个数
//赔率定义
#define RATE_TWO_PAIR				12									//对子赔率

#define SERVER_LEN					32									//房间长度

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

//机器人信息
struct tagRobotInfo
{
	int nChip[7];														//筹码定义
	int nAreaChance[AREA_COUNT];										//区域几率
	TCHAR szCfgFileName[MAX_PATH];										//配置文件
	int	nMaxTime;														//最大赔率

	tagRobotInfo()
	{
		int nTmpChip[7] = {100,1000,10000,100000,1000000,5000000,10000000};
		int nTmpAreaChance[AREA_COUNT] = {3, 3, 3, 3, 10, 10, 10, 10};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("LuxuryCarConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//记录信息
struct tagServerGameRecord
{
	BYTE							cbAnimal;						   //开中动物
};

#define SUB_S_UPDATE_STORAGE        114									//更新库存
//更新库存
struct CMD_C_UpdateStorage
{
	BYTE                            cbReqType;						//请求类型
	LONGLONG						lStorageDeduct;					//库存衰减
	LONGLONG						lStorageCurrent;				//当前库存
	LONGLONG						lStorageMax1;					//库存上限1
	LONGLONG						lStorageMul1;					//系统输分概率1
	LONGLONG						lStorageMax2;					//库存上限2
	LONGLONG						lStorageMul2;					//系统输分概率2
};
//下注信息
struct tagUserBet
{
	TCHAR							szNickName[32];						//用户昵称
	DWORD							dwUserGameID;						//用户ID
	LONGLONG						lUserStartScore;					//用户金币
	LONGLONG						lUserWinLost;						//用户金币
	LONGLONG						lUserBet[AREA_COUNT];				//用户下注币
	LONGLONG						lUserInsure;						//银行存款
	bool							bAttention;							//是否关注
	bool							bAndroid;							//是否机器人
};
//下注信息数组
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;
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
//#define SUB_S_CHECK_IMAGE			109									//取消申请
#define SUB_S_ADMIN_COMMDN			110									//系统控制
#define SUB_S_WAIT_BANKER			111									//等待上庄
#define SUB_S_ROBOT_BANKER          112                                 //机器人


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
	WORD							wCancelUser;						//取消玩家
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
	LONGLONG						lUserMaxScore;						//玩家金币
	//庄家信息
	WORD							wBankerUser;						//当前庄家
	WORD							cbBankerTime;						//庄家局数
	LONGLONG						lBankerWinScore;					//庄家成绩
	LONGLONG						lBankerScore;						//庄家分数
	bool							bEnableSysBanker;					//系统做庄

	//控制信息
	LONGLONG						lApplyBankerCondition;				//申请条件
	LONGLONG						lAreaLimitScore;					//区域限制
	
	TCHAR							szGameRoomName[32];					//房间名称 
	TCHAR							szRoomTotalName[256];					//房间全名
	int								nMultiple;							//筹码比例
	tagCustomAndroid				CustomAndroid;						//机器人配置
	bool							bGenreEducate;						//练习模式
	LONGLONG						lUserStartScore[GAME_PLAYER];		//起始分数
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
	BYTE							cbTableCard;				//桌面扑克

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
	TCHAR							szGameRoomName[32];					//房间名称 
	TCHAR							szRoomTotalName[256];					//房间全名
	int								nMultiple;							//筹码比例
	tagCustomAndroid				CustomAndroid;						//机器人配置
	bool							bGenreEducate;						//练习模式
	LONGLONG						lUserStartScore[GAME_PLAYER];		//起始分数

};

//游戏空闲
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//剩余时间
	LONGLONG lStorageCurrent;
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
	int								nAndriodCount;						//机器人人数
};

//用户下注
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//用户位置
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目
	BYTE							cbAndroid;							//机器人
};

//游戏结束
struct CMD_S_GameEnd
{
	//下局信息
	BYTE							cbTimeLeave;						//剩余时间

	//扑克信息
	BYTE							cbTableCard;						//桌面扑克
	//庄家信息
	LONGLONG						lBankerScore;						//庄家成绩
	LONGLONG						lBankerTotallScore;					//庄家成绩
	INT								nBankerTime;						//做庄次数

	//玩家成绩
	LONGLONG						lUserScore;							//玩家成绩
	LONGLONG						lUserReturnScore;					//返回积分

	//全局信息
	LONGLONG						lRevenue;							//游戏税收
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON			11									//用户下注
#define SUB_C_APPLY_BANKER			12									//申请庄家
#define SUB_C_CANCEL_BANKER			13									//取消申请
#define SUB_C_CLEAR_JETTON		    14									//清除下注

#define SUB_C_UPDATE_STORAGE        113									//更新库存

#define SUB_C_ADMIN_COMMDN			16									//系统控制

//库存控制
#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2
//用户下注
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//筹码区域
	LONGLONG						lJettonScore;						//加注数目

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
//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN WM_USER+1000

//控制区域信息
struct tagControlInfo
{
	BYTE cbControlArea;						//控制区域
};


//服务器控制返回
#define	 S_CR_FAILURE				0		//失败
#define  S_CR_UPDATE_SUCCES			1		//更新成功
#define	 S_CR_SET_SUCCESS			2		//设置成功
#define  S_CR_CANCEL_SUCCESS		3		//取消成功
#define  S_CR_ACK_PRINT_STORAGE		4		//设置库存
struct CMD_S_ControlReturns
{
	BYTE cbReturnsType;				//回复类型
	BYTE cbControlStyle;			//控制方式
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
	LONGLONG lCurStorage;			//当前库存
};


//申请类型
#define  C_CA_UPDATE				1		//更新
#define	 C_CA_SET					2		//设置
#define  C_CA_CANCELS				3		//取消
#define  C_CA_SET_STORAGE			4		//库存
#define  C_CA_PRINT_STORAGE			5		//库存

//控制方式
#define		CS_BANKER_LOSE    1
#define		CS_BANKER_WIN	  2
#define		CS_BET_AREA		  3

struct CMD_C_ControlApplication
{
	BYTE cbControlAppType;			//申请类型
	BYTE cbControlStyle;			//控制方式
	BYTE cbControlArea;				//控制区域
	BYTE cbControlTimes;			//控制次数
	LONGLONG lSetStorage;			//设置库存
};

//////////////////////////////////////////////////////////////////////////
#pragma pack()
#endif
