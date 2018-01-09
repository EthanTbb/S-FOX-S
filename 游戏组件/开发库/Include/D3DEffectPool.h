#ifndef D3D_EFFECT_POOL_HEAD_FILE
#define D3D_EFFECT_POOL_HEAD_FILE
#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////
//�������ͱ�
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
//��������
#define    INVALID_EFFECT_INDEX               0xffff                 //��Ч����  

//////////////////////////////////////////////////////////////////////////////////

//Ч����
class GAME_ENGINE_CLASS  CD3DEffectPool
{
	friend class CD3DDevice;

	//�洢����
private:
	CWHArray<ID3DXEffect *>                         m_EffectArray;                   //Ч������

	//��������
protected:
	//���캯��
	CD3DEffectPool(void);

	//��������
public:
	//��������
	virtual ~CD3DEffectPool(void);

	//���ܺ���
public:
	//����Ч��
	INT CreateEffectFromFile( IDirect3DDevice9 * pID3DDevice9, LPCTSTR pSrcFile, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXBUFFER *ppCompilationErrors );
	//����Ч��
    INT CreateEffectFromResource( IDirect3DDevice9 * pID3DDevice9, HMODULE hSrcModule, LPCTSTR pSrcResource, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXBUFFER *ppCompilationErrors );

	//���ܺ���
public:
	//���ò���
	HRESULT SetParameter(INT nEffectIndex,enParameterType ParameterType,D3DXHANDLE hParameter,LPCVOID pInParam,INT ParamCount=NULL);
	//���ò���
	HRESULT GetParameter(INT nEffectIndex,enParameterType ParameterType,D3DXHANDLE hParameter,LPVOID pOutParam,INT ParamCount=NULL);
	//��ȡ�ӿ�
	ID3DXEffect * GetID3DXEffect(INT nEffectIndex);

};

//////////////////////////////////////////////////////////////////////////////////

#endif