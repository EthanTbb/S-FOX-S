#ifndef GAME_PROPERTY_ITEM_HEAD_FILE
#define GAME_PROPERTY_ITEM_HEAD_FILE

#pragma once

#include "GamePropertyHead.h"

//////////////////////////////////////////////////////////////////////////////////

//道具子项
class GAME_PROPERTY_CLASS CGamePropertyBase
{
	//友元定义
	friend class CGamePropertyManager;

	//变量定义
protected:
	tagPropertyItem					m_PropertyItem;						//道具信息
	tagPropertyAttrib				m_PropertyAttrib;					//道具属性

	//函数定义
public:
	//构造函数
	CGamePropertyBase();
	//析构函数
	virtual ~CGamePropertyBase();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage)=NULL;
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);

	//功能函数
public:
	//获取信息
	tagPropertyItem * GetPropertyItem() { return &m_PropertyItem; }
	//获取属性
	tagPropertyAttrib * GetPropertyAttrib() { return & m_PropertyAttrib; }

	//功能函数
public:
	//显示购买
	VOID ShowBuyPropertyWnd(IGamePropertySink * pIGamePropertySink);
	//命令时间
	VOID OnEventPerformCommand(WORD wCommandID,VOID * pCmdData);
};

//////////////////////////////////////////////////////////////////////////////////
//礼物部分

//汽车礼物
class GAME_PROPERTY_CLASS CGamePropertyCar : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyCar();
	//析构函数
	virtual ~CGamePropertyCar();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//臭蛋礼物
class GAME_PROPERTY_CLASS CGamePropertyEgg : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyEgg();
	//析构函数
	virtual ~CGamePropertyEgg();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//鼓掌礼物
class GAME_PROPERTY_CLASS CGamePropertyClap : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyClap();
	//析构函数
	virtual ~CGamePropertyClap();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//香吻礼物
class GAME_PROPERTY_CLASS CGamePropertyKiss : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyKiss();
	//析构函数
	virtual ~CGamePropertyKiss();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//啤酒礼物
class GAME_PROPERTY_CLASS CGamePropertyBeer : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyBeer();
	//析构函数
	virtual ~CGamePropertyBeer();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//蛋糕礼物
class GAME_PROPERTY_CLASS CGamePropertyCake : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyCake();
	//析构函数
	virtual ~CGamePropertyCake();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//钻戒礼物
class GAME_PROPERTY_CLASS CGamePropertyRing : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyRing();
	//析构函数
	virtual ~CGamePropertyRing();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//暴打礼物
class GAME_PROPERTY_CLASS CGamePropertyBeat : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyBeat();
	//析构函数
	virtual ~CGamePropertyBeat();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//炸弹礼物
class GAME_PROPERTY_CLASS CGamePropertyBomb : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyBomb();
	//析构函数
	virtual ~CGamePropertyBomb();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//香烟礼物
class GAME_PROPERTY_CLASS CGamePropertySmoke : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertySmoke();
	//析构函数
	virtual ~CGamePropertySmoke();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//别墅礼物
class GAME_PROPERTY_CLASS CGamePropertyVilla : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyVilla();
	//析构函数
	virtual ~CGamePropertyVilla();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//砖头礼物
class GAME_PROPERTY_CLASS CGamePropertyBrick : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyBrick();
	//析构函数
	virtual ~CGamePropertyBrick();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//鲜花礼物
class GAME_PROPERTY_CLASS CGamePropertyFlower : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyFlower();
	//析构函数
	virtual ~CGamePropertyFlower();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////
//道具部分

//2倍积分
class GAME_PROPERTY_CLASS CGamePropertyTwoScoreCard : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyTwoScoreCard();
	//析构函数
	virtual ~CGamePropertyTwoScoreCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//4倍积分
class GAME_PROPERTY_CLASS CGamePropertyFourScoreCard : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyFourScoreCard();
	//析构函数
	virtual ~CGamePropertyFourScoreCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//负分清零
class GAME_PROPERTY_CLASS CGamePropertyScoreClear : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyScoreClear();
	//析构函数
	virtual ~CGamePropertyScoreClear();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//逃跑清零
class GAME_PROPERTY_CLASS CGamePropertyEscapeClear : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyEscapeClear();
	//析构函数
	virtual ~CGamePropertyEscapeClear();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//小喇叭
class GAME_PROPERTY_CLASS CGamePropertyTrumpet  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyTrumpet();
	//析构函数
	virtual ~CGamePropertyTrumpet();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//大喇叭
class GAME_PROPERTY_CLASS CGamePropertyTyphon  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyTyphon();
	//析构函数
	virtual ~CGamePropertyTyphon();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//防踢卡
class GAME_PROPERTY_CLASS CGamePropertyGuardKickCard  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyGuardKickCard();
	//析构函数
	virtual ~CGamePropertyGuardKickCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//护身符
class GAME_PROPERTY_CLASS CGamePropertyPossess  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyPossess();
	//析构函数
	virtual ~CGamePropertyPossess();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


//////////////////////////////////////////////////////////////////////////////////

//蓝钻卡
class GAME_PROPERTY_CLASS CGamePropertyBlueRingCard  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyBlueRingCard();
	//析构函数
	virtual ~CGamePropertyBlueRingCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//黄钻卡
class GAME_PROPERTY_CLASS CGamePropertyYellowRingCard  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyYellowRingCard();
	//析构函数
	virtual ~CGamePropertyYellowRingCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//白钻卡
class GAME_PROPERTY_CLASS CGamePropertyWhiteRingCard  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyWhiteRingCard();
	//析构函数
	virtual ~CGamePropertyWhiteRingCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//红钻卡
class GAME_PROPERTY_CLASS CGamePropertyRedRingCard  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyRedRingCard();
	//析构函数
	virtual ~CGamePropertyRedRingCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


//////////////////////////////////////////////////////////////////////////////////

//Vip房卡
class GAME_PROPERTY_CLASS CGamePropertyVipRoomCard  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyVipRoomCard();
	//析构函数
	virtual ~CGamePropertyVipRoomCard();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage);
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////

//水晶
class GAME_PROPERTY_CLASS CGamePropertyCrystal  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyCrystal();
	//析构函数
	virtual ~CGamePropertyCrystal();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

//////////////////////////////////////////////////////////////////////////////////////
//玛瑙
class GAME_PROPERTY_CLASS CGamePropertyAgate  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyAgate();
	//析构函数
	virtual ~CGamePropertyAgate();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//玉石
class GAME_PROPERTY_CLASS CGamePropertyBoulder  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyBoulder();
	//析构函数
	virtual ~CGamePropertyBoulder();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//宝石
class GAME_PROPERTY_CLASS CGamePropertyGem  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyGem();
	//析构函数
	virtual ~CGamePropertyGem();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//钻石
class GAME_PROPERTY_CLASS CGamePropertyDiamond  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyDiamond();
	//析构函数
	virtual ~CGamePropertyDiamond();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//1小时双倍积分卡
class GAME_PROPERTY_CLASS CGamePropertyDouble1Hour  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyDouble1Hour();
	//析构函数
	virtual ~CGamePropertyDouble1Hour();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//3小时双倍积分卡
class GAME_PROPERTY_CLASS CGamePropertyDouble3Hour  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyDouble3Hour();
	//析构函数
	virtual ~CGamePropertyDouble3Hour();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1天双倍积分卡
class GAME_PROPERTY_CLASS CGamePropertyDouble1Day  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyDouble1Day();
	//析构函数
	virtual ~CGamePropertyDouble1Day();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1周双倍积分卡
class GAME_PROPERTY_CLASS CGamePropertyDouble1Week  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyDouble1Week();
	//析构函数
	virtual ~CGamePropertyDouble1Week();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//1小时防身卡
class GAME_PROPERTY_CLASS CGamePropertyAmulet1Hour  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyAmulet1Hour();
	//析构函数
	virtual ~CGamePropertyAmulet1Hour();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//3小时防身卡
class GAME_PROPERTY_CLASS CGamePropertyAmulet3Hour  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyAmulet3Hour();
	//析构函数
	virtual ~CGamePropertyAmulet3Hour();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1天防身卡
class GAME_PROPERTY_CLASS CGamePropertyAmulet1Day  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyAmulet1Day();
	//析构函数
	virtual ~CGamePropertyAmulet1Day();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1周防身卡
class GAME_PROPERTY_CLASS CGamePropertyAmulet1Week  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyAmulet1Week();
	//析构函数
	virtual ~CGamePropertyAmulet1Week();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};



/////////////////////////////////////////////////////////////////////////////////////////
//1小时防踢卡 
class GAME_PROPERTY_CLASS CGamePropertyGuardKick1Hour  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyGuardKick1Hour();
	//析构函数
	virtual ~CGamePropertyGuardKick1Hour();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};

/////////////////////////////////////////////////////////////////////////////////////////
//3小时防踢卡
class GAME_PROPERTY_CLASS CGamePropertyGuardKick3Hour  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyGuardKick3Hour();
	//析构函数
	virtual ~CGamePropertyGuardKick3Hour();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1天防踢卡
class GAME_PROPERTY_CLASS CGamePropertyGuardKick1Day  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyGuardKick1Day();
	//析构函数
	virtual ~CGamePropertyGuardKick1Day();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};


/////////////////////////////////////////////////////////////////////////////////////////
//1周防踢卡
class GAME_PROPERTY_CLASS CGamePropertyGuardKick1Week  : public CGamePropertyBase
{
	//函数定义
public:
	//构造函数
	CGamePropertyGuardKick1Week();
	//析构函数
	virtual ~CGamePropertyGuardKick1Week();

	//重载函数
public:
	//图片信息
	virtual VOID GetImageInfo(tagPropertyImage & PropertyImage){};
	//提示信息
	virtual LPCTSTR GetNotifyMessage(LPCTSTR szSourceNickName,LPCTSTR szTargerNickName,WORD wItemCount,LPTSTR szResultMessage);
};
#endif