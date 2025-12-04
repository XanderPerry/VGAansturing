/**
 * @file main.c
 * @brief API library source file
 *
 * This file contains the source code for all the
 * different API functions, currently API_draw_circle and
 * API_draw_line
 *
 * @author Tom Veldkamp
 * @date 2025-12-03
 */
#include <API_LIB.h>
#include <stdlib.h>
#include "stm32_ub_vga_screen.h"
/**
 * @brief Draws a filled circle on the VGA display.
 *
 * This function uses the midpoint circle algorithm to draw a filled circle
 * centered at the specified coordinates. The circle is filled by drawing
 * horizontal spans between symmetric points on each iteration.
 *
 * @param x0     	X-coordinate of the circle center.
 * @param y0     	Y-coordinate of the circle center.
 * @param radius 	Radius of the circle in pixels.
 * @param color  	16-bit color value used to fill the circle.
 * @parma reserved	Reserved variable
 *
 * @note This function requires the UB_VGA_SetPixel() function to be
 *       available for setting individual pixels on the display.
 *
 * @warning No boundary checking is performed. Ensure the circle fits
 *          within the screen to avoid memory access errors.
 */
void API_draw_circle(int x0, int y0, int radius, int color, int reserved)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;  /**< Bresenham decision variable, distance between ideal circle. */

    while (y <= x) /**< Filling circle outline. */
    {
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

/**
 * @brief Draws a line with adjustable thickness on the VGA display.
 *
 * This function implements Bresenham's line algorithm to efficiently draw a line
 * between two points using only integer arithmetic. Line thickness is achieved by
 * drawing a filled circle (brush) at each point along the path.
 *
 * @param x_1     	Starting X-coordinate of the line.
 * @param y_1     	Starting Y-coordinate of the line.
 * @param x_2     	Ending X-coordinate of the line.
 * @param y_2     	Ending Y-coordinate of the line.
 * @param weight  	Thickness of the line in pixels. The value represents the diameter;
 *                	internally, a circle with radius = weight/2 is drawn as the brush.
 * @param color   	8-bit color value used to draw the line.
 * @param reserved	Reserved variable
 *
 * @note This function calls API_draw_circle() to simulate line thickness.
 *       For very large weights, performance may decrease due to repeated circle drawing.
 *
 * @warning No boundary checking is performed. Ensure that the line (including brush size)
 *          fits within the display memory region to avoid invalid pixel writes.
 */
void API_draw_line(int x_1, int y_1, int x_2, int y_2,
                   int weight, int color, int reserved)
{
    int dx = abs(x_2 - x_1); /**< Horizontal distance between points. */
    int dy = abs(y_2 - y_1); /**< Vertical distance between points. */

    int sx = (x_1 < x_2) ? 1 : -1; /**< Step direction in X (+1 or -1). */
    int sy = (y_1 < y_2) ? 1 : -1; /**< Step direction in Y (+1 or -1). */

    int err = dx - dy; /**< Bresenham's error term dictating the distance to the ideal mathematical line */

    while (1)
    {
    	API_draw_circle(x_1, y_1, weight/2, color, 0); /**< Brush for drawing lines. */

        if (x_1 == x_2 && y_1 == y_2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {  /**< Horizontal movement. */
            err -= dy;
            x_1 += sx;
        }
        if (e2 < dx) {   /**< Vertical movement. */
            err += dx;
            y_1 += sy;
        }
    }
}


