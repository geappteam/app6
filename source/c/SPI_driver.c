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
    MCBSP_FMKS(RCR, RWDLEN1, 16BIT)         | // This changes to 8 bits per frame
    MCBSP_FMKS(RCR, RWDREVRS, DISABLE),

    MCBSP_FMKS(XCR, XPHASE, SINGLE)         |
    MCBSP_FMKS(XCR, XFRLEN2, DEFAULT)       |
    MCBSP_FMKS(XCR, XWDLEN2, DEFAULT)       |
    MCBSP_FMKS(XCR, XCOMPAND, MSB)          |
    MCBSP_FMKS(XCR, XFIG, NO)               |
    MCBSP_FMKS(XCR, XDATDLY, 0BIT)          |
    MCBSP_FMKS(XCR, XFRLEN1, OF(0))         | // This changes to 1 FRAME
    MCBSP_FMKS(XCR, XWDLEN1, 16BIT)         | // This changes to 8 bits per frame
    MCBSP_FMKS(XCR, XWDREVRS, DISABLE),

    MCBSP_FMKS(SRGR, GSYNC, DEFAULT)        |
    MCBSP_FMKS(SRGR, CLKSP, DEFAULT)        |
    MCBSP_FMKS(SRGR, CLKSM, INTERNAL)       |
    MCBSP_FMKS(SRGR, FSGM, DEFAULT)         |
    MCBSP_FMKS(SRGR, FPER, DEFAULT)         |
    MCBSP_FMKS(SRGR, FWID, DEFAULT)         |
    MCBSP_FMKS(SRGR, CLKGDV, OF(16)),           // Min SCLK period is 238 ns -> rounding to 300 ns

    MCBSP_MCR_DEFAULT,
    MCBSP_RCER_DEFAULT,
    MCBSP_XCER_DEFAULT,

    MCBSP_FMKS(PCR, XIOEN, SP)              |
    MCBSP_FMKS(PCR, RIOEN, SP)              |
    MCBSP_FMKS(PCR, FSXM, INTERNAL)         |
    MCBSP_FMKS(PCR, FSRM, INTERNAL)         |
    MCBSP_FMKS(PCR, CLKXM, OUTPUT)          |
    MCBSP_FMKS(PCR, CLKRM, OUTPUT)          |
    MCBSP_FMKS(PCR, CLKSSTAT, DEFAULT)      |
    MCBSP_FMKS(PCR, DXSTAT, DEFAULT)        |
    MCBSP_FMKS(PCR, FSXP, ACTIVELOW)        |
    MCBSP_FMKS(PCR, FSRP, ACTIVEHIGH)       |
    MCBSP_FMKS(PCR, CLKXP, RISING)          |
    MCBSP_FMKS(PCR, CLKRP, RISING)
};

const Uint32 SPI_WRITE_CONFIG = 0xC000;
const Uint32 SPI_READ_CONFIG = 0x4000;
const Uint32 SPI_WRITE_DATA = 0x8000;
const Uint32 SPI_READ_DATA = 0x0000;

#define N_FIFO_ENABLE 13
#define SHDNi 12
#define N_TM 11
#define N_RM 10
#define N_PM 9
#define N_RAM 8
#define TWO_STOP_BITS 7
#define PARITY_ENABLE 6
#define WORD_LENGHT 5
#define UART_BAUD_RATE_DIV 0

const Uint32 MAX3111_Config =
    0   << N_FIFO_ENABLE |
    0   << SHDNi |
    0   << N_TM |
    1   << N_RM |
    0   << N_PM |
    0   << N_RAM |
    1   << TWO_STOP_BITS |
    0   << PARITY_ENABLE |
    0   << WORD_LENGHT |
    0   << UART_BAUD_RATE_DIV;


/****************************************************************************
	Private functions :
****************************************************************************/



/****************************************************************************
	Public functions :
****************************************************************************/

void SPI_init()
{
    /* enable NMI and GI */
    IRQ_globalEnable();
    IRQ_nmiEnable();


    /* point to the IRQ vector table */
    IRQ_setVecs(vectors); // CCS Error does not result in compilation fault

    MCBSP_reset(DSK6713_AIC23_CONTROLHANDLE);
    MCBSP_close(DSK6713_AIC23_CONTROLHANDLE);

    // Divert McBSP signals to daughter card
    DSK6713_rset(DSK6713_MISC, MCBSP1SEL);

    SPI_PortHandle = MCBSP_open(MCBSP_DEV0, MCBSP_OPEN_RESET);

    MCBSP_config(SPI_PortHandle, &MCBSP_SPI_Config);

    MCBSP_enableSrgr(SPI_PortHandle);
    DSK6713_waitusec(10);

    // Renable selected interrupt
    IRQ_map(IRQ_EVT_RINT0, IRQ_EVT_RINT0);

    /* Wake up the McBSP as receiver */
    MCBSP_enableRcv(SPI_PortHandle);
    MCBSP_enableXmt(SPI_PortHandle);
    DSK6713_waitusec(10);

    // Configuring MAX3111 registers
    MCBSP_write(SPI_PortHandle, SPI_WRITE_CONFIG | MAX3111_Config);
    DSK6713_waitusec(100);
    MCBSP_read(SPI_PortHandle);
    DSK6713_waitusec(100);

    // Check if config is applied
    MCBSP_write(SPI_PortHandle, SPI_READ_CONFIG);
    DSK6713_waitusec(100);
    Uint32 configReturned = MCBSP_read(SPI_PortHandle) & ~SPI_WRITE_CONFIG;

    if (configReturned != MAX3111_Config)
    {
        printf ("Error Setting up the MAX3111!\n");
        return;
    }

    IRQ_enable(IRQ_EVT_RINT0);

    printf ("Success Setting up the MAX3111!\n");
}


/****************************************************************************
	ISR :
****************************************************************************/

// Runs when the SPI data is ready to read
void interrupt spi_receive_int0(){
    short data = MCBSP_read(SPI_PortHandle);

}

// Unused
void interrupt spi_transmit_int0(){

}
