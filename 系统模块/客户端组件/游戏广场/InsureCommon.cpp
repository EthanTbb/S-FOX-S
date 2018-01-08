#include "StdAfx.h"
#include "InsureCommon.h"

CNumberConvert::CNumberConvert()
{
	
}
CNumberConvert::~CNumberConvert()
{

}

VOID CNumberConvert::SwitchScoreFormat(SCORE lScore, UINT uSpace, LPTSTR pszBuffer, WORD wBufferSize)
{
	//转换数值
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//转换字符
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//拷贝字符
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//插入逗号
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//结束字符
	pszBuffer[wTargetIndex++]=0;

	return;
}


SCORE CNumberConvert::GetUserControlScore(LPCTSTR pszScore)
{
	tString tStrScore(TEXT(" "));
	//去掉所有的，号
	tString::size_type pos = -1;
	while( (pos=tStrScore.find(TEXT(","))) != -1 ){
		tStrScore.erase(pos, 1);
	}

	//变量定义
	WORD wIndex=0;
	SCORE lScore=0L;

	//获取数值
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszScore,CountArray(szBuffer));

	//构造数值
	while (szBuffer[wIndex]!=0)
	{
		//变量定义
		WORD wCurrentIndex=wIndex++;

		//构造数值
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//转换字符
VOID CNumberConvert::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//变量定义
	LPCTSTR pszNumber[]={TEXT("零"),TEXT("壹"),TEXT("贰"),TEXT("叁"),TEXT("肆"),TEXT("伍"),TEXT("陆"),TEXT("柒"),TEXT("捌"),TEXT("玖")};
	LPCTSTR pszWeiName[]={TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("亿"),TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万")};
	LPCTSTR pszWeiName1[]={TEXT("拾"),TEXT("佰"),TEXT("仟"),TEXT("万"),TEXT("亿")};

	//转换数值
	TCHAR szSwitchScore[32]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//变量定义
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//字符长度
	UINT uItemLength=lstrlen(pszNumber[0]);
	int uSwitchLength=lstrlen(szSwitchScore);

	//转换操作
	for (int i=0;i<uSwitchLength;i++)
	{
		//变量定义
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//补零操作
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//拷贝数字
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//补零判断
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//补位判断
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//填补位名
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//拷贝位名
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//设置变量
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//转换字符
VOID CNumberConvert::NumberConvert(TCHAR* szNum, TCHAR szResult[], int nLen)
{
#ifndef _UNICODE
	#define SZ_LEN	10
#else
	#define SZ_LEN	5
#endif
	const TCHAR szBigUnits[][SZ_LEN] = { _T("萬"), _T("億"), _T("兆"), _T("京"), _T("垓"), _T("秭"), _T("穰"), 
										  _T("溝"), _T("澗"), _T("正"), _T("載"),_T("極"), _T("恒河沙"), _T("阿僧祇"), 
										  _T("那由他"), _T("不可思議"), _T("無量大數") };

	const TCHAR szSmallUnits[][SZ_LEN] = { _T("十"), _T("百"), _T("千") };
	const TCHAR _szNum[][SZ_LEN] = { TEXT("零"),TEXT("壹"),TEXT("贰"),TEXT("叁"),TEXT("肆"),TEXT("伍"),TEXT("陆"),TEXT("柒"),TEXT("捌"),TEXT("玖") };

	int nIndex = 0;
	int nPartLen = 0;
	UINT uEmptyBigUnitCount = 0;
	TCHAR szPart[32] = { 0 };

	while (nLen > nIndex)
	{
		if ((nLen - nIndex) % 4 == 0)
		{
			nPartLen = 4;
		}
		else
		{
			nPartLen = (nLen - nIndex) % 4;
		}
		lstrcpyn(szPart, szNum + nIndex, nPartLen+1);

		//全零跳过
		if (_ttoi(szPart) == 0)
		{
			if (uEmptyBigUnitCount == 0)
				_tcsnccat(szResult, _szNum[0], SZ_LEN);
			nIndex += nPartLen;
			uEmptyBigUnitCount++;
			continue;
		}

		bool bFillZero = false;
		//计算十单位
		for (int i = 0; i < nPartLen; ++i)
		{
			UINT uNum = szPart[i] - '0';
			if (nIndex > 0 && bFillZero == false && uNum == 0)
			{
				bFillZero = true;
			}
			else if (uNum != 0)
			{
				//首位遇零补零
				if (bFillZero == true && uNum != 0 && uEmptyBigUnitCount == 0)
					_tcsnccat(szResult, _szNum[0], SZ_LEN);
				_tcsnccat(szResult, _szNum[uNum], SZ_LEN);
				if (nPartLen - i - 2 >= 0)
					_tcsnccat(szResult, szSmallUnits[nPartLen - i - 2], SZ_LEN);
				bFillZero = false;
			}
		}

		//补万单位
		if ((nLen - nIndex) % 4 == 0)
		{
			if ((nLen - nIndex) / 4 - 2 >= 0)
				_tcsnccat(szResult, szBigUnits[(nLen - nIndex) / 4 - 2], SZ_LEN);
		}
		else
		{
			if ((nLen - nIndex) / 4 - 1 >= 0)
				_tcsnccat(szResult, szBigUnits[(nLen - nIndex) / 4 - 1], SZ_LEN);
		}

		uEmptyBigUnitCount = 0;
		nIndex += nPartLen;
	}

	// 清零
	tString tStrScore(szResult);
	//去掉所有的，号
	tString::size_type pos = -1;
	if((pos=tStrScore.rfind(_szNum[0])) != -1 )
	{
		if(pos == tStrScore.length()-1)
		{
			tStrScore.erase(pos);
		}
	}

    int nResultLength = (int)tStrScore.length()+1;
	lstrcpyn(szResult,tStrScore.c_str(),nResultLength);

	return;
}
