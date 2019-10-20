
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

void Read_INTMD_File(struct INTMD *intmdFile, struct IPCODE ipcode, int total_lines) {
    int i=0, j;

    FILE *fp;
    fp = fopen("intermediate.txt", "r");
    if(fp==NULL) {
        printf("File not Found.");
        return;
    }
    i=0;j=0;
    do {
        char addr[5];
        char label[100];
        char mnemonic[100];
        char operand[100];
        
        fscanf(fp, "%s", addr);
        memcpy(intmdFile[j].addr, addr, sizeof(addr));

        if(strlen(ipcode.lines[j][0])>0){
            fscanf(fp, "%s", label);
            memcpy(intmdFile[j].label, label, sizeof(label));
        } else {
            memset(intmdFile[j].label, 0, sizeof(intmdFile[j].label));
            // break;
        }
        

        fscanf(fp, "%s", mnemonic);
        memcpy(intmdFile[j].mnemonic, mnemonic, sizeof(mnemonic));

        fscanf(fp, "%s", operand);        
        memcpy(intmdFile[j].operand, operand, sizeof(operand));
        j++;
    } while(j<=total_lines+1);

    return;
}