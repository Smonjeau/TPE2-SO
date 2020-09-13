#include <lib.h>
#include <stdint.h>
#include <stdlib.h>
extern void initC();
extern void getTime(char t,int * result);
extern int cpuModel();
extern int cpuExtendedModel();
extern int cpuFamily();
extern char * cpuBrand();
extern char * cpuVendor();
extern int getTemp(int * temp);

/*Si bien el kernel lo maneja, tambien nos guardamos este estado del lado del userland para evitar
hacer una syscall solo para saber cual es la consola activa.*/
unsigned int activeConsoleNum;
unsigned int (*consoleFinishHandlers[2]) ();
unsigned int (*finishKeyHandlers[2]) (char chr);
void (*consoleKeyHandlers[2]) (char);
unsigned int errorFlag;

void init_consoles(){
    initC();
    return;
}

void printTime(){
	int hours,minutes,seconds;
	getTime(4,&hours);
	getTime(2,&minutes);
	getTime(0,&seconds);
    char buffer [3];
    numToString(hours,buffer);
    printString(buffer);
    printString(" hs ");
    numToString(minutes,buffer);
    printString(buffer);
    printString(" m ");
    numToString(seconds,buffer);
    printString(buffer);
    printString(" s");
    putChar('\n');
    return;
}
unsigned int isFinishKey(char chr) {
	return finishKeyHandlers[activeConsoleNum](chr);
}
void activateConsole(unsigned int consoleNum, void (*keyHandler) (char), unsigned int (* finishHandler)(char *), unsigned int (*finishKeyHandler) (char)) {
	activeConsoleNum = consoleNum;
	consoleKeyHandlers[consoleNum] = keyHandler;
	consoleFinishHandlers[consoleNum] = finishHandler;
	finishKeyHandlers[consoleNum] = finishKeyHandler;
	setActiveConsole(consoleNum);
}
void numToString(long num, char * buffer){
    if (num==0){
      buffer[0] = '0';
      return;
    }
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

void handleKey(char key) {
 	 if((key+1) == 0x01 || (key+1) == 0x02) { //Definimos chars especiales para manejar cambios de consola
		activeConsoleNum = key;
		setActiveConsole(activeConsoleNum);
	} else {
		consoleKeyHandlers[activeConsoleNum](key);
	}

}
unsigned int handleFinish() {
	return consoleFinishHandlers[activeConsoleNum]();
}
int isHexaDigit(char digit) {
	return (digit >= '0' && digit <= '9') || (digit >= 'A' && digit <= 'F') || (digit >= 'a' && digit <= 'f');
}
char * parseStrToHexa(char * str) {
	errorFlag = 0;
	uint64_t number = 0;
	unsigned int len = 0;
	while(*str != 0 && *str != ' ' && !errorFlag) {
		if(!isHexaDigit(*str))
			errorFlag = 1;
		number *= 16; //Multiplico por 16 para correr todo un lugar a la izquierda
		if(*str >= '0' && *str <= '9')
			number += (*str - '0');
		else if(*str >= 'A' && *str <= 'F')
			number += (*str - 'A');
		else
			number += (*str - 'a');
		str++;
		len++;
	}
	if(len > 16)
		errorFlag = 1;
	return (char *)number;
}
int isNumber(char c){
	return c>='0' && c<='9';
}
int showMemRead( char * str) {
	/*
	Como se lo puede testear? En la direccion 0x500000 esta el dataModule seteado por el kernel, el cual contiene el siguiente texto
	"HOLA DON PEPITO HOLA DON JOSE AA"
	por lo que al invocar printmem con dicha direccion se debe obtener el hexa correspondiente al texto anterior.
	*/
	while(*str == ' ')
		str++; //Hacemos un trim al principio
	while(*str != ' ')
		str++; //Salteamos el printmem
	while(*str == ' ' && *str != 0)
		str++; //Salteo espacios intermedios
	if(*str == 0)
		return 0; //No recibio argumento
	if(*str == '0' && (*(str+1)=='x' || *(str+1)=='X') && isHexaDigit(*(str+2))) {
		str++; //Me salteo el 0x inicial
		str++;

		char * startPointer = parseStrToHexa(str);
		if(errorFlag)
			return 0;
		char buffer[65]; //Se necesitan de N * 2 + 1 siendo N la cantidad de bytes a leer
		printMem(startPointer, buffer);
		printString(buffer);
		putChar('\n');


		return 1;

	} else {
		return 0;
	}
}

void printModelInfo(){
  int model_id = cpuModel();
  int model_id_ext = cpuExtendedModel();
  int model_id_family = cpuFamily();
  char buffer [4];
  char buffer_2 [100];

  numToString(model_id,buffer);
  printString("Model ID: ");
  printString(buffer);
  putChar('\n');

  numToString(model_id_ext,buffer);
  printString("Extended Model ID: ");
  printString(buffer);
  putChar('\n');

  numToString(model_id_family,buffer);
  printString("Family ID: ");
  printString(buffer);
  putChar('\n');

  printString("Vendor: ");
  printString(cpuVendor(buffer_2));
  putChar('\n');

  printString("Brand: ");
  printString(cpuBrand(buffer_2));
  putChar('\n');
}

void printTemp(){
  int temp = 0;
  getTemp(&temp);
  char buffer [4];

  numToString(temp,buffer);
  printString("CPU Temp: ");
  printString(buffer);
  putChar('C');
  putChar('\n');
}
