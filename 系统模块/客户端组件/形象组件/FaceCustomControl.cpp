#include "StdAfx.h"
#include "Resource.h"
#include "CameraControl.h"
#include "FaceCustomControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_BT_TURN_LEFT			100									//��ת��ť
#define IDC_BT_TURN_RIGHT			101									//��ת��ť
#define IDC_BT_SHOW_VLINE			102									//���ư�ť
#define IDC_BT_LOAD_IMAGE			103									//����ͼƬ
#define IDC_BT_LOAD_CAMERA			104									//��������

//�ؼ���ʶ
#define IDC_IMAGE_EDIT_CTRL			200									//�༭�ؼ�

//��ɫ����
#define CR_BACK_GROUND				RGB(189,173,139)					//������ɫ

//////////////////////////////////////////////////////////////////////////////////
const TCHAR* const szTabLayoutCustomControlName = TEXT("TabLayoutCustom");
const TCHAR* const szButtonLocalControlName = TEXT("ButtonLocal"); 
const TCHAR* const szButtonSelfieControlName = TEXT("ButtonSelfie"); 
const TCHAR* const szButtonCWControlName = TEXT("ButtonCW");
const TCHAR* const szButtonCCWControlName = TEXT("ButtonCCW");
const TCHAR* const szButtonCornerControlName = TEXT("ButtonCorner");
const TCHAR* const szButtonReturnControlName = TEXT("ButtonReturn");


BEGIN_MESSAGE_MAP(CFaceItemCustomWnd, CFGuiWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_CREATE()

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFaceItemCustomWnd::CFaceItemCustomWnd():m_pTabLayoutCustom(NULL)
{
	//����ؼ�
	m_pbtTurnLeft=NULL;
	m_pbtTurnRight=NULL;
	m_pbtShowVLine=NULL;
	m_pbtLoadImage=NULL;
	m_pbtLoadCamera=NULL;
}

//��������
CFaceItemCustomWnd::~CFaceItemCustomWnd()
{
}

//�����ж�
bool CFaceItemCustomWnd::IsNull()
{
	return m_ImageEditorControl.IsNull();
}

//����ͼƬ
VOID CFaceItemCustomWnd::DestoryImage()
{
	//״̬�ж�
	if (m_ImageEditorControl.IsNull()==false)
	{
		//���ÿؼ�
		m_pbtTurnLeft->SetVisible(false);
		m_pbtTurnRight->SetVisible(false);
		m_pbtShowVLine->SetVisible(false);

		//����ͼƬ
		m_ImageEditorControl.DestoryImage();
		m_ImageEditorControl.ShowWindow(SW_HIDE);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	}

	return;
}

//��ʼ�ؼ�
VOID CFaceItemCustomWnd::InitControlUI()
{
	__super::InitControlUI();

	m_pTabLayoutCustom = (CTabLayoutUI*)GetControlByName(szTabLayoutCustomControlName);

	//��ת��ť
	m_pbtTurnLeft = (CButtonUI*)GetControlByName(szButtonCWControlName);
	if(m_pbtTurnLeft)m_pbtTurnLeft->SetVisible(false);

	//��ת��ť
	m_pbtTurnRight = (CButtonUI*)GetControlByName(szButtonCCWControlName);
	if(m_pbtTurnRight)m_pbtTurnRight->SetVisible(false);

	//���߰�ť
	m_pbtShowVLine = (CButtonUI*)GetControlByName(szButtonCornerControlName);
	if(m_pbtShowVLine) m_pbtShowVLine->SetVisible(false);

	//����ͼƬ
	m_pbtLoadImage = (CButtonUI*)GetControlByName(szButtonLocalControlName);

	//��������
	m_pbtLoadCamera = (CButtonUI*)GetControlByName(szButtonSelfieControlName);

	m_bLoadCustom = false;
}

//��Ϣ����
VOID CFaceItemCustomWnd::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if (pControlUI==m_pbtTurnLeft)
		{
			return OnBnClickedTurnLeft();
		}
		else if (pControlUI==m_pbtTurnRight)
		{
			return OnBnClickedTurnRight();
		}
		else if (pControlUI==m_pbtShowVLine)
		{
			return OnBnClickedShowVLine();
		}
		else if (pControlUI==m_pbtLoadImage)
		{
			return OnBnClickedLoadImage();
		}
		else if (pControlUI==m_pbtLoadCamera)
		{
			return OnBnClickedLoadCamera();
		}
		else if( pControlUI->GetName() == szButtonReturnControlName )
		{
			if(m_pTabLayoutCustom) m_pTabLayoutCustom->SelectItem(0);
			//���ÿؼ�
			m_ImageEditorControl.DestoryImage();
			m_ImageEditorControl.ShowWindow(SW_HIDE);
		}
	}
}

//�滭��Ϣ
VOID CFaceItemCustomWnd::OnBeginPaintWindow(HDC hDC)
{
	return;
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	pDC->FillSolidRect(0,0,rcClient.Width(),rcClient.Height(),CR_BACK_GROUND);

	//����ͼƬ
	CPngImage ImageFaceEditBack;
	ImageFaceEditBack.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("FACE_EDIT_BACK"));

	//�滭����
	ImageFaceEditBack.DrawImage(pDC,30,70);

	//��ȡ����
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//���û���
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(pSkinResourceManager->GetDefaultFont());

	//�滭����
	if (m_ImageEditorControl.IsWindowVisible()==FALSE)
	{
		pDC->TextOut(60,150,TEXT("������ѡ������һ�ַ�ʽ����ͷ��:"));
		pDC->TextOut(60,180,TEXT("������Ƭ��ѡ��һ�ű���ͼƬ�༭���ϴ�Ϊͷ��"));
		pDC->TextOut(60,205,TEXT("����ͷ��ͨ������ͷ���ձ༭���ϴ�Ϊͷ��"));
	}

	return;
}

//���ýӿ�
VOID CFaceItemCustomWnd::SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent)
{
	//���ýӿ�
	m_ImageEditorControl.SetImageEditorEvent(pIImageEditorEvent);

	return;
}

//��ȡͼƬ
VOID CFaceItemCustomWnd::DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//��ȡͼƬ
	m_ImageEditorControl.DrawEditImage(pDC,nXPos,nYPos,nWidth,nHeight);

	return;
}

//λ����Ϣ
VOID CFaceItemCustomWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//�ƶ��ؼ�
	if( ::IsWindow( m_ImageEditorControl.GetSafeHwnd() ) )
	{
		SIZE szEditor={451, 276};
		m_ImageEditorControl.SetWindowPos(NULL,(cx-szEditor.cx)/2,12,szEditor.cx,szEditor.cy,SWP_NOZORDER);
	}
	return;
}

//������Ϣ
INT CFaceItemCustomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=GetModuleHandle(AVATAR_CONTROL_DLL_NAME);

	//�����ؼ�
	m_ImageEditorControl.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_IMAGE_EDIT_CTRL);

	return 0;
}

//��תͼ��
VOID CFaceItemCustomWnd::OnBnClickedTurnLeft()
{
	//ѡ��ͼƬ
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_LEFT);

	return;
}

//��תͼ��
VOID CFaceItemCustomWnd::OnBnClickedTurnRight()
{
	//ѡ��ͼƬ
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_RIGHT);

	return;
}

//��ʾ����
VOID CFaceItemCustomWnd::OnBnClickedShowVLine()
{
	//��������
	bool bShowVLine=m_ImageEditorControl.GetVLineStatus();
	m_ImageEditorControl.SetVLineStatus((bShowVLine==false));

	return;
}

//����ͼ��
VOID CFaceItemCustomWnd::OnBnClickedLoadImage()
{
	//��������
	CFileDialog FileDialog(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,TEXT("ͼ���ļ���*.BMP *.PNG *.JPG��|*.BMP;*.PNG;*JPG||"),this);

	//���ļ�
	if (FileDialog.DoModal()==IDOK)
	{
		//����ͼƬ
		if (m_ImageEditorControl.LoadEditImage(FileDialog.GetPathName())==false)
		{
			//���ð�ť
			m_pbtTurnLeft->SetVisible(false);
			m_pbtTurnRight->SetVisible(false);
			m_pbtShowVLine->SetVisible(false);

			//���ÿؼ�
			m_ImageEditorControl.ShowWindow(SW_HIDE);

			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("ͼƬ����ʧ�ܣ���ѡ������ͼƬ���ԣ�"),MB_ICONERROR,0);
		}
		else
		{
			//���ð�ť
			m_pbtTurnLeft->SetVisible(true);
			m_pbtTurnRight->SetVisible(true);
			m_pbtShowVLine->SetVisible(true);
			if(m_pTabLayoutCustom) m_pTabLayoutCustom->SelectItem(1);
			//���ÿؼ�
			m_ImageEditorControl.ShowWindow(SW_SHOW);

			m_bLoadCustom = true;
		}
	}

	return;
}

//����ͼ��
VOID CFaceItemCustomWnd::OnBnClickedLoadCamera()
{
	//��������
	CCameraControl CameraControl(this);

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(AVATAR_CONTROL_DLL_NAME));

	//����ͼƬ
	if (CameraControl.DoModal()==IDOK)
	{
		//����ͼƬ
		m_ImageEditorControl.ShowWindow(SW_SHOW);

		//����ͼƬ
		ASSERT(CameraControl.m_ImageResult.IsNull()==false);
		bool bSuccess=m_ImageEditorControl.LoadEditImage(CameraControl.m_ImageResult);

		//���ð�ť
		m_pbtTurnLeft->SetEnabled(bSuccess);
		m_pbtTurnRight->SetEnabled(bSuccess);
		m_pbtShowVLine->SetEnabled(bSuccess);

		m_bLoadCustom = true;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
