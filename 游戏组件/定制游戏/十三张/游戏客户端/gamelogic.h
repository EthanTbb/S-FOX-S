#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////	

//扑克类型

#define CT_INVALID						0								//错误类型
#define CT_SINGLE						1								//单牌类型
#define CT_ONE_DOUBLE					2								//只有一对
#define CT_TWO_DOUBLE					3								//两对牌型
#define CT_THREE						4								//三张牌型
#define CT_FIVE_MIXED_FLUSH_FIRST_A		5								//A前顺子
#define CT_FIVE_MIXED_FLUSH_NO_A		6								//普通顺子
#define CT_FIVE_MIXED_FLUSH_BACK_A		7								//A后顺子
#define CT_FIVE_FLUSH					8								//同花
#define CT_FIVE_THREE_DEOUBLE			9								//三条一对
#define CT_FIVE_FOUR_ONE				10								//四带一张
#define CT_FIVE_STRAIGHT_FLUSH_FIRST_A	11								//A同花顺
#define CT_FIVE_STRAIGHT_FLUSH			12								//同花顺牌

//特殊类型
#define CT_EX_INVALID					14								//非特殊牌
#define CT_EX_SANTONGHUA				15								//三同花
#define CT_EX_SANSHUNZI					16								//三顺子
#define CT_EX_LIUDUIBAN					17								//六对半
#define CT_EX_WUDUISANTIAO				18								//五对三条
#define CT_EX_SITAOSANTIAO				19								//四套三条
#define CT_EX_SHUANGGUAICHONGSAN		20								//双怪冲三
#define CT_EX_COUYISE					21								//凑一色
#define CT_EX_QUANXIAO					22								//全小
#define CT_EX_QUANDA					23								//全大
#define CT_EX_SANFENGTIANXIA			24								//三分天下
#define CT_EX_SANTONGHUASHUN			25								//三同花顺
#define CT_EX_SHIERHUANGZU				26								//十二皇族
#define CT_EX_YITIAOLONG				27								//一条龙
#define CT_EX_ZHIZUNQINGLONG			28								//至尊清龙

//数值掩码
#define	LOGIC_MASK_COLOR				0xF0							//花色掩码
#define	LOGIC_MASK_VALUE				0x0F							//数值掩码

//分析结构
struct tagAnalyseData
{
	BYTE							bOneCount;								//单张数目
	BYTE							bTwoCount;								//两张数目 
	BYTE							bThreeCount;							//三张数目
	BYTE							bFourCount;								//四张数目
	BYTE							bOneFirst[13];							//单牌位置
	BYTE							bTwoFirst[13];							//对牌位置
	BYTE							bThreeFirst[13];						//三条位置
	BYTE							bFourFirst[13];							//四张位置
	bool							bSameColor;								//是否同花
};

//分析结构
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];						//同牌数目
	BYTE							cbCardData[4][13];						//扑克列表
};

//分布信息
struct tagDistributing
{
	BYTE							cbCardCount;						//扑克数目
	BYTE							cbDistributing[13][5];				//分布信息
};

//搜索结果
struct tagSearchCardResult
{
	BYTE							cbSearchCount;						//结果数目
	BYTE							cbCardCount[13];					//扑克数目
	BYTE							cbResultCard[13][13];				//扑克列表
};

//转化数据
struct tagTransData
{
	BYTE							bKingCount;							//王牌数目
	BYTE							bHaveKing;							//是否有王	(0 无 1 小王 2 大王 3 大小王)		
	CList<BYTE>						transList;							//转化成的数字链表
	tagTransData()
	{
		bKingCount = 0;
		bHaveKing = 0;
		transList.RemoveAll();
	}
};

enum COMRESULT
{
	enCRLess = -1,
	enCREqual,
	enCRGreater,
	enCRError
};

//排列类型
enum enSortCardType
{
	enDescend,																//降序类型 
	enAscend,																//升序类型
	enColor																	//花色类型
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////	
//调试函数
static void MyMsgBox(LPCSTR lpszFormat, ...)
{
//	va_list args;
//	int		nBuf;
//	TCHAR	szBuffer[512];
//
//	va_start(args, lpszFormat);
//	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
//#ifdef _DEBUG
//	AfxMessageBox(szBuffer);
//#else
//	TCHAR	szFormat[512];
//	_stprintf(szFormat, _T("[JJ]: %s"), szBuffer);
//	OutputDebugString(szFormat);
//#endif
//
//	va_end(args);
}

static void _DebugOutPut(LPWSTR lpszFormat, ...)
{
//#ifdef _DEBUG
//
//	va_list args;
//	int		nBuf;
//	TCHAR	szBuffer[1024];
//	TCHAR	szFormat[1024];
//	TCHAR   szTime[1024];
//
//	
//	
//	time_t timer = time(NULL);  
//	wcsftime(szTime, 1024, TEXT("%Y-%m-%d %H:%M:%S"), localtime(&timer)); 
//
//	va_start(args, lpszFormat);
//	nBuf = _vsntprintf(szBuffer, sizeof(szBuffer)-sizeof(LPWSTR), lpszFormat, args);
//	_stprintf(szFormat, _T("[Server]%s: %s"), szTime, szBuffer);
//	va_end(args);
//
//	OutputDebugString(szFormat);
//
//#endif
	/*CStdioFile file;
	file.Open(TEXT("ServerThirteen.log"),CFile::modeCreate|CFile::modeReadWrite);
	CString str;
	str.Format(TEXT("%s\n"),szFormat);
	file.Seek(0,CFile::end);
	file.WriteString( str );
	file.Close();*/
}
//////////////////////////////////////////////////////////////////////////


//游戏逻辑类
class CGameLogic
{
	//变量定义
protected:
	static const BYTE				m_bCardListData[52];				//扑克数据

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取类型
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount, BYTE & bMaxCardData);
	//特殊牌型
	BYTE GetSpecialType(BYTE bHandCardData[], BYTE bCardCount);

	BYTE GetSpecialCardData(BYTE bHandCardData[13], BYTE bHandCardCount, BYTE bSpecCardData[3][5]);
	//获取数值
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //十六进制前面四位表示牌的数值
	//获取花色
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //十六进制后面四位表示牌的花色 

	//控制函数
public:
	//排列扑克
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//混乱扑克
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//删除扑克
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);
	
    /*
	@此函数进行优化
	@优化日期2014-3-21
	*/
	//自动摆牌
	void AutoPutCard(BYTE bCardData[], BYTE bPutCardData[], bool bIsAndroidUser, bool bTest);

	//逻辑函数
public:
	//逻辑数值
	BYTE GetCardLogicValue(BYTE bCardData);
	////对比扑克
	//bool CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//对比扑克
	int CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//检查龙牌
	bool IsStraightDragon(const BYTE cbCardData[], BYTE bCardCount);
	//是否顺子
	bool IsLinkCard(const BYTE cbCardData[], BYTE cbCardCount);
	//是否同花
	bool IsSameColorCard(const BYTE cbCardData[], BYTE cbCardCount);
	//获取对数
	BYTE GetDoubleCount(BYTE cbFrontCard[], BYTE cbMidCard[], BYTE cbBackCard[]);

	//功能函数
public:
	//分析扑克
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) ;
	//分析扑克
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) ;
	//分析分布
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
	//王牌转换
	BYTE AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTransData & TransData);
	//王牌转换
	void TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, 
		const tagAnalyseData & tad, BYTE bTransCardData[], tagTransData & TransData);
	//单张比较
	bool CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTransData & ttdFst, tagTransData & ttdNxt);
	//抽取大牌
	BYTE GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE cbMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount = 5);
	//构造扑克
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
    
	//搜索同牌
	BYTE SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount,BYTE cbSameCardCount, tagSearchCardResult *pSearchCardResult );
	//搜索带牌(三带一，四带一等)
	BYTE SearchTakeCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, BYTE cbTakeCardCount, tagSearchCardResult *pSearchCardResult );
	//搜索同花
	BYTE SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, tagSearchCardResult *pSearchCardResult);
	//搜索连牌
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult);
	//搜索同花顺
	BYTE SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult);

public:

	/*
	@以下函数为机器人控制做出的更改
	@修改日期2014-3-21
	*/
	//牌类型转换权值
	int CardTypeToWeights(BYTE bType, BYTE cbIndex);
	/*
	@以下函数为配合自动摆牌函数（AutoPutCard ）使用的更改
	@修改日期2014-3-21
	*/
	//优化组合牌型
	void OptimizationCombo(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
