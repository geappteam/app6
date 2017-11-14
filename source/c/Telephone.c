/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier principal Telephone.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/ 

/***************************************************************************
	Include headers :
***************************************************************************/

// Used modules headers
#include "SPI_driver.h"
#include "Audio_driver.h"
#include "C6713Helper_UdeS.h"
#include "playback.h"
#include "Relay_driver.h"

// standard libraries 
#include <csl.h>
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition
#include <dsk6713.h>
#include <dsk6713_led.h>
#include <dsk6713_dip.h>

/****************************************************************************
	Private macros and constants :
****************************************************************************/

//vos  #defines ou const int blablabla
//unique à ce fichier

/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern bool isRecording;
extern bool isPlaying;


/****************************************************************************
	Private Types :
****************************************************************************/

// type struct, enum , etc. definition here

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

// Use static keyword here

/****************************************************************************
	Main program private functions prototypes :
****************************************************************************/

//déclarer vos prototype de fonction ici
static void initAll(void);

/****************************************************************************
	Main Program :
****************************************************************************/

void main()
{
    /* initialize the CSL and BSL library */
    initAll();

	while(1)
	{	
	    if(flagAIC){
	        sendByteUART(aicToUart(micReading)); //MIC
	        flagAIC = false;
	    }

	    if (flagUART) {
	        speakerValue = uartToAIC(readByteUART());
	        flagUART = false;
	    }

	    RELAY_update();

//        //Bonus features
//        // Recording
//        if((DSK6713_DIP_get(DIP1) && !DSK6713_DIP_get(DIP2)) || isRecording)
//            handleRecord();
//
//        // Playing
//        if((DSK6713_DIP_get(DIP2) && !isRecording) || isPlaying)
//            handlePlay();
//        else
//            isPlaying = false;
	}
}

/****************************************************************************
	Main program private functions :
****************************************************************************/
static void initAll(void){
    CSL_init();
    DSK6713_init();
    DSK6713_LED_init();
    DSK6713_DIP_init();

    RELAY_init();

    Audio_init();
    DSK6713_waitusec(100);

    SPI_init();
}

/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

// end of Telephone.c
