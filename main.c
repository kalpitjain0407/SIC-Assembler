#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(){
    char f[100][100];
    int i=0,j,labels[256],locctr;
    FILE *fp;

    // Reading of input file

	fp=fopen("input.txt","r");
    do{
		fscanf(fp,"%s",f[i]);
		i++; 
	}while(strcmp(f[i-1],"END")!=0);
    
    // Initialisation of LOCCCTR

    i=0;
    if(strcmp(f[i],"START")==0){
        locctr=atoi(f[i+1]);
    }
    else{
        locctr=0;
    }
    printf("%d\n",locctr);
    


    return 0;
}