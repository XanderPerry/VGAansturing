#ifndef __UART_H
#define __UART_H

#include <stdint.h>
#include <stdbool.h>

// --- Configuration ---
#define UART_RX_BUFFER_SIZE 64 // Define the size of the circular buffer

// --- Global Variables (Extern Declarations) ---

// Circular buffer storage and indices
extern volatile uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
extern volatile uint16_t uart_rx_head; // Index where the next received byte will be stored
extern volatile uint16_t uart_rx_tail; // Index where the next byte will be read from

// Flag to indicate a complete line has been received (ending with LF)
extern volatile bool uart_rx_line_ready;

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

// --- Interrupt Reception ---
/**
 * @brief Enables the Receive Not Empty (RXNE) interrupt for USART2.
 */
void usart2_enable_rx_interrupt(void);

/**
 * @brief USART2 Interrupt Service Routine (ISR).
 * This function must be defined in the startup file's vector table.
 */
void USART2_IRQHandler(void);

/**
 * @brief Reads all characters from the buffer up to the first LF or the buffer limit.
 * @param dest Pointer to the destination string buffer.
 * @param max_len Maximum length of the destination buffer (including null terminator).
 * @return true if a line (terminated by LF) was successfully read, false otherwise.
 */
bool usart2_read_line(char *dest, uint16_t max_len);

#endif /* __UART_H */
