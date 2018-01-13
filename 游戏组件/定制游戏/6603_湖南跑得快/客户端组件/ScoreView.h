#ifndef SCORE_VIEW_HEAD_FILE
#define SCORE_VIEW_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//������ͼ��
class CScoreView : public CWnd
{
	//��������
protected:
	LONGLONG							m_lTax;								//��Ϸ˰��
	LONGLONG							m_lScore[GAME_PLAYER];				//��Ϸ�÷�
	TCHAR								m_szUserName[GAME_PLAYER][LEN_NICKNAME];//�û�����

	//��Դ����
protected:
	CBitImage							m_ImageBack;						//����ͼ��

	//��������
public:
	//���캯��
	CScoreView(void);
	//��������
	virtual ~CScoreView(void);
	
	//���ܺ���
public:
	//���û���
	void ResetScore();
	//����˰��
	void SetTax(LONGLONG lTax);
	//���û���
	void SetGameScore(WORD wChairID, LPCTSTR pszUserName, LONGLONG lScore);

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif