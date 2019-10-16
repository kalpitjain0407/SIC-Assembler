#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 1

/* Constants for OpTab */
#define OPCODE_SIZE 10
#define REGISTERS 8

struct OPTAB {
    char reg[9][2][10];
    char code[10][4][10];
};