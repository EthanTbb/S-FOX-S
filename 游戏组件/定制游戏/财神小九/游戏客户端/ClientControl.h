#pragma once

//游戏控制基类
class IClientControlDlg : public CDialog 
{
public:
	IClientControlDlg(UINT UID, CWnd* pParent) : CDialog(UID, pParent){}
	virtual ~IClientControlDlg(void){}

public:
	//更新控制
	virtual void  OnAllowControl(bool bEnable) = NULL;
	//申请结果
	virtual bool  ReqResult(const void * pBuffer) = NULL;

	//下注信息
	virtual bool  PlayersBet(BYTE cbViewIndex, LONGLONG lScoreCount) = NULL;
};
