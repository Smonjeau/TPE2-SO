#include <stdlib.h>
unsigned int strlen(char * str){
    int i=0;
    while (str[i]!=0)
    {
        i++;
    }
    return i;
}
void putChar(char c){
    write(&c,1,0);
    return;
}
char getChar() {
    char buffer[1];
    if(read(0, buffer, 1) == 0)
        return 0;
    else
        return buffer[0];
}
void printString(char * str){
    /*Preferimos recorrer una vez para calcular la long del string y hacer un solo llamado a la syscall
    en lugar de hacer un putChar (por ende int80h) hasta encontrar el */
    write(str,strlen(str),0);
    return;
}
unsigned int commandEql(char * str1, char * str2) {
    while(*str1 == ' ')
        str1++; //Hago un "trim"

    int eql = 1, i;
    for(i = 0; str2[i] != 0; i++) //El de la derecha es el comando a comparar.
        if(str1[i] != str2[i])
            eql = 0;
    if(str1[i] != 0 && str1[i] != ' ')
        eql = 0;
    return eql;    
}

unsigned int scan(char * buffer, unsigned int len) {
    return read(0, buffer, len);
}