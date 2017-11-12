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
#include <stdio.h>   // get standard I/O functions (as printf)
#include <stddef.h>  // get null and size_t definition
#include <stdbool.h> // get boolean, true and false definition


/****************************************************************************
	Private macros and constants :
****************************************************************************/

//vos  #defines ou const int blablabla
//unique � ce fichier

/****************************************************************************
	Extern content declaration :
****************************************************************************/

// d�claration des contenus utilis�s ici mais d�finis ailleurs

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

//d�clarer vos prototype de fonction ici

/****************************************************************************
	Main Program :
****************************************************************************/

void main()
{
	// initialisation des modules et des p�riph�riques
	myModule_init(); // initialisation du module exemple ; � enlever
	
	DSK6713_init();
	DSK6713_LED_init();
	DSK6713_DIP_init();

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
