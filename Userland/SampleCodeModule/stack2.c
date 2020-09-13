#include <stack2.h>
/*Este stack sirve para pasar de expresiones postfija a infija*/
 char  stackVec2 [SIZE2] ={0};
 int stackDim2=0;
void push2( char  p){
    stackVec2[stackDim2++]= p;
    return;
}
char  peek2(){
    if(stackDim2>0)
        return stackVec2[stackDim2-1];
    return 0;
}
char  pop2(){
    if(stackDim2>0)
        return stackVec2[--stackDim2];
    return 0;
}
unsigned int size2(){
    return stackDim2; 
}