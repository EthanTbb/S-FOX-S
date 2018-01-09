#pragma once

#include "GameEngineHead.h"

//��Դ
class GAME_ENGINE_CLASS CD3DLight			
{

public:
	DWORD					dwType;			//�ƹ�����

public:
	CD3DLight()
	{
		dwType = D3DLIGHT_FORCE_DWORD;
	}
};

//���Դ
class GAME_ENGINE_CLASS CD3DPointLight : public CD3DLight
{

public:
	D3DCOLORVALUE			Diffuse;		//�����
	D3DCOLORVALUE			Specular;		//�����
	D3DCOLORVALUE			Ambient;		//������
	D3DVECTOR				Position;       //��Դλ��
	float					Range;          //���
	float					Attenuation0;   //�ⳣ��
	float					Attenuation1;   //������
	float					Attenuation2;	//��2�ξ���˥��ϵ��

public:
	CD3DPointLight()
	{
		dwType = D3DLIGHT_POINT;
	}
	~CD3DPointLight(){};
};

//�����
class GAME_ENGINE_CLASS CD3DDirectionalLight : public CD3DLight
{

public:
	D3DCOLORVALUE			Diffuse;		//�����
	D3DCOLORVALUE			Specular;		//�����
	D3DCOLORVALUE			Ambient;		//������
	D3DVECTOR				Direction;      //����

public:
	CD3DDirectionalLight()
	{
		dwType = D3DLIGHT_DIRECTIONAL;
	}
	~CD3DDirectionalLight(){};
};

//�۹��
class GAME_ENGINE_CLASS CD3DSpotLight : public CD3DLight
{

public:
	D3DCOLORVALUE			Diffuse;		//�����
	D3DCOLORVALUE			Specular;		//�����
	D3DCOLORVALUE			Ambient;		//������
	D3DVECTOR				Position;       //��Դλ��
	D3DVECTOR				Direction;      //����
	float					Range;          //���
	float					Falloff;        //��׶����׶˥����
	float					Attenuation0;   //�ⳣ��
	float					Attenuation1;   //������
	float					Attenuation2;	//��2�ξ���˥��ϵ��
	float					Theta;          //��׶��
	float					Phi;            //��׶��

public:
	CD3DSpotLight()
	{
		dwType = D3DLIGHT_SPOT;
	}
	~CD3DSpotLight(){};
};