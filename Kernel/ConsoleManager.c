#include <ConsoleManager.h>
#include <videoDriver.h>
#include <font.h>

#define NEGRO 0
#define VERDE 1
#define MARGINVERTICAL 5 //pixeles de margen en la consola
#define MARGINHORIZONTAL 2
#define MARGIN 8

unsigned char currentConsoleNum;

static ConsoleUnit consolas[2];

unsigned char inReadMode = 0;


void pintarBorde(unsigned int consoleNum, unsigned char color)
{
	unsigned int startCol = 1;
	unsigned int endCol = ANCHO/2-1;
	if(consoleNum == CONSOLE_B) {
		startCol += endCol;
		endCol += endCol;
	}
	//Pinto techo
	for(int col = startCol; col <= endCol; col++ )
		if(color == NEGRO)
			writePixel(col, 0, 0,0,0);
		else
			writePixel(col, 0, 0,255,0);
	//Pinto piso
	for(int col = startCol; col <= endCol; col++ )
		if(color == NEGRO)
			writePixel(col, ALTURA-1, 0,0,0);
		else
			writePixel(col, ALTURA-1, 0,255,0);

	//Pinto pared izquierda
	for(int row = 0; row < ALTURA; row++ )
		if(color == NEGRO)
			writePixel(startCol, row, 0,0,0);
		else
			writePixel(startCol, row, 0,255,0);

	//Pinto pared derecha
	for(int row = 0; row < ALTURA; row++ )
		if(color == NEGRO)
			writePixel(endCol, row, 0,0,0);
		else
			writePixel(endCol, row, 0,255,0);
}

void setActiveConsole(unsigned char newCurrentConsole)
{
	
	
	if(newCurrentConsole == CONSOLE_B) {
		pintarBorde(CONSOLE_A, NEGRO);
		pintarBorde(CONSOLE_B, VERDE);
	} else {
		pintarBorde(CONSOLE_A, VERDE);
		pintarBorde(CONSOLE_B, NEGRO);
	}

	currentConsoleNum = newCurrentConsole;

}


void initConsoles() {
	//Inicializo consola A
	consolas[CONSOLE_A].startColumn = MARGIN;
	consolas[CONSOLE_A].currentPos = 0; // Posicion 0 de la consola
	consolas[CONSOLE_A].startReadPos = 0;

	//Inicializo consola B
	consolas[CONSOLE_B].startColumn = (ANCHO / 2) + MARGIN;
	consolas[CONSOLE_B].currentPos = 0; // Posicion 0 de la consola
	consolas[CONSOLE_B].startReadPos = 0;

	setActiveConsole(CONSOLE_A);

}


void draw(unsigned char chr, int rowStart, int colStart)
{
	int set, x, y;
	char * bitmap = font8x8_basic[chr]; 
	
	for (y=0; y < 8; y++) {
        for (x=0; x < 8; x++) {
            set = bitmap[x] & 1 << y;
			if (set) {
				writePixel(rowStart + y*2, colStart + x*2, 255, 255, 255);
				writePixel(rowStart + y*2 + 1, colStart + x*2, 255, 255, 255);
				writePixel(rowStart + y*2, colStart + x*2 +1, 255, 255, 255);
				writePixel(rowStart + y*2 +1, colStart + x*2+1, 255, 255, 255);
			} else {
				writePixel(rowStart + y*2, colStart + x*2, 0, 0, 0);
				writePixel(rowStart + y*2 + 1, colStart + x*2, 0, 0, 0);
				writePixel(rowStart + y*2, colStart + x*2 +1, 0, 0, 0);
				writePixel(rowStart + y*2 +1, colStart + x*2+1, 0, 0, 0);
			}
        }
    }
}

unsigned int getTotalColumns()
{
	return (ANCHO/2 - 1 - MARGIN - MARGIN) / (16 + MARGINHORIZONTAL);
}
unsigned int getTotalRows()
{
	return (ALTURA - MARGIN - MARGIN) / (16 + MARGINVERTICAL);
}

void printLineBreak() {
	ConsoleUnit currentConsole = consolas[currentConsoleNum];
	currentConsole.currentPos++; //Así permite incluso hacer multiples line breaks
	unsigned int totalColumns = getTotalColumns();
	int column = currentConsole.currentPos % totalColumns;
	while(column != 0) {
		showCharInScreen(' ');
		currentConsole.currentPos++;
		column = currentConsole.currentPos % totalColumns;
	}
	consolas[currentConsoleNum] = currentConsole;
}
void backSpace()
{
	ConsoleUnit currentConsole = consolas[currentConsoleNum];
	if(currentConsole.currentPos > currentConsole.startReadPos) { //Para evitar que borre texto que no le pertenece
		currentConsole.currentPos--;
		unsigned int totalColumns = getTotalColumns();
		int row = currentConsole.currentPos / totalColumns;
		int column = currentConsole.currentPos % totalColumns;
		draw(0, currentConsole.startColumn + column * (16 + MARGINHORIZONTAL), MARGIN + row*(16+MARGINVERTICAL));
		consolas[currentConsoleNum] = currentConsole;
	}
	
}

void verifyNeedMoveUp() {	
	//Necesito verificar que si se lleno la consola, se mueva todo para arriba.
	ConsoleUnit currentConsole = consolas[currentConsoleNum];
	unsigned int totalRows = getTotalRows();
	unsigned int totalColumns = getTotalColumns();
	int row = currentConsole.currentPos / totalColumns;
	if(row > totalRows) {
		//En este caso vamos a mover todo 5 lineas para arriba.
		unsigned int rowsToMove = 8; //Se puede cambiar y funciona
		unsigned int startCol = 1;
		unsigned int endCol = ANCHO/2-1;
		if(currentConsoleNum == CONSOLE_B) {
			startCol += endCol;
			endCol += endCol;
		}
		for(unsigned int row = MARGIN; row <= ((ALTURA - MARGIN) - rowsToMove*(16+MARGINVERTICAL)); row++) {
			for(unsigned int col = startCol+1; col <= endCol; col++) {
				char * pixelData = getPixelDataByPos(col, row + rowsToMove*(16+MARGINVERTICAL));
				writePixel(col, row, pixelData[0], pixelData[1], pixelData[2]);
			}
		}
		consolas[currentConsoleNum] = currentConsole;


		//Falta poner en negro toda la parte de abajo que se liberó
		for(unsigned int counter = 0; counter < totalColumns * rowsToMove; counter++)
			backSpace();


	}
	
	
}

void showCharInScreen(char chr) // FALTA HACER CLEAR SI LLEGO ABAJO DE TODO
{
	
	if(chr == '\n') //Enter
	{
		printLineBreak();
		
	}
	else if(chr == '\b') //Backspace
	{
			backSpace();
	}
	else
	{
		
		ConsoleUnit currentConsole = consolas[currentConsoleNum];
		unsigned int totalColumns = getTotalColumns();
		int row = currentConsole.currentPos / totalColumns;
		int column = currentConsole.currentPos % totalColumns;
		draw(chr, currentConsole.startColumn + column * (16 + MARGINHORIZONTAL), MARGIN + row*(16+MARGINVERTICAL));
		consolas[currentConsoleNum].currentPos++;	
	}
	verifyNeedMoveUp();
	
}

void showStringInScreen(char * str, unsigned int len)
{
	unsigned int strPos;
	for(strPos = 0; strPos < len && str[strPos] != 0; strPos++)
		showCharInScreen(str[strPos]);
}

void clearConsole() {
	while(consolas[currentConsoleNum].currentPos > 0)
		showCharInScreen('\b');	
}
void clearAll() {
	clearConsole();
	if(currentConsoleNum == CONSOLE_A)
		currentConsoleNum = CONSOLE_B;
	else
		currentConsoleNum = CONSOLE_A;
	clearConsole();
}