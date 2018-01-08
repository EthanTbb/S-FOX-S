#ifndef OGL_DEVICE_HEAD_FILE
#define OGL_DEVICE_HEAD_FILE

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//类说明
class COGLDevice;

//////////////////////////////////////////////////////////////////////////////////

//线程句柄
struct tagOGLThreadRC
{
	HDC								hDC;								//设备句柄
	HGLRC							hRC;								//渲染句柄
};

//数组说明
typedef CWHArray<tagOGLThreadRC>	COGLThreadRCArray;					//线程对象

//////////////////////////////////////////////////////////////////////////////////

//设备接口
interface IOGLDeviceSink
{
	//设备事件
public:
	//配置设备
	virtual VOID OnInitDevice(COGLDevice * pOGLDevice, INT nWidth, INT nHeight)=NULL;
	//渲染窗口
	virtual VOID OnRenderWindow(COGLDevice * pOGLDevice, INT nWidth, INT nHeight)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//设备对象
class GAME_ENGINE_CLASS COGLDevice
{
	//窗口属性
protected:
	HWND 							m_hWndDevice;						//设备窗口
	CSize							m_SizeWindow;						//窗口大小

	//内核对象
protected:
	HDC 							m_hDC;								//绘画句柄
	HGLRC							m_hRC;								//渲染句柄
	COGLThreadRCArray				m_OGLThreadRCArray;					//线程对象

	//接口变量
protected:
	IOGLDeviceSink *				m_pIOGLDeviceSink;					//回调接口

	//函数定义
public:
	//构造函数
	COGLDevice();
	//析构函数
	virtual ~COGLDevice();

	//信息函数
public:
	//获取窗口
	HWND GetWndDevice() { return m_hWndDevice; }
	//获取大小
	CSize GetSizeWindow() { return m_SizeWindow; }

	//控制函数
public:
	//删除设备
	bool DeleteDevice();
	//创建设备
	bool CreateDevice(HWND hWndDevice, IOGLDeviceSink * pIOGLDeviceSink);

	//绘画函数
public:
	//绘画矩形
	VOID DrawRect(CRect rcRect, COLORREF crColor);
	//绘画单点
	VOID DrawPoint(INT nXPos, INT nYPos, COLORREF crColor);
	//绘画线条
	VOID DrawLine(INT nX1, INT nY1, INT nX2, INT nY2, COLORREF crColor);
};

//////////////////////////////////////////////////////////////////////////////////

#endif