#ifndef DLG_DLGINSURE_HEAD_FILE
#define DLG_DLGINSURE_HEAD_FILE

#pragma once
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

//ǰ������
class CInsureItemWealth;
class CInsureItemAccess;
class CInsureItemPresent;
//////////////////////////////////////////////////////////////////////////////////
//��������
class CDlgInsure : public CFGuiDialog, public CMissionItem
{
public:
	//���캯��
	CDlgInsure();
	//��������
	virtual ~CDlgInsure();

	//MFC����
public:
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//FGUI����
public:
	//��ʼ�ؼ�
	virtual VOID InitControlUI();
	//��Ϣ����
    virtual void Notify(TNotifyUI &  msg);
	//Ƥ������
	virtual LPCTSTR GetSkinFile();

	//�����¼�
public:
	//�����¼�
	virtual bool OnEventMissionLink(INT nErrorCode);
	//�ر��¼�
	virtual bool OnEventMissionShut(BYTE cbShutReason);
	//��ȡ�¼�
	virtual bool OnEventMissionRead(TCP_Command Command, VOID * pData, WORD wDataSize);

	//�¼�����
public:
	//��ʼ����
	VOID OnMissionStart();
	//��ֹ����
	VOID OnMissionConclude();

public:
	//��ѯ�¼�
	virtual VOID PerformQueryRebate();
	//��ѯ�¼�
	virtual VOID PerformQueryInfo();
	//��ͨ�¼�
	virtual VOID PerformEnableInsure(LPCTSTR pszInsurePass,LPCTSTR pszLogonPass);
	//��ѯ�¼�
	virtual VOID QueryUserInsureInfo();
	//�����¼�
	virtual VOID PerformSaveScore(SCORE lSaveScore);	
	//ȡ���¼�
	virtual VOID PerformTakeScore(SCORE lTakeScore, LPCTSTR pszInsurePass);
	//ת���¼�
	virtual VOID PerformTransferScore(LPCTSTR pszAccounts, SCORE lTransferScore, LPCTSTR pszInsurePass,LPCTSTR pszTransRemark,BYTE cbByNickName);

	//�û�����
public:
	tagUserInsureInfo* GetUserInsureInfo();

	//��������
	VOID InitData();
	VOID LoadDataFromMemory();
	VOID LoadDataFromUI();
	VOID UpdateData();
	//����ٿ�
	VOID InitUI();
	VOID UpdateUI();
	//�����߼�
	VOID OnDefault();
	VOID DoApply();
	//���ش�ȡ
	void ShowPresent();

public:
	CContainerUI* GetWealthContainer();
	CContainerUI* GetAccessContainer();
	CContainerUI* GetPresentContainer();
	CContainerUI* GetRecordContainer();

	//�������
protected:
	CInsureItemWealth*		m_pItemWealth;						//�ҵ��ʲ�
	CInsureItemAccess*		m_pItemAccess;						//��Ϸ�Ҵ�ȡ
	CInsureItemPresent*		m_pItemPresent;						//��Ϸ������

	//�ؼ�����
protected:
	CContainerUI*			m_pContainerWealth;					//�ҵ��ʲ�
	CContainerUI*			m_pContainerAccess;					//��Ϸ�Ҵ�ȡ
	CContainerUI*			m_pContainerPresent;				//��Ϸ������
	CContainerUI*			m_pContainerRecord;					//������¼

	//�������
protected:
	CMissionManager			m_MissionManager;					//�������

	//��������
protected:
	//��ѯ����
public:
	bool					m_bInitInfo;						//��ʼ��ʶ
	tagUserInsureInfo 		m_UserInsureInfo;					//������Ϣ

	bool					m_bEnableMission;					//��ͨ����
	bool					m_bSaveMission;						//�������
	bool					m_bTakeMission;						//ȡ������
	bool					m_bQueryMission;					//��ѯ����
	bool					m_bTransferMission;					//ת������
	bool					m_bQueryRebateMission;				//��ѯ����

	SCORE					m_lScore;							//��Ϸ����Ŀ
	BYTE					m_cbByNickName;						//�����ǳ�
	DWORD					m_dwTargetUserID;					//Ŀ��ID
	TCHAR					m_szAccounts[LEN_NICKNAME];			//�û��ʺ�	
	TCHAR					m_szLogonPass[LEN_PASSWORD];		//��������
	TCHAR					m_szInsurePass[LEN_PASSWORD];		//��������
	TCHAR					m_szTransRemark[LEN_TRANS_REMARK];	//ת�ʱ�ע

	bool					m_bIsTransfer;

	BYTE					cbRebateEnabled;					//ʹ�ܱ�ʶ	
	SCORE					lRebateIngot;						//����Ԫ��
	SCORE					lRebateLoveLiness;					//��������ֵ
};


//////////////////////////////////////////////////////////////////////////////////
//�ҵ��ʲ���������ҳ�棩
class CInsureItemWealth
{
public:
	CInsureItemWealth(CDlgInsure* pDlgInsureMain);
	~CInsureItemWealth();
	
	//��������
	VOID InitData();
	//VOID LoadDataFromUI();
	//VOID UpdateData();
	
	//����ٿ�
	VOID InitUI();
	VOID UpdateUI();

	//�����߼�
	//VOID OnDefault();
	//VOID DoApply();

	//Ӧ�ú���
	void Notify(TNotifyUI &  msg);

private:
	//�ҵ��ʲ�����
	CDlgInsure* m_pDlgInsureMain;
	CContainerUI* m_pContainerUI;
};


//////////////////////////////////////////////////////////////////////////////////
//��Ϸ�Ҵ�ȡ��������ҳ�棩
class CInsureItemAccess
{
public:
	CInsureItemAccess(CDlgInsure* pDlgInsureMain);
	~CInsureItemAccess();

	//��������
	VOID InitData();
	VOID LoadDataFromUI();
	//VOID UpdateData();

	//����ٿ�
	VOID InitUI();
	VOID UpdateUI();

	//�����߼�
	//VOID OnDefault();
	//VOID DoApply();

	//��ʾ��д
	VOID ShowCapital();

	//Ӧ�ú���
	void Notify(TNotifyUI &  msg);
	void OnBnClickAccess(TNotifyUI& msg);
	VOID OnBnClickedSaveScore();
	VOID OnBnClickedTakeScore();

	void SetEditFocus();

private:
	//�ҵ��ʲ�����
	CDlgInsure*		m_pDlgInsureMain;
	CContainerUI*	m_pContainerUI;
	CEditUI*		m_pEditInOut;
	CEditUI*		m_pEditPassWord;
	CLabelUI*		m_LabelInOut;

	SCORE			m_InOutScore;		//����ķ���
};


//////////////////////////////////////////////////////////////////////////////////
//������ͣ�������ҳ�棩
class CInsureItemPresent
{
public:
	CInsureItemPresent(CDlgInsure* pDlgInsureMain);
	~CInsureItemPresent();

	//��������
	VOID InitData();
	//VOID LoadDataFromUI();
	//VOID UpdateData();

	//����ٿ�
	VOID InitUI();
	VOID UpdateUI();

	//�����߼�
	//VOID OnDefault();
	//VOID DoApply();

	//Ӧ�ú���
	void Notify(TNotifyUI &  msg);

	//ȷ������
	VOID OnBnClickedTransferScore();
	//����ID
	VOID OnBnCheckedTransferByUserID();
	//�����ǳ�
	VOID OnBnCheckedTransferByNickName();

private:
	//�ҵ��ʲ�����
	CDlgInsure* m_pDlgInsureMain;
	CContainerUI* m_pContainerUI;

	CRadioButtonUI* m_pRadioByNickName;		//�ǳƵ�ѡ
	CRadioButtonUI* m_pRadioByUserID;		//ID��ѡ
	CEditUI*		m_pEditAccount;			//�ǳ�
	CEditUI*		m_pEditAmount;			//����
	CEditUI*		m_pEditInsurePass;		//����
	CEditUI*		m_pEditRemarks;			//��ע
	CLabelUI*		m_pLabelTarget;			//����ID or �ǳ�
	CLabelUI*		m_pLabelChineseNumeral;	//��д
};



//��������
class CDlgInsureServerNew : public CDlgInsure
{
	//��������
public:
	//���캯��
	CDlgInsureServerNew();
	//��������
	virtual ~CDlgInsureServerNew();




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

	VOID QueryUserInsureInfo();

	//��������
protected:
	ITCPSocket *					m_pITCPSocket;						//����ӿ�
	IClientUserItem *				m_pIMySelfUserItem;					//�Լ�ָ��



};


#endif