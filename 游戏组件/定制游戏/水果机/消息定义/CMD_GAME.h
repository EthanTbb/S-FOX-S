#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						131									//��Ϸ I D
#define GAME_PLAYER					MAX_CHAIR									//��Ϸ����
#define GAME_NAME					TEXT("ˮ����")						//��Ϸ����

//�汾��Ϣ
#define VERSION_SERVER			    PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//״̬����
#define	GS_GAME_FREE				GAME_STATUS_FREE
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY+1					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+2						//����״̬
#define	GS_COMPARE_END				GAME_STATUS_PLAY+3						//����״̬

//��������
#define ID_TIAN_MEN					1									//˳��
#define ID_DI_MEN					2									//��߽�
#define ID_XUAN_MEN					3									//��
#define ID_HUANG_MEN				4									//����

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������
#define HUAN_MEN_INDEX				4									//��������

#define AREA_COUNT					8									//������Ŀ

//���ʶ���
#define RATE_TWO_PAIR				12									//��������

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
		lRobotBankGetMin = 1000;
		lRobotBankGetMax = 30000000;
		lRobotBankGetBankerMin = 10000000;
		lRobotBankGetBankerMax = 50000000;
		lRobotBankStoMul = 50;
	}
};

//��������Ϣ
struct tagRobotInfo
{
	int nChip[8];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	int nAreaOdds[AREA_COUNT];											//��������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

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

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							bWinMen[AREA_COUNT+1];						//˳��ʤ��
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE				99									//��Ϸ����
#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_PLACE_JETTON			101									//�û���ע
#define SUB_S_GAME_END				102									//��Ϸ����
#define SUB_S_APPLY_BANKER			103									//����ׯ��
#define SUB_S_CHANGE_BANKER			104									//�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE		105									//���»���
#define SUB_S_SEND_RECORD			106									//��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL		107									//��עʧ��
#define SUB_S_CANCEL_BANKER			108									//ȡ������
#define SUB_S_CHECK_IMAGE			109									//ȡ������
#define SUB_S_ADMIN_COMMDN			110									//ϵͳ����
#define	SUB_S_CHANGE_GOLD			111									//�һ�����
#define SUB_S_GAME_COMPARE			112									//��Ϸ�´�С
#define	SUB_S_COMPARE				113									//����ʱ��
#define	SUB_S_GUESS					114									//�´�С
#define SUB_S_ADMIN_STORAGE_INFO	115									//ˢ�¿��Ʒ����

//���Ʒ������Ϣ
struct CMD_S_ADMIN_STORAGE_INFO
{
	LONGLONG						lCurrentStorage;					//��ǰ���
	LONGLONG						lCurrentDeduct;						//��ǰ˥��
	LONGLONG						lMaxStorage[2];						//�������
	WORD							wStorageMul[2];						//ϵͳ��ָ���
	LONGLONG						lUserTotalScore[GAME_PLAYER];		//�������Ӯ
};
//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
	WORD							wPlaceUser;							//��ע���
	BYTE							lJettonArea;						//��ע����
	LONGLONG						lPlaceScore;						//��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
	WORD							wChairID;							//���Ӻ���
	DOUBLE							lScore;								//��һ���

	//ׯ����Ϣ
	WORD							wCurrentBankerChairID;				//��ǰׯ��
	BYTE							cbBankerTime;						//ׯ�Ҿ���
	DOUBLE							lCurrentBankerScore;				//ׯ�ҷ���
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
	LONGLONG						lBankerScore;						//ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�����Ϣ
	LONGLONG						lUserMaxScore;							//��ҽ��

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������
	int								CheckImage;

	TCHAR							szGameRoomName[32];					//�������� 
	int								nExchange;							//�һ�����

	LONGLONG						lStartStorage;						//��ʼ���
	tagCustomAndroid				CustomAndroid;						//����������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_COUNT+1];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_COUNT+1];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[5];				//�����˿�

	//ׯ����Ϣ
	WORD							wBankerUser;						//��ǰׯ��
	WORD							cbBankerTime;						//ׯ�Ҿ���
	LONGLONG						lBankerWinScore;					//ׯ��Ӯ��
	LONGLONG						lBankerScore;						//ׯ�ҷ���
	bool							bEnableSysBanker;					//ϵͳ��ׯ

	//������Ϣ
	LONGLONG						lEndBankerScore;					//ׯ�ҳɼ�
	LONGLONG						lEndUserScore;						//��ҳɼ�
	LONGLONG						lEndUserReturnScore;				//���ػ���
	LONGLONG						lEndRevenue;						//��Ϸ˰��

	//ȫ����Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	BYTE							cbGameStatus;						//��Ϸ״̬
	int								CheckImage;
	TCHAR							szGameRoomName[32];			//�������� 
	int								nExchange;							//�һ�����

	LONGLONG						lStartStorage;						//��ʼ���
	tagCustomAndroid				CustomAndroid;						//����������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	INT64                             nListUserCount;						//�б�����
	LONGLONG						lStorageStart;	
	LONGLONG						lBankerWinScore;					//ׯ�ҳɼ�
	LONGLONG						lExchangeGold[GAME_PLAYER];			//�һ������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ��λ��
	LONGLONG						lBankerScore;						//ׯ�ҽ��
	LONGLONG						lUserMaxScore;						//�ҵĽ��
	BYTE							cbTimeLeave;						//ʣ��ʱ��	
	bool							bContiueCard;						//��������
	int								nChipRobotCount;					//�������� (��ע������)
	int								nAndroidCount;						//��������ׯ��

};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	BYTE							cbAndroid;							//������
	bool							bAndroid;							//
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbTableCardArray[5];				//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							bcFirstCard;
 
	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����

	//��ҳɼ�
	LONGLONG						lUserScore;							//��ҳɼ�
	LONGLONG						lUserReturnScore[GAME_PLAYER];		//���Ӯ��Ǯ
	int								nProbability;						//�н�����
	int								nRand;								//��ǹ����

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

//�һ�����
struct CMD_S_ChangeGold
{	
	LONGLONG						lExchangeGold[GAME_PLAYER];			//�һ������
	WORD							wChairID;							//�һ����
};

//�һ�����
struct CMD_S_Compare
{	
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��
};

//�´�С
struct CMD_S_Guess
{	
	int								nGuessNum;							//���е�����
	LONGLONG						lUserReturnScore;					//���Ӯ��Ǯ
	WORD							wChairID;							//�´�С�����
	
	INT								nBankerTime;						//��ׯ����
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_CHECK_IMAGE			5									//��������
#define SUB_C_ADMIN_COMMDN			6									//ϵͳ����
#define SUB_C_STORAGE				7									//���¿��
#define SUB_C_DEUCT					8									//���¿��
#define	SUB_C_CHANGE_GOLD			9									//�һ�����
#define	SUB_C_GUESS					10									//�´�С
#define SUB_C_STORAGEINFO			11									//�޸��·�����

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
};

struct CMD_C_CheckImage
{
	int Index;
};

//�һ�����
struct CMD_C_ChangeGold
{
	LONGLONG						lChangeGold;						//�һ������
};

//�´�С
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
//����������Ϣ
struct tagControlInfo
{
	BYTE cbControlArea;						//��������
};

struct CMD_C_FreshStorage
{
	LONGLONG						lStorageStart;						//�����ֵ
};

struct CMD_C_FreshDeuct
{
	LONGLONG						lStorageDeuct;						//�����ֵ
};

struct CMD_C_StorageInfo
{
	LONGLONG						lMaxStorage[2];						//�������
	WORD							wStorageMul[2];						//�·ָ���
};

//���������Ʒ���
#define	 S_CR_FAILURE				0		//ʧ��
#define  S_CR_UPDATE_SUCCES			1		//���³ɹ�
#define	 S_CR_SET_SUCCESS			2		//���óɹ�
#define  S_CR_CANCEL_SUCCESS		3		//ȡ���ɹ�
struct CMD_S_ControlReturns
{
	BYTE cbReturnsType;				//�ظ�����
	BYTE cbControlArea;				//��������
	BYTE cbControlTimes;			//���ƴ���
};


//�ͻ��˿�������
#define  C_CA_UPDATE				1		//����
#define	 C_CA_SET					2		//����
#define  C_CA_CANCELS				3		//ȡ��
struct CMD_C_ControlApplication
{
	BYTE cbControlAppType;			//��������
	BYTE cbControlArea;				//��������
	BYTE cbControlTimes;			//���ƴ���
};

//////////////////////////////////////////////////////////////////////////

#endif
