#include "data_structure.c"
#include "optab_helper.c"
#include "Input_helper.c"

struct symtab{
    char s[100][20];
    int l; 
};

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

int main(){
    char f[100][100];
    int locctr;
    FILE *fpw;
    // Reading of input file

	struct IPCODE ipcode = Read_IP_Code();
    // for(int i=0;i<11;i++)
    // printf("%s\n",ipcode.lines[i][0]);

    // Initialisation of LOCCCTR

    int i=0;
    if(strcmp(ipcode.lines[0][1],"START")==0){
        locctr=atoi(ipcode.lines[0][2]);
    }
    else{
        locctr=0;
    }

    //Symtab created

    fpw=fopen("symtab.txt","w");
    fprintf(fpw,"%s %d\n",ipcode.lines[0][0],locctr);

    struct OPTAB optab = Read_OpCodes();
    i=1;
    
    while(i!=10){
        char *st=ToHex(locctr);
        if(strcmp(ipcode.lines[i][1],"BYTE")==0){                   // opcode=byte
            fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            int len=strlen(ipcode.lines[i][2])-3;
            if(ipcode.lines[i][2][0]=='C'){
                locctr+=len;
            }
            else{
                locctr+=(len/2);
            }
        }
        else if(strcmp(ipcode.lines[i][1],"WORD")==0){             // opcode=word
            fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            locctr+=3;
        }
        else if(strcmp(ipcode.lines[i][1],"RESW")==0){             // opcode=resw
            fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            int x=atoi(ipcode.lines[i][2]);
            locctr+=3*x;
        }
        else if(strcmp(ipcode.lines[i][1],"RESB")==0){             // opcode=resb
            fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            int x=atoi(ipcode.lines[i][2]);
            locctr+=x;
        }
        else{                                                      // opcode=something else
            if(strcmp(ipcode.lines[i][0],"")==0){
                locctr+=3;
            }
            else{
                locctr+=3;
                fprintf(fpw,"%s %s\n",ipcode.lines[i][0],st);
            }
        }
        i++;
    }
    return 0;
}