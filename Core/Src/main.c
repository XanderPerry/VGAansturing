//--------------------------------------------------------------
// File     : main.c
// Datum    : 30.03.2016
// Version  : 1.0
// Autor    : UB
// mods by	: J.F. van der Bent
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.x
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : VGA_core DMA LIB 320x240, 8bit color
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>

#include "uart.h"


int main(void)
{
	SystemInit(); // System speed to 168MHz

	usart2_init(); // initialize UART

	char rx_byte;	// RX buffer

	UB_VGA_Screen_Init(); // Init VGA-Screen

	UB_VGA_FillScreen(VGA_COL_GREEN);

  while(1)
  {
	  // Example: Send a status message
	  usart2_send_string("Ready. Send a character...\r\n");

	  // Example: Wait for and echo a character
	  rx_byte = usart2_receive_char();
	  usart2_send_string("Received: ");
	  usart2_send_char(rx_byte);
	  usart2_send_string("\r\n");
  }
}
