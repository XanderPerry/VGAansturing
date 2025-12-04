/**
 * @file main.c
 * @brief API library header file
 *
 * This file contains the prototypes for all the
 * different API functions, currently API_draw_circle and
 * API_draw_line
 *
 * @author Tom Veldkamp
 * @date 2025-12-03
 */

#ifndef INC_API_LIB_H_
#define INC_API_LIB_H_

#include <stdint.h>
//function prototype
void API_draw_line(uint16_t x_1, uint16_t y_1, uint16_t x_2, uint16_t y_2, uint16_t weight, uint8_t color);
void API_draw_circle(int x0, int y0, int radius, uint16_t color);
#endif /* INC_API_LIB_H_ */
