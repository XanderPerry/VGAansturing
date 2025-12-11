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

#include <API_LIB.h>
#include "main.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>

#include "uart.h"
#include "LogicLayer.h"

#define CMD_BUFF_SIZE 64

int main(void)
{
	SystemInit(); // System speed to 168MHz

	usart2_init(); // initialize UART

	usart2_enable_rx_interrupt(); // Enable UART the interrupt

	char command_buffer[CMD_BUFF_SIZE]; // Create buffer for UART reception

	UB_VGA_Screen_Init(); // Init VGA-Screen

	UB_VGA_FillScreen(VGA_COL_GREEN); // Startup VGA screen with green background

  while(1)
  {
	  // Check the flag raised by the ISR
	  if (uart_rx_line_ready)
	  {
		  // Read the data from the UART buffer and reset the flag
		  if (usart2_read_line(command_buffer, sizeof(command_buffer)))
		  {
			  // Echo received message for debugging
			  usart2_send_string("Received: ");
			  usart2_send_string(command_buffer); // Echoes back the line, including the LF
			  // Process error message
			  int error = CmdToFunc(command_buffer);

			  // Reply error message if applicable
			  if(error)
			  {
				  char error_msg[12];
				  sprintf(error_msg, "%d", error);

				  usart2_send_string("ERROR: ");
				  usart2_send_string(error_msg);
				  usart2_send_string("\r\n");
			  }

		  }
	  }
  }
}
