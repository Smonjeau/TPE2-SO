#ifndef __STDLIB__H
#define __STDLIB__H
void write(char * rbx, unsigned int rcx, unsigned int rdx);
unsigned int read (unsigned int rbx, char * rcx,unsigned int rdx);
unsigned int scan(char * buffer, unsigned int len);
void putChar(char c);
char getChar();
void printString(char * str);
unsigned int strlen(char * str);
unsigned int commandEql(char * str1, char * str2);

#endif