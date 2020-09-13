
#include <expressionManager.h>
#include <lib.h>
#include <stdlib.h>
static int precedenceMatrix[5][6] ={
    {1,1,0,0,0,1},
    {1,1,0,0,0,1},
    {1,1,1,1,0,1},
    {1,1,1,1,0,1},
    {0,0,0,0,0,0}
};
/*Funcion para obtener la precedencia de los operadores en base a la matriz anterior */
int getPrecedence(char tope, char currentElem);


/*Operaciones básias manejadas por la calculadora*/
 int suma (char* a, char * b, char * result);
 int  resta(char * a, char * b,char * result);
 int  multiplicacion(char * a, char * b,char * result);
 int division (char * a ,char * b,char * result);


/*Devuelve un puntero a función de la operación*/
void getOperation (char operator, int (**fp) (char * a, char * b, char * result)){
    
    //Ya se que solo hay operadores validos.
     switch (operator)
    {
    case '+':
        *fp = suma;
        break;
    case '-':
        *fp=resta;
        break;
    case '*':
        *fp=multiplicacion;
        break;
    case '/':
        *fp=division;
        break;
    } 
   
    

 }
 //Redondeamos a 4 decimales como máximo
 void round(char * num){
     int i=0;
     int isNegative=0;
     char  digitsBeforePoint=0;
     while(num[i]!=0 && num[i]!='.'){
        if(num[i]!='-')
             digitsBeforePoint++;
        else
            isNegative=!isNegative;
        i++;

     }
    if(num[i]==0)
        return;
    i++;
    int j=i;
    while(num[i]!=0 && i<j+4){
        i++;
    }
    i--; //Por el incremento q corta el while
    if(num[i]==0 || num[i+1]==0)
        return;
    if(num[i+1] < '5'){
        num[i+1]=0;
    }
    else{
        int passedPoint=0;
        char aux=digitsBeforePoint;
        if(num[i]=='9'){
            while(num[i]=='9' &&aux>0){
                if(num[i-1]=='.'){
                    //i--;
                    passedPoint=1;
                    i--;
                }
                else{
                    //num[i--]='0';
                    if(passedPoint)
                        aux--;
                }
                i--;
            }
            if(aux==0){ 
                //En este caso me aumentarian las cifras de la parte entera del número
                //Es para casos de numeros como 99.99999, que al aproximar va a dar 100

                int k=0;
                if(isNegative)
                    num[k++]='-';
                num[k++]='1';
                while(digitsBeforePoint>0){
                    num[k++]='0';
                    digitsBeforePoint--;
                }
                num[k]=0;
            }
            else{
                
                num[i] += 1;
                num[i+1]=0;
            }
        }
        else{
            num[i] +=1;
            num[i+1] =0;
            
        }
    }
        
     
 }
int infijaToPostFija(char * infija,char * postfija);


/*El algoritmo evaluador implementado recibe una postfija*/
 int evaluatePostfija(char * exp, char * resultOfExp) 
{
    //Si la dimension del stack no es 0, lo vacio.
    while(size()!=0)
        pop();
    int (*operation) (char * a, char * b, char * r);
    int i = 0 ;
    char resultIntermedio[10];
    char * firstOperand;
    char * secondOperand;
    
   // putChar(size()+'0');
    while(exp[i]!=0)
    {   
        
        /*si tengo un numero, lo pongo en el stack de char *
        Hago la comp asi pues ya se que hay solo caracteres válidos, me lo asegura 
        la funcion q pasa a posfija. Me fijo si en la primer o segunda pos hay un numero    
        pues puede ser q sea negativo, entonces la primer pos tendría un - */
        while(exp[i]==' ')
            i++;
        if(exp[i]!=0){
            if(isNumber(exp[i])||isNumber(exp[i+1])){
                push(exp + i);
                while(exp[i]!=' '&& exp[i]!=0)
                    i++;
       
                
            }        
            else{
                //Tengo un operador
                getOperation(*(exp+i),&operation);
                if(size()<2)
                    return 6; //Error: Estan faltando operandos
                secondOperand = pop();
                firstOperand = pop();    
    
                if(!(operation(firstOperand,secondOperand,resultIntermedio)))
                    return 8;// Error: division por cero
                round(resultIntermedio);
                push(resultIntermedio);
                i++;
            }
            
            
        }
         
                                                
    }
    if(size()!=1)
        return 7; //Estan faltando operadores
    char * aux = pop();
    int k=0;
    for(;aux[k]!=0;k++){

        resultOfExp[k]=aux[k];
    }
    
    resultOfExp[k]=0;
    round(resultOfExp);
    return -1;
} 
 /*Esta funcion evalua las expresiones ingresadas por el user*/
int evaluateInFija(char * infija,char * resultOfExp){
    char postfija [50];
    int result = infijaToPostFija(infija,postfija);
    if(result != -1)    
        return result;
    return evaluatePostfija(postfija,resultOfExp);
}
/*Esta funcion recibe un string y lo pasa a entero, chequeando 
si hay punto o no. Devuelve la cantidad de veces que se corrió el punto de lugar*/

int transformNumber(char * number, long * transformedNumber){
    long aux=0;
    int despl=0;
    int hasPoint=0;
    int isNegative=0;
    for(int i=0;number[i]!=0 && number[i]!=' ';i++){
        if(number[i]=='-')
            isNegative = !isNegative;
        else if(number[i]!='.'){
            aux *=10;
            aux+=number[i] - '0';
            if(hasPoint)
                despl++; /*entro a este if con los nros q vienen dsp del punto, me guardo cuantos son asi dsp
                        arreglo el corrimiento*/
        }
        else
            hasPoint = 1;
    }
    if(isNegative)
        aux = -1 * aux;
    *transformedNumber=aux;
    return despl;
}
long pot (long base, int exp){
    long ret = 1;
    while(exp>0){
        ret *= base;
        exp--;
    }
    return ret;
}
int getCifras(long num){
    int rta=0;
    if(num <0){
        num = -1 * num;
    }
    if(num==0)
        return 1;
    while(num>0){
        rta++;
        num /= 10;
    }
    return rta;
}
void longToStringWithPoint(long num, int cifras, int despl, char * buffer){
    int posPunto = cifras - despl;
    int i =0;
    int agregueMenos =0;
    int j =0; //para poner el 0 al final
    //esto seria para el 0.xxxx
    if(num<0)
    {
        buffer[i++]='-';
        num = -1 * num;
        j++;
        agregueMenos=1;
    }
    posPunto += agregueMenos;

    if(posPunto<=0 + agregueMenos&&despl>0){ /* aca hago eso de 0 + agregueMenos pq si no un nro tipo -0.25 lo imprimira -.25
                                                ya q no entraria a este if*/
        buffer[i++]='0';
        buffer[i++]='.';
        posPunto++;
        j+=2;
        while(posPunto<=0+ agregueMenos)
        {
            buffer[i++]='0';
            posPunto++;
            j++;
        }
         //ahora pongo el resto de los numeros
        //me posiciono al final
        i += cifras-1;
        while(cifras>0){
            buffer[i--] = (num%10) + '0';
            num /=10;
            j++;
            cifras--;
    }
    }
    else if(posPunto>0 && despl>0){
        i+=cifras;
        buffer[posPunto]='.';
        j=cifras+1+agregueMenos;
        while(cifras>0){
            if(i!=posPunto){
                buffer[i] = (num%10) + '0';
                num /=10;
                cifras--;      
            }
            i--;    
        }
    }
    else if (despl==0){
        //para numeros sin punto
        i+=cifras-1 ;
        while(cifras>0){
            buffer[i--]=(num%10) + '0';
            num/=10;
            cifras--;
            j++;
        }
    }
    
    buffer[j]=0;
}

int suma (char * a, char *b, char * result){
    long n1, n2;
    long rta;
    int despl1 = transformNumber(a,&n1);
    int despl2 = transformNumber(b,&n2);
    n1 *= pot(10,despl2);
    n2 *= pot(10,despl1);
    rta = n1 + n2;
    int despl=despl1 + despl2;
    int cifras = getCifras(rta);
    longToStringWithPoint(rta,cifras,despl,result);
   
    return 1;

}
int resta(char * a, char * b, char * result){
    char aux [15];
    int i=0;
    aux[0] = '-';
    while(b[i]!=0){
        aux[i+1] = b[i];
        i++;
    }
    aux[i+1]=0;
    suma(a,aux,result);
    return 1 ;

}
int multiplicacion (char * a, char *b, char * result){
    long n1, n2;
    long rta;
    int despl1 = transformNumber(a,&n1);
    int despl2 = transformNumber(b,&n2);
    rta = n1 * n2;
    int despl=despl1 + despl2;
    int cifras = getCifras(rta);
   longToStringWithPoint(rta,cifras,despl,result);
   return 1;
}
int division (char * a, char *b, char * result){
      long n1, n2;
    long rta;
    int despl1 = transformNumber(a,&n1);
    int despl2 = transformNumber(b,&n2);
    rta = n1 * pot(10,despl2);
    rta *= pot(10,5); //esto lo hago para asegurarme la cantidad de decimales
    if(n2==0)
         return 0;

    rta /=  n2;
    int despl=despl1 +5;// el 5 es pq multiplique por pot(10,5)
    int cifras = getCifras(rta);
    longToStringWithPoint(rta,cifras,despl,result);
    return 1;
}
int getPrecedence(char tope, char currentElem) {

    //Lo ideal seria un hash
    int topIdx;
    int currentIdx;
    switch (tope)
    {
        case '+' :
            topIdx=ADD_INDEX;
            break;
        
        case '-':
            topIdx=SUB_INDEX;
            break;
        case '*':
            topIdx=MUL_INDEX;
            break;
        case '/' :
            topIdx=DIV_INDEX;
            break;
        case '(':
            topIdx=OPEN_BRACKET;
            break;

    }
    switch (currentElem)
    {
      case '+' :
        currentIdx=ADD_INDEX;
        break;
    
        case '-':
            currentIdx=SUB_INDEX;
            break;
        case '*':
            currentIdx=MUL_INDEX;
            break;
        case '/' :
            currentIdx=DIV_INDEX;
            break;
        case '(':
            currentIdx=OPEN_BRACKET;
            break;
        case ')':
            currentIdx=CLOSE_BRACKET;
            break;
    }
    return precedenceMatrix[topIdx][currentIdx];
}
int isOperator(char c)
{
    return c=='+' || c=='-' || c=='/' || c=='*';
}

int infijaToPostFija(char * infija,char * postfija)
{
    int i=0;
    int j=0;
    //Si la dimension del stack no es 0, lo vacio.
    while(size2()!=0)
        pop2();
    while( infija[i]!=0)
    {
        if(infija[i] != ' ') {
            
            if(!(isOperator(infija[i]) || isNumber(infija[i]) || infija[i] == '.' || infija[i]=='('|| infija[i]==')'))
                return 0;
            
            if(infija[i]=='(') {
                push2('(');
            } else if(infija[i]==')') {
                while(size2() > 0 && getPrecedence(peek2(),')'))
                {
                    //lo q popeo es char
                    postfija[j++] = pop2();
                    postfija[j++] =' ';
                }
                if(peek2()=='(')
                    pop2();
                else {
                    //ERROR, falta un (
                    return 1;
                }
            } else if((isOperator(infija[i]) && infija[i] != '-') || (infija[i]=='-' && infija[i+1]==' ') ) {
                if(size2()==0) {
                    push2(infija[i]);
                } else {
                    if(!getPrecedence(peek2(),infija[i]))
                        push2(infija[i]);
                    else{
                        while(size2()!=0 && getPrecedence(peek2(),infija[i])) {
                            postfija[j++] = pop2();
                            postfija[j++] =' ';
                        }
                        push2(infija[i]);
                    }
                }
            } else {
                if(infija[i] == '-') {
                    int flag=0;
                   
                    if(!(isNumber(infija[i+1]))) 
                        return 2; //puso un menos y dsp no hay nro
                    postfija[j++]=infija[i++];
                    while(isNumber(infija[i])){
                        postfija[j++]=infija[i++];
                        flag=1;
                    }
                    //Habra un punto?? quien sabe, habra que chequear
                    if(infija[i] == '.' &&flag) {
                        //si, tenia un punto
                        int auxPosInicioPunto = i;
                        postfija[j++] = infija[i++];

                        while(isNumber(infija[i]))
                            postfija[j++]=infija[i++];
                        if(!isNumber(infija[auxPosInicioPunto+1])) //no hubo nro dsp del punto
                            return 3;
                        if(infija[i]=='.') //error, esta encadenando
                            return 4;   
                    }
                     else if (infija[i]=='.'&&!flag)
                    {
                        return 5;
                    }
    
                } else {
                    
                    int flag = 0;
                    while(isNumber(infija[i])){
                        postfija[j++]=infija[i++];
                        flag=1;
                    }
                    //Habra un punto?? quien sabe, habra que chequear
                    if(infija[i] == '.' &&flag) {
                        //si, tenia un punto
                        int auxPosInicioPunto = i;
                        postfija[j++] = infija[i++];

                        while(isNumber(infija[i]))
                            postfija[j++]=infija[i++];
                        if(!isNumber(infija[auxPosInicioPunto+1])) //no hubo nro dsp del punto
                            return 3;
                        if(infija[i]=='.') //error, esta encadenando
                            return 4;

                    }  
                    else if (infija[i]=='.'&&!flag)
                    {
                        return 5;
                    }
    
                    
                }
                postfija[j++] = ' ';
                i--; //Por el i++ que hizo de mas      
            }   
        }
        i++;   
    }
    infija[i] = 0;

    //termino, popeo lo q quedó en el stack
    while(size2()>0){
        char aux = pop2();
        if(aux=='(')
            return 9;//ERROR; FALTA UN )
        postfija[j++]= aux;
        postfija[j++]=' ';
    }
    postfija[j]=0; //end of string

    return -1; //ok

    /*Error del tipo que hayan mas operadores que operandos o al reves, se detecta en etapa
    posterior a calcular el resultado*/
}
