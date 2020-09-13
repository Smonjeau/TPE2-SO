#include <ConsoleManager.h>
#include <setup.h>
#include <time.h>
#include <stdint.h>
#include <Utilities.h>
#include <interrupts.h>

#define ZERO_EXCEPTION_ID 0
#define OP_CODE_EXCEPTION_ID 6
#define TIME_FOR_EX_MESSAGE 8
#define REGISTER_SIZE 8
static void zero_division(uint64_t rsp);
static void notValidOpCode(uint64_t rsp);

extern void recoverRSP(void * const sampleCodeModuleAddress);

extern void saveRegisters(uint64_t rsp);

extern char * getRegistersData();

void inforeg(){
    char * data = getRegistersData();
    int i;
    unsigned char aux;
    int iByte;
    int idx;
    char buffer [17]; //16 + 1 para el cero
    char registersName [18][6] = {"R15","R14","R13","R12","R11","R10"," R9"
                                   ," R8","RAX","RBX","RCX","RDX","RDI","RSI","RBP","RIP","FLAGS","RSP"};                                          
    for(i=0;i<18;i++){
        showStringInScreen(registersName[i],8);
        showStringInScreen(": 0x",5);
        
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
        showStringInScreen(buffer,16);
        showCharInScreen('\n');

    }
}

void exceptionDispatcher(int exception,uint64_t rsp) {
	switch (exception)
	{
	case ZERO_EXCEPTION_ID:
		zero_division(rsp);
		break;
	
	case OP_CODE_EXCEPTION_ID:
		notValidOpCode(rsp);
		break;
	}
}
void generic_ex_handler(uint64_t rsp, char * msg, int len){
	// Handler para manejar excepcíon
	/*
	La idea es popear del stack hasta alcanzar el mismo nivel que el main 
	donde inicializa el userspace. Asi podemos recuperar la consola sin
	llenar el stack innecesariamente.
	*/
	int startTime = seconds_elapsed();
	int finishTime = startTime + TIME_FOR_EX_MESSAGE;
	
	showStringInScreen(msg, len);
	saveRegisters(rsp);
	inforeg();
	
	while(seconds_elapsed() < finishTime)
		_hlt(); //Pues el tick es una interrupcion. Evitamos consumir recursos innecesariamente.
	clearAll();
	recoverRSP(sampleCodeModuleAddress);	
}
 void zero_division(uint64_t rsp) {
	generic_ex_handler(rsp,"\nExcepcion division por cero. Reiniciando...\n",50);
	return;	
}
 void notValidOpCode( uint64_t rsp)
{
	generic_ex_handler(rsp,"\nExcepcion codigo de op invalido. Reiniciando...\n",50);
	return;
}