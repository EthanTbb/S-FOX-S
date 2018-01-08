#include "StdAfx.h"
#include "Resource.h"
#include "Dlgpresentproof.h"
#include "InsureCommon.h"

//标签控件
const TCHAR* const szLabelByAccountControlName = TEXT("LabelByAccount");
const TCHAR* const szLabelByIDControlName = TEXT("LabelByID");
const TCHAR* const szLabelTargetAccountControlName = TEXT("LabelTargetAccount");
const TCHAR* const szLabelTargetIDControlName = TEXT("LabelTargetID");
const TCHAR* const szLabelTransferScore = TEXT("TransferScore");
const TCHAR* const szLabelChineseNumControlName = TEXT("LabelChineseNum");
const TCHAR* const szLabelDateControlName = TEXT("LabelDate");
const TCHAR* const szLabelProofNumControlName = TEXT("LabelProofNum");

//按钮控件
const TCHAR* const szButtonCloseControlName = TEXT("ButtonClose");
const TCHAR* const szButtonSaveAndCloseControlName = TEXT("ButtonSaveAndClose");
const TCHAR* const szButtonSaveNotCloseControlName = TEXT("ButtonSaveNotClose");

const TCHAR* const szInsureMainControlName = TEXT("InsureMain");
//////////////////////////////////////////////////////////////////////////////////

//构造函数
CDlgPresentProof::CDlgPresentProof() : CFGuiDialog(IDD_DLG_POPUP)
{
}

//析构函数
CDlgPresentProof::~CDlgPresentProof()
{
}

//创建函数
BOOL CDlgPresentProof::OnInitDialog()
{
	__super::OnInitDialog();

	//修改样式
	ModifyStyle(0, WS_MINIMIZEBOX|WS_MAXIMIZEBOX);

	//设置标题
	SetWindowText(TEXT("赠送凭证"));

	//居中窗口
	CenterWindow(this);

	return TRUE;
}

//确定函数
VOID CDlgPresentProof::OnOK()
{
	CControlUI * pControl = static_cast<CControlUI *>(GetControlByName(szInsureMainControlName));
	if (pControl)
	{
		HBITMAP pImg = CRenderEngine::GenerateBitmap(&m_PaintManager,pControl,pControl->GetPos());

		if (pImg)
		{
			//工作目录
			TCHAR szWorkDirectory[MAX_PATH]=TEXT("");
			CWHService::GetWorkDirectory(szWorkDirectory,CountArray(szWorkDirectory));

			//文件目录
			TCHAR szUserGameForder[MAX_PATH]=TEXT("");
			_sntprintf(szUserGameForder,CountArray(szUserGameForder),TEXT("%s\\Proof"),szWorkDirectory);
			if (!PathIsDirectory(szUserGameForder))
			{
				::CreateDirectory(szUserGameForder, NULL);
			}

			//用户信息
			CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
			tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

			
			//获取时间
			SYSTEMTIME SystemTime;
			GetLocalTime(&SystemTime);

			//插入前叠
			TCHAR szTimeString[128]=TEXT("");
			_sntprintf(szTimeString,CountArray(szTimeString),TEXT("%d%d%d%02d%02d%02d"),SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);

			//变量定义
			TCHAR szUserGameName[MAX_PATH]=TEXT("");
			_sntprintf(szUserGameName,CountArray(szUserGameName),TEXT("%s\\Proof%d%d%s.bmp"),szUserGameForder,pGlobalUserData->dwGameID,m_tagTransferScoreInfo.dwTargetID,szTimeString);

			SaveBmp(pImg,szUserGameName);
		}
		
	}

	return __super::OnOK();
}

//取消消息
VOID CDlgPresentProof::OnCancel()
{
	return __super::OnCancel();
}

void CDlgPresentProof::InitControlUI()
{
	//获取对象
	CGlobalUserInfo * pGlobalUserInfo = CGlobalUserInfo::GetInstance();
	tagGlobalUserData * pGlobalUserData = pGlobalUserInfo->GetGlobalUserData();

	//设置控件
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
		//获取时间
		SYSTEMTIME SystemTime;
		GetLocalTime(&SystemTime);

		//插入前叠
		TCHAR szTimeString[128]=TEXT("");
		_sntprintf(szTimeString,CountArray(szTimeString),TEXT("%d.%d.%d—%02d:%02d:%02d"),SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
		pLabelUI->SetText(szTimeString);
	}
	pLabelUI = static_cast<CLabelUI *>(GetControlByName(szLabelProofNumControlName));
	if(pLabelUI != NULL)
	{
		GUID Guid;
		CoCreateGuid(&Guid);
		//插入前叠
		TCHAR szTimeString[128]=TEXT("");
		_sntprintf(szTimeString,CountArray(szTimeString),TEXT("%X-%X-%X-%X"),Guid.Data1,Guid.Data2,Guid.Data3,Guid.Data4);
		pLabelUI->SetText(szTimeString);
	}


	return __super::InitControlUI();
}

//消息提醒
void CDlgPresentProof::Notify(TNotifyUI &  msg)
{
	//获取对象
	CControlUI * pControlUI = msg.pSender;

	//点击事件
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

//转账信息
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
	//当前分辨率下每象素所占字节数       
	int     iBits;       
	//位图中每象素所占字节数       
	WORD     wBitCount;       
	//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数           
	DWORD     dwPaletteSize=0,   dwBmBitsSize=0,   dwDIBSize=0,   dwWritten=0;           
	//位图属性结构           
	BITMAP     Bitmap;               
	//位图文件头结构       
	BITMAPFILEHEADER     bmfHdr;               
	//位图信息头结构           
	BITMAPINFOHEADER     bi;               
	//指向位图信息头结构               
	LPBITMAPINFOHEADER     lpbi;               
	//定义文件，分配内存句柄，调色板句柄           
	HANDLE     fh,   hDib,   hPal,hOldPal=NULL;           

	//计算位图文件每个像素所占字节数           
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

	//为位图内容分配内存           
	hDib  = GlobalAlloc(GHND,dwBmBitsSize+dwPaletteSize+sizeof(BITMAPINFOHEADER));           
	lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);           
	*lpbi  = bi;           

	//     处理调色板               
	hPal  = GetStockObject(DEFAULT_PALETTE);           
	if (hPal)           
	{           
		hDC  = ::GetDC(NULL);           
		hOldPal = ::SelectPalette(hDC,(HPALETTE)hPal, FALSE);           
		RealizePalette(hDC);           
	}       

	//     获取该调色板下新的像素值           
	GetDIBits(hDC,hBitmap, 0,(UINT)Bitmap.bmHeight,
		(LPSTR)lpbi+ sizeof(BITMAPINFOHEADER)+dwPaletteSize, 
		(BITMAPINFO *)lpbi, DIB_RGB_COLORS);           

	//恢复调色板               
	if (hOldPal)           
	{           
		::SelectPalette(hDC,   (HPALETTE)hOldPal,   TRUE);           
		RealizePalette(hDC);           
		::ReleaseDC(NULL,   hDC);           
	}           

	//创建位图文件               
	fh  = CreateFile(FileName,   GENERIC_WRITE,0,   NULL,   CREATE_ALWAYS,         
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,   NULL);           

	if (fh     ==  INVALID_HANDLE_VALUE)         return     FALSE;           

	//     设置位图文件头           
	bmfHdr.bfType  = 0x4D42;     //     "BM"           
	dwDIBSize  = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+dwPaletteSize+dwBmBitsSize;               
	bmfHdr.bfSize  = dwDIBSize;           
	bmfHdr.bfReserved1  = 0;           
	bmfHdr.bfReserved2  = 0;           
	bmfHdr.bfOffBits  = (DWORD)sizeof(BITMAPFILEHEADER)+(DWORD)sizeof(BITMAPINFOHEADER)+dwPaletteSize;           
	//     写入位图文件头           
	WriteFile(fh,   (LPSTR)&bmfHdr,   sizeof(BITMAPFILEHEADER),   &dwWritten,   NULL);           
	//     写入位图文件其余内容           
	WriteFile(fh,   (LPSTR)lpbi,   dwDIBSize,   &dwWritten,   NULL);           
	//清除               
	GlobalUnlock(hDib);           
	GlobalFree(hDib);           
	CloseHandle(fh);           

	return     TRUE;       
}
