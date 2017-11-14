#ifndef INCLUDES_RELAY_DRIVER_H_
#define INCLUDES_RELAY_DRIVER_H_

#include <dsk6713.h>
#include <dsk6713_led.h>
#include <dsk6713_dip.h>

#include "io_constants.h"

#ifdef RELAY_DRIVER_MODULE_IMPORT
    #define EXTERN
#else
    #define EXTERN extern
#endif

/***************************************************************************
    Types :
***************************************************************************/

typedef enum {
    RS_232 = 0,
    RS_485 = 1
} RelayMode;

/***************************************************************************
    Constants :
***************************************************************************/

#define RELAY_DIP DIP3
#define RS_232_LED LED3
#define RS_485_LED LED2
#define RS_232_DIP 1
#define RS_485_DIP 0

/***************************************************************************
    Function prototype :
***************************************************************************/

void RELAY_init();

void RELAY_update();


#undef RELAY_DRIVER_MODULE_IMPORT
#undef EXTERN
#endif
