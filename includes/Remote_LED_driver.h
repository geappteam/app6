#include <dsk6713.h>
#include <dsk6713_led.h>
#include <dsk6713_dip.h>

#include "io_constants.h"

#ifndef INCLUDES_REMOTE_LED_DRIVER_H_
#define INCLUDES_REMOTE_LED_DRIVER_H_

#ifdef REMOTELED_DRIVER_MODULE_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif


typedef bool DesiredState;

/***************************************************************************
    Constants :
***************************************************************************/

#define REMOTE_LED LED1
#define REMOTE_DIP DIP1

#define RLED_ENABLE_CODE 0xFF
#define RLED_DISABLE_CODE 0xFE
#define DEFAULT_SAT_CODE 0xFD

#define ON true
#define OFF false
#define INIT_DESIRED false


/***************************************************************************
    Function prototype :
***************************************************************************/

void RLED_init();

void RLED_checkAndApply(unsigned char receivedByte);
void RLED_overwriteMessage(unsigned char * byteToBeSent);


#endif /* INCLUDES_REMOTE_LED_DRIVER_H_ */
