#pragma once
#include "D3DDevice.h"
#include "GameEngineHead.h"

class GAME_ENGINE_CLASS CD3DFontEx
{
	//�������
public:
	ID3DXFont*							m_ppFont;				// D3D����
	int									m_nWidth;				// ������


	//���캯��
public:
	CD3DFontEx(void);
	~CD3DFontEx(void);

	//�жϺ���
public:
	//�Ƿ�Ϊ��
	bool IsNull();

	//��������
public:
	//ɾ������
	bool DeleteFont();
	//��������
	bool CreateFont(CD3DDevice * pD3DDevice, INT Height, UINT Weight, LPCWSTR pFaceName);
	// ��ȡ���ִ�С
	CSize GetTextSize(LPCTSTR pszString);
	//��ȡ�ַ�����
	static int GetCharLength(TCHAR chInfo, int nWidth);

	//�滭����
public:
	//�滭����
	void DrawText( LPCWSTR str, LPRECT lpRect, COLORREF colorref, UINT nFormat);
	//�滭����
	void DrawText( LPCWSTR str, int XPos, int YPos, COLORREF colorref, UINT nFormat);
};
