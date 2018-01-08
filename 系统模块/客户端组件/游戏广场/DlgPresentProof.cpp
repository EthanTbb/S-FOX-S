#include "StdAfx.h"
#include "Resource.h"
#include "Dlgpresentproof.h"
#include "InsureCommon.h"

//��ǩ�ؼ�
const TCHAR* const szLabelByAccountControlName = TEXT("LabelByAccount");
const TCHAR* const szLabelByIDControlName = TEXT("LabelByID");
const TCHAR* const szLabelTargetAccountControlName = TEXT("LabelTargetAccount");
const TCHAR* const szLabelTargetIDControlName = TEXT("LabelTargetID");
const TCHAR* const szLabelTransferScore = TEXT("TransferScore");
const TCHAR* const szLabelChineseNumControlName = TEXT("LabelChineseNum");
const TCHAR* const szLabelDateControlName = TEXT("LabelDate");
const TCHAR* const szLabelProofNumControlName = TEXT("LabelProofNum");

//��ť�ؼ�
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonSaveAndCloseControlName = TEXT("ButtonSaveAndClose");
const TCHAR* const szButtonSaveNotCloseControlName = TEXT("ButtonSaveNotClose");

const TCHAR* const szInsureMainControlName = TEXT("InsureMain");
//////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgPresentProof::CDlgPresentProof() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//��������
CDlgPresentProof::~CDlgPresentProof()
{
}

//��������
BOOL CDlgPresentProof::OnInitDialog()
{
	__super::OnInitDialog();

	//�޸���ʽ
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//���ñ���
	SetWindowText(TEXT("����ƾ֤"));

	//���д���
	CenterWindow(this);

	return TRUE;
}

//ȷ������
VOID CDlgPresentProof::OnOK()
{
	CControlUI * pControl = static_cast<CControlUI *>(GetControlByName(szInsureMainControlName));
	if (pControl)
	{
		HBITMAP pImg = CRenderEngine::GenerateBitmap(&m_PaintManager,pControl,pControl->GetPos());

		if (pImg)
		{
			//����Ŀ¼
			TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

			//�ļ�Ŀ¼
			TCHAR szUserGameForder[MAX_PATH]=TEXT("");
			_sntprintf(szUserGameForder,CountArray(szUserGameForder),TEXT("%s\\Proof"),szWorkDirectory);
			if (!PathIsDirectory(szUserGameForder))
			{
				::CreateDirectory(szUserGameForder, NULL);
			}

			//�û���Ϣ
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			
			//��ȡʱ��
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);

			//����ǰ��
			TCHAR szTimeString[128]=TEXT("");
			_sntprintf(szTimeString,CountArray(szTimeString),TEXT("%d%d%d%02d%02d%02d"),SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

			//��������
			TCHAR szUserGameName[MAX_PATH]=TEXT("");
			_sntprintf(szUserGameName,CountArray(szUserGameName),TEXT("%s\\Proof%d%d%s.bmp"),szUserGameForder,pGlobalUserData->dwGameID,m_tagTransferScoreInfo.dwTargetID,szTimeString);

			SaveBmp(pImg,szUserGameName);
		}
		
	}

	return __super::OnOK();
}

//ȡ����Ϣ
VOID CDlgPresentProof::OnCancel()
{
	return __super::OnCancel();
}

void CDlgPresentProof::InitControlUI()
{
	//��ȡ����
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//���ÿؼ�
	CLabelUI *pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelByAccountControlName));
	if(pLabelUI != NULL) pLabelUI->SetText(pGlobalUserData->szAccounts);
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelByIDControlName));
	if(pLabelUI != NULL)
	{
		TCHAR szText[64] = {0};
		_sntprintf(szText,CountArray(szText),TEXT("%d"),pGlobalUserData->dwGameID);
		pLabelUI->SetText(szText);
	}
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelTargetAccountControlName));
	if(pLabelUI != NULL) pLabelUI->SetText(m_tagTransferScoreInfo.szTargetAccounts);
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelTargetIDControlName));
	if(pLabelUI != NULL)
	{
		TCHAR szText[64] = {0};
		_sntprintf(szText,CountArray(szText),TEXT("%d"),m_tagTransferScoreInfo.dwTargetID);
		pLabelUI->SetText(szText);
	}
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelTransferScore));
	if(pLabelUI != NULL)
	{
		TCHAR szText[64] = {0};
		_sntprintf(szText,CountArray(szText),TEXT("%I64d"),m_tagTransferScoreInfo.lTransferScore);
		pLabelUI->SetText(szText);
	}
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelChineseNumControlName));
	if(pLabelUI != NULL)
	{
		TCHAR szBuffer[256]=TEXT("");
		TCHAR szSource[256]=TEXT("");
		_sntprintf(szSource,CountArray(szSource),TEXT("%I64d"),m_tagTransferScoreInfo.lTransferScore);
		CNumberConvert nc;
		nc.NumberConvert(szSource, szBuffer,(int)(_tcslen(szSource)));
		pLabelUI->SetText(szBuffer);
	}
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelDateControlName));
	if(pLabelUI != NULL)
	{
		//��ȡʱ��
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//����ǰ��
		TCHAR szTimeString[128]=TEXT("");
		_sntprintf(szTimeString,CountArray(szTimeString),TEXT("%d.%d.%d��%02d:%02d:%02d"),SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		pLabelUI->SetText(szTimeString);
	}
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelProofNumControlName));
	if(pLabelUI != NULL)
	{
		GUID Guid;
		CoCreateGuid(&Guid);
		//����ǰ��
		TCHAR szTimeString[128]=TEXT("");
		_sntprintf(szTimeString,CountArray(szTimeString),TEXT("%X-%X-%X-%X"),Guid.Data1,Guid.Data2,Guid.Data3,Guid.Data4);
		pLabelUI->SetText(szTimeString);
	}


	return __super::InitControlUI();
}

//��Ϣ����
void CDlgPresentProof::Notify(TNotifyUI &  msg)
{
	//��ȡ����
	CControlUI * pControlUI = msg.pSender;

	//����¼�
	if (lstrcmp(msg.sType, TEXT("click")) == 0)
	{
		if(pControlUI->GetName() == szButtonCloseControlName)
		{
			OnCancel();
			return;
		}
		if(pControlUI->GetName() == szButtonSaveNotCloseControlName)
		{
			OnCancel();
			return;
		}
		if(pControlUI->GetName() == szButtonSaveAndCloseControlName)
		{
			OnOK();
			return;
		}
		return;
	}

	return;
}

LPCTSTR CDlgPresentProof::GetSkinFile()
{
	return TEXT("DlgPresentProof.xml");
}

//ת����Ϣ
void CDlgPresentProof::SetTransferScoreInfo(tagTransferScoreInfo TransferScoreInfo)
{
	m_tagTransferScoreInfo.lTransferScore = TransferScoreInfo.lTransferScore;
	m_tagTransferScoreInfo.dwTargetID = TransferScoreInfo.dwTargetID;
	lstrcpyn(m_tagTransferScoreInfo.szTargetAccounts,TransferScoreInfo.szTargetAccounts,CountArray(TransferScoreInfo.szTargetAccounts));
	lstrcpyn(m_tagTransferScoreInfo.szTransRemark,TransferScoreInfo.szTransRemark,CountArray(TransferScoreInfo.szTransRemark));

	return;
}



BOOL CDlgPresentProof:: SaveBmp(HBITMAP     hBitmap,   CString     FileName)       
{       
	HDC     hDC;       
	//��ǰ�ֱ�����ÿ������ռ�ֽ���       
	int     iBits;       
	//λͼ��ÿ������ռ�ֽ���       
	WORD     wBitCount;       
	//�����ɫ���С��     λͼ�������ֽڴ�С     ��λͼ�ļ���С     ��     д���ļ��ֽ���           
	DWORD     dwPaletteSize=0,   dwBmBitsSize=0,   dwDIBSize=0,   dwWritten=0;           
	//λͼ���Խṹ           
	BITMAP     Bitmap;               
	//λͼ�ļ�ͷ�ṹ       
	BITMAPFILEHEADER     bmfHdr;               
	//λͼ��Ϣͷ�ṹ           
	BITMAPINFOHEADER     bi;               
	//ָ��λͼ��Ϣͷ�ṹ               
	LPBITMAPINFOHEADER     lpbi;               
	//�����ļ��������ڴ�������ɫ����           
	HANDLE     fh,   hDib,   hPal,hOldPal=NULL;           

	//����λͼ�ļ�ÿ��������ռ�ֽ���           
	hDC  = CreateDC(_T("DISPLAY"),   NULL,   NULL,   NULL);       
	iBits  = GetDeviceCaps(hDC,   BITSPIXEL)     *     GetDeviceCaps(hDC,   PLANES);           
	DeleteDC(hDC);           
	if(iBits <=  1)                                                 
		wBitCount = 1;           
	else  if(iBits <=  4)                             
		wBitCount  = 4;           
	else if(iBits <=  8)                             
		wBitCount  = 8;           
	else                                                                                                                             
		wBitCount  = 24;           

	GetObject(hBitmap,   sizeof(Bitmap),   (LPSTR)&Bitmap);       
	bi.biSize= sizeof(BITMAPINFOHEADER);       
	bi.biWidth = Bitmap.bmWidth;       
	bi.biHeight =  Bitmap.bmHeight;       
	bi.biPlanes =  1;       
	bi.biBitCount = wBitCount;       
	bi.biCompression= BI_RGB;       
	bi.biSizeImage=0;       
	bi.biXPelsPerMeter = 0;       
	bi.biYPelsPerMeter = 0;       
	bi.biClrImportant = 0;       
	bi.biClrUsed =  0;       

	dwBmBitsSize  = ((Bitmap.bmWidth *wBitCount+31) / 32)*4* Bitmap.bmHeight;       

	//Ϊλͼ���ݷ����ڴ�           
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));           
	lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);           
	*lpbi  = bi;           

	//     �����ɫ��               
	hPal  = GetStockObject(DEFAULT_PALETTE);           
	if (hPal)           
	{           
		hDC  = ::GetDC(NULL);           
		hOldPal = ::SelectPalette(hDC,(HPALETTE)hPal, FALSE);           
		RealizePalette(hDC);           
	}       

	//     ��ȡ�õ�ɫ�����µ�����ֵ           
	GetDIBits(hDC,hBitmap, 0,(UINT)Bitmap.bmHeight,
		(LPSTR)lpbi+ sizeof(BITMAPINFOHEADER)+dwPaletteSize, 
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);           

	//�ָ���ɫ��               
	if (hOldPal)           
	{           
		::SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);           
		RealizePalette(hDC);           
		::ReleaseDC(NULL,   hDC);           
	}           

	//����λͼ�ļ�               
	fh  = CreateFile(FileName,   GENERIC_WRITE,0,   NULL,   CREATE_ALWAYS,         
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,   NULL);           

	if (fh     ==  INVALID_HANDLE_VALUE)         return     FALSE;           

	//     ����λͼ�ļ�ͷ           
	bmfHdr.bfType  = 0x4D42;     //     "BM"           
	dwDIBSize  = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;               
	bmfHdr.bfSize  = dwDIBSize;           
	bmfHdr.bfReserved1  = 0;           
	bmfHdr.bfReserved2  = 0;           
	bmfHdr.bfOffBits  = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;           
	//     д��λͼ�ļ�ͷ           
	WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);           
	//     д��λͼ�ļ���������           
	WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize,   &dwWritten,   NULL);           
	//���               
	GlobalUnlock(hDib);           
	GlobalFree(hDib);           
	CloseHandle(fh);           

	return     TRUE;       
}
