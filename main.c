#include "data_structure.c"
#include "optab_helper.c"
#include "Input_helper.c"

struct symtab{
    char s[100][20];
    int l; 
};

int main(){
    char f[100][100];
    int locctr;

    // Reading of input file

	struct IPCODE ipcode = Read_IP_Code();
    // Initialisation of LOCCCTR

    // i=0;
    // if(strcmp(f[i],"START")==0){
    //     locctr=atoi(f[i+1]);
    // }
    // else{
    //     locctr=0;
    // }
    // // printf("%d\n",locctr);

    // struct OPTAB optab = Read_OpCodes();
    // while(strcmp(f[i],"END")!=0){
    //     if(strcmp(f[i],"START")==0){
    //         i+=2;
    //         continue;
    //     }
    //     else{
            
    //     }
    // }

    return 0;
}