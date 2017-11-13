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
     /*   if(!isRecording){
          isRecording = true;
          resetSDRAMIterator();
          //enableTimerInterrupt(TIMER1);
          printf("\nBEGIN RECORDING\n\n");
      }

      // Timer 1
      if (timer1Flag){
          timer1Flag = false;
          if(timer1counter % 800 == 0){
              toggleLed(DEL1);
          }

          ++timer1counter;

          if(timer1counter >= 80000){
              isRecording = false;
              DSK6713_LED_off(DEL1);
              setEndOfLastRecordingAddress();
              //disableTimerInterrupt(TIMER1);
              timer1counter = 0;
              printf("\nEND RECORDING\n\n");
          }
      }*/
}

void play(){
    /*if(!isPlaying){
        isPlaying = true;
        setLed(DEL1, HIGH);
        resetSDRAMIterator();
        printf("\nBEGIN PLAYING\n\n");
        printf("\nIterator : %x End address : %x \n\n",getSDRAMAddressIt(), getEndOfLastRecordingAddress());
    }

    if(codecFlag && isPlaying && !(getSDRAMAddressIt() > getEndOfLastRecordingAddress())){
        codecFlag = false;
        if((readDipsVolume() == TUNE_DOWN) && !isTuningDown){
            isTuningDown = true;
            tuneDown();
        }
        else if((readDipsVolume() == TUNE_UP) && !isTuningUp){
            isTuningUp = true;
            tuneUp();
        }

        if(readDipsVolume() == NO_FEATURES)
            resetTuningFlags();

        dacOutput(convertADCDataToVoltage(processReadingInSDRAM()), ALL, getGain());
    }

    else if(getSDRAMAddressIt() > getEndOfLastRecordingAddress())
    {
        isPlaying = false;
        setLed(DEL1, LOW);
        printf("\nIterator : %x End address : %x \n\n",getSDRAMAddressIt(), getEndOfLastRecordingAddress());
        printf("\nEND PLAYING\n\n");
    }*/
}

