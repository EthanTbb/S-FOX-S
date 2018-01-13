#pragma once

enum enMessageType
{
	enSystem=0,
	enChat,
	enNormalString,
	enPrompt,
	enAffiche,
	enCustom,
	enWhisperChat,
	enUserTrumpet,
	enUserTyphon,
	enExpression
};

struct ChatMsg
{
	LPCTSTR pszSendUser;					//������
	LPCTSTR pszString;						//���͵���Ϣ
	COLORREF crColor;						//������ɫ
};


class CStringMessage : public IStringMessage
{

public:
	CStringMessage(void);
	~CStringMessage(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release();
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//�¼���Ϣ
public:
	//�����¼�
	virtual bool InsertUserEnter(LPCTSTR pszUserName);
	//�뿪�¼�
	virtual bool InsertUserLeave(LPCTSTR pszUserName);
	//�����¼�
	virtual bool InsertUserOffLine(LPCTSTR pszUserName);

	//�ַ���Ϣ
public:
	//��ͨ��Ϣ
	virtual bool InsertNormalString(LPCTSTR pszString);
	//ϵͳ��Ϣ
	virtual bool InsertSystemString(LPCTSTR pszString);
	//��ʾ��Ϣ
	virtual bool InsertPromptString(LPCTSTR pszString);
	//������Ϣ
	virtual bool InsertAfficheString(LPCTSTR pszString);

	//������Ϣ
public:
	//������Ϣ
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor);
	//������Ϣ
	virtual bool InsertCustomString(LPCTSTR pszString, COLORREF crColor, COLORREF crBackColor);

	//������Ϣ
public:
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath);
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszImagePath, bool bMyselfString);
	//�û�����
	virtual bool InsertExpression(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszImagePath);

	//������Ϣ
public:
	//�û�����
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor);
	//�û�����
	virtual bool InsertUserChat(LPCTSTR pszSendUser, LPCTSTR pszRecvUser, LPCTSTR pszString, COLORREF crColor);
	//�û�˽��
	virtual bool InsertWhisperChat(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor, bool bMyselfString);
	//�û�����
	virtual bool InsertUserTrumpet(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor);
	//�û�����
	virtual bool InsertUserTyphon(LPCTSTR pszSendUser,LPCTSTR pszString,COLORREF crColor);
//�ڲ�����
private:
	void SendMessageToEngine(LPCTSTR pszSendUser, LPCTSTR pszString, COLORREF crColor,int nMessageType);

};
