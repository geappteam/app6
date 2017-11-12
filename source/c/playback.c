/*
 * playback.c
 *
 *  Created on: 12 nov. 2017
 *      Author: Humanismus
 */
#include "playback.h"

//Global program process flags
bool isRecording = false;
bool isPlaying = false;

void record(){
/*    if(!isRecording){
        isRecording = true;
        resetSDRAMIterator();
        enableTimerInterrupt(TIMER1);
        printf("\nBEGIN RECORDING\n\n");
    }

        // Timer 1
        if (timer1Flag){
            timer1Flag = false;
            startADConv();
            if(timer1counter % 800 == 0){
                toggleLed(DEL0);
            }

            ++timer1counter;

            if(timer1counter >= 80000){
                isRecording = false;
                setLed(DEL0, LOW);
                setEndOfLastRecordingAddress();
                disableTimerInterrupt(TIMER1);
                timer1counter = 0;
                printf("\nEND RECORDING\n\n");
            }
        }

        // End of conversion
        if (eocFlag){
            eocFlag = false;
            processSavingInSDRAM(readADCvalue());
        }*/
}

void play(){
    isPlaying = true;
}

