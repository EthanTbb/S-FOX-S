#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						108//109									//��Ϸ I D
#define GAME_PLAYER					MAX_CHAIR									//��Ϸ����
#define GAME_NAME					TEXT("����")					    //��Ϸ����

//״̬����
#define	GS_PLACE_JETTON				GAME_STATUS_PLAY					//��ע״̬
#define	GS_GAME_END					GAME_STATUS_PLAY+1					//����״̬
#define	GS_MOVECARD_END				GAME_STATUS_PLAY+2					//����״̬

//�汾��Ϣ
#define VERSION_SERVER			    PROCESS_VERSION(6,0,3)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(6,0,3)				//����汾

//��������
#define ID_SHUN_MEN					1									//����
#define ID_DUI_MEN					2									//����
#define ID_DAO_MEN					3									//����
#define ID_JIAO_L					4									//��߽�
#define ID_QIAO						5									//��
#define ID_JIAO_R					6									//�ұ߽�

//�������
#define BANKER_INDEX				0									//ׯ������
#define SHUN_MEN_INDEX				1									//˳������
#define DUI_MEN_INDEX				2									//��������
#define DAO_MEN_INDEX				3									//��������
#define MAX_INDEX					4									//�������

#define AREA_COUNT					3									//������Ŀ
#define AREA_ARRY_COUNT				7									//������Ŀ

//���ʶ���
#define RATE_TWO_PAIR				12									//��������

#define SERVER_LEN					32									//���䳤��

//��������
#define STAR_COUNT				10

//ʤ����ʾ
enum WinFlag
{
	WIN_FLAG_LOST,
	WIN_FLAG_WIN,
	WIN_FLAG_EQUAL,
};	

//��������Ϣ
struct tagRobotInfo
{
	int nChip[6];														//���붨��
	int nAreaChance[AREA_COUNT];										//������
	TCHAR szCfgFileName[MAX_PATH];										//�����ļ�
	int	nMaxTime;														//�������

	tagRobotInfo()
	{
		int nTmpChip[6] = {1000,10000, 100000, 1000000, 5000000, 10000000};
		int nTmpAreaChance[AREA_COUNT] = {1, 1,1};
		TCHAR szTmpCfgFileName[MAX_PATH] = _T("GodInConfig.ini");

		nMaxTime = 1;
		memcpy(nChip, nTmpChip, sizeof(nChip));
		memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
		memcpy(szCfgFileName, szTmpCfgFileName, sizeof(szCfgFileName));
	}
};

//��¼��Ϣ
struct tagServerGameRecord
{
	BYTE							bWinShunMen;						//˳��ʤ��
	BYTE							bWinDuiMen;							//����ʤ��
	BYTE							bWinDaoMen;							//����ʤ��
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
#define SUB_S_CANCEL_CAN			109									//ȡ������
#define SUB_S_ANDROA_AREA			110									//Ӯ����
#define SUB_S_AMDIN_COMMAND_RESULT  111									//����Ա����
#define SUB_S_TO_BANKER_MAX         113									//�ﵽׯ������
#define SUB_S_ROBOT_BANKER			114									//��ׯ֪ͨ (������)
#define SUB_S_NOTICE_TIME           115                                 //֪ͨʱ��

//����ظ�
struct CMD_S_CommandResult
{
	BYTE cbAckType;					//�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
	BYTE cbResult;
#define CR_ACCEPT  2			//����
#define CR_REFUSAL 3			//�ܾ�
	BYTE cbExtendData[20];			//��������
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
	TCHAR							szCancelUser[32];					//ȡ�����
};

//�Ƿ�ɹ�ȡ������
struct CMD_S_bCanCancelBanker
{
	WORD							wChariID;
	BOOL							blCancel;
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
	WORD							wBankerUser;						//��ׯ���
	LONGLONG						lBankerScore;						//ׯ�ҽ��
};

//����ʱ��֪ͨ
struct CMD_S_FreeTime
{
	BYTE                            cbTimeLeave;						//ʣ��ʱ��
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
	float							fStarWinRate[STAR_COUNT];
	LONGLONG						llStarAllCount[STAR_COUNT];
	LONGLONG						llStarWinCount[STAR_COUNT];
	LONGLONG						llAllStarCount;
	int								nRedBack;

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//������Ϣ
	TCHAR							szServerName[SERVER_LEN];			//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//ȫ����ע
	LONGLONG						lAllJettonScore[AREA_ARRY_COUNT];		//ȫ����ע

	//�����ע
	LONGLONG						lUserJettonScore[AREA_ARRY_COUNT];		//������ע

	//��һ���
	LONGLONG						lUserMaxScore;						//�����ע							

	//������Ϣ
	LONGLONG						lApplyBankerCondition;				//��������
	LONGLONG						lAreaLimitScore;					//��������

	//�˿���Ϣ
	BYTE							cbTableCardArray[4][2];				//�����˿�

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
	//������Ϣ
	BYTE							cbStarSelect[3];
	float							fStarWinRate[STAR_COUNT];
	LONGLONG						llStarAllCount[STAR_COUNT];
	LONGLONG						llStarWinCount[STAR_COUNT];
	LONGLONG						llAllStarCount;
	int								nRedBack;

	//������Ϣ
	TCHAR							szServerName[SERVER_LEN];			//��������
};

//��Ϸ����
struct CMD_S_GameFree
{
	BYTE							cbTimeLeave;						//ʣ��ʱ��
	bool                            bApplyBanker;						//����ׯ��
	
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
	LONGLONG                        lUserLimitScore;					//���Ʒ���
	BYTE							cbStarSelect[3];
	float							fStarWinRate[STAR_COUNT];
	LONGLONG						llStarAllCount[STAR_COUNT];
	LONGLONG						llStarWinCount[STAR_COUNT];
	LONGLONG						llAllStarCount;
	int								nRedBack;
};

//�û���ע
struct CMD_S_PlaceJetton
{
	WORD							wChairID;							//�û�λ��
	BYTE							cbJettonArea;						//��������
	LONGLONG						lJettonScore;						//��ע��Ŀ
	BYTE							cbAndroidUser;						//������ʶ
	BYTE							cbAndroidUserT;						//������ʶ
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//�¾���Ϣ
	BYTE							cbTimeLeave;						//ʣ��ʱ��

	//�˿���Ϣ
	BYTE							cbTableCardArray[4][2];				//�����˿�
	BYTE							cbLeftCardCount;					//�˿���Ŀ

	BYTE							bcFirstCard;
 
	//ׯ����Ϣ
	LONGLONG						lBankerScore;						//ׯ�ҳɼ�
	LONGLONG						lBankerTotallScore;					//ׯ�ҳɼ�
	INT								nBankerTime;						//��ׯ����
	bool							bBankerBomb;

	//��ҳɼ�
	LONGLONG						lUserScore;							//��ҳɼ�
	LONGLONG						lUserReturnScore;					//���ػ���

	//ȫ����Ϣ
	LONGLONG						lRevenue;							//��Ϸ˰��
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON			1									//�û���ע
#define SUB_C_APPLY_BANKER			2									//����ׯ��
#define SUB_C_CANCEL_BANKER			3									//ȡ������
#define SUB_C_CONTINUE_CARD			4									//��������
#define SUB_C_MANAGE_CONTROL		5									//ȡ������
#define SUB_C_AMDIN_COMMAND			6									//����Ա����

//�û���ע
struct CMD_C_PlaceJetton
{
	BYTE						cbJettonArea;						//��������
	LONGLONG					lJettonScore;						//��ע��Ŀ
};

//////////////////////////////////////////////////////////////////////////

//Ӯ������
struct CMD_S_AdminControl
{
	BYTE cbControlType;						//��������
};

#define IDM_ADMIN_COMMDN					(WM_USER+3201)
//�ͻ��˿�����Ϣ
struct CMD_C_AdminControl
{
	BYTE cbControlArea[MAX_INDEX];			//��������
};

#endif
