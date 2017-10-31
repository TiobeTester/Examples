/*----------------------------------------------------------------------------
 * Name:    Measure.c
 * Purpose: Remote Measurement Recorder
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

#include <stdio.h>                           /* standard I/O .h-file          */
#include <ctype.h>                           /* character functions           */
#include <stm32f10x.h>                       /* STM32F10x definitions         */
#include "Serial.h"
#include "LED.h"
#include "ADC.h"
#include "Measure.h"

const char menu[] =
   "\n"
   "+***************** REMOTE MEASUREMENT RECORDER *****************+\n"
   "| This program is a simple Measurement Recorder. It is based on |\n"
   "| the STM32F103RB and records the state of the Buttons S2,S3    |\n"
   "| and the voltage on the analog input ADC1 channel1.            |\n"
   "+ command -+ syntax -----+ function ----------------------------+\n"
   "| Read     | R [n]       | read <n> recorded measurements       |\n"
   "| Display  | D           | display current measurement values   |\n"
   "| Time     | T hh:mm:ss  | set time                             |\n"
   "| Interval | I mm:ss.ttt | set interval time                    |\n"
   "| Clear    | C           | clear measurement records            |\n"
   "| Quit     | Q           | quit measurement recording           |\n"
   "| Start    | S           | start measurement recording          |\n"
   "+----------+-------------+--------------------------------------+\n";

struct interval setinterval;                 /* interval setting values       */
struct interval interval;                    /* interval counter              */

volatile int measurement_interval = 0;       /* measurement interval over     */
volatile int mdisplay = 0;                   /* measurement display requested */
volatile int startflag = 0;                  /* start measurement recording   */

struct mrec current;                         /* current measurements          */

#define SCNT 20                              /* number of records             */

struct mrec save_record[SCNT];               /* buffer for measurements       */
int sindex;                                  /* save index                    */
int savefirst;                               /* save first index              */

char ERROR_STR [] = "\n*** ERROR: %s\n";     /* ERROR message string in code  */

#define WRONGINDEX 0xffff                    /* error signal for wrong index  */


/*----------------------------------------------------------------------------
  Configure Push Buttons
 *----------------------------------------------------------------------------*/
void KBD_Init (void) {

  RCC->APB2ENR |= (RCC_APB2ENR_IOPAEN |      /* Enable GPIOA clock            */
                   RCC_APB2ENR_IOPCEN  );    /* Enable GPIOC clock            */

  GPIOA->CRL   &= ~0x0000000F;               /* PA.0 clear                    */
  GPIOA->CRL   |=  0x00000004;               /* PA.0 is Floating Input        */

  GPIOC->CRH   &= ~0x00F00000;               /* PC.13 clear                   */
  GPIOC->CRH   |=  0x00400000;               /* PC.13 is Floating Input       */
}


/*----------------------------------------------------------------------------
  Configure Timer/Counter 3
  Generate interrupt every 1ms @ 72 MHZ Timer clock
 *----------------------------------------------------------------------------*/
void TIM_Init (void) {

  RCC->APB1ENR |=  (1UL << 1);               /* enable TIM3 clock             */

  TIM3->PSC = 1;                             /* set prescaler to 1            */
  TIM3->ARR = 36000 - 1;

  TIM3->CR1 = 0;                             /* reset command register 1      */
  TIM3->CR2 = 0;                             /* reset command register 2      */

  TIM3->DIER = (1UL <<  0);                  /* enable update interrupt       */

  NVIC_EnableIRQ(TIM3_IRQn);                 /* enable TIM3 Interrupt         */

  TIM3->CR1 |= (1UL <<  0);                  /* enable counter                */
}


/*----------------------------------------------------------------------------
  Save current measurements in save_record
 *----------------------------------------------------------------------------*/
static void save_current_measurements (void) {
  save_record[sindex++] = current;           /* copy current measurements     */
  if (sindex == SCNT) sindex = 0;            /* check bounds of sindex        */
  if (sindex == savefirst)  {                /* check circular buffer limits  */
    if (++savefirst == SCNT)  savefirst = 0; /* check bounds of savefirst     */
  }
}


/*----------------------------------------------------------------------------
  Timer Counter 3 interrupt service function
 *----------------------------------------------------------------------------*/
void TIM3_IRQHandler (void) {

  if (TIM3->SR & (1<<0)) {                   /* UIF interrupt ?               */

  if (AD_done == 1) {
    AD_done = 0;
    ADC_StartCnv ();
  }

    if (measurement_interval)  {             /* measurement done ?            */
      save_current_measurements ();          /* yes -> save measurements      */
      measurement_interval = 0;              /* Save measurements done        */
    }
                                             /* check if interval is over     */
    if (interval.min  == 0 &&
        interval.sec  == 0 &&
        interval.msec == 0     )  {
      interval = setinterval;                /* set interval time again       */
      measurement_interval = startflag;      /* determine measurement flag    */
    }
    else  {                                  /* update interval time          */
      if (interval.msec-- == 0)  {           /* calculate millisecond         */
        interval.msec = 999;
        if (interval.sec-- == 0)  {          /* calculate second              */
          interval.sec = 59;
          interval.min--;                    /* calculate minute              */
        }
      }
    }
                                             /* update current time           */
    if (++current.time.msec == 1000)  {      /* update millisecond cnt        */
      current.time.msec = 0;

      if (++current.time.sec == 60)  {       /* update second counter         */
        current.time.sec = 0;

        if (++current.time.min == 60)  {     /* update minute counter         */
          current.time.min = 0;

          if (++current.time.hour == 24)  {  /* update hour counter           */
            current.time.hour = 0;
          }
        }
      }
    }

    if (current.time.msec == 0) {
      LED_On (0);                            /* Turn LED#0 On                 */
    }
    if (current.time.msec == 100) {
      LED_Off(0);                            /* Turn LED#0 Off                */
    }

    if (measurement_interval || mdisplay)  { /* process measurement           */
      current.gpioa = GPIOA->IDR;            /* S2 status                     */
      current.gpioc = GPIOC->IDR;            /* S3 status                     */

      /* converted values a transmitted via DMA */
      current.analog[0] = AD_last;           /* AD value (12 bit)             */

      mdisplay = 0;                          /* mdisplay = 0 for ready sig.   */
    }

    TIM3->SR &= ~(1UL <<  0);                /* clear UIF flag                */
  }

}


/*----------------------------------------------------------------------------
  Calculate first Read Index
 *----------------------------------------------------------------------------*/
static int read_index (char *buffer) {
  int index = 0;
  int args;

  if (setinterval.min == 0     &&            /* check if setinterval is       */
      setinterval.sec == 0     &&            /* below 1 second and            */
      setinterval.msec < 999   &&            /* measurements are collected    */
      startflag                   )  {
    printf (ERROR_STR, "QUIT MEASUREMENTS BEFORE READ");
    return (WRONGINDEX);                     /* no display on the fly if      */
  }                                          /* interval time < 1 second      */
  args = sscanf (buffer, "%d", &index);      /* scan input for read count     */
  if (args == 0  ||  index == 0  || args == EOF)  index = SCNT-1;
  index = sindex - index;                    /* calculate first read index    */
  if (index < 0) index += SCNT;              /* from read count               */
  return (index);
}


/*----------------------------------------------------------------------------
  Clear Measurement Records
 *----------------------------------------------------------------------------*/
static void clear_records (void) {
  int idx;                                   /* index for loop                */

  startflag = 0;                             /* stop measurement collecting   */
  sindex = savefirst = 0;                    /* reset circular buffer index   */
  for (idx = 0; idx != SCNT; idx++)  {       /* mark all records unused       */
    save_record[idx].time.hour = 0xff;       /* unused flag: hour = 0xff      */
  }
}


/*----------------------------------------------------------------------------
  Main Program
 *----------------------------------------------------------------------------*/
int main (void)  {                           /* main entry for program        */
  char cmdbuf [15];                          /* command input buffer          */
  int i;                                     /* index for command buffer      */
  int idx;                                   /* index for circular buffer     */

  /*--------------------------------------------------------------------------*/
  LED_Init();                                /* LED Initialization            */
  KBD_Init();                                /* Push Button Initialization    */
  SER_Init();                                /* UART Initialization           */
  TIM_Init();                                /* TIM3 Initialisation           */
  ADC_Init();                                /* ADC Initialization            */
  ADC_StartCnv ();                           /* start initial AD conversion   */

  clear_records ();                          /* initialize circular buffer    */
  printf ( menu );                           /* display command menu          */

  while (1)  {                               /* loop forever                  */
    printf ("\nCommand: ");
    getline (&cmdbuf[0], sizeof (cmdbuf));   /* input command line            */

    for (i = 0; cmdbuf[i] != 0; i++)  {      /* convert to upper characters   */
      cmdbuf[i] = toupper(cmdbuf[i]);
    }

    for (i = 0; cmdbuf[i] == ' '; i++);      /* skip blanks                   */

    switch (cmdbuf[i])  {                    /* proceed to command function   */

      case 'R':                              /* Read circular Buffer          */
        if ((idx = read_index (&cmdbuf[i+1])) == WRONGINDEX)  break;
        while (idx != sindex)  {             /* check end of table            */
          if (SER_CheckChar())  {            /* check serial interface        */
            if (SER_GetChar() == 0x1B) break;/* for escape character          */
          }
          if (save_record[idx].time.hour != 0xff)  {
            measure_display (save_record[idx]);  /* display record            */
            printf ("\n");
          }
          if (++idx == SCNT) idx = 0;        /* next circular buffer entry    */
        }
        break;

      case 'T':                              /* Enter Current Time            */
        set_time (&cmdbuf[i+1]);
        break;

      case 'I':                              /* Enter Interval Time           */
        set_interval (&cmdbuf[i+1]);
        break;

      case 'D':                              /* Display Command               */
        printf ("\nDisplay current Measurements: (ESC to abort)\n");
        do  {
          while (!(SER_CheckChar()))  {      /* check serial interface        */
            mdisplay = 1;                    /* request measurement           */
            while (mdisplay);                /* wait for measurement          */
            measure_display (current);       /* display values                */
          }
        } while (SER_GetChar () != 0x1B);    /* escape terminates command     */
        printf ("\n\n");
        break;

      case 'S':                              /* Start Command                 */
        printf ("\nStart Measurement Recording\n");
        startflag = 1;
        break;

      case 'Q':                              /* Quit Command                  */
        printf ("\nQuit Measurement Recording\n");
        startflag = 0;
        break;

      case 'C':                              /* Clear Command                 */
        printf ("\nClear Measurement Records\n");
        clear_records ();
        break;

      default:                               /* Error Handling                */
        printf (ERROR_STR, "UNKNOWN COMMAND");
        printf (menu);                       /* display command menu          */
        break;
    }
  }
}
