#pragma once
#include "D3DDevice.h"
#include "GameEngineHead.h"
#include "D3DTexture.h"

#define MAX_PARTICLES						1000 

struct	tgaParticle							// �����������ݽṹ
{
	bool			bActive;				// �Ƿ񼤻�
	float			fLife;					// ��������
	float			fFade;					// ˥���ٶ�
	D3DCOLORVALUE	Colour;					// ��ɫ
	D3DVECTOR		vecSite;				// λ��
	D3DVECTOR		vecAngle;				// �Ƕ�
	D3DVECTOR		vecSpeed;				// �ٶ�
	D3DVECTOR		vecAcceleration;		// ���ٶ�
};

class GAME_ENGINE_CLASS CD3DParticles
{
	//����Դ
public:
	tgaParticle						m_Particle[MAX_PARTICLES];

	//��Դ����
public:
	CD3DTexture						m_D3DTexture;	

public:
	CD3DParticles(void);
	~CD3DParticles(void);

public:
	//��ʼ��
	bool OnCreate( CD3DDevice * pD3DDevice );

	//�滭
	bool DrawParticles( CD3DDevice * pD3DDevice, INT nWidth, INT nHeight );

	//�滭
	bool DrawParticles( CD3DDevice * pD3DDevice );


};
