#ifndef CMD_HK_FIVE_CARD_HEAD_FILE
#define CMD_HK_FIVE_CARD_HEAD_FILE


#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID             6                 //��Ϸ I D
#define GAME_PLAYER           5                 //��Ϸ����
#define GAME_NAME           TEXT("թ��")            //��Ϸ����
#define MAX_COUNT           3                 //�˿���Ŀ
#define VERSION_SERVER            PROCESS_VERSION(7,0,1)        //����汾
#define VERSION_CLIENT            PROCESS_VERSION(7,0,1)        //����汾

//��Ϸ����
#define GIVE_UP             2                 //��������
#define LOOK_CARD           8                 //���Ƹ���
#define COMPARE_CARD          5                 //���Ƹ���
#define MAX_LOOK_INDEX          10                  //��������
#define MAX_OPEN_INDEX          5                 //��������
#define SERVER_LEN            32
#define LEN_NICKNAME          32                  //�ǳƳ���

//����ԭ��
#define GER_NO_PLAYER         0x10                //û�����
#define GER_COMPARECARD         0x20                //���ƽ���
#define GER_OPENCARD          0x30                //���ƽ���

//��Ϸ״̬
#define GS_T_FREE         GAME_STATUS_FREE            //�ȴ���ʼ
#define GS_T_SCORE          GAME_STATUS_PLAY            //�з�״̬
#define GS_T_PLAYING        GAME_STATUS_PLAY+1            //��Ϸ����

#define IDM_ADMIN_MODIFY_STORAGE          WM_USER+1011
#define IDM_ADMIN_REQUEST_ADD_USERROSTER      WM_USER+1012
#define IDM_ADMIN_REQUEST_DELETE_USERROSTER     WM_USER+1013
#define IDM_REQUEST_UPDATE_USERROSTER       WM_USER+1014

//�ڰ�����
typedef enum {INVALID_ROSTER = 0, WHITE_ROSTER, BLACK_ROSTER} USERROSTER;

//�����û���Ϣ
typedef struct
{
  DWORD             dwGameID;             //GAMEID
  TCHAR             szNickName[LEN_NICKNAME];     //�û��ǳ�
  LONGLONG            lGameScore;             //��Ϸ��Ӯ��
  LONGLONG            lScoreLimit;            //�û����Ʒ���
  USERROSTER            userroster;             //�û�����
} ROOMUSERINFO;

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START        100                 //��Ϸ��ʼ
#define SUB_S_ADD_SCORE         101                 //��ע���
#define SUB_S_GIVE_UP         102                 //������ע
#define SUB_S_SEND_CARD         103                 //������Ϣ
#define SUB_S_GAME_END          104                 //��Ϸ����
#define SUB_S_COMPARE_CARD        105                 //���Ƹ�ע
#define SUB_S_LOOK_CARD         106                 //���Ƹ�ע
#define SUB_S_PLAYER_EXIT       107                 //�û�ǿ��
#define SUB_S_OPEN_CARD         108                 //������Ϣ
#define SUB_S_WAIT_COMPARE        109                 //�ȴ�����
#define SUB_S_ANDROID_CARD        110                 //������Ϣ
#define SUB_S_CHEAT_CARD        111                 //������Ϣ

#define SUB_S_ADMIN_STORAGE_INFO    112                 //ˢ�¿��
#define SUB_S_RESULT_ADD_USERROSTER   113                 //����û��������
#define SUB_S_RESULT_DELETE_USERROSTER  114                 //ɾ���û��������
#define SUB_S_UPDATE_USERROSTER     115                 //�����û�����
#define SUB_S_REMOVEKEY_USERROSTER    116                 //�Ƴ��û�����
#define SUB_S_DUPLICATE_USERROSTER    117                 //�ظ��û�����

struct CMD_S_DuplicateUserRoster
{
  DWORD             dwGameID;
  USERROSTER            userroster;             //�û�����
  LONGLONG            lScoreLimit;            //�û����Ʒ���
};

struct CMD_S_RemoveKeyUserRoster
{
  DWORD             dwUserID;
  DWORD             dwGameID;
  USERROSTER            userroster;             //�û�����
  LONGLONG            lScoreLimit;            //�û����Ʒ���
};

struct CMD_S_AddUserRoster_Result
{
  DWORD             dwGameID;
  DWORD             dwUserID;
  TCHAR             szNickName[LEN_NICKNAME];     //�û��ǳ�
  LONGLONG            lScoreLimit;            //�û����Ʒ���
  USERROSTER            userroster;             //�û�����
  LONGLONG            lGameScore;             //��Ϸ��Ӯ��
  bool              bSucceed;
};

struct CMD_S_DeleteUserRoster_Result
{
  DWORD             dwGameID;
  DWORD             dwUserID;
  bool              bFind;                //�ҵ���ʶ
  bool              bExistRoster;           //֮ǰ���ںڰ�������ʶ
  USERROSTER            beforeuserroster;         //֮ǰ�û�����
};

//�����û����������кڰ��������û���
struct CMD_S_UpdateUserRoster
{
  DWORD             dwUserID;             //�û�ID
  ROOMUSERINFO          roomUserInfo;
};

//���Ʒ������Ϣ
struct CMD_S_ADMIN_STORAGE_INFO
{
	LONGLONG  lCurrentStorage;
	LONGLONG  lCurrentDeduct;
	LONGLONG  lMaxStorage;
	WORD    wStorageMul;
};

struct tagRobotConfig
{
  LONGLONG            lRobotScoreMin;
  LONGLONG            lRobotScoreMax;
  LONGLONG            lRobotBankGet;
  LONGLONG            lRobotBankGetBanker;
  LONGLONG            lRobotBankStoMul;
};



//��Ϸ״̬
struct CMD_S_StatusFree
{
  LONGLONG              lCellScore;             //��������
  TCHAR               szGameRoomName[SERVER_LEN];     //��������
  tagRobotConfig            RobotConfig;            //����������
  LONGLONG              lStartStorage;            //��ʼ���
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
  //��ע��Ϣ
  LONGLONG              lMaxCellScore;            //��Ԫ����
  LONGLONG              lCellScore;             //��Ԫ��ע
  LONGLONG              lCurrentTimes;            //��ǰ����
  LONGLONG              lUserMaxScore;            //�û���������

  //״̬��Ϣ
  WORD                wBankerUser;            //ׯ���û�
  WORD                wCurrentUser;           //��ǰ���
  BYTE                cbPlayStatus[GAME_PLAYER];      //��Ϸ״̬
  bool                bMingZhu[GAME_PLAYER];        //����״̬
  LONGLONG              lTableScore[GAME_PLAYER];     //��ע��Ŀ
  TCHAR               szGameRoomName[SERVER_LEN];     //��������

  //�˿���Ϣ
  BYTE                cbHandCardData[MAX_COUNT];      //�˿�����

  //״̬��Ϣ
  bool                bCompareState;            //����״̬
  tagRobotConfig            RobotConfig;            //����������
  LONGLONG              lStartStorage;            //��ʼ���
};

//�������˿�
struct CMD_S_AndroidCard
{
  BYTE                cbRealPlayer[GAME_PLAYER];        //�������
  BYTE                cbAndroidStatus[GAME_PLAYER];     //������Ŀ
  BYTE                cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
  LONGLONG              lStockScore;              //��ǰ���
};


//�������˿�
struct CMD_S_CheatCardInfo
{
  BYTE                cbAllHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�
};



//��Ϸ��ʼ
struct CMD_S_GameStart
{
  //��ע��Ϣ
  LONGLONG              lMaxScore;              //�����ע
  LONGLONG              lCellScore;             //��Ԫ��ע
  LONGLONG              lCurrentTimes;            //��ǰ����
  LONGLONG              lUserMaxScore;            //��������

  //�û���Ϣ
  WORD                wBankerUser;            //ׯ���û�
  WORD                wCurrentUser;           //��ǰ���
  BYTE                cbPlayStatus[GAME_PLAYER];      //�û�״̬
};

//�û���ע
struct CMD_S_AddScore
{
  WORD                wCurrentUser;           //��ǰ�û�
  WORD                wAddScoreUser;            //��ע�û�
  WORD                wCompareState;            //����״̬
  LONGLONG              lAddScoreCount;           //��ע��Ŀ
  LONGLONG              lCurrentTimes;            //��ǰ����
};

//�û�����
struct CMD_S_GiveUp
{
  WORD                wGiveUpUser;            //�����û�
};

//�������ݰ�
struct CMD_S_CompareCard
{
  WORD                wCurrentUser;           //��ǰ�û�
  WORD                wCompareUser[2];          //�����û�
  WORD                wLostUser;              //�����û�
};

//�������ݰ�
struct CMD_S_LookCard
{
  WORD                wLookCardUser;            //�����û�
  BYTE                cbCardData[MAX_COUNT];        //�û��˿�
};

//�����˿�
struct CMD_S_SendCard
{
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //�û��˿�
};

//�������ݰ�
struct CMD_S_OpenCard
{
  WORD                wWinner;              //ʤ���û�
};

//��Ϸ����
struct CMD_S_GameEnd
{
  LONGLONG              lGameTax;             //��Ϸ˰��
  LONGLONG              lGameScore[GAME_PLAYER];      //��Ϸ�÷�
  BYTE                cbCardData[GAME_PLAYER][3];     //�û��˿�
  WORD                wCompareUser[GAME_PLAYER][4];   //�����û�
  WORD                wEndState;              //����״̬
  bool                bDelayOverGame;           //�ӳٿ�ʼ
  WORD                wServerType;            //��������
};

//�û��˳�
struct CMD_S_PlayerExit
{
  WORD                wPlayerID;              //�˳��û�
};

//�ȴ�����
struct CMD_S_WaitCompare
{
  WORD                wCompareUser;           //�����û�
};

//////////////////////////////////////////////////////////////////////////

//�ͻ�������ṹ
#define SUB_C_ADD_SCORE         1                 //�û���ע
#define SUB_C_GIVE_UP         2                 //������Ϣ
#define SUB_C_COMPARE_CARD        3                 //������Ϣ
#define SUB_C_LOOK_CARD         4                 //������Ϣ
#define SUB_C_OPEN_CARD         5                 //������Ϣ
#define SUB_C_WAIT_COMPARE        6                 //�ȴ�����
#define SUB_C_FINISH_FLASH        7                 //��ɶ���
#define SUB_C_ADD_SCORE_TIME      8                 //��ɶ���

#define SUB_C_MODIFY_STORAGE      10                  //�޸Ŀ��
#define SUB_C_REQUEST_ADD_USERROSTER  11                  //��������û�����
#define SUB_C_REQUEST_DELETE_USERROSTER 12                  //����ɾ���û�����
#define SUB_C_REQUEST_UPDATE_USERROSTER 13                  //��������û�����

//�������
struct CMD_C_Request_Add_Userroster
{
  DWORD           dwUserGameID;
  LONGLONG          lScoreLimit;
  bool            bWin;
};

//ɾ������
struct CMD_C_Request_Delete_Userroster
{
  DWORD           dwUserGameID;
};

struct CMD_C_ModifyStorage
{
  LONGLONG            lStorageCurrent;
  LONGLONG            lStorageDeduct;
  LONGLONG            lMaxStorage;            //�������
  WORD              wStorageMul;            //Ӯ�ָ���
};

//�û���ע
struct CMD_C_AddScore
{
  LONGLONG              lScore;               //��ע��Ŀ
  WORD                wState;               //��ǰ״̬
};

//�������ݰ�
struct CMD_C_CompareCard
{
  WORD                wCompareUser;           //�����û�
};

//////////////////////////////////////////////////////////////////////////


#pragma pack()

#endif
