#include <Utilities.h>

void SyscallReadMem(char * from, char * buffer) {
	int iByte;
	unsigned char aux;


	for(iByte = 0; iByte < 32; iByte++) {
		//Por cada byte leo 2 valores hexa
		aux = from[iByte] & 0xF0;// & 1111 0000
		aux = aux >> 4;
		buffer[iByte * 2] = getPrintableChar(aux);
		aux = from[iByte] & 0x0F;// & 0000 1111
		buffer[iByte * 2 + 1] = getPrintableChar(aux);
	}
	buffer[iByte * 2] = 0;
}

