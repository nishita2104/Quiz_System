#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cdma.c"
 
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    printf("hiii-0\n");
    struct CDMA codes;
    printf("hiii-0.5\n");
    int data[] = {1,1,1,1,1,1,1,1};
    printf("hiii-0.75\n");
    setUp(&codes, data, 8);
    printf("hiii-1\n");
    // Accepting incoming connections
        // printf("start\n");
 
 
    // char buffer[MAX_BUFFER_SIZE] = {0};
    // read(new_socket, buffer, MAX_BUFFER_SIZE);
    // printf("Message from client: %s\n", buffer);
    // buffer[strlen(buffer)]='\0';
    char serverquestion[1024];
    char serverresponse[1024] ={0};
    memset(serverquestion,'\0', sizeof(serverquestion));
    memset(serverresponse,'\0', sizeof(serverresponse));
    printf("hiii-2\n");
    // char answer[512];
    // fgets(answer, sizeof(answer), stdin);
    // answer[strlen(answer) - 1] = '\0';
    FILE *outputFile;
    char character;
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
          // Close the input file
        return 1;
    }
    printf("hiii-3\n");
    // if(strcmp(buffer,"Maths")==0)
    {
            // printf("subj is maths \n");
            strcpy(serverquestion,"What is square root of 25?\n 1. 2\n 2. 3\n 3. 5\n 4. 7\n Enter 1, 2, 3 or 4\n");
            int val;
    // printf("%s\n",serverquestion);
            
            // printf("%d \n", val);
            printf("hiii-4\n");
            int int_array[4*MAX_BUFFER_SIZE / sizeof(int)]={1,-1,1,-1,1,-1,1,-1,2,0,2,0,2,0,2,0};
            char response[MAX_BUFFER_SIZE] = {0};
            // int * bitarray1 = charToBitArray(response[0]);
            // int * bitarray2 = charToBitArray(response[1]);
            // printf("%s is response \n", response);
            printf("hiii-5\n");
            for (int i = 0; i < 8; i++)
            {
                int sum1=0;
                int sum2=0;
                printf("hiii-6\n");
                for (int j = 0; j < 8; j++)
                {
                    sum1 += int_array[j]*(codes.copy[i][j]);
                    sum2 += int_array[j+8]*(codes.copy[i][j]);
                    // printf("%d\n", codes.wtable[i][j]);
                }
                sum1 = sum1/8;
                sum2 = sum2/8;
                printf("hiii-7\n");
                memset(serverresponse,'\0', sizeof(serverresponse));
                printf("%d %d\n",sum1,sum2);
                if (sum1==0&&sum2==1)
                {
                    strcpy(serverresponse,"Correct\0");
                }
                else strcpy(serverresponse,"Incorrect\0");
                printf("hiii-8\n");
                fprintf(outputFile, "%s\n", serverresponse);
            }
            
 
        }
fclose(outputFile);
    return 0;
 
}