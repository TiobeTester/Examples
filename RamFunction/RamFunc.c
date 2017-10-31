/*----------------------------------------------------------------------------
 * Name:    RamFunc.c
 * Purpose: RAM Function
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

extern int count;


/*----------------------------------------------------------------------------
  Function performed in RAM
    - Assignment of Code Memory for Module to RAM
        under Options for file 'RamFunc.c'
        RO - Code/Const:      IRAM2 (0x20004000-0x20004FFF)
 *----------------------------------------------------------------------------*/
void RamFunc (void) {

  count++;
}
