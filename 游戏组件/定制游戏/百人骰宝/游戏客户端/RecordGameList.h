#ifndef RECORD_GAME_LIST_HEAD_FILE
#define RECORD_GAME_LIST_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "../��Ϸ������/GameLogic.h"
//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
struct GameRecord
{
	//��Ϸ��Ϣ
	WORD							wDrawCount;							//��Ϸ����	
	BYTE							cbWinerSide;						//ʤ�����
	BYTE							cbPlayerPoint;						//�м��Ƶ�
	BYTE							cbBankerPoint;						//ׯ���Ƶ�	

	//�ҵ���Ϣ
	SCORE						lGameScore;							//��Ϸ�ɼ�	
	SCORE						lMyAddGold;							//��Ϸ��ע	

	//��ע����
	SCORE						lDrawTieScore;						//��ƽ��ע
	SCORE						lDrawBankerScore;					//��ׯ��ע
	SCORE						lDrawPlayerScore;					//������ע
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��¼
class CGameRecord : public CSkinDialog
{
	//�ؼ�����
public:
	CSkinListCtrl					m_RecordGameList;					//��Ϸ�б�

	//��������
public:
	//���캯��
	CGameRecord();
	//��������
	virtual ~CGameRecord();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK() { return; }
	//ȡ����Ϣ
	virtual void OnCancel() { return; }

	//���ܺ���
public:
	//�����б�
	void FillGameRecourd(GameRecord & GameRecord);

	//��Ϣӳ��
protected:
	//λ�ñ仯
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif
