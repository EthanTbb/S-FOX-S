#ifndef SKIN_IMAGE_HEAD_FILE
#define SKIN_IMAGE_HEAD_FILE

#pragma once

//#include "SkinControls.h"

//////////////////////////////////////////////////////////////////////////

//GDI 资源接口
interface IGDIResource
{
	//增加引用
	virtual int LockResource()=NULL;
	//减少引用
	virtual int UnLockResource()=NULL;
	//获取引用计数
	virtual int GetReferenceCount()=NULL;
};

//资源句柄接口
interface IGDIResourceHandle
{
	//是否绑定
	virtual bool IsAttached()=NULL;
	//是否有效
	virtual bool IsResourceValid()=NULL;
	//解除绑定
	virtual bool DetchResource()=NULL;
	//绑定资源
	virtual bool AttachResource(IGDIResource * pGDIObject)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//加载位图结构
struct tagImageLoadInfo
{
	//变量定义
	UINT								uResourceID;						//资源 ID
	CString								strFileName;						//文件名字
	HINSTANCE							hResourceDLL;						//资源句柄

	//函数定义
	tagImageLoadInfo()
	{
		uResourceID=0;
		hResourceDLL=NULL;
	}
};

//位图对象类
class  CSkinImage : public CImage, public IGDIResource
{
	friend class CImageHandle;

	//内部变量
private:
	int									m_nRefCount;						//引用计数
	bool								m_bLoadAlways;						//是否常在
	tagImageLoadInfo					m_LoadInfo;							//加载参数

	//函数定义
public:
	//构造函数
	CSkinImage(void);
	//析构函数
	virtual ~CSkinImage(void);

	//接口函数
protected:
	//增加引用
	virtual int LockResource();
	//减少引用
	virtual int UnLockResource();
	//获取引用计数
	virtual int GetReferenceCount() { return m_nRefCount; }

	//功能函数
public:
	//是否设置加载信息
	bool IsSetLoadInfo();
	//清除加载参数
	bool RemoveLoadInfo();
	//获取加载参数
	bool GetLoadInfo(tagImageLoadInfo & LoadInfo);
	//设置加载参数
	bool SetLoadInfo(LPCTSTR pszFileName, bool bLoadAlways=false);
	//设置加载参数
	bool SetLoadInfo(UINT uResourceID, HINSTANCE hResourceDLL=NULL, bool bLoadAlways=false);
	bool SetLoadInfo(LPCTSTR szResourceName, HINSTANCE hResourceDLL, bool bLoadAlways=false);

	//重载函数
public:
	//位图指针
	operator CBitmap * () { return CBitmap::FromHandle((HBITMAP)(*this)); }

	//绘图函数
public:
	//创建区域
	HRGN CreateImageRegion(COLORREF cTransparentColor, COLORREF cTolerance=0x0);
	//透明绘画
	bool AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, COLORREF crTrans);
	//透明绘画
	bool AlphaDrawImage(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, COLORREF crTrans);
	//透明绘画
	bool AlphaDrawImageEx(CDC * pDestDC, int xDest, int yDest, COLORREF crTrans);
	//透明绘画
	bool AlphaDrawImageEx(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, COLORREF crTrans);
	//水平平铺
	bool DrawTileHorizontal(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, int cxSrc);
	//垂直平铺
	bool DrawTileVertical(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, int cySrc);
	//混合平铺
	bool DrawTileBoth(CDC * pDestDC, int xDest, int yDest, int cxDest, int cyDest, int xSrc, int ySrc, int cxSrc, int cySrc);

	//绘画成一个框架类型 	rcSrcBorder是边框离边的距离 extend是不是拉伸处理 StretchBltMode拉伸的模式在extend=true有效
	bool DrawFrame(CDC *pDestDC, LPCRECT rcDest, LPCRECT rcSrc, LPCRECT rcSrcBorder, bool extend=false, int StretchBltMode=HALFTONE);

};

//////////////////////////////////////////////////////////////////////////

//位图资源句柄
class  CImageHandle : public IGDIResourceHandle
{
	//变量定义
protected:
	CSkinImage							* m_pImage;							//位图指针
	
	//函数定义
public:
	//构造函数
	CImageHandle(CSkinImage * pImageObject=NULL);
	//析构函数
	virtual ~CImageHandle();
	
	//接口函数
public:
	//是否绑定
	virtual bool IsAttached();
	//是否有效
	virtual bool IsResourceValid();
	//解除绑定
	virtual bool DetchResource();
	//绑定资源
	virtual bool AttachResource(IGDIResource * pImageObject);

	//操作重载
public:
	//获取句柄
	HBITMAP GetBitmapHandle();
	//获取对象
	CSkinImage * GetImageObject() { return m_pImage; };
	//指针重载
	CSkinImage * operator->() { return GetImageObject(); }
};

//////////////////////////////////////////////////////////////////////////

#endif