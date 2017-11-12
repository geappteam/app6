/********************************************************
**  Session 5 - APP6 - T�l�phonie par DSP
**  Fichier principal Telephone.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/ 

/***************************************************************************
	Include headers :
***************************************************************************/

// Used modules headers
#include "module_example.h"
#include "SPI_driver.h"
#include "Audio_driver.h"


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
//unique � ce fichier

#define LED2 2
#define LED3 3

#define DIP3 3

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// d�claration des contenus utilis�s ici mais d�finis ailleurs

extern void vectors();   // Vecteurs d'interruption
extern volatile unsigned inData;
extern volatile unsigned outData;
extern volatile bool flagInt11;
extern bool flagRS232;

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

//d�clarer vos prototype de fonction ici
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
	    if(flagInt11){
	        //...
	        flagInt11 = false;
	    }

        if(DSK6713_DIP_get(DIP3)){
            flagRS232 = false;
            DSK6713_LED_on(LED3);
            DSK6713_LED_off(LED2);
        }
        else{
            flagRS232 = true;
            DSK6713_LED_on(LED2);
            DSK6713_LED_off(LED3);
        }

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

    SPI_init();
}

/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

// end of Telephone.c
