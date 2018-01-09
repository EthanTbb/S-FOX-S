#ifndef OGL_FOUNT_HEAD_FILE
#define OGL_FOUNT_HEAD_FILE

#include "OGLDevice.h"
#include "OGLTexture.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagOGLFontItem
{
	//�ַ���Ϣ
	UINT							uChar;								//�������
	COGLTexture						OGLTexture;							//��������

	//���λ��
	CSize							SizeCell;							//��Ԫ��С
	CSize							SizeFont;							//�����С
	CPoint							PointOrigin;						//ԭ��λ��
};

//���Ͷ���
typedef CWHArray<tagOGLFontItem *> COGLFontItemPtrArray;
typedef CMap<UINT,UINT,tagOGLFontItem *,tagOGLFontItem *> COGLFontItemPtrMap;

//////////////////////////////////////////////////////////////////////////////////

//�������
class GAME_ENGINE_CLASS COGLFont
{
	//���ñ���
protected:
	UINT							m_uGrayLevel;						//�Ҷȵȼ�
	UINT							m_uMaxBufferCount;					//������Ŀ

	//�������
protected:
	CFont							m_Font;								//�������
	TEXTMETRIC						m_TextMetric;						//��������

	//�ں˶���
protected:
	COGLFontItemPtrMap				m_FontItemPtrMap;					//��������
	COGLFontItemPtrArray			m_FontItemPtrActive;				//��������

	//��������
public:
	//���캯��
	COGLFont();
	//��������
	virtual ~COGLFont();

	//������Ŀ
public:
	//������Ŀ
	UINT GetMaxBufferCount() { return m_uMaxBufferCount; }
	//������Ŀ
	VOID SetMaxBufferCount(UINT uMaxBufferCount) { m_uMaxBufferCount=uMaxBufferCount; }

	//��������
public:
	//ɾ������
	bool DeleteFont();
	//��������
	bool CreateFont(LOGFONT & LogFont, UINT uGrayLevel);
	//��������
	bool CreateFont(INT nPointSize, LPCTSTR lpszFaceName, UINT uGrayLevel);

	//�������
public:
	//�������
	bool DrawText(COGLDevice * pOGLDevice, LPCTSTR pszString, CRect rcDraw,UINT uFormat, D3DCOLOR D3DColor);
	//�������
	bool DrawText(COGLDevice * pOGLDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT uFormat, D3DCOLOR D3DColor);

	//�������
private:
	//��������
	tagOGLFontItem * ActiveFontItem(COGLDevice * pOGLDevice, UINT uChar);
	//����λ��
	CSize CalePostion(COGLDevice * pOGLDevice, LPCTSTR pszString, UINT uFormat, INT nAreaWidth);
};

//////////////////////////////////////////////////////////////////////////////////

#endif