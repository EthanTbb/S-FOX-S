#ifndef D3D_EFFECT_POOL_HEAD_FILE
#define D3D_EFFECT_POOL_HEAD_FILE
#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//参数类型表
enum enParameterType
{
	enTypeValue=0,
	enTypeBool,
	enTypeInt,
	enTypeFloat,
	enTypeVector,
	enTypeMatrix,
	enTypeString,
	enTypeTexture,
	enTypeMatrixTranspose,
	enTypeVertexshader,
	enTypePixelshader,
	enTypeBoolArray,
	enTypeIntArray,
	enTypeFloatArray,
	enTypeVectorArray,
	enTypeMatrixArray,
	enTypeMatrixPointerArray,
	enTypeMatrixTrnasposeArray,
	enTypeMatrixTransposePointerArray
};

//////////////////////////////////////////////////////////////////////////////////
//常量定义
#define    INVALID_EFFECT_INDEX               0xffff                 //无效索引  

//////////////////////////////////////////////////////////////////////////////////

//效果池
class GAME_ENGINE_CLASS  CD3DEffectPool
{
	friend class CD3DDevice;

	//存储变量
private:
	CWHArray<ID3DXEffect *>                         m_EffectArray;                   //效果缓存

	//函数定义
protected:
	//构造函数
	CD3DEffectPool(void);

	//函数定义
public:
	//析构函数
	virtual ~CD3DEffectPool(void);

	//功能函数
public:
	//创建效果
	INT CreateEffectFromFile( IDirect3DDevice9 * pID3DDevice9, LPCTSTR pSrcFile, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXBUFFER *ppCompilationErrors );
	//创建效果
    INT CreateEffectFromResource( IDirect3DDevice9 * pID3DDevice9, HMODULE hSrcModule, LPCTSTR pSrcResource, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXBUFFER *ppCompilationErrors );

	//功能函数
public:
	//设置参数
	HRESULT SetParameter(INT nEffectIndex,enParameterType ParameterType,D3DXHANDLE hParameter,LPCVOID pInParam,INT ParamCount=NULL);
	//设置参数
	HRESULT GetParameter(INT nEffectIndex,enParameterType ParameterType,D3DXHANDLE hParameter,LPVOID pOutParam,INT ParamCount=NULL);
	//获取接口
	ID3DXEffect * GetID3DXEffect(INT nEffectIndex);

};

//////////////////////////////////////////////////////////////////////////////////

#endif