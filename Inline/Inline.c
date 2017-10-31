/*----------------------------------------------------------------------------
 * Name:    Inline.c
 * Purpose: In-line Assembly Demo
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

#include <stdio.h>                /* prototype declarations for I/O functions */


extern void SER_Init(void);                                   /* see Serial.c */

/*----------------------------------------------------------------------------
  User 'strcpy' Function in In-line Assembly
 *----------------------------------------------------------------------------*/
void my_strcpy (char *dst, const char *src) {
  int ch;

  __asm {
  loop: LDRB    ch, [src], #1
        STRB    ch, [dst], #1
        CMP     ch, #0
        BNE     loop
  }
}


/*----------------------------------------------------------------------------
  Multiply & Accumulate in In-line Assembly
 *----------------------------------------------------------------------------*/
#define lo64(a) (((unsigned int *)&a)[0])      /* Low  32-bits of a long long */
#define hi64(a) (((         int *)&a)[1])      /* High 32-bits of a long long */

__inline __int64 mlal (__int64 sum, int a, int b) {
  __asm { SMLAL lo64(sum), hi64(sum), a, b }
  return sum;
}


const char *txt = "Hello World!";
      char buf[16];

int a[10] = {  1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
int b[10] = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1  };

__int64 sum;


/*----------------------------------------------------------------------------
  main program
 *----------------------------------------------------------------------------*/
int main (void) {
  int n;

  SER_Init();                                  /* initialize serila interface */

  my_strcpy(buf, txt);                         /* Copy String                 */
  printf("%s\n\n", buf);

  sum = 0;
  printf("Dotproduct = ");
  for (n = 0; n < 10; n++) {
    sum = mlal(sum, a[n], b[n]);              /* Multiply & Accumulate        */
    if (n != 0) putchar('+');
    printf("%d*%d", a[n], b[n]);
  }
  printf(" = %lld\n", sum);

  while (1) {                    /* An embedded program does not stop and     */
    ;  /* ... */                 /* never returns. We use an endless loop.    */
  }                              /* Replace the dots (...) with your own code.*/

}
