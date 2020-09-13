#ifndef __EXPRESSION_MANAGER__H
#define __EXPRESSION_MANAGER__H
#include <stack.h>
#include <stack2.h>
#define MAX_EXP_SIZE 40
#define ADD_INDEX 0
#define SUB_INDEX 1
#define MUL_INDEX 2
#define DIV_INDEX 3
#define OPEN_BRACKET 4
#define CLOSE_BRACKET 5

int evaluateInFija(char * exp, char * resultOfExp);

#endif