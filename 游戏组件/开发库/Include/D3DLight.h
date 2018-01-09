#pragma once

#include "GameEngineHead.h"

//光源
class GAME_ENGINE_CLASS CD3DLight			
{

public:
	DWORD					dwType;			//灯光类型

public:
	CD3DLight()
	{
		dwType = D3DLIGHT_FORCE_DWORD;
	}
};

//电光源
class GAME_ENGINE_CLASS CD3DPointLight : public CD3DLight
{

public:
	D3DCOLORVALUE			Diffuse;		//漫射光
	D3DCOLORVALUE			Specular;		//镜面光
	D3DCOLORVALUE			Ambient;		//环境光
	D3DVECTOR				Position;       //光源位置
	float					Range;          //光程
	float					Attenuation0;   //光常量
	float					Attenuation1;   //光线性
	float					Attenuation2;	//光2次距离衰减系数

public:
	CD3DPointLight()
	{
		dwType = D3DLIGHT_POINT;
	}
	~CD3DPointLight(){};
};

//方向光
class GAME_ENGINE_CLASS CD3DDirectionalLight : public CD3DLight
{

public:
	D3DCOLORVALUE			Diffuse;		//漫射光
	D3DCOLORVALUE			Specular;		//镜面光
	D3DCOLORVALUE			Ambient;		//环境光
	D3DVECTOR				Direction;      //方向

public:
	CD3DDirectionalLight()
	{
		dwType = D3DLIGHT_DIRECTIONAL;
	}
	~CD3DDirectionalLight(){};
};

//聚光灯
class GAME_ENGINE_CLASS CD3DSpotLight : public CD3DLight
{

public:
	D3DCOLORVALUE			Diffuse;		//漫射光
	D3DCOLORVALUE			Specular;		//镜面光
	D3DCOLORVALUE			Ambient;		//环境光
	D3DVECTOR				Position;       //光源位置
	D3DVECTOR				Direction;      //方向
	float					Range;          //光程
	float					Falloff;        //内锥到外锥衰减度
	float					Attenuation0;   //光常量
	float					Attenuation1;   //光线性
	float					Attenuation2;	//光2次距离衰减系数
	float					Theta;          //内锥角
	float					Phi;            //外锥角

public:
	CD3DSpotLight()
	{
		dwType = D3DLIGHT_SPOT;
	}
	~CD3DSpotLight(){};
};