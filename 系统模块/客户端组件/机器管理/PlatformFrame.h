#ifndef PLATFORM_FRAME_HEAD_FILE
#define PLATFORM_FRAME_HEAD_FILE

#pragma once

//ϵͳ�ļ�
#include "Stdafx.h"
#include "Resource.h"

//�����ļ�
#include "MissionList.h"
#include "MissionLogon.h"
#include "MissionAndroid.h"

//
#include "ServerListData.h"

//////////////////////////////////////////////////////////////////////////////////

//ƽ̨���
class CPlatformFrame : public CSkinDialog,public IServerListDataSink,IAndroidOperateCenter
{
	//��������
protected:
	bool							m_bLogonSuccess;					//��¼�ɹ�
	int								m_nSelectItem;						//ѡ������

	//��Դ����
protected:
	CBrush							m_brBackGround;						//������ˢ

	//��Ϸ�б�
protected:
	CServerListData					m_ServerListData;					//��Ϸ����

	//�ؼ�����
protected:
	CSkinComboBox					m_cbGameList;						//��Ϸ�б�
	CSkinComboBox					m_cbServerList;						//�����б�
	CSkinComboBox					m_cbEnterHour;						//����ʱ��
	CSkinComboBox					m_cbEnterMinute;					//����ʱ��
	CSkinComboBox					m_cbEnterSecond;					//����ʱ��
	CSkinComboBox					m_cbLeaveHour;						//�뿪ʱ��
	CSkinComboBox					m_cbLeaveMinute;					//�뿪ʱ��
	CSkinComboBox					m_cbLeaveSecond;					//�뿪ʱ��

	//�ؼ�����
protected:
	CSkinEdit						m_edtAndroidCount;					//������Ŀ
	CSkinEdit						m_edtTableScoreMin;					//Я������
	CSkinEdit						m_edtTableScoreMax;					//Я������
	CSkinEdit						m_edtEnterIntervalMin;				//������
	CSkinEdit						m_edtEnterIntervalMax;				//������
	CSkinEdit						m_edtLeaveIntervalMin;				//�뿪���
	CSkinEdit						m_edtLeaveIntervalMax;				//�뿪���
	CSkinEdit						m_edtSwitchTableMin;				//��������
	CSkinEdit						m_edtSwitchTableMax;				//��������
	CSkinEdit						m_edtCountMember0;					//��ͨ��Ա
	CSkinEdit						m_edtCountMember1;					//һ����Ա
	CSkinEdit						m_edtCountMember2;					//������Ա
	CSkinEdit						m_edtCountMember3;					//������Ա
	CSkinEdit						m_edtCountMember4;					//�ļ���Ա
	CSkinEdit						m_edtCountMember5;					//�弶��Ա
	//��ť�ؼ�
protected:
	CSkinButton						m_btQuery;							//��ѯ��ť
	CSkinButton						m_btAddItem;						//�������
	CSkinButton						m_btModifyItem;						//�޸�����
	CSkinButton						m_btDeleteItem;						//ɾ������

	//�ؼ�����
protected:
	CSkinListCtrl					m_ParemeterList;					//�����б�
	CSystemTrayIcon					m_SystemTrayIcon;					//����ͼ��

	//�������
protected:
	CMissionList					m_MissionList;						//�б�����
	CMissionLogon					m_MissionLogon;						//��¼����
	CMissionAndroid					m_MissionAndroid;					//��������
	CMissionManager					m_MissionManager;					//�������

	//��̬����
protected:
	static CPlatformFrame *			m_pPlatformFrame;					//���ָ��

	//��������
public:
	//���캯��
	CPlatformFrame();
	//��������
	virtual ~CPlatformFrame();

	//���غ���
protected:
	//��ʼ����
	virtual BOOL OnInitDialog();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//��������
	virtual VOID DoDataExchange(CDataExchange* pDX);

	//״̬֪ͨ
public:
	//���֪ͨ
	virtual VOID OnGameItemFinish();
	//���֪ͨ
	virtual VOID OnGameKindFinish(WORD wKindID);
	//����֪ͨ
	virtual VOID OnGameItemUpdateFinish();

	//����֪ͨ
public:
	//����֪ͨ
	virtual VOID OnGameItemInsert(CGameListItem * pGameListItem);
	//����֪ͨ
	virtual VOID OnGameItemUpdate(CGameListItem * pGameListItem);
	//ɾ��֪ͨ
	virtual VOID OnGameItemDelete(CGameListItem * pGameListItem);

	//�����ӿ�
public:
	//�ؼ�ʹ��
	virtual VOID OnEventEnableControls(BOOL bEnabled);
	//��������
	virtual VOID OnEventAndroidParenter(WORD wSubCommdID,WORD wParameterCount,tagAndroidParameter * pAndroidParameter);

	//��������
private:
	//��ʼ���ؼ�
	VOID InitControls();
	//����޸�
	VOID AddModifyParameter(bool bModify);
	//ѡ������
	VOID SelectItem(int nIndex);
	//��������
	VOID SetItemToParameterList(int nItemIndex,tagAndroidParameter * pAndroidParameter);
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�ر���Ϣ
	VOID OnClose();	
	//���ٴ���
	VOID OnDestroy();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//λ����Ϣ
	VOID OnGetMinMaxInfo(MINMAXINFO * lpMMI);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�ı���Ϣ
	VOID OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//ѡ��ı�
	VOID OnCbnComboGameSelchange();
	//��������
	VOID OnHdnListParameterItemclick(NMHDR *pNMHDR, LRESULT *pResult);
	//ɾ������
	VOID OnLvnListParameterDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	//���̰���
	VOID OnLvnKeydownListParameter(NMHDR *pNMHDR, LRESULT *pResult);

	//�Զ���Ϣ
protected:
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	//�¼�����
protected:
	//�������
	VOID OnBnClickedBtAdd();
	//�༭����
	VOID OnBnClickedBtModify();
	//ɾ������
	VOID OnBnClickedBtDelete();
	//��ѯ����
	VOID OnBnClickedBtQuery();

	//���ܺ���
public:
	//��ȡʵ��
	static CPlatformFrame * GetInstance() { return m_pPlatformFrame; }

	DECLARE_MESSAGE_MAP()			
};

//////////////////////////////////////////////////////////////////////////////////

#endif