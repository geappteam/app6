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
#include "Remote_LED_driver.h"

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
extern bool hasRecordedOnce;


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

    int speakerValueBuffer = 0;
    short temp_speakerValueBuffer = 0;

	while(1)
	{	
	    if(flagAIC){
	        uint8_t toSendByte = aicToUart(micReading);
	        RLED_overwriteMessage(&toSendByte);
	        sendByteUART(toSendByte); //MIC
	        flagAIC = false;
	    }

	    if (flagUART) {
	        unsigned char spiReceivedByte = readByteUART();
	        RLED_checkAndApply(spiReceivedByte);
	        speakerValue = uartToAIC(spiReceivedByte);

	        // To ignore 16 bits MSB of sign extension
	        speakerValue = speakerValue << 16;
	        speakerValue = (speakerValue | speakerValue >> 16);

	        if(isPlaying)
                speakerValue = (speakerValue >> 16 | (int)processReadingInSDRAM() << 16);
	        else if(isRecording){
	            speakerValueBuffer = speakerValue >> 16;
	            temp_speakerValueBuffer = (short)speakerValueBuffer;
	            processSavingInSDRAM(temp_speakerValueBuffer);
	        }

	        flagUART = false;
	    }

	    RELAY_update();

//        //Bonus features
//        // Recording
        if((!DSK6713_DIP_get(DIP1) && DSK6713_DIP_get(DIP2)) || isRecording)
            handleRecord();

        // Playing
        if(((!DSK6713_DIP_get(DIP2) && !isRecording) || isPlaying) && hasRecordedOnce)
            handlePlay();
        else
            isPlaying = false;
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

    RLED_init();
}

/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

// end of Telephone.c
