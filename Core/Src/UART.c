/**
 * @file uart.c
 * @brief UART code file
 *
 * This file contains the code required to make use of uarts
 *
 *
 * @author Tom Veldkamp, Xander Perry & Daniël Wit Ariza
 * @date 2026-1-13
 */

#include "uart.h"
#include "stm32f4xx.h"
#include <string.h>
#include <stdbool.h>

// --- Global Buffer Definitions ---

volatile uint8_t uart_rx_buffer[UART_RX_BUFFER_SIZE];
volatile uint16_t uart_rx_head = 0; // Write index (updated by ISR)
volatile uint16_t uart_rx_tail = 0; // Read index (updated by main loop)
volatile bool uart_rx_line_ready = false; // Flag for a complete line (LF received)

/**
 * @brief Initializes USART2 for 115200 baud, 8N1 (Register-level)
 * Steps: Clock -> GPIO -> Baud Rate -> Enable
 */
void usart2_init(void)
{
    // --- Clock Configuration ---

    // Enable GPIOA clock (AHB1)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Bit 0 is GPIOAEN

    // Enable USART2 clock (APB1)
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Bit 17 is USART2EN


    // --- GPIO Configuration (PA3=TX, PA2=RX) ---

    // Configure PA2 and PA3 for Alternate Function (AF) mode (Mode = 10)
    // Clear bits 4-5 (PA2) and 6-7 (PA3) and then set them to 10b
    GPIOA->MODER &= ~( (3 << 4) | (3 << 6) );
    GPIOA->MODER |= ( (2 << 4) | (2 << 6) );

    // Configure Alternate Function Register Low (AFRL) to AF7 for PA2 and PA3
    // AF7 is 0111
    GPIOA->AFR[0] &= ~( (0xF << 8) | (0xF << 12) ); // Clear bits 8-15
    GPIOA->AFR[0] |= ( (7 << 8) | (7 << 12) );      // Set AF7 (for PA2 and PA3)


    // --- GPIO Pull-up/Pull-down Configuration ---

	// Set PA2 (RX pin) to Pull-up (Mode = 01). Bits 4-5 of PUPDR. <-- CHANGE: PA2 is now RX
	// Clear bits 4-5 (PA2)
	GPIOA->PUPDR &= ~(3 << 4);
	// Set bits 4-5 to 01b (Pull-up)
	GPIOA->PUPDR |= (1 << 4);

	// --- GPIO Output Speed Configuration ---

	// Configure PA2 (RX) and PA3 (TX) for High Speed (Mode = 10b)
	// Clear bits 4-5 (PA2) and 6-7 (PA3)
	GPIOA->OSPEEDR &= ~( (3 << 4) | (3 << 6) );
	// Set bits 4-5 and 6-7 to 10b (High Speed)
	GPIOA->OSPEEDR |= ( (2 << 4) | (2 << 6) );


    // --- USART Configuration ---

    // Set Baud Rate to 115200 (using the correct value for PCLK1 = 42MHz)
    // BRR = 0x16E (22.89) <-- FIX FOR GIBBERISH ISSUE
    USART2->BRR = 0x111;

    // Enable Transmitter (TE, Bit 3) and Receiver (RE, Bit 2)
    USART2->CR1 |= (USART_CR1_TE | USART_CR1_RE);

    // Enable USART (UE, Bit 13)
    USART2->CR1 |= USART_CR1_UE;
}

// -------------------------------------------------------------------
// --- Polling Transmission Functions ---
// -------------------------------------------------------------------

/**
 * @brief Sends a single byte (character) via USART2.
 * @param data The byte to send.
 */
void usart2_send_char(uint8_t data)
{
    // Wait until the Transmit data register is empty (TXE=1). Bit 7 of SR.
    while (!(USART2->SR & USART_SR_TXE));

    // Write the data to the Data Register (DR).
    USART2->DR = data;
}

/**
 * @brief Sends a null-terminated string via USART2.
 * @param str Pointer to the string to send.
 */
void usart2_send_string(char *str)
{
    while (*str)
    {
        usart2_send_char(*str++);
    }
}

// -------------------------------------------------------------------
// --- Polling Reception Function ---
// -------------------------------------------------------------------

/**
 * @brief Receives a single byte (character) via USART2. Blocking/Polling.
 * @return The received byte.
 */
uint8_t usart2_receive_char(void)
{
    // 1. Wait until the Read data register is not empty (RXNE=1). Bit 5 of SR.
    while (!(USART2->SR & USART_SR_RXNE));

    // 2. Read the data from the Data Register (DR).
    // Reading DR clears the RXNE flag.
    return (uint8_t)(USART2->DR & 0xFF);
}

// -------------------------------------------------------------------
// --- Interrupt-based Reception Functions ---
// -------------------------------------------------------------------

/**
 * @brief Enables the Receive Not Empty (RXNE) interrupt for USART2
 * and enables the corresponding interrupt in the NVIC.
 */
void usart2_enable_rx_interrupt(void)
{
    // 1. Enable the Receive Not Empty Interrupt (RXNEIE, Bit 5) in USART_CR1
    USART2->CR1 |= USART_CR1_RXNEIE;

    // 2. Enable the USART2 interrupt in the NVIC
    NVIC_EnableIRQ(USART2_IRQn);
}


#define LF_CHAR 0x0A // Line Feed character

/**
 * @brief USART2 Interrupt Service Routine (ISR).
 */
void USART2_IRQHandler(void)
{
    // Check if the cause of the interrupt is the Read Data Register Not Empty (RXNE) flag
    if (USART2->SR & USART_SR_RXNE)
    {
        uint8_t data = (uint8_t)(USART2->DR & 0xFF); // Read data and clear RXNE flag

        // Calculate the next head index (write index)
        uint16_t next_head = (uart_rx_head + 1) % UART_RX_BUFFER_SIZE;

        // Check for buffer overrun (if the next head equals the tail)
        if (next_head != uart_rx_tail)
        {
            // Store the data in the buffer
            uart_rx_buffer[uart_rx_head] = data;

            // Update the head index
            uart_rx_head = next_head;

            // Check if the received character is a Line Feed (LF)
            if (data == LF_CHAR)
            {
                // Set the flag to notify the main loop that a line is ready
                uart_rx_line_ready = true;
            }
        }
        // If overrun, the character is discarded, and the head/tail are not updated.
    }
}

/**
 * @brief Reads all characters from the buffer up to the first LF or the buffer limit.
 * @param dest Pointer to the destination string buffer.
 * @param max_len Maximum length of the destination buffer (including null terminator).
 * @return true if a line (terminated by LF) was successfully read, false otherwise.
 */
bool usart2_read_line(char *dest, uint16_t max_len)
{
    // Check if the ready flag is set
    if (!uart_rx_line_ready)
    {
        return false;
    }

    uint16_t i = 0;
    bool lf_found = false;

    // Read while buffer is not empty AND destination buffer has space
    while (uart_rx_tail != uart_rx_head && i < (max_len - 1))
    {
        uint8_t data = uart_rx_buffer[uart_rx_tail];

        // Update the tail index (read index)
        uart_rx_tail = (uart_rx_tail + 1) % UART_RX_BUFFER_SIZE;

        // Store data in the destination buffer
        dest[i++] = data;

        // Check for the Line Feed character
        if (data == LF_CHAR)
        {
            lf_found = true;
            break; // Stop reading after the LF is found
        }
    }

    // Null-terminate the destination string
    dest[i] = '\0';

    // If the LF was found, clear the flag and return success
    if (lf_found)
    {
        uart_rx_line_ready = false;
        return true;
    }

    return false;
}
