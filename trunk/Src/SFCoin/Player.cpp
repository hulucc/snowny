#include "stdafx.h"
#include "Player.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

DWORD CPlayer::m_UnionCoins = 0;
DWORD CPlayer::m_OldUnionCoins = -1;

CPlayer::CPlayer(DWORD id)
: m_dId(id)
, m_Status(PS_IDLE)
, m_OldCoins(-1)
, m_dCoins(0)
, m_StatusChangeTime(GetTickCount())
{
	m_ContinueVal = NULL; // 1P、2P的死亡标志：continue的值，
						  // 靠，2P设备开始游戏的话其实控制的是1P，那么continue值也是用1P的，游戏本身就是混乱的
						  // 所以这里赋值不正确的话就会产生无法检测到对应任务死亡状态的bug
						  // 也所以暂时由外面设置地址，cxb

	if (g_Config.IsFree == 1)	// 免费生命值设为99
	{
		DWORD *pCoins = g_Config.CoinMode == 0 ? &m_UnionCoins : &m_dCoins; // 单式、双式
		*pCoins = g_Config.UnitCoin * 99;
	}
}

void CPlayer::RefreshStatus( GAMEFLOW gameFlow )
{
	if (m_Status != PS_IDLE)
	{
		if (gameFlow == flow_mainmenu && m_Status == PS_CLICKSTART)
		{
			SetStatus(PS_STARTTING);
			TRACE(TEXT("SF4 player[%d] PS_STARTTING"), m_dId);
		}
		else if (gameFlow == flow_game && m_Status == PS_STARTTING)
		{
			*m_ContinueVal = 0; // continue值清零，因为在continue状态中开始，游戏不会清零
			SetStatus(PS_GAMING);
			TRACE(TEXT("SF4 player[%d] PS_GAMING 0x%x"), m_dId, m_ContinueVal);
		}
		else if (*m_ContinueVal != 0 && m_Status == PS_GAMING)
		{
			SetStatus(PS_DEAD);
			TRACE(TEXT("SF4 player[%d] PS_DEAD"), m_dId);
		}
		else if (*m_ContinueVal == 0 && m_Status == PS_DEAD)
		{
			SetStatus(PS_GAMEOVER);
			TRACE(TEXT("SF4 player[%d] PS_GAMEOVER"), m_dId);
		}
		else if (gameFlow == flow_titlemenu && m_Status == PS_GAMEOVER)
		{
			SetStatus(PS_IDLE);
			DWORD *pCoins = g_Config.CoinMode == 0 ? &m_OldUnionCoins : &m_OldCoins; // 单式、双式
			*pCoins = -1;// 为了回到title界面显示请投币，囧，cxb
			TRACE(TEXT("SF4 player[%d] PS_IDLE"), m_dId);
		}
	}
}

BOOL CPlayer::ClickStart(DWORD *continueValAddr)
{
	if (m_Status == PS_IDLE || m_Status == PS_DEAD || m_Status == PS_GAMEOVER)
	{
		DWORD *pCoins = g_Config.CoinMode == 0 ? &m_UnionCoins : &m_dCoins; // 单式、双式
		if (*pCoins >= g_Config.UnitCoin || g_Config.IsFree == 1)
		{
			if (g_Config.IsFree != 1)	// 不免费才扣币
			{
				AddCoins(-g_Config.UnitCoin);// 进入到游戏才扣币
			}
			m_Status = PS_CLICKSTART;

			if (continueValAddr != NULL) // NULL 表示不做修改，第一次必须传递非NULL
			{
				m_ContinueVal = continueValAddr;
			}
			ASSERT(m_ContinueVal != NULL);

			TRACE(TEXT("SF4 player[%d] start success"), m_dId);
			return TRUE;
		}
		else
		{
			TRACE(TEXT("SF4 player[%d] start fail: has no coins %d, %d, %d"),
				m_dId, *pCoins, g_Config.UnitCoin, m_Status);
		}
	}
	else
	{
		TRACE(TEXT("SF4 player[%d] start fail: error status %d"), m_dId, m_Status);
	}

	return FALSE;
}

void CPlayer::IncrementCoin()
{
	AddCoins(1);
}

void CPlayer::AddCoins( int increCoins )
{
	PlaySound(TEXT(".\\SF4Con\\InsertCoin.wav"), NULL, SND_ASYNC);
	DWORD *pCoins = g_Config.CoinMode == 0 ? &m_UnionCoins : &m_dCoins; // 单式、双式

	*pCoins += increCoins;
}

BOOL CPlayer::CoinsChanged()
{
	BOOL changed = FALSE;
	DWORD *pCoins = g_Config.CoinMode == 0 ? &m_UnionCoins : &m_dCoins;			// 单式、双式
	DWORD *pOldCoins = g_Config.CoinMode == 0 ? &m_OldUnionCoins : &m_OldCoins; // 单式、双式

	changed = *pCoins != *pOldCoins;
	*pOldCoins = *pCoins;

	return changed;
}

DWORD CPlayer::GetCoinNumber()
{
	return g_Config.CoinMode == 0 ? m_UnionCoins : m_dCoins;// 单式、双式
}

int CPlayer::QueryHP() const
{
	DWORD getGlobalDataAddr = 0x0043A670,
		getPlayerDataAddr = 0x00439F80,
		globalDataAddr = 0,
		playerAddr = 0;
	int playerHP = -1;

	DWORD id = m_dId;

	__asm
	{
		pushad
		mov eax, getGlobalDataAddr
		call eax
		mov globalDataAddr, eax
	
		push id
		mov ecx, globalDataAddr
		mov edx, getPlayerDataAddr
		call edx
		test eax, eax
		jz jmp1
		mov playerAddr, eax
		movsx edi, word ptr [eax + 51F6h]
		mov playerHP, edi
jmp1:
		popad
	}

	//TRACE(TEXT("SF4 player[%d] playeraddr:0x%x HP: %d"), m_dId, playerAddr, playerHP);
	return playerHP;
}
