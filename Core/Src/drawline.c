//--------------------------------------------------------------
// File     : main.c
// Datum    : 03/12/2025
// Version  : 1.0
// Autor    : Tom Veldkamp
// mods by	:
// CPU      : STM32F4
// IDE      : STM32CubeIDE 1.19.0
//--------------------------------------------------------------
#include "drawline.h"
#include <stdlib.h>

extern void UB_VGA_SetPixel(uint16_t xp, uint16_t yp, uint8_t color);

static void drawSquare(int x, int y, int thickness, uint16_t color)
{
	int radius = thickness / 2; // Radius from center


	// Draw a thickness × thickness square
	for (int distY = -radius; distY <= radius; distY++)
	{
		for (int distX = -radius; distX <= radius; distX++)
		{
			UB_VGA_SetPixel(x + distX, y + distY, color);
		}
	}
}
/*
 * Draw a line using Bresenham's Line Algorithm.
 * Works using ONLY integer math.
 *
 * (x0, y0) : start point
 * (x1, y1) : end point
 * thickness : line width in pixels
 * color : line color
 */
void VGADrawLine(uint16_t x0, uint16_t y0, uint16_t xp, uint16_t yp, uint16_t thickness, uint8_t color)
{
	// Distance between endpoints
	uint16_t distX = abs(xp-x0);
	uint16_t distY = abs(yp-y0);

    // Determine X and Y step directions (±1)
	int8_t stepDirX = x0 < xp ? 1: -1;
	int8_t stepDirY = y0 < yp ? 1: -1;

	//how far we are drifting from ideal mathematical line(this is a fraction)
	uint16_t driftError= distX-distY;

	while(1)
	{
		//draw square at current position
		drawSquare(x0, y0, thickness, color);

		if(x0 == xp && y0 == yp)
			break;

		int dEx2 = driftError * 2; //multiply by two to avoid floating point math

		//check horizontal movement
		if(dEx2 >= distY)
		{
			driftError += distY;//update error because increase in X
			x0 += stepDirX;//increase X
		}

		//check vertical movement
		if (dEx2 <= distX)
		{
			driftError += distX;//update error because increase in Y
			y0 += stepDirY;//increase Y
		}
	}
}

