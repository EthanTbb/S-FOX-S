#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////	

//调试函数
static VOID MyMsgBox(LPCSTR lpszFormat, ...)
{
	va_list args;
	//int		nBuf;
	TCHAR	szBuffer[512];

	va_start(args, lpszFormat);
	//nBuf = myprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
#ifdef _DEBUG
	AfxMessageBox(szBuffer);
#else
	TCHAR	szFormat[512];
	myprintf(szFormat, sizeof(szFormat), _T("[JJ][SB]: %s"), szBuffer);
	OutputDebugString(szFormat);
#endif

	va_end(args);
}

static bool MyDebug(LPCSTR lpszFormat, ...)
{
	va_list args;
	//int		nBuf;
	TCHAR	szBuffer[512];
	TCHAR	szFormat[512];

	va_start(args, lpszFormat);
	//nBuf = myprintf(szBuffer, sizeof(szBuffer)-sizeof(TCHAR), lpszFormat, args);
	myprintf(szFormat,sizeof(szFormat), _T("[JJ][SB]: %s"), szBuffer);
	OutputDebugString(szFormat);

	va_end(args);

	return true;
}


static CString GetFileDialogPath()
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//模块目录
	GetModuleFileName(AfxGetInstanceHandle(),szModuleDirectory,sizeof(szModuleDirectory));
	int nModuleLen=lstrlen(szModuleDirectory);
	int nProcessLen=lstrlen(AfxGetApp()->m_pszExeName)+lstrlen(TEXT(".EXE")) + 1;
	if (nModuleLen<=nProcessLen) 
		return TEXT("");
	szModuleDirectory[nModuleLen-nProcessLen]=0;
	strFileDlgPath = szModuleDirectory;
	return strFileDlgPath;
}

static void NcaTextOut(CString strInfo)
{
	CString strName = GetFileDialogPath()+	"\\SicBoBattle.log";
	CTime time = CTime::GetCurrentTime() ;
	CString strTime ;
	strTime.Format( TEXT( "%d-%d-%d %d:%d:%d" ) ,
		time.GetYear() ,
		time.GetMonth() ,
		time.GetDay() ,
		time.GetHour() ,
		time.GetMinute() ,
		time.GetSecond()
		);
	CString strMsg;
	strMsg.Format(TEXT("%s,%s\r\n"),strTime,strInfo);

	CFile fLog;
	if(fLog.Open( strName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite ))
	{

		fLog.SeekToEnd(); 	
		int strLength=strMsg.GetLength();
#ifdef _UNICODE
		BYTE bom[2] = {0xff, 0xfe};
		fLog.Write(bom,sizeof(BYTE)*2);
		strLength*=2;
#endif
		fLog.Write((LPCTSTR)strMsg,strLength);
		fLog.Close();
	}

}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//牌型
struct tagDiceMutiple
{
	BYTE cbAreaIndex;
	int	 nMutiple;
};

static const tagDiceMutiple DiceMutiple[6][6]=
{
	{{2,8} ,{29,5},{30,5},{31,5},{32,5},{33,5}},
	{{29,5},{3,8} ,{34,5},{35,5},{36,5},{37,5}},
	{{30,5},{34,5},{4,8} ,{38,5},{39,5},{40,5}},
	{{31,5},{35,5},{38,5},{5,8} ,{41,5},{42,5}},
	{{32,5},{36,5},{39,5},{41,5},{6,8} ,{43,5}},
	{{33,5},{37,5},{40,5},{42,5},{43,5},{7,8} }
};

static const tagDiceMutiple DiceMutipleEx[14]=
{
	{15,50},{16,18},{17,14},{18,12},{19,8},
	{20,6},{21,6} ,{22,6},{23,6},{24,8},
	{25,12},{26,14},{27,18} ,{28,50}
};



enum emCardType
{
	CT_PAIR_TEN		=		10,				//对十
	CT_PAIR_NIGHT	=		9,				//对九
	CT_PAIR_EIGHT	=		8,				//对八
	CT_PAIR_SEVEN	=		7,				//对七
	CT_PAIR_SISEX	=		6,				//对六
	CT_PAIR_FIVE	=		5,				//对五
	CT_PAIR_FOUR	=		4,				//对四
	CT_PAIR_THREE	=		3,				//对三
	CT_PAIR_TWO		=		2,				//对二
	CT_PAIR_ONE		=		1,				//对一
	CT_POINT		=		0,				//点数类型
	CT_PAIR			=		11

};
		
//数值掩码
#define	LOGIC_MASK_COLOR			0xF0								//花色掩码
#define	LOGIC_MASK_VALUE			0x0F								//数值掩码

//排序类型
#define	ST_VALUE					1									//数值排序
#define	ST_NEW					    3									//数值排序
#define	ST_LOGIC					2									//逻辑排序

//////////////////////////////////////////////////////////////////////////

struct tagAnalyseResult
{
	struct tagDiceDate
	{
		BYTE cbDate;
		BYTE cbCount;
	} m_DiceType[6];
	BYTE cbUnEqualDice;
	BYTE cbAllDiceValue;
};




//游戏逻辑
class CGameLogic
{
	//变量定义
private:
	static const BYTE				m_cbCardListData[ALL_CARD_COUNT];		//扑克定义

	//函数定义
public:
	//构造函数
	CGameLogic();
	//析构函数
	virtual ~CGameLogic();

	//类型函数
public:
	//获取数值
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
		return cbCardData&LOGIC_MASK_VALUE; 
	}
	//获取花色
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//控制函数
public:
	//混乱扑克
	VOID RandCardList(BYTE cbCardBuffer[], BYTE cbBufferCount);
	//排列扑克
	VOID SortCardList(BYTE cbCardData[], BYTE cbCardCount, BYTE cbSortType);

	//逻辑函数
public:
	//推断赢家
	VOID DeduceWinner(int *cbWinMultiple,BYTE *cbDiceValue);
	//获取牌点
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//获取牌型
	BYTE GetCardType(const BYTE cbCardData[], BYTE cbCardCount);
	//是否相等
	bool EqualCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[]);
	//大小比较
	int CompareCard(const BYTE cbFirstCardData[],const BYTE cbNextCardData[]);
	//逻辑大小
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
};

//////////////////////////////////////////////////////////////////////////

#endif
