#include "LogicLayer.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stm32_ub_vga_screen.h"

/**
 * @brief Receives the command string from the CmdForwarder, extracts the required function and parameters and calls the corresponding API function.
 * @param cmd Pointer to the command string buffer.
 * @return 0 if no errors occured, otherwise returns the error code.
 */
int CmdToFunc (char *cmd)
{
	const char delimiter[] = ",\r\n"; // Separation character
	char *token;

	// Get requested function from command
	token = strtok (cmd, delimiter);

	if (strcmp(token, "pixel") == 0)
	{
		uint16_t x = atoi (strtok (NULL, delimiter));
		if (XOutOfBound(x))
		{
			return ERROR_X_OUT_OF_BOUND;
		}

		uint16_t y = atoi (strtok (NULL, delimiter));
		if (YOutOfBound(y))
		{
			return ERROR_Y_OUT_OF_BOUND;
		}

		uint8_t col = StrToCol (strtok (NULL, delimiter));
		if (col==1)
		{
			return ERROR_INVALID_PARAM_INPUT;
		}

		UB_VGA_SetPixel (x, y, col);
	}

	else
	{
		// Return error for unsupported command.
		return ERROR_UNSUPPORTED_COMMAND;
	}

	return 0;
}

/**
 * @brief Translates a color string to a color code.
 * @param cmd Pointer to the color string buffer.
 * @return Color code if no errors occured, otherwise returns 1.
 */
uint8_t StrToCol (char *str)
{
	// Compare strings to colors and return color code.
	if (strcmp(str, "zwart") == 0)
		return VGA_COL_BLACK;
	else if (strcmp(str, "blauw") == 0)
		return VGA_COL_BLUE;
	else if (strcmp(str, "groen") == 0)
		return VGA_COL_GREEN;
	else if (strcmp(str, "rood") == 0)
		return VGA_COL_RED;
	else if (strcmp(str, "zwart") == 0)
		return VGA_COL_WHITE;
	else if (strcmp(str, "lichtcyaan") == 0)
		return VGA_COL_CYAN;
	else if (strcmp(str, "magenta") == 0)
		return VGA_COL_MAGENTA;
	else if (strcmp(str, "geel") == 0)
		return VGA_COL_YELLOW;

	// Return 1 if color not found.
	else
		return 1;
}

/**
 * @brief Checks whether x coordinate is out of bounds.
 * @param x coordinate.
 * @return 0 if within bounds, 1 if out of bounds.
 */
bool XOutOfBound(uint16_t x)
{
	if ( (x >= 0) && (x <= VGA_DISPLAY_X))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief Checks whether y coordinate is out of bounds.
 * @param y coordinate.
 * @return 0 if within bounds, 1 if out of bounds.
 */
bool YOutOfBound(uint16_t y)
{
	if ( (y >= 0) && (y <= VGA_DISPLAY_Y))
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
