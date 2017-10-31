/*----------------------------------------------------------------------------
 * Name:    CppSamp.cpp
 * Purpose: C++ example program with operator overloading and friend functions
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

#include <stdio.h>
#include <string.h>
#include <stddef.h>

extern "C"  {
  extern void SER_Init (void);           /* Initialize Serial Interface       */
}


class Complex  {
  int     re_part;
  int     im_part;

public:
  Complex(int re, int im) { re_part=re; im_part = im;  }
  Complex(int re)         { re_part = re; im_part = 0; }
  Complex()               { re_part = 0; im_part = 0;  }

  void    Print (int i1);
  void    Print();
  friend  Complex operator+(Complex, Complex);
  friend  Complex operator-(Complex, Complex);
  friend  Complex operator-(Complex);

  void     SetKey (unsigned int n);
private:
  unsigned int m_KeyCode;
};



void Complex::SetKey (unsigned int n)  {
  m_KeyCode = n;
}

void Complex::Print(int i1)  {
  printf ("i1 = %d\n", i1);
}

void Complex::Print(void)  {
  printf("re = %d, im = %d\n", re_part, im_part);
}

Complex operator+(Complex c1, Complex c2)  {
  return Complex (c1.re_part + c2.re_part, c1.im_part + c2.im_part);
}

Complex operator-(Complex c1, Complex c2)  {
  return Complex (c1.re_part - c2.re_part, c1.im_part - c2.im_part);
}

Complex operator-(Complex c1)  {
  return Complex(-c1.re_part, -c1.im_part);
}


  int    Adj_i;
  int  Adj_N_i;

typedef struct myt  {
  short int  node;
  short int  type;
  struct myt   *L;
  struct myt   *R;

} MyType;

namespace ADJ  {
  int    Adj_i;
  namespace ADJ_N  {
    int Adj_N_i;
  }
  MyType   ex1;
  MyType   ex2;
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)  {

  SER_Init();                            /* Initialize Serial Interface       */

  printf ("Example: CppSamp - Operator overloading / friend functions.\n\n");

  Adj_i   = 1000;
  Adj_N_i = 2000;

  ADJ::Adj_i          = 2;
  ADJ::ADJ_N::Adj_N_i = 2;

  ADJ::ex1.node = 41;
  ADJ::ex2.node = 42;
  ADJ::ex1.type = 10;
  ADJ::ex2.type = 11;

  Complex c1(1,2);
  Complex c2(2);
  Complex c3;

  c1.SetKey (1000);

  printf( "Complex number c1: " );
  c1.Print();
  printf( "Complex number c2: " );
  c2.Print();

  printf( "Calculate c1 + c2: " );
  c3 = c1 + c2;
  c3.Print();

  printf( "Calculate c2 - c1: " );
  c3 = c2 - c1;
  c3.Print();

  printf( "Negate previous  : " );
  c3 = -c3;
  c3.Print();

  c3.Print (10);
  printf( "\nEnd of Program.\n" );

  while (1);
}
