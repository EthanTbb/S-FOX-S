#pragma once

#include "Stdafx.h"
class CChatMessage : public CRichEditMessage
{
	//图片资源
protected:
	CDataObject                     m_ImageDataChat;                  //资源变量 

public:
	CChatMessage(void);
	virtual ~CChatMessage(void);

	//重写函数
public:
	//用户聊天
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor);
	//重载函数
protected:
	//控件绑定
	virtual VOID PreSubclassWindow();
	//事件消息
public:
	//进入事件
	virtual bool InsertUserEnter(LPCTSTR pszUserName);
	//离开事件
	virtual bool InsertUserLeave(LPCTSTR pszUserName);
	//断线事件
	virtual bool InsertUserOffLine(LPCTSTR pszUserName);

	//字符消息
public:
	//普通消息
	virtual bool InsertNormalString(LPCTSTR pszString);
	//系统消息
	virtual bool InsertSystemString(LPCTSTR pszString);
	//提示消息
	virtual bool InsertPromptString(LPCTSTR pszString);
	//公告消息
	virtual bool InsertAfficheString(LPCTSTR pszString);

	//定制消息
public:
	//定制消息
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor);
	//定制消息
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor);

	//表情消息
public:
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath);
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString);
	//用户表情
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath);

	//聊天消息
public:
	//用户聊天
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor);
	//用户私聊
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString);
	//用户私聊
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString, const SYSTEMTIME &time);
	//用户喇叭
	virtual bool InsertUserTrumpet(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor);
	//用户喇叭
	virtual bool InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor);
	
	//格式化消息
protected:
	//用户名
	void InsertUserName(LPCTSTR pszSendUser);
	//翻译字符
	bool TranslateString(LPCTSTR pszString, COLORREF crColor);


	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////////