#include "Stdafx.h"
#include "SkinImage.h"

/////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CSkinImage::CSkinImage(void)
{
	m_nRefCount=0;
	m_bLoadAlways=false;
}

//析构函数
CSkinImage::~CSkinImage(void)
{
	Destroy();
}

//增加引用
int CSkinImage::LockResource()
{
	if (m_nRefCount==0)
	{
		if (m_LoadInfo.hResourceDLL==NULL) Load(m_LoadInfo.strFileName);
		else if (m_LoadInfo.uResourceID == 0)
			LoadFromResource(m_LoadInfo.hResourceDLL,m_LoadInfo.strFileName);
		else  
			LoadFromResource(m_LoadInfo.hResourceDLL,m_LoadInfo.uResourceID);

		if (IsNull()) return 0;
	}
	return ++m_nRefCount;
}

//减少引用
int CSkinImage::UnLockResource()
{
	try
	{
		if (m_nRefCount>0) m_nRefCount--;
		if (m_nRefCount==0) Destroy();
	}
	catch (...) {}
	return m_nRefCount;
}

//是否设置加载信息
bool CSkinImage::IsSetLoadInfo()
{
	return ((m_LoadInfo.hResourceDLL)||(m_LoadInfo.strFileName.IsEmpty()==false));
}

//清除加载参数
bool CSkinImage::RemoveLoadInfo()
{
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=NULL;
	m_LoadInfo.strFileName.Empty();
	return true;
}

//获取加载参数
bool CSkinImage::GetLoadInfo(tagImageLoadInfo & LoadInfo)
{
	LoadInfo=m_LoadInfo;
	return IsSetLoadInfo();
}

//设置加载参数
bool CSkinImage::SetLoadInfo(LPCTSTR pszFileName, bool bLoadAlways)
{
	//保存变量
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//清理资源
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;

	//设置变量
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=NULL;
	m_LoadInfo.strFileName=pszFileName;

	//恢复锁定
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//设置加载参数
bool CSkinImage::SetLoadInfo(UINT uResourceID, HINSTANCE hResourceDLL, bool bLoadAlways)
{
	//保存变量
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//清理资源
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;
	m_LoadInfo.strFileName.Empty();

	//设置变量
	m_LoadInfo.uResourceID=uResourceID;
	m_LoadInfo.hResourceDLL=hResourceDLL;

	//恢复锁定
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

bool CSkinImage::SetLoadInfo(LPCTSTR szResourceName, HINSTANCE hResourceDLL, bool bLoadAlways)
{
	//保存变量
	int nOldRefCount=m_nRefCount;
	bool bOldLoadAlways=m_bLoadAlways;

	//清理资源
	Destroy();
	m_nRefCount=0;
	m_bLoadAlways=bLoadAlways;
	m_LoadInfo.strFileName.Empty();

	//设置变量
	m_LoadInfo.uResourceID=0;
	m_LoadInfo.hResourceDLL=hResourceDLL;
	m_LoadInfo.strFileName.Append(szResourceName);

	//恢复锁定
	if (bOldLoadAlways) nOldRefCount--;
	if (m_bLoadAlways) nOldRefCount++;
	while (nOldRefCount>0)
	{
		nOldRefCount--;
		LockResource();
	};

	return true;
}

//创建区域
HRGN CSkinImage::CreateImageRegion(COLORREF cTransparentColor, COLORREF cTolerance)
{
	HDC pdc = GetDC();

	HRGN	hRgn = NULL;
	HBITMAP hBmp;
	if (pdc == NULL) return NULL;

	hBmp = (HBITMAP)GetCurrentObject(pdc, OBJ_BITMAP);
	if (hBmp)
	{
		BITMAP bm;
		memset(&bm, 0, sizeof(bm));
		::GetObject(hBmp, sizeof(bm), &bm);
		while (bm.bmWidthBytes % 4)
			bm.bmWidthBytes++;

#define ALLOC_UNIT	100
		DWORD maxRects = ALLOC_UNIT;
		HANDLE hData = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
		RGNDATA *pData = (RGNDATA *)::GlobalLock(hData);
		pData->rdh.dwSize = sizeof(RGNDATAHEADER);
		pData->rdh.iType = RDH_RECTANGLES;
		pData->rdh.nCount = pData->rdh.nRgnSize = 0;
		::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

		// Keep on hand highest and lowest values for the "transparent" pixels
		BYTE lr = GetRValue(cTransparentColor);
		BYTE lg = GetGValue(cTransparentColor);
		BYTE lb = GetBValue(cTransparentColor);
		BYTE hr = min(0xff, lr + GetRValue(cTolerance));
		BYTE hg = min(0xff, lg + GetGValue(cTolerance));
		BYTE hb = min(0xff, lb + GetBValue(cTolerance));

		// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
		BYTE *p32 = (BYTE *)bm.bmBits + (bm.bmHeight - 1) * bm.bmWidthBytes;
		for (int y = 0; y < bm.bmHeight; y++)
		{
			// Scan each bitmap pixel from left to right
			for (int x = 0; x < bm.bmWidth; x++)
			{
				// Search for a continuous range of "non transparent pixels"
				int x0 = x;
				LONG *p = (LONG *)p32 + x;
				while (x < bm.bmWidth)
				{
					BYTE b = GetRValue(*p);
					if (b >= lr && b <= hr)
					{
						b = GetGValue(*p);
						if (b >= lg && b <= hg)
						{
							b = GetBValue(*p);
							if (b >= lb && b <= hb)
								// This pixel is "transparent"
								break;
						}
					}
					p++;
					x++;
				}

				if (x > x0)
				{
					// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
					if (pData->rdh.nCount >= maxRects)
					{
						::GlobalUnlock(hData);
						maxRects += ALLOC_UNIT;
						hData = ::GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
						pData = (RGNDATA *)::GlobalLock(hData);
					}
					RECT *pr = (RECT *)&pData->Buffer;
					::SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
					if (x0 < pData->rdh.rcBound.left)
						pData->rdh.rcBound.left = x0;
					if (y < pData->rdh.rcBound.top)
						pData->rdh.rcBound.top = y;
					if (x > pData->rdh.rcBound.right)
						pData->rdh.rcBound.right = x;
					if (y+1 > pData->rdh.rcBound.bottom)
						pData->rdh.rcBound.bottom = y+1;
					pData->rdh.nCount++;

					// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
					// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
					if (pData->rdh.nCount == 2000)
					{
						HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
						if (hRgn)
						{
							::CombineRgn(hRgn, hRgn, h, RGN_OR);
							::DeleteObject(h);
						}
						else
							hRgn = h;
						pData->rdh.nCount = 0;
						::SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
					}
				}
			}

			// Go to next row (remember, the bitmap is inverted vertically)
			p32 -= bm.bmWidthBytes;
		}

		// Create or extend the region with the remaining rectangles
		HRGN h = ::ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
		if (hRgn)
		{
			::CombineRgn(hRgn, hRgn, h, RGN_OR);
			::DeleteObject(h);
		}
		else
			hRgn = h;

		// Clean up
		::GlobalUnlock(hData);
		::GlobalFree(hData);
	}
	ReleaseDC();
	return hRgn;
}

//透明绘画
bool CSkinImage::AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, COLORREF crTrans)
{
	return AlphaDrawImage(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTrans);
}

//透明绘画
bool CSkinImage::AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, COLORREF crTrans)
{
	//效验状态
	if (IsNull()) return false;

	//建立 DC
	CDC DCImage;
	DCImage.CreateCompatibleDC(pDestDC);
	DCImage.SelectObject(HBITMAP(*this));
	DCImage.SetBkColor(crTrans);
	DCImage.SetBkMode(TRANSPARENT);

	//建立掩码图
	CDC DCMask;
	CBitmap BMPMask;
	BMPMask.CreateBitmap(cxDest,cyDest,1,1,NULL);
	DCMask.CreateCompatibleDC(pDestDC);
	DCMask.SelectObject(&BMPMask);
	DCMask.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCCOPY);

	//绘画目标图
	CDC DCDest;
	CBitmap BMPDest;
	DCDest.CreateCompatibleDC(pDestDC);
	BMPDest.CreateCompatibleBitmap(pDestDC,cxDest,cyDest);
	DCDest.SelectObject(&BMPDest);
	DCDest.BitBlt(0,0,cxDest,cyDest,pDestDC,xDest,yDest,SRCCOPY);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCMask,0,0,SRCAND);
	DCDest.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCDest,0,0,SRCCOPY);

	//清理资源
	DCMask.DeleteDC();
	DCDest.DeleteDC();
	DCImage.DeleteDC();
	BMPMask.DeleteObject();
	BMPDest.DeleteObject();
	
	return true;
}

//透明绘画
bool CSkinImage::AlphaDrawImageEx(CDC * pDestDC, int xDest, int yDest, COLORREF crTrans)
{
	return AlphaDrawImageEx(pDestDC,xDest,yDest,GetWidth(),GetHeight(),0,0,crTrans);
}

//透明绘画
bool CSkinImage::AlphaDrawImageEx(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, COLORREF crTrans)
{
	//效验状态
	if (IsNull()) return false;

	//建立 DC
	CDC DCImage;
	DCImage.CreateCompatibleDC(pDestDC);
	DCImage.SelectObject(HBITMAP(*this));
	DCImage.SetBkColor(crTrans);
	DCImage.SetBkMode(TRANSPARENT);

	//建立掩码图
	CDC DCMask;
	CBitmap BMPMask;
	BMPMask.CreateBitmap(cxDest,cyDest,1,1,NULL);
	DCMask.CreateCompatibleDC(pDestDC);
	DCMask.SelectObject(&BMPMask);
	DCMask.BitBlt(0,0,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCCOPY);

	//绘画目标图
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,pDestDC,xDest,yDest,SRCCOPY);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCMask,0,0,SRCAND);
	pDestDC->BitBlt(xDest,yDest,cxDest,cyDest,&DCImage,xSrc,ySrc,SRCINVERT);

	//清理资源
	DCMask.DeleteDC();
	DCImage.DeleteDC();
	BMPMask.DeleteObject();
	
	return true;
}


//水平平铺
bool CSkinImage::DrawTileHorizontal(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, int cxSrc)
{
	int		dstl;
	int		dstw;

	for (dstl=xDest; dstl<xDest+cxDest; dstl += cxSrc)
	{
		if (dstl+cxSrc>xDest+cxDest)
			dstw = xDest+cxDest-dstl;
		else
			dstw = cxSrc;
		BitBlt(pDestDC->m_hDC, dstl, yDest, dstw, cyDest, xSrc, ySrc, SRCCOPY);
	}
	return true;
}

//垂直平铺
bool CSkinImage::DrawTileVertical(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, int cySrc)
{
	int		dstt;
	int		dsth;

	for (dstt=yDest; dstt<yDest+cyDest; dstt += cySrc)
	{
		if (dstt+cySrc>yDest+cyDest)
			dsth = yDest+cyDest-dstt;
		else
			dsth = cySrc;

		BitBlt(pDestDC->m_hDC, xDest, dstt, cxDest, dsth, xSrc, ySrc, SRCCOPY);
	}
	return true;
}

//混合平铺
bool CSkinImage::DrawTileBoth(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, int cxSrc, int cySrc)
{
	int		dstl, dstt;
	int		dsth, dstw;

	
	for (dstt=yDest; dstt<yDest+cyDest; dstt += cySrc)
	{
		if (dstt+cySrc < yDest+cyDest)
			dsth = cySrc;
		else 
		dsth = yDest+cyDest-dstt;

		for (dstl=xDest; dstl<xDest+cxDest; dstl += cxSrc)
		{
			if (dstl+cxSrc < xDest+cxDest)
				dstw = cxSrc;
			else
			dstw = xDest+cxDest-dstl;
			BitBlt(pDestDC->m_hDC, dstl, dstt, dstw, dsth, xSrc, ySrc, SRCCOPY);
		}
	}
	return true;
}

bool CSkinImage::DrawFrame(CDC *pDestDC, LPCRECT rcDest, LPCRECT rcSrc, LPCRECT rcSrcBorder, bool extend, int StretchBltMode)
{
	if (!extend)
	{
		DrawTileBoth(pDestDC,
			rcDest->left+rcSrcBorder->left, rcDest->top+rcSrcBorder->top, 
			(rcDest->right-rcDest->left)-(rcSrcBorder->left+rcSrcBorder->right),
			(rcDest->bottom-rcDest->top)-(rcSrcBorder->top+rcSrcBorder->bottom),
			rcSrc->left+rcSrcBorder->left,  rcSrc->top+rcSrcBorder->top,
			rcSrc->right-rcSrcBorder->right-rcSrcBorder->left,	rcSrc->bottom-rcSrcBorder->bottom-rcSrcBorder->top);

		//上下
		DrawTileHorizontal(pDestDC, 
			rcDest->left+rcSrcBorder->left, rcDest->top, (rcDest->right-rcDest->left)-(rcSrcBorder->left+rcSrcBorder->right), rcSrcBorder->top,
			rcSrc->left+rcSrcBorder->left, rcSrc->top, (rcSrc->right-rcSrc->left)-(rcSrcBorder->left+rcSrcBorder->right));

		DrawTileHorizontal(pDestDC, 
			rcDest->left+rcSrcBorder->left, rcDest->bottom-rcSrcBorder->bottom, (rcDest->right-rcDest->left)-(rcSrcBorder->left+rcSrcBorder->right), rcSrcBorder->bottom,
			rcSrc->left+rcSrcBorder->left, rcSrc->bottom-rcSrcBorder->bottom, (rcSrc->right-rcSrc->left)-(rcSrcBorder->left+rcSrcBorder->right));

		//左右
		DrawTileVertical(pDestDC, 
			rcDest->left, rcDest->top+rcSrcBorder->top, rcSrcBorder->left, (rcDest->bottom-rcDest->top)-(rcSrcBorder->bottom+rcSrcBorder->top),
			rcSrc->left, rcSrc->top+rcSrcBorder->top, (rcSrc->bottom-rcSrc->top)-(rcSrcBorder->bottom+rcSrcBorder->top));

		DrawTileVertical(pDestDC, 
			rcDest->right-rcSrcBorder->right, rcDest->top+rcSrcBorder->top, rcSrcBorder->right, (rcDest->bottom-rcDest->top)-(rcSrcBorder->bottom+rcSrcBorder->top),
			rcSrc->right-rcSrcBorder->right, rcSrc->top+rcSrcBorder->top, (rcSrc->bottom-rcSrc->top)-(rcSrcBorder->bottom+rcSrcBorder->top));
	}
	else
	{
		int		oldmode;
		CDC		srcdc;
		int		dstw, dsth;
		int		srcw, srch;
		int		borderw, borderh;

		srcdc.Attach(GetDC());
		oldmode=pDestDC->GetStretchBltMode();
		pDestDC->SetStretchBltMode(StretchBltMode);


		borderw = rcSrcBorder->right+rcSrcBorder->left;
		borderh = rcSrcBorder->top+rcSrcBorder->bottom;

		//back
		dstw = rcDest->right-rcDest->left - borderw;
		dsth = rcDest->bottom-rcDest->top - borderh;
		srcw = rcSrc->right-rcSrc->left-borderw;
		srch = rcSrc->bottom-rcSrc->top-borderh;

// 		pDestDC->StretchBlt(
// 			rcDest->left+rcSrcBorder->left, rcDest->top+rcSrcBorder->top, dstw, dsth, 
// 			&srcdc, rcSrc->left+rcSrcBorder->left, rcSrc->top+rcSrcBorder->top,	srcw, srch,
// 			SRCCOPY);

		pDestDC->StretchBlt(
		 	rcDest->left+rcSrcBorder->left, rcDest->top+rcSrcBorder->top, dstw, dsth, 
		 	&srcdc, rcSrc->left+rcSrcBorder->left, rcSrc->top+rcSrcBorder->top,	srcw, srch,
		 	SRCCOPY);

		//top, bottom
		pDestDC->StretchBlt(rcDest->left+rcSrcBorder->left, rcDest->top, dstw, rcSrcBorder->top, 
			&srcdc, rcSrc->left+rcSrcBorder->left, rcSrc->top, srcw, rcSrcBorder->top,
			SRCCOPY);

		pDestDC->StretchBlt(rcDest->left+rcSrcBorder->left, rcDest->bottom-rcSrcBorder->bottom, dstw, rcSrcBorder->bottom,
			&srcdc, rcSrc->left+rcSrcBorder->left, rcSrc->bottom-rcSrcBorder->bottom, srcw, rcSrcBorder->bottom,
			SRCCOPY);

		//left right
		pDestDC->StretchBlt(rcDest->left, rcDest->top+rcSrcBorder->top, rcSrcBorder->left, dsth, 
			&srcdc, rcSrc->left, rcSrc->top+rcSrcBorder->top, rcSrcBorder->left, srch, SRCCOPY);

		pDestDC->StretchBlt(rcDest->right-rcSrcBorder->right, rcDest->top+rcSrcBorder->top, rcSrcBorder->right, dsth, 
			&srcdc, rcSrc->right-rcSrcBorder->right, rcSrc->top+rcSrcBorder->top, rcSrcBorder->right, srch, SRCCOPY);

		pDestDC->SetStretchBltMode(oldmode);
		srcdc.Detach();
		ReleaseDC();
	}

	//四个角
	BitBlt(pDestDC->m_hDC, rcDest->left, rcDest->top, rcSrcBorder->left, rcSrcBorder->top, rcSrc->left, rcSrc->top, SRCCOPY);
	BitBlt(pDestDC->m_hDC, rcDest->left, rcDest->bottom-rcSrcBorder->bottom, rcSrcBorder->left, rcSrcBorder->bottom, rcSrc->left, rcSrc->bottom-rcSrcBorder->bottom, SRCCOPY);

	BitBlt(pDestDC->m_hDC, rcDest->right-rcSrcBorder->right, rcDest->top, rcSrcBorder->right, rcSrcBorder->top, rcSrc->right-rcSrcBorder->right, rcSrc->top, SRCCOPY);
	BitBlt(pDestDC->m_hDC, rcDest->right-rcSrcBorder->right, rcDest->bottom-rcSrcBorder->bottom,
		rcSrcBorder->right, rcSrcBorder->bottom, rcSrc->right-rcSrcBorder->right, rcSrc->bottom-rcSrcBorder->bottom, SRCCOPY);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////

//构造函数
CImageHandle::CImageHandle(CSkinImage * pImageObject)
{
	m_pImage=NULL;
	AttachResource(pImageObject);
	return;
}

//析构函数
CImageHandle::~CImageHandle()
{
	DetchResource();
	return;
}

//绑定资源
bool CImageHandle::AttachResource(IGDIResource * pImageObject)
{
	DetchResource();
	if (pImageObject)
	{
		CSkinImage * pSkinImage=(CSkinImage *)pImageObject;
		if (pImageObject->LockResource()>0)
		{
			m_pImage=pSkinImage;
			return true;
		}
	}
	return false;
}

//解除绑定
bool CImageHandle::DetchResource()
{
	if (m_pImage) 
	{
		m_pImage->UnLockResource();
		m_pImage=NULL;
		return true;
	} 
	return false;
}

//是否绑定
bool CImageHandle::IsAttached()
{
	return (m_pImage!=NULL);
}

//是否有效
bool CImageHandle::IsResourceValid()
{
	return ((m_pImage)&&(m_pImage->IsNull()==false));
}

//获取句柄
HBITMAP CImageHandle::GetBitmapHandle()
{
	if (IsResourceValid()==false) return NULL;
	return (HBITMAP)(*m_pImage);
}

/////////////////////////////////////////////////////////////////////////////////////////
