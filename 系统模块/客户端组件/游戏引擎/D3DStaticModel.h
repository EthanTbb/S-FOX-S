#ifndef  D3D_STATIC_MODEL_HEAD
#define  D3D_STATIC_MODEL_HEAD
#pragma once

//�����ļ�
#include "D3DDevice.h"
#include "D3DTexture.h"
#include "D3DEffectPool.h"
#include "GameEngineHead.h"

/////////////////////////////////////////////////////////////////////////////////////////
//��������
const D3DVERTEXELEMENT9 D3DVerTexelement[4] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, 
    D3DDECL_END()
};

/////////////////////////////////////////////////////////////////////////////////////////

//�������
struct CMeshMaterial
{
    ID3DXEffect *					m_pEffect;                 //Ч��ָ��
    D3DXHANDLE						m_hParamBlock;             //�������
    IDirect3DBaseTexture9 *			m_pTexture;                //����ָ��
	D3DMATERIAL9                    m_Materials;               //������Ϣ 

public:
	//���캯��
    CMeshMaterial()
    {
        m_pEffect = NULL;
        m_hParamBlock = NULL;
        m_pTexture = NULL;
		ZeroMemory(&m_Materials,sizeof(m_Materials));
    }

	//��������
    ~CMeshMaterial()
    {
		SafeRelease( m_pEffect );
		SafeRelease( m_pTexture );
    }

	//��������
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

//��̬ģ��
class  GAME_ENGINE_CLASS  CD3DStaticModel
{
	//�������
protected:
	TCHAR							m_wszMeshFile[MAX_PATH];      //�����ļ�   
	DWORD							m_dwNumVertices;              //������Ŀ
    DWORD							m_dwNumFaces;                 //��Ƭ��Ŀ
    DWORD							m_dwBytesPerVertex;           //�ֽ���Ŀ 
    
	//ָ�����
protected:
	ID3DXMesh  *					m_pMesh;                      //����ӿ� 
	IDirect3DVertexBuffer9* 		m_pVertexBuffer;              //���㻺��  
    IDirect3DIndexBuffer9*          m_pIndexBuffer;               //���л���
    IDirect3DVertexDeclaration9*    m_pVertexDecl;                //��������
	CD3DTexture *					m_pDefaultTexture;            //����ָ��

	//���Ʊ���
protected:
    bool		                    m_bUseMaterials;              //ʹ�ò���  

	//���ʱ���
protected:
	DWORD							m_dwNumMaterials;             //������Ŀ       
    CMeshMaterial *					m_pMaterials;				  //���ʽṹ

	//�ߴ���Ϣ
public:
	D3DXVECTOR3						m_vModelCenter;               //ģ������
	float							m_fModelRadius;               //ģ�Ͱ뾶

	//Ƕ�ױ���
protected:
	CD3DStaticModel *               m_pNestedModel;               //Ƕ��ģ��                   
	D3DXMATRIX *                    m_pNestedMatrix;              //Ƕ�׾���  

	//��������
public:
	//���캯��
	CD3DStaticModel();
	//���캯��
	CD3DStaticModel(const CD3DStaticModel & StaticModel );
	//��������
    ~CD3DStaticModel();

	//��������
public:
	//���ø�ʽ
	HRESULT SetMeshFVF( CD3DDevice * pD3DDevice, DWORD dwMeshFVF );
	//���ʿ���
	VOID  SetUseMeshMaterials( bool bFlag ) { m_bUseMaterials = bFlag; }
	//��������
	VOID  SetDefaultTexture(CD3DTexture *	pTexture);
	//Ƕ��ģ��
	VOID  SetNestedModel(CD3DStaticModel * pNestedModel) { m_pNestedModel = pNestedModel; }
	//Ƕ�׾���
	VOID  SetNestedMatrix(D3DXMATRIX * pNestedMatrix) { m_pNestedMatrix = pNestedMatrix; }
	//��������
	HRESULT SetVertexDecl( CD3DDevice * pD3DDevice, const D3DVERTEXELEMENT9 *pDecl,bool bComputeNormals=true,bool bComputeTangents=true,bool bSplitVertexForOptimalTangents = false );

	//���غ���
public:
	//��ֵ����
	const CD3DStaticModel &operator=( const CD3DStaticModel & rhs );

	//��������
public:
	//����ģ��
	HRESULT LoadModel( CD3DDevice * pD3DDevice ,LPCTSTR wszFileName,INT nEffectIndex=INVALID_EFFECT_INDEX);
	//��Ƕ����
	HRESULT GenerateEnhancedMesh( CD3DDevice * pD3DDevice, UINT dwNewNumSegs );
	//��ȡ����
    ID3DXMesh *GetMesh() { return m_pMesh; }
	//������Ŀ
	DWORD GetNumMaterials() const { return m_dwNumMaterials; }
	//����ģ��
	VOID  DestroyModel();	

	//��Ⱦ����
public:
	//��Ⱦ����
	HRESULT RenderModel( CD3DDevice * pd3dDevice );
	//��Ⱦ����
	HRESULT RenderModel( CD3DDevice * pd3dDevice ,bool bDrawOpaqueSubsets,bool bDrawAlphaSubsets );
};

/////////////////////////////////////////////////////////////////////////////////////////

#endif