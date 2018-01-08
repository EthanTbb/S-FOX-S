#ifndef ENGINE_TEXTURE_HEAD_FILE
#define ENGINE_TEXTURE_HEAD_FILE

#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//引擎纹理
class CEngineTexture
{
	//图片信息
public:
	CSize							m_ImageSize;						//图片大小

	//旋转信息
protected:
	FLOAT							m_fAngle;							//旋转变量
	FLOAT							m_fRotationX;						//旋转变量
	FLOAT							m_fRotationY;						//旋转变量

	//函数定义
public:
	//构造函数
	CEngineTexture();
	//析构函数
	virtual ~CEngineTexture();

	//图片信息
public:
	//加载判断
	virtual bool IsNull()=NULL;
	//纹理宽度
	virtual INT GetWidth() { return m_ImageSize.cx; }
	//纹理高度
	virtual INT GetHeight() { return m_ImageSize.cy; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif