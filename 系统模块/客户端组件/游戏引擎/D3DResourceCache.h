#ifndef  D3D_RESOURCE_CACHE_HEAD
#define  D3D_RESOURCE_CACHE_HEAD
#pragma once

//包含文件
#include "GameEngineHead.h"

/////////////////////////////////////////////////////////////////////////////////////////
//结构定义

enum D3DCACHE_SOURCELOCATION { D3DCACHE_LOCATION_FILE, D3DCACHE_LOCATION_RESOURCE };

//纹理缓冲
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

//效果缓冲
struct D3DCache_Effect
{
    D3DCACHE_SOURCELOCATION Location;
    TCHAR wszSource[MAX_PATH];
    HMODULE hSrcModule;
    DWORD dwFlags;
    ID3DXEffect *pEffect;
};

//字体缓冲
struct D3DCache_Font : public D3DXFONT_DESC
{
    ID3DXFont *pFont;
};

/////////////////////////////////////////////////////////////////////////////////////////

//资源缓存
class CD3DResourceCache
{
	//存储变量
protected:
	CWHArray< D3DCache_Texture >				m_TextureCache;        //纹理缓存
    CWHArray< D3DCache_Effect >					m_EffectCache;		   //效果缓存	
    CWHArray< D3DCache_Font >					m_FontCache;           //字体缓存

	//函数定义
protected:
	//构造函数
    CD3DResourceCache() { };

	//函数定义
public:
	//析构函数
    ~CD3DResourceCache();

	//友元函数
protected:
	//获取函数
    friend CD3DResourceCache & D3DGetGlobalResourceCache();

	//函数定义
public:
	//普通纹理
    HRESULT CreateTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage,				D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette,			BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );
	//普通纹理
    HRESULT CreateTextureFromFileDataEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT		MipLevels, DWORD Usage,	D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,PALETTEENTRY *pPalette,BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );
	//普通纹理
    HRESULT CreateTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height, UINT			MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,			PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture );
	//立体纹理
    HRESULT CreateCubeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT				Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette,					LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//立体纹理
    HRESULT CreateCubeTextureFromFileDataEx( LPDIRECT3DDEVICE9 pDevice,  LPCTSTR pSrcFile, UINT Size, UINT MipLevels,		DWORD Usage, D3DFORMAT	Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette,LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//立体纹理
    HRESULT CreateCubeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Size, UINT MipLevels,		DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,					PALETTEENTRY *pPalette, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//立体纹理   
	HRESULT CreateVolumeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels,		 DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,					PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppTexture );
	//立体纹理   
	HRESULT CreateVolumeTextureFromFileDataEx( LPDIRECT3DDEVICE9 pDevice,  LPCTSTR pSrcFile, UINT Width, UINT Height,		UINT Depth, UINT MipLevels,	DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo,PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppTexture );
	//立体纹理
    HRESULT CreateVolumeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height,		UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey,					D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );

	//函数定义
public:
	//普通纹理
    HRESULT CreateTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile,BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );	
	//普通纹理
    HRESULT CreateTextureFromFileData( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile,BOOL bCreateCache,LPDIRECT3DTEXTURE9 *ppTexture );
	//普通纹理
    HRESULT CreateTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DTEXTURE9 *ppTexture );
	//立体纹理
    HRESULT CreateCubeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//立体纹理
    HRESULT CreateCubeTextureFromFileData( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//立体纹理
    HRESULT CreateCubeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
	//立体纹理
    HRESULT CreateVolumeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
	//立体纹理
    HRESULT CreateVolumeTextureFromFileData( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
	//立体纹理
    HRESULT CreateVolumeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
	//创建字体
    HRESULT CreateFont( LPDIRECT3DDEVICE9 pDevice, UINT Height, UINT Width, UINT Weight, UINT MipLevels, BOOL Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCTSTR pFacename, LPD3DXFONT *ppFont );
	//创建字体
    HRESULT CreateFontIndirect( LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC *pDesc, LPD3DXFONT *ppFont );
	//创建效果
    HRESULT CreateEffectFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors );
	//创建效果
    HRESULT CreateEffectFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors );

	//设备回调
public:
	//设备创建
    HRESULT OnCreateDevice( IDirect3DDevice9 *pd3dDevice );
	//设备重置
    HRESULT OnResetDevice( IDirect3DDevice9 *pd3dDevice );
	//设备丢失
    HRESULT OnLostDevice();
	//设备销毁
    HRESULT OnDestroyDevice();
};

/////////////////////////////////////////////////////////////////////////////////////////

//友元声明
CD3DResourceCache & D3DGetGlobalResourceCache();

/////////////////////////////////////////////////////////////////////////////////////////

#endif