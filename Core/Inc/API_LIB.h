/**
 * @file API_LIB.h
 * @brief API library header file
 *
 * This file contains the prototypes for all the
 * different API functions.
 *
 * @author Tom Veldkamp, Xander Perry & Daniël Wit Arendza
 * @date 2025-12-03
 */

#ifndef INC_API_LIB_H_
#define INC_API_LIB_H_

#include <stdint.h>

// API_draw_rectangle error codes
#define ERR_RECT_WIDTH_INVALID            601  /**< The width parameter is 0 or negative, resulting in an empty rectangle. */
#define ERR_RECT_HEIGHT_INVALID           602  /**< The height parameter is 0 or negative, resulting in an empty rectangle. */
#define ERR_OBJ_OUT_OF_BOUNDS			  603  /**< Part of the object is out of bounds*/
#define ERR_COLOR_INVALID            	  604  /**< An unknown color value has been assigned to the function. (e.g. color > 255 for 8-bit) */
#define ERR_WEIGHT_INVALID           	  605  /**< Weight is below 0, which is not possible for line thickness. */
#define ERR_BITMAP_INVALID				  606  /**< Requested bitmap does not exist*/
#define ERR_CIR_RADIUS_INVALID			  607  /**< Requested radius is 0 or negative*/



//function prototype
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
int API_draw_line(int x_1, int y_1, int x_2, int y_2, int weight, int color, int reserved);

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
int API_draw_circle(int x0, int y0, int radius, int color, int reserved);
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
int API_draw_bitmap(int x_lup, int y_lup, int bitnr);
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
int API_draw_rectangle (int x, int y, int width, int height, int color, int filled, int weight, int bordercolor);
// e.g.: weight, bordercolor
 /* @brief Draws a figure based on 5 coordinates
 *
 * This function draws a polygon based on 5 coordinates using the API_draw_line function.
 * If infill is enabled the winding number algorithm is used to color pixels within the polygon.
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
 */
int API_draw_figure(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x_4, int y_4, int x_5, int y_5, int color, int filled);

/**
 * @brief Returns smaller of two numbers
 *
 * @param a 		Number 1
 * @param b 		Number 2
 *
 * @return			Smaller of the two numbers.
 */
int _Min(int a, int b);

/**
 * @brief Returns bigger of two numbers
 *
 * @param a 		Number 1
 * @param b 		Number 2
 *
 * @return			Bigger of the two numbers.
 */
int _Max(int a, int b);

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
int _CrossProduct(int x_1, int y_1, int x_2, int y_2, int x_3, int y_3);

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
int _IsOnSegment(int x_p, int y_p, int x_1, int y_1, int x_2, int y_2);

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
int _IsInPolygon(int x_p, int y_p, int x_1, int y_1, int x_2, int y_2, int x_3, int y_3, int x_4, int y_4, int x_5, int y_5);

/**
 * @brief Sets all pixels to given color.
 *
 * @param color		Screen color
 *
 * @return			0 if succesfull, otherwise error code
 */
int API_clearscreen (int color);

#endif /* INC_API_LIB_H_ */
