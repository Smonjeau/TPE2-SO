#ifndef __LIB__H
#define __LIB__H

#define CONSOLE_A_ID 0
#define CONSOLE_B_ID 1

void init_consoles();
void printTime();
void activateConsole(unsigned int consoleNum, void (*keyHandler) (char), unsigned int (* finishHandler)(char *), unsigned int (*finishKeyHandler) (char));
unsigned int handleFinish();
void handleKey(char key);
void numToString(long num, char * buffer);
int showMemRead(char * str);
void printMem(char * from, char * buffer);
int isNumber(char c);
unsigned int isFinishKey(char chr);
void printModelInfo();
void printTemp();
extern void setActiveConsole(char console);
#endif
