#ifndef USER_ITEM_ELEMENT_HEAD_FILE
#define USER_ITEM_ELEMENT_HEAD_FILE

#pragma once

#include "ShareControlHead.h"

//////////////////////////////////////////////////////////////////////////////////

//接口说明
interface IUserOrderParser;
interface IFaceItemControl;

//////////////////////////////////////////////////////////////////////////////////

//密码等级
#define PASSWORD_LEVEL_0			0									//没有等级
#define PASSWORD_LEVEL_1			1									//没有等级
#define PASSWORD_LEVEL_2			2									//没有等级
#define PASSWORD_LEVEL_3			3									//没有等级

//////////////////////////////////////////////////////////////////////////////////

//用户元素
class SHARE_CONTROL_CLASS CUserItemElement
{
	//等级配置
protected:
	WORD							m_wLevelCount;						//等级数目
	tagGrowLevelConfig *			m_pGrowLevelConfig;					//等级配置
	
	//会员配置
protected:
	WORD							m_wMemberCount;						//配置数目
	tagMemberParameterNew			m_MemberParameter[10];				//会员配置

	//配置变量
protected:
	IUserOrderParser *				m_pIUserOrderParser;				//等级组件
	IFaceItemControl *				m_pIFaceItemControl;				//头像组件

	//静态变量
protected:	
	static CUserItemElement *		m_pUserItemElement;					//对象指针

	//函数定义
public:
	//构造函数
	CUserItemElement();
	//析构函数
	virtual ~CUserItemElement();

	//配置函数
public:
	//设置接口
	VOID SetUserOrderParser(IUserOrderParser * pIUserOrderParser);
	//设置接口
	VOID SetFaceItemControl(IFaceItemControl * pIFaceItemControl);
	//配置等级
	VOID SetGrowLevelConfig(WORD wLevelCount,tagGrowLevelConfig * pGrowLevelConfig);
	//配置会员
	VOID SetMemberParameter(tagMemberParameterNew* pMemberParameter, WORD wMemberCount);

	//获取数据
public:
	//会员配置
	const tagMemberParameterNew* GetUserMemberParameter(const WORD wUserMember);

	//功能函数
public:
	//绘画等级
	VOID DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience);
	//绘画等级
	VOID DrawExperience(CDC * pDC, CRect & rcSubItem, DWORD dwExperience);
	//构造名片
	VOID ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard);

	//等级函数
public:
	//密码等级
	BYTE GetPasswordLevel(LPCTSTR pszPassword);
	//获取等级
	WORD GetExperienceLevel(DWORD dwExperience);	
	//等级信息
	WORD GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount);	
	//等级信息
	WORD GetExperienceInfo(tagGrowLevelConfig GrowLevelConfig[],WORD wBufferCount);

	//合法效验
public:
	//非法验证
	bool EfficacyIllegal(LPCTSTR pszIllegal);
	//汉字验证
	bool EfficacyChineseCharacter(TCHAR chr);
	//效验邮箱
	bool EfficacyEmail(LPCTSTR pszEmail, TCHAR szDescribe[], WORD wMaxCount);	
	//效验帐号
	bool EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount);
	//效验昵称
	bool EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount);
	//效验密码
	bool EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount);	
	//效验证件
	bool EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount);	
	//效验姓名
	bool EfficacyCompellation(LPCTSTR pszCompellation, TCHAR szDescribe[], WORD wMaxCount);	
	//效验手机
	bool EfficacyMobilePhone(LPCTSTR pszMobilePhone, TCHAR szDescribe[], WORD wMaxCount);
	//校验QQ
	bool EfficacyQQ(LPCTSTR pszQQ, TCHAR szDescribe[], WORD wMaxCount); 

	//保护函数
public:
	//名字保护
	VOID ProtectCompellation(LPCTSTR pszPassPortID,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);
	//证件保护
	VOID ProtectPassPortID(LPCTSTR pszPassPortID,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);
	//号码保护
	VOID ProtectSeatPhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);
	//号码保护
	VOID ProtectMobilePhone(LPCTSTR pszModileMuber,TCHAR szResult[],WORD wMaxCount,CHAR MaskChar);

	//辅助函数
public:
	//绘制数字
	VOID DrawNumber(CDC * pDC,CPngImage * pImageNumber,INT nXPos,INT nYPos,LPCTSTR pszNumber,INT nNumber,LPCTSTR pszFormat,UINT nFormat=0);

	//静态函数
public:
	//获取对象
	static CUserItemElement * GetInstance() { return m_pUserItemElement; }
};

//////////////////////////////////////////////////////////////////////////////////

#endif