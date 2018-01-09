#pragma once
#include "D3DDevice.h"
#include "GameEngineHead.h"

/////////////////////////////////////////////////////////////////////////////////////////
//结构定义

typedef struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16        CombinedTransformationMatrix;

	//构造函数
	D3DXFRAME_DERIVED()
	{
		//设置变量
		pMeshContainer = NULL;
        pFrameSibling = NULL;
        pFrameFirstChild = NULL;

		//设置矩阵
		D3DXMatrixIdentity(&TransformationMatrix);
		D3DXMatrixIdentity(&CombinedTransformationMatrix);
	};

} *LPD3DXFRAME_DERIVED;

typedef struct D3DXMESHCONTAINER_DERIVED: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;        
	LPD3DXMESH           pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable;
	DWORD                NumAttributeGroups; 
	DWORD                NumInfl;
	LPD3DXBUFFER         pBoneCombinationBuf;
	D3DXMATRIX**         ppBoneMatrixPtrs;
	D3DXMATRIX*          pBoneOffsetMatrices;
	DWORD                NumPaletteEntries;
	bool                 UseSoftwareVP;
	DWORD                iAttributeSW;   

	//构造函数
	D3DXMESHCONTAINER_DERIVED()
	{
		//设置变量
		pMaterials = NULL;
		pEffects = NULL;
		pAdjacency = NULL;
		pSkinInfo = NULL;
		pNextMeshContainer = NULL;
		ppTextures = NULL;
		pOrigMesh = NULL;
		pAttributeTable = NULL;
		pBoneCombinationBuf = NULL;
		ppBoneMatrixPtrs = NULL;
		ppBoneMatrixPtrs = NULL;
		pBoneOffsetMatrices = NULL;	
	}

} *LPD3DXMESHCONTAINER_DERIVED;

/////////////////////////////////////////////////////////////////////////////////////////
//类型声明

class CD3DDynamicModel;

/////////////////////////////////////////////////////////////////////////////////////////


//分配层次
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
	//模型变量
public:
	CD3DDynamicModel*		          m_pSkinMesh;              //模型对象 

	//函数定义
public:
	//构造函数
	CAllocateHierarchy(CD3DDynamicModel *pSkinMesh) :m_pSkinMesh(pSkinMesh) {};

public:
	//创建框架
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	//创建容器
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);

	//销毁框架
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	//销毁容器
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);	
};

/////////////////////////////////////////////////////////////////////////////////////////

//动态模型
class GAME_ENGINE_CLASS CD3DDynamicModel  
{
	//变量定义
public:
	D3DCAPS9 					m_D3DCaps;				// 设备能力
	D3DXVECTOR3                 m_vModelCenter;			// 中心偏移
	FLOAT                       m_fModelRadius;			// 模型半径
	TCHAR						m_strFileDir[MAX_PATH];	// 文件目录
	CD3DDevice *				m_pD3DDevice;			// 设备指针
	LPDIRECT3DTEXTURE9			m_pBlendTexture;		// 混合纹理
	bool                        m_bByClone;             // 克隆标识  

	//动画变量
protected:
	BOOL						m_bMoving;				// 移动标识
	LPD3DXFRAME					m_pD3DFrame;			// 模型网格
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;		// 动画控制
	DOUBLE						m_lfTotalFrameTime;		// 播放时间

	//时间变量
protected:
	DOUBLE						m_dElapsedTime;			// 流逝时间
	DOUBLE						m_dCreateTime;			// 创建时间
	DOUBLE						m_dSetTime;				// 设置时间
	bool						m_bSetTimeKeep;			// 保持时间
	DOUBLE						m_dAnimationVelocity;	// 动画速度

	//模型材质
protected:
	BOOL						m_bStartMaterial;		// 启用材质
	D3DMATERIAL9				m_CustomMaterial;		// 自定材质

	//函数定义
public:
	//构造函数
	CD3DDynamicModel();
	//析构函数
	virtual ~CD3DDynamicModel();

	//控制函数
public:
	//动画开关
	VOID SetAnimation(BOOL bAnim);
	//设置关键值
	BOOL SetAnimationName(const char *strAnimName);
	//更新矩阵
	VOID UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

	//返回函数
public:
	//获取动画控制
	LPD3DXANIMATIONCONTROLLER GetAnimController() { return  m_pAnimController; };

	//功能函数
public:
	//载入模型
	HRESULT LoadModel(CD3DDevice * pD3DDevice, TCHAR* strFileName);
	//克隆模型
	bool CloneModel(CD3DDynamicModel & DynamicModel);
	
	//模型绘画
public:
	//正交绘画
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, D3DXMATRIXA16* pD3DXMatrixa, BYTE cbAlpha = 255 );
	//正交绘画
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest  );
	//正交绘画
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest , BYTE cbAlpha);
	//正交绘画
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest , FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian);
	//正交绘画
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest , FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian, BYTE cbAlpha);

	//模型绘画
public:
	//透视绘画
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, D3DXMATRIXA16* pD3DXMatrixa, BYTE cbAlpha = 255 );
	//透视绘画
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest );
	//透视绘画
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest, BYTE cbAlpha);
	//透视绘画
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest, FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian);
	//透视绘画
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest, FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian, BYTE cbAlpha);

	//绘画函数
protected:
	//绘画模型
	bool DrawModel( CD3DDevice * pD3DDevice, D3DXMATRIXA16* pD3DXMatrixa, BYTE cbAlpha = 255 );

	//功能函数
public:
	//绑定动画
	HRESULT GenerateSkinnedMesh(CD3DDevice * pD3DDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer);
	//设置时间
	void SetTime( FLOAT fTime, bool bKeep );
	//动画速度
	void AnimationVelocity( FLOAT fVelocity );
	//设置材质
	void SetCustomMaterial( D3DMATERIAL9* pMaterial );
	//设置材质
	void SetReflectionMaterial( LPDIRECT3DTEXTURE9 pTexture ) { m_pBlendTexture = pTexture; };

	//内部函数
protected:
	//绘画网格
	VOID DrawFrame( CD3DDevice * pD3DDevice, LPD3DXFRAME pFrame );
	//内部绘画
	VOID DrawMeshContainer(CD3DDevice * pD3DDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	//设置指针
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	//设置框架
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
};
