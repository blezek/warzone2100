/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2010  Warzone 2100 Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
/** @file
 *  Definitions for multi player statistics and scores for league tables.
 *  Also Definitions for saved Arena Forces to enable teams to be saved to disk
 */

#ifndef __INCLUDED_SRC_MULTISTATS_H__
#define __INCLUDED_SRC_MULTISTATS_H__

#include "lib/netplay/netplay.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

typedef struct
{
	uint32_t played;						/// propogated stats.
	uint32_t wins;
	uint32_t losses;
	uint32_t totalKills;
	uint32_t totalScore;

	uint32_t recentKills;				// score/kills in last game.
	uint32_t recentScore;

	uint32_t killsToAdd;					// things to add next time score is updated.
	uint32_t scoreToAdd;
} PLAYERSTATS;

BOOL saveMultiStats(const char *sFName, const char *sPlayerName, const PLAYERSTATS *playerStats);	// to disk
BOOL loadMultiStats(char *sPlayerName, PLAYERSTATS *playerStats);					// form disk
PLAYERSTATS getMultiStats(UDWORD player);									// get from net
BOOL setMultiStats(SDWORD player, PLAYERSTATS plStats,BOOL bLocal);			// send to net.
void updateMultiStatsDamage(UDWORD attacker, UDWORD defender, UDWORD inflicted);
void updateMultiStatsGames(void);
void updateMultiStatsWins(void);
void updateMultiStatsLoses(void);
void updateMultiStatsKills(BASE_OBJECT *psKilled,UDWORD player);
void recvMultiStats(NETQUEUE queue);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INCLUDED_SRC_MULTISTATS_H__
