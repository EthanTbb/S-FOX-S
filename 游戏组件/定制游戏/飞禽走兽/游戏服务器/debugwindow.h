#ifndef _DBWINDOW_
#define _DBWINDOW_

#include <windows.h>

//����̨���ʱ���ı���ɫ
#define WDS_T_RED    FOREGROUND_RED
#define WDS_T_GREEN  FOREGROUND_GREEN
#define WDS_T_BLUE   FOREGROUND_BLUE
//����̨���ʱ���ı�������ɫ
#define WDS_BG_RED   BACKGROUND_RED
#define WDS_BG_GREEN BACKGROUND_GREEN
#define WDS_BG_BLUE   BACKGROUND_BLUE

//���ÿ���̨������ڱ���
BOOL DBWindowTile(LPCTSTR tile);
//��ʽ���ı����
BOOL DBWindowWrite(LPCTSTR fmt,...);
//����ɫ��ʽ���ı����
BOOL DBWindowWrite(WORD Attrs,LPCTSTR fmt,...);

#else
#define DBWindowTile
#define DBWindowWrite
#endif


#ifndef _UNICODE
#define myprintf  _snprintf
#else
#define myprintf  swprintf
#endif

#define CONSOLE_TILE _T("�������޷���˵��Դ���")

class ConsoleWindow
{
public:
  ConsoleWindow();
  virtual ~ConsoleWindow();

  BOOL SetTile(LPCTSTR lpTile);
  BOOL WriteString(LPCTSTR lpString);
  BOOL WriteString(WORD Attrs,LPCTSTR lpString);
private:
  HANDLE m_hConsole;
  BOOL   m_bCreate;
  BOOL   m_bAttrs;
  WORD   m_OldColorAttrs;
};



