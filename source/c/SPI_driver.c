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
#include <dsk6713.h>
#include <dsk6713_aic23.h>

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

MCBSP_Handle SPI_PortHandle;

MCBSP_Config MCBSP_SPI_Config = {
    MCBSP_FMKS(SPCR, FREE, NO)              |
    MCBSP_FMKS(SPCR, SOFT, NO)              |
    MCBSP_FMKS(SPCR, FRST, YES)             |
    MCBSP_FMKS(SPCR, GRST, YES)             |
    MCBSP_FMKS(SPCR, XINTM, XRDY)           |
    MCBSP_FMKS(SPCR, XSYNCERR, NO)          |
    MCBSP_FMKS(SPCR, XRST, YES)             |
    MCBSP_FMKS(SPCR, DLB, OFF)              |
    MCBSP_FMKS(SPCR, RJUST, RZF)            |
    MCBSP_FMKS(SPCR, CLKSTP, DELAY)         |
    MCBSP_FMKS(SPCR, DXENA, OFF)            |
    MCBSP_FMKS(SPCR, RINTM, RRDY)           |
    MCBSP_FMKS(SPCR, RSYNCERR, NO)          |
    MCBSP_FMKS(SPCR, RRST, YES),

    MCBSP_FMKS(RCR, RPHASE, SINGLE)         |
    MCBSP_FMKS(RCR, RFRLEN2, DEFAULT)       |
    MCBSP_FMKS(RCR, RWDLEN2, DEFAULT)       |
    MCBSP_FMKS(RCR, RCOMPAND, MSB)          |
    MCBSP_FMKS(RCR, RFIG, NO)               |
    MCBSP_FMKS(RCR, RDATDLY, 0BIT)          |
    MCBSP_FMKS(RCR, RFRLEN1, OF(0))         | // This changes to 1 FRAME
    MCBSP_FMKS(RCR, RWDLEN1, 8BIT)          | // This changes to 8 bits per frame
    MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

    MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
    MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
    MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
    MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
    MCBSP_FMKS(XCR, XFIG, NO)               |
    MCBSP_FMKS(XCR, XDATDLY, 0BIT)          |
    MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
    MCBSP_FMKS(XCR, XWDLEN1, 8BIT)          | // This changes to 8 bits per frame
    MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

    MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
    MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
    MCBSP_FMKS(SRGR, CLKSM, INTERNAL)       |
    MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |
    MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
    MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
    MCBSP_FMKS(SRGR, CLKGDV, OF(68)),           // Min SCLK period is 238 ns -> rounding to 300 ns

    MCBSP_MCR_DEFAULT,
    MCBSP_RCER_DEFAULT,
    MCBSP_XCER_DEFAULT,

    MCBSP_FMKS(PCR, XIOEN, SP)              |
    MCBSP_FMKS(PCR, RIOEN, SP)              |
    MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
    MCBSP_FMKS(PCR, FSRM, EXTERNAL)         |
    MCBSP_FMKS(PCR, CLKXM, OUTPUT)          |
    MCBSP_FMKS(PCR, CLKRM, INPUT)           |
    MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
    MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
    MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
    MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
    MCBSP_FMKS(PCR, CLKXP, RISING)          |
    MCBSP_FMKS(PCR, CLKRP, RISING)
};

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

    MCBSP_reset(DSK6713_AIC23_CONTROLHANDLE);
    MCBSP_close(DSK6713_AIC23_CONTROLHANDLE);

    // Divert McBSP signals to daughter card
    DSK6713_rset(DSK6713_MISC, MCBSP1SEL);

    SPI_PortHandle = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);

    MCBSP_config(SPI_PortHandle, &MCBSP_SPI_Config);

    // Renable selected interrupt
    IRQ_enable(IRQ_EVT_RINT0);

    MCBSP_enableSrgr(SPI_PortHandle);
    DSK6713_waitusec(10);

    /* Wake up the McBSP as receiver */
    MCBSP_enableRcv(SPI_PortHandle);
    MCBSP_enableXmt(SPI_PortHandle);
}


/****************************************************************************
	ISR :
****************************************************************************/

interrupt void spi_receive_int0(void)
{

}
