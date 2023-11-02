#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cdma.c"
#include <stdbool.h>
#include <pthread.h>
/*fprintf(outputFile, "%s\n", serverresponse);*/
#define PORT 8080
#define PORT2 8090
#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "172.19.235.128"
#define NUMBER_OF_CONNECTIONS 4

bool server_responded;
bool client_responded[8];
char server_side[512];
char client_side[512];
struct CDMA cdma;
int server_fd, new_socket;
int subserver_socket;
struct sockaddr_in server_address;
struct sockaddr_in address;
int addrlen = sizeof(address);
struct in_addr ip_addr;

void client_server(int id)
{
    printf("waiting over accept %d \n", id);
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        printf("%d", addrlen);
        printf("The source IP address is %s\n", inet_ntoa(ip_addr));
        // printf(inet_ntoa(address));
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("connected to client no %d \n", id);
    while (server_responded == false)
    {
        ;
    }
    //client_responded[id] = false;
    // now we send Q to the clients
    char qs[2][1024];
    sprintf(qs[0],"%d",id);
    strcpy(qs[1], server_side);
    printf("Question is: %s %s\n",qs[0],qs[1]);
    send(new_socket, qs, sizeof(qs), 0);
    // we recieve response from clients and respectively set the client_responded[id] to true
    char response[MAX_BUFFER_SIZE] = {0};
    recv(new_socket, response, 512, 0);
    // printf("%s \n",response);
    int data[2] = {0, 0};
    printf("For ID: %d, response = %s\n", id, response);
    if (strcmp(response,"2")==0)
    {
        data[0] = 1;
    }
    else if (strcmp(response,"3")==0)
    {
        data[1] = 1;
    }
    else if (strcmp(response,"4")==0)
    {
        data[0] = 1;
        data[1] = 1;
    }
    printf("wtable\n");
    for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
    {
        for (int j = 0; j < NUMBER_OF_CONNECTIONS; j++)
        {
            printf("%d ",cdma.wtable[i][j]);
        }
        printf("\n");
    }
    printf("%d ", data[1]);
    printf("%d \n", data[0]);
    Encode2(&cdma, data, NUMBER_OF_CONNECTIONS, id);
    for (int i = 0; i < 4; i++)
    {
        printf("%d ", cdma.response[i]);
    }
    printf("\n %d",sizeof(cdma.response));
    printf("\n");
    client_responded[id] = true;
}

int main()
{
    for(int i=0;i<4;i++)
    client_responded[i] = false;
    server_responded = false;
    for (int i = 0; i < 512; i++)
    {
        client_side[i] = 0;
    }

    if ((subserver_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
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

    if (connect(subserver_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
    {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server \n");

    ip_addr.s_addr = address.sin_addr.s_addr;
    printf("The source IP address is %s\n", inet_ntoa(ip_addr));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT2);

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

    printf("Server listening on port %d...\n", PORT2);

    // Accepting incoming connections
    setUp(&cdma, NUMBER_OF_CONNECTIONS);
    // while (1)
    {
        pthread_t client_servers[NUMBER_OF_CONNECTIONS];

        int nos[NUMBER_OF_CONNECTIONS];
        for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
        {
            nos[i] = i;
        }

        for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
        {

            pthread_create(&client_servers[i], NULL, (void *)&client_server, (void *)nos[i]);
            printf("%d\n", i);
            printf("\n");
        }

        for (int i = 0; i < 512; i++)
            server_side[i] = 0;
        int val = recv(subserver_socket, server_side, MAX_BUFFER_SIZE, 0);
        sleep(4);
        server_responded = true;

        // Server has sent the Q and it is set in server_side
        bool all_clients_responded = true;

        for (int i = 0; i < 128; i++)
        {
            ;
        }
        sleep(4);
        for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
        {
            printf("client_responded %d is %d \n", i, client_responded[i]);
            if (client_responded[i] == false)
                all_clients_responded = false;
        }

        while (all_clients_responded == false)
        {
            all_clients_responded = true;
            for (int i = 0; i < NUMBER_OF_CONNECTIONS; i++)
            {
                if (client_responded[i] == false)
                {
                    all_clients_responded = false;
                }
            }
        }
        // send the cdma encoded bitstring to server
        printf("CDMA response is : \n");
        for (int i = 0; i < 2 * NUMBER_OF_CONNECTIONS; i++)
        {
            printf("%d ", cdma.response[i]);
        }
        
        printf("\n %d is size \n",sizeof(cdma.response));
        send(subserver_socket, cdma.response, NUMBER_OF_CONNECTIONS*sizeof(cdma.response), 0);
        printf("sent answer to server");
    }
}