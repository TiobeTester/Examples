/*----------------------------------------------------------------------------
 * Name:    RamFunction.c
 * Purpose: Example showing how to execute functions in RAM
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

#include <stdlib.h>


extern void RamFunc (void);                        /* see RamFunc.c          */

unsigned int count = 0;


/*----------------------------------------------------------------------------
  Main Program
    - Mapped to Read-Only Code Region (RO-CODE same as CODE)
      which is a part of Read-Only Region (RO)
 *----------------------------------------------------------------------------*/
int main (void)  {

  while (1) {
    RamFunc ();                                    /* execute function in RAM */
  }

}
