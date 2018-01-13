#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID           123                 //游戏 I D
#define GAME_PLAYER         100                 //游戏人数
#define GAME_NAME         TEXT("飞禽走兽")          //游戏名字

//状态定义
#define GS_PLACE_JETTON       GAME_STATUS_PLAY          //下注状态
#define GS_GAME_END         GAME_STATUS_PLAY+1          //结束状态
#define GS_MOVECARD_END       GAME_STATUS_PLAY+2          //结束状态

#define VERSION_SERVER          PROCESS_VERSION(7,0,1)        //程序版本
#define VERSION_CLIENT        PROCESS_VERSION(7,0,1)        //程序版本

//燕子X6  鸽子X8  孔雀X8  老鹰X12 狮子X12 熊猫X8  猴子X8  兔子X6 鲨鱼24 飞禽2 走兽2

//区域索引
#define ID_TU_ZI          1                 //兔子
#define ID_YAN_ZI         2                 //燕子
#define ID_GE_ZI          3                 //鸽子
#define ID_KONG_QUE         4                 //孔雀
#define ID_LAO_YING         5                 //老鹰
#define ID_SHI_ZI         6                 //狮子
#define ID_XIONG_MAO        7                 //熊猫
#define ID_HOU_ZI         8                 //猴子
#define ID_SHA_YU         9                 //鲨鱼
#define ID_FEI_QIN          10                  //飞禽    //通赔
#define ID_ZOU_SHOU         11                  //走兽    //通杀
#define ID_GLOD_SHA         12                  //金鲨    //金鲨
//#define ID_TONG_SHA         13                  //通杀
//#define ID_TONG_PEI         14                  //通赔


//燕子 //鸽子 //孔雀 //老鹰 //狮子 //熊猫 //猴子 //兔子 //鲨鱼 //飞禽 //走兽

#define AREA_YAN_ZI                    0
#define AREA_GE_ZI                   1
#define AREA_KONG_QUE                    2
#define AREA_LAO_YING                    3
#define AREA_SHI_ZI                    4
#define AREA_XIONG_MAO                   5
#define AREA_HOU_ZI                    6
#define AREA_TU_ZI                    7
#define AREA_SHA_YU                    8
#define AREA_FEI_QIN                    9
#define AREA_ZOU_SHOU                    10


#define AREA_COUNT          12                  //区域数目

#define AREA_ALL          12                  //动物种类

#define ANIMAL_COUNT                28                                  //动物个数
//赔率定义
#define RATE_TWO_PAIR       12                  //对子赔率

#define SERVER_LEN          32                  //房间长度

#ifndef _UNICODE
#define myprintf  _snprintf
#define mystrcpy  strcpy
#define mystrlen  strlen
#define myscanf   _snscanf
#define myLPSTR   LPCSTR
#else
#define myprintf  swprintf
#define mystrcpy  wcscpy
#define mystrlen  wcslen
#define myscanf   _snwscanf
#define myLPSTR   LPWSTR
#endif

#define TEST_OUT(num)   CString str;str.Format(L"this is:%d",num);AfxMessageBox(str);
//机器人信息
struct tagRobotInfo
{
  int nChip[5];                           //筹码定义
  int nAreaChance[AREA_COUNT];                    //区域几率
  TCHAR szCfgFileName[MAX_PATH];                    //配置文件
  int nMaxTime;                           //最大赔率

  tagRobotInfo()
  {
    int nTmpChip[6] = {100,1000,10000,100000,1000000,5000000};
    int nTmpAreaChance[AREA_COUNT] = {2, 2, 2, 2, 2, 2, 2, 2,2,2,2};
    TCHAR szTmpCfgFileName[MAX_PATH] = _T("AnimalBattleConfig.ini");

    nMaxTime = 10;
    memcpy(nChip, nTmpChip, sizeof(nChip));
    memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
    memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
  }
};

//记录信息
struct tagServerGameRecord
{
  BYTE              bWinMen[AREA_COUNT+1];            //顺门胜利
};

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_FREE       99                  //游戏空闲
#define SUB_S_GAME_START      100                 //游戏开始
#define SUB_S_PLACE_JETTON      101                 //用户下注
#define SUB_S_GAME_END        102                 //游戏结束
#define SUB_S_APPLY_BANKER      103                 //申请庄家
#define SUB_S_CHANGE_BANKER     104                 //切换庄家
#define SUB_S_CHANGE_USER_SCORE   105                 //更新积分
#define SUB_S_SEND_RECORD     106                 //游戏记录
#define SUB_S_PLACE_JETTON_FAIL   107                 //下注失败
#define SUB_S_CANCEL_BANKER     108                 //取消申请
//#define SUB_S_CHECK_IMAGE     109                 //取消申请
#define SUB_S_ADMIN_COMMDN      110                 //系统控制
#define SUB_S_CEAN_JETTON     111                 //清楚下注
#define SUB_S_CONTINU_JETTON    112                 //继续下注
#define SUB_C_UPDATE_STORAGE        113                 //更新库存

#define SUB_S_UPDATE_STORAGE        114                 //更新库存
//库存控制
#define RQ_REFRESH_STORAGE    1
#define RQ_SET_STORAGE      2
//客户端消息
#define IDM_ADMIN_COMMDN      WM_USER+1000
#define IDM_UPDATE_STORAGE      WM_USER+1001

struct tagCustomAndroid
{
  //坐庄
  BOOL              nEnableRobotBanker;       //是否做庄
  LONGLONG            lRobotBankerCountMin;     //坐庄次数
  LONGLONG            lRobotBankerCountMax;     //坐庄次数
  LONGLONG            lRobotListMinCount;       //列表人数
  LONGLONG            lRobotListMaxCount;       //列表人数
  LONGLONG            lRobotApplyBanker;        //最多申请个数
  LONGLONG            lRobotWaitBanker;       //空盘重申

  //下注
  LONGLONG            lRobotMinBetTime;       //下注筹码个数
  LONGLONG            lRobotMaxBetTime;       //下注筹码个数
  LONGLONG            lRobotMinJetton;        //下注筹码金额
  LONGLONG            lRobotMaxJetton;        //下注筹码金额
  LONGLONG            lRobotBetMinCount;        //下注机器人数
  LONGLONG            lRobotBetMaxCount;        //下注机器人数
  LONGLONG            lRobotAreaLimit;        //区域限制

  //存取款
  LONGLONG            lRobotScoreMin;         //金币下限
  LONGLONG            lRobotScoreMax;         //金币上限
  LONGLONG            lRobotBankGetMin;       //取款最小值(非庄)
  LONGLONG            lRobotBankGetMax;       //取款最大值(非庄)
  LONGLONG            lRobotBankGetBankerMin;     //取款最小值(坐庄)
  LONGLONG            lRobotBankGetBankerMax;     //取款最大值(坐庄)
  LONGLONG            lRobotBankStoMul;       //存款百分比

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
//更新库存
struct CMD_C_UpdateStorage
{
  BYTE                            cbReqType;            //请求类型
  LONGLONG            lStorageDeduct;         //库存衰减
  LONGLONG            lStorageCurrent;        //当前库存
  LONGLONG            lCurrentBonus;          //当前彩金池
  LONGLONG            lStorageMax1;         //库存上限1
  LONGLONG            lStorageMul1;         //系统输分概率1
  LONGLONG            lStorageMax2;         //库存上限2
  LONGLONG            lStorageMul2;         //系统输分概率2
};
//清楚下注
struct CMD_S_CeanJetton
{
  LONGLONG            lAllCPlaceScore[AREA_COUNT+1];    //当前下注
  WORD              wChairID;             //用户位置
};

struct CMD_S_ContiueJetton
{
  LONGLONG            lAllJettonScore[AREA_COUNT+1];          //全体总注
  LONGLONG            lUserJettonScore[AREA_COUNT+1];         //个人总注


  WORD              wChairID;             //用户位置
  BYTE              cbAndroid;              //机器人
};
//失败结构
struct CMD_S_PlaceJettonFail
{
  WORD              wPlaceUser;             //下注玩家
  BYTE              lJettonArea;            //下注区域
  LONGLONG            lPlaceScore;            //当前下注
};

//更新积分
struct CMD_S_ChangeUserScore
{
  WORD              wChairID;             //椅子号码
  DOUBLE              lScore;               //玩家积分

  //庄家信息
  WORD              wCurrentBankerChairID;        //当前庄家
  BYTE              cbBankerTime;           //庄家局数
  DOUBLE              lCurrentBankerScore;        //庄家分数
};

//申请庄家
struct CMD_S_ApplyBanker
{
  WORD              wApplyUser;             //申请玩家
};

//取消申请
struct CMD_S_CancelBanker
{
  TCHAR             szCancelUser[32];         //取消玩家
};

//切换庄家
struct CMD_S_ChangeBanker
{
  WORD              wBankerUser;            //当庄玩家
  LONGLONG            lBankerScore;           //庄家金币
};

//游戏状态
struct CMD_S_StatusFree
{
  LONGLONG            lStorageStart;            //库存数值
  LONGLONG            lBonus;               //彩金池

  //全局信息
  BYTE              cbTimeLeave;            //剩余时间

  //玩家信息
  LONGLONG            lUserMaxScore;            //玩家金币
  INT               nAnimalPercent[AREA_ALL];     //开中比例
  //庄家信息
  WORD              wBankerUser;            //当前庄家
  WORD              cbBankerTime;           //庄家局数
  LONGLONG            lBankerWinScore;          //庄家成绩
  LONGLONG            lBankerScore;           //庄家分数
  bool              bEnableSysBanker;         //系统做庄

  //控制信息
  LONGLONG            lApplyBankerCondition;        //申请条件
  LONGLONG            lAreaLimitScore;          //区域限制

  TCHAR             szGameRoomName[32];         //房间名称
  tagCustomAndroid        CustomAndroid;            //机器人配置

};
//下注信息
struct tagUserBet
{
  TCHAR             szNickName[32];           //用户昵称
  DWORD             dwUserGameID;           //用户ID
  LONGLONG            lUserStartScore;          //用户金币
  LONGLONG            lUserWinLost;           //用户金币
  LONGLONG            lUserBet[AREA_COUNT];       //用户下注
};
//下注信息数组
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;
//游戏状态
struct CMD_S_StatusPlay
{
  //全局下注
  LONGLONG            lAllJettonScore[AREA_COUNT+1];    //全体总注

  //玩家下注
  LONGLONG            lUserJettonScore[AREA_COUNT+1];   //个人总注

  //玩家积分
  LONGLONG            lUserMaxScore;            //最大下注
  INT               nAnimalPercent[AREA_ALL];     //开中比例
  //控制信息
  LONGLONG            lApplyBankerCondition;        //申请条件
  LONGLONG            lAreaLimitScore;          //区域限制

  //扑克信息
  BYTE              cbTableCardArray[2];        //桌面扑克

  //庄家信息
  WORD              wBankerUser;            //当前庄家
  WORD              cbBankerTime;           //庄家局数
  LONGLONG            lBankerWinScore;          //庄家赢分
  LONGLONG            lBankerScore;           //庄家分数
  bool              bEnableSysBanker;         //系统做庄

  //结束信息
  LONGLONG            lEndBankerScore;          //庄家成绩
  LONGLONG            lEndUserScore;            //玩家成绩
  LONGLONG            lEndUserReturnScore;        //返回积分
  LONGLONG            lEndRevenue;            //游戏税收

  //全局信息
  BYTE              cbTimeLeave;            //剩余时间
  BYTE              cbGameStatus;           //游戏状态
  TCHAR             szGameRoomName[32];         //房间名称

  LONGLONG            lStorageStart;            //库存数值
  LONGLONG            lBonus;               //彩金池
  tagCustomAndroid        CustomAndroid;            //机器人配置

};

//游戏空闲
struct CMD_S_GameFree
{
  BYTE              cbTimeLeave;            //剩余时间
  LONGLONG            lStorageControl;          //库存数值
  LONGLONG            lStorageStart;            //彩池数值
  LONGLONG            lBonus;               //彩金池
};

//游戏开始
struct CMD_S_GameStart
{
  WORD              wBankerUser;            //庄家位置
  LONGLONG            lBankerScore;           //庄家金币
  LONGLONG            lUserMaxScore;            //我的金币
  BYTE              cbTimeLeave;            //剩余时间
  bool              bContiueCard;           //继续发牌
  int               nChipRobotCount;          //人数上限 (下注机器人)
  LONGLONG            lStorageStart;            //库存数值
  LONGLONG            lBonus;               //彩金池
};

//用户下注
struct CMD_S_PlaceJetton
{
  WORD              wChairID;             //用户位置
  BYTE              cbJettonArea;           //筹码区域
  LONGLONG            lJettonScore;           //加注数目
  BYTE              cbAndroid;              //机器人
  LONGLONG            lUserStartScore[GAME_PLAYER];       //起始分数
  //玩家下注
  LONGLONG            lUserJettonScore[AREA_COUNT+1];   //个人总注
};

//游戏结束
struct CMD_S_GameEnd
{
  //下局信息
  BYTE              cbTimeLeave;            //剩余时间

  //扑克信息
  BYTE              cbTableCardArray[2];        //桌面扑克
  BYTE                            cbShaYuAddMulti;          //附加倍率
  //庄家信息
  LONGLONG            lBankerScore;           //庄家成绩
  LONGLONG            lBankerTotallScore;         //庄家成绩
  INT               nBankerTime;            //做庄次数

  //玩家成绩
  LONGLONG            lUserScore;             //玩家成绩
  LONGLONG            lUserReturnScore;         //返回积分

  //全局信息
  LONGLONG            lRevenue;             //游戏税收
  INT               nAnimalPercent[AREA_ALL];     //开中比例

  LONGLONG            lStorageStart;            //库存数值
  LONGLONG            lLastJetton[AREA_COUNT+1];      //上局下注
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_PLACE_JETTON      11                  //用户下注
#define SUB_C_APPLY_BANKER      12                  //申请庄家
#define SUB_C_CANCEL_BANKER     13                  //取消申请

#define SUB_C_ADMIN_COMMDN      16                  //系统控制

#define SUB_C_CLEAN_JETTON      157                 //清零命令
#define SUB_C_CONTINUE_JETTON   158                 //继续下注

//用户下注
struct CMD_C_PlaceJetton
{
  BYTE              cbJettonArea;           //筹码区域
  LONGLONG            lJettonScore;           //加注数目
};

struct CMD_C_CleanMeJetto
{
  WORD              wChairID;             //用户位置
};

struct CMD_C_ContinueJetton
{
  WORD          wChairID;             //用户位置
  LONGLONG        lLastAllJettonPlace[AREA_COUNT +1]; //全部筹码
  LONGLONG        lLastAllJettonArea[AREA_COUNT +1];  //区域筹码

  BYTE          cbJettonArea;           //筹码区域
  LONGLONG        lJettonScore;           //加注数目
  BYTE          cbAndroid;              //机器人
};
//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN WM_USER+1000

//控制区域信息
struct tagControlInfo
{
  BYTE cbControlArea1;            //控制区域
  BYTE cbControlArea2;            //控制区域
};



//更新库存
struct CMD_S_UpdateStorage
{
  BYTE                            cbReqType;            //请求类型
  LONGLONG            lStorageStart;          //起始库存
  LONGLONG            lStorageDeduct;         //库存衰减
  LONGLONG            lStorageCurrent;        //当前库存
  LONGLONG            lCurrentBonus;          //当前彩金池
  LONGLONG            lStorageMax1;         //库存上限1
  LONGLONG            lStorageMul1;         //系统输分概率1
  LONGLONG            lStorageMax2;         //库存上限2
  LONGLONG            lStorageMul2;         //系统输分概率2
};
//下注信息数组
//typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;

//机器人信息

//服务器控制返回
#define  S_CR_FAILURE       0   //失败
#define  S_CR_UPDATE_SUCCES     1   //更新成功
#define  S_CR_SET_SUCCESS     2   //设置成功
#define  S_CR_CANCEL_SUCCESS    3   //取消成功
#define  S_CR_ACK_PRINT_STORAGE   4   //设置库存
struct CMD_S_ControlReturns
{
  BYTE cbReturnsType;       //回复类型
  BYTE cbControlStyle;      //控制方式
  BYTE cbControlArea;       //控制区域
  BYTE cbControlTimes;      //控制次数
  LONGLONG lCurStorage;     //当前库存
};


//申请类型
#define  C_CA_UPDATE        1   //更新
#define  C_CA_SET         2   //设置
#define  C_CA_CANCELS       3   //取消
#define  C_CA_SET_STORAGE     4   //库存
#define  C_CA_PRINT_STORAGE     5   //库存

//控制方式
#define   CS_BANKER_LOSE    1
#define   CS_BANKER_WIN   2
#define   CS_BET_AREA     3

struct CMD_C_ControlApplication
{
  BYTE cbControlAppType;      //申请类型
  BYTE cbControlStyle;      //控制方式
  BYTE cbControlArea;       //控制区域
  BYTE cbControlTimes;      //控制次数
  LONGLONG lSetStorage;     //设置库存
  LONGLONG lSetDeduct;      //设置库存衰减值
};

//////////////////////////////////////////////////////////////////////////

#endif
