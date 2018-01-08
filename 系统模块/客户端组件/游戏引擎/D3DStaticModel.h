#ifndef  D3D_STATIC_MODEL_HEAD
#define  D3D_STATIC_MODEL_HEAD
#pragma once

//包含文件
#include "D3DDevice.h"
#include "D3DTexture.h"
#include "D3DEffectPool.h"
#include "GameEngineHead.h"

/////////////////////////////////////////////////////////////////////////////////////////
//顶点声明
const D3DVERTEXELEMENT9 D3DVerTexelement[4] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, 
    D3DDECL_END()
};

/////////////////////////////////////////////////////////////////////////////////////////

//网格材质
struct CMeshMaterial
{
    ID3DXEffect *					m_pEffect;                 //效果指针
    D3DXHANDLE						m_hParamBlock;             //参数句柄
    IDirect3DBaseTexture9 *			m_pTexture;                //纹理指针
	D3DMATERIAL9                    m_Materials;               //材质信息 

public:
	//构造函数
    CMeshMaterial()
    {
        m_pEffect = NULL;
        m_hParamBlock = NULL;
        m_pTexture = NULL;
		ZeroMemory(&m_Materials,sizeof(m_Materials));
    }

	//析构函数
    ~CMeshMaterial()
    {
		SafeRelease( m_pEffect );
		SafeRelease( m_pTexture );
    }

	//操作重载
    const CMeshMaterial &operator=( const CMeshMaterial &rhs )
    {
        m_pEffect = rhs.m_pEffect;
        m_hParamBlock = rhs.m_hParamBlock;
        m_pTexture = rhs.m_pTexture;
		m_Materials = rhs.m_Materials;
        m_pEffect->AddRef();
        m_pTexture->AddRef();
        return *this;
    }
};

/////////////////////////////////////////////////////////////////////////////////////////

//静态模型
class  GAME_ENGINE_CLASS  CD3DStaticModel
{
	//网格变量
protected:
	TCHAR							m_wszMeshFile[MAX_PATH];      //网格文件   
	DWORD							m_dwNumVertices;              //顶点数目
    DWORD							m_dwNumFaces;                 //面片数目
    DWORD							m_dwBytesPerVertex;           //字节数目 
    
	//指针变量
protected:
	ID3DXMesh  *					m_pMesh;                      //网格接口 
	IDirect3DVertexBuffer9* 		m_pVertexBuffer;              //顶点缓冲  
    IDirect3DIndexBuffer9*          m_pIndexBuffer;               //所有缓冲
    IDirect3DVertexDeclaration9*    m_pVertexDecl;                //顶点声明
	CD3DTexture *					m_pDefaultTexture;            //纹理指针

	//控制变量
protected:
    bool		                    m_bUseMaterials;              //使用材质  

	//材质变量
protected:
	DWORD							m_dwNumMaterials;             //材质数目       
    CMeshMaterial *					m_pMaterials;				  //材质结构

	//尺寸信息
public:
	D3DXVECTOR3						m_vModelCenter;               //模型中心
	float							m_fModelRadius;               //模型半径

	//嵌套变量
protected:
	CD3DStaticModel *               m_pNestedModel;               //嵌套模型                   
	D3DXMATRIX *                    m_pNestedMatrix;              //嵌套矩阵  

	//函数定义
public:
	//构造函数
	CD3DStaticModel();
	//构造函数
	CD3DStaticModel(const CD3DStaticModel & StaticModel );
	//析构函数
    ~CD3DStaticModel();

	//设置属性
public:
	//设置格式
	HRESULT SetMeshFVF( CD3DDevice * pD3DDevice, DWORD dwMeshFVF );
	//材质控制
	VOID  SetUseMeshMaterials( bool bFlag ) { m_bUseMaterials = bFlag; }
	//设置纹理
	VOID  SetDefaultTexture(CD3DTexture *	pTexture);
	//嵌套模型
	VOID  SetNestedModel(CD3DStaticModel * pNestedModel) { m_pNestedModel = pNestedModel; }
	//嵌套矩阵
	VOID  SetNestedMatrix(D3DXMATRIX * pNestedMatrix) { m_pNestedMatrix = pNestedMatrix; }
	//顶点声明
	HRESULT SetVertexDecl( CD3DDevice * pD3DDevice, const D3DVERTEXELEMENT9 *pDecl,bool bComputeNormals=true,bool bComputeTangents=true,bool bSplitVertexForOptimalTangents = false );

	//重载函数
public:
	//赋值重载
	const CD3DStaticModel &operator=( const CD3DStaticModel & rhs );

	//辅助函数
public:
	//加载模型
	HRESULT LoadModel( CD3DDevice * pD3DDevice ,LPCTSTR wszFileName,INT nEffectIndex=INVALID_EFFECT_INDEX);
	//镶嵌网格
	HRESULT GenerateEnhancedMesh( CD3DDevice * pD3DDevice, UINT dwNewNumSegs );
	//获取网格
    ID3DXMesh *GetMesh() { return m_pMesh; }
	//材质数目
	DWORD GetNumMaterials() const { return m_dwNumMaterials; }
	//销毁模型
	VOID  DestroyModel();	

	//渲染函数
public:
	//渲染网格
	HRESULT RenderModel( CD3DDevice * pd3dDevice );
	//渲染网格
	HRESULT RenderModel( CD3DDevice * pd3dDevice ,bool bDrawOpaqueSubsets,bool bDrawAlphaSubsets );
};

/////////////////////////////////////////////////////////////////////////////////////////

#endif