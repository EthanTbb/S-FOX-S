#ifndef OGL_TEXTURE_HEAD_FILE
#define OGL_TEXTURE_HEAD_FILE

#pragma once

#include "OGLDevice.h"
#include "EngineTexture.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//纹理对象
class GAME_ENGINE_CLASS COGLTexture : public CEngineTexture
{
	//图片信息
public:
	CSize							m_ImageSize;						//图片大小
	CSize							m_MemorySize;						//内存大小

	//内核信息
protected:
	GLuint							m_nTextureID;						//纹理标识

	//函数定义
public:
	//构造函数
	COGLTexture();
	//析构函数
	virtual ~COGLTexture();

	//图片信息
public:
	//加载判断
	virtual bool IsNull();
	//纹理宽度
	virtual INT GetWidth() { return m_ImageSize.cx; }
	//纹理高度
	virtual INT GetHeight() { return m_ImageSize.cy; }

	//纹理管理
public:
	//销毁纹理
	bool Destory();
	//加载纹理
	bool LoadImage(COGLDevice * pOGLDevice, HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName);

	//绘画函数
public:
	//绘画图片
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest);
	//绘画图片
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//绘画图片
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);

	//绘画函数
public:
	//绘画图片
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, BYTE cbAlpha);
	//绘画图片
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, BYTE cbAlpha);
	//绘画图片
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight, BYTE cbAlpha);

	//内部函数
protected:
	//获取资源
	bool GetResourceInfo(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, tagResourceInfo & ResourceInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif