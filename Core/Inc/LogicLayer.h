#ifndef __LOGIC_LAYER_H
#define __LOGIC_LAYER_H

#include "stm32f4xx.h"
#include <stdint.h>

// Define error codes
#define ERROR_UNSUPPORTED_COMMAND 300


/**
 * @brief Receives the command string from the CmdForwarder, extracts the required function and parameters and calls the corresponding API function.
 * @param cmd Pointer to the command string buffer.
 * @return 0 if no errors occured, otherwise returns the error code.
 */
int CmdToFunc(char *cmd);

uint8_t StrToCol (char *str);

#endif /* __UART_H */
