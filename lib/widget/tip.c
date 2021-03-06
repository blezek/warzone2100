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
 *  The tool tip display system.
 */

#include "lib/framework/frame.h"
#include "lib/framework/frameint.h"
#include "widget.h"
#include "widgint.h"
#include "tip.h"
// FIXME Direct iVis implementation include!
#include "lib/ivis_common/pieblitfunc.h"

/* Time delay before showing the tool tip */
#define TIP_PAUSE	200

/* How long to display the tool tip */
#define TIP_TIME	4000

/* Size of border around tip text */
#define TIP_HGAP	6
#define TIP_VGAP	3


/* The tool tip state */
static enum _tip_state
{
	TIP_NONE,			// No tip, and no button hilited
	TIP_WAIT,			// A button is hilited, but not yet ready to show the tip
	TIP_ACTIVE,			// A tip is being displayed
} tipState;


static SDWORD		startTime;			// When the tip was created
static SDWORD		mx,my;				// Last mouse coords
static SDWORD		wx,wy,ww,wh;		// Position and size of button to place tip by
static SDWORD		tx,ty,tw,th;		// Position and size of the tip box
static SDWORD		fx,fy;				// Position of the text
static const char *pTip;				// Tip text
static PIELIGHT		*pColours;			// The colours for the tool tip
static WIDGET		*psWidget;			// The button the tip is for
static enum iV_fonts FontID = font_regular;	// ID for the Ivis Font.
static PIELIGHT TipColour;

/* Initialise the tool tip module */
void tipInitialise(void)
{
	tipState = TIP_NONE;
	TipColour = WZCOL_WHITE;
}

// Set the global toop tip text colour.
void widgSetTipColour(PIELIGHT colour)
{
	TipColour = colour;
}

/*
 * Setup a tool tip.
 * The tip module will then wait until the correct points to
 * display and then remove the tool tip.
 * i.e. The tip will not be displayed immediately.
 * Calling this while another tip is being displayed will restart
 * the tip system.
 * psSource is the widget that started the tip.
 * x,y,width,height - specify the position of the button to place the
 * tip by.
 */
void tipStart(WIDGET *psSource, const char *pNewTip, enum iV_fonts NewFontID,
					 PIELIGHT *pNewColours, SDWORD x, SDWORD y, UDWORD width, UDWORD height)
{
	ASSERT( psSource != NULL,
		"tipStart: Invalid widget pointer" );
//	ASSERT( pNewTip != NULL,
//		"tipStart: Invalid tip pointer" );
	ASSERT( pNewColours != NULL,
		"tipStart: Invalid colours pointer" );

	tipState = TIP_WAIT;
	startTime = SDL_GetTicks();
	mx = mouseX();
	my = mouseY();
	wx = x; wy = y;
	ww = width; wh = height;
	pTip = pNewTip;
	psWidget = psSource;
	FontID = NewFontID;
	pColours = pNewColours;
}


/* Stop a tool tip (e.g. if the hilite is lost on a button).
 * psSource should be the same as the widget that started the tip.
 */
void tipStop(WIDGET *psSource)
{
	ASSERT( psSource != NULL,
		"tipStop: Invalid widget pointer" );

	if (tipState != TIP_NONE && psSource == psWidget)
	{
		tipState = TIP_NONE;
	}
}


#define RIGHTBORDER		(0)
#define BOTTOMBORDER	(0)


/* Update and possibly display the tip */
void tipDisplay(void)
{
	SDWORD		newMX,newMY;
	SDWORD		currTime;
	SDWORD		fw, topGap;
//	UDWORD		time;

	switch (tipState)
	{
	case TIP_WAIT:
		/* See if the tip has to be shown */
		newMX = mouseX();
		newMY = mouseY();
		currTime = SDL_GetTicks();
		if (newMX == mx &&
			newMY == my &&
			(currTime - startTime > TIP_PAUSE))
		{
			/* Activate the tip */
			tipState = TIP_ACTIVE;

			/* Calculate the size of the tip box */
			topGap = TIP_VGAP;
			iV_SetFont(FontID);

			fw = iV_GetTextWidth(pTip);
			tw = fw + TIP_HGAP*2;
			th = topGap*2 + iV_GetTextLineSize()+iV_GetTextBelowBase();

			/* Position the tip box */
			tx = wx + (ww >> 1);
			ty = wy + wh + TIP_VGAP;

			/* Check the box is on screen */
			if (tx < 0)
			{
				tx = 0;
			}
			if (tx + tw >= (SDWORD)screenWidth-RIGHTBORDER)
			{
				tx = screenWidth-RIGHTBORDER - tw - 1;
			}
			if (ty < 0)
			{
				ty = 0;
			}
			if (ty + th >= (SDWORD)screenHeight-BOTTOMBORDER)
			{
				/* Position the tip above the button */
				ty = wy - th - TIP_VGAP;
			}


			/* Position the text */
			fx = tx + TIP_HGAP;

			fy = ty + (th - iV_GetTextLineSize())/2 - iV_GetTextAboveBase();


			/* Note the time */
			startTime = SDL_GetTicks();
		}
		else if (newMX != mx ||
				 newMY != my ||
				 mousePressed(MOUSE_LMB))
		{
			mx = newMX;
			my = newMY;
			startTime = currTime;
		}
		break;
	case TIP_ACTIVE:
		/* See if the tip still needs to be displayed */
//		time = GetTickCount();
//		if (mousePressed(MOUSE_LMB) ||
//			((time - startTime) > TIP_TIME))
//		{
//			tipState = TIP_NONE;
//			return;
//		}


		/* Draw the tool tip */
		pie_BoxFill(tx,ty, tx+tw, ty+th, pColours[WCOL_TIPBKGRND]);
		iV_Box(tx,ty, tx+tw-1, ty+th-1, pColours[WCOL_LIGHT]);
		iV_Line(tx+1, ty+th-2, tx+1,    ty+1, pColours[WCOL_DARK]);
		iV_Line(tx+2, ty+1,    tx+tw-2, ty+1, pColours[WCOL_DARK]);
		iV_Line(tx,	  ty+th,   tx+tw,   ty+th, pColours[WCOL_DARK]);
		iV_Line(tx+tw,ty+th-1, tx+tw,   ty, pColours[WCOL_DARK]);

		iV_SetFont(FontID);
		iV_SetTextColour(TipColour);
		iV_DrawText(pTip,fx,fy);

		break;
	default:
		break;
	}
}
