/**
 * @file LogicLayer.h
 * @brief Logic layer library header file
 *
 * This file contains the prototypes for logic layer functions
 *
 * @author Xander Perry
 * @date 2025-12-03
 */

#ifndef __LOGIC_LAYER_H
#define __LOGIC_LAYER_H

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

// Define error codes
#define ERR_UNSUPPORTED_COMMAND 300
#define ERR_INVALID_PARAM_INPUT 301
#define ERR_X_OUT_OF_BOUND 302
#define ERR_Y_OUT_OF_BOUND 303
#define ERR_INVALID_COLOR_INPUT 304

/**
 * @brief Receives the command string from the CmdForwarder, extracts the required function and parameters and calls the corresponding API function.
 * @param cmd Pointer to the command string buffer.
 * @return 0 if no errors occured, otherwise returns the error code.
 */
int CmdToFunc(char *cmd);

/**
 * @brief Translates a color string to a color code.
 * @param cmd Pointer to the color string buffer.
 * @return Color code if no errors occured, otherwise returns 1.
 */
uint8_t StrToCol (char *str);

/**
 * @brief Checks whether x coordinate is out of bounds.
 * @param x coordinate.
 * @return 0 if within bounds, 1 if out of bounds.
 */
bool XOutOfBound(uint16_t x);

/**
 * @brief Checks whether y coordinate is out of bounds.
 * @param y coordinate.
 * @return 0 if within bounds, 1 if out of bounds.
 */
bool YOutOfBound(uint16_t y);

#endif /* __UART_H */
