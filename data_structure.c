#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 1

/* Constants for OpTab */
#define OPCODE_SIZE 100
#define REGISTERS 9

/* Constraints of Input Code */
#define MAX_IP_Lines 10
#define IP_COLUMNS 3
struct OPTAB {
    // Number of Lines, No of Columns, Size of each array
    char reg[REGISTERS][2][10];
    char code[OPCODE_SIZE][4][10];
};

struct IPCODE {
    char lines[MAX_IP_Lines][IP_COLUMNS][100];
};