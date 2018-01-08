#ifndef DLG_UPDATE_STATUS_HEAD_FILE
#define DLG_UPDATE_STATUS_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////////////

//类型声明
class CDownLoadTaskSink;
class CUpdateCheckSink;
class CWndUpdateMessage;
class CUpdateStatusView;

////////////////////////////////////////////////////////////////////////////////////////////////
struct tagUpdateUICheck
{
	TCHAR								szCheckFile[MAX_PATH];		//检查文件
	WORD								wCurrNum;					//当前数量
	WORD								wTotalNum;					//总共数量
};

struct tagUpdateUIDownload
{
	TCHAR								szCheckFile[MAX_PATH];		//下载文件
	DWORD								dwDownSpeed;				//下载速度
	DWORD								dwDownLoadSize;				//下载大小
	DWORD								dwTotalFileSize;			//文件大小
};

////////////////////////////////////////////////////////////////////////////////////////////////
//下载信息
struct tagDownloadItem
{
	TCHAR								szDownloadFile[MAX_PATH];		//下载文件
	TCHAR								szFileCheckSum[LEN_MD5];		//文件校验和
};

//类型定义
typedef CWHArray<tagDownloadItem *>		CDownloadItemArray;				//下载子项

//接口定义
interface IUpdateStatusSink
{
	//更新事件
public:
	//下载完成
	virtual VOID OnEventDownLoadTaskFinish()=NULL;
	//检测完成
	virtual VOID OnEventUpdateCheckFinish(bool bNeedUpdate)=NULL;	
};

//接口定义
interface IUpdateViewSink
{
	//下载事件
public:	
	//更新消息
	virtual VOID OnEventDownLoadTaskUpdate(bool bUpdate)=NULL;
	//完成消息
	virtual VOID OnEventDownLoadTaskFinish(bool bSuccessed)=NULL;

	//检查事件
public:
	//检查出错
	virtual VOID OnEventUpdateCheckError()=NULL;	
	//完成事件
	virtual VOID OnEventUpdateCheckFinish()=NULL;
	//地址事件
	virtual VOID OnEventUpdateCheckUpdateInfo(tagUpdateInfo * UpdateInfo)=NULL;
	//添加事件
	virtual VOID OnEventUpdateCheckUpdateItem(tagUpdateItem * UpdateItem)=NULL;

public:
	//更新检查
	virtual VOID OnEventUpdateUICheck(tagUpdateUICheck * UIInfo)=NULL;
	//更新下载
	virtual VOID OnEventUpdateUIDownload(tagUpdateUIDownload * UIInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//下载回调
class CDownLoadTaskSink : public CWnd,public IDownLoadSink
{	
	friend class  CUpdateStatusView;

	//状态变量
protected:
	BYTE							m_cbDownLoadStatus;					//下载状态

	//下载速度
protected:
	DWORD							m_dwDownSpeed;						//下载速度
	DWORD							m_dwLastCalcSize;					//上次大小
	DWORD							m_dwLastCalcTime;					//上次时间

	//文件信息
protected:
	DWORD							m_dwDownLoadSize;					//下载大小
	DWORD							m_dwTotalFileSize;					//文件大小

	//任务属性
protected:	
	TCHAR							m_szTaskName[MAX_PATH];				//任务名字

	//内核信息
protected:
	CFile							m_DownFile;							//下载文件
	CDownLoad						m_DownLoad;							//下载组件
	CWndUpdateMessage *				m_pUpdateMessage;					//更新窗口

	//函数定义
public:
	//构造函数
	CDownLoadTaskSink();
	//析构函数
	virtual ~CDownLoadTaskSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { return; }
	//接口查询
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//状态接口
public:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//序列接口
public:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//下载信息
	virtual bool OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//辅助函数
protected:
	//关闭下载
	bool CloseDownLoad();
	//重置变量
	VOID ResetDownLoadSink();
	//加载文件
	bool LoadLocationFile(LPCTSTR pszLocation);
	//创建文件
	bool CreateDownLoadFile(LPCTSTR pszLocation);		
	//执行下载
	bool PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad=false);
};

//////////////////////////////////////////////////////////////////////////////////

//更新回调
class CUpdateCheckSink : public CWnd, public IDownLoadSink
{
	//友员定义
	friend class CUpdateStatusView;		

	//状态变量
protected:
	volatile bool					m_bCheckData;						//校验数据
	HANDLE 							m_hCheckThread;   					//校验线程
	BYTE							m_cbDownLoadStatus;					//下载状态

	//文件信息
protected:
	DWORD							m_dwDownLoadSize;					//下载大小
	DWORD							m_dwTotalFileSize;					//文件大小

	//内核信息
protected:	
	BYTE *							m_pFileData;						//文件数据
	CDownLoad						m_DownLoad;							//下载组件
	CWndUpdateMessage *				m_pUpdateMessage;					//更新窗口

	//函数定义
public:
	//构造函数
	CUpdateCheckSink();
	//析构函数
	virtual ~CUpdateCheckSink();

	//基础接口
public:
	//释放对象
	virtual VOID  Release() { return; }
	//接口查询
	virtual VOID *  QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能函数
public:
	//关闭下载
	bool CloseDownLoad();
	//执行下载
	bool PerformDownLoad(LPCTSTR pszDownLoadFile);

	//状态接口
public:
	//下载异常
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//下载状态
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//序列接口
public:
	//下载数据
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//下载信息
	virtual bool OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//静态函数
private:
	//校验函数
	static VOID PerformCheckSum(LPVOID pThreadData);
};

//////////////////////////////////////////////////////////////////////////////////
//更新消息
class CWndUpdateMessage : public CWnd
{
	//接口变量
protected:
	IUpdateViewSink *				m_pIUpdateViewSink;					//更新视图			

	//函数定义
public:
	//构造函数
	CWndUpdateMessage();
	//析构函数
	virtual ~CWndUpdateMessage();

	//功能函数
public:
	//设置接口
	VOID SetUpdateViewSink(IUpdateViewSink * pIUpdateViewSink);

	//自定消息
protected:
	//完成消息
	LRESULT OnMessageDownLoadTaskFinish(WPARAM wParam, LPARAM lParam);
	//更新消息
	LRESULT OnMessageDownLoadTaskUpdate(WPARAM wParam,LPARAM lParam);
	//出错事件
	LRESULT OnMessageUpdateCheckError(WPARAM wParam, LPARAM lParam);	
	//完成事件
	LRESULT OnMessageUpdateCheckFinish(WPARAM wParam, LPARAM lParam);
	//地址事件
	LRESULT OnMessageUpdateCheckUpdateInfo(WPARAM wParam, LPARAM lParam);
	//添加事件
	LRESULT OnMessageUpdateCheckUpdateItem(WPARAM wParam, LPARAM lParam);	

	//更新检查
	LRESULT OnMessageUpdateUICheck(WPARAM wParam, LPARAM lParam);
	//更新下载
	LRESULT OnMessageUpdateUIDownload(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////////////

//视图子项
class CUpdateStatusView : public CFGuiDialog,public IUpdateViewSink
{
	
protected:
	//下载目录
	TCHAR                           m_szDownDirectory[64];				//下载目录
	//状态变量
protected:
	UINT							m_uImageIndex;						//位图位置	

	//下载变量
protected:
	DWORD							m_dwTickCount;						//时间计数
	DWORD                           m_dwTotalTaskCount;                 //任务总数
	DWORD                           m_dwTaskFinished;                   //完成任务
	BYTE							m_cbCurrRetryCount;					//重试次数

	//描述变量
protected:
	TCHAR                           m_szFileName[64];					//文件名称 	
	TCHAR							m_szDescribe[128];					//状态描述
	TCHAR							m_szUpdateFile[MAX_PATH];			//更新文件

	//任务信息
protected:
	TCHAR                           m_szDownLoadAddress[MAX_PATH];      //下载地址 
	CDownloadItemArray              m_DownLoadFileArray;                //下载列表

	//指针变量
protected:
	CUpdateCheckSink *              m_pUpdateCheckSink;                 //回调对象
	CDownLoadTaskSink *             m_pDownLoadTaskSink;                //回调对象

	//控件变量
protected:
	CWndUpdateMessage 				m_UpdateMessage;					//消息窗口	

	//接口变量
protected:
	IUpdateStatusSink *				m_pIUpdateStatusSink;				//回调接口

	//函数定义
public:
	//构造函数
	CUpdateStatusView();
	//析构函数
	virtual ~CUpdateStatusView();

	//重载函数
protected:
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();

	//接口函数
public:
	//初始控件
	virtual void InitControlUI();
	//消息提醒
	virtual void Notify(TNotifyUI &  msg);	
	//皮肤名称
	virtual LPCTSTR GetSkinFile();



	//下载事件
public:	
	//更新消息
	virtual VOID OnEventDownLoadTaskUpdate(bool bUpdate);
	//完成消息
	virtual VOID OnEventDownLoadTaskFinish(bool bSuccessed);

	//检查事件
public:
	//检查出错
	virtual VOID OnEventUpdateCheckError();	
	//完成事件
	virtual VOID OnEventUpdateCheckFinish();
	//更新地址
	virtual VOID OnEventUpdateCheckUpdateInfo(tagUpdateInfo * UpdateInfo);
	//更新子项
	virtual VOID OnEventUpdateCheckUpdateItem(tagUpdateItem * UpdateItem);
public:
	//更新检查
	virtual VOID OnEventUpdateUICheck(tagUpdateUICheck * UIInfo);
	//更新下载
	virtual VOID OnEventUpdateUIDownload(tagUpdateUIDownload * UIInfo);

	//功能函数
public:
	//结束更新
	VOID EndUpdateStatus();
	//初始更新
	VOID InitUpdateStatus(IUpdateStatusSink * pIUpdateStatusSink, LPCTSTR pszProgressName);

	//设置目录
	VOID SetDownDirectory(LPCTSTR pszProgressName);

	//辅助函数
protected:	
	//启动检查
	VOID StartUpdateCheck();
	//启动下载
	VOID StartDownLoadTask();		
	//切换任务
	VOID SwitchDownLoadTask(LPCTSTR pszFileName);

	//时间消息
	VOID OnTimer(UINT nIDEvent);

	//DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////
#endif