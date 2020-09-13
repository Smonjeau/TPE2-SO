#ifndef __VIDEO_DRIVER_H_
#define __VIDEO_DRIVER_H_

#define ALTURA 768
#define ANCHO 1024

char * getPixelDataByPos(int x, int y);
void writePixel(int x, int y,char blue,char green,char red);
void printString(char * s, unsigned int len);


#endif