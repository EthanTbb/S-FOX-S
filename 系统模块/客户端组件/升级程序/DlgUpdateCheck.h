#ifndef DLG_UPDATE_CHECK_HEAD_FILE
#define DLG_UPDATE_CHECK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "DownLoadHead.h"
#include "DownLoad.h"

//////////////////////////////////////////////////////////////////////////////////

//下载状态
#define DOWN_LOAD_IDLE					0								//空闲状态
#define DOWN_LOAD_LINK					1								//连接状态
#define DOWN_LOAD_READ					2								//读取状态
#define DOWN_LOAD_SETUP					3								//安装状态
#define DOWN_LOAD_ERROR					4								//错误状态
#define DOWN_LOAD_FINISH				5								//安装完成

//消息定义
#define WM_DOWNLOAD_FINISH			   (WM_USER+100)					//下载完毕

//////////////////////////////////////////////////////////////////////////////////

//下载回调
class CUpdateCheckSink : public CWnd, public IDownLoadSink
{
	//友员定义
	friend class CDlgUpdateCheck;

	//状态变量
protected:
	BYTE							m_cbDownLoadStatus;					//下载状态
	INT								m_nUpdateCount;						//更新数目

	//文件信息
protected:
	DWORD							m_dwDownLoadSize;					//下载大小
	DWORD							m_dwTotalFileSize;					//文件大小

	//内核信息
protected:	
	BYTE *							m_pFileData;						//文件数据
	CUDownLoad						m_DownLoad;							//下载组件
	CDlgUpdateCheck *				m_pDlgUpdateCheck;					//下载窗口

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

	//内部函数
private:
	//执行校验
	bool PerformCheckSum();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//更新检查
class CDlgUpdateCheck : public CDialog
{
	//状态变量
protected:
	UINT							m_uImageIndex;						//位图位置
	CString							m_strDescribe;						//描述信息
	CString                         m_pszDownLoadFile;					//下载路径
	BOOL                            m_bCheckFinish;                     //检查完成
	BOOL                            m_bNeedUpdate;                      //检查更新

	//控件变量
protected:
	CSkinLayered					m_SkinLayered;						//分层窗口

	//数据变量
protected:
	CUpdateCheckSink *              m_pUpdateCheckSink;                 //回调对象

	//函数定义
public:
	//构造函数
	CDlgUpdateCheck();
	//析构函数
	virtual ~CDlgUpdateCheck();

	//重载函数
protected:
	//控件绑定
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//消息解释
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//创建函数
	virtual BOOL OnInitDialog();

	//功能函数
public:
	//更新数目
	INT GetUpdateCount();
	//下载配置
	VOID DownLoadConfig(LPCTSTR pszConfigName);
	//设置消息
	VOID SetWindowDescribe(LPCTSTR pszDescribe);
	//完成事件
	VOID OnEventCheckFinish(BOOL bNeedUpdate);
	//出错事件
	VOID OnEventDownLoadError(int nResult);

	//消息映射
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);
	//绘画背景
	BOOL OnEraseBkgnd(CDC * pDC);
	//显示消息
	VOID OnShowWindow(BOOL bShow, UINT nStatus);
	//鼠标消息
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//位置改变
	VOID OnWindowPosChanged(WINDOWPOS * lpWndPos);

	//自定消息
protected:
	//状态消息
	LRESULT OnDownLoadConfigFinish(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif