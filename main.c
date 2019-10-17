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
    int locctr,first_address,last_address, prog_length, symtab_index, symtab_length, total_lines, listing_line=0;
    char *header1,*header2,*header3,*prefix;
    FILE *fpw,*fpi,*fpr,*fpo;
    total_lines = 0;
    symtab_index = 0;

    // Reading of input file
	struct IPCODE ipcode = Read_IP_Code();
    // for(int i=0;i<15;i++)
    // printf("%s\n",ipcode.lines[i][1]);

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
    fpw=fopen("symtab","w");
    fprintf(fpw,"%s %d\n",ipcode.lines[0][0],locctr);
    fpi=fopen("intermediate.txt","w");
    char* st_hex = ToHex(locctr);
    fprintf(fpi,"%s %s %s %s\n",st_hex, ipcode.lines[0][0],ipcode.lines[0][1],ipcode.lines[0][2]);

    // Enter into Symtab Data Structure
    memcpy(symtab[symtab_index].label, ipcode.lines[0][0], sizeof(ipcode.lines[0][0]));
    symtab[symtab_index].addr = locctr;
    symtab_index++;
    print(symtab, symtab_index);
    
    struct OPTAB optab = Read_OpCodes();
    i=1;

    while(strcmp(ipcode.lines[i][1], "END")!=0){
        total_lines++;
        bool writeToSymTabFile = true;
        char *st=ToHex(locctr);
        fprintf(fpi,"%s %s %s %s\n",st,ipcode.lines[i][0],ipcode.lines[i][1],ipcode.lines[i][2]); //writing in input file

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
    // printf("%s\n",ipcode.lines[i][1]);
    fprintf(fpi,"%d\n",total_lines);
    symtab_length = symtab_index;
    last_address = locctr;
    prog_length = last_address - first_address;

    struct INTMD intmdFile[MAX_IP_Lines];
    // printf("%s\n",ipcode.lines[i][1]);
    Read_INTMD_File(intmdFile, ipcode, total_lines);
    for(i=0;i<7;i++)
    printf("%s %s %s %s\n",intmdFile[i].addr,intmdFile[i].label,intmdFile[i].mnemonic,intmdFile[i].operand);
    //Pass 2 algorithm

    if(strcmp(intmdFile[0].mnemonic,"START")==0){     //writing the header part of object file
        listing_line=1;
        char* h="H";
        
        header1=(char*)malloc(strlen(intmdFile[0].label)+1);
        strcpy(header1,h);
        strcat(header1,intmdFile[0].label);
        int y=strlen(header1);
        header1[y-1]='\0';
        // printf("%s\n",header1);

        char* first_addr=ToHex(first_address);
        prefix="00";
        header2=(char*)malloc(strlen(first_addr)+strlen(prefix));
        strcpy(header2,prefix);
        strcat(header2,first_addr);
        // printf("%s\n",header2);

        char* second_addr=ToHex(prog_length);
        header3=(char*)malloc(strlen(first_addr)+strlen(prefix));
        strcpy(header3,prefix);
        strcat(header3,second_addr);
        // printf("%s\n",header3);

        fpo=fopen("object.txt","w");
        fprintf(fpo,"%s %s%s\n",header1,header2,header3);
    }
    
    //Text record
    // i=1;
    
    char **text_record=(char**)malloc(100*sizeof(char*));
    for(i=0;i<100;i++){
        text_record[i]=(char*)malloc(62);
    }
    char* start_addr=header2;
    i=1;
    // for(int j=0;j<100;j++){
    //     printf("%s %s %s %s\n",optab.code[j][0],optab.code[j][1],optab.code[j][2],optab.code[j][3]);
    // }
    int k=0,y;
    while(i<=6){
        text_record[i-1][0]='T';
        strcat(text_record[k],start_addr);
        text_record[k][7]='0';
        text_record[k][8]='0';
        int j;
        for(j=0;j<100;j++){
            if(strcmp(intmdFile[i].mnemonic,optab.code[j][3])==0){
                strcat(text_record[k],optab.code[j][0]);
                
            }
        }
        break;
    }


    return 0;
}