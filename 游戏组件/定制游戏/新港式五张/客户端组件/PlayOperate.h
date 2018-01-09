#pragma once
#include "SHButton.h"
#include "SkinButtonEx.h"

#define PYOE_LEFT                     0x00000000
#define PYOE_CENTER                   0x00000001
#define PYOE_RIGHT                    0x00000002
#define PYOE_TOP                      0x00000000
#define PYOE_VCENTER                  0x00000004
#define PYOE_BOTTOM                   0x00000008

// ��Ҳ���
class CPlayOperate
{
	// ��������
protected:
	bool							m_bShow;				// ��ʾ��ʾ
	bool							m_bShowAdd;				// ��ʾ��ע
	bool							m_bMouseDown;			// �����
	LONG							m_lCellScore;			// ��Ϸ��ע
	CPoint							m_ptMouse;				// �϶�λ��
	int								m_lDownSite;			// �϶�λ��
	int								m_lDragSite;			// �϶�λ��
	SCORE							m_lScore;				// ��ǰ����
	SCORE							m_lMaxScore;			// �����ע
	SCORE							m_lMinScore;			// ��С��ע

	// ���Զ���
protected:
	CPoint							m_ptPos;				// λ����Ϣ
	CSize							m_sizeBakc;				// ��С��Ϣ
	CRect							m_rectDrag;				// �϶�����
	CSize							m_sizeDrag;				// �϶�ͼƬ��С

	// ��Դ����
public:
	CPngImageEx						m_ImageBack;			// ����ͼƬ
	CPngImageEx						m_ImageBTBack;			// ����ͼƬ
	CPngImageEx						m_ImageScoreNumber;		// ����ͼƬ
	CPngImageEx						m_ImageDragBack;		// δѡͼƬ
	CPngImageEx						m_ImageDragSelect;		// ѡ��ͼƬ
	CPngImageEx						m_ImageDrag;			// �϶�ͼƬ
	CSkinButtonEx					m_btOneScore;			// 1����ť
	CSkinButtonEx					m_btTwoScore;			// 2����ť
	CSkinButtonEx					m_btThreeScore;			// 3����ť	
	CSkinButtonEx					m_btFourScore;			// 4����ť
	CSHButton						m_btOK;					// ȷ�ϰ�ť

	CSkinButtonEx					m_btAddScore;			// ��ע��ť
	CSkinButtonEx					m_btFollow;				// ��ע��ť
	CSkinButtonEx					m_btGiveUp;				// ������ť
	CSkinButtonEx					m_btShowHand;			// �����ť

	CFont							m_fontScoreInput;		// ��������

public:
	CPlayOperate(void);
	~CPlayOperate(void);

	// ���Ժ���
public:
	// ��ʼ��
	void Initialization(CWnd* pParent);
	// ��ť��Ϣ
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// �滭����
public:
	// �滭����
	void DrawPlayOperate(CDC * pDC, INT nWidth, INT nHeight);
	// �滭����
	void DrawNumber(CDC * pDC , CPngImageEx* ImageNumber, TCHAR * szImageNum, LPCTSTR szOutNum ,INT nXPos, INT nYPos,  UINT uFormatX /*= DT_LEFT*/, UINT uFormatY /*= DT_TOP*/);
	// �滭����
	void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormatX = DT_LEFT,  UINT uFormatY = DT_TOP);

	// ���ú���
public:
	// ����λ��
	void SetOperatePos( int nPosX, int nPosY);
	// ������Ϣ
	void SetCellScore( LONG lCellScore);
	// ������ע��Χ��
	void SetBetCount( LONGLONG lMaxBetCount, LONGLONG lMinBetCount );

	// ��ȡ����
public:
	// ��ȡ��������
	LONGLONG GetBetCount();
	// ��ȡλ��
	CRect GetOperateRect();
	// ��ȡ��ʾ״̬
	bool	 IsShow() { return m_bShow; }
	// ��ȡ��ע״̬
	bool	 IsAddShow() { return m_btAddScore.IsWindowEnabled()&&m_btAddScore.IsWindowVisible(); }


	// ���ܺ���
public:
	// ��ʾ����
	void ShowOperate( bool bShow );
	// ��ʾ����
	void ShowOperateAdd( bool bShow );
	
	// ��Ϣ����
	BOOL PreTranslateMessage(MSG * pMsg);
	// ���û���
	void SetEditScore();
	// ���û���
	void SetDragScore();
	// ���û���
	void ScoreADD();
	// ���û���
	void ScoreSUB();
	// ��Ӷ���
	CString AddComma( LONGLONG lScore );
	// ��Ӷ���
	CString AddComma( CString strScore );
	// ɾ������
	LONGLONG DeleteComma( CString strScore );
};
