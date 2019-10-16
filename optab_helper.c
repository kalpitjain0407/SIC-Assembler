
struct OPTAB Read_OpCodes() {

    char regs[9][2][10];
    char codes[10][4][10];
    int i=0, j=0;
    FILE *fp;
    fp = fopen("instruction", "r");

    // Fill in the register codes
    i=0;j=0;
    do {
        if(j==2){
            i++;
            j=0;
        }
        fscanf(fp, "%s", regs[i][j]);
        j++;
    }while(strcmp(regs[i][0], "OPCODE")!=0);
    // Fill in the OpCodes
    i=0;j=0;
    do {
        if(j==4){
            i++;
            j=0;
        }
        fscanf(fp, "%s", codes[i][j]);
        j++;
    }while(strcmp(codes[i-1][0], "END")!=0);
    struct OPTAB optab;
    memcpy(optab.reg, regs, sizeof(regs)); 
    memcpy(optab.code, codes, sizeof(codes));
    return optab;
}