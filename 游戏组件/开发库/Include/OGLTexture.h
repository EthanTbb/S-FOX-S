#ifndef OGL_TEXTURE_HEAD_FILE
#define OGL_TEXTURE_HEAD_FILE

#pragma once

#include "OGLDevice.h"
#include "EngineTexture.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�������
class GAME_ENGINE_CLASS COGLTexture : public CEngineTexture
{
	//ͼƬ��Ϣ
public:
	CSize							m_ImageSize;						//ͼƬ��С
	CSize							m_MemorySize;						//�ڴ��С

	//�ں���Ϣ
protected:
	GLuint							m_nTextureID;						//�����ʶ

	//��������
public:
	//���캯��
	COGLTexture();
	//��������
	virtual ~COGLTexture();

	//ͼƬ��Ϣ
public:
	//�����ж�
	virtual bool IsNull();
	//������
	virtual INT GetWidth() { return m_ImageSize.cx; }
	//����߶�
	virtual INT GetHeight() { return m_ImageSize.cy; }

	//�������
public:
	//��������
	bool Destory();
	//��������
	bool LoadImage(COGLDevice * pOGLDevice, HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest);
	//�滭ͼƬ
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource);
	//�滭ͼƬ
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight);

	//�滭����
public:
	//�滭ͼƬ
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, BYTE cbAlpha);
	//�滭ͼƬ
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, BYTE cbAlpha);
	//�滭ͼƬ
	bool DrawImage(COGLDevice * pOGLDevice, INT nXDest, INT nYDest, INT nDestWidth, INT nDestHeight, INT nXSource, INT nYSource, INT nSourceWidth, INT nSourceHeight, BYTE cbAlpha);

	//�ڲ�����
protected:
	//��ȡ��Դ
	bool GetResourceInfo(HINSTANCE hInstance, LPCTSTR pszResource, LPCTSTR pszTypeName, tagResourceInfo & ResourceInfo);
};

//////////////////////////////////////////////////////////////////////////////////

#endif