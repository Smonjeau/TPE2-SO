#include <ConsoleManager.h>
void SyscallSetConsole(unsigned char console)
{
    if(console ==0 || console ==1)
    {
        setActiveConsole(console);
    }
}