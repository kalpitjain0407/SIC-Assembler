
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