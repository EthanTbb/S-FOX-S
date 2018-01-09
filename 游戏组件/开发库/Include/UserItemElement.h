#ifndef USER_ITEM_ELEMENT_HEAD_FILE
#define USER_ITEM_ELEMENT_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//�ӿ�˵��
interface IUserOrderParser;
interface IFaceItemControl;

//////////////////////////////////////////////////////////////////////////////////

//����ȼ�
#define PASSWORD_LEVEL_0			0									//û�еȼ�
#define PASSWORD_LEVEL_1			1									//û�еȼ�
#define PASSWORD_LEVEL_2			2									//û�еȼ�
#define PASSWORD_LEVEL_3			3									//û�еȼ�

//////////////////////////////////////////////////////////////////////////////////

//�û�Ԫ��
class SHARE_CONTROL_CLASS CUserItemElement
{
	//�ȼ�����
protected:
	WORD							m_wLevelCount;						//�ȼ���Ŀ
	tagGrowLevelConfig *			m_pGrowLevelConfig;					//�ȼ�����
	
	//��Ա����
protected:
	WORD							m_wMemberCount;						//������Ŀ
	tagMemberParameterNew			m_MemberParameter[10];				//��Ա����

	//���ñ���
protected:
	IUserOrderParser *				m_pIUserOrderParser;				//�ȼ����
	IFaceItemControl *				m_pIFaceItemControl;				//ͷ�����

	//��̬����
protected:	
	static CUserItemElement *		m_pUserItemElement;					//����ָ��

	//��������
public:
	//���캯��
	CUserItemElement();
	//��������
	virtual ~CUserItemElement();

	//���ú���
public:
	//���ýӿ�
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser);
	//���ýӿ�
	VOID SetFaceItemControl(IFaceItemControl * pIFaceItemControl);
	//���õȼ�
	VOID SetGrowLevelConfig(WORD wLevelCount,tagGrowLevelConfig * pGrowLevelConfig);
	//���û�Ա
	VOID SetMemberParameter(tagMemberParameterNew* pMemberParameter, WORD wMemberCount);

	//��ȡ����
public:
	//��Ա����
	const tagMemberParameterNew* GetUserMemberParameter(const WORD wUserMember);

	//���ܺ���
public:
	//�滭�ȼ�
	VOID DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience);
	//�滭�ȼ�
	VOID DrawExperience(CDC * pDC, CRect & rcSubItem, DWORD dwExperience);
	//������Ƭ
	VOID ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard);

	//�ȼ�����
public:
	//����ȼ�
	BYTE GetPasswordLevel(LPCTSTR pszPassword);
	//��ȡ�ȼ�
	WORD GetExperienceLevel(DWORD dwExperience);	
	//�ȼ���Ϣ
	WORD GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount);	
	//�ȼ���Ϣ
	WORD GetExperienceInfo(tagGrowLevelConfig GrowLevelConfig[],WORD wBufferCount);

	//�Ϸ�Ч��
public:
	//�Ƿ���֤
	bool EfficacyIllegal(LPCTSTR pszIllegal);
	//������֤
	bool EfficacyChineseCharacter(TCHAR chr);
	//Ч������
	bool EfficacyEmail(LPCTSTR pszEmail, TCHAR szDescribe[], WORD wMaxCount);	
	//Ч���ʺ�
	bool EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount);
	//Ч���ǳ�
	bool EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount);
	//Ч������
	bool EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount);	
	//Ч��֤��
	bool EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount);	
	//Ч������
	bool EfficacyCompellation(LPCTSTR pszCompellation, TCHAR szDescribe[], WORD wMaxCount);	
	//Ч���ֻ�
	bool EfficacyMobilePhone(LPCTSTR pszMobilePhone, TCHAR szDescribe[], WORD wMaxCount);
	//У��QQ
	bool EfficacyQQ(LPCTSTR pszQQ, TCHAR szDescribe[], WORD wMaxCount); 

	//��������
public:
	//���ֱ���
	VOID ProtectCompellation(LPCTSTR pszPassPortID,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);
	//֤������
	VOID ProtectPassPortID(LPCTSTR pszPassPortID,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);
	//���뱣��
	VOID ProtectSeatPhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);
	//���뱣��
	VOID ProtectMobilePhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);

	//��������
public:
	//��������
	VOID DrawNumber(CDC * pDC,CPngImage * pImageNumber,INT nXPos,INT nYPos,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat=0);

	//��̬����
public:
	//��ȡ����
	static CUserItemElement * GetInstance() { return m_pUserItemElement; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif