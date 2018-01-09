#ifndef DLG_INSURE_COUNTER_HEAD_FILE
#define DLG_INSURE_COUNTER_HEAD_FILE

#pragma once

#include "MissionManager.h"
#include "PasswordControl.h"
#include "WebPublicize.h"
#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��Ϣ����
#define WM_INSUREPLAZA_EVENT      (WM_USER+1680)             //�����¼�

//�����
#define INSURE_COMMAND_UPDATE	  1							//��������
#define INSURE_COMMAND_EXCHANGE	  2							//�һ�����			

//���½ṹ
struct tagInsurePlazaEvent
{
	bool   bHeapData;
	WORD   wCommandID;			
	SCORE  lUserScore;			
	SCORE  lUserInsure;		
};

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CDlgInsureItem;
class CDlgInsureMain;
class CDlgInsureSave;
class CDlgInsureOperater;

//////////////////////////////////////////////////////////////////////////////////

//������ʽ
#define INSURE_SAVE					0									//��ȡ��ʽ
#define INSURE_TRANSFER				1									//ת�ʷ�ʽ
#define INSURE_RECORD				2									//������¼

//��������
#define INSURE_MASK_SAVE			0x01								//��ȡ����
#define INSURE_MASK_TRANSFER		0x02								//ת������
#define INSURE_MASK_RECORD			0x04								//��¼����

//////////////////////////////////////////////////////////////////////////////////

//������Ϣ
struct tagUserInsureInfo
{
	BYTE							cbEnjoinTransfer;					//ת�ʿ���
	WORD							wRevenueTake;						//˰�ձ���
	WORD							wRevenueTransfer;					//˰�ձ���
	WORD							wRevenueTransferMember;				//˰�ձ���
	WORD							wServerID;							//�����ʶ
	SCORE							lUserScore;							//�û���Ϸ��
	SCORE							lUserInsure;						//������Ϸ��
	SCORE							lTransferPrerequisite;				//ת������
};

//////////////////////////////////////////////////////////////////////////////////

//���нӿ�
interface IInsureCounterAction
{
	//��ѯ�¼�
	virtual VOID PerformQueryInfo()=NULL;	
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore)=NULL;
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass)=NULL;
	//��ͨ�¼�
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass)=NULL;
	//ת���¼�
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//ҳ������
class CDlgInsureItem
{
	//���ܺ���
public:
	//��ȡ��Ϸ��
	static SCORE GetUserControlScore(LPCTSTR pszScore);

	//���ܺ���
public:
	//�滭����
	static VOID DrawNumberString(CDC * pDC, SCORE lScore, INT nXPos, INT nYPos);
	//ת���ַ�
	static VOID SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize);
	//ת���ַ�
	static VOID SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize);
};

//////////////////////////////////////////////////////////////////////////////////

//��ȡ��Ϸ��
class CDlgInsureSave : public CFGuiWnd
{
	//��Ԫ����
	friend class CDlgInsureOperater;

	//��������
protected:
	bool							m_bSwitchIng;						//ת����־

	//����ָ��
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//����ָ��
	CDlgInsureOperater *			m_pDlgInsureOperater;				//����ָ��
	tagUserInsureInfo *				m_pUserInsureInfo;					//�û���Ϣ

	//����ؼ�
public:
    CEditUI *						m_pEditAmount;						//�����༭	
	CEditUI *						m_pEditInsurePass;					//����༭
	CTextUI *						m_pTextForget;						//��������
	CButtonUI *						m_pButtonSave;						//��ť
	CButtonUI *						m_pButtonTake;						//ȡ�ť
	CButtonUI *						m_pButtonExchange;					//�һ���ť
	//CButtonUI *						m_pButtonRefresh;					//ˢ�°�ť

	//��������
public:
	//���캯��
	CDlgInsureSave();
	//��������
	virtual ~CDlgInsureSave();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	

	//�ؼ�����
protected:
	//�������
	VOID OnEnChangeScore();
	//��ť
	VOID OnBnClickedSaveScore();
	//ȡ�ť
	VOID OnBnClickedTakeScore();
	//��������
	VOID OnBnClickedForgetInsure();
	//�һ���Ϸ��
	VOID OnBnClickedExChangeScore();	
};

//////////////////////////////////////////////////////////////////////////////////

//ת����Ϸ��
class CDlgInsureTransfer : public CFGuiWnd
{
	//��Ԫ����
	friend class CDlgInsureOperater;

	//��������
protected:
	bool							m_bSwitchIng;						//ת����־

	//����ָ��
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//����ָ��
	CDlgInsureOperater *			m_pDlgInsureOperater;				//����ָ��
	tagUserInsureInfo *				m_pUserInsureInfo;					//�û���Ϣ

	//����ؼ�
public:
	CEditUI *						m_pEditAccount;						//�����ʺ�
    CEditUI *						m_pEditAmount;						//���ͽ��	
	CEditUI *						m_pEditInsurePass;					//��������
	CEditUI *						m_pEditRemarks;						//��ע˵��

	//����ؼ�
public:
	CLabelUI *						m_pLabelTarget;						//Ŀ���ǩ
	CButtonUI *						m_pButtonConfirm;					//ȷ�ϰ�ť
	CRadioButtonUI *				m_pRadioByUserID;					//��ѡ��ť
	CRadioButtonUI *				m_pRadioByNickName;					//��ѡ��ť

	//��������
public:
	//���캯��
	CDlgInsureTransfer();
	//��������
	virtual ~CDlgInsureTransfer();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	

	//�ؼ�����
protected:
	//�������
	VOID OnEnChangeScore();
	//ת�ʰ�ť
	VOID OnBnClickedTransferScore();
	//����ID
	VOID OnBnCheckedTransferByUserID();
	//�����ǳ�
	VOID OnBnCheckedTransferByNickName();
};

//////////////////////////////////////////////////////////////////////////////////
//���м�¼
class CDlgInsureRecord : public CFGuiWnd
{
	//��Ԫ����
	friend class CDlgInsureOperater;

	//����ָ��
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//����ָ��
	CDlgInsureOperater *			m_pDlgInsureOperater;				//����ָ��

	//�����ؼ�
public:
	CWebPublicize			        m_PublicizeRecord;					//������¼

	//��������
public:
	//���캯��
	CDlgInsureRecord();
	//��������
	virtual ~CDlgInsureRecord();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//���ܺ���
public:
	//ˢ�¼�¼
	VOID RefreshInsureRecord();

	//��Ϣ����
public:
	//������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//������С
	VOID OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()		
};

//////////////////////////////////////////////////////////////////////////////////

//���в���
class SHARE_CONTROL_CLASS CDlgInsureOperater : public CFGuiWnd
{
	//��ѯ����
public:
	bool							m_bInitInfo;						//��ʼ��ʶ
	tagUserInsureInfo *				m_pUserInsureInfo;					//������Ϣ

	//ѡ��ؼ�
public:
	BYTE							m_cbInsureMode;						//����ģʽ
	BYTE							m_cbInsureMask;						//��������
	CTabButtonUI *					m_TabButton[3];						//ѡ��ť

	//�����ؼ�
public:
	CLabelUI *						m_pLabelRound;						//���Ʊ�ǩ
	CDlgInsureMain *				m_pDlgInsureMain;					//������

	//�����
public:	
	CDlgInsureSave					m_DlgInsureSave;					//��ȡ����
	CDlgInsureRecord				m_DlgInsureRecord;					//���м�¼
	CDlgInsureTransfer				m_DlgInsureTransfer;				//ת������	

	//��������
public:
	//���캯��
	CDlgInsureOperater();
	//��������
	virtual ~CDlgInsureOperater();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);
	//��ʼ�ؼ�
	virtual VOID InitControlUI();

	//���ƺ���
public:
	//��������
	VOID RectifyTabItem();
	//��������
	VOID InsertTabItem(BYTE cbItemMask);
	//ɾ������
	VOID DeleteTabItem(BYTE cbItemMask);

	//���ܺ���
public:
	//����ģʽ
	VOID SwitchInsureModeView(BYTE cbInsureMode);

	//��������
public:
	//��ȡ��ɫ
	COLORREF GetItemBackColor() { return RGB(189,173,139); }
	//���ö���
	VOID SetDlgInsureMain(CDlgInsureMain * pDlgInsureMain);
	//������Ϣ
	VOID SetUserInsureInfo(tagUserInsureInfo * pUserInsureInfo);

	//��Ϣ����
protected:
	//��ѯ��ť
	VOID OnBnClickedQueryInfo();

	//��Ϣ����
public:
	//������Ϣ
	int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()	
};

//////////////////////////////////////////////////////////////////////////////////
//

//��������
class CDlgInsureSetup : public CFGuiWnd
{
	//����ָ��
protected:
	CDlgInsureMain *				m_pDlgInsureMain;					//����ָ��

	//�ؼ�ָ��
public:
	CLabelUI *						m_pLabelRound;						//���Ʊ�ǩ
	CEditUI *						m_pEditLogonPass;					//��¼����
	CEditUI *						m_pEditInsurePass;					//��������
    CEditUI *						m_pEditInsurePass1;					//ȷ������
	CButtonUI *						m_pButtonEnable;					//��ͨ��ť	

	//��������
public:
	//���캯��
	CDlgInsureSetup();
	//��������
	virtual ~CDlgInsureSetup();

	//�滭�ӿ�
public:
	//��ʼ�滭
	virtual VOID OnBeginPaintWindow(HDC hDC);

	//���غ���
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
	virtual VOID Notify(TNotifyUI &  msg);	

	//���ܺ���
public:
	//���ö���
	VOID SetDlgInsureMain(CDlgInsureMain * pDlgInsureMain) { m_pDlgInsureMain=pDlgInsureMain; }

	//�ؼ�����
protected:
	//��ͨ����
	VOID OnBnClickedEnableInsure();
};

//////////////////////////////////////////////////////////////////////////////////

//������
class SHARE_CONTROL_CLASS CDlgInsureMain : public CSkinDialog, public IInsureCounterAction
{
	//��������
protected:
	bool							m_bEnableMission;					//��ͨ����
	bool							m_bSaveMission;						//�������
	bool							m_bTakeMission;						//ȡ������
	bool							m_bQueryMission;					//��ѯ����
	bool							m_bTransferMission;					//ת������

	//��ѯ����
public:
	bool							m_bInitInfo;						//��ʼ��ʶ
	tagUserInsureInfo 				m_UserInsureInfo;					//������Ϣ

	//�ؼ�����
protected:
	CDlgInsureSetup					m_DlgInsureSetup;					//��������
	CDlgInsureOperater				m_DlgInsureOperater;				//���в���

	//��������
public:
	//���캯��
	CDlgInsureMain();
	//��������
	virtual ~CDlgInsureMain();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//ȷ����ť
	virtual VOID OnOK();

	//���ں���
protected:
	//�滭����
	virtual VOID OnDrawSystemTitle(CDC * pDC, INT nWdith, INT nHeight);
	//�滭��Ϣ
	virtual VOID OnDrawClientAreaBegin(CDC * pDC, INT nWidth, INT nHeight);

	//�¼�����
public:
	//��ʼ����
	VOID OnMissionStart();
	//��ֹ����
	VOID OnMissionConclude();

	//��������
protected:
	//��������
	VOID RectifyInsureStep();

	//���ܺ���
public:
	//��ѯ����
	VOID QueryUserInsureInfo();	
};

//////////////////////////////////////////////////////////////////////////////////

//�㳡����
class SHARE_CONTROL_CLASS CDlgInsurePlaza : public CDlgInsureMain, public CMissionItem
{
	//��������
protected:
	SCORE							m_lScore;							//��Ϸ����Ŀ
	BYTE							m_cbByNickName;						//�����ǳ�
	TCHAR							m_szAccounts[LEN_NICKNAME];			//�û��ʺ�	
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//��������
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//��������
	TCHAR							m_szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע

	//�������
protected:
	CMissionManager					m_MissionManager;					//�������

	//��������
public:
	//���캯��
	CDlgInsurePlaza();
	//��������
	virtual ~CDlgInsurePlaza();

	//ִ�к���
protected:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore);	
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);
	//��ͨ�¼�
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass);
	//ת���¼�
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName);

	//�����¼�
private:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class SHARE_CONTROL_CLASS CDlgInsureServer : public CDlgInsureMain
{
	//��������
protected:
	SCORE							m_lScore;							//��Ϸ����Ŀ
	BYTE							m_cbByNickName;						//�����ǳ�
	TCHAR							m_szAccounts[LEN_NICKNAME];			//�û��ʺ�
	TCHAR							m_szLogonPass[LEN_PASSWORD];		//��¼����
	TCHAR							m_szInsurePass[LEN_PASSWORD];		//��������
	TCHAR							m_szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע

	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��

	//��������
public:
	//���캯��
	CDlgInsureServer();
	//��������
	virtual ~CDlgInsureServer();

	//ִ�к���
protected:
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();	
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore);	
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);
	//��ͨ�¼�
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass);
	//ת���¼�
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName);

	//���ܺ���
public:
	//��������
	VOID Initialization(ITCPSocket * pITCPSocket, IClientUserItem * pIMySelfUserItem);
	//������Ϣ
	bool OnServerInsureMessage(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize);
};

//////////////////////////////////////////////////////////////////////////////////

#endif