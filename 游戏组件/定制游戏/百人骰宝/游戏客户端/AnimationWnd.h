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
			int  m_nIndex;				//��ǰ֡���
			int  m_nTimer;				//��һ֡�����ʱ��
			int  m_nXPos;				//ͼ���е�λ��X
			int  m_nYPos;				//ͼ���е�λ��Y
			int  m_nCX;					
			int  m_nCY;
			bool m_bSendMsg;			//������Ϣ
			
			CPngImage *m_PngImage;		//ͼ��ID;
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
		//����֡
		bool UpdataFrame(int cbIndex,int nXPos,int nYPos);
		//����֡��Ϣ
		bool SetFrameInfo(int cbIndex,int cbTimer,int nXPos,int nYPos,int nCX,int nCY,CPngImage *pPngImage,bool bSendMsg=false);
		//���ö�����Ϣ
		bool SetCartoonInfo(int nCartoonID,int nFrameCount,LPCTSTR pszSoundName=NULL);
		//��ȡ֡��Ϣ
		tagFrameInfo* GetFrameInfo(int nIndex);

		~CGameCartoon();
	public:
		//����ID
		int m_nCartoonID;
		//����֡��
		int m_nFrameCount;
		//������Ч
		CString m_pszSoundName;
		//����֡�б�
		tagFrameInfo*m_pFramList;
};

class CAnimationWnd : public CWnd
{
	DECLARE_DYNAMIC(CAnimationWnd)
public:
	CAnimationWnd(CWnd*pParent);
   //���Ŷ���
	bool PlayAnimation(
						WORD wCharID,				//����λ��
						CGameCartoon&CatroonInfo,	//֡��Ϣ
						int	 nPlayTimes=1,			//ִ�д���	
						bool bloop=false,			//�Ƿ�ѭ������
						bool bPostEndEvent=true,	//�Ƿ����Ϣ�¼�
						bool bSoundLoop=false		//���������Ƿ�ѭ������
					  );
	//����˫����
	void  CreateMemDC();
	//�رն���
	bool  CloseAninamation();
	//��������
	void  CopyBackGroud(CDC*pDC);
	//�����ڸ�������λ��
	void SetPosInParent(CRect&Pos);
	//���ö�������
	void  SetBGImage(UINT RCImgeID,bool IsWndSize=false);
	//������Ч
	bool PlayAninamationSound(HINSTANCE hInstance, LPCTSTR pszSoundName);
	//��������
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
	CRect     m_rcInParent;         //�ڸ������е�λ��
	CRect     m_rcClientRect;       //����ߴ�
	int       m_nAniamationID;      //�������
	int       m_nFrameCount;        //����֡��
	int       m_nCurFrame;          //��ǰ֡
	int		  m_nPlayTime;			//ִ�д���
	bool      m_bISRuning;          //����ִ�ж���
	bool      m_bloop;             //�Ƿ�ѭ������
	bool      m_bHaveBGImage;       //�Ƿ񿽱�������
	bool      m_bPostEndEvent;      //�Ƿ�Ͷ�ݽ�����Ϣ
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


