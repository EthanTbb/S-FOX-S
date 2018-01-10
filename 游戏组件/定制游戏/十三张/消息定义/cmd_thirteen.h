#ifndef CMD_THIRTEEN_HEAD_FILE
#define CMD_THIRTEEN_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#define KIND_ID							7									//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("ʮ����")					    //��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

#define VERSION_SERVER					PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT					PROCESS_VERSION(7,0,1)				//����汾

//��������
#define ME_VIEW_CHAIR					2									//�Լ���ͼ
#define HAND_CARD_COUNT					13									//�˿���Ŀ
#define DISPATCH_COUNT					52									//�ɷ���Ŀ

#define SET_CHIP_COUNT					4									//��ע����

//��Ϸ״̬
#define GS_WK_FREE				    	GAME_STATUS_FREE					//�ȴ���ʼ
#define GS_WK_CALL_BANKER				GAME_STATUS_PLAY					//��ׯ״̬
#define GS_WK_SETCHIP			    	GAME_STATUS_PLAY+1					//������ע
#define GS_WK_PLAYING			    	GAME_STATUS_PLAY+2					//��Ϸ����

//�ֶ�����
enum enSegmentType
{
	enSegFront,																//ǰ������
	enSegMid,																//�ж�����
	enSegBack,																//�������
	enSegErr																//��������
};

//�ֶ���Ϣ
struct tagSegInfo
{
	bool bHaveSeg[3];		//�Ƿ�ֶ�
	BYTE bSegData[3][5];	//�ֶ�����
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
		//ʵ�ʷ���
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
//����������ṹ
#define SUB_S_SETCHIP					201									//������ע
#define SUB_S_SETSEGMENT				202									//���÷ֶ�
#define SUB_S_SHOW_CARD					203									//���̯��
#define SUB_S_GAME_END					204									//��Ϸ����
#define SUB_S_COMPARE_CARD				205									//�Ƚ��˿�
#define SUB_S_GAME_START				206									//��Ϸ��ʼ
#define SUB_S_START_CALL_BANKER			207									//��ʼ��ׯ
#define SUB_S_CALL_BANKER				208									//��ҽ�ׯ
#define SUB_S_END_ALL_NO_CALL			209									//������ׯ
#define SUB_S_PLAYER_EXIT				210									//�û�ǿ��
#define SUB_S_DISPATCH_START			211									//��ʼ����
#define SUB_S_DISPATCH_FINISH			212									//���ƽ���
#define SUB_S_SPLIT_CARD				213									//������Ϣ
#define SUB_S_TRUSTEE					214									//�й���Ϣ
#define SUB_S_SEND_CARD_EX				215									//ȫ������
#define SUB_S_VOICE_PHRASE				216									//��������
#define SUB_S_CHEAT_CARD				217									//�����˿�
#define SUB_S_ROBOT_CONTROL             218                                 //�����˿���

//////////////////////////////////////////////////////////////////////////
//��ʼ��ׯ
struct CMD_S_StartCallBanker
{
	WORD								wCurrentCaller;						//��ǰ���
	BYTE								bUserCount;							//��Ϸ����
	bool								bGameStatus[GAME_PLAYER];			//��Ϸ״̬
};

//��ҽ�ׯ
struct CMD_S_CallBanker
{
	WORD								wLastCaller ;						//�ѽ����
	bool								bCallBanker ;						//��ׯ��ʶ
	WORD								wCurrentCaller;						//��ǰ���
	SCORE								lMaxChip;							//�����ע	(���˽�ׯ֮ǰΪ0)
	LONGLONG							lBankerScore;						//ׯ�ҽ��
	LONGLONG							lChipArray[GAME_PLAYER][SET_CHIP_COUNT];			//�����ע
} ;

//������ע
struct CMD_S_SetChip
{
	WORD								wChipUser;							//��ע���
	SCORE								lChip;								//��ע��С
};

//�������
struct CMD_S_SplitCard
{
	WORD								wLastUser;							//�������
};

//�����˿�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						//��ǰ���
	WORD								wBanker;							//ׯ�����
	BYTE								bCardData[HAND_CARD_COUNT];			//�����˿�
	BYTE								cbPlayCount;						//��Ϸ����
};

//���÷ֶ�
struct CMD_S_SetSegment
{
	WORD								wCurrentUser;						//��ǰ���
	enSegmentType						SegmentType;						//�ֶ�����
};

//���̯��
struct CMD_S_ShowCard
{
	WORD								wCurrentUser;						//��ǰ���
	bool								bCanSeeShowCard;					//�ܷ���
	BYTE								bFrontCard[3];						//ǰ���˿�
	BYTE								bMidCard[5];						//�ж��˿�
	BYTE								bBackCard[5];						//����˿�
};

//��ʼ����
struct CMD_S_CompareCard
{
	WORD								wBankUser;							//ׯ�����
	BYTE								bSegmentCard[GAME_PLAYER][3][5];	//�ֶ��˿�
	SCORE								lScoreTimes[GAME_PLAYER][3];		//�ֶα���
	BYTE								bSpecialType[GAME_PLAYER];			//��������
	bool								bCompared;							//�Ƿ����
	BYTE								cbPlayCount;						//��Ϸ����
};

//�û��й�
struct CMD_S_Trustee
{
	WORD								wChairID;							//�й��û�
	bool								bTrustee;							//�Ƿ��й�
};

struct CMD_S_ChoseYuYin
{
	BYTE								bChoseYuYin;						//����
	WORD								wChairID;							//��Ϸ�û�
	BYTE								bType;								//����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	SCORE								lGameTax;							//��Ϸ˰��
	SCORE								lGameScore[GAME_PLAYER];			//��Ϸ����
	SCORE								lScoreTimes[GAME_PLAYER];			//��Ӯ����
	SCORE								lCompareResult[GAME_PLAYER][3];		//���ƽ��
	BYTE								bEndMode;							//������ʽ
	bool								bDragonInfo[GAME_PLAYER];			//������Ϣ
	BYTE								bWinInfo[GAME_PLAYER];				//ʤ����Ϣ(0�� 1ƽ 2ʤ)
};

//�û��˳�
struct CMD_S_PlayerExit
{
	WORD						      	wPlayerID;							//�˳��û�
	SCORE								lScore[GAME_PLAYER];				//����
};

//����״̬
struct CMD_S_StatusFree
{	
	//��Ϸ����
	LONG								lBaseScore;							//��������
	bool								bGameStatus[GAME_PLAYER];			//��Ϸ״̬

	//��ʷ�ɼ�
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ

	//ʱ�䶨��
	BYTE								cbTimeStartGame;					//��ʼʱ��
	BYTE								cbTimeCallBanker;					//��ׯʱ��
	BYTE								cbTimeSetChip;						//��עʱ��
	BYTE								cbTimeRangeCard;					//����ʱ��
	BYTE								cbTimeShowCard;						//����ʱ��

	WORD								wServerID;							//�����ʶ
};

//��ׯ״̬
struct CMD_S_StatusCallBanker
{
	WORD								wCurrentCaller;						//��ǰ���
	bool								bGameStatus[GAME_PLAYER];			//��Ϸ״̬

	//��ʷ�ɼ�
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ

	//ʱ�䶨��
	BYTE								cbTimeStartGame;					//��ʼʱ��
	BYTE								cbTimeCallBanker;					//��ׯʱ��
	BYTE								cbTimeSetChip;						//��עʱ��
	BYTE								cbTimeRangeCard;					//����ʱ��
	BYTE								cbTimeShowCard;						//����ʱ��

	WORD								wServerID;							//�����ʶ
} ;

//��ע״̬
struct  CMD_S_StatusSetChip
{
	SCORE								lChip[GAME_PLAYER];					//��ע��С
	BYTE								bHandCardData[HAND_CARD_COUNT];		//�˿�����
	WORD								wBanker;							//ׯ�����
	bool								bGameStatus[GAME_PLAYER];			//��Ϸ״̬
	bool								bSetChipFinally;					//��ע�Ƿ����
	LONGLONG							lChipArray[GAME_PLAYER][SET_CHIP_COUNT];			//�����ע

	//��ʷ�ɼ�
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ

	//ʱ�䶨��
	BYTE								cbTimeStartGame;					//��ʼʱ��
	BYTE								cbTimeCallBanker;					//��ׯʱ��
	BYTE								cbTimeSetChip;						//��עʱ��
	BYTE								cbTimeRangeCard;					//����ʱ��
	BYTE								cbTimeShowCard;						//����ʱ��

	WORD								wServerID;							//�����ʶ
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONGLONG							lChipArray[GAME_PLAYER][SET_CHIP_COUNT];			//�����ע
	SCORE								lChip[GAME_PLAYER];					//��ע��С
	BYTE								bHandCardData[HAND_CARD_COUNT];		//�˿�����
	BYTE								bSegmentCard[GAME_PLAYER][3][5];	//�ֶ��˿�
	bool								bFinishSegment[GAME_PLAYER];		//��ɷֶ�
	WORD								wBanker;							//ׯ�����
	bool								bGameStatus[GAME_PLAYER];			//��Ϸ״̬
	SCORE								lScoreTimes[GAME_PLAYER][3];		//�ֶα���
	BYTE								bSpecialType[GAME_PLAYER];			//��������
	bool								bCompared;							//�Ƿ����
	BYTE								cbPlayCount;						//��Ϸ����
	//��ʷ�ɼ�
	SCORE								lTurnScore[GAME_PLAYER];			//������Ϣ
	SCORE								lCollectScore[GAME_PLAYER];			//������Ϣ

	//ʱ�䶨��
	BYTE								cbTimeStartGame;					//��ʼʱ��
	BYTE								cbTimeCallBanker;					//��ׯʱ��
	BYTE								cbTimeSetChip;						//��עʱ��
	BYTE								cbTimeRangeCard;					//����ʱ��
	BYTE								cbTimeShowCard;						//����ʱ��

	WORD								wServerID;							//�����ʶ
};

//�����˿�
struct CMD_S_CheatCard
{
	WORD							wCardUser[GAME_PLAYER ];				//�������
	BYTE							cbUserCount;							//��������
	BYTE							cbCardData[GAME_PLAYER ][HAND_CARD_COUNT];	//�˿��б�
	BYTE							cbCardCount[GAME_PLAYER ];				//�˿�����

};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_CALL_BANKER				300									//��ҽ�ׯ
#define SUB_C_NO_CALL_BANKER			301									//��Ҳ���
#define SUB_C_SETCHIP					302									//ѹע����
#define SUB_C_SEGCARD					303									//�ֶ�����
#define SUB_C_SHOWARD					304									//���̯��
#define SUB_C_COMPLETE_COMPARE			306									//��ɱȽ�
#define SUB_C_SHUFFLE_FINISH			307									//ϴ�ƽ���
#define SUB_C_SPLIT_CARD				308									//�������
#define SUB_C_SPLIT_FINISH				309									//���ƽ���
#define SUB_C_TRUSTEE					310									//�й���Ϣ
#define SUB_C_VOICE_PHRASE				311									//��������

//����ѹע
struct CMD_C_SetChip
{
	SCORE								nChip;								//ѹע��С
};

//���÷ֶ�
struct CMD_C_SegCard
{
	enSegmentType						SegmentType;						//�ֶ�����
};

//�ֶ���Ϣ
struct CMD_C_ShowCard
{
	BYTE								bFrontCard[3];						//ǰ���˿�
	BYTE								bMidCard[5];						//�ж��˿�
	BYTE								bBackCard[5];						//����˿�
};

//�����й�
struct CMD_C_Trustee
{
	bool								bTrustee;							//�Ƿ��й�
};

//ѡ�����
struct CMD_C_ChoseYuYin
{
	BYTE								bYuYinIndex;						//��������
	BYTE								bType;								//��������
	WORD								wChairID;							//��Ϸ�û�
};
//////////////////////////////////////////////////////////////////////////
///�����˿�����Ϣ
//������Ϣ
struct CMD_A_Control
{
	BOOL  bIsWin;                                        //�������Ƿ���ҪӮǮ
	BYTE  bPlayCount;                                    //�������
};
//////////////////////////////////////////////////////////////////////////
#endif
