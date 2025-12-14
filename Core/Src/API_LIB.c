/**
 * @file API_LIB.c
 * @brief API library source file
 *
 * This file contains the source code for all the
 * different API functions.
 *
 * @author Tom Veldkamp, Xander Perry & Daniël Wit Arendza
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

/**
 * @brief Draws a rectangle on the VGA display.
 *
 * This function draws a rectangle defined by its top-left corner (x, y),
 * width, and height. It can draw either an outline or a filled rectangle
 * based on the 'filled' parameter.
 *
 * @param x         Starting X-coordinate (top-left corner) of the rectangle.
 * @param y         Starting Y-coordinate (top-left corner) of the rectangle.
 * @param width     Width of the rectangle in pixels.
 * @param height    Height of the rectangle in pixels.
 * @param color     8-bit color value used to draw the rectangle.
 * @param filled    Flag indicating whether the rectangle should be filled (non-zero for filled, 0 for outline).
 * @param weight  Reserved variable (unused).
 * @param bordercolor Reserved variable (unused).
 *
 * @note If 'filled' is 0, the function typically calls an API to draw four lines.
 * If 'filled' is non-zero, the function typically iterates and calls an API to draw horizontal lines.
 */
int API_draw_rectangle (int x, int y, int width, int height, int color, int filled, int weight, int bordercolor)
{

	//!  Error handling
	/*!
	  Code block below handles errors. If the parameters get values which are out of bounds or wrong, return a corresponding error code.
	*/
	if (color != VGA_COL_BLACK &&
	    color != VGA_COL_BLUE  &&
	    color != VGA_COL_GREEN &&
	    color != VGA_COL_RED   &&
	    color != VGA_COL_WHITE &&
	    color != VGA_COL_CYAN  &&
	    color != VGA_COL_MAGENTA &&
	    color != VGA_COL_YELLOW)
		return ERR_COLOR_INVALID;

	if (weight < 0) return ERR_WEIGHT_INVALID;
//	if (x < 0 || x > VGA_DISPLAY_X) return ERROR_X_OUT_OF_BOUND;
//	if (y < 0 || x > VGA_DISPLAY_Y) return ERROR_Y_OUT_OF_BOUND;
	if (width < 0) return ERR_RECT_WIDTH_INVALID;
	if (height < 0) return ERR_RECT_HEIGHT_INVALID;


	int xEnd = x + width - 1; 	/**< Calculate the second X coordinate. */
	int yEnd = y + height - 1;	/**< Calculate the second Y coordinate. */

	if (xEnd > VGA_DISPLAY_X || yEnd > VGA_DISPLAY_Y) return ERR_OBJ_OUT_OF_BOUNDS;


	int i,j;

	if(filled)
	{
		for( i = x; i <= xEnd; i++)
		{The width parameter is 0 or negative, resulting in an empty rectangle

			for(j = y; j <= yEnd; j++)
			{
				UB_VGA_SetPixel(i, j, color);
			}
		}

		API_draw_line(x, y, x, yEnd, weight, bordercolor, 0); /**< Draw a borderline around the rectangle> */
		API_draw_line(x, y, xEnd, y, weight, bordercolor, 0);
		API_draw_line(xEnd, yEnd, x, yEnd, weight, bordercolor, 0);
		API_draw_line(xEnd, yEnd, xEnd, y, weight, bordercolor, 0);
	}

	else
	{
		/**<  Draw horizontal lines (Top and Bottom) */
		for ( i = x; i <= xEnd; i++)
		{
			UB_VGA_SetPixel(i, y, color);        /**<  Top edge */
			UB_VGA_SetPixel(i, yEnd, color);    /**<  Bottom edge */
		}

		/**< Draw vertical lines (Left and Right)
		 We start/end at y+1 / y_end-1 to avoid drawing the corners twice */
		for (j = y + 1; j <= yEnd - 1; j++)
		{
			UB_VGA_SetPixel(x, j, color);        /**<  Left edge */
			UB_VGA_SetPixel(xEnd, j, color);    /**< Right edge */
		}
	}
    return 0; /**<  Return 0 on success */
}
/**
 * @brief Draws a bitmap on the screen at the specified upper-left position.
 *
 * This function takes a bitmap index from a global bitmap array and renders it
 * at the given (x, y) coordinates on the screen. Each bitmap is expected to have
 * the following format:
 * - bmp[0] = height of the bitmap in pixels
 * - bmp[1] = width of the bitmap in pixels
 * - bmp[2..] = pixel color data (row-major order)
 *
 * @param x_lup The x-coordinate of the upper-left pixel where the bitmap will be drawn.
 * @param y_lup The y-coordinate of the upper-left pixel where the bitmap will be drawn.
 * @param bitnr The index of the bitmap in the global `bitmaps` array.
 *
 * @note The function assumes that the `bitmaps` array and `UB_VGA_SetPixel` function
 *       are properly defined and accessible.
 */
void API_draw_bitmap(int x_lup, int y_lup, int bitnr)
{
	const char* bmp = bitmaps[bitnr];/**< Select bitmap from argument. */
	uint8_t height = bmp[0];
	uint8_t width  = bmp[1];
	const uint8_t* pixels = &bmp[2];

    for (uint16_t y = 0; y < height; y++) /**< Move up y coordinate. */
    {
        for (uint16_t x = 0; x < width; x++) /**< Move up x coordinate. */
        {
            uint16_t color = pixels[y * width + x];
            UB_VGA_SetPixel(x+x_lup, y+y_lup, color); /**< Color current pixel. */
        }
    }
}
 /* @brief Draws a figure based on 5 coordinates
 *
 * @param x_1 		X-coordinate 1
 * @param y_1 		Y-coordinate 1
 * @param x_2 		X-coordinate 2
 * @param y_2 		Y-coordinate 2
 * @param x_3 		X-coordinate 3
 * @param y_3 		Y-coordinate 3
 * @param x_4 		X-coordinate 4
 * @param y_4 		Y-coordinate 4
 * @param x_5 		X-coordinate 5
 * @param y_5		Y-coordinate 5
 * @param color		Figure color
 * @param filled	Figure Fill
 *
 * @return			0 if no errors occured, otherwise returns the error code.
 *
 * @note This function calls API_draw_line().
 *
 */
int API_draw_figure(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x_4, int y_4, int x_5, int y_5, int color, int filled)
{
	// Draw outline
	API_draw_line(x_1, y_1, x_2, y_2, 1, color, 0);
	API_draw_line(x_2, y_2, x_3, y_3, 1, color, 0);
	API_draw_line(x_3, y_3, x_4, y_4, 1, color, 0);
	API_draw_line(x_4, y_4, x_5, y_5, 1, color, 0);
	API_draw_line(x_5, y_5, x_1, y_1, 1, color, 0);

	// Draw infill if needed
	if(filled)
	{
		// Get coordinates for smallest box around polygon
		int min_x = x_1;
		if (x_2 < min_x)	min_x = x_2;
		if (x_3 < min_x)	min_x = x_3;
		if (x_4 < min_x)	min_x = x_4;
		if (x_5 < min_x)	min_x = x_5;

		int max_x = x_1;
		if (x_2 > max_x)	max_x = x_2;
		if (x_3 > max_x)	max_x = x_3;
		if (x_4 > max_x)	max_x = x_4;
		if (x_5 > max_x)	max_x = x_5;

		int min_y = y_1;
		if (y_2 < min_y)	min_y = y_2;
		if (y_3 < min_y)	min_y = y_3;
		if (y_4 < min_y)	min_y = y_4;
		if (y_5 < min_y)	min_y = y_5;

		int max_y = y_1;
		if (y_2 > max_y)	max_y = y_2;
		if (y_3 > max_y)	max_y = y_3;
		if (y_4 > max_y)	max_y = y_4;
		if (y_5 > max_y)	max_y = y_5;

		// Check if pixels in smallest box are within polygon
		for (int i = min_x; i <= max_x; i++)
		{
			for (int j = min_y; j <= max_y; j++)
			{
				if (_IsInPolygon(i, j, x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4, x_5, y_5))
				{
					// Color pixel if within polygon
					UB_VGA_SetPixel(i, j, color);
				}
			}
		}
	}
}

/**
 * @brief Returns smaller of two numbers
 *
 * @param a 		Number 1
 * @param b 		Number 2
 *
 * @return			Smaller of the two numbers.
 */
int _Min(int a, int b)
{
	return a < b ? a : b;
}

/**
 * @brief Returns bigger of two numbers
 *
 * @param a 		Number 1
 * @param b 		Number 2
 *
 * @return			Bigger of the two numbers. *
 */
int _Max(int a, int b)
{
	return a > b ? a : b;
}

/**
 * @brief Calculates cross product of three coordinates.
 *
 * @param x_1 		X-coordinate 1
 * @param y_1 		Y-coordinate 1
 * @param x_2 		X-coordinate 2
 * @param y_2 		Y-coordinate 2
 * @param x_3 		X-coordinate 3
 * @param y_3 		Y-coordinate 3
 *
 * @return			Cross prodcuct.
 */
int _CrossProduct(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3)
{
	return (x_2 - x_1) * (y_3 - y_1) - (y_2 - y_1) * (x_3 - x_1);
}

/**
 * @brief determines if point is on segment 1-2.
 *
 * @param x_p 		X-coordinate of point
 * @param y_p 		Y-coordinate of point
 * @param x_1 		X-coordinate 1
 * @param y_1 		Y-coordinate 1
 * @param x_2 		X-coordinate 2
 * @param y_2 		Y-coordinate 2
 *
 * @return			1 if on segment, 0 if not.
 */
int _IsOnSegment(int x_p, int y_p, int x_1, int y_1, int x_2, int y_2)
{
	// Check collinearity
	if (_CrossProduct(x_1, y_1, x_p, y_p, x_2, y_2) != 0)
	{
		return 0;
	}

	// Check whether values are within bouding box
	if (x_p >= _Min(x_1, x_2 ) && x_p <= _Max(x_1, x_2) &&
		y_p >= _Min(y_1, y_2 ) && y_p <= _Max(y_1, y_2))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

/**
 * @brief determines if point is on segment 1-2.
 *
 * @param x_p 		X-coordinate of point
 * @param y_p 		Y-coordinate of point
 * @param x_1 		X-coordinate 1
 * @param y_1 		Y-coordinate 1
 * @param x_2 		X-coordinate 2
 * @param y_2 		Y-coordinate 2
 * @param x_3 		X-coordinate 3
 * @param y_3 		Y-coordinate 3
 * @param x_4 		X-coordinate 4
 * @param y_4 		Y-coordinate 4
 * @param x_5 		X-coordinate 5
 * @param y_5		Y-coordinate 5
 *
 * @return			1 if on segment, 0 if not.
 */
int _IsInPolygon(int x_p, int y_p, int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x_4, int y_4, int x_5, int y_5)
{
	// Make list of polygon coordinates for iteration
	int poly_x[] = {x_1, x_2, x_3, x_4, x_5};
	int poly_y[] = {y_1, y_2, y_3, y_4, y_5};

	// Winding number tracker
	int winding_number = 0;

	// Loop over polygon sides
	for (int i = 0; i < 5; i++)
	{
		// Get polygon side from current coordinate to next coordinate
		int current_x = poly_x[i];
		int current_y = poly_y[i];
		int next_x = poly_x[(i+1)%5];
		int next_y = poly_y[(i+1)%5];

		// If point is on segment it is within polygon
		if (_IsOnSegment(x_p, y_p, current_x, current_y, next_x, next_y))
		{
			return 1;
		}

		// Get cross product
		int cross_product = _CrossProduct(current_x, current_y, next_x, next_y, x_p, y_p);

		// Calculate winding number
		if (current_y <= y_p)
		{
			if (next_y > y_p && cross_product > 0)
			{
				winding_number++;
			}
		}
		else
		{
			if (next_y <= y_p && cross_product < 0)
			{
				winding_number--;
			}
		}
	}

	// Winding number is 0 if point is within polygon
	return (winding_number != 0);
}

/**
 * @brief Sets all pixels to given color.
 *
 * @param color		Screen color
 *
 * @return			0 if succesfull, otherwise error code
 */
int API_clearscreen (int color)
{
	UB_VGA_FillScreen(color);
	return 0;
}






