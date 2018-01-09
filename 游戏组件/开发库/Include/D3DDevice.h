#ifndef D3D_DEVICE_HEAD_FILE
#define D3D_DEVICE_HEAD_FILE

#include "D3DCamera.h"
#include "D3DLight.h"
#include "D3DEffectPool.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class CD3DDirect;
class CD3DDevice;
class CD3DTexture;

//////////////////////////////////////////////////////////////////////////////////

//设备接口
interface ID3DDeviceSink
{
	//设备事件
public:
	//配置设备
	virtual VOID OnInitDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//丢失设备
	virtual VOID OnLostDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//重置设备
	virtual VOID OnResetDevice(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
	//渲染窗口
	virtual VOID OnRenderWindow(CD3DDevice * pD3DDevice, INT nWidth, INT nHeight)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//设备对象
class GAME_ENGINE_CLASS CD3DDevice
{
	//窗口属性
public:
	HWND 							m_hWndDevice;						//设备窗口
	CSize							m_SizeWindow;						//窗口大小

	//配置变量
protected:
	D3DCOLOR						m_crBackColor;						//背景颜色
	bool							m_bMultiThreaded;					//多线程值
	bool							m_bEnableMultiSample;				//采样标识	

	//接口变量
protected:
	ID3DDeviceSink *				m_pID3DDeviceSink;					//回调接口
	IDirect3DDevice9 *				m_pIDirect3DDevice;					//设备对象

	//矩形绘画
protected:
	IDirect3DVertexBuffer9 *		m_pIFillRectBuffer;					//填充矩形顶点索引
	IDirect3DVertexBuffer9 *		m_pIBoxRectBuffer;					//矩形框顶点索引
	IDirect3DVertexBuffer9 *		m_pILineBuffer;						//直线顶点索引

	//默认信息
protected:
	IDirect3DTexture9*				m_pDefaultTex;						//默认纹理
	D3DMATERIAL9					m_DefaultMaterial;					//默认材质
	D3DLIGHT9						m_DefaultLight;						//默认光源

	//投影矩阵
protected:
	D3DXMATRIX						m_MatrixOrtho;						//正交矩阵
	D3DXMATRIX						m_MatrixPerspective;				//透视矩阵

	//摄像机
protected:
	CD3DCamera						m_D3DCamera;						//摄像机

	//效果池
protected:
	CD3DEffectPool                  m_D3DEffectPool;                    //效果池

	//函数定义
public:
	//构造函数
	CD3DDevice();
	//析构函数
	virtual ~CD3DDevice();

	//信息函数
public:
	//获取窗口
	HWND GetWndDevice() { return m_hWndDevice; }
	//获取大小
	CSize GetSizeWindow() { return m_SizeWindow; }

	//配置接口
public:
	//设多线程
	VOID SetMultiThreaded(bool bMultiThreaded);
	//设置采样
	VOID SetEnableMultiSample(bool bEnableMultiSample) { m_bEnableMultiSample=bEnableMultiSample; }
	//设置颜色
	VOID SetBackColor(D3DCOLOR crBackColor) { m_crBackColor=crBackColor; }

	//获取接口
public:
	//回调接口
	ID3DDeviceSink * GetD3DDeviceSink() { return m_pID3DDeviceSink; }
	//设备接口
	IDirect3DDevice9 * GetDirect3DDevice() { return m_pIDirect3DDevice; }

	//获取默认信息
public:
	//获取默认纹理
	IDirect3DTexture9*	GetDefaultTex() { return m_pDefaultTex; };
	//获取默认材质
	D3DMATERIAL9*	GetDefaultMaterial() { return &m_DefaultMaterial; };

	//获取摄像头信息
public:
	//获取摄像机
	CD3DCamera* GetD3DCamera() { return &m_D3DCamera; };
	//获取当前观察点
	D3DXMATRIX* GetCurrentView() { return m_D3DCamera.GetCurrentView(); };
	//获取默认观察点
	D3DXMATRIX* GetDefaultView() { return m_D3DCamera.GetDefaultView(); };

	//效果池信息
public:
	//获取效果池
	CD3DEffectPool * GetD3DEffectPool() { return &m_D3DEffectPool; };

	//获取投影矩阵
public:
	//获取正交
	D3DXMATRIX* GetMatrixOrtho() { return &m_MatrixOrtho; };
	//获取透视
	D3DXMATRIX* GetMatrixPerspective() { return &m_MatrixPerspective; };

	//灯光设置
public:
	//设置灯光
	void SetLight( DWORD Index,CONST CD3DLight* pD3DLight );
	//激活灯光
	void LightEnable( DWORD Index, BOOL Enable );

	//控制函数
public:
	//重置设备
	bool ResetD3DDevice();
	//渲染设备
	bool RenderD3DDevice();
	//创建设备
	bool CreateD3DDevice(HWND hWndDevice, ID3DDeviceSink * pID3DDeviceSink);

	//渲染状态
public:
	//渲染属性
	HRESULT SetRenderState(D3DRENDERSTATETYPE State, DWORD Value);
	//渲染属性
	HRESULT GetRenderState(D3DRENDERSTATETYPE State, DWORD & Value);

	//设置投影
public:
	//设置正交投影
	void SetOrthoProjection( INT nWidth, INT nHeight, FLOAT fNear, FLOAT fFar );
	//设置透视投影
	void SetPerspectiveProjection( FLOAT fovy, FLOAT Aspect, FLOAT fNear, FLOAT fFar );

	//设置观察点
public:
	//设置观察点
	void SetViewpoint( D3DXVECTOR3 vEye, D3DXVECTOR3 Lookat );
	
	//填充函数
public:
	//填充颜色
	VOID FillRect(CRect rcRect, D3DCOLOR D3DColor);
	//填充颜色
	VOID FillRect(INT nXPos, INT nYPos, INT nWidth, INT nHeigth, D3DCOLOR D3DColor);

	//绘画函数
public:
	//绘画矩形
	VOID DrawRect(CRect rcRect, D3DCOLOR D3DColor);
	//绘画线条
	VOID DrawLine(INT nXPos1, INT nYPos1, INT nXPos2, INT nYPos2, D3DCOLOR D3DColor);

	//功能函数
public:
	//信息还原
	static void InfoReduction(BYTE* pImageData, UINT& lFileLength);
	//图片解析
	static void DecryptionOld( BYTE* pDataDec, BYTE* pHead, UINT& uSize );
	//图片解析
	static void DecryptionNew( BYTE* pDataDec, BYTE* pHead, UINT& uSize );

	//内部函数
private:
	//设置环境
	VOID InitD3DDevice();
	//设置数据
	VOID InitPresentParameters(D3DPRESENT_PARAMETERS & PresentParameters, HWND hWnd, INT nWidth, INT nHeight, D3DFORMAT BufferFormat);
};

//////////////////////////////////////////////////////////////////////////////////

#endif