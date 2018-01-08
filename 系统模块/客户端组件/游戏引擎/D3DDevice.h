#ifndef D3D_DEVICE_HEAD_FILE
#define D3D_DEVICE_HEAD_FILE

#include "D3DCamera.h"
#include "D3DLight.h"
#include "D3DEffectPool.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CD3DDirect;
class CD3DDevice;
class CD3DTexture;

//////////////////////////////////////////////////////////////////////////////////

//�豸�ӿ�
interface ID3DDeviceSink
{
	//�豸�¼�
public:
	//�����豸
	virtual VOID OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//��ʧ�豸
	virtual VOID OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//�����豸
	virtual VOID OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//��Ⱦ����
	virtual VOID OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//�豸����
class GAME_ENGINE_CLASS CD3DDevice
{
	//��������
public:
	HWND 							m_hWndDevice;						//�豸����
	CSize							m_SizeWindow;						//���ڴ�С

	//���ñ���
protected:
	D3DCOLOR						m_crBackColor;						//������ɫ
	bool							m_bMultiThreaded;					//���߳�ֵ
	bool							m_bEnableMultiSample;				//������ʶ	

	//�ӿڱ���
protected:
	ID3DDeviceSink *				m_pID3DDeviceSink;					//�ص��ӿ�
	IDirect3DDevice9 *				m_pIDirect3DDevice;					//�豸����

	//���λ滭
protected:
	IDirect3DVertexBuffer9 *		m_pIFillRectBuffer;					//�����ζ�������
	IDirect3DVertexBuffer9 *		m_pIBoxRectBuffer;					//���ο򶥵�����
	IDirect3DVertexBuffer9 *		m_pILineBuffer;						//ֱ�߶�������

	//Ĭ����Ϣ
protected:
	IDirect3DTexture9*				m_pDefaultTex;						//Ĭ������
	D3DMATERIAL9					m_DefaultMaterial;					//Ĭ�ϲ���
	D3DLIGHT9						m_DefaultLight;						//Ĭ�Ϲ�Դ

	//ͶӰ����
protected:
	D3DXMATRIX						m_MatrixOrtho;						//��������
	D3DXMATRIX						m_MatrixPerspective;				//͸�Ӿ���

	//�����
protected:
	CD3DCamera						m_D3DCamera;						//�����

	//Ч����
protected:
	CD3DEffectPool                  m_D3DEffectPool;                    //Ч����

	//��������
public:
	//���캯��
	CD3DDevice();
	//��������
	virtual ~CD3DDevice();

	//��Ϣ����
public:
	//��ȡ����
	HWND GetWndDevice() { return m_hWndDevice; }
	//��ȡ��С
	CSize GetSizeWindow() { return m_SizeWindow; }

	//���ýӿ�
public:
	//����߳�
	VOID SetMultiThreaded(bool bMultiThreaded);
	//���ò���
	VOID SetEnableMultiSample(bool bEnableMultiSample) { m_bEnableMultiSample=bEnableMultiSample; }
	//������ɫ
	VOID SetBackColor(D3DCOLOR crBackColor) { m_crBackColor=crBackColor; }

	//��ȡ�ӿ�
public:
	//�ص��ӿ�
	ID3DDeviceSink * GetD3DDeviceSink() { return m_pID3DDeviceSink; }
	//�豸�ӿ�
	IDirect3DDevice9 * GetDirect3DDevice() { return m_pIDirect3DDevice; }

	//��ȡĬ����Ϣ
public:
	//��ȡĬ������
	IDirect3DTexture9*	GetDefaultTex() { return m_pDefaultTex; };
	//��ȡĬ�ϲ���
	D3DMATERIAL9*	GetDefaultMaterial() { return &m_DefaultMaterial; };

	//��ȡ����ͷ��Ϣ
public:
	//��ȡ�����
	CD3DCamera* GetD3DCamera() { return &m_D3DCamera; };
	//��ȡ��ǰ�۲��
	D3DXMATRIX* GetCurrentView() { return m_D3DCamera.GetCurrentView(); };
	//��ȡĬ�Ϲ۲��
	D3DXMATRIX* GetDefaultView() { return m_D3DCamera.GetDefaultView(); };

	//Ч������Ϣ
public:
	//��ȡЧ����
	CD3DEffectPool * GetD3DEffectPool() { return &m_D3DEffectPool; };

	//��ȡͶӰ����
public:
	//��ȡ����
	D3DXMATRIX* GetMatrixOrtho() { return &m_MatrixOrtho; };
	//��ȡ͸��
	D3DXMATRIX* GetMatrixPerspective() { return &m_MatrixPerspective; };

	//�ƹ�����
public:
	//���õƹ�
	void SetLight( DWORD Index,CONST CD3DLight* pD3DLight );
	//����ƹ�
	void LightEnable( DWORD Index, BOOL Enable );

	//���ƺ���
public:
	//�����豸
	bool ResetD3DDevice();
	//��Ⱦ�豸
	bool RenderD3DDevice();
	//�����豸
	bool CreateD3DDevice(HWND hWndDevice, ID3DDeviceSink * pID3DDeviceSink);

	//��Ⱦ״̬
public:
	//��Ⱦ����
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	//��Ⱦ����
	HRESULT GetRenderState(D3DRENDERSTATETYPE State, DWORD & Value);

	//����ͶӰ
public:
	//��������ͶӰ
	void SetOrthoProjection( INT nWidth, INT nHeight, FLOAT fNear, FLOAT fFar );
	//����͸��ͶӰ
	void SetPerspectiveProjection( FLOAT fovy, FLOAT Aspect, FLOAT fNear, FLOAT fFar );

	//���ù۲��
public:
	//���ù۲��
	void SetViewpoint( D3DXVECTOR3 vEye, D3DXVECTOR3 Lookat );
	
	//��亯��
public:
	//�����ɫ
	VOID FillRect(CRect rcRect, D3DCOLOR D3DColor);
	//�����ɫ
	VOID FillRect(INT nXPos, INT nYPos, INT nWidth, INT nHeigth, D3DCOLOR D3DColor);

	//�滭����
public:
	//�滭����
	VOID DrawRect(CRect rcRect, D3DCOLOR D3DColor);
	//�滭����
	VOID DrawLine(INT nXPos1, INT nYPos1, INT nXPos2, INT nYPos2, D3DCOLOR D3DColor);

	//���ܺ���
public:
	//��Ϣ��ԭ
	static void InfoReduction(BYTE* pImageData, UINT& lFileLength);
	//ͼƬ����
	static void DecryptionOld( BYTE* pDataDec, BYTE* pHead, UINT& uSize );
	//ͼƬ����
	static void DecryptionNew( BYTE* pDataDec, BYTE* pHead, UINT& uSize );

	//�ڲ�����
private:
	//���û���
	VOID InitD3DDevice();
	//��������
	VOID InitPresentParameters(D3DPRESENT_PARAMETERS & PresentParameters, HWND hWnd, INT nWidth, INT nHeight, D3DFORMAT BufferFormat);
};

//////////////////////////////////////////////////////////////////////////////////

#endif