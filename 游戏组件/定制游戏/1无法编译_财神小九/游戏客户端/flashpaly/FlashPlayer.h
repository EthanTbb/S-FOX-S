#pragma once
#include "ShockwaveFlash.h"
#include <string>
#include <list>
using namespace std;

class FlashSink;
class ControlSite;
class FlashPlayer2
{
	friend class FlashSink;
	friend class ControlSite;
public:
	typedef struct tagFlashCommand
	{
		std::string sCommand;
		std::string sParam;
	}FLASHCOMMAND;

	// FLASH ����״̬
	typedef enum tagFlashState
	{
		STATE_IDLE=0,
		STATE_PLAYING,
		STATE_STOPPED
	}FLASHSTATE;

	// FLASH ����Ʒ��
	typedef enum tagFlashQuality
	{
		QUALITY_LOW=0,
		QUALITY_MEDIUM,
		QUALITY_HIGH
	}FLASHQUALITY;

public:
	FLASHSTATE				m_State;

	HMODULE					m_hFlashLibHandle;					//flash.ocx

	ControlSite*			m_pControlSite;						//����̨
	FlashSink*				m_pFlashSink;						//�¼�������
	ShockwaveFlashObjects2::IShockwaveFlash2* m_pFlashInterface;	//flash�ӿ�
	IOleObject*				m_pOleObject;						//OLE����ָ��
	IOleInPlaceObjectWindowless* m_pWindowlessObject;			//OLD���ڶ���

	int						m_nCOMCount;						//COM������
	int						m_nPauseCount;
	BOOL					m_bFlashDirty;
	RECT					m_rcFlashDirty;

	std::list<FLASHCOMMAND>	m_FlashCommands;

	LONG					m_nWidth;
	LONG					m_nHeight;

	HDC						m_hFlashDC;
	HBITMAP					m_hBitmap;
	BITMAPINFO				m_BMPINFO;
	void*					m_pFrameBuffer;

private:
	void					CleanupBuffer();
	void					RebuildBuffer(LONG nWidth,LONG nHeight);

public:
	void					GetFlashSize(LONG* nWidth,LONG* nHeight);
	void					GetBitmapSize(LONG* nWidth,LONG* nHeight);

	HDC						GetFlashDC();
	HBITMAP					GetFlashBitmap();
	BITMAPINFO				GetBitmapInfo();
	void*					GetFlashFrameBuffer();

public:
	FlashPlayer2();
	~FlashPlayer2();

	static double			GetFlashVersion();

	void					ReSetShowRect(int nw, int nh);
	bool					StartAnimation(const std::wstring& theFileName,LONG nWidth,LONG nHeight,HWND hWnd=0,LONG nBmpWidth=0,LONG nBmpHeight=0);	
	void					SetQuality(FLASHQUALITY theQuality);

	bool					GetFlashCommand(std::string& sCommand, std::string& sParam);
	void					FlashCommand(const std::string& sCommand, const std::string& sParam);

	bool					IsPlaying();
	void					Stop();
	void					Pause(bool bShow=false);
	void					Unpause();
	void					Back();
	void					Rewind();
	void					Forward();	
	void					GotoFrame(int theFrameNum);

	int						GetCurrentFrame();
	int						GetTotalFrames();
	int						GetFrameNum();
	int						GetPercentLoaded();
	std::string				GetCurrentLabel(const std::string& theTimeline);

	LONG					GetBackgroundColor();
	void					SetBackgroundColor(LONG theColor);

	BOOL					GetLoop();
	void					SetLoop(BOOL bLoop);

	void					CallFrame(const std::string& theTimeline, int theFrameNum);
	void					CallLabel(const std::string& theTimeline, const std::string& theLabel);

	std::string				GetVariable(const std::string& theName);
	void					SetVariable(const std::string& theName, const std::string& theValue);

	void					MouseMove(int x, int y);
	void					MouseDown(int x, int y);		
	void					MouseUp(int x, int y);

	BOOL					Update();
	BOOL					IsRender(RECT& rcDirty);
	BOOL					Render(HDC hDC,LONG x,LONG y,BOOL bClear);

	void					SaveFlashBitmapBuffer(char* lpsFileName);
};

void ReplaceName(const char fmt[], const char name_a[], const char name_b[], const int num, char result[]);