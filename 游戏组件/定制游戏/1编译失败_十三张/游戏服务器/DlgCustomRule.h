#ifndef GAME_DLG_CUSTOM_RULE_HEAD_FILE
#define GAME_DLG_CUSTOM_RULE_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////

//���ýṹ
struct tagCustomRule
{
	//ʱ�䶨��
	BYTE							cbTimeStartGame;					//��ʼʱ��
	BYTE							cbTimeCallBanker;					//��ׯʱ��
	BYTE							cbTimeSetChip;						//��עʱ��
	BYTE							cbTimeRangeCard;					//����ʱ��
	BYTE							cbTimeShowCard;						//����ʱ��
};

//////////////////////////////////////////////////////////////////////////////////

//���ô���
class CDlgCustomRule : public CDialog
{
	//���ñ���
protected:
	tagCustomRule					m_CustomRule;						//���ýṹ

	//��������
public:
	//���캯��
	CDlgCustomRule();
	//��������
	virtual ~CDlgCustomRule();

	//���غ���
protected:
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//���ܺ���
public:
	//���¿ؼ�
	bool FillDataToControl();
	//��������
	bool FillControlToData();

	//���ú���
public:
	//��ȡ����
	bool GetCustomRule(tagCustomRule & CustomRule);
	//��������
	bool SetCustomRule(tagCustomRule & CustomRule);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif