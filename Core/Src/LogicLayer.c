/**
 * @file LogicLayer.h
 * @brief Logic layer library header file
 *
 * This file contains the code for logic layer functions
 *
 * @author Xander Perry
 * @date 2025-12-03
 */

#include "LogicLayer.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "stm32_ub_vga_screen.h"
#include "API_LIB.h"

/**
 * @brief Receives the command string from the CmdForwarder, extracts the required function and parameters and calls the corresponding API function.
 * @param cmd Pointer to the command string buffer.
 * @return 0 if no errors occured, otherwise returns the error code.
 */
int CmdToFunc (char *cmd)
{
	const char delimiter[] = ",\r\n"; // Separation character
	char *token;
	uint16_t zero = 0; // All input array positions start pointing to zero
	char * input_buffer[12] = {zero}; // Buffer for input processing

	// Get requested function from command
	token = strtok (cmd, delimiter);

	if (strcmp(token, "pixel") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint16_t x = atoi (input_buffer[0]);
		if (XOutOfBound(x)) return ERR_X_OUT_OF_BOUND;

		uint16_t y = atoi (input_buffer[1]);
		if (YOutOfBound(y)) return ERR_Y_OUT_OF_BOUND;

		uint8_t col = StrToCol (input_buffer[2]);
		if (col==1) return ERR_INVALID_COLOR_INPUT;

		UB_VGA_SetPixel (x, y, col);
	}

	else if (strcmp(token, "lijn") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint16_t x_1 = atoi (input_buffer[0]);
		if (XOutOfBound(x_1))return ERR_X_OUT_OF_BOUND;

		uint16_t y_1 = atoi (input_buffer[1]);
		if (YOutOfBound(y_1)) return ERR_Y_OUT_OF_BOUND;

		uint16_t x_2 = atoi (input_buffer[2]);
		if (XOutOfBound(x_2)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_2 = atoi (input_buffer[3]);
		if (YOutOfBound(y_2)) return ERR_Y_OUT_OF_BOUND;

		uint8_t col = StrToCol (input_buffer[4]);
		if (col==1) return ERR_INVALID_COLOR_INPUT;

		uint16_t weight = atoi (input_buffer[5]);

		uint16_t reserved = 0;
		if (input_buffer[6])
		{
			reserved = atoi (input_buffer[6]);
		}

		int ErrorCode = API_draw_line(x_1, y_1, x_2, y_2, weight, col, reserved);
		if (ErrorCode)
		{
			return ErrorCode;
		}
	}

	else if (strcmp(token, "rechthoek") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint16_t x = atoi (input_buffer[0]);
		if (XOutOfBound(x))	return ERR_X_OUT_OF_BOUND;

		uint16_t y = atoi (input_buffer[1]);
		if (YOutOfBound(y))	return ERR_Y_OUT_OF_BOUND;

		uint16_t width = atoi (input_buffer[2]);

		uint16_t height = atoi (input_buffer[3]);

		uint8_t color = StrToCol (input_buffer[4]);
		if (color==1) return ERR_INVALID_COLOR_INPUT;

		uint16_t filled = atoi (input_buffer[5]);

		uint16_t weight = 1;
		if (input_buffer[6])
		{
			weight = atoi (input_buffer[6]);
		}

		uint8_t bordercolor = color;
		if (input_buffer[7])
		{
			bordercolor = StrToCol (input_buffer[7]);
			if (bordercolor==1) return ERR_INVALID_COLOR_INPUT;
		}

		int ErrorCode = API_draw_rectangle(x, y, width, height, color, filled, weight, bordercolor);
		if (ErrorCode)
		{
			return ErrorCode;
		}
	}

	else if (strcmp(token, "bitmap") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint16_t bitnr = atoi (input_buffer[0]);

		uint16_t x_lup = atoi (input_buffer[1]);
		if (XOutOfBound(x_lup)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_lup = atoi (input_buffer[2]);
		if (YOutOfBound(y_lup))	return ERR_Y_OUT_OF_BOUND;

		int ErrorCode = API_draw_bitmap(x_lup, y_lup, bitnr);
		if (ErrorCode)
		{
			return ErrorCode;
		}
	}

	else if (strcmp(token, "clearscherm") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint8_t color = StrToCol (input_buffer[0]);
		if (color==1) return ERR_INVALID_COLOR_INPUT;

		API_clearscreen(color);
	}

	else if (strcmp(token, "cirkel") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint16_t x0 = atoi (input_buffer[0]);
		if (XOutOfBound(x0)) return ERR_X_OUT_OF_BOUND;

		uint16_t y0 = atoi (input_buffer[1]);
		if (YOutOfBound(y0)) return ERR_Y_OUT_OF_BOUND;

		uint16_t radius = atoi (input_buffer[2]);

		uint8_t color = StrToCol (input_buffer[3]);
		if (color==1) return ERR_INVALID_COLOR_INPUT;

		uint8_t reserved = 0;
		if (input_buffer[4])
		{
			reserved = atoi (input_buffer[4]);
		}

		int ErrorCode = API_draw_circle(x0, y0, radius, color, reserved);
		if (ErrorCode)
		{
			return ErrorCode;
		}
	}

	else if (strcmp(token, "figuur") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		uint16_t x_1 = atoi (input_buffer[0]);
		if (XOutOfBound(x_1)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_1 = atoi (input_buffer[1]);
		if (YOutOfBound(y_1)) return ERR_Y_OUT_OF_BOUND;

		uint16_t x_2 = atoi (input_buffer[2]);
		if (XOutOfBound(x_2)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_2 = atoi (input_buffer[3]);
		if (YOutOfBound(y_2)) return ERR_Y_OUT_OF_BOUND;

		uint16_t x_3 = atoi (input_buffer[4]);
		if (XOutOfBound(x_3)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_3 = atoi (input_buffer[5]);
		if (YOutOfBound(y_3)) return ERR_Y_OUT_OF_BOUND;

		uint16_t x_4 = atoi (input_buffer[6]);
		if (XOutOfBound(x_4)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_4 = atoi (input_buffer[7]);
		if (YOutOfBound(y_4)) return ERR_Y_OUT_OF_BOUND;

		uint16_t x_5 = atoi (input_buffer[8]);
		if (XOutOfBound(x_5)) return ERR_X_OUT_OF_BOUND;

		uint16_t y_5 = atoi (input_buffer[9]);
		if (YOutOfBound(y_5)) return ERR_Y_OUT_OF_BOUND;

		uint8_t color = StrToCol (input_buffer[10]);
		if (color==1) return ERR_INVALID_COLOR_INPUT;

		uint16_t filled = 0;
		if (input_buffer[11])
		{
			filled = atoi (input_buffer[11]);
		}

		int ErrorCode = API_draw_figure(x_1, y_1, x_2, y_2, x_3, y_3, x_4, y_4, x_5, y_5, color, filled);
		if (ErrorCode)
		{
			return ErrorCode;
		}
	}

	else if (strcmp(token, "tekst") == 0)
	{
		// Fill input_buffer[] with command parameters
		char i = 0;
		char * ptr = strtok (NULL, delimiter);
		while(ptr != NULL)
		{
			input_buffer[i++] = ptr;
			ptr = strtok (NULL, delimiter);
		}

		// 1. Get X Coordinate
		uint16_t x = atoi (input_buffer[0]);
		if (XOutOfBound(x)) return ERR_X_OUT_OF_BOUND;

		// 2. Get Y Coordinate
		uint16_t y = atoi (input_buffer[1]);
		if (YOutOfBound(y)) return ERR_Y_OUT_OF_BOUND;

		// 3. Get Color
		uint8_t color = StrToCol (input_buffer[2]);
		if (color==1) return ERR_INVALID_COLOR_INPUT;

		// 4. Get Text String
		char *text_string = input_buffer[3];
		// Basic safety check to ensure text exists
		if (text_string == 0) return ERR_INVALID_PARAM_INPUT; // Ensure this error is defined, or return a generic error

		// 5. Get Font Name
		char *font_name = input_buffer[4];

		// 6. Get Font Size (Optional, default to 1)
		uint16_t fontsize = 1;
		if (input_buffer[5])
		{
			fontsize = atoi (input_buffer[5]);
		}

		// 7. Get Font Style (Optional, default to 1 for Normal)
		// 1 = Normal, 2 = Bold, 3 = Italic
		char *fontstyle = input_buffer[6];

		// Call the API function
		int ErrorCode = API_draw_text(x, y, color, text_string, font_name, fontsize, fontstyle);
		if (ErrorCode)
		{
			return ErrorCode;
		}
	}
	else
	{
		// Return error for unsupported command.
		return ERR_UNSUPPORTED_COMMAND;
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
	else if (strcmp(str, "lichtblauw") == 0)
		return VGA_COL_LIGHT_BLUE;
	else if (strcmp(str, "groen") == 0)
		return VGA_COL_GREEN;
	else if (strcmp(str, "lichtgroen") == 0)
		return VGA_COL_LIGHT_GREEN;
	else if (strcmp(str, "rood") == 0)
		return VGA_COL_RED;
	else if (strcmp(str, "lichtrood") == 0)
		return VGA_COL_LIGHT_RED;
	else if (strcmp(str, "wit") == 0)
		return VGA_COL_WHITE;
	else if (strcmp(str, "cyaan") == 0)
		return VGA_COL_CYAN;
	else if (strcmp(str, "lichtcyaan") == 0)
		return VGA_COL_LIGHT_CYAN;
	else if (strcmp(str, "magenta") == 0)
		return VGA_COL_MAGENTA;
	else if (strcmp(str, "lichtmagenta") == 0)
		return VGA_COL_LIGHT_MAGENTA;
	else if (strcmp(str, "geel") == 0)
		return VGA_COL_YELLOW;
	else if (strcmp(str, "bruin") == 0)
		return VGA_COL_BROWN;
	else if (strcmp(str, "grijs") == 0)
		return VGA_COL_GRAY;

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
