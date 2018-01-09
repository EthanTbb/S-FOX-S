#pragma once
#include "D3DDevice.h"
#include "GameEngineHead.h"
#include "D3DTexture.h"

#define MAX_PARTICLES						1000 

struct	tgaParticle							// 创建粒子数据结构
{
	bool			bActive;				// 是否激活
	float			fLife;					// 粒子生命
	float			fFade;					// 衰减速度
	D3DCOLORVALUE	Colour;					// 颜色
	D3DVECTOR		vecSite;				// 位置
	D3DVECTOR		vecAngle;				// 角度
	D3DVECTOR		vecSpeed;				// 速度
	D3DVECTOR		vecAcceleration;		// 加速度
};

class GAME_ENGINE_CLASS CD3DParticles
{
	//点资源
public:
	tgaParticle						m_Particle[MAX_PARTICLES];

	//资源变量
public:
	CD3DTexture						m_D3DTexture;	

public:
	CD3DParticles(void);
	~CD3DParticles(void);

public:
	//初始化
	bool OnCreate( CD3DDevice * pD3DDevice );

	//绘画
	bool DrawParticles( CD3DDevice * pD3DDevice, INT nWidth, INT nHeight );

	//绘画
	bool DrawParticles( CD3DDevice * pD3DDevice );


};
