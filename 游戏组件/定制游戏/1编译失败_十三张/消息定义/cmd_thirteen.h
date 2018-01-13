#ifndef CMD_THIRTEEN_HEAD_FILE
#define CMD_THIRTEEN_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//公共宏定义
#define KIND_ID							7									//游戏 I D
#define GAME_PLAYER						4									//游戏人数
#define GAME_NAME						TEXT("十三张")					    //游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//程序版本
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//程序版本

//常量数据
#define ME_VIEW_CHAIR					2									//自己视图
#define HAND_CARD_COUNT					13									//扑克数目
#define DISPATCH_COUNT					52									//派发数目

#define SET_CHIP_COUNT					4									//下注档次

//游戏状态
#define GS_WK_FREE				    	GAME_STATUS_FREE					//等待开始
#define GS_WK_CALL_BANKER				GAME_STATUS_PLAY					//叫庄状态
#define GS_WK_SETCHIP			    	GAME_STATUS_PLAY+1					//设置下注
#define GS_WK_PLAYING			    	GAME_STATUS_PLAY+2					//游戏进行

//分段类型
enum enSegmentType
{
	enSegFront,																//前墩类型
	enSegMid,																//中墩类型
	enSegBack,																//后墩类型
	enSegErr																//错误类型
};

//分段信息
struct tagSegInfo
{
	bool bHaveSeg[3];		//是否分段
	BYTE bSegData[3][5];	//分段牌组
	tagSegInfo() 
	{
		Init();
	}

	void Init()
	{ 
		ZeroMemory(bHaveSeg, sizeof(bHaveSeg)); 
		ZeroMemory(bSegData, sizeof(bSegData)); 
	}

	void SetSegData(BYTE bSegNum, BYTE bSegCard[], BYTE bSegCount)
	{
		if (bSegNum < 0 || bSegNum > 3 || (bSegCount!=3&&bSegCount!=5))		return;
		if (bSegCard == NULL)
		{
			bHaveSeg[bSegNum] = false;
			ZeroMemory(bSegData[bSegNum], sizeof(bSegData[bSegNum]));
			return;
		}
		ZeroMemory(bSegData[bSegNum], sizeof(bSegData[bSegNum]));
		bHaveSeg[bSegNum] = true;
		CopyMemory(bSegData[bSegNum], bSegCard, sizeof(BYTE)*bSegCount); 
	}

	BYTE GetSegData(BYTE bSegNum, BYTE bSegCard[], BYTE bSegCount)
	{
		if (bSegNum < 0 || bSegNum > 3 || (bSegCount!=3&&bSegCount!=5) || bSegCard == NULL)		return 0;
		if (!bHaveSeg[bSegNum])		return 0;
		CopyMemory(bSegCard, bSegData[bSegNum], sizeof(BYTE)*bSegCount);
		//实际返回
		BYTE bRealCount = bSegCount;
		for (int i = 0; i < bSegCount; i++)
		{
			if (bSegData[i] == 0)
				bRealCount--;
		}
		return bRealCount;
	}

	BYTE GetCardCount(BYTE bSegNum = 255)
	{
		BYTE bCCount = 0;
		for (int i = 0; i < 3; i++)
		{
			if (bSegNum == i || bSegNum == 255)
			{
				for (int j = 0; j < 5; j++)
				{
					if (bSegData[i][j] != 0)
						bCCount++;
				}
			}
		}
		return bCCount;
	}
};

#ifndef _UNICODE
#define myprintf	_snprintf
#define mystrcpy	strcpy
#define mystrlen	strlen
#define myscanf		_snscanf
#define	myLPSTR		LPCSTR
#define myatoi      atoi
#define myatoi64    _atoi64
#else
#define myprintf	swprintf
#define mystrcpy	wcscpy
#define mystrlen	wcslen
#define myscanf		_snwscanf
#define	myLPSTR		LPWSTR
#define myatoi      _wtoi
#define myatoi64	_wtoi64
#endif

//////////////////////////////////////////////////////////////////////////
//服务器命令结构
#define SUB_S_SETCHIP					201									//设置下注
#define SUB_S_SETSEGMENT				202									//设置分段
#define SUB_S_SHOW_CARD					203									//玩家摊牌
#define SUB_S_GAME_END					204									//游戏结束
#define SUB_S_COMPARE_CARD				205									//比较扑克
#define SUB_S_GAME_START				206									//游戏开始
#define SUB_S_START_CALL_BANKER			207									//开始叫庄
#define SUB_S_CALL_BANKER				208									//玩家叫庄
#define SUB_S_END_ALL_NO_CALL			209									//都不叫庄
#define SUB_S_PLAYER_EXIT				210									//用户强退
#define SUB_S_DISPATCH_START			211									//开始发牌
#define SUB_S_DISPATCH_FINISH			212									//发牌结束
#define SUB_S_SPLIT_CARD				213									//切牌消息
#define SUB_S_TRUSTEE					214									//托管消息
#define SUB_S_SEND_CARD_EX				215									//全部发牌
#define SUB_S_VOICE_PHRASE				216									//语音短语
#define SUB_S_CHEAT_CARD				217									//作弊扑克
#define SUB_S_ROBOT_CONTROL             218                                 //机器人控制

//////////////////////////////////////////////////////////////////////////
//开始叫庄
struct CMD_S_StartCallBanker
{
	WORD								wCurrentCaller;						//当前玩家
	BYTE								bUserCount;							//游戏人数
	bool								bGameStatus[GAME_PLAYER];			//游戏状态
};

//玩家叫庄
struct CMD_S_CallBanker
{
	WORD								wLastCaller ;						//已叫玩家
	bool								bCallBanker ;						//叫庄标识
	WORD								wCurrentCaller;						//当前玩家
	SCORE								lMaxChip;							//最大下注	(有人叫庄之前为0)
	LONGLONG							lBankerScore;						//庄家金币
	LONGLONG							lChipArray[GAME_PLAYER][SET_CHIP_COUNT];			//玩家下注
} ;

//设置下注
struct CMD_S_SetChip
{
	WORD								wChipUser;							//下注玩家
	SCORE								lChip;								//下注大小
};

//玩家切牌
struct CMD_S_SplitCard
{
	WORD								wLastUser;							//切牌玩家
};

//发送扑克
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						//当前玩家
	WORD								wBanker;							//庄家玩家
	BYTE								bCardData[HAND_CARD_COUNT];			//手上扑克
	BYTE								cbPlayCount;						//游戏人数
};

//设置分段
struct CMD_S_SetSegment
{
	WORD								wCurrentUser;						//当前玩家
	enSegmentType						SegmentType;						//分段类型
};

//玩家摊牌
struct CMD_S_ShowCard
{
	WORD								wCurrentUser;						//当前玩家
	bool								bCanSeeShowCard;					//能否看牌
	BYTE								bFrontCard[3];						//前墩扑克
	BYTE								bMidCard[5];						//中墩扑克
	BYTE								bBackCard[5];						//后墩扑克
};

//开始比牌
struct CMD_S_CompareCard
{
	WORD								wBankUser;							//庄家玩家
	BYTE								bSegmentCard[GAME_PLAYER][3][5];	//分段扑克
	SCORE								lScoreTimes[GAME_PLAYER][3];		//分段倍数
	BYTE								bSpecialType[GAME_PLAYER];			//特殊牌型
	bool								bCompared;							//是否比牌
	BYTE								cbPlayCount;						//游戏人数
};

//用户托管
struct CMD_S_Trustee
{
	WORD								wChairID;							//托管用户
	bool								bTrustee;							//是否托管
};

struct CMD_S_ChoseYuYin
{
	BYTE								bChoseYuYin;						//语音
	WORD								wChairID;							//游戏用户
	BYTE								bType;								//类型
};

//游戏结束
struct CMD_S_GameEnd
{
	SCORE								lGameTax;							//游戏税收
	SCORE								lGameScore[GAME_PLAYER];			//游戏积分
	SCORE								lScoreTimes[GAME_PLAYER];			//输赢倍数
	SCORE								lCompareResult[GAME_PLAYER][3];		//比牌结果
	BYTE								bEndMode;							//结束方式
	bool								bDragonInfo[GAME_PLAYER];			//乌龙信息
	BYTE								bWinInfo[GAME_PLAYER];				//胜负信息(0负 1平 2胜)
};

//用户退出
struct CMD_S_PlayerExit
{
	WORD						      	wPlayerID;							//退出用户
	SCORE								lScore[GAME_PLAYER];				//分数
};

//空闲状态
struct CMD_S_StatusFree
{	
	//游戏属性
	LONG								lBaseScore;							//基础积分
	bool								bGameStatus[GAME_PLAYER];			//游戏状态

	//历史成绩
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息

	//时间定义
	BYTE								cbTimeStartGame;					//开始时间
	BYTE								cbTimeCallBanker;					//叫庄时间
	BYTE								cbTimeSetChip;						//下注时间
	BYTE								cbTimeRangeCard;					//理牌时间
	BYTE								cbTimeShowCard;						//开牌时间

	WORD								wServerID;							//房间标识
};

//叫庄状态
struct CMD_S_StatusCallBanker
{
	WORD								wCurrentCaller;						//当前玩家
	bool								bGameStatus[GAME_PLAYER];			//游戏状态

	//历史成绩
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息

	//时间定义
	BYTE								cbTimeStartGame;					//开始时间
	BYTE								cbTimeCallBanker;					//叫庄时间
	BYTE								cbTimeSetChip;						//下注时间
	BYTE								cbTimeRangeCard;					//理牌时间
	BYTE								cbTimeShowCard;						//开牌时间

	WORD								wServerID;							//房间标识
} ;

//下注状态
struct  CMD_S_StatusSetChip
{
	SCORE								lChip[GAME_PLAYER];					//下注大小
	BYTE								bHandCardData[HAND_CARD_COUNT];		//扑克数据
	WORD								wBanker;							//庄家玩家
	bool								bGameStatus[GAME_PLAYER];			//游戏状态
	bool								bSetChipFinally;					//下注是否完成
	LONGLONG							lChipArray[GAME_PLAYER][SET_CHIP_COUNT];			//玩家下注

	//历史成绩
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息

	//时间定义
	BYTE								cbTimeStartGame;					//开始时间
	BYTE								cbTimeCallBanker;					//叫庄时间
	BYTE								cbTimeSetChip;						//下注时间
	BYTE								cbTimeRangeCard;					//理牌时间
	BYTE								cbTimeShowCard;						//开牌时间

	WORD								wServerID;							//房间标识
};

//游戏状态
struct CMD_S_StatusPlay
{
	LONGLONG							lChipArray[GAME_PLAYER][SET_CHIP_COUNT];			//玩家下注
	SCORE								lChip[GAME_PLAYER];					//下注大小
	BYTE								bHandCardData[HAND_CARD_COUNT];		//扑克数据
	BYTE								bSegmentCard[GAME_PLAYER][3][5];	//分段扑克
	bool								bFinishSegment[GAME_PLAYER];		//完成分段
	WORD								wBanker;							//庄家玩家
	bool								bGameStatus[GAME_PLAYER];			//游戏状态
	SCORE								lScoreTimes[GAME_PLAYER][3];		//分段倍数
	BYTE								bSpecialType[GAME_PLAYER];			//特殊牌型
	bool								bCompared;							//是否比牌
	BYTE								cbPlayCount;						//游戏人数
	//历史成绩
	SCORE								lTurnScore[GAME_PLAYER];			//积分信息
	SCORE								lCollectScore[GAME_PLAYER];			//积分信息

	//时间定义
	BYTE								cbTimeStartGame;					//开始时间
	BYTE								cbTimeCallBanker;					//叫庄时间
	BYTE								cbTimeSetChip;						//下注时间
	BYTE								cbTimeRangeCard;					//理牌时间
	BYTE								cbTimeShowCard;						//开牌时间

	WORD								wServerID;							//房间标识
};

//作弊扑克
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//作弊玩家
	BYTE							cbUserCount;							//作弊数量
	BYTE							cbCardData[GAME_PLAYER ][HAND_CARD_COUNT];	//扑克列表
	BYTE							cbCardCount[GAME_PLAYER ];				//扑克数量

};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构
#define SUB_C_CALL_BANKER				300									//玩家叫庄
#define SUB_C_NO_CALL_BANKER			301									//玩家不叫
#define SUB_C_SETCHIP					302									//压注命令
#define SUB_C_SEGCARD					303									//分段命令
#define SUB_C_SHOWARD					304									//玩家摊牌
#define SUB_C_COMPLETE_COMPARE			306									//完成比较
#define SUB_C_SHUFFLE_FINISH			307									//洗牌结束
#define SUB_C_SPLIT_CARD				308									//玩家切牌
#define SUB_C_SPLIT_FINISH				309									//切牌结束
#define SUB_C_TRUSTEE					310									//托管消息
#define SUB_C_VOICE_PHRASE				311									//语音请求

//设置压注
struct CMD_C_SetChip
{
	SCORE								nChip;								//压注大小
};

//设置分段
struct CMD_C_SegCard
{
	enSegmentType						SegmentType;						//分段类型
};

//分段信息
struct CMD_C_ShowCard
{
	BYTE								bFrontCard[3];						//前墩扑克
	BYTE								bMidCard[5];						//中墩扑克
	BYTE								bBackCard[5];						//后墩扑克
};

//设置托管
struct CMD_C_Trustee
{
	bool								bTrustee;							//是否托管
};

//选择短语
struct CMD_C_ChoseYuYin
{
	BYTE								bYuYinIndex;						//语音索引
	BYTE								bType;								//语音类型
	WORD								wChairID;							//游戏用户
};
//////////////////////////////////////////////////////////////////////////
///机器人控制消息
//控制消息
struct CMD_A_Control
{
	BOOL  bIsWin;                                        //机器人是否需要赢钱
	BYTE  bPlayCount;                                    //玩家人数
};
//////////////////////////////////////////////////////////////////////////
#endif
