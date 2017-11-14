#include <stdio.h>

#define RELAY_DRIVER_MODULE_IMPORT
#include "Relay_driver.h"

RelayMode lastMode;

RelayMode readRequestedMode()
{
    if (DSK6713_DIP_get(RELAY_DIP) == RS_232_DIP)
        return RS_232;
    else
        return RS_485;
}

void setRelayMode(RelayMode mode)
{
    DSK6713_waitusec(100); // Waiting for end of transmission

    if (mode == RS_232)
    {
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL0));
        DSK6713_LED_on(RS_232_LED);
        DSK6713_LED_off(RS_485_LED);
        lastMode = RS_232;
        printf("Relay is set to RS-232\n");
    }
    else
    {
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL0));
        DSK6713_LED_on(RS_485_LED);
        DSK6713_LED_off(RS_232_LED);
        lastMode = RS_485;
        printf("Relay is set to RS-485\n");
    }

    // Waiting for transition time of HFD27/005-H ( > 7 ms )
    DSK6713_waitusec(8000);
}

void RELAY_init()
{
    DSK6713_init();
    DSK6713_LED_init();
    DSK6713_DIP_init();

    setRelayMode(readRequestedMode());
}

void RELAY_update()
{
    RelayMode currentMode = readRequestedMode();

    if (currentMode != lastMode)
        setRelayMode(currentMode);
}
