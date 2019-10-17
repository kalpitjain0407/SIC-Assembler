
struct OPTAB Read_OpCodes() {

    char regs[REGISTERS][2][10];
    char codes[OPCODE_SIZE][4][10];
    int i=0, j=0;
    FILE *fp;
    struct OPTAB optab;
    fp = fopen("instruction", "r");
    if(fp==NULL) {
        printf("File not Found.");
        return optab;
    }

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
    
    memcpy(optab.reg, regs, sizeof(regs)); 
    memcpy(optab.code, codes, sizeof(codes));
    
    return optab;
}


struct IPCODE Read_IP_Code() {

    char line[MAX_IP_Lines][IP_COLUMNS][100];
    int i=0, j=0, max_cols = 3;

    FILE *fp;
    struct IPCODE ipcode;
    fp = fopen("input", "r");
    if(fp==NULL) {
        printf("File not Found.");
        return ipcode;
    }
    i=0;j=0;
    do {
        if(line[i][0][strlen(line[i][0])-1] == ':') max_cols = 3;
        else max_cols = 2;
        if(j==max_cols) {
            i++;
            j=0;
        }
        fscanf(fp, "%s", line[i][j]);
        j++;        
    }while(strcmp(line[i-1][0], "END")!=0);

    for(int k=0;k<i;k++) {
        if(line[k][0][strlen(line[k][0])-1] != ':') {
            memcpy(line[k][2], line[k][1], sizeof(line[k][1]));
            memcpy(line[k][1], line[k][0], sizeof(line[k][0]));
            memset(line[k][0], 0, sizeof(line[k][0]));
        }
    }

    memcpy(ipcode.lines, line, sizeof(line)); 

    return ipcode;
}