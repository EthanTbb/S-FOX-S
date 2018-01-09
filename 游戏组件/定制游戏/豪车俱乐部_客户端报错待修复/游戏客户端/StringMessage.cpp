#include "StdAfx.h"
#include ".\stringmessage.h"

#define IDM_CHAT_MESSAGE			WM_USER+208

CStringMessage::CStringMessage(void)
{
}

CStringMessage::~CStringMessage(void)
{
}


//释放对象
VOID CStringMessage::Release()
{
	delete this;
}
//接口查询
VOID * CStringMessage::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IStringMessage,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IStringMessage,Guid,dwQueryVer);
	return NULL;
}

//进入事件
bool CStringMessage::InsertUserEnter(LPCTSTR pszUserName)
{
	SendMessageToEngine(pszUserName,_T("进来了"),COLOR_NORMAL,enSystem);

	return true;
}
//离开事件
bool CStringMessage::InsertUserLeave(LPCTSTR pszUserName)
{
	SendMessageToEngine(pszUserName,_T("离开了"),COLOR_NORMAL,enSystem);

	return true;
}
//断线事件
bool CStringMessage::InsertUserOffLine(LPCTSTR pszUserName)
{
	SendMessageToEngine(pszUserName,_T("断线了，请耐心等候！"),COLOR_NORMAL,enSystem);
	return true;
}

//普通消息
bool CStringMessage::InsertNormalString(LPCTSTR pszString)
{
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enNormalString);	
	return true;
}
//系统消息
bool CStringMessage::InsertSystemString(LPCTSTR pszString)
{	
	AfxMessageBox(pszString);
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enSystem);
	
	return true;
}
//提示消息
bool CStringMessage::InsertPromptString(LPCTSTR pszString)
{
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enPrompt);
	
	return true;
}
//公告消息
bool CStringMessage::InsertAfficheString(LPCTSTR pszString)
{
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enAffiche);
	
	return true;
}

//定制消息
bool CStringMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor)
{

	SendMessageToEngine(NULL,pszString,crColor,enCustom);

	return true;
}
//定制消息
bool CStringMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)
{
	return true;
}

//用户表情
bool CStringMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)
{
	SendMessageToEngine(pszSendUser,pszImagePath,COLOR_NORMAL,enExpression);

	return true;
}
//用户表情
bool CStringMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)
{
	SendMessageToEngine(pszSendUser,pszImagePath,COLOR_NORMAL,enExpression);

	return true;
}
//用户表情
bool CStringMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)
{
	SendMessageToEngine(pszSendUser,pszImagePath,COLOR_NORMAL,enExpression);

	return true;
}

//用户聊天
bool CStringMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)
{
	SendMessageToEngine(pszSendUser,pszString,crColor,enChat);

	return true;
}
//用户聊天
bool CStringMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)
{
	return true;
}
//用户私聊
bool CStringMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)
{
	return true;
}

//用户喇叭
bool CStringMessage::InsertUserTrumpet( LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor )
{	
	SendMessageToEngine(pszSendUser,pszString,crColor,enUserTrumpet);

	return true;
}

//用户喇叭
bool CStringMessage::InsertUserTyphon( LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor )
{
	SendMessageToEngine(pszSendUser,pszString,crColor,enUserTyphon);

	return true;
}

void CStringMessage::SendMessageToEngine(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor,int nMessageType)
{
	CGameFrameEngine * pGameFrameEngine=CGameFrameEngine::GetInstance();

	if (pGameFrameEngine!=NULL)
	{
		ChatMsg Msg;
		Msg.crColor = crColor;
		Msg.pszSendUser = pszSendUser;
		Msg.pszString = pszString;
		pGameFrameEngine->SendMessage(IDM_CHAT_MESSAGE,WPARAM(&Msg),nMessageType);
	}

}



