/********************************************************
**  Session 5 - APP6 - Téléphonie par DSP
**  Fichier Audio_driver.c
**  Auteurs : < vos noms >
**  Date : < derniere modification >
********************************************************/

/***************************************************************************
	Include headers :
***************************************************************************/

#include "C6713Helper_UdeS.h"
#include <dsk6713_led.h>
#include <dsk6713_dip.h>

#include "playback.h"

/***************************************************************************
	Include Module Header :
***************************************************************************/

#define AUDIO_DRIVER_MODULE_IMPORT
#include "Audio_driver.h"


/****************************************************************************
	Extern content declaration :
****************************************************************************/

extern void vectors();   // Vecteurs d'interruption
extern unsigned char int2ulaw(short linear);
extern int ulaw2int(unsigned char log);

extern bool isRecording;
extern bool isPlaying;

volatile unsigned inData;
volatile unsigned outData;
volatile bool flagInt11;
bool flagRS232;
bool flagCompanding;
bool previousCommute;

/****************************************************************************
	Private macros and constants :
****************************************************************************/
#define DSK6713_AIC23_INPUT_MIC 0x0015
#define DIP0 0
#define LED0 0

/****************************************************************************
	Private Types :
****************************************************************************/

// These type declaration are only valid in this .c

/****************************************************************************
	Private global variables :
****************************************************************************/

// Use static keyword here

/****************************************************************************
	Private functions :
****************************************************************************/

// these function can only be called by this .c
// Use static keyword

/****************************************************************************
	Public functions :
****************************************************************************/

// Function description here ...
void Audio_init(void)
{
    //DSK6713_init(); //Initialization might be already done by comm_intr

    inData = 0;
    outData = 0;
    flagInt11 = false;
    flagCompanding = false;
    flagRS232 = false;
    previousCommute = false;

    comm_intr(DSK6713_AIC23_FREQ_16KHZ, DSK6713_AIC23_INPUT_MIC); //Because 230,4kbauds/s for UART (11 bauds)

    /*DSK6713_AIC23_CodecHandle hCodec;
    hCodec = DSK6713_AIC23_openCodec(0, &config);
    DSK6713_AIC23_config(hCodec, config);*/

	return;
}

int uartToAIC(uint8_t uartDataByte){
    int aicDataMSB, aicDataLSB = 0;

    if(DSK6713_DIP_get(DIP0)){
        DSK6713_LED_on(LED0);
        flagCompanding = true;
    }
    else
        DSK6713_LED_off(LED0);

    if(flagCompanding){
        aicDataLSB = ulaw2int(uartDataByte);          //Transform 8 bits to 14 bits with sign extension bits
        aicDataLSB = aicDataLSB << 16;                  //Ignore 16 bits MSB of sign extension
        aicDataLSB = 16 >> aicDataLSB;                 //Return 32 bits with 16 bits MSB and LSB equal for output_sample()
        flagCompanding = false;
    }
    else
        aicDataLSB = (int)(aicDataLSB + uartDataByte) << 8;  //Transform 8 bits to 16 bits (8 bits becoming MSB bits of 16 bits)

    //  Return 32 bits with 16 bits MSB and LSB equal for output_sample()
    //  OR
    //  one 16 bits is the recording (MSB for left side)
    if(isPlaying)
        aicDataMSB = processReadingInSDRAM();
    else
        aicDataMSB = (aicDataLSB << 16);

    if(isRecording)
        processSavingInSDRAM((short)aicDataLSB);

    return _add2(aicDataMSB, aicDataLSB);
}

uint8_t aicToUart(short aicData){
    uint8_t uartData = 0;

    if(DSK6713_DIP_get(DIP0)){
        DSK6713_LED_on(LED0);
        flagCompanding = true;
    }
    else
        DSK6713_LED_off(LED0);

    if(previousCommute != flagRS232){
        if(flagRS232)
            DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL0));
        else
            DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL0));
        previousCommute = flagRS232;
        DSK6713_waitusec(100);          //DSP is too fast compared to relay's actuator
    }


    if(flagCompanding){
        uartData = int2ulaw(aicData);
        flagCompanding = false;
    }
    else
        uartData = aicData >> 8;

    //Return 8 bits unsigned for a write by SPI
    return uartData;
}

/****************************************************************************
	ISR :
****************************************************************************/

interrupt void c_int11(void)
{
    inData = input_right_sample(); //MIC
    DSK6713_waitusec(10);          //DSP is too fast compared to McBSP1
    output_sample(outData);        //HEADPHONES
    flagInt11 = true;
	return;
}

// end of Audio_driver.c
