#include <stdint.h>
#include <keyboardDriver.h>
#include <ConsoleManager.h>
#include <interrupts.h>
//scancodes de teclas especiales
#define CONTROL_PRESSED 29
#define CONTROL_RELEASED 157
#define LEFT_SHIFT_PRESSED 42
#define LEFT_SHIFT_RELEASED 170
#define RIGHT_SHIFT_PRESSED 54
#define RIGHT_SHIFT_RELEASED 182
#define BLOQMAYUS 58
#define ENTER 28
#define ESC_SCANCODE 0x81
#define ESC_ASCII 27
#define CAPTURE_FLAGS 2
#define ALT_SCAN_CODE 0xb8
//entre codes de pressed y released hay un defasaje de 128
#define INPUT_BUFFER 255 //Cant max de caracteres en buffer de input
extern void saveRegisters(uint64_t rsp);
static char keyboardBuffer[INPUT_BUFFER];
static unsigned int bufferSize = 0;

//estado de teclas especiales
static int pressingShift =0;
static int bloqMayus = 0; //boolean

static int pressingCtrl =0;
extern uint8_t getKeyScanCode();

//matriz obtenida de http://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/scancode 
static char asccode[58][2] ={
    { 0, 0 }, { 0, 0 }, { '1','!' }, { '2','@' },
    { '3','#' }, { '4','$' }, { '5','%' }, { '6','^' },
    { '7','&' }, { '8','*' }, { '9','(' }, { '0',')' },
    { '-','_' }, { '=','+' }, {'\b','\b'}, {'\t','\t'},
    { 'q','Q' }, { 'w','W' }, { 'e','E' }, { 'r','R' },
    { 't','T' }, { 'y','Y' }, { 'u','U' }, { 'i','I' },
    { 'o','O' }, { 'p','P' }, { '[','{' }, { ']','}' },
    {'\n','\n'}, {   0,0   }, { 'a','A' }, { 's','S' },
    { 'd','D' }, { 'f','F' }, { 'g','G' }, { 'h','H' },
    { 'j','J' }, { 'k','K' }, { 'l','L' }, { ';',':' },
    {'\'','\"'}, { '`','~' }, {   0,0   }, { '\\','|'},
    { 'z','Z' }, { 'x','X' }, { 'c','C' }, { 'v','V' },
    { 'b','B' }, { 'n','N' }, { 'm','M' }, { ',','<' },
    { '.','>' }, { '/','?' }, {   0,0   }, {   0,0   },
    {   0,0   }, { ' ',' ' }
};



int codeMatchesAscii(int scancode)
{
  return scancode >=0 && scancode< 58 && asccode[scancode][1] !=0;
}
int getScanCode()
{
  int code = getKeyScanCode();
  if(code==CONTROL_PRESSED)
    pressingCtrl=1;
  else if(code == CONTROL_RELEASED)
    pressingCtrl=0;
  else if(code == LEFT_SHIFT_PRESSED || code == RIGHT_SHIFT_PRESSED)
    pressingShift=1;
  else if (code== LEFT_SHIFT_RELEASED || code == RIGHT_SHIFT_RELEASED)
    pressingShift=0;
  else if(code==BLOQMAYUS)
    bloqMayus = !bloqMayus;

  return code;
}
int isLetter(char c)
{
  return (c>='a' && c<='z') || (c>='A' && c<='Z');
}
char getMatchingAscii( int scancode)
{
  if(codeMatchesAscii(scancode))
  {
  
    if(bloqMayus && isLetter(asccode[scancode][1]))
    {
      return asccode[scancode][1];
    }
    // si llega aca es porque esta con bloqmayus pero no tocando una letra, en cuyo caso debo ignorar este flag, 
    // o porque no está tocando la mayus, asi q solo me falta comprobar el shift
    return asccode[scancode][pressingShift]; 
  }
  return -1;
}
void onKeyPressed(char chr) {
  if(bufferSize < INPUT_BUFFER)
    keyboardBuffer[bufferSize++] = chr;
}
unsigned int readStandardInput(char * buffer,int dim) {
  while(bufferSize == 0)
    _hlt();
  
  unsigned int i;
  for(i = 0; i < dim && i < bufferSize; i++)
    buffer[i] = keyboardBuffer[i];
  //Ahora muevo el buffer a la izquierda "i" veces
  if(i > 0)
    for(int j = 0; j < bufferSize-i; j++)
      keyboardBuffer[j] = keyboardBuffer[j+i];
  bufferSize -= i;
  return i;
  
}
void keyboard_handler(uint64_t rsp)
{
    int scanCode = getScanCode();
    char c;


    if (scanCode == 2 && pressingCtrl){
      //Se presiono 1 teniendo antes control presionado (se presionan ambos)
      //Se activa la ventana A
      //setActiveConsole(CONSOLE_A); //Esto ahora se hace en userland
      c = CONSOLE_A;
    } else if (scanCode == 3 && pressingCtrl){
      //Se presiono 2 teniendo antes control presionado (se presionan ambos)
      //Se activa la ventana B
      //setActiveConsole(CONSOLE_B); //Esto ahora se hace en userland
      c = CONSOLE_B;
    } else if(scanCode == ESC_SCANCODE) {
      c = ESC_ASCII;
    } else if(scanCode == ALT_SCAN_CODE) {
      c = CAPTURE_FLAGS;

      saveRegisters(rsp);

    }
    else if(codeMatchesAscii(scanCode) && !pressingCtrl)
    {
        c = getMatchingAscii(scanCode);
    } else {
      return;
    }
    onKeyPressed(c);
    
  
}
