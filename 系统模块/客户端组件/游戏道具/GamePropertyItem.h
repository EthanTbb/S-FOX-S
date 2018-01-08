#ifndef GAME_PROPERTY_ITEM_HEAD_FILE
#define GAME_PROPERTY_ITEM_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyBase
{
	//��Ԫ����
	friend class CGamePropertyManager;

	//��������
protected:
	tagPropertyItem					m_PropertyItem;						//������Ϣ
	tagPropertyAttrib				m_PropertyAttrib;					//��������

	//��������
public:
	//���캯��
	CGamePropertyBase();
	//��������
	virtual ~CGamePropertyBase();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage)=NULL;
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);

	//���ܺ���
public:
	//��ȡ��Ϣ
	tagPropertyItem * GetPropertyItem() { return &m_PropertyItem; }
	//��ȡ����
	tagPropertyAttrib * GetPropertyAttrib() { return & m_PropertyAttrib; }

	//���ܺ���
public:
	//��ʾ����
	VOID ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink);
	//����ʱ��
	VOID OnEventPerformCommand(WORD wCommandID,VOID * pCmdData);
};

//////////////////////////////////////////////////////////////////////////////////
//���ﲿ��

//��������
class GAME_PROPERTY_CLASS CGamePropertyCar : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyCar();
	//��������
	virtual ~CGamePropertyCar();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyEgg : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyEgg();
	//��������
	virtual ~CGamePropertyEgg();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyClap : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyClap();
	//��������
	virtual ~CGamePropertyClap();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyKiss : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyKiss();
	//��������
	virtual ~CGamePropertyKiss();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//ơ������
class GAME_PROPERTY_CLASS CGamePropertyBeer : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyBeer();
	//��������
	virtual ~CGamePropertyBeer();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyCake : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyCake();
	//��������
	virtual ~CGamePropertyCake();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//�������
class GAME_PROPERTY_CLASS CGamePropertyRing : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyRing();
	//��������
	virtual ~CGamePropertyRing();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyBeat : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyBeat();
	//��������
	virtual ~CGamePropertyBeat();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//ը������
class GAME_PROPERTY_CLASS CGamePropertyBomb : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyBomb();
	//��������
	virtual ~CGamePropertyBomb();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertySmoke : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertySmoke();
	//��������
	virtual ~CGamePropertySmoke();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyVilla : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyVilla();
	//��������
	virtual ~CGamePropertyVilla();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//שͷ����
class GAME_PROPERTY_CLASS CGamePropertyBrick : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyBrick();
	//��������
	virtual ~CGamePropertyBrick();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//�ʻ�����
class GAME_PROPERTY_CLASS CGamePropertyFlower : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyFlower();
	//��������
	virtual ~CGamePropertyFlower();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////
//���߲���

//2������
class GAME_PROPERTY_CLASS CGamePropertyTwoScoreCard : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyTwoScoreCard();
	//��������
	virtual ~CGamePropertyTwoScoreCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//4������
class GAME_PROPERTY_CLASS CGamePropertyFourScoreCard : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyFourScoreCard();
	//��������
	virtual ~CGamePropertyFourScoreCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyScoreClear : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyScoreClear();
	//��������
	virtual ~CGamePropertyScoreClear();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class GAME_PROPERTY_CLASS CGamePropertyEscapeClear : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyEscapeClear();
	//��������
	virtual ~CGamePropertyEscapeClear();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//С����
class GAME_PROPERTY_CLASS CGamePropertyTrumpet  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyTrumpet();
	//��������
	virtual ~CGamePropertyTrumpet();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//������
class GAME_PROPERTY_CLASS CGamePropertyTyphon  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyTyphon();
	//��������
	virtual ~CGamePropertyTyphon();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//���߿�
class GAME_PROPERTY_CLASS CGamePropertyGuardKickCard  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyGuardKickCard();
	//��������
	virtual ~CGamePropertyGuardKickCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//�����
class GAME_PROPERTY_CLASS CGamePropertyPossess  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyPossess();
	//��������
	virtual ~CGamePropertyPossess();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


//////////////////////////////////////////////////////////////////////////////////

//���꿨
class GAME_PROPERTY_CLASS CGamePropertyBlueRingCard  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyBlueRingCard();
	//��������
	virtual ~CGamePropertyBlueRingCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//���꿨
class GAME_PROPERTY_CLASS CGamePropertyYellowRingCard  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyYellowRingCard();
	//��������
	virtual ~CGamePropertyYellowRingCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//���꿨
class GAME_PROPERTY_CLASS CGamePropertyWhiteRingCard  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyWhiteRingCard();
	//��������
	virtual ~CGamePropertyWhiteRingCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//���꿨
class GAME_PROPERTY_CLASS CGamePropertyRedRingCard  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyRedRingCard();
	//��������
	virtual ~CGamePropertyRedRingCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


//////////////////////////////////////////////////////////////////////////////////

//Vip����
class GAME_PROPERTY_CLASS CGamePropertyVipRoomCard  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyVipRoomCard();
	//��������
	virtual ~CGamePropertyVipRoomCard();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//ˮ��
class GAME_PROPERTY_CLASS CGamePropertyCrystal  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyCrystal();
	//��������
	virtual ~CGamePropertyCrystal();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////////
//���
class GAME_PROPERTY_CLASS CGamePropertyAgate  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyAgate();
	//��������
	virtual ~CGamePropertyAgate();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//��ʯ
class GAME_PROPERTY_CLASS CGamePropertyBoulder  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyBoulder();
	//��������
	virtual ~CGamePropertyBoulder();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//��ʯ
class GAME_PROPERTY_CLASS CGamePropertyGem  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyGem();
	//��������
	virtual ~CGamePropertyGem();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//��ʯ
class GAME_PROPERTY_CLASS CGamePropertyDiamond  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyDiamond();
	//��������
	virtual ~CGamePropertyDiamond();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//1Сʱ˫�����ֿ�
class GAME_PROPERTY_CLASS CGamePropertyDouble1Hour  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyDouble1Hour();
	//��������
	virtual ~CGamePropertyDouble1Hour();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//3Сʱ˫�����ֿ�
class GAME_PROPERTY_CLASS CGamePropertyDouble3Hour  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyDouble3Hour();
	//��������
	virtual ~CGamePropertyDouble3Hour();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1��˫�����ֿ�
class GAME_PROPERTY_CLASS CGamePropertyDouble1Day  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyDouble1Day();
	//��������
	virtual ~CGamePropertyDouble1Day();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1��˫�����ֿ�
class GAME_PROPERTY_CLASS CGamePropertyDouble1Week  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyDouble1Week();
	//��������
	virtual ~CGamePropertyDouble1Week();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//1Сʱ����
class GAME_PROPERTY_CLASS CGamePropertyAmulet1Hour  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyAmulet1Hour();
	//��������
	virtual ~CGamePropertyAmulet1Hour();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//3Сʱ����
class GAME_PROPERTY_CLASS CGamePropertyAmulet3Hour  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyAmulet3Hour();
	//��������
	virtual ~CGamePropertyAmulet3Hour();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1�����
class GAME_PROPERTY_CLASS CGamePropertyAmulet1Day  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyAmulet1Day();
	//��������
	virtual ~CGamePropertyAmulet1Day();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1�ܷ���
class GAME_PROPERTY_CLASS CGamePropertyAmulet1Week  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyAmulet1Week();
	//��������
	virtual ~CGamePropertyAmulet1Week();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};



/////////////////////////////////////////////////////////////////////////////////////////
//1Сʱ���߿� 
class GAME_PROPERTY_CLASS CGamePropertyGuardKick1Hour  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyGuardKick1Hour();
	//��������
	virtual ~CGamePropertyGuardKick1Hour();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//3Сʱ���߿�
class GAME_PROPERTY_CLASS CGamePropertyGuardKick3Hour  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyGuardKick3Hour();
	//��������
	virtual ~CGamePropertyGuardKick3Hour();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1����߿�
class GAME_PROPERTY_CLASS CGamePropertyGuardKick1Day  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyGuardKick1Day();
	//��������
	virtual ~CGamePropertyGuardKick1Day();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1�ܷ��߿�
class GAME_PROPERTY_CLASS CGamePropertyGuardKick1Week  : public CGamePropertyBase
{
	//��������
public:
	//���캯��
	CGamePropertyGuardKick1Week();
	//��������
	virtual ~CGamePropertyGuardKick1Week();

	//���غ���
public:
	//ͼƬ��Ϣ
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//��ʾ��Ϣ
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};
#endif