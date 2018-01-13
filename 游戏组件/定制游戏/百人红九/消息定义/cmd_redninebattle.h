#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE


//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID           105                 //��Ϸ I D
#define GAME_NAME         _TEXT("����")           //��Ϸ����
#define GAME_PLAYER         MAX_CHAIR             //��Ϸ����

//�汾��Ϣ
#define VERSION_SERVER          PROCESS_VERSION(7,0,1)        //����汾
#define VERSION_CLIENT        PROCESS_VERSION(7,0,1)        //����汾

//״̬����
#define GAME_SCENE_FREE       GAME_STATUS_FREE          //�ȴ���ʼ
#define GS_PLACE_JETTON       GAME_STATUS_PLAY          //��ע״̬
#define GS_GAME_END         GAME_STATUS_PLAY+1          //����״̬

//��������
#define ID_SHUN_MEN         1                 //˳��
#define ID_JIAO_L         2                 //��߽�
#define ID_QIAO           3                 //��
#define ID_DUI_MEN          4                 //����
#define ID_DAO_MEN          5                 //����
#define ID_JIAO_R         6                 //�ұ߽�

//�������
#define BANKER_INDEX        0                 //ׯ������
#define SHUN_MEN_INDEX        1                 //˳������
#define DUI_MEN_INDEX       2                 //��������
#define DAO_MEN_INDEX       3                 //��������
#define MAX_INDEX         3                 //�������

#define AREA_COUNT          6                 //������Ŀ
#define CONTROL_AREA        3                 //�ܿ�����

//���ʶ���
#define RATE_TWO_PAIR       12                  //��������
#define SERVER_LEN          32                  //���䳤��

#define MAX_CARD          2
#define MAX_CARDGROUP       4

struct tagCustomAndroid
{
  //��ׯ
  BOOL              nEnableRobotBanker;       //�Ƿ���ׯ
  LONGLONG            lRobotBankerCountMin;     //��ׯ����
  LONGLONG            lRobotBankerCountMax;     //��ׯ����
  LONGLONG            lRobotListMinCount;       //�б�����
  LONGLONG            lRobotListMaxCount;       //�б�����
  LONGLONG            lRobotApplyBanker;        //����������
  LONGLONG            lRobotWaitBanker;       //��������

  //��ע
  LONGLONG            lRobotMinBetTime;       //��ע�������
  LONGLONG            lRobotMaxBetTime;       //��ע�������
  LONGLONG            lRobotMinJetton;        //��ע������
  LONGLONG            lRobotMaxJetton;        //��ע������
  LONGLONG            lRobotBetMinCount;        //��ע��������
  LONGLONG            lRobotBetMaxCount;        //��ע��������
  LONGLONG            lRobotAreaLimit;        //��������

  //��ȡ��
  LONGLONG            lRobotScoreMin;         //�������
  LONGLONG            lRobotScoreMax;         //�������
  LONGLONG            lRobotBankGetMin;       //ȡ����Сֵ(��ׯ)
  LONGLONG            lRobotBankGetMax;       //ȡ�����ֵ(��ׯ)
  LONGLONG            lRobotBankGetBankerMin;     //ȡ����Сֵ(��ׯ)
  LONGLONG            lRobotBankGetBankerMax;     //ȡ�����ֵ(��ׯ)
  LONGLONG            lRobotBankStoMul;       //���ٷֱ�

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

//��������Ϣ
struct tagRobotInfo
{
  int nChip[7];                           //���붨��
  int nAreaChance[AREA_COUNT];                    //������
  int nMaxTime;                           //�������

  tagRobotInfo()
  {
    int nTmpChip[7] = {100, 1000, 10000, 100000, 500000, 1000000, 5000000};
    int nTmpAreaChance[AREA_COUNT] = {2, 1, 1, 2, 2, 1};

    nMaxTime = 1;
    memcpy(nChip, nTmpChip, sizeof(nChip));
    memcpy(nAreaChance, nTmpAreaChance, sizeof(nAreaChance));
  }
};

//��¼��Ϣ
struct tagServerGameRecord
{
  bool              bWinShunMen;            //˳��ʤ��
  bool              bWinDuiMen;             //����ʤ��
  bool              bWinDaoMen;             //����ʤ��
};

//��ע��Ϣ
struct tagUserBet
{
  TCHAR             szNickName[32];           //�û��ǳ�
  DWORD             dwUserGameID;           //�û�ID
  LONGLONG            lUserStartScore;          //�û����
  LONGLONG            lUserWinLost;           //�û����
  LONGLONG            lUserBet[AREA_COUNT+1];       //�û���ע
};

//��ע��Ϣ����
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;

///////////////////////////////////////////////////////////////////////////////////////

//�궨��
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

//������
#define RQ_REFRESH_STORAGE    1
#define RQ_SET_STORAGE      2
//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_FREE       99                  //��Ϸ����
#define SUB_S_GAME_START      100                 //��Ϸ��ʼ
#define SUB_S_PLACE_JETTON      101                 //�û���ע
#define SUB_S_GAME_END        102                 //��Ϸ����
#define SUB_S_APPLY_BANKER      103                 //����ׯ��
#define SUB_S_CHANGE_BANKER     104                 //�л�ׯ��
#define SUB_S_CHANGE_USER_SCORE   105                 //���»���
#define SUB_S_SEND_RECORD     106                 //��Ϸ��¼
#define SUB_S_PLACE_JETTON_FAIL   107                 //��עʧ��
#define SUB_S_CANCEL_BANKER     108                 //ȡ������
#define SUB_S_CHEAT         109                 //������Ϣ

#define SUB_S_AMDIN_COMMAND     110                 //����Ա����
#define SUB_S_UPDATE_STORAGE        111                 //���¿��
#define SUB_S_SEND_USER_BET_INFO    112                 //������ע

//����ظ�
struct CMD_S_CommandResult
{
  BYTE cbAckType;         //�ظ�����
#define ACK_SET_WIN_AREA  1
#define ACK_PRINT_SYN     2
#define ACK_RESET_CONTROL 3
  BYTE cbResult;
#define CR_ACCEPT  2      //����
#define CR_REFUSAL 3      //�ܾ�
  BYTE cbExtendData[20];      //��������
};

//���¿��
struct CMD_S_UpdateStorage
{
  BYTE                            cbReqType;            //��������
  LONGLONG            lStorageStart;        //��ʼ���
  LONGLONG            lStorageDeduct;       //���˥��
  LONGLONG            lStorageCurrent;        //��ǰ���
  LONGLONG            lStorageMax1;         //�������1
  LONGLONG            lStorageMul1;         //ϵͳ��ָ���1
  LONGLONG            lStorageMax2;         //�������2
  LONGLONG            lStorageMul2;         //ϵͳ��ָ���2
};

//������ע
struct CMD_S_SendUserBetInfo
{
  LONGLONG            lUserStartScore[GAME_PLAYER];       //��ʼ����
  LONGLONG            lUserJettonScore[AREA_COUNT+1][GAME_PLAYER];//������ע
};

//ʧ�ܽṹ
struct CMD_S_PlaceJettonFail
{
  WORD              wPlaceUser;             //��ע���
  BYTE              lJettonArea;            //��ע����
  LONGLONG            lPlaceScore;            //��ǰ��ע
};

//���»���
struct CMD_S_ChangeUserScore
{
  WORD              wChairID;             //���Ӻ���
  DOUBLE              lScore;               //��һ���

  //ׯ����Ϣ
  WORD              wCurrentBankerChairID;        //��ǰׯ��
  BYTE              cbBankerTime;           //ׯ�Ҿ���
  DOUBLE              lCurrentBankerScore;        //ׯ�ҷ���
};

//����ׯ��
struct CMD_S_ApplyBanker
{
  WORD              wApplyUser;             //�������
};

//ȡ������
struct CMD_S_CancelBanker
{
  WORD              wCancelUser;          //ȡ�����
};

//�л�ׯ��
struct CMD_S_ChangeBanker
{
  WORD              wBankerUser;            //��ׯ���
  LONGLONG            lBankerScore;           //ׯ�ҽ��
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
  //ȫ����Ϣ
  BYTE              cbTimeLeave;            //ʣ��ʱ��

  //�����Ϣ
  LONGLONG            lUserMaxScore;            //��ҽ��

  //ׯ����Ϣ
  WORD              wBankerUser;            //��ǰׯ��
  WORD              cbBankerTime;           //ׯ�Ҿ���
  LONGLONG            lBankerWinScore;          //ׯ�ҳɼ�
  LONGLONG            lBankerScore;           //ׯ�ҷ���
  INT               nEndGameMul;            //��ǰ���ưٷֱ�
  bool              bEnableSysBanker;         //ϵͳ��ׯ

  //������Ϣ
  LONGLONG            lApplyBankerCondition;        //��������
  LONGLONG            lAreaLimitScore;          //��������

  //������Ϣ
  TCHAR             szGameRoomName[SERVER_LEN];     //��������
  bool              bGenreEducate;            //��ϰģʽ
  tagCustomAndroid        CustomAndroid;            //����������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
  //ȫ����ע
  LONGLONG            lAllJettonScore[AREA_COUNT+1];    //ȫ����ע

  //�����ע
  LONGLONG            lUserJettonScore[AREA_COUNT+1];   //������ע

  //��һ���
  LONGLONG            lUserMaxScore;            //�����ע

  //������Ϣ
  LONGLONG            lApplyBankerCondition;        //��������
  LONGLONG            lAreaLimitScore;          //��������

  //�˿���Ϣ
  BYTE              cbTableCardArray[4][2];       //�����˿�

  //ׯ����Ϣ
  WORD              wBankerUser;            //��ǰׯ��
  WORD              cbBankerTime;           //ׯ�Ҿ���
  LONGLONG            lBankerWinScore;          //ׯ��Ӯ��
  LONGLONG            lBankerScore;           //ׯ�ҷ���
  INT               nEndGameMul;            //��ǰ���ưٷֱ�
  bool              bEnableSysBanker;         //ϵͳ��ׯ

  //������Ϣ
  LONGLONG            lEndBankerScore;          //ׯ�ҳɼ�
  LONGLONG            lEndUserScore;            //��ҳɼ�
  LONGLONG            lEndUserReturnScore;        //���ػ���
  LONGLONG            lEndRevenue;            //��Ϸ˰��

  //ȫ����Ϣ
  BYTE              cbTimeLeave;            //ʣ��ʱ��
  BYTE              cbGameStatus;           //��Ϸ״̬

  //������Ϣ
  TCHAR             szGameRoomName[SERVER_LEN];     //��������
  bool              bGenreEducate;            //��ϰģʽ
  tagCustomAndroid        CustomAndroid;            //����������
};

//��Ϸ����
struct CMD_S_GameFree
{
  BYTE              cbTimeLeave;            //ʣ��ʱ��
  WORD              wCurrentBanker;           //��ǰׯ��
  INT               nBankerTime;            //��ׯ����
  INT64                           nListUserCount;           //�б�����
  LONGLONG            lStorageStart;            //�����ֵ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
  WORD              wBankerUser;            //ׯ��λ��
  LONGLONG            lBankerScore;           //ׯ�ҽ��
  LONGLONG            lUserMaxScore;            //�ҵĽ��
  BYTE              cbTimeLeave;            //ʣ��ʱ��
  bool              bContiueCard;           //��������
  int               nChipRobotCount;          //�������� (��ע������)
  int               nAndriodApplyCount;         //�������б�����
};

//�û���ע
struct CMD_S_PlaceJetton
{
  WORD              wChairID;             //�û�λ��
  BYTE              cbJettonArea;           //��������
  LONGLONG            lJettonScore;           //��ע��Ŀ
  bool              bIsAndroid;             //�Ƿ������
  bool              bAndroid;           //������ʶ
};

//��Ϸ����
struct CMD_S_GameEnd
{
  //�¾���Ϣ
  BYTE              cbTimeLeave;            //ʣ��ʱ��

  //�˿���Ϣ
  BYTE              cbTableCardArray[4][2];       //�����˿�
  BYTE              cbLeftCardCount;          //�˿���Ŀ

  BYTE              bcFirstCard;

  //ׯ����Ϣ
  WORD              wCurrentBanker;           //��ǰׯ��
  LONGLONG            lBankerScore;           //ׯ�ҳɼ�
  LONGLONG            lBankerTotallScore;         //ׯ�ҳɼ�
  INT               nBankerTime;            //��ׯ����

  //��ҳɼ�
  LONGLONG            lUserScore;             //��ҳɼ�
  LONGLONG            lUserReturnScore;         //���ػ���

  //ȫ����Ϣ
  LONGLONG            lRevenue;             //��Ϸ˰��
};

//��Ϸ����
struct CMD_S_Cheat
{
  BYTE              cbTableCardArray[4][2];       //�����˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON      1                 //�û���ע
#define SUB_C_APPLY_BANKER      2                 //����ׯ��
#define SUB_C_CANCEL_BANKER     3                 //ȡ������
#define SUB_C_CONTINUE_CARD     4                 //��������
#define SUB_C_AMDIN_COMMAND     5                 //����Ա����
#define SUB_C_UPDATE_STORAGE        6                 //���¿��
//�ͻ�����Ϣ
#define IDM_ADMIN_COMMDN      WM_USER+1000
#define IDM_UPDATE_STORAGE      WM_USER+1001


//����������Ϣ
struct tagControlInfo
{
  BYTE cbControlArea[MAX_INDEX];      //��������
};

struct tagAdminReq
{
  BYTE              m_cbExcuteTimes;          //ִ�д���
  BYTE              m_cbControlStyle;         //���Ʒ�ʽ
#define   CS_BANKER_LOSE    1
#define   CS_BANKER_WIN   2
#define   CS_BET_AREA     3
  bool              m_bWinArea[3];            //Ӯ������
};


struct CMD_C_AdminReq
{
  BYTE cbReqType;
#define RQ_SET_WIN_AREA  1
#define RQ_RESET_CONTROL 2
#define RQ_PRINT_SYN   3
  BYTE cbExtendData[20];      //��������
};

//�û���ע
struct CMD_C_PlaceJetton
{
  BYTE              cbJettonArea;           //��������
  LONGLONG            lJettonScore;           //��ע��Ŀ
};

//���¿��
struct CMD_C_UpdateStorage
{
  BYTE                            cbReqType;            //��������
  LONGLONG            lStorageDeduct;         //���˥��
  LONGLONG            lStorageCurrent;        //��ǰ���
  LONGLONG            lStorageMax1;         //�������1
  LONGLONG            lStorageMul1;         //ϵͳ��ָ���1
  LONGLONG            lStorageMax2;         //�������2
  LONGLONG            lStorageMul2;         //ϵͳ��ָ���2
};

//////////////////////////////////////////////////////////////////////////

#endif
