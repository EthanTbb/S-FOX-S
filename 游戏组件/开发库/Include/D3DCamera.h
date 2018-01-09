#pragma once

#include "GameEngineHead.h"

class GAME_ENGINE_CLASS CD3DCamera
{
	//��Ϣ����
protected:
	IDirect3DDevice9*				m_pIDirect3DDevice;			//�豸����				

	//�۲����
public:
	FLOAT							m_fViewDistance;			//�۲����
	D3DXVECTOR3						m_vEye;						//�������λ��
	D3DXVECTOR3						m_vLookat;					//�����Ŀ���λ��
	D3DXVECTOR3						m_vUp;						//����������Ϸ�
	D3DXMATRIX						m_CurrentView;				//��ǰ�Ϲ۲����

	//Ĭ�Ͼ���
public:
	D3DXMATRIX						m_DefaultView;				//Ĭ�Ϲ۲����

	//���캯��
public:
	CD3DCamera(void);
	~CD3DCamera(void);


	//���ƺ���
public:
	//��������ͷ
	void CreateCamera( IDirect3DDevice9* pIDirect3DDevice );
	//�ƶ�����ͷ
	void MoveCamera();

	//��ȡ����
public:
	//��ȡ��ǰ�۲��
	D3DXMATRIX* GetCurrentView() { return &m_CurrentView; };
	//��ȡĬ�Ϲ۲��
	D3DXMATRIX* GetDefaultView() { return &m_DefaultView; };
	//��ȡ�۾�
	D3DXVECTOR3 GetEye() { return m_vEye; };
	//��ȡ�۲��
	D3DXVECTOR3 GetLookat() { return m_vLookat; };
	//��ȡ������
	D3DXVECTOR3 GetUp() { return m_vUp; };

	//���ú���
public:
	//�����۾�
	void SetEye( D3DXVECTOR3 vEye ) { m_vEye = vEye; };
	//���ù۲��
	void SetLookat( D3DXVECTOR3 vLookat ) { m_vLookat = vLookat; };
	//����������
	void SetUp( D3DXVECTOR3 vUp ) { m_vUp = vUp; };

	//���ƺ���
public:
	//�����X��ת
	void EyeRotationX( float fRadian );
	//�����Y��ת
	void EyeRotationY( float fRadian );
	//�����X�۲��ת
	void LookatRotationX( float fRadian );
	//�����Y�۲��ת
	void LookatRotationY( float fRadian );
	//�����ƶ�
	void MoveLeftRight( float fRadian );
	//�����ƶ�
	void MoveTopBottom( float fRadian );
	//ǰ���ƶ�
	void MoveFrontBack( float fRadian );


	//���º���
public:
	//���������
	void UpdateCamera();
};
