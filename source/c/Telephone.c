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
#include "module_example.h"
#include "SPI_driver.h"
#include "Audio_driver.h"


// standard libraries 
#include <csl.h>
#include <dsk6713.h>
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition


/****************************************************************************
	Private macros and constants :
****************************************************************************/

//vos  #defines ou const int blablabla
//unique à ce fichier

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// déclaration des contenus utilisés ici mais définis ailleurs

extern void vectors();   // Vecteurs d'interruption
extern volatile unsigned inData;
extern volatile unsigned outData;
extern volatile bool flagInt11;

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

/****************************************************************************
	Main Program :
****************************************************************************/

void main()
{
    /* initialize the CSL and BSL library */
    CSL_init();
    DSK6713_init();
    DSK6713_LED_init();
    DSK6713_DIP_init();

    SPI_init();

	// initialisation des modules et des périphériques
	myModule_init(); // initialisation du module exemple ; à enlever
	

	// Boucle infinie
	while(1)
	{	
	    if(flagInt11){
	        //...
	        flagInt11 = false;
	    }

	}
}

/****************************************************************************
	Main program private functions :
****************************************************************************/

/****************************************************************************
	Main program interrupt service routines (ISR) :
****************************************************************************/

// end of Telephone.c
