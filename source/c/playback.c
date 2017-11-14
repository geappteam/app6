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
bool toggler = false;
bool hasRecordedOnce = false;

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

              toggler = !toggler;
              if(toggler)
                  DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL1));
              else
                  DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL1));
          }

          ++counter;

          if(counter >= 160000){
              isRecording = false;
              DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL1));
              setEndOfLastRecordingAddress();
              counter = 0;
              hasRecordedOnce = true;
              printf("\nEND RECORDING\n\n");
          }
      }
}

void handlePlay(){
    if(!isPlaying){
        isPlaying = true;
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) | DC_CNTL1));
        resetSDRAMIterator();
        printf("\nBEGIN PLAYING\n\n");
    }

    else if(getSDRAMAddressIt() > getEndOfLastRecordingAddress()){
        isPlaying = false;
        DSK6713_rset(DSK6713_DC_REG, (DSK6713_rget(DSK6713_DC_REG) & ~DC_CNTL1));
        printf("\nEND PLAYING\n\n");
    }
}

