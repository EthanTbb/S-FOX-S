#pragma once
#include "SHButton.h"
#include "SkinButtonEx.h"

#define PYOE_LEFT                     0x00000000
#define PYOE_CENTER                   0x00000001
#define PYOE_RIGHT                    0x00000002
#define PYOE_TOP                      0x00000000
#define PYOE_VCENTER                  0x00000004
#define PYOE_BOTTOM                   0x00000008

// 玩家操作
class CPlayOperate
{
	// 变量定义
protected:
	bool							m_bShow;				// 显示标示
	bool							m_bShowAdd;				// 显示加注
	bool							m_bMouseDown;			// 鼠标点击
	LONG							m_lCellScore;			// 游戏底注
	CPoint							m_ptMouse;				// 拖动位置
	int								m_lDownSite;			// 拖动位置
	int								m_lDragSite;			// 拖动位置
	SCORE							m_lScore;				// 当前分数
	SCORE							m_lMaxScore;			// 最大下注
	SCORE							m_lMinScore;			// 最小下注

	// 属性定义
protected:
	CPoint							m_ptPos;				// 位置信息
	CSize							m_sizeBakc;				// 大小信息
	CRect							m_rectDrag;				// 拖动区域
	CSize							m_sizeDrag;				// 拖动图片大小

	// 资源定义
public:
	CPngImageEx						m_ImageBack;			// 背景图片
	CPngImageEx						m_ImageBTBack;			// 背景图片
	CPngImageEx						m_ImageScoreNumber;		// 分数图片
	CPngImageEx						m_ImageDragBack;		// 未选图片
	CPngImageEx						m_ImageDragSelect;		// 选择图片
	CPngImageEx						m_ImageDrag;			// 拖动图片
	CSkinButtonEx					m_btOneScore;			// 1倍按钮
	CSkinButtonEx					m_btTwoScore;			// 2倍按钮
	CSkinButtonEx					m_btThreeScore;			// 3倍按钮	
	CSkinButtonEx					m_btFourScore;			// 4倍按钮
	CSHButton						m_btOK;					// 确认按钮

	CSkinButtonEx					m_btAddScore;			// 加注按钮
	CSkinButtonEx					m_btFollow;				// 跟注按钮
	CSkinButtonEx					m_btGiveUp;				// 放弃按钮
	CSkinButtonEx					m_btShowHand;			// 梭哈按钮

	CFont							m_fontScoreInput;		// 积分输入

public:
	CPlayOperate(void);
	~CPlayOperate(void);

	// 属性函数
public:
	// 初始化
	void Initialization(CWnd* pParent);
	// 按钮消息
	BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	// 绘画函数
public:
	// 绘画函数
	void DrawPlayOperate(CDC * pDC, INT nWidth, INT nHeight);
	// 绘画数字
	void DrawNumber(CDC * pDC , CPngImageEx* ImageNumber, TCHAR * szImageNum, LPCTSTR szOutNum ,INT nXPos, INT nYPos,  UINT uFormatX /*= DT_LEFT*/, UINT uFormatY /*= DT_TOP*/);
	// 绘画数字
	void DrawNumber(CDC * pDC, CPngImageEx* ImageNumber, TCHAR * szImageNum, LONGLONG lOutNum, INT nXPos, INT nYPos, UINT uFormatX = DT_LEFT,  UINT uFormatY = DT_TOP);

	// 设置函数
public:
	// 设置位置
	void SetOperatePos( int nPosX, int nPosY);
	// 设置信息
	void SetCellScore( LONG lCellScore);
	// 设置下注范围数
	void SetBetCount( LONGLONG lMaxBetCount, LONGLONG lMinBetCount );

	// 获取函数
public:
	// 获取下载数量
	LONGLONG GetBetCount();
	// 获取位置
	CRect GetOperateRect();
	// 获取显示状态
	bool	 IsShow() { return m_bShow; }
	// 获取加注状态
	bool	 IsAddShow() { return m_btAddScore.IsWindowEnabled()&&m_btAddScore.IsWindowVisible(); }


	// 功能函数
public:
	// 显示函数
	void ShowOperate( bool bShow );
	// 显示函数
	void ShowOperateAdd( bool bShow );
	
	// 消息解释
	BOOL PreTranslateMessage(MSG * pMsg);
	// 设置积分
	void SetEditScore();
	// 设置积分
	void SetDragScore();
	// 设置积分
	void ScoreADD();
	// 设置积分
	void ScoreSUB();
	// 添加逗号
	CString AddComma( LONGLONG lScore );
	// 添加逗号
	CString AddComma( CString strScore );
	// 删除逗号
	LONGLONG DeleteComma( CString strScore );
};
