#pragma once

#define MAX_CARD					2

//��Ϸ���ƻ���
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//����������
	virtual bool  ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame) = NULL;

	//��Ҫ����
	virtual bool  NeedControl() = NULL;

	//��ɿ���
	virtual bool  CompleteControl() = NULL;

	//��ʼ����
	virtual void  GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] ) = NULL;
};
