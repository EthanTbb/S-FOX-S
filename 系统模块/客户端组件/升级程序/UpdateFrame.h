#ifndef DLG_DOWN_LOAD_HEAD_FILE
#define DLG_DOWN_LOAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "DownLoad.h"

////////////////////////////////////////////////////////////////////////////////////////////////

//下载状态
#define DOWN_LOAD_IDLE					0								//空闲状态
#define DOWN_LOAD_LINK					1								//连接状态
#define DOWN_LOAD_READ					2								//读取状态
#define DOWN_LOAD_SETUP					3								//安装状态
#define DOWN_LOAD_ERROR					4								//错误状态

//消息定义
#define WM_ITEM_DOWNLOAD_Finish			(WM_USER+100)					//状态更新
#define WM_ITEM_DOWNLOAD_UPDATE			(WM_USER+101)					//状态更新

////////////////////////////////////////////////////////////////////////////////////////////////
//下载信息
struct tagDownloadItem
{
	TCHAR								szDownloadFile[MAX_PATH];		//下载文件
	TCHAR								szFileCheckSum[LEN_MD5];		//文件校验和
};

//下载结果
struct tagDownLoadResult
{
	bool								bDownLoadSuccess;				//下载成功
	TCHAR								szSourceFilePath[MAX_PATH];		//原始路径
	TCHAR								szTargetFilePath[MAX_PATH];		//目标路径
};

///////////////////////////////////////////////////////////////////////////////////////////////
//类型定义
typedef CWHArray<tagDownloadItem *>		CDownloadItemArray;				//下载子项

///////////////////////////////////////////////////////////////////////////////////////////////


//下载回调
class CUDownLoadSink :public CWnd,public IDownLoadSink
{	
	friend class  CDownLoadManager;
	friend class  CGameUpdateFrame;

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
	CUDownLoad					m_DownLoad;							//下载组件
	CDownLoadManager *				m_pDownLoadManager;  				//管理接口

	//函数定义
public:
	//构造函数
	CUDownLoadSink();
	//析构函数
	virtual ~CUDownLoadSink();

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

	//内部函数
protected:
	//创建文件
	bool CreateDownFile(LPCTSTR pszLocation);
	//加载文件
	bool LoadDownFile(LPCTSTR pszLocation);
	//关闭下载
	bool CloseDownLoad();
	//执行下载
	bool PerformDownLoad(LPCTSTR pszNetAddress,LPCTSTR pszFileName,bool bBreakpointDownLoad=false);
	//执行安装
	VOID PerformInstall();
	//重置变量
	VOID ResetDownLoadSink();

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////////////////

//下载管理
class CDownLoadManager : public CWnd
{
	friend class  CGameUpdateFrame;

	//变量定义
protected:
	DWORD							m_dwTickCount;						//时间间隔
	DWORD                           m_dwTotalTaskCount;                 //任务总数
	DWORD                           m_dwTaskFinished;                   //完成任务
	BYTE							m_cbCurrRetryCount;					//重试次数

	//任务信息
protected:
	TCHAR                           m_szDownLoadAddress[MAX_PATH];      //下载地址 
	CDownloadItemArray              m_DownLoadFileArray;                //下载列表
	CStringArray					m_SourcePathArray;					//路径列表
	CStringArray					m_TargetPathArray;					//路径列表	

	//核心变量
protected:
	CUDownLoadSink *                 m_pDownLoadSink;                    //回调接口
	CGameUpdateFrame *              m_pGameUpdateFrame;                 //视图接口  

	//函数定义
public:
	//构造函数
	CDownLoadManager();
	//析构函数
	virtual ~CDownLoadManager();

	//内部函数
protected:
	//读取信息
	bool ReadDownLoadInfo();
	//执行下载
	bool StartDownLoadTask();
	//移动文件
	VOID PerformMoveFiles();

	//自定消息
protected:
	//状态消息
	LRESULT OnItemDownLoadFinishMessage(WPARAM wParam, LPARAM lParam);
	//状态消息
	LRESULT OnItemDownLoadUpdateMessage(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

////////////////////////////////////////////////////////////////////////////////////////////////

//升级框架
class CGameUpdateFrame : public CFrameWnd
{
	//字符变量
protected:
	TCHAR                           m_pszFileName[128];                  //文件名称 	

	//控件变量
protected:	
	CSkinButton						m_btClose;							//关闭按钮
	CSkinButton						m_btSelfUpdate;						//手动更新
	CSkinLayered					m_SkinLayered;						//分层窗口	

	//指针变量
protected:
	CDownLoadManager *				m_pDownLoadManager;	     			//下载管理

	//函数定义
public:
	//构造函数
	CGameUpdateFrame();
	//析构函数
	virtual ~CGameUpdateFrame();

	//事件函数
public:
	//下载结束
	VOID OnEventDownLoadEnd();
	//切换任务
	VOID OnEventSwitchDownLoadTask(LPCTSTR pszFileName);

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//重载函数
protected:
	//绘画消息
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//功能函数
public:
	//启动下载
	VOID StartDownLoadTask();

	//辅助函数
private:
	//调整资源
	VOID RectifyWindow(INT nWidth, INT nHeight);

	//按钮消息
protected:
	//关闭按钮
	VOID OnBnClickedClose();

	//手动按钮
	VOID OnBnClickedSelfUpdate(); 

	//消息函数
protected:
	//关闭消息
	VOID OnClose();
	//销毁非客户
	VOID OnNcDestroy();
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//位置消息
	VOID OnSize(UINT nType, INT cx, INT cy);
	//创建消息
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);	

	DECLARE_MESSAGE_MAP()	
};

////////////////////////////////////////////////////////////////////////////////////////////////

#endif