#include <consolaB.h>
#include <stdlib.h>
#include <lib.h>
#include <expressionManager.h>
#include <registers.h>
#include <stdint.h>
extern void ex6();
extern void ex6_2();
extern void ex0();
extern void testRegisters();
#define ESC_ASCII 27
#define MAX 255

unsigned int consoleNumB;
unsigned int inputReadSizeB = 0;

char inputReadB[MAX] = {0};

int testEx0(){
	return 1/0;
}

void help() {
	printString("Ventana de comandos, pulse Ctrl + 1/2 para cambiar de consola\n");
	printString("Comandos posibles:\n");
	printString("help - Ver comandos\n");
	printString("time - Consultar hora del sistema\n");
	printString("inforeg - Estado de\nregistros. Primero debe capturar con Alt\n");
	printString("printmem 0xDIR - Volcado de memoria\n");
	printString("model - Consultar datos de CPU\n");
	printString("temp - Consultar temperatura de CPU\n");
	return;
}

void asignarModulo(char * str) {
	if(commandEql(str, "help")) {
		help();
	}
	else if(commandEql(str,"time")){
		printTime();
	}
	else if(commandEql(str,"ex6test")){
		ex6();
	}
	else if(commandEql(str,"ex0test")){
		testEx0();
	}
	else if(commandEql(str,"ex0test2")){
		ex0();
	}
	else if(commandEql(str,"ex6test2")){
		ex6_2();
	}
	else if(commandEql(str,"inforeg")){
		if(!areRegistersCaptured())
			printString("No se capturaron los registros\n");
		else
			inforeg();
	}
	else if(commandEql(str,"testreg")){
		printString("Registros alterados\n");
		testRegisters();
	}
	else if(commandEql(str, "printmem")) {
		if(!showMemRead(str))
			printString("Ingrese una direccion como argumento\n");
	}
	else if (commandEql(str, "model")){
		printModelInfo();
	}
	else if (commandEql(str, "temp")){
		printTemp();
	}
	else {
		printString("Ingrese un comando valido.\n");
	}
}


unsigned int consoleBFinishHandler() {
	inputReadB[inputReadSizeB] = 0;//Le agrego 0 final
	putChar('\n');

	asignarModulo(inputReadB);

	//Reseteamos el buffer
	inputReadSizeB = 0;
	inputReadB[0] = 0;
	return 1;
}
unsigned int finishCharB(char chr) {
	return chr == '\n';
}
void consoleBKeyHandler(char input) {
	if(input == 2) {
		/*Existe el caso de que primero comenzo a escribir cosas y luego presiono Alt.
		En ese caso, para que el texto no quede inmerso en el texto viejo, se lo borra y se lo printea
		luego de mostrar la leyendo de captura hecha.*/

		setRegistersFlag(); //Alt hace captura de registros
		
		//Primero "borro" si es que hay texto impreso antes
		for(int i = 0; i < inputReadSizeB; i++)
			putChar('\b');
		//Imprimo la leyenda
		printString("Captura de registros hecha.");

		//Vuelvo a printear lo que ya estaba, si es que habia
		for(int i = 0; i < inputReadSizeB; i++)
			putChar(inputReadB[i]);

		
	}
	else if( input == ESC_ASCII) {
		//IGNORAR


	} else if(input == '\b'){

		if(inputReadSizeB > 0) {
			//Logicamente solo permite borrar si hay algo escrito.
			//Asi evitamos que borrer cosas que no le pertenece.
			inputReadB[inputReadSizeB--] = 0;
			putChar(input);

		}
	} else if(inputReadSizeB < MAX) {
		inputReadB[inputReadSizeB++] = input;
		putChar(input);
	}


	//Si el buffer esta lleno no se lee mas.




}

void showConsoleB(unsigned int fConsoleNum) {
	consoleNumB = fConsoleNum;
	activateConsole(consoleNumB, consoleBKeyHandler, consoleBFinishHandler, finishCharB);
	help();
}
