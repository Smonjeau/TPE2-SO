/* sampleCodeModule.c */
#include <stdlib.h>
#include <lib.h>
#include <consolaA.h>
#include <consolaB.h>
#include <inputs.h>

//Desde lib.h se puede modiifcar el id de la consola A y B, son genericas.




int main() {
	

	init_consoles();


	showConsoleA(CONSOLE_A_ID); //Le asigno el numero de consola que le va a corresponder
	showConsoleB(CONSOLE_B_ID);

	readKeyBoard();


	return 0;
}

