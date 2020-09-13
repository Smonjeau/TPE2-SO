#include <Utilities.h>

int get_Time_format(int t){
    return (t/16)*10+(t%16);
}
void numToString(long num, char * buffer){
    int i=0;
    int j=0;
    while(num > 0){
        buffer[i++] = num%10 + '0';
        num = num / 10 ;
    }
    char aux; //j apunta al comienzo del buffer, i al final
    buffer[i--]=0;
     while(j<i){
        aux = buffer[i];
        buffer[i] = buffer[j];
        buffer[j]=aux;
        j++;
        i--;
    }  
}
char getPrintableChar(unsigned char hexaNum) {
	return (hexaNum < 0xA) ? hexaNum + '0' : hexaNum + 'A' - 10;
}