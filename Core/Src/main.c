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
//Test token

int main(void)
{
	SystemInit(); // System speed to 168MHz

	UB_VGA_Screen_Init(); // Init VGA-Screen

	UB_VGA_FillScreen(VGA_COL_BLUE);

	API_draw_line(20, 120, 50, 160, 12, VGA_COL_RED);//x_1, y_1, x_2, y_2, weight, color
	API_draw_circle(30, 30, 30, VGA_COL_BLACK);

  while(1)
  {


  }
}
