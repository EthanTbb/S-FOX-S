#ifndef CMD_BACCARAT_HEAD_FILE
#define CMD_BACCARAT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID           123                 //��Ϸ I D
#define GAME_PLAYER         100                 //��Ϸ����
#define GAME_NAME         TEXT("��������")          //��Ϸ����

//״̬����
#define GS_PLACE_JETTON       GAME_STATUS_PLAY          //��ע״̬
#define GS_GAME_END         GAME_STATUS_PLAY+1          //����״̬
#define GS_MOVECARD_END       GAME_STATUS_PLAY+2          //����״̬

#define VERSION_SERVER          PROCESS_VERSION(7,0,1)        //����汾
#define VERSION_CLIENT        PROCESS_VERSION(7,0,1)        //����汾

//����X6  ����X8  ��ȸX8  ��ӥX12 ʨ��X12 ��èX8  ����X8  ����X6 ����24 ����2 ����2

//��������
#define ID_TU_ZI          1                 //����
#define ID_YAN_ZI         2                 //����
#define ID_GE_ZI          3                 //����
#define ID_KONG_QUE         4                 //��ȸ
#define ID_LAO_YING         5                 //��ӥ
#define ID_SHI_ZI         6                 //ʨ��
#define ID_XIONG_MAO        7                 //��è
#define ID_HOU_ZI         8                 //����
#define ID_SHA_YU         9                 //����
#define ID_FEI_QIN          10                  //����    //ͨ��
#define ID_ZOU_SHOU         11                  //����    //ͨɱ
#define ID_GLOD_SHA         12                  //����    //����
//#define ID_TONG_SHA         13                  //ͨɱ
//#define ID_TONG_PEI         14                  //ͨ��


//���� //���� //��ȸ //��ӥ //ʨ�� //��è //���� //���� //���� //���� //����

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


#define AREA_COUNT          12                  //������Ŀ

#define AREA_ALL          12                  //��������

#define ANIMAL_COUNT                28                                  //�������
//���ʶ���
#define RATE_TWO_PAIR       12                  //��������

#define SERVER_LEN          32                  //���䳤��

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
//��������Ϣ
struct tagRobotInfo
{
  int nChip[5];                           //���붨��
  int nAreaChance[AREA_COUNT];                    //������
  TCHAR szCfgFileName[MAX_PATH];                    //�����ļ�
  int nMaxTime;                           //�������

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

//��¼��Ϣ
struct tagServerGameRecord
{
  BYTE              bWinMen[AREA_COUNT+1];            //˳��ʤ��
};

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
//#define SUB_S_CHECK_IMAGE     109                 //ȡ������
#define SUB_S_ADMIN_COMMDN      110                 //ϵͳ����
#define SUB_S_CEAN_JETTON     111                 //�����ע
#define SUB_S_CONTINU_JETTON    112                 //������ע
#define SUB_C_UPDATE_STORAGE        113                 //���¿��

#define SUB_S_UPDATE_STORAGE        114                 //���¿��
//������
#define RQ_REFRESH_STORAGE    1
#define RQ_SET_STORAGE      2
//�ͻ�����Ϣ
#define IDM_ADMIN_COMMDN      WM_USER+1000
#define IDM_UPDATE_STORAGE      WM_USER+1001

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
//���¿��
struct CMD_C_UpdateStorage
{
  BYTE                            cbReqType;            //��������
  LONGLONG            lStorageDeduct;         //���˥��
  LONGLONG            lStorageCurrent;        //��ǰ���
  LONGLONG            lCurrentBonus;          //��ǰ�ʽ��
  LONGLONG            lStorageMax1;         //�������1
  LONGLONG            lStorageMul1;         //ϵͳ��ָ���1
  LONGLONG            lStorageMax2;         //�������2
  LONGLONG            lStorageMul2;         //ϵͳ��ָ���2
};
//�����ע
struct CMD_S_CeanJetton
{
  LONGLONG            lAllCPlaceScore[AREA_COUNT+1];    //��ǰ��ע
  WORD              wChairID;             //�û�λ��
};

struct CMD_S_ContiueJetton
{
  LONGLONG            lAllJettonScore[AREA_COUNT+1];          //ȫ����ע
  LONGLONG            lUserJettonScore[AREA_COUNT+1];         //������ע


  WORD              wChairID;             //�û�λ��
  BYTE              cbAndroid;              //������
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
  TCHAR             szCancelUser[32];         //ȡ�����
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
  LONGLONG            lStorageStart;            //�����ֵ
  LONGLONG            lBonus;               //�ʽ��

  //ȫ����Ϣ
  BYTE              cbTimeLeave;            //ʣ��ʱ��

  //�����Ϣ
  LONGLONG            lUserMaxScore;            //��ҽ��
  INT               nAnimalPercent[AREA_ALL];     //���б���
  //ׯ����Ϣ
  WORD              wBankerUser;            //��ǰׯ��
  WORD              cbBankerTime;           //ׯ�Ҿ���
  LONGLONG            lBankerWinScore;          //ׯ�ҳɼ�
  LONGLONG            lBankerScore;           //ׯ�ҷ���
  bool              bEnableSysBanker;         //ϵͳ��ׯ

  //������Ϣ
  LONGLONG            lApplyBankerCondition;        //��������
  LONGLONG            lAreaLimitScore;          //��������

  TCHAR             szGameRoomName[32];         //��������
  tagCustomAndroid        CustomAndroid;            //����������

};
//��ע��Ϣ
struct tagUserBet
{
  TCHAR             szNickName[32];           //�û��ǳ�
  DWORD             dwUserGameID;           //�û�ID
  LONGLONG            lUserStartScore;          //�û����
  LONGLONG            lUserWinLost;           //�û����
  LONGLONG            lUserBet[AREA_COUNT];       //�û���ע
};
//��ע��Ϣ����
typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;
//��Ϸ״̬
struct CMD_S_StatusPlay
{
  //ȫ����ע
  LONGLONG            lAllJettonScore[AREA_COUNT+1];    //ȫ����ע

  //�����ע
  LONGLONG            lUserJettonScore[AREA_COUNT+1];   //������ע

  //��һ���
  LONGLONG            lUserMaxScore;            //�����ע
  INT               nAnimalPercent[AREA_ALL];     //���б���
  //������Ϣ
  LONGLONG            lApplyBankerCondition;        //��������
  LONGLONG            lAreaLimitScore;          //��������

  //�˿���Ϣ
  BYTE              cbTableCardArray[2];        //�����˿�

  //ׯ����Ϣ
  WORD              wBankerUser;            //��ǰׯ��
  WORD              cbBankerTime;           //ׯ�Ҿ���
  LONGLONG            lBankerWinScore;          //ׯ��Ӯ��
  LONGLONG            lBankerScore;           //ׯ�ҷ���
  bool              bEnableSysBanker;         //ϵͳ��ׯ

  //������Ϣ
  LONGLONG            lEndBankerScore;          //ׯ�ҳɼ�
  LONGLONG            lEndUserScore;            //��ҳɼ�
  LONGLONG            lEndUserReturnScore;        //���ػ���
  LONGLONG            lEndRevenue;            //��Ϸ˰��

  //ȫ����Ϣ
  BYTE              cbTimeLeave;            //ʣ��ʱ��
  BYTE              cbGameStatus;           //��Ϸ״̬
  TCHAR             szGameRoomName[32];         //��������

  LONGLONG            lStorageStart;            //�����ֵ
  LONGLONG            lBonus;               //�ʽ��
  tagCustomAndroid        CustomAndroid;            //����������

};

//��Ϸ����
struct CMD_S_GameFree
{
  BYTE              cbTimeLeave;            //ʣ��ʱ��
  LONGLONG            lStorageControl;          //�����ֵ
  LONGLONG            lStorageStart;            //�ʳ���ֵ
  LONGLONG            lBonus;               //�ʽ��
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
  LONGLONG            lStorageStart;            //�����ֵ
  LONGLONG            lBonus;               //�ʽ��
};

//�û���ע
struct CMD_S_PlaceJetton
{
  WORD              wChairID;             //�û�λ��
  BYTE              cbJettonArea;           //��������
  LONGLONG            lJettonScore;           //��ע��Ŀ
  BYTE              cbAndroid;              //������
  LONGLONG            lUserStartScore[GAME_PLAYER];       //��ʼ����
  //�����ע
  LONGLONG            lUserJettonScore[AREA_COUNT+1];   //������ע
};

//��Ϸ����
struct CMD_S_GameEnd
{
  //�¾���Ϣ
  BYTE              cbTimeLeave;            //ʣ��ʱ��

  //�˿���Ϣ
  BYTE              cbTableCardArray[2];        //�����˿�
  BYTE                            cbShaYuAddMulti;          //���ӱ���
  //ׯ����Ϣ
  LONGLONG            lBankerScore;           //ׯ�ҳɼ�
  LONGLONG            lBankerTotallScore;         //ׯ�ҳɼ�
  INT               nBankerTime;            //��ׯ����

  //��ҳɼ�
  LONGLONG            lUserScore;             //��ҳɼ�
  LONGLONG            lUserReturnScore;         //���ػ���

  //ȫ����Ϣ
  LONGLONG            lRevenue;             //��Ϸ˰��
  INT               nAnimalPercent[AREA_ALL];     //���б���

  LONGLONG            lStorageStart;            //�����ֵ
  LONGLONG            lLastJetton[AREA_COUNT+1];      //�Ͼ���ע
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_PLACE_JETTON      11                  //�û���ע
#define SUB_C_APPLY_BANKER      12                  //����ׯ��
#define SUB_C_CANCEL_BANKER     13                  //ȡ������

#define SUB_C_ADMIN_COMMDN      16                  //ϵͳ����

#define SUB_C_CLEAN_JETTON      157                 //��������
#define SUB_C_CONTINUE_JETTON   158                 //������ע

//�û���ע
struct CMD_C_PlaceJetton
{
  BYTE              cbJettonArea;           //��������
  LONGLONG            lJettonScore;           //��ע��Ŀ
};

struct CMD_C_CleanMeJetto
{
  WORD              wChairID;             //�û�λ��
};

struct CMD_C_ContinueJetton
{
  WORD          wChairID;             //�û�λ��
  LONGLONG        lLastAllJettonPlace[AREA_COUNT +1]; //ȫ������
  LONGLONG        lLastAllJettonArea[AREA_COUNT +1];  //�������

  BYTE          cbJettonArea;           //��������
  LONGLONG        lJettonScore;           //��ע��Ŀ
  BYTE          cbAndroid;              //������
};
//////////////////////////////////////////////////////////////////////////

#define IDM_ADMIN_COMMDN WM_USER+1000

//����������Ϣ
struct tagControlInfo
{
  BYTE cbControlArea1;            //��������
  BYTE cbControlArea2;            //��������
};



//���¿��
struct CMD_S_UpdateStorage
{
  BYTE                            cbReqType;            //��������
  LONGLONG            lStorageStart;          //��ʼ���
  LONGLONG            lStorageDeduct;         //���˥��
  LONGLONG            lStorageCurrent;        //��ǰ���
  LONGLONG            lCurrentBonus;          //��ǰ�ʽ��
  LONGLONG            lStorageMax1;         //�������1
  LONGLONG            lStorageMul1;         //ϵͳ��ָ���1
  LONGLONG            lStorageMax2;         //�������2
  LONGLONG            lStorageMul2;         //ϵͳ��ָ���2
};
//��ע��Ϣ����
//typedef CWHArray<tagUserBet,tagUserBet&> CUserBetArray;

//��������Ϣ

//���������Ʒ���
#define  S_CR_FAILURE       0   //ʧ��
#define  S_CR_UPDATE_SUCCES     1   //���³ɹ�
#define  S_CR_SET_SUCCESS     2   //���óɹ�
#define  S_CR_CANCEL_SUCCESS    3   //ȡ���ɹ�
#define  S_CR_ACK_PRINT_STORAGE   4   //���ÿ��
struct CMD_S_ControlReturns
{
  BYTE cbReturnsType;       //�ظ�����
  BYTE cbControlStyle;      //���Ʒ�ʽ
  BYTE cbControlArea;       //��������
  BYTE cbControlTimes;      //���ƴ���
  LONGLONG lCurStorage;     //��ǰ���
};


//��������
#define  C_CA_UPDATE        1   //����
#define  C_CA_SET         2   //����
#define  C_CA_CANCELS       3   //ȡ��
#define  C_CA_SET_STORAGE     4   //���
#define  C_CA_PRINT_STORAGE     5   //���

//���Ʒ�ʽ
#define   CS_BANKER_LOSE    1
#define   CS_BANKER_WIN   2
#define   CS_BET_AREA     3

struct CMD_C_ControlApplication
{
  BYTE cbControlAppType;      //��������
  BYTE cbControlStyle;      //���Ʒ�ʽ
  BYTE cbControlArea;       //��������
  BYTE cbControlTimes;      //���ƴ���
  LONGLONG lSetStorage;     //���ÿ��
  LONGLONG lSetDeduct;      //���ÿ��˥��ֵ
};

//////////////////////////////////////////////////////////////////////////

#endif
