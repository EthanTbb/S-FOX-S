#include "StdAfx.h"
#include ".\chatmessage.h"
#include "GameClientEngine.h"

//////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CChatMessage, CRichEditMessage)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

CChatMessage::CChatMessage(void)
{
}

CChatMessage::~CChatMessage(void)
{
}


//用户聊天
bool CChatMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)
{
	//插入换行
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	if(m_ImageDataChat.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_PLAYER,m_ImageDataChat);
	}

	//插入图片
	InsertDataObject(&m_ImageDataChat);

	InsertUserName(pszSendUser);
	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//构造数据
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,255,255);
	CharFormat.yOffset=STRING_OFFSET;

	//插入消息
	InsertString(_T("说: "),CharFormat);
	CharFormat.crTextColor = crColor;
	InsertString(pszString,CharFormat);
	return true;
}


//控件绑定
VOID CChatMessage::PreSubclassWindow()
{
	__super::PreSubclassWindow();
}


//进入事件
bool CChatMessage::InsertUserEnter(LPCTSTR pszUserName)
{
	CString str;
	str.Format(_T("%s 进来了"),pszUserName);
	InsertSystemString(str);
	return true;
}
//离开事件
bool CChatMessage::InsertUserLeave(LPCTSTR pszUserName)
{
	CString str;
	str.Format(_T("%s 离开了"),pszUserName);
	InsertSystemString(str);
	return true;
}
//断线事件
bool CChatMessage::InsertUserOffLine(LPCTSTR pszUserName)
{
	CString str;
	str.Format(_T("%s 断线了，请耐心等候"),pszUserName);
	InsertSystemString(str);
	return true;
}

//普通消息
bool CChatMessage::InsertNormalString(LPCTSTR pszString)
{
	return __super::InsertNormalString(pszString);
}
//系统消息
bool CChatMessage::InsertSystemString(LPCTSTR pszString)
{
	//插入换行
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//加载图片
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_SYSTEM,m_ImageDataSystem);
	}

	//插入图片
	InsertDataObject(&m_ImageDataSystem);

	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//构造数据
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,208,0);
	CharFormat.yOffset=STRING_OFFSET;

	//插入消息
	InsertString(_T(" "),CharFormat);
	InsertString(pszString,CharFormat);

	return true;
}
//提示消息
bool CChatMessage::InsertPromptString(LPCTSTR pszString)
{
	return __super::InsertPromptString(pszString);
}
//公告消息
bool CChatMessage::InsertAfficheString(LPCTSTR pszString)
{
	return __super::InsertAfficheString(pszString);
}

//定制消息
bool CChatMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor)
{
	return __super::InsertCustomString(pszString,crColor);
}
//定制消息
bool CChatMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)
{
	return __super::InsertCustomString(pszString,crColor,crBackColor);
}

//用户表情
bool CChatMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)
{
	//插入换行
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	if(m_ImageDataChat.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_PLAYER,m_ImageDataChat);
	}

	//插入图片
	InsertDataObject(&m_ImageDataChat);
	//插入消息
	InsertUserName(pszSendUser);

	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//构造数据
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,255,255);
	CharFormat.yOffset=STRING_OFFSET;

	//插入消息
	InsertString(TEXT("说："),CharFormat);

	//插入表情
	InsertImage(pszImagePath);	
	return true;
}
//用户表情
bool CChatMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)
{
	return InsertExpression(pszSendUser,pszImagePath);
}
//用户表情
bool CChatMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)
{
	return InsertExpression(pszSendUser,pszImagePath);
}
//用户聊天
bool CChatMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)
{
	return __super::InsertUserChat(pszSendUser,pszRecvUser,pszString,crColor);
}
//用户私聊
bool CChatMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)
{
	return __super::InsertWhisperChat(pszSendUser,pszString,crColor,bMyselfString);
}
//用户私聊
bool CChatMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString, const SYSTEMTIME &time)
{
	return __super::InsertWhisperChat(pszSendUser,pszString,crColor,bMyselfString,time);
}
//用户喇叭
bool CChatMessage::InsertUserTrumpet(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor)
{
	//插入换行
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);
	//加载图片
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_SYSTEM,m_ImageDataSystem);
	}
	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));
	//构造数据
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_NORMAL;
	CharFormat.yOffset=STRING_OFFSET;
	//插入图片
	InsertDataObject(&m_ImageDataSystem);
	//插入消息
	InsertUserName(pszSendUser);
	InsertString(TEXT("发小喇叭 "),CharFormat);

	//加载图片
	if (m_ImageDataTrumpet.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_IMAGE_TRUMPET,m_ImageDataTrumpet);
	}
	//插入数据
	InsertDataObject(&m_ImageDataTrumpet);

	//插入消息
	InsertString(TEXT("说："),CharFormat);
	TranslateString(pszString,crColor);

	return true;
}

//用户喇叭
bool CChatMessage::InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor)
{
	//插入换行
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//加载图片
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_SYSTEM,m_ImageDataSystem);
	}

	//插入图片
	InsertDataObject(&m_ImageDataSystem);
	//插入消息
	InsertUserName(pszSendUser);

	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//构造数据
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_NORMAL;
	CharFormat.yOffset=STRING_OFFSET;

	InsertString(TEXT("发大喇叭 "),CharFormat);

	//加载图片
	if (m_ImageDataTrumpet.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_IMAGE_TYPHON,m_ImageDataTrumpet);
	}
	//插入数据
	InsertDataObject(&m_ImageDataTrumpet);

	//插入消息
	InsertString(TEXT("说："),CharFormat);
	TranslateString(pszString,crColor);
	return true;
}

void CChatMessage::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CRichEditMessage::OnRButtonDown(nFlags, point);
}
//用户名
void CChatMessage::InsertUserName(LPCTSTR pszSendUser)
{
	//变量定义
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//构造数据
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,255,255);
	CharFormat.yOffset=STRING_OFFSET;
	CString str;
	str.Format(_T(" [%s] "),pszSendUser);
	InsertString(str.GetBuffer(0),CharFormat);
}

//翻译字符
bool CChatMessage::TranslateString(LPCTSTR pszString, COLORREF crColor)
{
	//翻译判断
	if (m_pExpressionManager==NULL) return false;

	//变量定义
	tagTranslateResult TranslateResult;
	ZeroMemory(&TranslateResult,sizeof(TranslateResult));

	//字符变量
	INT nTranslateIndex=0;
	INT nStringLength=lstrlen(pszString);

	while ((nStringLength-nTranslateIndex)>0)
	{
		//解释字符
		bool bSuccess=m_pExpressionManager->TranslateString(&pszString[nTranslateIndex],TranslateResult);

		//结果处理
		if (bSuccess==true)
		{
			//插入字符
			if (TranslateResult.nStartPos>0)
			{
				//变量定义
				TCHAR szString[LEN_USER_CHAT]=TEXT("");
				WORD wCopyCount=__min(TranslateResult.nStartPos,CountArray(szString));

				//构造字符
				szString[wCopyCount]=0;
				CopyMemory(szString,&pszString[nTranslateIndex],wCopyCount*sizeof(TCHAR));
				//变量定义
				CHARFORMAT2 CharFormat;
				ZeroMemory(&CharFormat,sizeof(CharFormat));

				//构造数据
				CharFormat.cbSize=sizeof(CharFormat); 
				CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
				CharFormat.crTextColor=crColor;
				CharFormat.yOffset=STRING_OFFSET;
				//插入字符
				InsertString(szString,CharFormat);
			}

			//构造路径
			TCHAR ImagePath[MAX_PATH]=TEXT("");
			TranslateResult.pExpressionItem->GetExpressionPath(ImagePath,CountArray(ImagePath));

			//插入表情
			InsertImage(ImagePath);

			//设置索引
			nTranslateIndex+=TranslateResult.nStartPos+TranslateResult.nDescribeLen;
		}
		else
		{
			//变量定义
			CHARFORMAT2 CharFormat;
			ZeroMemory(&CharFormat,sizeof(CharFormat));

			//构造数据
			CharFormat.cbSize=sizeof(CharFormat); 
			CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
			CharFormat.crTextColor=crColor;
			CharFormat.yOffset=STRING_OFFSET;
			//插入字符
			InsertString(&pszString[nTranslateIndex],CharFormat);
			break;
		}

	}

	return true;
}
