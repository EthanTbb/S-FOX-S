#ifndef PLAZA_USER_INFO_HEAD_FILE
#define PLAZA_USER_INFO_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////////////
#define IDI_CHECK_USER_BUFF			11				//������Buffʱ��
#define IDI_CHECK_SEND_PRESENT		12				//��������û�б����͵���

//������ͼ
class CPlazaUserInfo : public CMissionItem
{
	//��������
public:
	//���캯��
	CPlazaUserInfo(CPaintManagerUI*	pPaintManager);
	//��������
	virtual ~CPlazaUserInfo();
	//���״̬
	void ClearBuff();
	//���غ���
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//FGUI����
public:
	//��Ϣ����
	bool Notify(TNotifyUI &  msg);
	//��ʼ�ؼ�
	bool InitControlUI();
	//�ж���Ϣ
	bool IsNotify(TNotifyUI &  msg);
	//��ȡ�ؼ�
	CControlUI * GetControlByName(LPCTSTR pszControlName);
	
	//�¼���Ϣ
public:
	//�¼���Ϣ
	bool OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);
	VOID OnTimer(UINT_PTR nIDEvent);
	//�û�����
public:
	//��������
	VOID AvtiveMission();

	//ˢ�½���
	VOID UpdateUI();

	//ˢ�µȼ�
	VOID UpdateLevelUI();

	//����Buff����
	VOID UpdateBuffData(const tagPropertyBuff& Buff);

	//��ѯ�ȼ�
	VOID QueryLevel();

	//��ѯ��ҵ���״̬
	VOID QueryPropBuff();

	//��ѯʵ��
	VOID QueryIndividual();

	//��ѯ��������
	VOID QuerySendPresent();

	//�����ѯ��������
	VOID AvtiveQuerySendPresentMission();

	//ˢ�µȼ�
	VOID UpdateLevel();

	//ˢ��BUFF
	VOID UpdateBuffUI();
	bool VisibleBuff(const tagPropertyBuff& buff, CControlUI* pControl, LPCTSTR lpImage);
	
	//ˢ��ͷ��
	VOID UpdateUserFace(const WORD wFaceID);
	VOID UpdateUserFace(DWORD dwCustomFace[48*48]);

	//��ȡͷ������
	RECT GetUserFacePos();

	//���ý���
	void SetRealTipAward();
	
	//��ʾʵ��
	void ShowRealTip();

	//ȡ��ʵ��
	void CancleRealTip();

	//����ʵ��
	void HideRealTip();

	//�洢����
protected:
	tagGrowLevelParameter			m_GrowLevelParameter;				//�ɳ�����
	CMissionManager					m_MissionManager;					//�������
	CPaintManagerUI*				m_pPaintManager;
	CImageUI*						m_pPlazaHallAvtatr;					//ͷ��
	CControlUI*						m_pUserDoubleBuff;					//˫��
	CControlUI*						m_pUserAmuletBuff;					//����
	CControlUI*						m_pUserGuardKickBuff;				//����
	WORD							m_wFaceID;							
	bool							m_bQueryLevel;						//��ѯ�ȼ�
	bool							m_bQueryPropBuff;					//����BUff
	bool                            m_bQueryIndividual;                 //��ѯ����
	bool							m_bQuerySendPresent;				//��ѯ����
	vector<tagPropertyBuff>			m_vecPropertyBuff;					//����BUff
	CStdString						m_sGuardKickBuffImage;
	CStdString						m_sAmuletBuffImage;
	CStdString						m_sDoubleBuffImage;
	int								m_nRealNum;							//ʵ������
};

//////////////////////////////////////////////////////////////////////////////////

#endif