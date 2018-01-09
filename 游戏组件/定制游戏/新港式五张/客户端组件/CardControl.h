#ifndef CARD_CONTROL_HEAD_FILE
#define CARD_CONTROL_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//�궨��

//���Զ���
#define MAX_CARD_COUNT				5									//�˿���Ŀ
#define DEF_X_DISTANCE				21									//Ĭ�ϼ��

//��ֵ����
#define	CARD_MASK_COLOR				0xF0								//��ɫ����
#define	CARD_MASK_VALUE				0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

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

//�˿˿ؼ�
class CCardControl
{
	//״̬����
protected:
	bool							m_bPositively;						//��Ӧ��־
	bool							m_bDisplayHead;						//��ʾ��־

	//�˿�����
protected:
	WORD							m_wCardCount;						//�˿���Ŀ
	BYTE							m_cbCardData[MAX_CARD_COUNT];		//�˿�����

	//�������
protected:
	UINT							m_nXDistance;						//������

	//λ�ñ���
protected:
	CPoint							m_BenchmarkPos;						//��׼λ��
	enXCollocateMode				m_XCollocateMode;					//��ʾģʽ
	enYCollocateMode				m_YCollocateMode;					//��ʾģʽ

	//��Դ����
protected:
	CSize							m_CardSize;							//�˿˴�С
	CPngImage						m_ImageCard;						//ͼƬ��Դ

	//��������
public:
	//���캯��
	CCardControl();
	//��������
	virtual ~CCardControl();

	//�˿˿���
public:
	//�˿���Ŀ
	WORD GetCardCount() { return m_wCardCount; }
	//��ȡ�˿�
	WORD GetCardData(BYTE cbCardData[], WORD wBufferCount);
	//�����˿�
	bool SetCardData(const BYTE cbCardData[], WORD wCardCount);

	//״̬��ѯ
public:
	//��ȡ��С
	CSize GetCardSize() { return m_CardSize; }
	//��ѯ��Ӧ
	bool GetPositively() { return m_bPositively; }
	//��ѯ��ʾ
	bool GetDisplayHead() { return m_bDisplayHead; }

	//״̬����
public:
	//���þ���
	VOID SetXDistance(UINT nXDistance) { m_nXDistance=nXDistance; }
	//������Ӧ
	VOID SetPositively(bool bPositively) { m_bPositively=bPositively; }
	//������ʾ
	VOID SetDisplayHead(bool bDisplayHead) { m_bDisplayHead=bDisplayHead; }
	//��ȡ��βλ��
	CPoint GetTailPos();
	//��ȡ���ĵ�
	VOID GetCenterPoint(CPoint & CenterPoint);
	
	//�ؼ�����
public:
	//��׼λ��
	VOID SetBenchmarkPos(INT nXPos, INT nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);
	//��׼λ��
	VOID SetBenchmarkPos(const CPoint & BenchmarkPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode);

	//�¼�����
public:
	//��ʼ��
	VOID Initialize(bool bSmall);
	//�滭�˿�
	VOID DrawCardControl(CDC * pDC);
	//�����Ϣ
	bool OnEventSetCursor(CPoint Point);
	//�����Ϣ
	bool OnEventLeftMouseDown(CPoint Point);

	//�ڲ�����
private:
	//��ȡ��С
	VOID GetControlSize(CSize & ControlSize);
	//�����л�
	WORD SwitchCardPoint(CPoint & MousePoint);
	//��ȡԭ��
	VOID GetOriginPoint(CPoint & OriginPoint);
};

//////////////////////////////////////////////////////////////////////////

#endif