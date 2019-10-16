#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "data_structure.c"
#include "optab_helper.c"

struct symtab{
    char s[100][20];
    int l; 
};

int main(){
    char f[100][100];
    int i=0,j,labels[256],locctr;
    FILE *fp;

    // Reading of input file

	fp=fopen("input.txt","r");
    do{
		fscanf(fp,"%s",f[i]);
        // printf("%s\n",f[i]);
		i++; 
	}while(strcmp(f[i-1],"END")!=0);
    
    // Initialisation of LOCCTR

    i=0;
    if(strcmp(f[i],"START")==0){
        locctr=atoi(f[i+1]);
    }
    else{
        locctr=0;
    }
    // printf("%d\n",locctr);

    struct OPTAB optab = Read_OpCodes();
    while(strcmp(f[i],"END")!=0){
        if(strcmp(f[i],"START")==0){
            i+=2;
            continue;
        }
        else{
             
        }
    }

    return 0;
}