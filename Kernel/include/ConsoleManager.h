#ifndef __CONSOLE_MANAGER_H_
#define __CONSOLE_MANAGER_H_
#define CONSOLE_A 0x00
#define CONSOLE_B 0x01


/*
static unsigned char CONSOLE_A = 0;
static unsigned char CONSOLE_B = 1;*/




typedef struct {
	unsigned int startColumn;
	unsigned int currentPos;
	unsigned int startReadPos;
} ConsoleUnit;
typedef struct ConsoleUnit * pConsoleUnit;




void initConsoles();

void setActiveConsole(unsigned char newCurrentConsole);

void showCharInScreen(char chr);

void showStringInScreen(char * str, unsigned int len);

void clearAll();




#endif