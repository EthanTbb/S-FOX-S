#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//��������
#define	ST_VALUE					1									//��ֵ����
#define	ST_NEW					    3									//��ֵ����
#define	ST_LOGIC					2									//�߼�����

//�˿���Ŀ
#define CARD_COUNT					32									//�˿���Ŀ
//////////////////////////////////////////////////////////////////////////

static CString GetFileDialogPath()
{
	CString strFileDlgPath;
	TCHAR szModuleDirectory[MAX_PATH];	//ģ��Ŀ¼
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
	CString strName = GetFileDialogPath()+	"\\LuxuryCar.log";
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
//��Ϸ�߼�
class CGameLogic
{

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE cbCardData) 
	{ 
	   return cbCardData&LOGIC_MASK_VALUE; 
	}
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE cbCardData)
	{
		return (cbCardData&LOGIC_MASK_COLOR)>>4;
	}

	//���ƺ���
public:
	//��������
	void RandArray(INT nArrayOne[], INT nCountOne, INT nArrayTwo[], INT nCountTwo);
	
	//�߼�����
public:
	//��ȡ�Ƶ�
	BYTE GetCardListPip(const BYTE cbCardData[], BYTE cbCardCount);
	//��ȡ����
	BYTE GetCardType(const BYTE cbCardData, BYTE cbCardCount,BYTE *bcOutCadData = NULL);
	//��С�Ƚ�
	int CompareCard(const BYTE cbFirstCardData[], BYTE cbFirstCardCount,const BYTE cbNextCardData[], BYTE cbNextCardCount,BYTE &Multiple);
	//�߼���С
	BYTE GetCardLogicValue(BYTE cbCardData);

	BYTE GetCardNewValue(BYTE cbCardData);
	//��ȡ����
	BYTE GetAnimalIndex(BYTE cbAllIndex);
};

//////////////////////////////////////////////////////////////////////////

#endif
