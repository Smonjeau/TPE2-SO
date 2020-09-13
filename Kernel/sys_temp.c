#include <Utilities.h>
extern int cpuTemperature();

void SyscallGetTemperature (int * result){
  *result = cpuTemperature();
  return;
}
