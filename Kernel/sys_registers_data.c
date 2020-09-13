extern char * getRegistersData();
void SyscallRegistersData(char * buffer){
    char * registers = getRegistersData();
    for(int i=0;i<144;i++){
        buffer[i]=registers[i];
    }
}