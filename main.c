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
    if(locctr<100){
        ans[0]=ans[2];
        ans[1]=ans[3];
        ans[2]='\0';
    }
    return (char *)ans;
}
int hexadecimalToDecimal(char hexVal[]) 
{    
    int len = strlen(hexVal); 
      
    // Initializing base value to 1, i.e 16^0 
    int base = 1; 
      
    int dec_val = 0; 
      
    // Extracting characters as digits from last character 
    for (int i=len-1; i>=0; i--) 
    {    
        // if character lies in '0'-'9', converting  
        // it to integral 0-9 by subtracting 48 from 
        // ASCII value. 
        if (hexVal[i]>='0' && hexVal[i]<='9') 
        { 
            dec_val += (hexVal[i] - 48)*base; 
                  
            // incrementing base by power 
            base = base * 16; 
        } 
  
        // if character lies in 'A'-'F' , converting  
        // it to integral 10 - 15 by subtracting 55  
        // from ASCII value 
        else if (hexVal[i]>='A' && hexVal[i]<='F') 
        { 
            dec_val += (hexVal[i] - 55)*base; 
          
            // incrementing base by power 
            base = base*16; 
        } 
    } 
      
    return dec_val; 
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
    int locctr,first_address,last_address, prog_length, symtab_index, symtab_length, total_lines, listing_line=0,i,j,k;
    int starting_address_of_block,ending_address_of_block,block_length;
    char *header1,*header2,*header3,*prefix,*first_addr,*object_code,*final_address,*block_length_in_hex;
    FILE *fpw,*fpi,*fpr,*fpo;
    total_lines = 0;
    symtab_index = 0;

    // Reading of input file
	struct IPCODE ipcode = Read_IP_Code();
    // for(int i=0;i<15;i++)
    // printf("%s\n",ipcode.lines[i][1]);

    // Initialisation of LOCCCTR
    i=0;
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
    ipcode.lines[0][0][strlen(ipcode.lines[0][0])-1]='\0';
    memcpy(symtab[symtab_index].label, ipcode.lines[0][0], sizeof(ipcode.lines[0][0]));
    ipcode.lines[0][0][strlen(ipcode.lines[0][0])]=':';
    symtab[symtab_index].addr = locctr;
    symtab_index++;
    print(symtab, symtab_index);
    
    struct OPTAB optab = Read_OpCodes();
    // for(i=1;i<=6;i++){
    //     printf("%s\n",optab.code[i-1][0]);
    // }
    i=1;

    while(strcmp(ipcode.lines[i][1], "END")!=0){
        total_lines++;
        bool writeToSymTabFile = true;
        char *st=ToHex(locctr);
        // printf("%s %s %s %s\n",st,ipcode.lines[i][0],ipcode.lines[i][1],ipcode.lines[i][2]);
        if(strcmp(ipcode.lines[i][1],"WORD")==0 || strcmp(ipcode.lines[i][1],"RESW")==0 || strcmp(ipcode.lines[i][1],"BYTE")==0 || strcmp(ipcode.lines[i][1],"RESB")==0){
            ipcode.lines[i][0][strlen(ipcode.lines[i][0])-1]='\0';
            // printf("%s\n",ipcode.lines[i][0]);
            fprintf(fpi,"%s %s %s %s\n",st,ipcode.lines[i][0],ipcode.lines[i][1],ipcode.lines[i][2]);
            ipcode.lines[i][0][strlen(ipcode.lines[i][0])]=':';
        }
        else{
            fprintf(fpi,"%s %s %s %s\n",st,ipcode.lines[i][0],ipcode.lines[i][1],ipcode.lines[i][2]); //writing in input file
        }

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
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])-1]='\0';
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
                memcpy(symtab[symtab_index].label, ipcode.lines[i][0], sizeof(ipcode.lines[i][0]));
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])]=':';
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
            if(writeToSymTabFile) {
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])-1]='\0';
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])]=':';
            }
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
            if(writeToSymTabFile){
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])-1]='\0';
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])]=':';
            }
            locctr+=3;
            writeToSymTabFile = false;
        }
        else if(strcmp(ipcode.lines[i][1],"RESW")==0){             // opcode=resw
            if(writeToSymTabFile){
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])-1]='\0';
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])]=':';
            }
            int x=atoi(ipcode.lines[i][2]);
            locctr+=3*x;
            writeToSymTabFile = false;
        }
        else if(strcmp(ipcode.lines[i][1],"RESB")==0){             // opcode=resb
            if(writeToSymTabFile){
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])-1]='\0';
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
                ipcode.lines[i][0][strlen(ipcode.lines[i][0])]=':';
            }
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
    fprintf(fpi,"%s\n",ipcode.lines[i][1]);
    symtab_length = symtab_index;
    last_address = locctr;
    prog_length = last_address - first_address;
    fclose(fpi);
    struct INTMD intmdFile[MAX_IP_Lines];
    Read_INTMD_File(intmdFile, ipcode, total_lines);
    // for(i=0;i<=total_lines+1;i++){
    //     printf("%d %s\n",i,intmdFile[i].label);
    // }
    //Pass 2 algorithm
    fpo=fopen("object.txt","w");
    first_addr=(char*)malloc(6);
    prefix=(char*)malloc(2);
    if(strcmp(intmdFile[0].mnemonic,"START")==0){     //writing the header part of object file
        listing_line=1;
        char* h="H";
        
        header1=(char*)malloc(strlen(intmdFile[0].label)+1);
        strcpy(header1,h);
        strcat(header1,intmdFile[0].label);
        int y=strlen(header1);
        header1[y-1]='\0';
        // printf("%s\n",header1);

        first_addr=ToHex(first_address);
        prefix="00";
        header2=(char*)malloc(strlen(first_addr)+strlen(prefix));
        strcpy(header2,prefix);
        strcat(header2,first_addr);
        // printf("%s\n",header2);

        char* second_addr=ToHex(prog_length);
        header3=(char*)malloc(6);
        strcpy(header3,prefix);
        strcat(header3,prefix);
        strcat(header3,second_addr);
        // printf("header3=%s\n",header3);

        
        fprintf(fpo,"%s %s%s\n",header1,header2,header3);
    }
    
    //Text record
    
    char **text_record=(char**)malloc(100*sizeof(char*));
    for(i=0;i<100;i++){
        text_record[i]=(char*)malloc(70);
    }
    strcpy(first_addr,header2);
    i=1;
    object_code=(char*)malloc(6);
    final_address=(char*)malloc(4);
    k=0;
    while(strcmp(intmdFile[i].addr,"END")!=0){          //Read the intermediate file
        // printf("***********start**********\n");
        // text_record[k]=(char*)malloc(70);
        text_record[k][0]='T';
        j=0;
        strcat(text_record[k],prefix);
        strcat(text_record[k],intmdFile[i].addr);
        j+=6;
        j++;
        text_record[k][j]='F';
        j++;
        text_record[k][j]='F';
        j++;
        text_record[k][j]='\0';
        strcpy(first_addr,intmdFile[i].addr);
        starting_address_of_block=hexadecimalToDecimal(intmdFile[i].addr);
        
        while(i==1 || (strcmp(intmdFile[i].addr,"END")!=0 && j-6<70)){
            bool opcode_found=false;
            int x=0;
            
            if(strcmp(intmdFile[i].mnemonic,"RESW")==0 || strcmp(intmdFile[i].mnemonic,"RESB")==0){
                i++;
                //if a new subroutine starts
                if(intmdFile[i].label[strlen(intmdFile[i].label)-1]==':'){
                    memcpy(final_address,intmdFile[i].addr,sizeof(intmdFile[i].addr));
                    ending_address_of_block=hexadecimalToDecimal(final_address);
                    block_length=ending_address_of_block-starting_address_of_block;
                    block_length_in_hex=ToHex(block_length);
                    text_record[k][7]=block_length_in_hex[0];
                    text_record[k][8]=block_length_in_hex[1];
                    break;
                }
                continue;
            }

            // if opcode=BYTE or WORD
            else if(strcmp(intmdFile[i].mnemonic,"WORD")==0 || strcmp(intmdFile[i].mnemonic,"BYTE")==0){
                if(strcmp(intmdFile[i].mnemonic,"BYTE")==0){
                    if(intmdFile[i].operand[0]=='C'){
                        char *first_byte=ToHex((int)intmdFile[i].operand[2]);
                        char *second_byte=ToHex((int)intmdFile[i].operand[3]);
                        char *third_byte=ToHex((int)intmdFile[i].operand[4]);
                        object_code=first_byte;
                        strcat(object_code,second_byte);
                        strcat(object_code,third_byte);
                    }
                    else if(intmdFile[i].operand[0]=='X'){
                        object_code[0]=intmdFile[i].operand[2];
                        object_code[1]=intmdFile[i].operand[3];
                    }
                }
                else if(strcmp(intmdFile[i].mnemonic,"WORD")==0){
                    int cnst = atoi(intmdFile[i].operand);
                    char *c=ToHex(cnst);
                    // printf("%s\n",c);
                    strcpy(object_code,prefix);
                    // printf("%d\n",strlen(object_code));
                    if(strlen(object_code)==2){
                        strcat(object_code,prefix);
                    }
                    // printf("%s\n",object_code);
                    strcat(object_code,c);
                }
                strcat(text_record[k],object_code);
                i++;
                //if a new subroutine starts
                if(intmdFile[i].label[strlen(intmdFile[i].label)-1]==':'){
                    memcpy(final_address,intmdFile[i].addr,sizeof(intmdFile[i].addr));
                    ending_address_of_block=hexadecimalToDecimal(final_address);
                    block_length=ending_address_of_block-starting_address_of_block;
                    block_length_in_hex=ToHex(block_length);
                    text_record[k][7]=block_length_in_hex[0];
                    text_record[k][8]=block_length_in_hex[1];
                    break;
                }
                continue;
            }

            while(strcmp(optab.code[x][0],"END")!=0){           // search optab for opcode
                // printf("%d %s %s %s\n",x,intmdFile[i].mnemonic,optab.code[x][3],optab.code[x][0]);
                if(strcmp(intmdFile[i].mnemonic,optab.code[x][3])==0){
                    opcode_found=true;
                }
                if(opcode_found){            //if opcode is found
                    strcpy(object_code,optab.code[x][0]);
                    // printf("object_code=%s %s\n",object_code,optab.code[x][0]);
                    bool label_found=false;
                    int y;
                    for(y=0;y<symtab_index;y++){      // search for operand in symtab
                        if(strcmp(intmdFile[i].operand,symtab[y].label)==0){
                            label_found=true;
                        }
                        if(label_found){      // if operand is found
                            char* c=ToHex(symtab[y].addr);
                            strcat(object_code,c);
                            opcode_found=false;
                            x=0;
                            break;
                        }
                    }
                    if(y==symtab_index){         // if operand is not found in symtab
                        printf("Error. Undefined symbol\n");
                    }
                    else{
                        break;
                    }
                }
                x++;
            }
            strcat(text_record[k],object_code);
            // printf("%s\n",t);
            j+=strlen(object_code);
            j++;
            i++;
            //if a new subroutine starts
            if(intmdFile[i].label[strlen(intmdFile[i].label)-1]==':'){
                memcpy(final_address,intmdFile[i].addr,sizeof(intmdFile[i].addr));
                ending_address_of_block=hexadecimalToDecimal(final_address);
                block_length=ending_address_of_block-starting_address_of_block;
                block_length_in_hex=ToHex(block_length);
                text_record[k][7]=block_length_in_hex[0];
                text_record[k][8]=block_length_in_hex[1];
                break;
            }
        }
        // fprintf(fpo,"%s\n",t);
        k++;
    }
    //for last text record
    final_address=ToHex(last_address);
    ending_address_of_block=hexadecimalToDecimal(final_address);
    block_length=ending_address_of_block-starting_address_of_block;
    block_length_in_hex=ToHex(block_length);
    text_record[k-1][7]=block_length_in_hex[0];
    text_record[k-1][8]=block_length_in_hex[1];
    for(i=0;i<k;i++){
        fprintf(fpo,"%s\n",text_record[i]);
    }
    // for end record
    char *end_record=(char*)malloc(8);
    end_record[0]='E';
    strcat(end_record,header2);
    fprintf(fpo,"%s\n",end_record);
    return 0;
}