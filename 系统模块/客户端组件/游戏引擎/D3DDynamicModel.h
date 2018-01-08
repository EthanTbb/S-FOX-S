#pragma once
#include "D3DDevice.h"
#include "GameEngineHead.h"

/////////////////////////////////////////////////////////////////////////////////////////
//�ṹ����

typedef struct D3DXFRAME_DERIVED: public D3DXFRAME
{
	D3DXMATRIXA16        CombinedTransformationMatrix;

	//���캯��
	D3DXFRAME_DERIVED()
	{
		//���ñ���
		pMeshContainer = NULL;
        pFrameSibling = NULL;
        pFrameFirstChild = NULL;

		//���þ���
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

	//���캯��
	D3DXMESHCONTAINER_DERIVED()
	{
		//���ñ���
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
//��������

class CD3DDynamicModel;

/////////////////////////////////////////////////////////////////////////////////////////


//������
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
	//ģ�ͱ���
public:
	CD3DDynamicModel*		          m_pSkinMesh;              //ģ�Ͷ��� 

	//��������
public:
	//���캯��
	CAllocateHierarchy(CD3DDynamicModel *pSkinMesh) :m_pSkinMesh(pSkinMesh) {};

public:
	//�������
	STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	//��������
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name, 
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials, 
		CONST D3DXEFFECTINSTANCE *pEffectInstances, 
		DWORD NumMaterials, 
		CONST DWORD *pAdjacency, 
		LPD3DXSKININFO pSkinInfo, 
		LPD3DXMESHCONTAINER *ppNewMeshContainer);

	//���ٿ��
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
	//��������
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);	
};

/////////////////////////////////////////////////////////////////////////////////////////

//��̬ģ��
class GAME_ENGINE_CLASS CD3DDynamicModel  
{
	//��������
public:
	D3DCAPS9 					m_D3DCaps;				// �豸����
	D3DXVECTOR3                 m_vModelCenter;			// ����ƫ��
	FLOAT                       m_fModelRadius;			// ģ�Ͱ뾶
	TCHAR						m_strFileDir[MAX_PATH];	// �ļ�Ŀ¼
	CD3DDevice *				m_pD3DDevice;			// �豸ָ��
	LPDIRECT3DTEXTURE9			m_pBlendTexture;		// �������
	bool                        m_bByClone;             // ��¡��ʶ  

	//��������
protected:
	BOOL						m_bMoving;				// �ƶ���ʶ
	LPD3DXFRAME					m_pD3DFrame;			// ģ������
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;		// ��������
	DOUBLE						m_lfTotalFrameTime;		// ����ʱ��

	//ʱ�����
protected:
	DOUBLE						m_dElapsedTime;			// ����ʱ��
	DOUBLE						m_dCreateTime;			// ����ʱ��
	DOUBLE						m_dSetTime;				// ����ʱ��
	bool						m_bSetTimeKeep;			// ����ʱ��
	DOUBLE						m_dAnimationVelocity;	// �����ٶ�

	//ģ�Ͳ���
protected:
	BOOL						m_bStartMaterial;		// ���ò���
	D3DMATERIAL9				m_CustomMaterial;		// �Զ�����

	//��������
public:
	//���캯��
	CD3DDynamicModel();
	//��������
	virtual ~CD3DDynamicModel();

	//���ƺ���
public:
	//��������
	VOID SetAnimation(BOOL bAnim);
	//���ùؼ�ֵ
	BOOL SetAnimationName(const char *strAnimName);
	//���¾���
	VOID UpdateFrameMatrices( LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix );

	//���غ���
public:
	//��ȡ��������
	LPD3DXANIMATIONCONTROLLER GetAnimController() { return  m_pAnimController; };

	//���ܺ���
public:
	//����ģ��
	HRESULT LoadModel(CD3DDevice * pD3DDevice, TCHAR* strFileName);
	//��¡ģ��
	bool CloneModel(CD3DDynamicModel & DynamicModel);
	
	//ģ�ͻ滭
public:
	//�����滭
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, D3DXMATRIXA16* pD3DXMatrixa, BYTE cbAlpha = 255 );
	//�����滭
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest  );
	//�����滭
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest , BYTE cbAlpha);
	//�����滭
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest , FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian);
	//�����滭
	bool DrawOrthoModel( CD3DDevice * pD3DDevice, FLOAT fScaling, INT nXDest, INT nYDest, INT nZDest , FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian, BYTE cbAlpha);

	//ģ�ͻ滭
public:
	//͸�ӻ滭
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, D3DXMATRIXA16* pD3DXMatrixa, BYTE cbAlpha = 255 );
	//͸�ӻ滭
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest );
	//͸�ӻ滭
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest, BYTE cbAlpha);
	//͸�ӻ滭
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest, FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian);
	//͸�ӻ滭
	bool DrawPerspectiveModel( CD3DDevice * pD3DDevice, FLOAT fScaling, FLOAT fXDest, FLOAT fYDest, FLOAT fZDest, FLOAT fXRadian, FLOAT fYRadian, FLOAT fZRadian, BYTE cbAlpha);

	//�滭����
protected:
	//�滭ģ��
	bool DrawModel( CD3DDevice * pD3DDevice, D3DXMATRIXA16* pD3DXMatrixa, BYTE cbAlpha = 255 );

	//���ܺ���
public:
	//�󶨶���
	HRESULT GenerateSkinnedMesh(CD3DDevice * pD3DDevice, D3DXMESHCONTAINER_DERIVED *pMeshContainer);
	//����ʱ��
	void SetTime( FLOAT fTime, bool bKeep );
	//�����ٶ�
	void AnimationVelocity( FLOAT fVelocity );
	//���ò���
	void SetCustomMaterial( D3DMATERIAL9* pMaterial );
	//���ò���
	void SetReflectionMaterial( LPDIRECT3DTEXTURE9 pTexture ) { m_pBlendTexture = pTexture; };

	//�ڲ�����
protected:
	//�滭����
	VOID DrawFrame( CD3DDevice * pD3DDevice, LPD3DXFRAME pFrame );
	//�ڲ��滭
	VOID DrawMeshContainer(CD3DDevice * pD3DDevice, LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	//����ָ��
	HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase );
	//���ÿ��
	HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame );
};
