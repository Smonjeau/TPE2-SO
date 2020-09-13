#include <Utilities.h>
extern unsigned int get_Time(char t);
void SyscallGetTime (char t,int * result){
  *result = get_Time_format(get_Time(t));
  return;
}
