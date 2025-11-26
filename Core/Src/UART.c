#include "uart.h"
#include <string.h>  // Required for strlen (though not strictly needed if usart2_send_string is implemented character-by-character)
#include "stm32f4xx.h" // Includes the peripheral definitions (GPIOA, RCC, USART2, etc.)


/**
 * @brief Initializes USART2 for 115200 baud, 8N1 (Register-level)
 * Steps: Clock -> GPIO -> Baud Rate -> Enable
 */
void usart2_init(void)
{
    // --- 1. Clock Configuration ---

    // Enable GPIOA clock (AHB1)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Bit 0 is GPIOAEN

    // Enable USART2 clock (APB1)
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // Bit 17 is USART2EN


    // --- 2. GPIO Configuration (PA3=TX, PA2=RX) ---

    // Configure PA2 and PA3 for Alternate Function (AF) mode (Mode = 10)
    // Clear bits 4-5 (PA2) and 6-7 (PA3) and then set them to 10b
    GPIOA->MODER &= ~( (3 << 4) | (3 << 6) );
    GPIOA->MODER |= ( (2 << 4) | (2 << 6) );

    // Configure Alternate Function Register Low (AFRL) to AF7 for PA2 and PA3
    // AF7 is 0111
    GPIOA->AFR[0] &= ~( (0xF << 8) | (0xF << 12) ); // Clear bits 8-15
    GPIOA->AFR[0] |= ( (7 << 8) | (7 << 12) );      // Set AF7 (for PA2 and PA3)


    // --- 2.5. GPIO Pull-up/Pull-down Configuration ---

	// Set PA2 (RX pin) to Pull-up (Mode = 01). Bits 4-5 of PUPDR. <-- CHANGE: PA2 is now RX
	// Clear bits 4-5 (PA2)
	GPIOA->PUPDR &= ~(3 << 4);
	// Set bits 4-5 to 01b (Pull-up)
	GPIOA->PUPDR |= (1 << 4);

	// Note: PA3 (TX pin) is left as no Pull-up/Pull-down (00b), which is standard for TX.


	// --- 2.7. GPIO Output Speed Configuration ---

	// Configure PA2 (RX) and PA3 (TX) for High Speed (Mode = 10b)
	// Clear bits 4-5 (PA2) and 6-7 (PA3)
	GPIOA->OSPEEDR &= ~( (3 << 4) | (3 << 6) );
	// Set bits 4-5 and 6-7 to 10b (High Speed)
	GPIOA->OSPEEDR |= ( (2 << 4) | (2 << 6) );


    // --- 3. USART Configuration ---

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

void usart2_send_char(uint8_t data)
{
    // 1. Wait until the Transmit data register is empty (TXE=1). Bit 7 of SR.
    while (!(USART2->SR & USART_SR_TXE));

    // 2. Write the data to the Data Register (DR).
    USART2->DR = data;
}

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

uint8_t usart2_receive_char(void)
{
    // 1. Wait until the Read data register is not empty (RXNE=1). Bit 5 of SR.
    while (!(USART2->SR & USART_SR_RXNE));

    // 2. Read the data from the Data Register (DR).
    // Reading DR clears the RXNE flag.
    return (uint8_t)(USART2->DR & 0xFF);
}

// Optional: You would add interrupt functions here if you choose that method.
// void USART2_IRQHandler(void) { ... }
