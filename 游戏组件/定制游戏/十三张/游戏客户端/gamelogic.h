#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once
#include "afxtempl.h"
//////////////////////////////////////////////////////////////////////////	

//�˿�����

#define CT_INVALID						0								//��������
#define CT_SINGLE						1								//��������
#define CT_ONE_DOUBLE					2								//ֻ��һ��
#define CT_TWO_DOUBLE					3								//��������
#define CT_THREE						4								//��������
#define CT_FIVE_MIXED_FLUSH_FIRST_A		5								//Aǰ˳��
#define CT_FIVE_MIXED_FLUSH_NO_A		6								//��ͨ˳��
#define CT_FIVE_MIXED_FLUSH_BACK_A		7								//A��˳��
#define CT_FIVE_FLUSH					8								//ͬ��
#define CT_FIVE_THREE_DEOUBLE			9								//����һ��
#define CT_FIVE_FOUR_ONE				10								//�Ĵ�һ��
#define CT_FIVE_STRAIGHT_FLUSH_FIRST_A	11								//Aͬ��˳
#define CT_FIVE_STRAIGHT_FLUSH			12								//ͬ��˳��

//��������
#define CT_EX_INVALID					14								//��������
#define CT_EX_SANTONGHUA				15								//��ͬ��
#define CT_EX_SANSHUNZI					16								//��˳��
#define CT_EX_LIUDUIBAN					17								//���԰�
#define CT_EX_WUDUISANTIAO				18								//�������
#define CT_EX_SITAOSANTIAO				19								//��������
#define CT_EX_SHUANGGUAICHONGSAN		20								//˫�ֳ���
#define CT_EX_COUYISE					21								//��һɫ
#define CT_EX_QUANXIAO					22								//ȫС
#define CT_EX_QUANDA					23								//ȫ��
#define CT_EX_SANFENGTIANXIA			24								//��������
#define CT_EX_SANTONGHUASHUN			25								//��ͬ��˳
#define CT_EX_SHIERHUANGZU				26								//ʮ������
#define CT_EX_YITIAOLONG				27								//һ����
#define CT_EX_ZHIZUNQINGLONG			28								//��������

//��ֵ����
#define	LOGIC_MASK_COLOR				0xF0							//��ɫ����
#define	LOGIC_MASK_VALUE				0x0F							//��ֵ����

//�����ṹ
struct tagAnalyseData
{
	BYTE							bOneCount;								//������Ŀ
	BYTE							bTwoCount;								//������Ŀ 
	BYTE							bThreeCount;							//������Ŀ
	BYTE							bFourCount;								//������Ŀ
	BYTE							bOneFirst[13];							//����λ��
	BYTE							bTwoFirst[13];							//����λ��
	BYTE							bThreeFirst[13];						//����λ��
	BYTE							bFourFirst[13];							//����λ��
	bool							bSameColor;								//�Ƿ�ͬ��
};

//�����ṹ
struct tagAnalyseResult
{
	BYTE 							cbBlockCount[4];						//ͬ����Ŀ
	BYTE							cbCardData[4][13];						//�˿��б�
};

//�ֲ���Ϣ
struct tagDistributing
{
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbDistributing[13][5];				//�ֲ���Ϣ
};

//�������
struct tagSearchCardResult
{
	BYTE							cbSearchCount;						//�����Ŀ
	BYTE							cbCardCount[13];					//�˿���Ŀ
	BYTE							cbResultCard[13][13];				//�˿��б�
};

//ת������
struct tagTransData
{
	BYTE							bKingCount;							//������Ŀ
	BYTE							bHaveKing;							//�Ƿ�����	(0 �� 1 С�� 2 ���� 3 ��С��)		
	CList<BYTE>						transList;							//ת���ɵ���������
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

//��������
enum enSortCardType
{
	enDescend,																//�������� 
	enAscend,																//��������
	enColor																	//��ɫ����
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////	
//���Ժ���
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


//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount, BYTE & bMaxCardData);
	//��������
	BYTE GetSpecialType(BYTE bHandCardData[], BYTE bCardCount);

	BYTE GetSpecialCardData(BYTE bHandCardData[13], BYTE bHandCardCount, BYTE bSpecCardData[3][5]);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; } //ʮ������ǰ����λ��ʾ�Ƶ���ֵ
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return (bCardData&LOGIC_MASK_COLOR)>>4; } //ʮ�����ƺ�����λ��ʾ�ƵĻ�ɫ 

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount,enSortCardType SortCardType=enDescend);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);
	
    /*
	@�˺��������Ż�
	@�Ż�����2014-3-21
	*/
	//�Զ�����
	void AutoPutCard(BYTE bCardData[], BYTE bPutCardData[], bool bIsAndroidUser, bool bTest);

	//�߼�����
public:
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE bCardData);
	////�Ա��˿�
	//bool CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//�Ա��˿�
	int CompareCard(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount , bool bComperWithOther);
	//�������
	bool IsStraightDragon(const BYTE cbCardData[], BYTE bCardCount);
	//�Ƿ�˳��
	bool IsLinkCard(const BYTE cbCardData[], BYTE cbCardCount);
	//�Ƿ�ͬ��
	bool IsSameColorCard(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetDoubleCount(BYTE cbFrontCard[], BYTE cbMidCard[], BYTE cbBackCard[]);

	//���ܺ���
public:
	//�����˿�
	void AnalyseCard(const BYTE bCardDataList[] , const BYTE bCardCount , tagAnalyseData& AnalyseData) ;
	//�����˿�
	void AnalysebCardData(const BYTE cbCardData[], BYTE cbCardCount, tagAnalyseResult & AnalyseResult) ;
	//�����ֲ�
	VOID AnalysebDistributing(const BYTE cbCardData[], BYTE cbCardCount, tagDistributing & Distributing);
	//����ת��
	BYTE AnalyseMaxTransform(const BYTE cbCardData[], const BYTE cbCardCount, BYTE bTransCardData[], tagTransData & TransData);
	//����ת��
	void TransformCard(const BYTE cbNkCardData[], BYTE bCardCount, BYTE bKCount, BYTE bCardType, 
		const tagAnalyseData & tad, BYTE bTransCardData[], tagTransData & TransData);
	//���űȽ�
	bool CompareOneCardEx(BYTE bFirstCard, BYTE bNextCard, tagTransData & ttdFst, tagTransData & ttdNxt);
	//��ȡ����
	BYTE GetMaxCardData(const BYTE cbCardData[], const BYTE cbCardCount, BYTE cbMaxCardData[], BYTE & bMaxCardCount, BYTE bNeedCCount = 5);
	//�����˿�
	BYTE MakeCardData(BYTE cbValueIndex, BYTE cbColorIndex);
    
	//����ͬ��
	BYTE SearchSameCard( const BYTE cbHandCardData[], BYTE cbHandCardCount,BYTE cbSameCardCount, tagSearchCardResult *pSearchCardResult );
	//��������(����һ���Ĵ�һ��)
	BYTE SearchTakeCardType( const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, BYTE cbTakeCardCount, tagSearchCardResult *pSearchCardResult );
	//����ͬ��
	BYTE SearchSameColorType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbSameCount, tagSearchCardResult *pSearchCardResult);
	//��������
	BYTE SearchLineCardType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult);
	//����ͬ��˳
	BYTE SearchSameColorLineType(const BYTE cbHandCardData[], BYTE cbHandCardCount, BYTE cbLineCount, tagSearchCardResult *pSearchCardResult);

public:

	/*
	@���º���Ϊ�����˿��������ĸ���
	@�޸�����2014-3-21
	*/
	//������ת��Ȩֵ
	int CardTypeToWeights(BYTE bType, BYTE cbIndex);
	/*
	@���º���Ϊ����Զ����ƺ�����AutoPutCard ��ʹ�õĸ���
	@�޸�����2014-3-21
	*/
	//�Ż��������
	void OptimizationCombo(BYTE bInFirstList[], BYTE bInNextList[], BYTE bFirstCount, BYTE bNextCount);
};

//////////////////////////////////////////////////////////////////////////

#endif
