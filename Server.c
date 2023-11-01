#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
// #include "subserver.c"
#include "cdma.c"

#define NUMBER_OF_CONNECTIONS 4
#define PORT 8080
#define MAX_BUFFER_SIZE 1024
#define ARRAY_SIZE NUMBER_OF_CONNECTIONS*MAX_BUFFER_SIZE / (sizeof(int)*128)

// int* charToBitArray(char c) {
//     int* bitArray = (int*)malloc(8 * sizeof(int));
//     if (bitArray == NULL) {
//         perror("Memory allocation failed");
//         exit(1);
//     }

//     for (int i = 0; i < 8; i++) {
//         bitArray[i] = (c >> i) & 1;
//     }

//     return bitArray;
// }

int main() {
    struct CDMA codes;
    // int data[] = {1,1,1,1,1,1,1,1};
    setUp(&codes, NUMBER_OF_CONNECTIONS);
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct in_addr ip_addr;
    ip_addr.s_addr = address.sin_addr.s_addr;
    printf("The source IP address is %s\n", inet_ntoa(ip_addr));
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
 
    // Binding the socket to a specific address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
 
    // Listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
 
    printf("Server listening on port %d...\n", PORT);
 
    // Accepting incoming connections
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        printf("%d",addrlen);
        printf("The source IP address is %s\n", inet_ntoa(ip_addr));
        // printf(inet_ntoa(address));
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    char serverquestion[1024];
    char serverresponse[1024] ={0};
    memset(serverquestion,'\0', sizeof(serverquestion));
    memset(serverresponse,'\0', sizeof(serverresponse));
    FILE *outputFile;
    char character;
    outputFile = fopen("output.txt", "w");
    if (outputFile == NULL) {
        perror("Error opening output file");
          // Close the input file
        return 1;
    }
    char options[4][MAX_BUFFER_SIZE]={0};

    {
            printf("Enter Question: \n");
            fgets(serverquestion, sizeof(serverquestion), stdin);
            printf("Enter 4 options\n");
            serverquestion[strlen(serverquestion)-1] = ' ';
                for(int i=0;i<4;i++)
                {
                    sprintf(options[i],"%d",i+1);
                    sprintf(options[i]+1,"%c",'.');
                    sprintf(options[i]+2,"%c",' ');
                    fgets(options[i]+3, sizeof(options[i]), stdin);
                    serverquestion[strlen(serverquestion)-1] = ' ';
                    serverquestion[strlen(serverquestion)] = '\n';
                    strcpy(serverquestion+(strlen(serverquestion)), options[i]);
                }
            serverquestion[strlen(serverquestion)] = '\0';
            printf("%s\n",serverquestion);
            
            char correct_option[MAX_BUFFER_SIZE];
            printf("Enter correct option number\n");
            fgets(correct_option, sizeof(correct_option), stdin);
            correct_option[strlen(correct_option)-1]='\0';
            printf("%s\n",correct_option);
            int csum1=0, csum2=0;
            if(strcmp(correct_option,"2")==0) csum1=1;
            else if(strcmp(correct_option,"3")==0) csum2=1;
            else if(strcmp(correct_option,"4")==0)
            {
                csum1=1;
                csum2=1;
            }
            
            // strcpy(serverquestion,"What is square root of 25?\n 1. 2\n 2. 3\n 3. 5\n 4. 7\n Enter 1, 2, 3 or 4\n");
            int val;
            send(new_socket, serverquestion, strlen(serverquestion), 0);
            int int_array[ARRAY_SIZE];
            printf("size %ld \n",ARRAY_SIZE);
            recv(new_socket, int_array, sizeof(int_array) ,0);
            for(int i=0;i<ARRAY_SIZE;i++){
                printf("%d ",int_array[i]);
            }
            char response[MAX_BUFFER_SIZE] = {0};
            printf("end checking\n");
            printf("correct answers %d %d\n",csum1,csum2);
            for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
            {
                int sum1=0;
                int sum2=0;
                for (int j = 0; j < NUMBER_OF_CONNECTIONS; j++)
                {
                    sum1 += int_array[j]*(codes.wtable[i][j]);
                    sum2 += int_array[j+NUMBER_OF_CONNECTIONS]*(codes.wtable[i][j]);
                }
                sum1 = sum1/NUMBER_OF_CONNECTIONS;
                sum2 = sum2/NUMBER_OF_CONNECTIONS;
                memset(serverresponse,'\0', sizeof(serverresponse));
                printf("%d %d\n",sum2,sum1);
                if (sum1==csum1&&sum2==csum2)
                {
                    strcpy(serverresponse,"Correct\0");
                }
                else strcpy(serverresponse,"Incorrect\0");
                for (int j = 0; j < NUMBER_OF_CONNECTIONS; j++)
                {
                
                    fprintf(outputFile, "%d ",(codes.wtable[i][j]) );
                }
                fprintf(outputFile, "%s ","id:" );
                fprintf(outputFile, "%d ",i );
                fprintf(outputFile, "%s\n", serverresponse);
            }
            
 
        }
    fclose(outputFile);  
    close(new_socket);
    close(server_fd);

    return 0;
 
}