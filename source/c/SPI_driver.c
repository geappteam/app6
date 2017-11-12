/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier SPI_driver.c
**  Auteurs : Anthony Parris & Édouard Denommée
********************************************************/

/***************************************************************************
	Include headers :
***************************************************************************/
#include <csl.h>
#include <csl_mcbsp.h>
#include <C6713Helper_UdeS.h>.h>

/***************************************************************************
	Include Module Header :
***************************************************************************/

#define SPI_DRIVER_MODULE_IMPORT
#include "SPI_driver.h"

/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern void vectors();   // Vecteurs d'interruption

/****************************************************************************
	Private macros and constants :
****************************************************************************/



/****************************************************************************
	Private Types :
****************************************************************************/



/****************************************************************************
	Private global variables :
****************************************************************************/

MCBSP_Handle hMcbsp0;

/****************************************************************************
	Private functions :
****************************************************************************/



/****************************************************************************
	Public functions :
****************************************************************************/

void SPI_init()
{
    /* enable NMI and GI */
    IRQ_nmiEnable();
    IRQ_globalEnable();

    /* point to the IRQ vector table */
    IRQ_setVecs(vectors); // CCS Error does not result in compilation fault
    IRQ_reset(IRQ_EVT_RINT0);   // Temporarly disable selected interrupt

    hMcbsp0 = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);



    // Renable selected interrupt
    IRQ_enable(IRQ_EVT_RINT0);

    /* Wake up the McBSP as receiver */
    MCBSP_enableRcv(hMcbsp0);
}


/****************************************************************************
	ISR :
****************************************************************************/

interrupt void spi_receive_int0(void)
{

}
