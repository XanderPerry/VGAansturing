#ifndef __UART_H
#define __UART_H

#include <stdint.h>

// --- Initialization ---
/**
 * @brief Initializes USART2 for 115200 baud, 8N1 (Register-level)
 */
void usart2_init(void);

// --- Polling Transmission ---
/**
 * @brief Sends a single byte (character) via USART2.
 * @param data The byte to send.
 */
void usart2_send_char(uint8_t data);

/**
 * @brief Sends a null-terminated string via USART2.
 * @param str Pointer to the string to send.
 */
void usart2_send_string(char *str);

// --- Polling Reception ---
/**
 * @brief Receives a single byte (character) via USART2. Blocking/Polling.
 * @return The received byte.
 */
uint8_t usart2_receive_char(void);


// Optional: For Interrupt-based reception later
// void usart2_enable_rx_interrupt(void);

#endif /* __UART_H */
