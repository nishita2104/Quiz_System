#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cdma.c"
 
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    char serverquestion[2048];
    printf("Enter Question: \n");
    char options[4][MAX_BUFFER_SIZE];
    memset(serverquestion,'\0', sizeof(serverquestion));
            fgets(serverquestion, sizeof(serverquestion), stdin);
            
            serverquestion[strlen(serverquestion)-1] = ' ';
                for(int i=0;i<4;i++)
                {
                    fgets(options[i], sizeof(options[i]), stdin);
                    serverquestion[strlen(serverquestion)-1] = ' ';
                    serverquestion[strlen(serverquestion)] = '\n';
                    strcpy(serverquestion+(strlen(serverquestion)), options[i]);
                    printf("%d %d\n",i,strlen(serverquestion));
                }
                // serverquestion[strlen(serverquestion)-1] = ' ';
                printf("%s\n",serverquestion);
            serverquestion[strlen(serverquestion)] = '\0';
            printf("%s\n",serverquestion);
            char correct_option[MAX_BUFFER_SIZE];
            printf("Enter correct option number\n");
            fgets(correct_option, sizeof(correct_option), stdin);
            int csum1=0, csum2=0;
            if(correct_option=="2") csum1=1;
            else if(correct_option=="3") csum2=1;
            else 
            {
                csum1=1;
                csum2=1;
            }
    return 0;
 
}