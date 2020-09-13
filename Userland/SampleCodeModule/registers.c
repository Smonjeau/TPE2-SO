
#include <stdlib.h>
#include <stdint.h>
#define REGISTER_SIZE 8 //en bytes

extern char * getRegistersData();
extern void getRegisters(char * buffer);
char getPrintableChar(unsigned char hexaNum) {
    return (hexaNum < 0xA) ? hexaNum + '0' : hexaNum + 'A' - 10;
}
void inforeg(){
    char data [144];

    getRegisters(data);
 
    int i;
    unsigned char aux;
    int iByte;
    int idx;
    char buffer [17]; //16 + 1 para el cero
    char registersName [18][6] = {"  R15","  R14","  R13","  R12","  R11","  R10","   R9"
                                   ,"   R8","  RAX","  RBX","  RCX","  RDX","  RDI","  RSI","  RBP","  RIP","FLAGS","  RSP"};                                          
    for(i=0;i<18;i++){
        printString(registersName[i]);
        printString(": 0x");
        
        for(iByte = i*REGISTER_SIZE; iByte < i*REGISTER_SIZE + REGISTER_SIZE; iByte++) {
    
            idx=REGISTER_SIZE-1-iByte%REGISTER_SIZE;  /*Para quedarme dentro de los limites de buffer[].
            Además consideramos que se guarda en little endian*/
            //Por cada byte leo 2 valores hexa
            
            aux = (data[iByte] & 0xF0); // & 1111 0000
            aux= aux >> 4;                                                
            buffer[idx * 2] = getPrintableChar(aux);
            aux = data[iByte] & 0x0F;// & 0000 1111
            buffer[idx * 2 + 1] = getPrintableChar(aux);
        }

        buffer[16] = 0;
        printString(buffer);
        putChar('\n');

    }
}