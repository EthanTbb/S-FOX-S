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


//�û�����
bool CChatMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	if(m_ImageDataChat.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_PLAYER,m_ImageDataChat);
	}

	//����ͼƬ
	InsertDataObject(&m_ImageDataChat);

	InsertUserName(pszSendUser);
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,255,255);
	CharFormat.yOffset=STRING_OFFSET;

	//������Ϣ
	InsertString(_T("˵: "),CharFormat);
	CharFormat.crTextColor = crColor;
	InsertString(pszString,CharFormat);
	return true;
}


//�ؼ���
VOID CChatMessage::PreSubclassWindow()
{
	__super::PreSubclassWindow();
}


//�����¼�
bool CChatMessage::InsertUserEnter(LPCTSTR pszUserName)
{
	CString str;
	str.Format(_T("%s ������"),pszUserName);
	InsertSystemString(str);
	return true;
}
//�뿪�¼�
bool CChatMessage::InsertUserLeave(LPCTSTR pszUserName)
{
	CString str;
	str.Format(_T("%s �뿪��"),pszUserName);
	InsertSystemString(str);
	return true;
}
//�����¼�
bool CChatMessage::InsertUserOffLine(LPCTSTR pszUserName)
{
	CString str;
	str.Format(_T("%s �����ˣ������ĵȺ�"),pszUserName);
	InsertSystemString(str);
	return true;
}

//��ͨ��Ϣ
bool CChatMessage::InsertNormalString(LPCTSTR pszString)
{
	return __super::InsertNormalString(pszString);
}
//ϵͳ��Ϣ
bool CChatMessage::InsertSystemString(LPCTSTR pszString)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//����ͼƬ
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_SYSTEM,m_ImageDataSystem);
	}

	//����ͼƬ
	InsertDataObject(&m_ImageDataSystem);

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,208,0);
	CharFormat.yOffset=STRING_OFFSET;

	//������Ϣ
	InsertString(_T(" "),CharFormat);
	InsertString(pszString,CharFormat);

	return true;
}
//��ʾ��Ϣ
bool CChatMessage::InsertPromptString(LPCTSTR pszString)
{
	return __super::InsertPromptString(pszString);
}
//������Ϣ
bool CChatMessage::InsertAfficheString(LPCTSTR pszString)
{
	return __super::InsertAfficheString(pszString);
}

//������Ϣ
bool CChatMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor)
{
	return __super::InsertCustomString(pszString,crColor);
}
//������Ϣ
bool CChatMessage::InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor)
{
	return __super::InsertCustomString(pszString,crColor,crBackColor);
}

//�û�����
bool CChatMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	if(m_ImageDataChat.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_PLAYER,m_ImageDataChat);
	}

	//����ͼƬ
	InsertDataObject(&m_ImageDataChat);
	//������Ϣ
	InsertUserName(pszSendUser);

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,255,255);
	CharFormat.yOffset=STRING_OFFSET;

	//������Ϣ
	InsertString(TEXT("˵��"),CharFormat);

	//�������
	InsertImage(pszImagePath);	
	return true;
}
//�û�����
bool CChatMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString)
{
	return InsertExpression(pszSendUser,pszImagePath);
}
//�û�����
bool CChatMessage::InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath)
{
	return InsertExpression(pszSendUser,pszImagePath);
}
//�û�����
bool CChatMessage::InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor)
{
	return __super::InsertUserChat(pszSendUser,pszRecvUser,pszString,crColor);
}
//�û�˽��
bool CChatMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString)
{
	return __super::InsertWhisperChat(pszSendUser,pszString,crColor,bMyselfString);
}
//�û�˽��
bool CChatMessage::InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString, const SYSTEMTIME &time)
{
	return __super::InsertWhisperChat(pszSendUser,pszString,crColor,bMyselfString,time);
}
//�û�����
bool CChatMessage::InsertUserTrumpet(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);
	//����ͼƬ
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_SYSTEM,m_ImageDataSystem);
	}
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));
	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_NORMAL;
	CharFormat.yOffset=STRING_OFFSET;
	//����ͼƬ
	InsertDataObject(&m_ImageDataSystem);
	//������Ϣ
	InsertUserName(pszSendUser);
	InsertString(TEXT("��С���� "),CharFormat);

	//����ͼƬ
	if (m_ImageDataTrumpet.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_IMAGE_TRUMPET,m_ImageDataTrumpet);
	}
	//��������
	InsertDataObject(&m_ImageDataTrumpet);

	//������Ϣ
	InsertString(TEXT("˵��"),CharFormat);
	TranslateString(pszString,crColor);

	return true;
}

//�û�����
bool CChatMessage::InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor)
{
	//���뻻��
	LONG lTextLength=GetWindowTextLength();
	if (lTextLength!=0L) InsertString(TEXT("\r\n"),COLOR_EVENT);

	//����ͼƬ
	if (m_ImageDataSystem.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_FLAG_SYSTEM,m_ImageDataSystem);
	}

	//����ͼƬ
	InsertDataObject(&m_ImageDataSystem);
	//������Ϣ
	InsertUserName(pszSendUser);

	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=COLOR_NORMAL;
	CharFormat.yOffset=STRING_OFFSET;

	InsertString(TEXT("�������� "),CharFormat);

	//����ͼƬ
	if (m_ImageDataTrumpet.GetBitmap()==NULL)
	{
		LoadRichEditImage(AfxGetInstanceHandle(),IDB_IMAGE_TYPHON,m_ImageDataTrumpet);
	}
	//��������
	InsertDataObject(&m_ImageDataTrumpet);

	//������Ϣ
	InsertString(TEXT("˵��"),CharFormat);
	TranslateString(pszString,crColor);
	return true;
}

void CChatMessage::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//CRichEditMessage::OnRButtonDown(nFlags, point);
}
//�û���
void CChatMessage::InsertUserName(LPCTSTR pszSendUser)
{
	//��������
	CHARFORMAT2 CharFormat;
	ZeroMemory(&CharFormat,sizeof(CharFormat));

	//��������
	CharFormat.cbSize=sizeof(CharFormat); 
	CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
	CharFormat.crTextColor=RGB(255,255,255);
	CharFormat.yOffset=STRING_OFFSET;
	CString str;
	str.Format(_T(" [%s] "),pszSendUser);
	InsertString(str.GetBuffer(0),CharFormat);
}

//�����ַ�
bool CChatMessage::TranslateString(LPCTSTR pszString, COLORREF crColor)
{
	//�����ж�
	if (m_pExpressionManager==NULL) return false;

	//��������
	tagTranslateResult TranslateResult;
	ZeroMemory(&TranslateResult,sizeof(TranslateResult));

	//�ַ�����
	INT nTranslateIndex=0;
	INT nStringLength=lstrlen(pszString);

	while ((nStringLength-nTranslateIndex)>0)
	{
		//�����ַ�
		bool bSuccess=m_pExpressionManager->TranslateString(&pszString[nTranslateIndex],TranslateResult);

		//�������
		if (bSuccess==true)
		{
			//�����ַ�
			if (TranslateResult.nStartPos>0)
			{
				//��������
				TCHAR szString[LEN_USER_CHAT]=TEXT("");
				WORD wCopyCount=__min(TranslateResult.nStartPos,CountArray(szString));

				//�����ַ�
				szString[wCopyCount]=0;
				CopyMemory(szString,&pszString[nTranslateIndex],wCopyCount*sizeof(TCHAR));
				//��������
				CHARFORMAT2 CharFormat;
				ZeroMemory(&CharFormat,sizeof(CharFormat));

				//��������
				CharFormat.cbSize=sizeof(CharFormat); 
				CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
				CharFormat.crTextColor=crColor;
				CharFormat.yOffset=STRING_OFFSET;
				//�����ַ�
				InsertString(szString,CharFormat);
			}

			//����·��
			TCHAR ImagePath[MAX_PATH]=TEXT("");
			TranslateResult.pExpressionItem->GetExpressionPath(ImagePath,CountArray(ImagePath));

			//�������
			InsertImage(ImagePath);

			//��������
			nTranslateIndex+=TranslateResult.nStartPos+TranslateResult.nDescribeLen;
		}
		else
		{
			//��������
			CHARFORMAT2 CharFormat;
			ZeroMemory(&CharFormat,sizeof(CharFormat));

			//��������
			CharFormat.cbSize=sizeof(CharFormat); 
			CharFormat.dwMask=CFM_COLOR|CFM_OFFSET;
			CharFormat.crTextColor=crColor;
			CharFormat.yOffset=STRING_OFFSET;
			//�����ַ�
			InsertString(&pszString[nTranslateIndex],CharFormat);
			break;
		}

	}

	return true;
}
