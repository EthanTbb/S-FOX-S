#pragma once
#include "../���������/ServerControl.h"

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����


class CServerControlItemSink : public IServerControl
{
public:
	CServerControlItemSink(void);
	virtual ~CServerControlItemSink(void);

public:
	//����������
	virtual bool __cdecl ServerControl(BYTE cbHandCardData[GAME_PLAYER][5], ITableFrame * pITableFrame);

	//��ȡ����Ϣ
	CString GetCradInfo( BYTE cbCardData );
};
