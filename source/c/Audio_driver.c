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

extern unsigned char int2ulaw(short linear);
extern int ulaw2int(unsigned char log);

extern bool isRecording;
extern bool isPlaying;
extern bool flagInt11;

bool flagRS232;
bool flagCompanding;
bool previousCommute;
bool flagTargetLED;

/****************************************************************************
	Private macros and constants :
****************************************************************************/

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

    micReading = 0;
    speakerValue = 0;
    flagAIC = false;
    flagCompanding = false;
    flagRS232 = false;
    previousCommute = false;
    flagTargetLED = false;

    comm_intr(DSK6713_AIC23_FREQ_16KHZ, DSK6713_AIC23_INPUT_MIC); //Because 230,4kbauds/s for UART (11 bauds)
    IRQ_globalDisable();

    /*DSK6713_AIC23_CodecHandle hCodec;
    hCodec = DSK6713_AIC23_openCodec(0, &config);
    DSK6713_AIC23_config(hCodec, config);*/

    flagAIC = false;

	return;
}

int uartToAIC(uint8_t uartDataByte){
    int aicData;

    if(uartDataByte == 0xFE)
        DSK6713_LED_on(LED1);
    else if(uartDataByte == 0xFF)
        DSK6713_LED_off(LED1);

    if(DSK6713_DIP_get(DIP0))
    {
        DSK6713_LED_on(LED0);

        aicData = ulaw2int(uartDataByte) << 2;      //Transform 8 bits to 14 <<2 bits with sign extension bits
    }
    else
    {
        DSK6713_LED_off(LED0);

        aicData =  (((int)uartDataByte)-128) << 8;  //Transform 8 bits to 16 bits (8 bits becoming MSB bits of 16 bits)
    }

    //  Return 32 bits with 16 bits MSB and LSB equal for output_sample()
    //  OR
    //  one 16 bits is the recording (MSB for left side)
//    if(isPlaying)
//        aicDataMSB = processReadingInSDRAM();
//    else
//        aicDataMSB = (aicDataLSB << 16);
//
//    if(isRecording)
//        processSavingInSDRAM((short)aicDataLSB);
//
    return aicData;
}

uint8_t aicToUart(short aicData){

    if(previousCommute != flagRS232){
        if(flagRS232)
            DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL0));
        else
            DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL0));
        previousCommute = flagRS232;
        DSK6713_waitusec(1000);          //DSP is too fast compared to relay's actuator
    }

    uint8_t uartData;

    if(DSK6713_DIP_get(DIP1) && !flagTargetLED){
            flagTargetLED = true;
            uartData = 0xFE;
    }
    else if(!DSK6713_DIP_get(DIP1) && flagTargetLED){
        flagTargetLED = false;
        uartData = 0xFF;
    }
    else if(DSK6713_DIP_get(DIP0)){
        DSK6713_LED_on(LED0);
        uartData = int2ulaw((aicData + 2) >> 2);
    }
    else
    {
        DSK6713_LED_off(LED0);
        uartData = (uint8_t)(((aicData + 0x80) >> 8)+128);
    }

    uartData = saturateByte(uartData);

    //Return 8 bits unsigned for a write by SPI
    return uartData;
}

uint8_t saturateByte(uint8_t data){
    if(data >= 0xFE)
        return 0xFD;
    else
        return data;
}

/****************************************************************************
	ISR :
****************************************************************************/

interrupt void c_int11(void)
{
    micReading = input_right_sample();
    output_sample(speakerValue | (speakerValue << 16));

    flagAIC = true;
    flagInt11 = true;
	return;
}

// end of Audio_driver.c
