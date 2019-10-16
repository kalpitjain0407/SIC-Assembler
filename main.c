#include "data_structure.c"
#include "optab_helper.c"

int main(){
    // FILE *fp;
	// fp=fopen("input.txt","r");
    struct OPTAB optab = Read_OpCodes();
    printf("%s\n", optab.code[3][3]);
    // Read_OpCodes();
    return 0;
}