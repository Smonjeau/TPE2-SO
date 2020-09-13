#include <stack.h>
/*Este stack sirve para el algoritmo de evaluar expresiones. Al recibir un string hace una copia elemento por elemento
y no por referencia. Esto es porque el algoritmo requiere pushear valores intermedios que, al depender de la expresion,
se tiene que manejar de manera dinámica, con la cual no contamos. Es por esto que predefinimos una matriz cuadrada de SIZE
en la que se guardan las copias de los strings*/
 char stackVec [SIZE][SIZE];// ={0};
 int stackDim=0;
void push( char * p){
    int index = stackDim++;
    int i;
    for(i=0;p[i]!=0 &&p[i]!=' ';i++){
        stackVec[index][i]=p[i];
    }
    stackVec[index][i]=0;
    return;
}
char * peek(){
    if(stackDim>0)
        return stackVec[stackDim-1];
    return 0;
}
char *  pop(){
    if(stackDim>0)
        return stackVec[--stackDim];
    return 0;
}

unsigned int size(){
    return stackDim; 
}