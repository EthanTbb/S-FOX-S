#pragma once

//��Ϸ���ƻ���
class IServerControl
{
public:
	IServerControl(void){};
	virtual ~IServerControl(void){};

public:
	//����������
	virtual bool __cdecl ServerControl(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame, bool m_bIsReady[]) = NULL;
	virtual bool __cdecl ServerControl(CMD_S_CheatCard *pCheatCard, ITableFrame * pITableFrame, IServerUserItem * pIServerUserItem) = NULL;
};
