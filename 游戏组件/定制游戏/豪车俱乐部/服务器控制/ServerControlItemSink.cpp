#include "StdAfx.h"
#include "servercontrolitemsink.h"
#include <locale>

CServerControlItemSink::CServerControlItemSink(void)
{
	m_cbControlArea = 0xff;
	m_cbControlTimes = 0;
	m_lCurStorage=0;
	ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
	m_cbControlStyle=0;
}

CServerControlItemSink::~CServerControlItemSink( void )
{

}

//服务器控制
bool  CServerControlItemSink::ServerControl(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem, ITableFrame * pITableFrame)
{		
	ASSERT(wDataSize==sizeof(CMD_C_ControlApplication));
	if (wDataSize!=sizeof(CMD_C_ControlApplication)) 
		return false;
	//如果不具有管理员权限 则返回错误
	if( !CUserRight::IsGameCheatUser(pIServerUserItem->GetUserRight()) )
		return false;

	CMD_C_ControlApplication* pControlApplication = (CMD_C_ControlApplication*)pDataBuffer;

	switch(pControlApplication->cbControlAppType)
	{
	case C_CA_UPDATE:	//更新
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_UPDATE_SUCCES;
			ControlReturns.cbControlArea = m_cbControlArea;
			ControlReturns.cbControlTimes = m_cbControlTimes;
			ControlReturns.cbControlStyle=m_cbControlStyle;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	case C_CA_SET:		//设置
		{
			
			switch(pControlApplication->cbControlStyle)
			{
				case CS_BET_AREA:	//区域设置
					{
						m_cbControlStyle=pControlApplication->cbControlStyle;
						
						m_cbControlArea = pControlApplication->cbControlArea;
						m_cbControlTimes = pControlApplication->cbControlTimes;

						CMD_S_ControlReturns ControlReturns;
						ZeroMemory(&ControlReturns,sizeof(ControlReturns));
						ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
						ControlReturns.cbControlArea = m_cbControlArea;
						ControlReturns.cbControlTimes = m_cbControlTimes;
						ControlReturns.cbControlStyle=m_cbControlStyle;
						UpdateControl(&ControlReturns,pIServerUserItem);
						pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
						
						break;
					}
				case CS_BANKER_LOSE:	//庄家输牌
					{
						m_cbControlStyle=pControlApplication->cbControlStyle;
						m_cbControlTimes=pControlApplication->cbControlTimes;
						CMD_S_ControlReturns ControlReturns;
						ZeroMemory(&ControlReturns,sizeof(ControlReturns));
						ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
						ControlReturns.cbControlArea = m_cbControlArea;
						ControlReturns.cbControlTimes = m_cbControlTimes;
						ControlReturns.cbControlStyle=m_cbControlStyle;
						UpdateControl(&ControlReturns,pIServerUserItem);
						pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));

						break;
					}
				case CS_BANKER_WIN:		//庄家赢牌
					{
						m_cbControlStyle=pControlApplication->cbControlStyle;
						m_cbControlTimes=pControlApplication->cbControlTimes;
						CMD_S_ControlReturns ControlReturns;
						ZeroMemory(&ControlReturns,sizeof(ControlReturns));
						ControlReturns.cbReturnsType = S_CR_SET_SUCCESS;
						ControlReturns.cbControlArea = m_cbControlArea;
						ControlReturns.cbControlTimes = m_cbControlTimes;
						ControlReturns.cbControlStyle=m_cbControlStyle;
						UpdateControl(&ControlReturns,pIServerUserItem);
						pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
						break;
					}
				default:	//拒绝请求
					{
						//设置失败
						CMD_S_ControlReturns ControlReturns;
						ZeroMemory(&ControlReturns,sizeof(ControlReturns));
						ControlReturns.cbReturnsType = S_CR_FAILURE;
						ControlReturns.cbControlArea = 0xff;
						pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
						break;
					}
			
			}
			return true;
		}
	case C_CA_CANCELS:	//取消
		{
			m_cbControlArea = 0xff;
			m_cbControlTimes = 0;
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_CANCEL_SUCCESS;
			ControlReturns.cbControlArea = 0xff;
			UpdateControl(&ControlReturns,pIServerUserItem);
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;

	case C_CA_PRINT_STORAGE:	//打印库存
		{
			CMD_S_ControlReturns ControlReturns;
			ZeroMemory(&ControlReturns,sizeof(ControlReturns));
			ControlReturns.cbReturnsType = S_CR_ACK_PRINT_STORAGE;
			ControlReturns.lCurStorage = m_lCurStorage;
			pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
		}
		return true;
	}
	//设置失败
	CMD_S_ControlReturns ControlReturns;
	ZeroMemory(&ControlReturns,sizeof(ControlReturns));
	ControlReturns.cbReturnsType = S_CR_FAILURE;
	ControlReturns.cbControlArea = 0xff;
	pITableFrame->SendUserItemData(pIServerUserItem,SUB_S_ADMIN_COMMDN,&ControlReturns,sizeof(ControlReturns));
	return true;
}

//需要控制
bool  CServerControlItemSink::NeedControl()
{
	/*if ( m_cbControlArea != 0xff && m_cbControlArea <= 7 && m_cbControlTimes > 0 )
	{
		return true;
	}*/

	if(m_cbControlStyle > 0 && m_cbControlTimes > 0)
	{
		return true;
	}

	return false;
	
}

//满足控制
bool  CServerControlItemSink::MeetControl(tagControlInfo ControlInfo)
{
	return true;
}

//完成控制
bool  CServerControlItemSink::CompleteControl()
{
	/*m_cbControlTimes--;
	if(m_cbControlTimes == 0)
	{
		m_cbControlArea = 0xff;
	}*/

	if (m_cbControlTimes > 0)
		m_cbControlTimes--;	

	if ( m_cbControlTimes == 0)
	{
		m_cbControlTimes = 0;								
		m_cbControlStyle = 0;	
		m_cbControlArea = 0xff;
	    ZeroMemory(m_lAllJettonScore,sizeof(m_lAllJettonScore));
		
	}

	return true;
}

//返回控制区域
bool  CServerControlItemSink::ReturnControlArea(tagControlInfo& ControlInfo,LONGLONG lAllJettonScore[])
{
	
	memcpy(m_lAllJettonScore, lAllJettonScore, sizeof(m_lAllJettonScore));

	switch(m_cbControlStyle)
	{
	case CS_BET_AREA:
		{
			ControlInfo.cbControlArea=GetRealArea(m_cbControlArea);
			break;
		}
	case CS_BANKER_WIN:
		{
			ControlInfo.cbControlArea=BankerWinCard(true);
			break;
		}
	case CS_BANKER_LOSE:
		{
			ControlInfo.cbControlArea=BankerWinCard(false);
			break;
		}

	}
	
	return true;
}

BYTE CServerControlItemSink::BankerWinCard(bool bIsWin)
{
	
	BYTE cbRealArea=1;
	//比较牌之间关系
	for(BYTE cbIndex=0;cbIndex<7;cbIndex++)
	{
		cbRealArea=GetRealArea(cbIndex);

		LONGLONG lBankerWinScore=0;

		lBankerWinScore= GetBankerWinScore(cbRealArea);

		if(lBankerWinScore==0) return cbRealArea;
				
		if (bIsWin&&lBankerWinScore>0)
		{
			return cbRealArea;
		}

		if ((!bIsWin)&&lBankerWinScore<0)
		{
			return cbRealArea;
		}

	}

	return cbRealArea;

}

LONGLONG CServerControlItemSink::GetBankerWinScore(BYTE cbRealArea)
{
	BYTE  bcResulteOut[AREA_COUNT];
	memset(bcResulteOut,0,AREA_COUNT);
	m_GameLogic.GetCardType(cbRealArea,1,bcResulteOut);
	
	LONGLONG lBankerWinScore=0;
	
	for (WORD wAreaIndex = 1; wAreaIndex <= AREA_COUNT; ++wAreaIndex)
	{

		if (bcResulteOut[wAreaIndex-1]>1) 
		{
			lBankerWinScore -= ( m_lAllJettonScore[wAreaIndex]* (bcResulteOut[wAreaIndex-1]-1) ) ;
		}
		else
		{
			if (bcResulteOut[wAreaIndex-1]==0)
			{
				lBankerWinScore += m_lAllJettonScore[wAreaIndex];
			}

		}
	}

	return lBankerWinScore;
}

//实际区域
BYTE CServerControlItemSink::GetRealArea(BYTE cbNeedControlArea)
{
	BYTE cbRealArea=0;

	//玛莎拉蒂
	if(0==cbNeedControlArea)  cbRealArea=1;

	//法拉利
	else if(1==cbNeedControlArea) cbRealArea=5;

	//兰博基尼
	else if(2==cbNeedControlArea) cbRealArea=13;

	//保时捷
	else if(3==cbNeedControlArea) cbRealArea=9;

	//路虎
	else if(4==cbNeedControlArea) 
	{
		BYTE cbControlArea[3] = { 8, 11, 14};
		cbRealArea=cbControlArea[rand()%3];
	}
	//宝马
	else if(5==cbNeedControlArea) 
	{
		BYTE cbControlArea[3] = {2, 12, 16};
		cbRealArea=cbControlArea[rand()%3];
	}
	//捷豹
	else if(6==cbNeedControlArea) 
	{
		BYTE cbControlArea[3] = { 4, 7, 10};
		cbRealArea=cbControlArea[rand()%3];
	}
	//奔驰
	else if(7==cbNeedControlArea) 
	{
		BYTE cbControlArea[3] = { 3, 6, 15};
		cbRealArea=cbControlArea[rand()%3];
	}

	return cbRealArea;
}

//记录库存
bool CServerControlItemSink::SetStorage(LONGLONG lCurStorage)
{
	m_lCurStorage=lCurStorage;
	return true;
}
//信息
void CServerControlItemSink::PrintingInfo( TCHAR* pText, WORD cbCount, BYTE cbArea, BYTE cbTimes )
{
	if (cbArea == 0xff )
	{
		myprintf(pText,cbCount,TEXT("暂时无控制。"));
		return;
	}
	TCHAR szDesc[32] = TEXT("");
	myprintf(pText,cbCount,TEXT("胜利区域："));
	if ( cbArea == 0 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("玛莎拉蒂×20，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 1 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("法拉利×20，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 2 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("兰博基尼×20，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 3 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("保时捷×20，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 4 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("路虎×4，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 5 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("宝马×4，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 6 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("捷豹×4，") );
		lstrcat( pText,szDesc );
	}
	else if ( cbArea == 7 )
	{
		_sntprintf( szDesc,CountArray(szDesc),TEXT("奔驰×4，") );
		lstrcat( pText,szDesc );
	}
	myprintf(szDesc,CountArray(szDesc),TEXT("执行次数：%d。"), cbTimes);
	lstrcat( pText,szDesc );
}
//更新控制
void  CServerControlItemSink::UpdateControl( CMD_S_ControlReturns* pControlReturns ,IServerUserItem * pIServerUserItem)
{
	CString str=TEXT("");
	switch(pControlReturns->cbReturnsType)
	{
	case S_CR_FAILURE:
		{
			str+="操作失败";
			break;
		}
	case S_CR_UPDATE_SUCCES:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			//PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			myprintf(zsText,CountArray(zsText),TEXT("更新数据成功！\r\n %s"),zsTextTemp);
			str+=zsText;


			break;
		}
	case S_CR_SET_SUCCESS:
		{
			TCHAR zsText[256] = TEXT("");
			TCHAR zsTextTemp[256] = TEXT("");
			PrintingInfo(zsTextTemp,256,pControlReturns->cbControlArea,pControlReturns->cbControlTimes);
			myprintf(zsText,CountArray(zsText),TEXT("设置数据成功！\r\n %s"),zsTextTemp);
			str+=zsText;


			switch(m_cbControlStyle)
			{
			case CS_BANKER_WIN:
				{
					str.Format(TEXT("请求已接受，控制方式：庄家赢，执行次数：%d"),m_cbControlTimes);break;
				}
			case CS_BANKER_LOSE:
				{
					str.Format(TEXT("请求已接受，控制方式：庄家输，执行次数：%d"),m_cbControlTimes);break;
				}
			case CS_BET_AREA:
				{

					str.Format(TEXT("请求已接受，控制方式：区域控制，执行次数：%d"),m_cbControlTimes);						

					break;
				}
			default:break;
			}


			break;
		}
	case S_CR_CANCEL_SUCCESS:
		{
			str+="取消设置成功！";	
			break;
		}

	case S_CR_ACK_PRINT_STORAGE:
		{

			{  
				CString strStorage=TEXT("");
				strStorage.Format(TEXT("%I64d"),pControlReturns->lCurStorage);
				str+=strStorage;

			}
			break;
		}
	}
	TCHAR szClientAddr[16]=TEXT("");
	DWORD dwAddr=pIServerUserItem->GetClientAddr();
	BYTE * pClientAddr=(BYTE *)&dwAddr;
	_sntprintf(szClientAddr,CountArray(szClientAddr),TEXT("%d.%d.%d.%d"),pClientAddr[0],pClientAddr[1],pClientAddr[2],pClientAddr[3]);

	CString strRecord;
	CTime Time(CTime::GetCurrentTime());
	strRecord.Format(TEXT(" %s :| 时间: %d-%d-%d %d:%d:%d |机器码:%s|IP地址:%d\n"),pIServerUserItem->GetNickName(), Time.GetYear(), Time.GetMonth(), Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond(),pIServerUserItem->GetMachineID(),szClientAddr);
	strRecord=strRecord+str;
	WriteInfo(strRecord);
}
//测试写信息
void CServerControlItemSink::WriteInfo( LPCTSTR pszString )
{
	//设置语言区域
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );

	CStdioFile myFile;
	CString strFileName;
	strFileName.Format(TEXT("豪车俱乐部控制记录.txt"));
	BOOL bOpen = myFile.Open(strFileName, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate);
	if ( bOpen )
	{	
		myFile.SeekToEnd();
		myFile.WriteString( pszString );
		myFile.Flush();
		myFile.Close();
	}

	//还原区域设定
	setlocale( LC_CTYPE, old_locale );
	free( old_locale );
}
