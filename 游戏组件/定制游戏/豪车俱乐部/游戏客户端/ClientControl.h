#pragma once

//游戏控制基类
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}
 CUserBetArray					m_UserBetArray;					//用户下注
public:
	//更新控制
	virtual void  UpdateControl(CMD_S_ControlReturns* pControlReturns) = NULL;
	//初始控件
	virtual bool  InitControl()=NULL;

	//更新库存
	virtual bool __cdecl UpdateStorage(const void * pBuffer) = NULL;
	//更新下注
	virtual void __cdecl UpdateUserBet(bool bReSet) = NULL;
	//清除信息
	virtual void ClearText()=NULL;
	//更新控件
	virtual void __cdecl UpdateControl()=NULL;
	//设定当前库存
	virtual void  SetStorageCurrentValue(LONGLONG lvalue)=NULL;
	//清理列表
	virtual void ClearClist()=NULL;
	//更新列表
	virtual void UpdateUserList(bool bDelUser=false)=NULL;
	//下注统计
	virtual void UpdateAllUserBet(LONGLONG lAllUserBet[AREA_COUNT+1],bool bClear=false)=NULL;
};
