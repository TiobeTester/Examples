/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher
 * Note(s):
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2012 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <stm32f10x.h>                       /* STM32F103 definitions         */

/*----------------------------------------------------------------------------
  wait function
 *----------------------------------------------------------------------------*/
void wait (void)  {
  int  d;

  for (d = 0; d < 2000000; d++);             /* only to delay for LED flashes */
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void) {
  unsigned int i;                            /* LED variable                  */

  RCC->APB2ENR |= (1UL << 3);                /* Enable GPIOB clock            */

  GPIOB->CRH    =  0x33333333;               /* PB.8..16 defined as Outputs   */

  while (1)  {                               /* Loop forever                  */
    for (i = 1<<8; i < 1<<15; i <<= 1) {     /* Blink LED 0,1,2,3,4,5,6       */
      GPIOB->BSRR = i;                       /* Turn LED on                   */
      wait ();                               /* call wait function            */
      GPIOB->BRR = i;                        /* Turn LED off                  */
    }
    for (i = 1<<15; i > 1<<8; i >>=1 ) {     /* Blink LED 7,6,5,4,3,2,1       */
      GPIOB->BSRR = i;                       /* Turn LED on                   */
      wait ();                               /* call wait function            */
      GPIOB->BRR = i;                        /* Turn LED off                  */
    }
  }
}
