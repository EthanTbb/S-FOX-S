#include "StdAfx.h"
#include ".\stringmessage.h"

#define IDM_CHAT_MESSAGE			WM_USER+208

CStringMessage::CStringMessage(void)
{
}

CStringMessage::~CStringMessage(void)
{
}


//�ͷŶ���
VOID CStringMessage::Release()
{
	delete this;
}
//�ӿڲ�ѯ
VOID * CStringMessage::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IStringMessage,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IStringMessage,Guid,dwQueryVer);
	return NULL;
}

//�����¼�
bool CStringMessage::InsertUserEnter(LPCTSTR pszUserName)
{
	SendMessageToEngine(pszUserName,_T("������"),COLOR_NORMAL,enSystem);

	return true;
}
//�뿪�¼�
bool CStringMessage::InsertUserLeave(LPCTSTR pszUserName)
{
	SendMessageToEngine(pszUserName,_T("�뿪��"),COLOR_NORMAL,enSystem);

	return true;
}
//�����¼�
bool CStringMessage::InsertUserOffLine(LPCTSTR pszUserName)
{
	SendMessageToEngine(pszUserName,_T("�����ˣ������ĵȺ�"),COLOR_NORMAL,enSystem);
	return true;
}

//��ͨ��Ϣ
bool CStringMessage::InsertNormalString(LPCTSTR pszString)
{
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enNormalString);	
	return true;
}
//ϵͳ��Ϣ
bool CStringMessage::InsertSystemString(LPCTSTR pszString)
{	
	AfxMessageBox(pszString);
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enSystem);
	
	return true;
}
//��ʾ��Ϣ
bool CStringMessage::InsertPromptString(LPCTSTR pszString)
{
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enPrompt);
	
	return true;
}
//������Ϣ
bool CStringMessage::InsertAfficheString(LPCTSTR pszString)
{
	SendMessageToEngine(NULL,pszString,COLOR_NORMAL,enAffiche);
	
	return true;
}

//������Ϣ
bool CStringMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor)
{

	SendMessageToEngine(NULL,pszString,crColor,enCustom);

	return true;
}
//������Ϣ
bool CStringMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)
{
	return true;
}

//�û�����
bool CStringMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)
{
	SendMessageToEngine(pszSendUser,pszImagePath,COLOR_NORMAL,enExpression);

	return true;
}
//�û�����
bool CStringMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)
{
	SendMessageToEngine(pszSendUser,pszImagePath,COLOR_NORMAL,enExpression);

	return true;
}
//�û�����
bool CStringMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)
{
	SendMessageToEngine(pszSendUser,pszImagePath,COLOR_NORMAL,enExpression);

	return true;
}

//�û�����
bool CStringMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)
{
	SendMessageToEngine(pszSendUser,pszString,crColor,enChat);

	return true;
}
//�û�����
bool CStringMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)
{
	return true;
}
//�û�˽��
bool CStringMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)
{
	return true;
}

//�û�����
bool CStringMessage::InsertUserTrumpet( LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor )
{	
	SendMessageToEngine(pszSendUser,pszString,crColor,enUserTrumpet);

	return true;
}

//�û�����
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



