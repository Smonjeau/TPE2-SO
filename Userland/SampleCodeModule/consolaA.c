#include <consolaA.h>
#include <stdlib.h>
#include <lib.h>
#include <expressionManager.h>
#include <registers.h>
#define MAX 255
#define ESC_ASCII 27

unsigned int consoleNumA;
unsigned int inputReadSizeA = 0;

char inputReadA[MAX] = {0};

int findError(int status) {
	switch(status) {
		case 0:
			printString("Ingreso un caracter invalido.\n");
			break;
		case 1:
			printString("Falta un \"(\".\n");
			break;
		case 2:
			printString("Falta un numero luego del \"-\".\n");
			break;
		case 3:
			printString("Falta un numero luego del \".\".\n");
			break;
		case 4:
			printString("Puntos encadenados.\n");
			break;
		case 5:
			printString("Falta numero antes del \".\".\n");
			break;
		case 6:
			printString("Faltan operandos.\n");
			break;
		case 7:
			printString("Faltan operadores.\n");
			break;
		case 8:
			printString("No puede dividir por 0.\n");
			break;
		case 9:
			printString("Falta un \")\".\n");
			break;	
		default:
			return 0; //Sin errores
			break;
	}
	return 1; //Error
}

unsigned int finishCharA(char chr) {
	return chr == '=';
}

unsigned int consoleAFinishHandler() {
	putChar('=');
	
	inputReadA[inputReadSizeA] = 0;//Le agrego 0 final

	char resultado[MAX];

	int status = evaluateInFija(inputReadA, resultado);
	putChar('\n');
	if(!findError(status)) {
		printString(resultado);
	}
	printString("\nIngrese una expresion. Recuerde que el operador - va seguido de espacio.\n");
	





	//Reseteamos el buffer
	inputReadSizeA = 0;
	inputReadA[0] = 0;
	return 1;
}
void consoleAKeyHandler(char input) {
	if(input == ESC_ASCII) {
		while(inputReadSizeA > 0) {
			inputReadA[inputReadSizeA--] = 0;
			putChar('\b');
		}
	} else if(input == '\b') {

		if(inputReadSizeA > 0) {
			//Logicamente solo permite borrar si hay algo escrito.
			//Asi evitamos que borrer cosas que no le pertenece.
			inputReadA[inputReadSizeA--] = 0;
			putChar(input);

		}
	} else if(input == 2) {
		//El 2 equivale a la captura de registros. Si bien no es para esta consola, 
		//permitimos igualmente que se pueda hacer captura desde aqui para que se pueda comparar
		//entre el estado de los registros en una consola y otra.
		setRegistersFlag(); //Alt hace captura de registros

		//Quiero printear en la otra consola
		setActiveConsole(CONSOLE_B_ID);
		printString("\nCaptura de registros hecha.");
		setActiveConsole(CONSOLE_A_ID);

	} else if(input == '\n') {
		//No haga nada, en la calcu no hace nada enter.
	} else if(inputReadSizeA < MAX) {
		inputReadA[inputReadSizeA++] = input;
		putChar(input);
	}

	//Si el buffer esta lleno no se lee mas.
	
	
	
}

void showConsoleA(unsigned int fConsoleNum) {
	consoleNumA = fConsoleNum;
	activateConsole(consoleNumA, consoleAKeyHandler, consoleAFinishHandler, finishCharA);

	printString("Ingrese expresion. El operando - va seguido de espacio. Esc borra expresion.\n");
}

