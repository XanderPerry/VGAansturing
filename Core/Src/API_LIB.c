//--------------------------------------------------------------
// File     : main.c
// Datum    : 03/12/2025
// Version  : 1.0
// Autor    : Tom Veldkamp
// mods by	:
// CPU      : STM32F4
// IDE      : STM32CubeIDE 1.19.0
//--------------------------------------------------------------
#include <API_LIB.h>
#include <stdlib.h>
#include "stm32_ub_vga_screen.h"

void API_draw_circle(int x0, int y0, int radius, uint16_t color)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;  // Decision criterion

    while (y <= x)
    {
        // Draw horizontal spans (fills circle)
        for (int xi = x0 - x; xi <= x0 + x; xi++)
        {
            UB_VGA_SetPixel(xi, y0 + y, color);
            UB_VGA_SetPixel(xi, y0 - y, color);
        }

        for (int xi = x0 - y; xi <= x0 + y; xi++)
        {
            UB_VGA_SetPixel(xi, y0 + x, color);
            UB_VGA_SetPixel(xi, y0 - x, color);
        }
        y++;
        if (decisionOver2 <= 0)
        {
            decisionOver2 += 2 * y + 1;
        }
        else
        {
            x--;
            decisionOver2 += 2 * (y - x) + 1;
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
void API_draw_line(uint16_t x_1, uint16_t y_1, uint16_t x_2, uint16_t y_2,
                   uint16_t weight, uint8_t color)
{
    int dx = abs(x_2 - x_1);
    int dy = abs(y_2 - y_1);

    int sx = (x_1 < x_2) ? 1 : -1;
    int sy = (y_1 < y_2) ? 1 : -1;

    int err = dx - dy;

    while (1)
    {
    	API_draw_circle(x_1, y_1, weight/2, color);

        if (x_1 == x_2 && y_1 == y_2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {  // horizontal movement
            err -= dy;
            x_1 += sx;
        }
        if (e2 < dx) {   // vertical movement
            err += dx;
            y_1 += sy;
        }
    }
}


