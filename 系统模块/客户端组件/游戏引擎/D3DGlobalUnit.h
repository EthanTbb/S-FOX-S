#ifndef   D3D_GLOBAL_UNIT_HEAD
#define   D3D_GLOBAL_UNIT_HEAD
#pragma once

//包含文件
#include "D3DDevice.h"

/////////////////////////////////////////////////////////////////////////////////////////

//引擎辅助
class GAME_ENGINE_CLASS CD3DEngineAssist
{
	//函数定义
protected:
	//搜索路径
	static TCHAR *  MediaSearchPath();
	//查找文件
	static BOOL FindMediaSearchTypicalDirs( TCHAR* strSearchPath, INT nchSearchLen, LPCTSTR strLeaf, TCHAR* strExePath, TCHAR* strExeName );
	//查找文件
	static BOOL FindMediaSearchParentDirs( TCHAR* strSearchPath, INT nchSearchLen, TCHAR* strStartAt, TCHAR* strLeafName );

	//函数定义
public:
	//读取文件
	static UINT ReadMediaFileData(LPCWSTR wszFileName,LPBYTE & pcbFileData);
	//查找文件
	static HRESULT FindDXSDKMediaFileCch( TCHAR* strDestPath, INT nchLength, LPCTSTR strFilename );
	//设置路径
	static HRESULT SetMediaSearchPath( LPCTSTR strPath );
	//获取路径
	static LPCTSTR GetMediaSearchPath();	
};

/////////////////////////////////////////////////////////////////////////////////////////
#endif