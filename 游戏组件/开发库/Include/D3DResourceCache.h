#ifndef  D3D_RESOURCE_CACHE_HEAD
#define  D3D_RESOURCE_CACHE_HEAD
#pragma once

//�����ļ�
#include "GameEngineHead.h"

/////////////////////////////////////////////////////////////////////////////////////////
//�ṹ����

enum D3DCACHE_SOURCELOCATION { D3DCACHE_LOCATION_FILE, D3DCACHE_LOCATION_RESOURCE };

//������
struct D3DCache_Texture
{
    D3DCACHE_SOURCELOCATION Location;
    TCHAR wszSource[MAX_PATH];
    HMODULE hSrcModule;
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    DWORD Usage;
    D3DFORMAT Format;
    D3DPOOL Pool;
    D3DRESOURCETYPE Type;
    IDirect3DBaseTexture9 *pTexture;
};

//Ч������
struct D3DCache_Effect
{
    D3DCACHE_SOURCELOCATION Location;
    TCHAR wszSource[MAX_PATH];
    HMODULE hSrcModule;
    DWORD dwFlags;
    ID3DXEffect *pEffect;
};

//���建��
struct D3DCache_Font : public D3DXFONT_DESC
{
    ID3DXFont *pFont;
};

/////////////////////////////////////////////////////////////////////////////////////////

//��Դ����
class CD3DResourceCache
{
	//�洢����
protected:
	CWHArray< D3DCache_Texture >				m_TextureCache;        //������
    CWHArray< D3DCache_Effect >					m_EffectCache;		   //Ч������	
    CWHArray< D3DCache_Font >					m_FontCache;           //���建��

	//��������
protected:
	//���캯��
    CD3DResourceCache() { };

	//��������
public:
	//��������
    ~CD3DResourceCache();

	//��Ԫ����
protected:
	//��ȡ����
    friend CD3DResourceCache & D3DGetGlobalResourceCache();

	//��������
public:
	//��ͨ����
    HRESULT CreateTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage,				D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette,			BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );
	//��ͨ����
    HRESULT CreateTextureFromFileDataEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT		MipLevels, DWORD Usage,	D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,PALETTEENTRY *pPalette,BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );
	//��ͨ����
    HRESULT CreateTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height, UINT			MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,			PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture );
	//��������
    HRESULT CreateCubeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT				Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette,					LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//��������
    HRESULT CreateCubeTextureFromFileDataEx( LPDIRECT3DDEVICE9 pDevice,  LPCTSTR pSrcFile, UINT Size, UINT MipLevels,		DWORD Usage, D3DFORMAT	Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette,LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//��������
    HRESULT CreateCubeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Size, UINT MipLevels,		DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,					PALETTEENTRY *pPalette, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//��������   
	HRESULT CreateVolumeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels,		 DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,					PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppTexture );
	//��������   
	HRESULT CreateVolumeTextureFromFileDataEx( LPDIRECT3DDEVICE9 pDevice,  LPCTSTR pSrcFile, UINT Width, UINT Height,		UINT Depth, UINT MipLevels,	DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppTexture );
	//��������
    HRESULT CreateVolumeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height,		UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey,					D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );

	//��������
public:
	//��ͨ����
    HRESULT CreateTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile,BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );	
	//��ͨ����
    HRESULT CreateTextureFromFileData( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile,BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );
	//��ͨ����
    HRESULT CreateTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DTEXTURE9 *ppTexture );
	//��������
    HRESULT CreateCubeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//��������
    HRESULT CreateCubeTextureFromFileData( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//��������
    HRESULT CreateCubeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//��������
    HRESULT CreateVolumeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
	//��������
    HRESULT CreateVolumeTextureFromFileData( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
	//��������
    HRESULT CreateVolumeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
	//��������
    HRESULT CreateFont( LPDIRECT3DDEVICE9 pDevice, UINT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCTSTR pFacename, LPD3DXFONT *ppFont );
	//��������
    HRESULT CreateFontIndirect( LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC *pDesc, LPD3DXFONT *ppFont );
	//����Ч��
    HRESULT CreateEffectFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors );
	//����Ч��
    HRESULT CreateEffectFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors );

	//�豸�ص�
public:
	//�豸����
    HRESULT OnCreateDevice( IDirect3DDevice9 *pd3dDevice );
	//�豸����
    HRESULT OnResetDevice( IDirect3DDevice9 *pd3dDevice );
	//�豸��ʧ
    HRESULT OnLostDevice();
	//�豸����
    HRESULT OnDestroyDevice();
};

/////////////////////////////////////////////////////////////////////////////////////////

//��Ԫ����
CD3DResourceCache & D3DGetGlobalResourceCache();

/////////////////////////////////////////////////////////////////////////////////////////

#endif