#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE


#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID             6                 //游戏 I D
#define GAME_PLAYER           5                 //游戏人数
#define GAME_NAME           TEXT("诈金花")            //游戏名字
#define MAX_COUNT           3                 //扑克数目
#define VERSION_SERVER            PROCESS_VERSION(7,0,1)        //程序版本
#define VERSION_CLIENT            PROCESS_VERSION(7,0,1)        //程序版本

//游戏概率
#define GIVE_UP             2                 //放弃概率
#define LOOK_CARD           8                 //看牌概率
#define COMPARE_CARD          5                 //比牌概率
#define MAX_LOOK_INDEX          10                  //看牌索引
#define MAX_OPEN_INDEX          5                 //开牌索引
#define SERVER_LEN            32
#define LEN_NICKNAME          32                  //昵称长度

//结束原因
#define GER_NO_PLAYER         0x10                //没有玩家
#define GER_COMPARECARD         0x20                //比牌结束
#define GER_OPENCARD          0x30                //开牌结束

//游戏状态
#define GS_T_FREE         GAME_STATUS_FREE            //等待开始
#define GS_T_SCORE          GAME_STATUS_PLAY            //叫分状态
#define GS_T_PLAYING        GAME_STATUS_PLAY+1            //游戏进行

#define IDM_ADMIN_MODIFY_STORAGE          WM_USER+1011
#define IDM_ADMIN_REQUEST_ADD_USERROSTER      WM_USER+1012
#define IDM_ADMIN_REQUEST_DELETE_USERROSTER     WM_USER+1013
#define IDM_REQUEST_UPDATE_USERROSTER       WM_USER+1014

//黑白名单
typedef enum {INVALID_ROSTER = 0, WHITE_ROSTER, BLACK_ROSTER} USERROSTER;

//房间用户信息
typedef struct
{
  DWORD             dwGameID;             //GAMEID
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称
  LONGLONG            lGameScore;             //游戏输赢分
  LONGLONG            lScoreLimit;            //用户限制分数
  USERROSTER            userroster;             //用户名单
} ROOMUSERINFO;

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START        100                 //游戏开始
#define SUB_S_ADD_SCORE         101                 //加注结果
#define SUB_S_GIVE_UP         102                 //放弃跟注
#define SUB_S_SEND_CARD         103                 //发牌消息
#define SUB_S_GAME_END          104                 //游戏结束
#define SUB_S_COMPARE_CARD        105                 //比牌跟注
#define SUB_S_LOOK_CARD         106                 //看牌跟注
#define SUB_S_PLAYER_EXIT       107                 //用户强退
#define SUB_S_OPEN_CARD         108                 //开牌消息
#define SUB_S_WAIT_COMPARE        109                 //等待比牌
#define SUB_S_ANDROID_CARD        110                 //智能消息
#define SUB_S_CHEAT_CARD        111                 //看牌消息

#define SUB_S_ADMIN_STORAGE_INFO    112                 //刷新库存
#define SUB_S_RESULT_ADD_USERROSTER   113                 //添加用户名单结果
#define SUB_S_RESULT_DELETE_USERROSTER  114                 //删除用户名单结果
#define SUB_S_UPDATE_USERROSTER     115                 //更新用户名单
#define SUB_S_REMOVEKEY_USERROSTER    116                 //移除用户名单
#define SUB_S_DUPLICATE_USERROSTER    117                 //重复用户名单

struct CMD_S_DuplicateUserRoster
{
  DWORD             dwGameID;
  USERROSTER            userroster;             //用户名单
  LONGLONG            lScoreLimit;            //用户限制分数
};

struct CMD_S_RemoveKeyUserRoster
{
  DWORD             dwUserID;
  DWORD             dwGameID;
  USERROSTER            userroster;             //用户名单
  LONGLONG            lScoreLimit;            //用户限制分数
};

struct CMD_S_AddUserRoster_Result
{
  DWORD             dwGameID;
  DWORD             dwUserID;
  TCHAR             szNickName[LEN_NICKNAME];     //用户昵称
  LONGLONG            lScoreLimit;            //用户限制分数
  USERROSTER            userroster;             //用户名单
  LONGLONG            lGameScore;             //游戏输赢分
  bool              bSucceed;
};

struct CMD_S_DeleteUserRoster_Result
{
  DWORD             dwGameID;
  DWORD             dwUserID;
  bool              bFind;                //找到标识
  bool              bExistRoster;           //之前存在黑白名单标识
  USERROSTER            beforeuserroster;         //之前用户名单
};

//更新用户名单（具有黑白名单的用户）
struct CMD_S_UpdateUserRoster
{
  DWORD             dwUserID;             //用户ID
  ROOMUSERINFO          roomUserInfo;
};

//控制服务端信息
struct CMD_S_ADMIN_STORAGE_INFO
{
	LONGLONG  lCurrentStorage;
	LONGLONG  lCurrentDeduct;
	LONGLONG  lMaxStorage;
	WORD    wStorageMul;
};

struct tagRobotConfig
{
  LONGLONG            lRobotScoreMin;
  LONGLONG            lRobotScoreMax;
  LONGLONG            lRobotBankGet;
  LONGLONG            lRobotBankGetBanker;
  LONGLONG            lRobotBankStoMul;
};



//游戏状态
struct CMD_S_StatusFree
{
  LONGLONG              lCellScore;             //基础积分
  TCHAR               szGameRoomName[SERVER_LEN];     //房间名称
  tagRobotConfig            RobotConfig;            //机器人配置
  LONGLONG              lStartStorage;            //起始库存
};

//游戏状态
struct CMD_S_StatusPlay
{
  //加注信息
  LONGLONG              lMaxCellScore;            //单元上限
  LONGLONG              lCellScore;             //单元下注
  LONGLONG              lCurrentTimes;            //当前倍数
  LONGLONG              lUserMaxScore;            //用户分数上限

  //状态信息
  WORD                wBankerUser;            //庄家用户
  WORD                wCurrentUser;           //当前玩家
  BYTE                cbPlayStatus[GAME_PLAYER];      //游戏状态
  bool                bMingZhu[GAME_PLAYER];        //看牌状态
  LONGLONG              lTableScore[GAME_PLAYER];     //下注数目
  TCHAR               szGameRoomName[SERVER_LEN];     //房间名称

  //扑克信息
  BYTE                cbHandCardData[MAX_COUNT];      //扑克数据

  //状态信息
  bool                bCompareState;            //比牌状态
  tagRobotConfig            RobotConfig;            //机器人配置
  LONGLONG              lStartStorage;            //起始库存
};

//机器人扑克
struct CMD_S_AndroidCard
{
  BYTE                cbRealPlayer[GAME_PLAYER];        //真人玩家
  BYTE                cbAndroidStatus[GAME_PLAYER];     //机器数目
  BYTE                cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//手上扑克
  LONGLONG              lStockScore;              //当前库存
};


//机器人扑克
struct CMD_S_CheatCardInfo
{
  BYTE                cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//手上扑克
};



//游戏开始
struct CMD_S_GameStart
{
  //下注信息
  LONGLONG              lMaxScore;              //最大下注
  LONGLONG              lCellScore;             //单元下注
  LONGLONG              lCurrentTimes;            //当前倍数
  LONGLONG              lUserMaxScore;            //分数上限

  //用户信息
  WORD                wBankerUser;            //庄家用户
  WORD                wCurrentUser;           //当前玩家
  BYTE                cbPlayStatus[GAME_PLAYER];      //用户状态
};

//用户下注
struct CMD_S_AddScore
{
  WORD                wCurrentUser;           //当前用户
  WORD                wAddScoreUser;            //加注用户
  WORD                wCompareState;            //比牌状态
  LONGLONG              lAddScoreCount;           //加注数目
  LONGLONG              lCurrentTimes;            //当前倍数
};

//用户放弃
struct CMD_S_GiveUp
{
  WORD                wGiveUpUser;            //放弃用户
};

//比牌数据包
struct CMD_S_CompareCard
{
  WORD                wCurrentUser;           //当前用户
  WORD                wCompareUser[2];          //比牌用户
  WORD                wLostUser;              //输牌用户
};

//看牌数据包
struct CMD_S_LookCard
{
  WORD                wLookCardUser;            //看牌用户
  BYTE                cbCardData[MAX_COUNT];        //用户扑克
};

//发送扑克
struct CMD_S_SendCard
{
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //用户扑克
};

//开牌数据包
struct CMD_S_OpenCard
{
  WORD                wWinner;              //胜利用户
};

//游戏结束
struct CMD_S_GameEnd
{
  LONGLONG              lGameTax;             //游戏税收
  LONGLONG              lGameScore[GAME_PLAYER];      //游戏得分
  BYTE                cbCardData[GAME_PLAYER][3];     //用户扑克
  WORD                wCompareUser[GAME_PLAYER][4];   //比牌用户
  WORD                wEndState;              //结束状态
  bool                bDelayOverGame;           //延迟开始
  WORD                wServerType;            //房间类型
};

//用户退出
struct CMD_S_PlayerExit
{
  WORD                wPlayerID;              //退出用户
};

//等待比牌
struct CMD_S_WaitCompare
{
  WORD                wCompareUser;           //比牌用户
};

//////////////////////////////////////////////////////////////////////////

//客户端命令结构
#define SUB_C_ADD_SCORE         1                 //用户加注
#define SUB_C_GIVE_UP         2                 //放弃消息
#define SUB_C_COMPARE_CARD        3                 //比牌消息
#define SUB_C_LOOK_CARD         4                 //看牌消息
#define SUB_C_OPEN_CARD         5                 //开牌消息
#define SUB_C_WAIT_COMPARE        6                 //等待比牌
#define SUB_C_FINISH_FLASH        7                 //完成动画
#define SUB_C_ADD_SCORE_TIME      8                 //完成动画

#define SUB_C_MODIFY_STORAGE      10                  //修改库存
#define SUB_C_REQUEST_ADD_USERROSTER  11                  //请求添加用户名单
#define SUB_C_REQUEST_DELETE_USERROSTER 12                  //请求删除用户名单
#define SUB_C_REQUEST_UPDATE_USERROSTER 13                  //请求更新用户名单

//添加名单
struct CMD_C_Request_Add_Userroster
{
  DWORD           dwUserGameID;
  LONGLONG          lScoreLimit;
  bool            bWin;
};

//删除名单
struct CMD_C_Request_Delete_Userroster
{
  DWORD           dwUserGameID;
};

struct CMD_C_ModifyStorage
{
  LONGLONG            lStorageCurrent;
  LONGLONG            lStorageDeduct;
  LONGLONG            lMaxStorage;            //库存上限
  WORD              wStorageMul;            //赢分概率
};

//用户加注
struct CMD_C_AddScore
{
  LONGLONG              lScore;               //加注数目
  WORD                wState;               //当前状态
};

//比牌数据包
struct CMD_C_CompareCard
{
  WORD                wCompareUser;           //比牌用户
};

//////////////////////////////////////////////////////////////////////////


#pragma pack()

#endif
