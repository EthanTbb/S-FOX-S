#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

#pragma pack(push)  
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						118									//��Ϸ I D
#define GAME_PLAYER					MAX_CHAIR									//��Ϸ����
#define GAME_NAME					TEXT("��������")					//��Ϸ����

//״̬����
#define GS_GAME_START				GAME_STATUS_PLAY					//��Ϸ��ʼ
#define	GS_PLAYER_BET				GAME_STATUS_PLAY+1					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+2					//����״̬
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+3					//����״̬


//�汾��Ϣ
#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//��������
#define ID_AREA_DRAGON				0									//��
#define ID_AREA_TIGER				1									//��
#define ID_AREA_LEOPARD				2									//��

//�������
#define DRAGON_INDEX				0									//��
#define TIGER_INDEX					1									//��
#define LEOPARD_INDEX				2									//��
#define BANKER_INDEX				3									//ׯ

//����
#define AREA_COUNT					52								//������Ŀ
#define CARD_COUNT					2									//ץ����Ŀ
#define DIRECT_COUNT				4									//��λ��Ŀ
#define ALL_CARD_COUNT				40									//�ƶ���Ŀ
#define MAX_ODDS					1									//�������
#define JETTON_COUNT				6									//��������
#define  DICE_COUNT					3									//���Ӹ���
//����ģ��

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

//������ֵ
static const LONG g_lScoreType[JETTON_COUNT]={500L,1000L,10000L,100000L,1000000L,5000000L};
//������
static const UINT g_nJettonIDI[JETTON_COUNT]={3000,3001,3002,3003,3004,3005};
//����λͼ
static const UINT g_nJettonIDB[JETTON_COUNT]={3100,3101,3102,3103,3104,3105};
//����IDC
static const UINT g_nJettonIDC[JETTON_COUNT]={3200,3201,3202,3203,3204,3205};

//���ʶ���
#define RATE_TWO_PAIR				12									//��������
#define SERVER_LEN					32									//���䳤��

//��������Ϣ
struct tagRobotInfo
{
	int nChip[JETTON_COUNT];											//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

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

//����������
struct tagCustomAndroid
{
	//��ׯ
	BOOL							nEnableRobotBanker;				//�Ƿ���ׯ
	LONGLONG						lRobotBankerCountMin;			//��ׯ����
	LONGLONG						lRobotBankerCountMax;			//��ׯ����
	LONGLONG						lRobotListMinCount;				//�б�����
	LONGLONG						lRobotListMaxCount;				//�б�����
	LONGLONG						lRobotApplyBanker;				//����������
	LONGLONG						lRobotWaitBanker;				//��������

	//��ע
	LONGLONG						lRobotMinBetTime;				//��ע�������
	LONGLONG						lRobotMaxBetTime;				//��ע�������
	LONGLONG						lRobotMinJetton;				//��ע������
	LONGLONG						lRobotMaxJetton;				//��ע������
	LONGLONG						lRobotBetMinCount;				//��ע��������
	LONGLONG						lRobotBetMaxCount;				//��ע��������
	LONGLONG						lRobotAreaLimit;				//��������

	//��ȡ��
	LONGLONG						lRobotScoreMin;					//�������
	LONGLONG						lRobotScoreMax;					//�������
	LONGLONG						lRobotBankGetMin;				//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetMax;				//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMin;			//ȡ����Сֵ(��ׯ)
	LONGLONG						lRobotBankGetBankerMax;			//ȡ�����ֵ(��ׯ)
	LONGLONG						lRobotBankStoMul;				//���ٷֱ�

	//���캯��
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

//����������
struct tagRobotConfig
{
	//��ׯ����
	SCORE lBankerCondition;											//��ׯ����

	//���в���
	SCORE lBankDrawCondition;										//ȡ������
	SCORE lBankDrawScore;											//ȡ������
	SCORE lBankSaveCondition;										//�������
	SCORE lBankSaveScore;											//�������

	//ׯ�Ҳ���
	SCORE lBankerDrawCount;											//ȡ����� (ׯ��)
	SCORE lBankerWinGiveUp;											//Ӯ����ׯ (ׯ��)
	SCORE lBankerLoseGiveUp;											//�����ׯ (ׯ��)

	//��ע����
	SCORE lJettonLimit[2];											//���뷶Χ
	SCORE lBetTimeLimit;												//��ע����

	//��������
	int		 nCfgRobotCount;											//��������
};

//��¼��Ϣ
struct tagGameRecord
{
	BYTE cbDiceValue[DICE_COUNT];
};

//��ע��Ϣ
struct tagUserBet
{
	TCHAR							szNickName[32];						//�û��ǳ�
	DWORD							dwUserGameID;						//�û�ID
	LONGLONG						lUserStartScore;					//�û����
	LONGLONG						lUserWinLost;						//�û����
	LONGLONG						lUserBet[AREA_COUNT];				//�û���ע
};

//��ע��Ϣ����
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

//������
#define RQ_REFRESH_STORAGE		1
#define RQ_SET_STORAGE			2
//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_START_BET				101
#define SUB_S_PLACE_JETTON			102									//�û���ע
#define SUB_S_GAME_END				103									//��Ϸ����
#define SUB_S_APPLY_BANKER			104									//����ׯ��
#define SUB_S_CHANGE_BANKER			105									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		106									//���»���
#define SUB_S_SEND_RECORD			107									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		108									//��עʧ��
#define SUB_S_CANCEL_BANKER			109									//ȡ������
#define SUB_S_ANDROA_AREA			110									//WinArea	
#define SUB_S_REVOCAT_BET			111									//����Ѻע
#define SUB_S_ROBOT_BANKER			112									//��ׯ֪ͨ (������)
#define SUB_S_ROBOT_CONFIG			113									//����֪ͨ (������)
#define SUB_S_AMDIN_COMMAND			120									//����Ա����
#define SUB_S_SEND_USER_BET_INFO    121									//������ע
#define SUB_S_UPDATE_STORAGE        122									//���¿��
#define SUB_S_CONTROL_WIN			123									//���Ƶ��������Ӯ


//����������Ϣ
struct tagControlInfo
{
	BYTE cbControlArea[5];			//��������
};

struct CMD_C_FreshStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorageDeduct;					//���˥��
	LONGLONG						lStorageCurrent;				//��ǰ���
	LONGLONG						lStorageMax1;					//�������1
	LONGLONG						lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						lStorageMax2;					//�������2
	LONGLONG						lStorageMul2;					//ϵͳ��ָ���2
};

//���¿��
struct CMD_S_UpdateStorage
{
	BYTE                            cbReqType;						//��������
	LONGLONG						lStorageStart;				//��ʼ���
	LONGLONG						lStorageDeduct;				//���˥��
	LONGLONG						lStorageCurrent;				//��ǰ���
	LONGLONG						lStorageMax1;					//�������1
	LONGLONG						lStorageMul1;					//ϵͳ��ָ���1
	LONGLONG						lStorageMax2;					//�������2
	LONGLONG						lStorageMul2;					//ϵͳ��ָ���2
};

struct CMD_C_ControlWinLose
{
	BYTE							cbWinLose;						//0Ϊ��Чֵ��1ΪӮ��2Ϊ��
	LONGLONG						lQueryGameID;					//���ID
};

struct CMD_S_ControlWinLose
{
	BYTE							cbWinLose;						//0Ϊ��Чֵ��1ΪӮ��2Ϊ��
	LONGLONG						lQueryGameID;					//���ID
};

//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_RESET_CONTROL 2
#define ACK_PRINT_SYN     3
	BYTE cbResult;
#define CR_ACCEPT  2			//����
#define CR_REFUSAL 3			//�ܾ�
#define CR_INVALID 4			//ʧЧ
	BYTE cbSicbo1;
	BYTE cbSicbo2;
	BYTE cbSicbo3;
	BYTE cbTotalCount;
	BYTE cbLastType;			//��һ�εĿ�������
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

//������ע
struct CMD_S_SendUserBetInfo
{
	BYTE							cbAreaCount;							//��ע����
	LONGLONG						lUserStartScore[GAME_PLAYER];				//��ʼ����
	LONGLONG						lUserJettonScore[GAME_PLAYER];//������ע
};

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	SCORE						lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	SCORE						lScore;								//��һ���
	SCORE						lCurrentBankerScore;				//ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
	WORD							wApplyUser;							//�������
};

//ȡ������
struct CMD_S_CancelBanker
{
	WORD							wCancelUser;					//ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	SCORE						lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ
	//�����Ϣ
	SCORE						lUserMaxScore;						//��ҽ��
	SCORE						lBankerWinScore;					//ׯ�ҳɼ�
	SCORE						lBankerScore;						//ׯ�ҷ���
	//������Ϣ
	SCORE						lApplyBankerCondition;				//��������
	SCORE						lAreaLimitScore;					//��������
	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������};
	tagCustomAndroid				CustomAndroid;						//����������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	SCORE							lAllJettonScore[AREA_COUNT];		//ȫ����ע

	//�����ע
	SCORE							lUserJettonScore[AREA_COUNT];		//������ע

	//��һ���
	SCORE							lUserMaxScore;						//�����ע							

	//������Ϣ
	SCORE							lApplyBankerCondition;				//��������
	SCORE							lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE								cbDiceValue[DICE_COUNT];				//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	SCORE						lBankerWinScore;					//ׯ��Ӯ��
	SCORE						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	SCORE						lEndBankerScore;					//ׯ�ҳɼ�
	SCORE						lEndUserScore;						//��ҳɼ�
	SCORE						lEndUserReturnScore;				//���ػ���
	LONG							lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬

	//������Ϣ
	TCHAR							szGameRoomName[SERVER_LEN];			//��������};
	tagCustomAndroid				CustomAndroid;						//����������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	INT64                           nListUserCount;						//�б�����
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;									//ׯ��λ��
	BYTE							cbTimeLeave;									//ʣ��ʱ��	
	SCORE						lBankerScore;									//ׯ�ҽ��
	SCORE						lUserMaxScore;									//�ҵĽ��
	int								nChipRobotCount;								//�������� (��ע������)
	int								nAndriodApplyCount;					//�������б�����
	BYTE							bWinFlag[AREA_COUNT];							//��Ӯ��Ϣ (������)
};

//��ʼ��ע
struct CMD_S_StartBet
{
	BYTE							cbTimeLeave;									//ʣ��ʱ��	
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	SCORE						lJettonScore;						//��ע��Ŀ
	bool							bIsAndroid;							//�Ƿ������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbDiceValue[DICE_COUNT];			//���ӵ���
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							bcFirstCard;
 
	//ׯ����Ϣ
	INT								nBankerTime;						//��ׯ����
	SCORE						lBankerScore;						//ׯ�ҳɼ�
	SCORE						lBankerTotallScore;					//ׯ�ҳɼ�
	//��ҳɼ�
	SCORE						lUserScore;							//��ҳɼ�
	SCORE						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONG							lRevenue;							//��Ϸ˰��
};

//����֪ͨ
struct CMD_S_RobotConfig
{
	tagRobotConfig					robotConfig;						//��������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_AMDIN_COMMAND			5									//����Ա����
#define SUB_C_STORAGE				6									//���¿��
#define SUB_C_CONTROL_WIN			7									//���Ƶ��������Ӯ
#define IDM_ADMIN_STORAGE			WM_USER+1001
#define IDM_CONTROL_WIN				WM_USER+1002

struct tagAdminReq
{
	BYTE		m_cbControlStyle;					//���Ʒ�ʽ
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

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	SCORE						lJettonScore;						//��ע��Ŀ
};

#pragma pack(pop)
//////////////////////////////////////////////////////////////////////////

#endif
