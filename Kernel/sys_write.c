#include <ConsoleManager.h>
void SyscallWrite(char * rbx, unsigned int rcx, unsigned int rdx)
{
    switch (rdx)
    {
	    case 0: //fd pantalla
      		showStringInScreen(rbx,rcx);
	        break;
    }
} 