#pragma once
#include <vector>
using namespace std;

#include "Mmsystem.h"
#pragma   comment(lib,   "winmm.lib") 

#define AM_GAMEDEAL			10
#define AM_OVERCARD			1
#define AM_TENHALFPOINT		3
#define AM_FIVECARD			4
#define AM_BANKERWIN		5
#define AM_LOOKCARD			6
#define AM_USERTHINK		7
#define AM_REMINDBET		8
#define AM_MIXCARD			9
#define WM_ANIMATIONFINISH WM_USER+5001
// CAnimationWnd

class CGameCartoon
{
	public:
		class tagFrameInfo
		{
		public:
			int  m_nIndex;				//当前帧序号
			int  m_nTimer;				//下一帧间隔的时间
			int  m_nXPos;				//图像中的位置X
			int  m_nYPos;				//图像中的位置Y
			int  m_nCX;					
			int  m_nCY;
			bool m_bSendMsg;			//发送消息
			
			CPngImage *m_PngImage;		//图像ID;
		public:
			void SetFrameInfo(int cbIndex,	int cbTimer,int nXPos,int nYPos,int nCX,int nCY,CPngImage *pPngImage,bool bSendMsg=false)
			{
				m_nIndex=cbIndex;
				m_nTimer=cbTimer;
				m_nXPos=nXPos;
				m_nYPos=nYPos;
				m_nCX=nCX;
				m_nCY=nCY;
				m_PngImage=pPngImage;
				m_bSendMsg=bSendMsg;
			}
			tagFrameInfo()
			{
				m_nIndex=0;
				m_nTimer=0;
				m_nXPos=0;
				m_nYPos=0;
				m_nCX=0;
				m_nCY=0;
				m_bSendMsg=false;
				m_PngImage=NULL;
			}
		};	
		public:
		CGameCartoon();
		//更新帧
		bool UpdataFrame(int cbIndex,int nXPos,int nYPos);
		//增加帧信息
		bool SetFrameInfo(int cbIndex,int cbTimer,int nXPos,int nYPos,int nCX,int nCY,CPngImage *pPngImage,bool bSendMsg=false);
		//设置动画信息
		bool SetCartoonInfo(int nCartoonID,int nFrameCount,LPCTSTR pszSoundName=NULL);
		//获取帧信息
		tagFrameInfo* GetFrameInfo(int nIndex);

		~CGameCartoon();
	public:
		//动画ID
		int m_nCartoonID;
		//动画帧数
		int m_nFrameCount;
		//背景音效
		CString m_pszSoundName;
		//动画帧列表
		tagFrameInfo*m_pFramList;
};

class CAnimationWnd : public CWnd
{
	DECLARE_DYNAMIC(CAnimationWnd)
public:
	CAnimationWnd(CWnd*pParent);
   //播放动画
	bool PlayAnimation(
						WORD wCharID,				//播放位置
						CGameCartoon&CatroonInfo,	//帧信息
						int	 nPlayTimes=1,			//执行次数	
						bool bloop=false,			//是否循环播放
						bool bPostEndEvent=true,	//是否发完成息事件
						bool bSoundLoop=false		//背景音乐是否循环播放
					  );
	//建立双缓冲
	void  CreateMemDC();
	//关闭动画
	bool  CloseAninamation();
	//拷贝背景
	void  CopyBackGroud(CDC*pDC);
	//设置在父窗体中位置
	void SetPosInParent(CRect&Pos);
	//设置动画背景
	void  SetBGImage(UINT RCImgeID,bool IsWndSize=false);
	//播放音效
	bool PlayAninamationSound(HINSTANCE hInstance, LPCTSTR pszSoundName);
	//析构函数
	virtual ~CAnimationWnd();

public:
	CSize							m_FrameSize;
	bool							m_bSoundLoop;
	bool							m_bUserFramList;
	bool							m_bFramListMode;
	CString							m_szSoundName;
	CGameCartoon::tagFrameInfo*		m_pFrameInfo;

protected:
	int       *m_pDelayTime;
	int       m_nCurUser;
	CRect     m_rcInParent;         //在父窗体中的位置
	CRect     m_rcClientRect;       //窗体尺寸
	int       m_nAniamationID;      //动画序号
	int       m_nFrameCount;        //动画帧数
	int       m_nCurFrame;          //当前帧
	int		  m_nPlayTime;			//执行次数
	bool      m_bISRuning;          //正在执行动画
	bool      m_bloop;             //是否循环播放
	bool      m_bHaveBGImage;       //是否拷贝父窗体
	bool      m_bPostEndEvent;      //是否投递结束消息
	CDC        m_MemDCBG;
	CBitmap    m_MemBmpBG;
	CBitmap    *m_pMemBmpBG;
	CBitImage m_ImageBackGround;
	bool m_bHasCopy;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};


