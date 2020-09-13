#include <keyboardDriver.h>
int SyscallRead(unsigned int fd,char * buffer, unsigned int dim)
{
    //hacemos solo  de std input
    switch (fd)
    {
        case 0:
            return readStandardInput(buffer,dim); //retorno un valor <= dim
            
        default:
            return 0;
    }
    

}