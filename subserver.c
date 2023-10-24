#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
//#include "Server.c"
#include "cdma.c"
#define SERVER_IP "172.19.235.128"

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

struct CDMA* cdma_struct;

void client_pthread(int clientID, int server_fd)
{
    close(server_fd);
    char buffer[MAX_BUFFER_SIZE] = {0};
    read(new_socket, buffer, MAX_BUFFER_SIZE);
    printf("Message from client: %s\n", buffer);
    // buffer[strlen(buffer)]='\0';
    char serverquestion[1024];
    char serverresponse[1024] = {0};
    memset(serverquestion, '\0', sizeof(serverquestion));
    memset(serverresponse, '\0', sizeof(serverresponse));
    // char answer[512];
    // fgets(answer, sizeof(answer), stdin);
    // answer[strlen(answer) - 1] = '\0';

    if (strcmp(buffer, "Maths") == 0)
    {
        printf("subj is maths \n");
        strcpy(serverquestion, "What is square root of 4?\n");
        int val;
        // printf("%s\n",serverquestion);
        if ((val = send(new_socket, serverquestion, strlen(serverquestion), 0)) < 0)
        {
            printf("Send Failed. Error!!!!\n");
            // return -1;
        }
        else
        {
            // printf("%d", val);
            // printf("%s", serverquestion);
            printf("sent question\n");
        }
        // printf("%d \n", val);

        char response[MAX_BUFFER_SIZE] = {0};
        recv(new_socket, response, MAX_BUFFER_SIZE, 0);

        printf("%s is response \n", response);
        char correctresponse[1024];
        memset(correctresponse, '\0', sizeof(correctresponse));
        strcpy(correctresponse, "2");
        //    printf("%s jj",correctresponse);
        val = strcmp(response, correctresponse);
        //    printf("%d val", val);
        strcpy(serverresponse, "Incorrect");
        //    printf("%s", serverresponse);
        switch (val)
        {
        case 0:
        {
            //    printf("hello");
            strcpy(serverresponse, "Correct");
            break;
        }
        }

        printf("Server response %s\n", serverresponse);
        send(new_socket, serverresponse, strlen(serverresponse), 0);
    }
    // send(new_socket, serverquestion, strlen(serverquestion), 0);

    else if (strcpy(buffer, "Science") == 0)
    {
        strcpy(serverquestion, "What is called the powerhouse of a cell?\n");
        // send(new_socket, serverquestion, strlen(serverquestion), 0);
        int val;
        // printf("%s\n",serverquestion);
        if ((val = send(new_socket, serverquestion, strlen(serverquestion), 0)) < 0)
        {
            printf("Send Failed. Error!!!!\n");
            // return -1;
        }
        else
        {
            // printf("%d", val);
            // printf("%s", serverquestion);
            printf("sent question");
        }
    }

    else if (strcpy(buffer, "English") == 0)
    {
        strcpy(serverquestion, "Opposite of hot?\n");
        // send(new_socket, serverquestion, strlen(serverquestion), 0);

        int val;
        // printf("%s\n",serverquestion);
        if ((val = send(new_socket, serverquestion, strlen(serverquestion), 0)) < 0)
        {
            printf("Send Failed. Error!!!!\n");
            // return -1;
        }
        else
        {
            // printf("%d", val);
            // printf("%s", serverquestion);
            printf("sent question");
        }
        printf("%d val \n", val);
    }
    exit(0);
}
int main()
{
    pthread_t client_processes[8];
    int client_socket;
    struct sockaddr_in server_address;

    // Creating socket file descriptor
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0)
    {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connecting to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    struct in_addr ip_addr;
    ip_addr.s_addr = address.sin_addr.s_addr;
    printf("The source IP address is %s\n", inet_ntoa(ip_addr));
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding the socket to a specific address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 3) < 0)
    {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // Accepting incoming connections
    int data[] = {1,1,1};
    setUp(cdma_struct, data, 8);
    showWalshTable(cdma_struct, 8);

    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            printf("%d", addrlen);
            printf("The source IP address is %s\n", inet_ntoa(ip_addr));
            // printf(inet_ntoa(address));
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        // int childpid;
        // if ((childpid = fork()) == 0)
        int nos[8] = {1, 2, 3, 4, 5, 6, 7, 8};

        for(int i = 0; i < 8; i++) {
        pthread_create(&client_processes[i], NULL, (void *)&client_pthread, (void *)&nos[i]);
        }
        //remove fork
         for(int i = 0; i < 8; i++) {
        pthread_join(client_processes[i], NULL);
        }
        

        close(new_socket);
    }
    close(server_fd);
    return 0;
}