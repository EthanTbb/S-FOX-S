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
	//ת����ֵ
	TCHAR szSwitchScore[16]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	WORD wTargetIndex=0;
	WORD wSourceIndex=0;
	UINT uSwitchLength=lstrlen(szSwitchScore);

	//ת���ַ�
	while (szSwitchScore[wSourceIndex]!=0)
	{
		//�����ַ�
		pszBuffer[wTargetIndex++]=szSwitchScore[wSourceIndex++];

		//���붺��
		if ((uSwitchLength!=wSourceIndex)&&(((uSwitchLength-wSourceIndex)%uSpace)==0L))
		{
			pszBuffer[wTargetIndex++]=TEXT(',');
		}
	}

	//�����ַ�
	pszBuffer[wTargetIndex++]=0;

	return;
}


SCORE CNumberConvert::GetUserControlScore(LPCTSTR pszScore)
{
	tString tStrScore(TEXT(" "));
	//ȥ�����еģ���
	tString::size_type pos = -1;
	while( (pos=tStrScore.find(TEXT(","))) != -1 ){
		tStrScore.erase(pos, 1);
	}

	//��������
	WORD wIndex=0;
	SCORE lScore=0L;

	//��ȡ��ֵ
	TCHAR szBuffer[32]=TEXT("");
	lstrcpyn(szBuffer,pszScore,CountArray(szBuffer));

	//������ֵ
	while (szBuffer[wIndex]!=0)
	{
		//��������
		WORD wCurrentIndex=wIndex++;

		//������ֵ
		if ((szBuffer[wCurrentIndex]>=TEXT('0'))&&(szBuffer[wCurrentIndex]<=TEXT('9')))
		{
			lScore=lScore*10L+(szBuffer[wCurrentIndex]-TEXT('0'));
		}
	}

	return lScore;
}

//ת���ַ�
VOID CNumberConvert::SwitchScoreString(SCORE lScore, LPTSTR pszBuffer, WORD wBufferSize)
{
	//��������
	LPCTSTR pszNumber[]={TEXT("��"),TEXT("Ҽ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("½"),TEXT("��"),TEXT("��"),TEXT("��")};
	LPCTSTR pszWeiName[]={TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��")};
	LPCTSTR pszWeiName1[]={TEXT("ʰ"),TEXT("��"),TEXT("Ǫ"),TEXT("��"),TEXT("��")};

	//ת����ֵ
	TCHAR szSwitchScore[32]=TEXT("");
	_sntprintf(szSwitchScore,CountArray(szSwitchScore),TEXT("%I64d"),lScore);

	//��������
	bool bNeedFill=false;
	bool bNeedZero=false;
	UINT uTargetIndex=0,uSourceIndex=0;

	//�ַ�����
	UINT uItemLength=lstrlen(pszNumber[0]);
	int uSwitchLength=lstrlen(szSwitchScore);

	//ת������
	for (int i=0;i<uSwitchLength;i++)
	{
		//��������
		WORD wNumberIndex=szSwitchScore[i]-TEXT('0');

		//�������
		if ((bNeedZero==true)&&(wNumberIndex!=0L))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszNumber[0],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//��������
		if (wNumberIndex!=0L)
		{
			_tcsncat(pszBuffer,pszNumber[wNumberIndex],wBufferSize-lstrlen(pszBuffer)-1);
		}

		if ((wNumberIndex!=0L)&&((uSwitchLength-i)>=2))
		{
			bNeedZero=false;
			_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
		}

		//�����ж�
		if ((bNeedZero==false)&&(wNumberIndex==0))
		{
			bNeedZero=true;
		}

		//��λ�ж�
		if ((bNeedFill==false)&&(wNumberIndex!=0))
		{
			bNeedFill=true;
		}

		//�λ��
		if (((uSwitchLength-i)==5)||((uSwitchLength-i)==9))
		{
			//����λ��
			if ((bNeedFill==true)&&(wNumberIndex==0L))
			{
				_tcsncat(pszBuffer,pszWeiName[uSwitchLength-i-2],wBufferSize-lstrlen(pszBuffer)-1);
			}

			//���ñ���
			bNeedZero=false;
			bNeedFill=false;
		}
	}

	return;
}

//ת���ַ�
VOID CNumberConvert::NumberConvert(TCHAR* szNum, TCHAR szResult[], int nLen)
{
#ifndef _UNICODE
	#define SZ_LEN	10
#else
	#define SZ_LEN	5
#endif
	const TCHAR szBigUnits[][SZ_LEN] = { _T("�f"), _T("�|"), _T("��"), _T("��"), _T("��"), _T("��"), _T("�"), 
										  _T("��"), _T("��"), _T("��"), _T("�d"),_T("�O"), _T("���ɳ"), _T("��ɮ�o"), 
										  _T("������"), _T("����˼�h"), _T("�o����") };

	const TCHAR szSmallUnits[][SZ_LEN] = { _T("ʮ"), _T("��"), _T("ǧ") };
	const TCHAR _szNum[][SZ_LEN] = { TEXT("��"),TEXT("Ҽ"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("½"),TEXT("��"),TEXT("��"),TEXT("��") };

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

		//ȫ������
		if (_ttoi(szPart) == 0)
		{
			if (uEmptyBigUnitCount == 0)
				_tcsnccat(szResult, _szNum[0], SZ_LEN);
			nIndex += nPartLen;
			uEmptyBigUnitCount++;
			continue;
		}

		bool bFillZero = false;
		//����ʮ��λ
		for (int i = 0; i < nPartLen; ++i)
		{
			UINT uNum = szPart[i] - '0';
			if (nIndex > 0 && bFillZero == false && uNum == 0)
			{
				bFillZero = true;
			}
			else if (uNum != 0)
			{
				//��λ���㲹��
				if (bFillZero == true && uNum != 0 && uEmptyBigUnitCount == 0)
					_tcsnccat(szResult, _szNum[0], SZ_LEN);
				_tcsnccat(szResult, _szNum[uNum], SZ_LEN);
				if (nPartLen - i - 2 >= 0)
					_tcsnccat(szResult, szSmallUnits[nPartLen - i - 2], SZ_LEN);
				bFillZero = false;
			}
		}

		//����λ
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

	// ����
	tString tStrScore(szResult);
	//ȥ�����еģ���
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
