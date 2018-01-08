#ifndef DLG_DOWN_LOAD_HEAD_FILE
#define DLG_DOWN_LOAD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//����״̬
#define DOWN_LOAD_IDLE					0								//����״̬
#define DOWN_LOAD_LINK					1								//����״̬
#define DOWN_LOAD_READ					2								//��ȡ״̬
#define DOWN_LOAD_SETUP					3								//��װ״̬
#define DOWN_LOAD_ERROR					4								//����״̬
#define DOWN_LOAD_FINISH				5								//��װ���

//��Ϣ����
#define WM_ITEM_STATUS_UPDATE			(WM_USER+100)					//״̬����
#define WM_ITEM_QUERY_UPDATE    		(WM_USER+101)					//��������
#define WM_ITEM_ADD_GAME    			(WM_USER+102)					//������Ϸ
#define WM_ITEM_ENTER_SERVER   			(WM_USER+103)					//���뷿��
//////////////////////////////////////////////////////////////////////////////////

class IDownLoadStateSink
{
public:
	virtual bool SendMessage(UINT message, WPARAM wParam , LPARAM lParam ) = 0;
};
//���ش���
class CPlazaHallGameDownLoadSink : public CWnd, public IDownLoadSink
{

	//״̬����
public:
	BYTE							m_cbDownLoadStatus;					//����״̬

	//�����ٶ�
protected:
	DWORD							m_dwDownSpeed;						//�����ٶ�	
	DWORD							m_dwLastCalcTime;					//�ϴ�ʱ��
	LONGLONG						m_lLastCalcSize;					//�ϴδ�С

	//�ļ���Ϣ
public:
	LONGLONG						m_lDownLoadSize;					//���ش�С
	LONGLONG						m_lTotalFileSize;					//�ļ���С

	//��������
public:
	WORD							m_wKindID;							//��Ϸ��ʶ
	WORD							m_wServerID;						//�����ʶ
	TCHAR							m_szClientName[LEN_KIND];			//��������
	IDownLoadStateSink *			m_pDlgDownLoad;						//���ؽӿ�	
	//�ں���Ϣ
protected:
	CFile							m_DownFile;							//�����ļ�
	CDownLoad						m_DownLoad;							//�������

	PROCESS_INFORMATION				m_ProcessInfoMation;				//������Ϣ

	//��������
public:
	//���캯��
	CPlazaHallGameDownLoadSink();
	//��������
	virtual ~CPlazaHallGameDownLoadSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܺ���
public:
	//�ر�����
	bool CloseDownLoad();
	//ִ������
	bool PerformDownLoad(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);

	//״̬�ӿ�
public:
	//�����쳣
	virtual bool OnDownLoadError(enDownLoadError DownLoadError);
	//����״̬
	virtual bool OnDownLoadStatus(enDownLoadStatus DownLoadStatus);

	//���нӿ�
public:
	//��������
	virtual bool OnDataStream(const VOID * pcbMailData, WORD wStreamSize);
	//������Ϣ
	virtual bool OnDataInformation(bool bPartialConent,DWORD dwTotalFileSize, LPCTSTR pszEntityTag, LPCTSTR pszLocation);

	//�ڲ�����
private:
	//ִ�а�װ
	bool PerformInstall();
	//�����ļ�
	bool CreateDownLoadFile(LPCTSTR pszLocation);

	//��Ϣ����
protected:
	//ʱ����Ϣ
	VOID OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
};



//////////////////////////////////////////////////////////////////////////////////


#endif