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
void API_draw_line(int x_1, int y_1, int x_2, int y_2, int weight, int color, int reserved);

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
void API_draw_circle(int x0, int y0, int radius, int color, int reserved);

/**
 * @brief Draws a figure based on 5 coordinates
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

#endif /* INC_API_LIB_H_ */
