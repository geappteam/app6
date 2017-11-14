#include <stdio.h>
#include <stdbool.h>

#define REMOTELED_DRIVER_MODULE_IMPORT
#include "Remote_LED_driver.h"

#define ON_DESIRED 1
#define OFF_DESIRED 0

DesiredState lastDesiredRLED_state;

bool needsToSend;

DesiredState readDesiredState ()
{
    if (DSK6713_DIP_get(REMOTE_DIP) == ON_DESIRED)
        return ON;
    else
        return OFF;
}

void applyState (DesiredState state)
{
    if (state == ON)
        DSK6713_LED_on(REMOTE_LED);
    else
        DSK6713_LED_off(REMOTE_LED);
}

void RLED_init()
{
    lastDesiredRLED_state = readDesiredState();
    needsToSend = true;

    applyState(INIT_DESIRED);
}

void RLED_checkAndApply(unsigned char receivedByte)
{
    if (receivedByte == RLED_ENABLE_CODE)
    {
        applyState(ON);
        //printf("Received remote LED ON code\n");
        return;
    }
    if (receivedByte == RLED_DISABLE_CODE) //else
    {
        applyState(OFF);
        //printf("Received remote LED OFF code\n");
        return;
    }
}

void RLED_overwriteMessage(unsigned char * byteToBeSent)
{
    DesiredState switchState = readDesiredState ();

    if (switchState != lastDesiredRLED_state || needsToSend)
    {
        needsToSend = false;
        lastDesiredRLED_state = switchState;

        if (lastDesiredRLED_state == ON)
        {
            *byteToBeSent = RLED_ENABLE_CODE;
            //printf("Sending remote LED ON code\n");
        }
        else
        {
            *byteToBeSent = RLED_DISABLE_CODE;
            //printf("Sending remote LED OFF code\n");
        }
        return;
    }
    // else
    if (*byteToBeSent == RLED_DISABLE_CODE || *byteToBeSent == RLED_ENABLE_CODE)
        *byteToBeSent = DEFAULT_SAT_CODE;
}
