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
 *  Interface to the routing functions
 */

#ifndef __INCLUDED_SRC_FPATH_H__
#define __INCLUDED_SRC_FPATH_H__

#include "droiddef.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

/** Return values for routing
 *
 *  @ingroup pathfinding
 *  @{
 */

typedef enum _fpath_movetype
{
	FMT_MOVE,		///< Move around all obstacles
	FMT_ATTACK,		///< Assume that we will destroy enemy obstacles
} FPATH_MOVETYPE;

typedef struct _jobNode
{
	PROPULSION_TYPE	propulsion;
	DROID_TYPE	droidType;
	int		destX, destY;
	int		origX, origY;
	UDWORD		droidID;
	struct _jobNode	*next;
	FPATH_MOVETYPE	moveType;
	int		owner;		///< Player owner
	struct PathBlockingMap *blockingMap;  ///< Map of blocking tiles.
	bool		acceptNearest;
} PATHJOB;

typedef enum _fpath_retval
{
	FPR_OK,         ///< found a route
	FPR_FAILED,     ///< failed to find a route
	FPR_WAIT,       ///< route is being calculated by the path-finding thread
} FPATH_RETVAL;

/** Initialise the path-finding module.
 */
extern BOOL fpathInitialise(void);

/** Shutdown the path-finding module.
 */
extern void fpathShutdown(void);

extern void fpathUpdate(void);

/** Find a route for a droid to a location.
 */
extern FPATH_RETVAL fpathDroidRoute(DROID* psDroid, SDWORD targetX, SDWORD targetY, FPATH_MOVETYPE moveType);

/// Returns true iff the parameters have equivalent behaviour in fpathBaseBlockingTile.
bool fpathIsEquivalentBlocking(PROPULSION_TYPE propulsion1, int player1, FPATH_MOVETYPE moveType1,
                               PROPULSION_TYPE propulsion2, int player2, FPATH_MOVETYPE moveType2);

/** Function pointer to the currently in-use blocking tile check function.
 *  
 *  This function will check if the map tile at the given location should be considered to block droids
 *  with the currently selected propulsion type. This is not identical to whether it will actually block,
 *  which can depend on hostilities and open/closed attributes.
 *
 * fpathBlockingTile -- when it is irrelevant who owns what buildings, they all block unless propulsion is right
 * fpathDroidBlockingTile -- when you may want to factor the above into account
 * fpathBaseBlockingTile -- set all parameters; the others are convenience functions for this one
 *
 *  @return true if the given tile is blocking for this droid
 */
BOOL fpathBlockingTile(SDWORD x, SDWORD y, PROPULSION_TYPE propulsion);
BOOL fpathDroidBlockingTile(DROID *psDroid, int x, int y, FPATH_MOVETYPE moveType);
BOOL fpathBaseBlockingTile(SDWORD x, SDWORD y, PROPULSION_TYPE propulsion, int player, FPATH_MOVETYPE moveType);

/** Set a direct path to position.
 *
 *  Plan a path from @c psDroid's current position to given position without
 *  taking obstructions into consideration.
 *
 *  Used for instance by VTOLs. Function is thread-safe.
 */
extern void fpathSetDirectRoute(DROID* psDroid, SDWORD targetX, SDWORD targetY);

/** Clean up path jobs and results for a droid. Function is thread-safe. */
extern void fpathRemoveDroidData(int id);

/** Check LOS (Line Of Sight) between two world positions.
 */
extern BOOL fpathTileLOS(DROID *psDroid, Vector3i dest);

/** Quick O(1) test of whether it is theoretically possible to go from origin to destination
 *  using the given propulsion type. orig and dest are in world coordinates. */
bool fpathCheck(Position orig, Position dest, PROPULSION_TYPE propulsion);

/** Unit testing. */
void fpathTest(int x, int y, int x2, int y2);

/** @} */

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INCLUDED_SRC_FPATH_H__
