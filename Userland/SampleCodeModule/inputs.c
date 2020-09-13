#include <inputs.h>
#include <stdlib.h>
#include <lib.h>


void readKeyBoard() {
	char chr;
	
	do {
		
		while(!isFinishKey((chr = getChar())))
			handleKey(chr);
	} while(handleFinish());
}