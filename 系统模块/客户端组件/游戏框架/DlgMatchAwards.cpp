#include "StdAfx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "Dlgmatchawards.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//��Ļλ��
#define LAYERED_SIZE						8									//�ֲ��С
#define ROUND_XSIZE							0									//Բ�Ǵ�С	
#define ROUND_YSIZE							0									//Բ�Ǵ�С	
#define CAPTION_SIZE						50									//�����С	

//////////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgMatchAwards::CDlgMatchAwards() : CFGuiDialog(IDD_DLG_MATCH_AWARDS)
{
	//���ñ���
	m_pMatchResult=NULL;

	//��������
	m_RewardFont.CreateFont(14,0,0,0,1200,0,0,0,134,3,2,1,2,TEXT("����"));
	m_AccountsFont.CreateFont(14,0,0,0,1200,0,0,0,134,3,2,1,2,TEXT("����"));
	m_WatermarkFont.CreateFont(21,0,0,0,100,0,0,0,134,3,2,1,2,TEXT("����"));
}

CDlgMatchAwards::~CDlgMatchAwards()
{
	//�ͷ���Դ
	m_RewardFont.DeleteObject();
	m_AccountsFont.DeleteObject();
	m_WatermarkFont.DeleteObject();
}

//���ò���
VOID CDlgMatchAwards::SetMatchResult(CMD_GR_MatchResult *pMatchResult) 
{ 
	//���ñ���
	m_pMatchResult=pMatchResult; 

	//ˢ�½���
	InvalidateRect(NULL);
}

//��ʼ�ؼ�
VOID CDlgMatchAwards::InitControlUI()
{
	//���ñ���
	CRect rcCaption(0,0,0,200);
	m_PaintManager.SetCaptionRect(rcCaption);

	//��ȡ����
	CContainerUI * pParent = static_cast<CContainerUI *>(m_PaintManager.GetRoot());

	//���ذ�ť
	m_pbtClose = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtClose->SetStatusImage(TEXT("file='BT_AWARD_CLOSE' restype='PNG'"));	
	m_pbtClose->SetRelativeSide(RelativeRight);
	m_pbtClose->SetRelativePos(CPoint(48,36));

	//���հ�ť
	m_pbtTakePhoto = CButtonUI::Create(&m_PaintManager,pParent,TEXT(""));
	m_pbtTakePhoto->SetStatusImage(TEXT("file='BT_TAKE_PHOTO' restype='PNG'"));	
	m_pbtTakePhoto->SetRelativeSide(RelativeRight);
	m_pbtTakePhoto->SetRelativePos(CPoint(83,36));
}

//��Ϣ����
VOID CDlgMatchAwards::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		//�˳���ť
		if (m_pbtClose==pControlUI) return OnBnClickedClose(); 

		//���հ�ť
		if (m_pbtTakePhoto==pControlUI) return OnBnClickedTakePhoto(); 
	}
}

//��ʼ�滭
VOID CDlgMatchAwards::OnBeginPaintWindow(HDC hDC)
{
	//��ȡ�豸
	CDC * pDC = CDC::FromHandle(hDC);

	//���û���
	pDC->SetBkMode(TRANSPARENT);

	//��ȡ����
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������	
	CPngImage ImageStar,ImageAwards,ImageStamp;

	//��Դ���
	HINSTANCE hResInstance=GetModuleHandle(GAME_FRAME_DLL_NAME);

	//����ͼƬ
	ImageStar.LoadImage(hResInstance,TEXT("MATCH_STAR"));
	ImageAwards.LoadImage(hResInstance,TEXT("MATCH_AWARDS"));

	//����λ��
	INT nXPos=(rcClient.Width()-ImageAwards.GetWidth())/2;
	INT nYPos=(rcClient.Height()-ImageAwards.GetHeight())/2;

	//�滭����
	ImageAwards.DrawImage(pDC,nXPos,nYPos);

	//�滭����
	if(m_pMatchResult!=NULL)
	{	
		//��������
		pDC->SetTextColor(RGB(255,255,255));

		//���û���
		pDC->SetTextColor(RGB(86,24,20));
		CFont * pOldFont=pDC->SelectObject(&m_AccountsFont);		

		//�滭�ǳ�
		TCHAR szNickName[64];		
		_sntprintf(szNickName,CountArray(szNickName),TEXT("%s:"),m_pMatchResult->szNickName);
		pDC->DrawText(szNickName,&CRect(nXPos+20,nYPos+74,nXPos+360,nYPos+90),DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�滭��������
		pDC->SetTextColor(RGB(190,40,20));
		pDC->DrawText(m_pMatchResult->szMatchName,&CRect(nXPos+115,nYPos+104,nXPos+310,nYPos+120),DT_CENTER|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//�滭����
		TCHAR szMatchRank[5];
		_sntprintf(szMatchRank,CountArray(szMatchRank),TEXT("%d"),m_pMatchResult->wMatchRank);
		pDC->DrawText(szMatchRank,&CRect(nXPos+371,nYPos+103,nXPos+414,nYPos+119),DT_CENTER|DT_TOP|DT_SINGLELINE|DT_END_ELLIPSIS);

		//���û���	
		pDC->SelectObject(&m_RewardFont);
		pDC->SetTextColor(RGB(190,40,20));

		//��������
		int nIndex=0;
		CString strText;

		//��Ϸ�ҽ���
		if (m_pMatchResult->lGold>0)
		{
			strText.Format(TEXT("��Ϸ��: %I64d"), m_pMatchResult->lGold);			
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+159,nXPos+360,nYPos+184),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//���ƽ���
		if (m_pMatchResult->lIngot>0)
		{
			strText.Format(TEXT("Ԫ	  ��: %I64d"), m_pMatchResult->lIngot);			
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+159+nIndex*25,nXPos+360,nYPos+184+nIndex*25),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//ʵ�ｱ��
		if (m_pMatchResult->dwExperience>0)
		{
			strText.Format(TEXT("��	  ��: %d"), m_pMatchResult->dwExperience);
			pDC->DrawText(strText, &CRect(nXPos+95,nYPos+159+nIndex*25,nXPos+360,nYPos+184+nIndex*25),DT_TOP|DT_LEFT|DT_SINGLELINE);
			nIndex++;
		}

		//���û���
		pDC->SetTextColor(RGB(247,57,23));
		pDC->SelectObject(&m_WatermarkFont);
	}
}

//��ʼ��Ϣ
BOOL CDlgMatchAwards::OnInitDialog()
{
	__super::OnInitDialog();

	//��������
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ѯ�ӿ�
	m_pIClientKernel=(IClientKernel *)pGlobalUnits->QueryGlobalModule(MODULE_CLIENT_KERNEL,IID_IClientKernel,VER_IClientKernel);
	m_pIStringMessage=m_pIClientKernel->GetIStringMessage();

	//��������
	CPngImage ImageBack;
	ImageBack.LoadImage(GetModuleHandle(GAME_FRAME_DLL_NAME), TEXT("MATCH_AWARDS"));

	//��������
	CSize SizeWindow(ImageBack.GetWidth(),ImageBack.GetHeight());

	//���ô���
	SetWindowPos(NULL,0,0,SizeWindow.cx,SizeWindow.cy,SWP_NOZORDER|SWP_NOMOVE|SWP_NOREDRAW);

	//��ȡ����
	CRect rcWindow;
	GetWindowRect(&rcWindow);

	//����λ��
	CRect rcUnLayered;
	rcUnLayered.left=LAYERED_SIZE;
	rcUnLayered.top=LAYERED_SIZE+16;	
	rcUnLayered.right=rcWindow.Width()-LAYERED_SIZE;
	rcUnLayered.bottom=rcWindow.Height()-LAYERED_SIZE;

	//��������
	CRgn RgnWindow;
	RgnWindow.CreateRoundRectRgn(rcUnLayered.left,rcUnLayered.top,rcUnLayered.right+1,rcUnLayered.bottom+1,ROUND_XSIZE,ROUND_YSIZE);

	//��������
	SetWindowRgn(RgnWindow,FALSE);

	//�ֲ㴰��
	m_SkinLayered.CreateLayered(m_hWnd);
	m_SkinLayered.InitLayeredArea(ImageBack,255,rcUnLayered,CPoint(ROUND_XSIZE,ROUND_YSIZE),false);

	//���д���
    CenterWindow();

	return TRUE; 
}

//�������
VOID CDlgMatchAwards::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags, Point);
	
	//ģ�ⰴ����
	if (Point.y<=CAPTION_SIZE)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));
	}
}


//������Ļ
HBITMAP CDlgMatchAwards::CopyScreenToBitmap(LPRECT lpRect)
{
	//����У��
	if (IsRectEmpty(lpRect)) return NULL; 

	//��������
	HDC hScrDC,hMemDC; 
	HBITMAP hOldBitmap,hBitmap;
	int nX, nY, nX2, nY2; 
	int nWidth, nHeight; 
	int xScrn, yScrn; 
	
	//�����豸
	hScrDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	hMemDC = CreateCompatibleDC(hScrDC); 

	//���ñ���
	nX = lpRect->left; 
	nY = lpRect->top; 
	nX2 = lpRect->right; 
	nY2 = lpRect->bottom; 

	//��ȡ�豸��Ϣ
	xScrn = GetDeviceCaps(hScrDC, HORZRES); 
	yScrn = GetDeviceCaps(hScrDC, VERTRES); 

	//��������
	if (nX < 0) nX = 0; 
	if (nY < 0) nY = 0; 
	if (nX2 > xScrn) nX2 = xScrn; 
	if (nY2 > yScrn) nY2 = yScrn; 

	//���ñ���
	nWidth = nX2 - nX; 
	nHeight = nY2 - nY; 

	//��������λͼ
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight); 
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); 	

	//��������
	BitBlt(hMemDC, 0, 0, nWidth, nHeight,hScrDC, nX, nY, SRCCOPY|CAPTUREBLT); 

	//��ԭ����
	hBitmap =(HBITMAP)SelectObject(hMemDC, hOldBitmap); 

	//�ͷ��豸
	DeleteDC(hScrDC); 
	DeleteDC(hMemDC); 

	return hBitmap; 
}

//��Ϸ����
bool CDlgMatchAwards::CaptureGameScreen(LPCTSTR pszSavePath)
{
	if(AfxGetMainWnd()==NULL) return false;

	//��������
	CRect rcWindow;
	m_SkinLayered.GetWindowRect(&rcWindow);

	//������Ļ
	HBITMAP hBitmap = CopyScreenToBitmap(rcWindow);

	//������ճ����
	CopyBitmapToClipBoard(hBitmap);

	//��������
	HDC hDC; 
	int iBits; 
	WORD wBitCount; 
	DWORD dwPaletteSize=0,dwBmBitsSize,dwDIBSize, dwWritten; 
	BITMAP Bitmap; 
	BITMAPFILEHEADER bmfHdr; 
	BITMAPINFOHEADER bi; 
	LPBITMAPINFOHEADER lpbi; 
	HANDLE fh,hDib,hPal,hOldPal=NULL; 

	//�����豸������
	HDC hWndDC = CreateDC(TEXT("DISPLAY"),NULL,NULL,NULL); 
	hDC = ::CreateCompatibleDC( hWndDC ) ; 
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES); 
	DeleteDC(hDC); 

	//���ñ���λ
	if (iBits <= 1) wBitCount = 1; 
	else if (iBits <= 4) wBitCount = 4; 
	else if (iBits <= 8) wBitCount = 8; 
	else if (iBits <= 24) wBitCount = 24; 
	else wBitCount = 24 ; 

	//�����ɫ���С 
	if (wBitCount <= 8) 
	dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD); 

	//����λͼ��Ϣͷ�ṹ 
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap); 
	bi.biSize = sizeof(BITMAPINFOHEADER); 
	bi.biWidth = Bitmap.bmWidth; 
	bi.biHeight = Bitmap.bmHeight; 
	bi.biPlanes = 1; 
	bi.biBitCount = wBitCount; 
	bi.biCompression = BI_RGB; 
	bi.biSizeImage = 0; 
	bi.biXPelsPerMeter = 0; 
	bi.biYPelsPerMeter = 0; 
	bi.biClrUsed = 0; 
	bi.biClrImportant = 0; 

	//����λͼ��С
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount+31)/32) * 4 * Bitmap.bmHeight ; 

	//�����ڴ� 
	hDib = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER)); 
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib); 
	*lpbi = bi; 

	// �����ɫ�� 
	hPal = GetStockObject(DEFAULT_PALETTE); 
	if (hPal) 
	{ 
		hDC = ::GetDC(NULL); 
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE); 
		RealizePalette(hDC); 
	} 

	// ��ȡ�õ�ɫ�����µ�����ֵ 
	GetDIBits(hDC,hBitmap,0,(UINT) Bitmap.bmHeight,(LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwPaletteSize,(LPBITMAPINFO ) 
	lpbi,DIB_RGB_COLORS); 

	//�ָ���ɫ�� 
	if (hOldPal) 
	{ 
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC); 
		::ReleaseDC(NULL, hDC); 
	} 

	//����λͼ�ļ� 
	fh = CreateFile(pszSavePath,GENERIC_WRITE,0, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (fh == INVALID_HANDLE_VALUE) return false; 

	// ����λͼ�ļ�ͷ 
	bmfHdr.bfType = 0x4D42;
	dwDIBSize = sizeof(BITMAPFILEHEADER) 
	+ sizeof(BITMAPINFOHEADER) 
	+ dwPaletteSize + dwBmBitsSize; 
	bmfHdr.bfSize = dwDIBSize; 
	bmfHdr.bfReserved1 = 0; 
	bmfHdr.bfReserved2 = 0; 
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) 
	+ (DWORD)sizeof(BITMAPINFOHEADER) 
	+ dwPaletteSize; 

	// д��λͼ�ļ�ͷ 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL); 

	// д��λͼ�ļ��������� 
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,&dwWritten, NULL); 

	//��� 
	GlobalUnlock(hDib); 
	GlobalFree(hDib); 
	CloseHandle(fh); 

	return true; 
}

//����λͼ
bool CDlgMatchAwards::CopyBitmapToClipBoard(HBITMAP hBitmap)
{
	//��������
	HWND hWnd=AfxGetMainWnd()->GetSafeHwnd();

	//�򿪼��а�
	if (::OpenClipboard(hWnd)==FALSE) return false;
	if (::EmptyClipboard()==FALSE) { CloseClipboard(); return false; }

	//����λͼ
	::SetClipboardData(CF_BITMAP,hBitmap);  

	//��������
	::CloseClipboard();

	return true;
}

//�ر���Ϣ
VOID CDlgMatchAwards::OnBnClickedClose()
{
	__super::OnCancel();

	//���ٴ���
	DestroyWindow();

	//�ر�������
	AfxGetMainWnd()->PostMessage(WM_CLOSE,0,0);
}

//������Ϣ
VOID CDlgMatchAwards::OnBnClickedTakePhoto()
{
	//����Ŀ¼
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));

	TCHAR szBitmapFilePath[MAX_PATH]={0};
	_sntprintf(szBitmapFilePath,CountArray(szBitmapFilePath),TEXT("%s\\MatchAwards"),szDirectory);

	//����Ŀ¼
	SHCreateDirectoryEx(NULL,szBitmapFilePath,NULL);

	//��������
	TCHAR szBitmapFile[MAX_PATH]={0};
	_sntprintf(szBitmapFile,CountArray(szBitmapFile),TEXT("%s\\%d_%d.jpg"),szBitmapFilePath,time(NULL),GetTickCount());	

	//ִ�н���
	if(CaptureGameScreen(szBitmapFile))
	{
		//��������
		TCHAR szMessage[MAX_PATH]={0};
		_sntprintf(szMessage,CountArray(szMessage),TEXT("���Ƴɹ������Ҽ�ճ����QQ�Ի���,ͼ���ѱ���Ϊ: %s"),szBitmapFile);

		//������Ϣ
		if(m_pIStringMessage!=NULL)
		{
			m_pIStringMessage->InsertPromptString(szMessage);
		}

		//��������
		g_GlobalUnits.PlayGameSound(GetModuleHandle(GAME_FRAME_DLL_NAME),TEXT("IDR_TAKE_PHOTO"));
	}
}