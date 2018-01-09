#ifndef CMD_OX_HEAD_FILE
#define CMD_OX_HEAD_FILE

#pragma pack(push)
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID             36                  //��Ϸ I D
#define GAME_PLAYER           6                 //��Ϸ����
#define GAME_NAME           TEXT("ͨ��ţţ")          //��Ϸ����
#define MAX_COUNT           5                 //�����Ŀ
#define MAX_JETTON_AREA         6                 //��ע����
#define MAX_TIMES           5                 //�������

#define VERSION_SERVER          PROCESS_VERSION(7,0,1)        //����汾
#define VERSION_CLIENT          PROCESS_VERSION(7,0,1)        //����汾

//����ԭ��
#define GER_NO_PLAYER         0x10                //û�����

//��Ϸ״̬
#define GS_TK_FREE            GAME_STATUS_FREE          //�ȴ���ʼ
#define GS_TK_PLAYING         GAME_STATUS_PLAY          //��Ϸ����
#define SERVER_LEN            32


//////////////////////////////////////////////////////////////////////////////////////
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

//////////////////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START        100                 //��Ϸ��ʼ
#define SUB_S_PLAYER_EXIT       101                 //�û�ǿ��
#define SUB_S_SEND_CARD         102                 //������Ϣ
#define SUB_S_GAME_END          103                 //��Ϸ����
#define SUB_S_OPEN_CARD         104                 //�û�̯��
#define SUB_S_ADMIN_STORAGE_INFO    105                 //ˢ�¿��
#define SUB_S_RESULT_ADD_USERROSTER   106                 //����û��������
#define SUB_S_RESULT_DELETE_USERROSTER  107                 //ɾ���û��������
#define SUB_S_UPDATE_USERROSTER     108                 //�����û�����
#define SUB_S_REMOVEKEY_USERROSTER    109                 //�Ƴ��û�����
#define SUB_S_DUPLICATE_USERROSTER    110                 //�ظ��û�����

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

struct tagRobotConfig
{
  LONGLONG            lRobotScoreMin;
  LONGLONG            lRobotScoreMax;
  LONGLONG            lRobotBankGet;
  LONGLONG            lRobotBankGetBanker;
  LONGLONG            lRobotBankStoMul;
};

//���Ʒ������Ϣ
struct CMD_S_ADMIN_STORAGE_INFO
{
  LONGLONG  lCurrentStorage;
  LONGLONG  lCurrentDeduct;
  LONGLONG  lMaxStorage;
  WORD    wStorageMul;
};

//��Ϸ״̬
struct CMD_S_StatusFree
{
  //��Ϸ����
  LONGLONG              lCellScore;             //��������
  TCHAR               szGameRoomName[SERVER_LEN];     //��������

  //��ʷ����
  LONGLONG              lTurnScore[GAME_PLAYER];      //������Ϣ
  LONGLONG              lCollectScore[GAME_PLAYER];     //������Ϣ
  tagRobotConfig            RobotConfig;            //����������
  LONGLONG              lStartStorage;            //��ʼ���
  LONGLONG              lBonus;
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
  //״̬��Ϣ
  BYTE                                cbDynamicJoin;                      //��̬����
  BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬
  LONGLONG              lTableScore[GAME_PLAYER];     //��ע��Ŀ
  LONGLONG              lCellScore;             //��������
  WORD                wBankerUser;            //ׯ���û�
  TCHAR               szGameRoomName[SERVER_LEN];     //��������

  //�˿���Ϣ
  BYTE                bOxCard[GAME_PLAYER];       //ţţ����
  BYTE                cbOxCardData[GAME_PLAYER][MAX_COUNT];//ţţ�˿�
  BYTE                cbHandCardData[GAME_PLAYER][MAX_COUNT];//�����˿�


  //��ʷ����
  LONGLONG              lTurnScore[GAME_PLAYER];      //������Ϣ
  LONGLONG              lCollectScore[GAME_PLAYER];     //������Ϣ
  tagRobotConfig            RobotConfig;            //����������
  LONGLONG              lStartStorage;            //��ʼ���
  LONGLONG              lBonus;
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
  WORD                wBankerUser;            //ׯ���û�
  BYTE                                cbPlayStatus[GAME_PLAYER];          //�û�״̬
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //�û��˿�
  LONGLONG              lCellScore;             //��Ϸ�׷�
};

//��Ϸ����
struct CMD_S_GameEnd
{
  LONGLONG              lGameTax[GAME_PLAYER];        //��Ϸ˰��
  LONGLONG              lGameScore[GAME_PLAYER];      //��Ϸ�÷�
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //�û��˿�
  BYTE                cbDelayOverGame;
  bool                bfiveKing[GAME_PLAYER];       //�廨ţ��ʶ
};

//�������ݰ�
struct CMD_S_SendCard
{
  BYTE                cbCardData[GAME_PLAYER][MAX_COUNT]; //�û��˿�
};

//�û��˳�
struct CMD_S_PlayerExit
{
  WORD                wPlayerID;              //�˳��û�
};

//�û�̯��
struct CMD_S_Open_Card
{
  WORD                wPlayerID;              //̯���û�
  BYTE                bOpen;                //̯�Ʊ�־
};
//////////////////////////////////////////////////////////////////////////

//�ͻ�������ṹ
#define SUB_C_OPEN_CARD         5                 //�û�̯��
#define SUB_C_MODIFY_STORAGE      6                 //�޸Ŀ��
#define SUB_C_REQUEST_ADD_USERROSTER  7                 //��������û�����
#define SUB_C_REQUEST_DELETE_USERROSTER 8                 //����ɾ���û�����
#define SUB_C_REQUEST_UPDATE_USERROSTER 9                 //��������û�����

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

//�û�̯��
struct CMD_C_OxCard
{
  BYTE                bOX;                //ţţ��־
  BYTE                cbOxCardData[MAX_COUNT];      //ţţ�˿�
};

struct CMD_C_ModifyStorage
{
  LONGLONG            lStorageCurrent;
  LONGLONG            lStorageDeduct;
  LONGLONG            lMaxStorage;            //�������
  WORD              wStorageMul;            //Ӯ�ָ���
};

#pragma pack(pop)

#endif
