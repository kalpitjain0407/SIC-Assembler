#include "data_structure.c"
#include "reader.c"

char* ToHex(int locctr){
    char* ans=(char*)malloc(5);
    int i=3;
    int digit=((locctr/10)%10)*10+locctr%10;
    ans[0]=(locctr/1000)+'0';
    ans[1]=((locctr/100)%10)+'0';
    ans[2]=digit/16+'0';
    if(digit%16==10){
        ans[3]='A';
    }
    else if(digit%16==11){
        ans[3]='B';
    }
    else if(digit%16==12){
        ans[3]='C';
    }
    else if(digit%16==13){
        ans[3]='D';
    }
    else if(digit%16==14){
        ans[3]='E';
    }
    else if(digit%16==15){
        ans[3]='F';
    }
    else{
        ans[3]=(digit%16)+'0';
    }
    return (char *)ans;
}

void print(struct SYMTAB* symtab, int length) {
    printf("SYMTAB\n");
    for(int i=0;i<length;i++) {
        char *st=ToHex(symtab[i].addr);
        printf("%s %s\n", symtab[i].label, st);
    }
    printf("=======================================\n");
}


int main(){
    int locctr,first_address,last_address, prog_length, symtab_index, symtab_length;
    FILE *fpw;
    
    // Reading of input file
	struct IPCODE ipcode = Read_IP_Code();

    // Initialisation of LOCCCTR
    int i=0;
    if(strcmp(ipcode.lines[0][1],"START")==0){
        locctr=atoi(ipcode.lines[0][2]);
    }
    else{
        locctr=0;
    }
    first_address=locctr;

    // Symtab created
    struct SYMTAB symtab[100];
    symtab_index = 0;
    fpw=fopen("symtab","w");
    fprintf(fpw,"%s %d\n",ipcode.lines[0][0],locctr);
    memcpy(symtab[symtab_index].label, ipcode.lines[0][0], sizeof(ipcode.lines[0][0]));
    symtab[symtab_index].addr = locctr;
    symtab_index++;
    print(symtab, symtab_index);
    
    struct OPTAB optab = Read_OpCodes();
    i=1;

    while(strcmp(ipcode.lines[i][1], "END")!=0){
        bool writeToSymTabFile = true;
        char *st=ToHex(locctr);

        // If there is a symbol in Label Field
        if(strlen(ipcode.lines[i][0])>0){
            bool label_found = false;
            for(int k=0;k<symtab_index;k++) {
                if(strcmp(symtab[k].label, ipcode.lines[i][0])==0) {  // found
                    label_found = true;  
                } 
            }
            if(label_found) {
                printf("Duplicate Symbol: %s", ipcode.lines[i][0]);
                return 0;
            } else {
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
                memcpy(symtab[symtab_index].label, ipcode.lines[i][0], sizeof(ipcode.lines[i][0]));
                symtab[symtab_index].addr = locctr;
                symtab_index++;
                writeToSymTabFile = false;
            }
        }
        
        // Search OPTAB for Opcode
        bool opcode_found = false;
        for(int k=0;k<OPCODE_SIZE;k++) {
            if(strcmp(optab.code[k][3], ipcode.lines[i][1])==0) { 
                opcode_found = true;
            }
        }

        if(opcode_found) locctr += 3;                                    // Opcode found
        else if(strcmp(ipcode.lines[i][1],"BYTE")==0){                   // opcode=byte
            if(writeToSymTabFile) fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            int len=strlen(ipcode.lines[i][2])-3;
            if(ipcode.lines[i][2][0]=='C'){
                locctr+=len;
            }
            else{
                locctr+=(len/2);
            }
            writeToSymTabFile = false;
        }
        else if(strcmp(ipcode.lines[i][1],"WORD")==0){             // opcode=word
            if(writeToSymTabFile) fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            locctr+=3;
            writeToSymTabFile = false;
        }
        else if(strcmp(ipcode.lines[i][1],"RESW")==0){             // opcode=resw
            if(writeToSymTabFile) fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            int x=atoi(ipcode.lines[i][2]);
            locctr+=3*x;
            writeToSymTabFile = false;
        }
        else if(strcmp(ipcode.lines[i][1],"RESB")==0){             // opcode=resb
            if(writeToSymTabFile) fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            int x=atoi(ipcode.lines[i][2]);
            locctr+=x;
            writeToSymTabFile = false;
        }
        else{                                                      // set Error Flag
            printf("Invalid Operation Code\n");
            return 0;
        }
        print(symtab, symtab_index);
        i++;
    }
    symtab_length = symtab_index;
    last_address = locctr;
    prog_length = last_address - first_address;
        
    return 0;
}