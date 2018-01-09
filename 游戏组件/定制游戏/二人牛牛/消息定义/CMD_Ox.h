#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID             102                 //游戏 I D
#define GAME_PLAYER           2                 //游戏人数
#define GAME_NAME           TEXT("二人牛牛")          //游戏名字
#define MAX_COUNT           5                 //最大数目
#define MAX_JETTON_AREA         4                 //下注区域
#define MAX_TIMES           5                 //最大赔率

#define VERSION_SERVER          PROCESS_VERSION(7,0,1)        //程序版本
#define VERSION_CLIENT          PROCESS_VERSION(7,0,1)        //程序版本

//结束原因
#define GER_NO_PLAYER         0x10                //没有玩家

//游戏状态
#define GS_TK_FREE            GAME_STATUS_FREE          //等待开始
#define GS_TK_CALL            GAME_STATUS_PLAY          //叫庄状态
#define GS_TK_SCORE           GAME_STATUS_PLAY+1          //下注状态
#define GS_TK_PLAYING         GAME_STATUS_PLAY+2          //游戏进行


#define IDM_ADMIN_MODIFY_STORAGE          WM_USER+1011
#define IDM_ADMIN_REQUEST_ADD_USERROSTER      WM_USER+1012
#define IDM_ADMIN_REQUEST_DELETE_USERROSTER     WM_USER+1013
#define IDM_REQUEST_UPDATE_USERROSTER       WM_USER+1014

#define SERVER_LEN            32

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
#define SUB_S_PLAYER_EXIT       102                 //用户强退
#define SUB_S_SEND_CARD         103                 //发牌消息
#define SUB_S_GAME_END          104                 //游戏结束
#define SUB_S_OPEN_CARD         105                 //用户摊牌
#define SUB_S_CALL_BANKER       106                 //用户叫庄
#define SUB_S_ALL_CARD          107                 //发牌消息
#define SUB_S_ANDROID_BANKOPERATOR    109                 //机器人存取款
#define SUB_S_ADMIN_STORAGE_INFO    110                 //刷新库存
#define SUB_S_RESULT_ADD_USERROSTER   111                 //添加用户名单结果
#define SUB_S_RESULT_DELETE_USERROSTER  112                 //删除用户名单结果
#define SUB_S_UPDATE_USERROSTER     113                 //更新用户名单
#define SUB_S_REMOVEKEY_USERROSTER    114                 //移除用户名单
#define SUB_S_DUPLICATE_USERROSTER    115                 //重复用户名单

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

struct tagRobotConfig
{
  LONGLONG            lRobotScoreMin;
  LONGLONG            lRobotScoreMax;
  LONGLONG            lRobotBankGet;
  LONGLONG            lRobotBankGetBanker;
  LONGLONG            lRobotBankStoMul;
};

//控制服务端信息
struct CMD_S_ADMIN_STORAGE_INFO
{
  LONGLONG  lCurrentStorage;
  LONGLONG  lCurrentDeduct;
  LONGLONG  lMaxStorage;
  WORD    wStorageMul;
};

//游戏状态
struct CMD_S_StatusFree
{
  LONGLONG              lCellScore;             //基础积分

  //历史积分
  LONGLONG              lTurnScore[GAME_PLAYER];      //积分信息
  LONGLONG              lCollectScore[GAME_PLAYER];     //积分信息
  TCHAR               szGameRoomName[SERVER_LEN];     //房间名称
  tagRobotConfig            RobotConfig;            //机器人配置
  LONGLONG              lStartStorage;            //起始库存
  LONGLONG              lBonus;
};

//游戏状态
struct CMD_S_StatusCall
{
  WORD                wCallBanker;            //叫庄用户
  BYTE                                cbDynamicJoin;                      //动态加入
  BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态

  //历史积分
  LONGLONG              lTurnScore[GAME_PLAYER];      //积分信息
  LONGLONG              lCollectScore[GAME_PLAYER];     //积分信息
  TCHAR               szGameRoomName[SERVER_LEN];     //房间名称
  tagRobotConfig            RobotConfig;            //机器人配置
  LONGLONG              lStartStorage;            //起始库存
  LONGLONG              lBonus;
};

//游戏状态
struct CMD_S_StatusScore
{
  //下注信息
  BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
  BYTE                                cbDynamicJoin;                      //动态加入
  LONGLONG              lTurnMaxScore;            //最大下注
  LONGLONG              lTableScore[GAME_PLAYER];     //下注数目
  WORD                wBankerUser;            //庄家用户
  TCHAR               szGameRoomName[SERVER_LEN];     //房间名称

  //历史积分
  LONGLONG              lTurnScore[GAME_PLAYER];      //积分信息
  LONGLONG              lCollectScore[GAME_PLAYER];     //积分信息
  tagRobotConfig            RobotConfig;            //机器人配置
  LONGLONG              lStartStorage;            //起始库存
  LONGLONG              lBonus;
};

//游戏状态
struct CMD_S_StatusPlay
{
  //状态信息
  BYTE                                cbPlayStatus[GAME_PLAYER];          //用户状态
  BYTE                                cbDynamicJoin;                      //动态加入
  LONGLONG              lTurnMaxScore;            //最大下注
  LONGLONG              lTableScore[GAME_PLAYER];     //下注数目
  WORD                wBankerUser;            //庄家用户

  //扑克信息
  BYTE                cbHandCardData[GAME_PLAYER][MAX_COUNT];//桌面扑克
  BYTE                bOxCard[GAME_PLAYER];       //牛牛数据

  //历史积分
  LONGLONG              lTurnScore[GAME_PLAYER];      //积分信息
  LONGLONG              lCollectScore[GAME_PLAYER];     //积分信息
  TCHAR               szGameRoomName[SERVER_LEN];     //房间名称
  tagRobotConfig            RobotConfig;            //机器人配置
  LONGLONG              lStartStorage;            //起始库存
  LONGLONG              lBonus;
};

//用户叫庄
struct CMD_S_CallBanker
{
  WORD                wCallBanker;            //叫庄用户
  bool                bFirstTimes;            //首次叫庄
};

//游戏开始
struct CMD_S_GameStart
{
  //下注信息
  LONGLONG              lTurnMaxScore;            //最大下注
  WORD                wBankerUser;            //庄家用户
};

//用户下注
struct CMD_S_AddScore
{
  WORD                wAddScoreUser;            //加注用户
  LONGLONG              lAddScoreCount;           //加注数目
};

//游戏结束
struct CMD_S_GameEnd
{
  LONGLONG              lGameTax[GAME_PLAYER];        //游戏税收
  LONGLONG              lGameScore[GAME_PLAYER];      //游戏得分
  BYTE                cbCardData[GAME_PLAYER];      //用户扑克
  bool                bfiveKing[GAME_PLAYER];       //五花牛标识
};

//发牌数据包
struct CMD_S_SendCard
{
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //用户扑克
  bool                                bAllAndroidUser;          //全部机器
};

//发牌数据包
struct CMD_S_AllCard
{
  bool                bAICount[GAME_PLAYER];
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //用户扑克
};

//用户退出
struct CMD_S_PlayerExit
{
  WORD                wPlayerID;              //退出用户
};

//用户摊牌
struct CMD_S_Open_Card
{
  WORD                wPlayerID;              //摊牌用户
  BYTE                bOpen;                //摊牌标志
};
//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER       1                 //用户叫庄
#define SUB_C_ADD_SCORE         2                 //用户加注
#define SUB_C_OPEN_CARD         3                 //用户摊牌
#define SUB_C_SPECIAL_CLIENT_REPORT     4                                   //特殊终端
#define SUB_C_MODIFY_STORAGE      6                 //修改库存
#define SUB_C_REQUEST_ADD_USERROSTER  7                 //请求添加用户名单
#define SUB_C_REQUEST_DELETE_USERROSTER 8                 //请求删除用户名单
#define SUB_C_REQUEST_UPDATE_USERROSTER 9                 //请求更新用户名单

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

//用户叫庄
struct CMD_C_CallBanker
{
  BYTE                bBanker;              //做庄标志
};

//终端类型
struct CMD_C_SPECIAL_CLIENT_REPORT
{
  WORD                                wUserChairID;                       //用户方位
};

//用户加注
struct CMD_C_AddScore
{
  LONGLONG              lScore;               //加注数目
};

//用户摊牌
struct CMD_C_OxCard
{
  BYTE                bOX;                //牛牛标志
};

struct CMD_C_ModifyStorage
{
  LONGLONG            lStorageCurrent;
  LONGLONG            lStorageDeduct;
  LONGLONG            lMaxStorage;            //库存上限
  WORD              wStorageMul;            //赢分概率
};


#endif
