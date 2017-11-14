/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier Audio_driver.h
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

#pragma once
#ifndef _AUDIO_MODULE_
#define _AUDIO_MODULE_

/***************************************************************************
	Include headers :
***************************************************************************/

// standard libraries 
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition
#include <stdint.h>

// #include "something.h"
#include "dsk6713.h"
/***************************************************************************
	Set EXTERN macro :
***************************************************************************/

#ifdef AUDIO_DRIVER_MODULE_IMPORT
	#define EXTERN  
#else 
	#define EXTERN extern
#endif

/***************************************************************************
	Constants declaration :
***************************************************************************/

//#define something something_else
#define DSK6713_AIC23_INPUT_MIC 0x0015

/***************************************************************************
	Types declaration here :
***************************************************************************/

// typedef, struct, enum, union, etc.

/***************************************************************************
	Global variables declaration :
***************************************************************************/

EXTERN volatile short micReading;
EXTERN volatile int finalSpeakerValue;
EXTERN volatile bool flagAIC;

/***************************************************************************		
	Function prototype :
***************************************************************************/

// Function description here ...
EXTERN void Audio_init(void);

// ajoutez vos prototype de fonction ici
int uartToAIC(uint8_t uartDataByte);
uint8_t aicToUart(short aicData);

#undef AUDIO_DRIVER_MODULE_IMPORT

#undef EXTERN

#endif // end of #ifndef _AUDIO_MODULE_

// end of Audio_driver.h
