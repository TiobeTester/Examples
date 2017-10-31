/*----------------------------------------------------------------------------
 * Name:    Memory.c
 * Purpose: Memory Layout Example
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

extern unsigned char FillVal;
extern unsigned char Buffer[4096];
extern void          Setup(void);


/* Mapped to Read-Only Data Region (RO-DATA same as CONST)
   which is a part of Read-Only Region (RO) */

const char Text[] =
  "This is a dummy text which will be loaded into the RO-DATA Region.";

const unsigned short TableS[16] = {
  0x0123, 0x4567, 0x89ab, 0xcdef, 0xfedc, 0xba98, 0x7654, 0x3210,
  0x0011, 0x2233, 0x4455, 0x6677, 0x8899, 0xaabb, 0xccdd, 0xeeff
};
const unsigned long  TableL[8] = {
  0x01234567, 0x89abcdef, 0xfedcba98, 0x76543210,
  0x00112233, 0x44556677, 0x8899aabb, 0xccddeeff
};


/* Mapped to Read/Write Region (RW) */
unsigned int BlockSize = 1024;

/* Mapped to Zero-Initialized Region (ZI) */
struct {
  unsigned short us[16];
  unsigned long  ul[8];
} Table;


/* Mapped to Read-Only Code Region (RO-CODE same as CODE)
   which is a part of Read-Only Region (RO) */

int main (void)  {
  char *m, *p, *t;
  int   i;

  Setup();

  for (i = 0; i < 16; i++) {
    Table.us[i] = TableS[i];     /* Load TableS                               */
  }
  for (i = 0; i < 8; i++) {
    Table.ul[i] = TableL[i];     /* Load TableL                               */
  }

  for (i = 0; i < sizeof(Buffer); i++) {
    Buffer[i] = FillVal;         /* Fill Buffer                               */
  }

  m = malloc(BlockSize);         /* Allocate Memory on Heap                   */
  if (m == NULL) return (1);

  p = m;
  t = (char *)&Text[0];
  while (*t) {
    *p++ = *t++;                 /* Copy Text to Heap                         */
  }
  *p = 0;                        /* Terminate String                          */

  free(m);                       /* Free Allocated Memory                     */

  while (1) {                    /* An embedded program does not stop and     */
    ;  /* ... */                 /* never returns. We use an endless loop.    */
  }                              /* Replace the dots (...) with your own code.*/
}
