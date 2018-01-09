#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//������
#define LEN_SERVER					32

//��Ϸ����
#define KIND_ID						25									//��Ϸ I D
#define GAME_NAME					TEXT("��ʽ����")						//��Ϸ����

//�������
#define GAME_PLAYER					5									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//////////////////////////////////////////////////////////////////////////

//����ԭ��
#define GER_NO_PLAYER				0x10								//û�����

//״̬����
#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//��Ϸ����


//////////////////////////////////////////////////////////////////////////
//��ť��ʶ 

//���ư�ť
#define IDC_START						100								//��ʼ��ť
#define IDC_ADD_SCORE_ONE				101
#define IDC_ADD_SCORE_TWO				102
#define IDC_ADD_SCORE_THREE				103
#define IDC_ADD_SCORE_FOUR				104
#define IDC_ADD_SCORE					105
#define IDC_CONTROL_FOLLOW				106
#define IDC_CONTROL_GIVEUP				107 
#define IDC_CONTROL_SHOWHAND			108	
#define IDC_CONTROL_OK					109	

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_ADD_SCORE				101									//�û���ע
#define SUB_S_GIVE_UP				102									//�û�����
#define SUB_S_SEND_CARD				103									//�����˿�
#define SUB_S_GAME_END				104									//��Ϸ����
#define SUB_S_GET_WINNER			105									//��ȡ��Ϣ
#define SUB_S_TRUE_END				106									//������Ϣ

//����״̬
struct CMD_S_StatusFree
{
	//��Ϸ����
	LONG							lCellScore;							//��������

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ

	//��������Ϣ
	LONGLONG						lRobotScoreMin;					// ���ֵ���ȡ��
	LONGLONG						lRobotScoreMax;					// ���ָ��ڴ��
	LONGLONG						lRobotBankTake[2];				// ȡ����
	LONGLONG						lRobotBankSave;					// �����
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��������
	LONG							lServiceCharge;						//�����

	//��ע��Ϣ
	LONGLONG						lDrawMaxScore;						//�����ע
	LONGLONG						lTurnMaxScore;						//�����ע
	LONGLONG						lTurnLessScore;						//��С��ע
	LONGLONG						lUserScore[GAME_PLAYER];			//�û���ע
	LONGLONG						lTableScore[GAME_PLAYER];			//������ע

	//״̬��Ϣ
	BYTE							cbShowHand;							//�����־
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbHandCardData[GAME_PLAYER][5];		//�����˿�

	//��ʷ����
	LONGLONG						lTurnScore[GAME_PLAYER];			//������Ϣ
	LONGLONG						lCollectScore[GAME_PLAYER];			//������Ϣ

	//��������Ϣ
	LONGLONG						lRobotScoreMin;					// ���ֵ���ȡ��
	LONGLONG						lRobotScoreMax;					// ���ָ��ڴ��
	LONGLONG						lRobotBankTake[2];				// ȡ����
	LONGLONG						lRobotBankSave;					// �����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ��ע
	LONG							lServiceCharge;						//�����

	//��ע��Ϣ
	LONGLONG						lDrawMaxScore;						//�����ע
	LONGLONG						lTurnMaxScore;						//�����ע
	LONGLONG						lTurnLessScore;						//��С��ע

	//�û���Ϣ
	WORD				 			wCurrentUser;						//��ǰ���

	//�˿�����
	BYTE							cbObscureCard;						//�����˿�
	BYTE							cbCardData[GAME_PLAYER];			//�û��˿�
	BYTE							cbHandCardData[GAME_PLAYER][5];		//�û��˿�
};

//�û�����
struct CMD_S_GiveUp
{
	WORD							wGiveUpUser;						//�����û�
	WORD							wCurrentUser;						//��ǰ�û�
	LONGLONG						lDrawMaxScore;						//�����ע
	LONGLONG						lTrunMaxScore;						//�����ע
};

//�û���ע
struct CMD_S_AddScore
{
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wAddScoreUser;						//��ע�û�
	LONGLONG						lTurnLessScore;						//���ټ�ע
	LONGLONG						lUserScoreCount;					//��ע��Ŀ
};

//�����˿�
struct CMD_S_SendCard
{
	//��Ϸ��Ϣ
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wStartChairID;						//��ʼ�û�
	LONGLONG						lTurnMaxScore;						//�����ע

	//�˿���Ϣ
	BYTE							cbSendCardCount;					//������Ŀ
	BYTE							cbCardData[2][GAME_PLAYER];			//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbCardData[GAME_PLAYER];			//�û��˿�
	LONGLONG						lGameScore[GAME_PLAYER];			//��Ϸ����
};

//��ȡ��Ϣ
struct CMD_S_GetWinner
{
	WORD								wOrderCount;					//�����
	WORD								wChairOrder[GAME_PLAYER];		//�������
};

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_GIVE_UP				1									//�û�����
#define SUB_C_ADD_SCORE				2									//�û���ע
#define SUB_C_GET_WINNER			3									//��ȡ��Ϣ
#define SUB_C_ADD_SCORE_TIME		4									//��ɶ���

//�û���ע
struct CMD_C_AddScore
{
	LONGLONG						lScore;								//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

#pragma pack()

#endif