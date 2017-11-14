/*
 * playback.c
 *
 *  Created on: 12 nov. 2017
 *      Author: Humanismus
 */
#include "playback.h"

//Global program process
bool isRecording = false;
bool isPlaying = false;
bool flagInt11 = false;
int counter = 0;
bool GPIO9_toggler = false;

GPIO_Handle hGpio;

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

              GPIO9_toggler = !GPIO9_toggler;
              GPIO_pinWrite(hGpio,GPIO_PIN9,GPIO9_toggler);
          }

          ++counter;

          if(counter >= 160000){
              isRecording = false;
              GPIO_pinWrite(hGpio,GPIO_PIN9, 0);
              setEndOfLastRecordingAddress();
              counter = 0;
              printf("\nEND RECORDING\n\n");
          }
      }
}

void handlePlay(){
    if(!isPlaying){
        isPlaying = true;
        GPIO_pinWrite(hGpio,GPIO_PIN9, 1);
        resetSDRAMIterator();
        printf("\nBEGIN PLAYING\n\n");
    }

    else if(getSDRAMAddressIt() > getEndOfLastRecordingAddress()){
        isPlaying = false;
        GPIO_pinWrite(hGpio,GPIO_PIN9, 0);
        printf("\nEND PLAYING\n\n");
    }
}

