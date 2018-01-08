#pragma once
#include "D3DDevice.h"
#include "GameEngineHead.h"

class GAME_ENGINE_CLASS CD3DFontEx
{
	//字体对象
public:
	ID3DXFont*							m_ppFont;				// D3D字体
	int									m_nWidth;				// 字体宽度


	//构造函数
public:
	CD3DFontEx(void);
	~CD3DFontEx(void);

	//判断函数
public:
	//是否为空
	bool IsNull();

	//创建函数
public:
	//删除字体
	bool DeleteFont();
	//创建字体
	bool CreateFont(CD3DDevice * pD3DDevice, INT Height, UINT Weight, LPCWSTR pFaceName);
	// 获取文字大小
	CSize GetTextSize(LPCTSTR pszString);
	//获取字符长度
	static int GetCharLength(TCHAR chInfo, int nWidth);

	//绘画字体
public:
	//绘画字体
	void DrawText( LPCWSTR str, LPRECT lpRect, COLORREF colorref, UINT nFormat);
	//绘画字体
	void DrawText( LPCWSTR str, int XPos, int YPos, COLORREF colorref, UINT nFormat);
};
