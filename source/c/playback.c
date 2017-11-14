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
bool flagInt11 = false;
int counter = 0;

void handleRecord(){
    if(!isRecording){
      isRecording = true;
      resetSDRAMIterator();
      printf("\nBEGIN RECORDING\n\n");
    }

      // Interrupt 11 went off
      if(flagInt11){
          flagInt11 = false;
          if(counter % 1600 == 0){
              //toggleLed(DEL1); TODO : Make an external LED to toggle its light
          }

          ++counter;

          if(counter >= 160000){
              isRecording = false;
              //DSK6713_LED_off(DEL1); TODO : Make external LED turn off its light
              setEndOfLastRecordingAddress();
              counter = 0;
              printf("\nEND RECORDING\n\n");
          }
      }
}

void handlePlay(){
    if(!isPlaying){
        isPlaying = true;
        //setLed(DEL1, HIGH); TODO : Make an external LED to light its light
        resetSDRAMIterator();
        printf("\nBEGIN PLAYING\n\n");
    }

    else if(getSDRAMAddressIt() > getEndOfLastRecordingAddress()){
        isPlaying = false;
        //setLed(DEL1, LOW); TODO : Make external LED turn off its light
        printf("\nEND PLAYING\n\n");
    }
}

