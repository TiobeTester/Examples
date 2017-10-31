/*----------------------------------------------------------------------------
 * Name:    OstrSt.cpp
 * Purpose: C++ example program with ostream, stl and exception demonstration
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

#include <string.h>
#include <stdio.h>

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stddef.h>

extern "C"  {
  extern int SER_GetChar (void);
}


/*---------------------------------------------------------------------------
  STL Test Code
 *---------------------------------------------------------------------------*/
typedef std::vector<int>       IntVectorType;
typedef IntVectorType::iterator IntVectorItr;
typedef std::map<int, int>     IntIntMapType;
typedef IntIntMapType::iterator IntIntMapItr;

IntVectorType v;
IntIntMapType m;
std::string str;


void stl_test (void)  {
  int   sum = 0;

/*- vector test -------------------------------------------------------------*/
  sum = v.size();
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  sum = v.size();

  sum = 0;
  for (IntVectorItr itr = v.begin(); itr != v.end(); itr++)  {
    sum += *itr;
  }

  printf ("*** stl_test: vector sum = %d\n", sum);


/*- map test ----------------------------------------------------------------*/
  sum  = 0;
  m[1] = 1;
  m[2] = 2;
  m[3] = 3;

  for (IntIntMapItr itr = m.begin(); itr != m.end(); itr++)  {
    sum += itr->first * itr->second;
  }

  printf ("*** stl_test: map sum = %d\n", sum);


/*- exeption test -----------------------------------------------------------*/
/*  Note: execptions need to be enabled in
     Options for Target - C/C++ - Misc controls: '--exceptions'              */
  std::cout << std::endl;
  std::cout << "Throw exception:" << std::endl;

  try  {
    throw "Test exception";
  } catch (const char *szMessage)  {
    str = szMessage;
  }

  if (str.length() > 0)  {
    str += " - has been caught!";
  }
  std::cout << str << std::endl;
}


/*---------------------------------------------------------------------------
  Main program
   Note: initialization of the serial port is done in Serial.c.
         The function '$Sub$$__rt_entry()' does this job. Initializing
         it in main() is too late because the iostream library code is
         already initialized when entering main.

   The following linker warning can be safely ignored:
   warning:  #223-D: function "$Super$$__rt_entry" declared implicitly

   Note: stl code requires an appropriate heap setup, otherwise it will not
         work. Check the settings in the Startup code 'startup_stm32f10x_md.s'
 *---------------------------------------------------------------------------*/
int main (void)  {
  char c;

  std::cout.sync_with_stdio(false);
  std::cout << "*** Hello C++ World!" << std::endl;
  std::cout << std::endl;

  std::cout << "Press any key to start STL test code:" << std::endl;
  std::cin  >> c;
  std::cout << std::endl;

  stl_test();

  std::cout << std::endl;
  std::cout << "End of example." << std::endl;

  while (1);
}
