#ifndef __KEYBOARD_DRIVER_H_
#define __KEYBOARD_DRIVER_H_
#define PRESSED 1
#define RELEASED 0
#define ACTIONCODE_ERROR -1
//static int enterPressed=0;
//static int currentCode;

void keyboard_handler();
unsigned int readStandardInput(char * buffer,int dim);












#endif