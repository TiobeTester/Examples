/*----------------------------------------------------------------------------
 * Name:    ExtMem.c
 * Purpose: External Memory: RO,RW & ZI sections
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


/*----------------------------------------------------------------------------
   External Memory Assignment (under Options for file 'ExtMem.c')
     RO - Code/Const:       ROM1 (0x70000000-0x7000FFFF)
     RW - Data:             RAM1 (0x60000000-0x6000FFFF)
     ZI - Zero Init. Data:  RAM1 (0x60000000-0x6000FFFF)
 *----------------------------------------------------------------------------*/


/* Mapped to Read/Write Region (RW) */
unsigned char FillVal = 0xff;

/* Mapped to Zero-Initialized Region (ZI) */
unsigned char Buffer[4096];


/* Mapped to Read-Only Code Region (RO-CODE same as CODE)
   which is a part of Read-Only Region (RO) */
void Setup (void) {
  FillVal ^= 0x55;
}
