#ifndef   D3D_GLOBAL_UNIT_HEAD
#define   D3D_GLOBAL_UNIT_HEAD
#pragma once

//�����ļ�
#include "D3DDevice.h"

/////////////////////////////////////////////////////////////////////////////////////////

//���渨��
class GAME_ENGINE_CLASS CD3DEngineAssist
{
	//��������
protected:
	//����·��
	static TCHAR *  MediaSearchPath();
	//�����ļ�
	static BOOL FindMediaSearchTypicalDirs( TCHAR* strSearchPath, INT nchSearchLen, LPCTSTR strLeaf, TCHAR* strExePath, TCHAR* strExeName );
	//�����ļ�
	static BOOL FindMediaSearchParentDirs( TCHAR* strSearchPath, INT nchSearchLen, TCHAR* strStartAt, TCHAR* strLeafName );

	//��������
public:
	//��ȡ�ļ�
	static UINT ReadMediaFileData(LPCWSTR wszFileName,LPBYTE & pcbFileData);
	//�����ļ�
	static HRESULT FindDXSDKMediaFileCch( TCHAR* strDestPath, INT nchLength, LPCTSTR strFilename );
	//����·��
	static HRESULT SetMediaSearchPath( LPCTSTR strPath );
	//��ȡ·��
	static LPCTSTR GetMediaSearchPath();	
};

/////////////////////////////////////////////////////////////////////////////////////////
#endif