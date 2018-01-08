#ifndef DLG_TASKDETAILS_HEAD_FILE
#define DLG_TASKDETAILS_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//帮助窗口
class CDlgTaskDetails  : public CFGuiDialog
{
protected:
	tagTaskStatus *							m_pTaskStatus;						//任务状态	
	tagTaskParameter *						m_pTaskParameter;					//任务参数	
	//函数定义
public:
	//构造函数
	CDlgTaskDetails ();
	//析构函数
	virtual ~CDlgTaskDetails ();

	//重载函数
protected:
	//创建函数
	virtual BOOL OnInitDialog();
	//确定函数
	virtual VOID OnOK();
	//取消消息
	virtual VOID OnCancel();

	//接口函数
public:
	//消息提醒
    virtual void Notify(TNotifyUI &  msg);
	
	//重载函数
public:
	//皮肤名称
	virtual LPCTSTR GetSkinFile();

public:
	//设置参数
	void SetTaskParameter(tagTaskParameter * pTaskParameter,tagTaskStatus * pTaskStatus);

	//刷新控件
	void UpdateDetails();

};

//////////////////////////////////////////////////////////////////////////////////

#endif