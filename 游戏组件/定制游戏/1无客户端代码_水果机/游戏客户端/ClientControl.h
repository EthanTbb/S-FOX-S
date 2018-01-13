#pragma once

struct PLAYER_INFO
{
	LPCWSTR		szNickName;
	DWORD		dwPlayerID;
	LONGLONG	lPlayerExchangeGold;
	LONGLONG	lUserJettonScore[AREA_COUNT+1];
};
//��Ϸ���ƻ���
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//���¿���
	virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
	virtual void UpdateClistCtrl(LONGLONG lAllPlayBet[GAME_PLAYER][AREA_COUNT],BYTE cbViewIndex, LONGLONG lScoreCount, WORD wChair, WORD wViewChairID,IClientUserItem* pUserData)=NULL;
	virtual void ClearClist()=NULL;
	virtual void SetStorageInfo(CMD_S_ADMIN_STORAGE_INFO *pStorageInof) = NULL;
};
