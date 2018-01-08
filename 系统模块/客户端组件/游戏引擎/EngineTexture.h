#ifndef ENGINE_TEXTURE_HEAD_FILE
#define ENGINE_TEXTURE_HEAD_FILE

#pragma once

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class CEngineTexture
{
	//ͼƬ��Ϣ
public:
	CSize							m_ImageSize;						//ͼƬ��С

	//��ת��Ϣ
protected:
	FLOAT							m_fAngle;							//��ת����
	FLOAT							m_fRotationX;						//��ת����
	FLOAT							m_fRotationY;						//��ת����

	//��������
public:
	//���캯��
	CEngineTexture();
	//��������
	virtual ~CEngineTexture();

	//ͼƬ��Ϣ
public:
	//�����ж�
	virtual bool IsNull()=NULL;
	//������
	virtual INT GetWidth() { return m_ImageSize.cx; }
	//����߶�
	virtual INT GetHeight() { return m_ImageSize.cy; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif