#ifndef GLOBAL_UNITES_HEAD_FILE
#define GLOBAL_UNITES_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "DlgDownLoad.h"
class CDlgBagPlaza;
class CDlgSetup;
//////////////////////////////////////////////////////////////////////////////////

//ȫ�ֵ�Ԫ
class CGlobalUnits
{
	//�汾��Ϣ
protected:
	DWORD							m_dwPlazaVersion;					//�����汾
	DWORD							m_dwFrameVersion;					//��ܰ汾

	//׷�ٱ���
protected:
	WORD							m_wTrackServerID;					//׷�ٷ���
	DWORD							m_dwTrackUserID;					//׷�ٱ�ʶ

	//��������
protected:
	tagAuthRealParameter			m_AuthRealParameter;				//ʵ������

	//���ָ��
protected:
	//CDlgDownLoad *					m_pDlgDownLoad;						//�������
	CDlgBagPlaza *					m_pDlgBagPlaza;						//�������
	CDlgSetup	 *					m_pDlgSetup;						//ϵͳ����

	//��̬����
protected:
	static CGlobalUnits *			m_pGlobalUnits;						//����ָ��

	//��������
public:
	//���캯��
	CGlobalUnits();
	//��������
	virtual ~CGlobalUnits();

	//�汾��Ϣ
public:
	//�����汾
	virtual DWORD GetPlazaVersion() { return m_dwPlazaVersion; }
	//��ܰ汾
	virtual DWORD GetFrameVersion() { return m_dwFrameVersion; }

	//�����Ϣ
public:
	//��¼���
	bool WriteUserCookie();
	//ɾ�����
	bool DeleteUserCookie();

	//��������
public:
	//��������
	bool PlayGameSound(LPCTSTR pszSoundName);
	//��������
	bool PlayGameSound(HINSTANCE hInstance, LPCTSTR pszSoundName);

	//׷�ٺ���
public:
	//��ȡ�û�
	DWORD GetTrackUserID();
	//��ȡ����
	WORD GetTrackServerID();
	//���ñ�ʶ
	VOID SetTrackUserID(DWORD dwTrackUserID) { m_dwTrackUserID=dwTrackUserID;}
	//���ñ�ʶ
	VOID SetTrackServerID(WORD wTrackServerID) { m_wTrackServerID=wTrackServerID;}

	//���ܺ���
public:
	////������Ϸ
	//bool DownLoadClient(LPCTSTR pszClientName, WORD wKindID, WORD wServerID);
	//��ѯ���͵���
	bool QuerySendPresentMisson();

	//���ܺ���
public:
	//��ʾ��ʾ
	VOID PerformShowRealTip();
	//��ʾ����
	VOID PerformShowPersonerCenter(BYTE cbSelectedIndex = 0);
	//��ʾ����
	VOID PerformShowInsure();
	//��ʾ����
	VOID PerformShowTaskView();
	//��ʾ�ƹ�
	VOID PerformShowMySpread();	
	//��ʾ�һ�
	VOID PerformShowExchange();
	//��ʾǩ��
	VOID PerformShowCheckIn();	
	//��ʾ�ͱ�
	VOID PerformShowBaseEnsure();
	//��������
	VOID PerformShowLockMachine();
	//�����û�
	VOID PerformShowSearchUser();
	//��ֵ����
	VOID PerformShowRecharge();
	//��Ϸ�̳�
	bool PerformShowShop();
	//�ҵı���
	bool PerformShowBag();
	//�ҵı���
	bool PerformShowBagRecord();
	//����˵�����ʹ��
	VOID PerformShowPropertyUse(IClientUserItem* pClientUserItem);

	//�ҵĺ���
	VOID PerformShowFriend();
	//ϵͳ����
	VOID PerformShowSetup();
	//��ϵ�ͷ�
	VOID PerformShowContact();
	//������Ϣ
	VOID PerformShowFeedBack();
	//��������
	VOID PerformShowHelp();
	//��ʾ��Ϣ
	VOID PerformShowGameAd();
	//��������
public:
	VOID SetAuthRealParameter(const tagAuthRealParameter* pInfo);
	const tagAuthRealParameter* GetAuthRealParameter();

	//��̬����
public:
	//��ȡ����
	static CGlobalUnits * GetInstance() { return m_pGlobalUnits; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif