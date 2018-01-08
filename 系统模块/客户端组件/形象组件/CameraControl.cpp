#include "Stdafx.h"
#include "Resource.h"
#include "CameraControl.h"

//////////////////////////////////////////////////////////////////////////////////

#define IDC_CAMERA_WND				100									//���ڱ�ʶ


//////////////////////////////////////////////////////////////////////////////////
const TCHAR* const szButtonCloseContorlName = TEXT("ButtonClose");
const TCHAR* const szButtonSaveContorlName = TEXT("ButtonSave");
const TCHAR* const szButtonRepeatControlName = TEXT("ButtonRepeat");
const TCHAR* const szButtonTakeControlName = TEXT("ButtonTake");
const TCHAR* const szLabelCameraTipControlName = TEXT("LabelCameraTip");

//���캯��
CCameraControl::CCameraControl(CWnd * pParent) : CFGuiDialog(IDD_DLG_CAMERA, pParent)
{
	//���ñ���
	m_bInit=false;
	m_hWndCamera=NULL;

	//����ؼ�
	m_pLabelText=NULL;
	m_pbtOk=NULL;
	m_pbtRepeat=NULL;
	m_pbtCameraDict=NULL;

	//���ñ���
	ZeroMemory(&m_CapStatus,sizeof(m_CapStatus));
	ZeroMemory(&m_CapTureparms,sizeof(m_CapTureparms));
	ZeroMemory(&m_CapDrivercaps,sizeof(m_CapDrivercaps));

	return;
}

//��������
CCameraControl::~CCameraControl()
{
}

//�ؼ���
VOID CCameraControl::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	//�����ؼ�
	DDX_Control(pDX, IDC_CAMERA_CONTROL, m_CameraControl);
}

//��Ϣ����
BOOL CCameraControl::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CCameraControl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ƶ����"));

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��ȡ��С
	CRect rcClient;
	m_CameraControl.GetClientRect(&rcClient);

	//��������
	DWORD dwStyle=WS_CHILD|WS_BORDER;
	m_hWndCamera=capCreateCaptureWindow(NULL,dwStyle,0,0,rcClient.Width(),rcClient.Height(),m_CameraControl,IDC_CAMERA_WND);

	//�����豸
	if (capDriverConnect(m_hWndCamera,0)==TRUE)
	{
		//��ȡ����
		capDriverGetCaps(m_hWndCamera,&m_CapDrivercaps,sizeof(m_CapDrivercaps));

		//����ж�
		if (m_CapDrivercaps.fCaptureInitialized==TRUE)
		{
			//���ñ���
			m_bInit=true;

			//��������
			capPreview(m_hWndCamera,TRUE);
			capPreviewRate(m_hWndCamera,30);

			//��ʾ����
			::ShowWindow(m_hWndCamera,SW_SHOW);

			//��ȡ״̬
			capGetStatus(m_hWndCamera,&m_CapStatus,sizeof(m_CapStatus));
		}
	}

	//���ÿؼ�
	if (m_bInit==true)
	{
		m_pbtCameraDict->SetEnabled(true);
		m_pLabelText->SetText(TEXT("����ͷ��ʼ���ɹ�"));
	}
	else
	{
		//������Ϣ
		m_pLabelText->SetText(TEXT("û�а�װ����ͷ��������ͷ��ʼ��ʧ��"));
	}

	return TRUE; 
}

//��ʼ�ؼ�
void CCameraControl::InitControlUI()
{
	__super::InitControlUI();

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	m_pLabelText = (CLabelUI*)GetControlByName(szLabelCameraTipControlName);

	//���水ť
	m_pbtOk = (CButtonUI*)GetControlByName(szButtonSaveContorlName);

	//���İ�ť
	m_pbtRepeat = (CButtonUI*)GetControlByName(szButtonRepeatControlName);

	//���հ�ť
	m_pbtCameraDict = (CButtonUI*)GetControlByName(szButtonTakeControlName);
}

//��Ϣ����
void CCameraControl::Notify(TNotifyUI &  msg)
{
	__super::Notify(msg);

	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (pControlUI==m_pbtOk)
		{
			return OnOK();
		}
		else if( pControlUI->GetName() == szButtonCloseContorlName)
		{
			return OnCancel();
		}
		else if (pControlUI==m_pbtRepeat)
		{
			return OnBnClickedRepeat();
		}
		else if (pControlUI==m_pbtCameraDict)
		{
			return OnBnClickedCameraDict();
		}
	}
}

//���İ�ť
VOID CCameraControl::OnBnClickedRepeat()
{
	//���ô���
	capPreview(m_hWndCamera,TRUE);

	//���ð�ť
	m_pbtOk->SetEnabled(false);
	m_pbtRepeat->SetEnabled(false);
	m_pbtCameraDict->SetEnabled(true);

	return;
}

//���հ�ť
VOID CCameraControl::OnBnClickedCameraDict()
{
	//�ػ�ͼ��
	capGrabFrame(m_hWndCamera);
	capEditCopy(m_hWndCamera);

	//����ͼ��
	if (OpenClipboard()==FALSE)
	{
		ASSERT(FALSE);
		return;
	}

	//����ͼ��
	if (m_ImageResult.IsNull()==false)
	{
		m_ImageResult.Destroy();
	}

	//��������
	HBITMAP hImageResult=(HBITMAP)CopyImage(GetClipboardData(CF_BITMAP),IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	//�������
	if (hImageResult!=NULL)
	{
		//��������
		m_ImageResult.Attach(hImageResult);

		//��ռ���
		EmptyClipboard();
		CloseClipboard();

		//���ð�ť
		m_pbtOk->SetEnabled(true);
		m_pbtRepeat->SetEnabled(true);
		m_pbtCameraDict->SetEnabled(false);

		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
