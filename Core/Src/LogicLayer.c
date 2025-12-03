#include "LogicLayer.h"
#include "stm32f4xx.h"

#include <stdlib.h>
#include <string.h>

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
		uint16_t y = atoi (strtok (NULL, delimiter));
		uint16_t col = StrToCol (strtok (NULL, delimiter));
		UB_VGA_SetPixel (x, y, col);
	}

	else
	{
		return ERROR_UNSUPPORTED_COMMAND; // EDIT: RETURN ERRORCODE FOR UNRECOGNIZED COMMAND
	}

	return 0;
}

uint8_t StrToCol (char *str)
{
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
	else
		return NULL;
}

