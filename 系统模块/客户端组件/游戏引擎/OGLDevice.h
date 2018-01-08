#ifndef OGL_DEVICE_HEAD_FILE
#define OGL_DEVICE_HEAD_FILE

#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class COGLDevice;

//////////////////////////////////////////////////////////////////////////////////

//�߳̾��
struct tagOGLThreadRC
{
	HDC								hDC;								//�豸���
	HGLRC							hRC;								//��Ⱦ���
};

//����˵��
typedef CWHArray<tagOGLThreadRC>	COGLThreadRCArray;					//�̶߳���

//////////////////////////////////////////////////////////////////////////////////

//�豸�ӿ�
interface IOGLDeviceSink
{
	//�豸�¼�
public:
	//�����豸
	virtual VOID OnInitDevice(COGLDevice * pOGLDevice, INT nWidth, INT nHeight)=NULL;
	//��Ⱦ����
	virtual VOID OnRenderWindow(COGLDevice * pOGLDevice, INT nWidth, INT nHeight)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

//�豸����
class GAME_ENGINE_CLASS COGLDevice
{
	//��������
protected:
	HWND 							m_hWndDevice;						//�豸����
	CSize							m_SizeWindow;						//���ڴ�С

	//�ں˶���
protected:
	HDC 							m_hDC;								//�滭���
	HGLRC							m_hRC;								//��Ⱦ���
	COGLThreadRCArray				m_OGLThreadRCArray;					//�̶߳���

	//�ӿڱ���
protected:
	IOGLDeviceSink *				m_pIOGLDeviceSink;					//�ص��ӿ�

	//��������
public:
	//���캯��
	COGLDevice();
	//��������
	virtual ~COGLDevice();

	//��Ϣ����
public:
	//��ȡ����
	HWND GetWndDevice() { return m_hWndDevice; }
	//��ȡ��С
	CSize GetSizeWindow() { return m_SizeWindow; }

	//���ƺ���
public:
	//ɾ���豸
	bool DeleteDevice();
	//�����豸
	bool CreateDevice(HWND hWndDevice, IOGLDeviceSink * pIOGLDeviceSink);

	//�滭����
public:
	//�滭����
	VOID DrawRect(CRect rcRect, COLORREF crColor);
	//�滭����
	VOID DrawPoint(INT nXPos, INT nYPos, COLORREF crColor);
	//�滭����
	VOID DrawLine(INT nX1, INT nY1, INT nX2, INT nY2, COLORREF crColor);
};

//////////////////////////////////////////////////////////////////////////////////

#endif