#ifndef DLG_DOWN_LOAD_HEAD_FILE
#define DLG_DOWN_LOAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//下载状态
#define DOWN_LOAD_IDLE					0								//空闲状态
#define DOWN_LOAD_LINK					1								//连接状态
#define DOWN_LOAD_READ					2								//读取状态
#define DOWN_LOAD_SETUP					3								//安装状态
#define DOWN_LOAD_ERROR					4								//错误状态
#define DOWN_LOAD_FINISH				5								//安装完成

//消息定义
#define WM_ITEM_STATUS_UPDATE			(WM_USER+100)					//状态更新
#define WM_ITEM_QUERY_UPDATE    		(WM_USER+101)					//增加子项
#define WM_ITEM_ADD_GAME    			(WM_USER+102)					//增加游戏
#define WM_ITEM_ENTER_SERVER   			(WM_USER+103)					//进入房间
//////////////////////////////////////////////////////////////////////////////////

class IDownLoadStateSink
{
public:
	virtual bool SendMessage(UINT message, WPARAM wParam , LPARAM lParam ) = 0;
};
//下载窗口
class CPlazaHallGameDownLoadSink : public CWnd, public IDownLoadSink
{

	//状态变量
public:
	BYTE							m_cbDownLoadStatus;					//下载状态

	//下载速度
protected:
	DWORD							m_dwDownSpeed;						//下载速度	
	DWORD							m_dwLastCalcTime;					//上次时间
	LONGLONG						m_lLastCalcSize;					//上次大小

	//文件信息
public:
	LONGLONG						m_lDownLoadSize;					//下载大小
	LONGLONG						m_lTotalFileSize;					//文件大小

	//任务属性
public:
	WORD							m_wKindID;							//游戏标识
	WORD							m_wServerID;						//房间标识
	TCHAR							m_szClientName[LEN_KIND];			//任务名字
	IDownLoadStateSink *			m_pDlgDownLoad;						//下载接口	
	//内核信息
protected:
	CFile							m_DownFile;							//下载文件
	CDownLoad						m_DownLoad;							//下载组件

	PROCESS_INFORMATION				m_ProcessInfoMation;				//进程信息

	//函数定义
public:
	//构造函数
	CPlazaHallGameDownLoadSink();
	//析构函数
	virtual ~CPlazaHallGameDownLoadSink();

	//基础接口
public:
	//释放对象
	virtual VOID Release() { return; }
	//接口查询
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//功能函数
public:
	//关闭下载
	bool CloseDownLoad();
	//执行下载
	bool PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

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
	//执行安装
	bool PerformInstall();
	//创建文件
	bool CreateDownLoadFile(LPCTSTR pszLocation);

	//消息函数
protected:
	//时间消息
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};



//////////////////////////////////////////////////////////////////////////////////


#endif