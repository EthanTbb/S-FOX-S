#include "StdAfx.h"
#include "servercontrolitemsink.h"


CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbExcuteTimes = 0;								
	ZeroMemory(m_cbControlArea, sizeof(m_cbControlArea));
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//����������
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{
	//��������й���ԱȨ�� �򷵻ش���
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
	{
		return false;
	}

	// ��Ϣװ��
	const CMD_C_AdminControl* pAdminControl = static_cast<const CMD_C_AdminControl*>(pDataBuffer);

	// ������Ϣ
	CMD_S_AdminControl SAdminControl;

	if ( pAdminControl->cbControlArea[BANKER_INDEX] 
		&& (pAdminControl->cbControlArea[SHUN_MEN_INDEX] || pAdminControl->cbControlArea[DUI_MEN_INDEX] || pAdminControl->cbControlArea[DAO_MEN_INDEX]) )
	{
		//"���ô���";
		SAdminControl.cbControlType = 0;
		pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_AMDIN_COMMAND_RESULT, &SAdminControl, sizeof(SAdminControl) );
		return false;
	}

	if ( pAdminControl->cbControlArea[BANKER_INDEX] )
	{
		//ȡ������
		m_cbExcuteTimes = 0;

		//����ȡ����Ϣ
		SAdminControl.cbControlType = 1;
		pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_AMDIN_COMMAND_RESULT, &SAdminControl, sizeof(SAdminControl) );
		return true;
	}

	//���ÿ���
	m_cbExcuteTimes = 1;
	CopyMemory( m_cbControlArea, pAdminControl->cbControlArea, sizeof(m_cbControlArea) );

	//���ͳɹ�
	SAdminControl.cbControlType = 2;
	pITableFrame->SendUserItemData(pIServerUserItem, SUB_S_AMDIN_COMMAND_RESULT, &SAdminControl, sizeof(SAdminControl) );

	return true;
}

//��Ҫ����
bool  CServerControlItemSink::NeedControl()
{
	if( m_cbExcuteTimes > 0 )
	{
		return true;
	}

	return false;
}

//��ɿ���
bool  CServerControlItemSink::CompleteControl()
{
	if (m_cbExcuteTimes > 0)
		m_cbExcuteTimes--;	

	if ( m_cbExcuteTimes == 0)
	{
		ZeroMemory(m_cbControlArea, sizeof(m_cbControlArea));
	}

	return true;
}


//��������
void  CServerControlItemSink::GetSuitResult( BYTE cbTableCardArray[][MAX_CARD], BYTE cbTableCard[], LONGLONG lAllJettonScore[] )
{
	BYTE cbCardArray[MAX_INDEX][MAX_CARD];
	CopyMemory(cbCardArray, cbTableCardArray, sizeof(cbCardArray));

	for ( int i = 0; i < MAX_INDEX; ++i )
	{
		for ( int j = 0; j < MAX_INDEX; ++j )
		{
			if ( m_GameLogic.CompareCard( cbCardArray[j], 2, cbCardArray[i], 2) == 1 )
			{
				BYTE cbCardTemp[MAX_CARD];
				CopyMemory(cbCardTemp, cbCardArray[i], sizeof(BYTE) * MAX_CARD);
				CopyMemory(cbCardArray[i], cbCardArray[j], sizeof(BYTE) * MAX_CARD);
				CopyMemory(cbCardArray[j], cbCardTemp, sizeof(BYTE) * MAX_CARD);
			}
		}
	}

	int nArrayCount = 0;

	//Ӯȡ����
	for ( int i = 0; i < MAX_INDEX; ++i )
	{
		if ( m_cbControlArea[i] )
		{
			CopyMemory(cbTableCardArray[i], cbCardArray[nArrayCount], sizeof(BYTE) * MAX_CARD);
			nArrayCount++;
		}
	}

	//������
	for ( int i = 0; i < MAX_INDEX; ++i )
	{
		if ( !m_cbControlArea[i] )
		{
			CopyMemory(cbTableCardArray[i], cbCardArray[nArrayCount], sizeof(BYTE) * MAX_CARD);
			nArrayCount++;
		}
	}
}

