#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//������

//��Ϸ����
#define KIND_ID						601									//��Ϸ I D
#define GAME_NAME					TEXT("�����ܵÿ�")					//��Ϸ����

//�������
#define GAME_PLAYER					3									//��Ϸ����
#define VERSION_SERVER				PROCESS_VERSION(7,0,1)				//����汾
#define VERSION_CLIENT				PROCESS_VERSION(7,0,1)				//����汾

//////////////////////////////////////////////////////////////////////////////////
//״̬����

#define GAME_SCENE_FREE				GAME_STATUS_FREE					//�ȴ���ʼ
#define GAME_SCENE_PLAY				GAME_STATUS_PLAY					//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD				103									//��������
#define SUB_S_GAME_END				104									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONGLONG						lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONGLONG						lBaseScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[16];						//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bBombCount[3];						//ը����Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[16];					//�����б�
	LONGLONG                        lAllTurnScore[3];					//�ֵܾ÷�
	LONGLONG                        lLastTurnScore[3];					//�Ͼֵ÷�
};

//�����˿�
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[16];						//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[16];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONGLONG						lGameTax;							//��Ϸ˰��
	LONGLONG						lGameScore[3];						//��Ϸ����
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bCardData[48];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[16];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////

#endif