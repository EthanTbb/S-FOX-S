#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��
//////////////////////////////////////////////////////////////////////////
//��������
#define INVALID_ITEM				0xFFFF								//��Ч����
//��ඨ��
#define DEF_X_DISTANCE				17									//Ĭ�ϼ��
#define DEF_Y_DISTANCE				20									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE			20									//Ĭ�ϼ��

//��ඨ��
#define DEF_X_DISTANCE_SMALL		17									//Ĭ�ϼ��
#define DEF_Y_DISTANCE_SMALL		20									//Ĭ�ϼ��
#define DEF_SHOOT_DISTANCE_SMALL	20									//Ĭ�ϼ��

#define SPACE_CARD_DATA				255									//����˿�

//��Ϣ����
#define	IDM_LEFT_HIT_CARD			(WM_USER+500)						//����˿�
#define	IDM_RIGHT_HIT_CARD			(WM_USER+501)						//�һ��˿�


//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����


//X ���з�ʽ
enum enXCollocateMode 
{ 
	enXLeft,						//�����
	enXCenter,						//�ж���
	enXRight,						//�Ҷ���
};

//Y ���з�ʽ
enum enYCollocateMode 
{ 
	enYTop,							//�϶���
	enYCenter,						//�ж���
	enYBottom,						//�¶���
};


//////////////////////////////////////////////////////////////////////////

//�˿˽ṹ
struct tagCardItem
{
	bool							bShoot;								//�����־
	BYTE							cbCardData;							//�˿�����
};

/////////////////////////////////////////////////////////////////////////
//�˿˿ؼ�
class CCardControl
{
	//״̬����
protected:
	bool							m_bSmallMode;						//С��ģʽ
	bool							m_bHorizontal;						//��ʾ����
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayItem;						//��ʾ��־
	bool							m_bShowControl;						//��ʾ�˿�

	//�˿�����
protected:
	WORD							m_wMaxSize;							//�����Ŀ
	WORD							m_wCardCount;						//�˿���Ŀ
	tagCardItem						m_CardItemArray[52];				//�˿�����

	//ѡ�����
protected:
	WORD							m_wEndIndex;						//��������
	WORD							m_wStartIndex;						//��ʼ����

	//�������
protected:
	UINT							m_nXDistance;						//������
	UINT							m_nYDistance;						//������
	UINT							m_nShootDistance;					//����߶�

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��Դ����
public:
	static bool						m_bLoad;							//���ر�ʶ
	static CSize					m_CardSize;							//�˿˴�С
	static CPngImage				m_ImageCard;						//ͼƬ��Դ
	static CPngImage				m_ImageHorCard;						//������ͼ
	
	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//�˿˿���
public:
	//�����˿�
	bool SetCardData(WORD wCardCount);
	//����ģʽ
	bool SetCardMode(bool bSmallMode);
	//�߼�����
	BYTE GetCardLogicValue(BYTE cbCardData);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);
	//�����˿�
	bool SetCardItem(const tagCardItem CardItemArray[], WORD wCardCount);
	//�����˿�
	bool SetCardItem(const BYTE cbCardData, WORD wIndex);
	//������Ŀ
	void SetMaxSize(WORD wMaxSize) { m_wMaxSize = wMaxSize;}

	//�����˿�
public:
	//�����˿�
	void ShootAllCard(bool bShoot);
	//�����˿�
	bool SetShootIndex(WORD wStartIndex, WORD wEndIndex);
	//�����˿�
	bool SetShootByIndex(const BYTE bCardDataIndex[], WORD wCardCount);
	//�����˿�
	bool SetSelectIndex(WORD wStartIndex, WORD wEndIndex);
	//�����˿�
	bool SetShootCard(const BYTE cbCardData[], WORD wCardCount);

	//�˿˿���
public:
	//ɾ���˿�
	bool RemoveShootItem();
	//�ƶ��˿�
	bool MoveCardItem(WORD wTargerItem);
	//�����˿�
	bool SwitchCardItem(WORD wSourceItem, WORD wTargerItem);

	//��ȡ�˿�
public:
	//��ȡ�˿�
	tagCardItem * GetCardFromIndex(WORD wIndex);
	//��ȡ�˿�
	tagCardItem * GetCardFromPoint(CPoint & MousePoint);

	//��ȡ�˿�
public:
	//�˿���Ŀ
	WORD GetCardCount() { return m_wCardCount; }
	//��ȡ�˿�
	WORD GetCardData(BYTE cbCardData[], WORD wBufferCount);
	//��ȡ�˿�
	WORD GetShootCard(BYTE cbCardData[], WORD wBufferCount);
	//��ȡ�˿�
	BYTE GetCardData(WORD wIndex);

	//������Դ
public:
	static void Load();
	//��ȡ��ͼ
	static  void GetCardImage(CPngImage **ppImage) { (*ppImage) = &m_ImageCard; }

	//״̬��ѯ
public:
	//��ѯ����
	bool GetDirection() { return m_bHorizontal; }
	//��ѯ��Ӧ
	bool GetPositively() { return m_bPositively; }
	//��ѯ��ʾ
	bool GetDisplayItem() { return m_bDisplayItem; }

	//״̬����
public:
	//���÷���
	VOID SetDirection(bool bHorizontal) { m_bHorizontal=bHorizontal; }
	//������Ӧ
	VOID SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//������ʾ
	VOID SetDisplayFlag(bool bDisplayItem) { m_bDisplayItem=bDisplayItem; }
	//���þ���
	VOID SetCardSpace(UINT nXDistance, UINT nYDistance, UINT nShootDistance);
	//��ʾ�˿�
	void ShowWindow(int nShow) { m_bShowControl = (nShow==SW_SHOW?true:false); }

	//�ؼ�����
public:
	//��ȡ��׼
	VOID GetBenchmarkPos(CPoint & BenchmarkPos) { BenchmarkPos = m_BenchmarkPos; };
	//��׼λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	VOID SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//�滭�˿�
	VOID DrawCardControl(CDC * pDC);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point);

	//�ڲ�����
public:
	//��ȡ��С
	VOID GetCardSize(CSize & CardSize);
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize);
	//��ȡԭ��
	VOID GetOriginPoint(CPoint & OriginPoint);
	//��ȡβ��
	VOID GetTailPoint(CPoint  & TailPoint);
	//��ȡ����
	VOID GetCenterPoint(CPoint & CenterPoint);
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);	
};

//////////////////////////////////////////////////////////////////////////

#endif