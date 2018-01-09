#pragma once

#include "Stdafx.h"

//������
struct DPOINT
{
	DOUBLE				x;
	DOUBLE				y;

	DPOINT() {  x = 0.0; y = 0.0; }
	DPOINT( POINT& Par ) {  x = Par.x; y = Par.y; }
	DPOINT( DPOINT& Par ) {  x = Par.x; y = Par.y; }
	DPOINT( DOUBLE ParX, DOUBLE ParY ) { x = ParX; y = ParY; }
	void operator+= (POINT& ptPar) { x += ptPar.x; y += ptPar.y; }
	void operator+= (DPOINT& ptPar) { x += ptPar.x; y += ptPar.y; }
	inline void SetPoint( DOUBLE ParX, DOUBLE ParY ) { x = ParX; y = ParY; }
};

// �����ƶ�
struct stuMoveChip
{
public:
	WORD		wChipIndex;				// ��������
	WORD		wChipChairID;			// ����λ��ID
	UINT		nTheTime;				// ��ǰ����
	UINT		nAllTime;				// �ܲ���
	DPOINT		ptBeginPos;				// ��ʼλ��
	DPOINT		ptEndPos;				// ����λ��
	DPOINT		ptCurrentPos;			// ��ǰλ��

	DPOINT		ptInitialVelocity;		// ���ٶ�
	DPOINT		ptFriction;				// Ħ����

	BOOL		bDeceleratedMotion;		// �����˶�
	BOOL		bShow;					// �Ƿ���ʾ


public:
	stuMoveChip( WORD wParChipIndex, WORD wParChipChairID, UINT nParAllTime, DPOINT ptParBeginPos, DPOINT ptParEndPos, BOOL bParDeceleratedMotion)
	{
		BeginMove(wParChipIndex, wParChipChairID, nParAllTime, ptParBeginPos, ptParEndPos, bParDeceleratedMotion);
	}

	void BeginMove( WORD wParChipIndex, WORD wParChipChairID, UINT nParAllTime, DPOINT ptParBeginPos, DPOINT ptParEndPos, BOOL bParDeceleratedMotion )
	{
		bShow        = TRUE;
		wChipIndex	 = wParChipIndex;
		wChipChairID = wParChipChairID;
		nTheTime	 = 0;
		nAllTime	 = nParAllTime;
		ptBeginPos	 = ptParBeginPos;
		ptEndPos     = ptParEndPos;
		ptCurrentPos = ptParBeginPos;
		bDeceleratedMotion = bParDeceleratedMotion;

		if ( nParAllTime != 0 )
		{
			// �����˶�
			if( bDeceleratedMotion )
			{
				// ���ٶ�
				ptInitialVelocity.x = (ptParEndPos.x - ptParBeginPos.x)/nParAllTime;
				ptInitialVelocity.y = (ptParEndPos.y - ptParBeginPos.y)/nParAllTime;

				// Ħ����
				ptFriction.x = -(ptInitialVelocity.x / nParAllTime / 2.035);
				ptFriction.y = -(ptInitialVelocity.y / nParAllTime / 2.035);
			}
			// �����˶�
			else
			{
				// �ٶ�
				ptInitialVelocity.x = (ptParEndPos.x - ptParBeginPos.x)/nParAllTime;
				ptInitialVelocity.y = (ptParEndPos.y - ptParBeginPos.y)/nParAllTime;
			}
		}
	}

	void SetShow( BOOL bParShow )
	{
		bShow = bParShow;
	}

	BOOL Move()
	{
		if ( nAllTime == 0 )
			return FALSE;

		// �����˶�
		if( bDeceleratedMotion )
		{
			// �ƶ�
			BOOL	bMove = FALSE;

			// ʱ��
			nTheTime++;

			// λ��
			DPOINT  ptDisplacement(ptInitialVelocity.x + ptFriction.x * nTheTime, ptInitialVelocity.y + ptFriction.y * nTheTime);

			// �����˶�
			if ( (ptInitialVelocity.x > 0 && ptDisplacement.x > 0)
				|| (ptInitialVelocity.x < 0 && ptDisplacement.x < 0) )
			{
				ptCurrentPos.x += ptDisplacement.x;
				bMove = TRUE;
			}

			if ( (ptInitialVelocity.y > 0 && ptDisplacement.y > 0)
				|| (ptInitialVelocity.y < 0 && ptDisplacement.y < 0) )
			{
				ptCurrentPos.y += ptDisplacement.y;
				bMove = TRUE;
			}

			return bMove;
		}
		// �����˶�
		else
		{
			if ( nTheTime == nAllTime)
				return FALSE;

			// �ƶ�ʱ��
			nTheTime++;

			// �ƶ�
			ptCurrentPos += ptInitialVelocity;

			
			return TRUE;
		}
	}
};

