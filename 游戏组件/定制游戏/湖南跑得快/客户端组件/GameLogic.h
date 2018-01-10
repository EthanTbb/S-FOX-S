#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_SINGLE_LINE				2									//��������
#define CT_DOUBLE_LINE				3									//��������
#define CT_THREE_LINE				4									//��������
#define CT_THREE_LINE_TAKE_SINGLE	5									//����һ��
#define CT_THREE_LINE_TAKE_DOUBLE	6									//����һ��
#define CT_BOMB						7									//ը������

//�궨��
#define MAX_COUNT					16									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							bFourCount;							//������Ŀ
	BYTE 							bThreeCount;						//������Ŀ
	BYTE 							bDoubleCount;						//������Ŀ
	BYTE							bSignedCount;						//������Ŀ
	BYTE 							bFourLogicVolue[4];					//�����б�
	BYTE 							bThreeLogicVolue[5];				//�����б�
	BYTE 							bDoubleLogicVolue[8];				//�����б�
	BYTE 							bSignedLogicVolue[16];				//�����б�
	BYTE							bFourCardData[MAX_COUNT];			//�����б�
	BYTE							bThreeCardData[MAX_COUNT];			//�����б�
	BYTE							bDoubleCardData[MAX_COUNT];			//�����б�
	BYTE							bSignedCardData[MAX_COUNT];			//������Ŀ
};

//���ƽ��
struct tagOutCardResult
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbResultCard[MAX_COUNT];			//����˿�
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[48];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);
	//�����ж�
	bool SearchOutCard(BYTE bCardData[], BYTE bCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, tagOutCardResult & OutCardResult);
};

//////////////////////////////////////////////////////////////////////////

#endif