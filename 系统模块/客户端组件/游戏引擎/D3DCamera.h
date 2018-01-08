#pragma once

#include "GameEngineHead.h"

class GAME_ENGINE_CLASS CD3DCamera
{
	//信息变量
protected:
	IDirect3DDevice9*				m_pIDirect3DDevice;			//设备对象				

	//观察变量
public:
	FLOAT							m_fViewDistance;			//观察距离
	D3DXVECTOR3						m_vEye;						//摄象机的位置
	D3DXVECTOR3						m_vLookat;					//摄象机目标的位置
	D3DXVECTOR3						m_vUp;						//摄象机的正上方
	D3DXMATRIX						m_CurrentView;				//当前认观察矩阵

	//默认矩阵
public:
	D3DXMATRIX						m_DefaultView;				//默认观察矩阵

	//构造函数
public:
	CD3DCamera(void);
	~CD3DCamera(void);


	//控制函数
public:
	//创建摄像头
	void CreateCamera( IDirect3DDevice9* pIDirect3DDevice );
	//移动摄像头
	void MoveCamera();

	//获取函数
public:
	//获取当前观察点
	D3DXMATRIX* GetCurrentView() { return &m_CurrentView; };
	//获取默认观察点
	D3DXMATRIX* GetDefaultView() { return &m_DefaultView; };
	//获取眼睛
	D3DXVECTOR3 GetEye() { return m_vEye; };
	//获取观察点
	D3DXVECTOR3 GetLookat() { return m_vLookat; };
	//获取正方向
	D3DXVECTOR3 GetUp() { return m_vUp; };

	//设置函数
public:
	//设置眼睛
	void SetEye( D3DXVECTOR3 vEye ) { m_vEye = vEye; };
	//设置观察点
	void SetLookat( D3DXVECTOR3 vLookat ) { m_vLookat = vLookat; };
	//设置正方向
	void SetUp( D3DXVECTOR3 vUp ) { m_vUp = vUp; };

	//控制函数
public:
	//摄像机X自转
	void EyeRotationX( float fRadian );
	//摄像机Y自转
	void EyeRotationY( float fRadian );
	//摄像机X观察点转
	void LookatRotationX( float fRadian );
	//摄像机Y观察点转
	void LookatRotationY( float fRadian );
	//左右移动
	void MoveLeftRight( float fRadian );
	//上下移动
	void MoveTopBottom( float fRadian );
	//前后移动
	void MoveFrontBack( float fRadian );


	//更新函数
public:
	//更新摄像机
	void UpdateCamera();
};
