#ifndef OGL_FOUNT_HEAD_FILE
#define OGL_FOUNT_HEAD_FILE

#include "OGLDevice.h"
#include "OGLTexture.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//字体子项
struct tagOGLFontItem
{
	//字符信息
	UINT							uChar;								//字体编码
	COGLTexture						OGLTexture;							//字体纹理

	//输出位置
	CSize							SizeCell;							//单元大小
	CSize							SizeFont;							//字体大小
	CPoint							PointOrigin;						//原点位置
};

//类型定义
typedef CWHArray<tagOGLFontItem *> COGLFontItemPtrArray;
typedef CMap<UINT,UINT,tagOGLFontItem *,tagOGLFontItem *> COGLFontItemPtrMap;

//////////////////////////////////////////////////////////////////////////////////

//字体管理
class GAME_ENGINE_CLASS COGLFont
{
	//配置变量
protected:
	UINT							m_uGrayLevel;						//灰度等级
	UINT							m_uMaxBufferCount;					//缓冲数目

	//字体对象
protected:
	CFont							m_Font;								//字体对象
	TEXTMETRIC						m_TextMetric;						//字体属性

	//内核对象
protected:
	COGLFontItemPtrMap				m_FontItemPtrMap;					//字体索引
	COGLFontItemPtrArray			m_FontItemPtrActive;				//字体数组

	//函数定义
public:
	//构造函数
	COGLFont();
	//析构函数
	virtual ~COGLFont();

	//缓冲数目
public:
	//缓冲数目
	UINT GetMaxBufferCount() { return m_uMaxBufferCount; }
	//缓冲数目
	VOID SetMaxBufferCount(UINT uMaxBufferCount) { m_uMaxBufferCount=uMaxBufferCount; }

	//创建函数
public:
	//删除字体
	bool DeleteFont();
	//创建字体
	bool CreateFont(LOGFONT & LogFont, UINT uGrayLevel);
	//创建字体
	bool CreateFont(INT nPointSize, LPCTSTR lpszFaceName, UINT uGrayLevel);

	//输出字体
public:
	//输出字体
	bool DrawText(COGLDevice * pOGLDevice, LPCTSTR pszString, CRect rcDraw,UINT uFormat, D3DCOLOR D3DColor);
	//输出字体
	bool DrawText(COGLDevice * pOGLDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT uFormat, D3DCOLOR D3DColor);

	//字体管理
private:
	//创建字体
	tagOGLFontItem * ActiveFontItem(COGLDevice * pOGLDevice, UINT uChar);
	//计算位置
	CSize CalePostion(COGLDevice * pOGLDevice, LPCTSTR pszString, UINT uFormat, INT nAreaWidth);
};

//////////////////////////////////////////////////////////////////////////////////

#endif