#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cdma.c"
#include <stdbool.h>
#include <pthread.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "172.19.235.128"

bool server_responded;
bool client_responded[8];
char server_side[512];
char client_side[512];
struct CDMA* cdma;
int server_fd, new_socket;
int subserver_socket;
struct sockaddr_in server_address;
struct sockaddr_in address;
int addrlen = sizeof(address);
struct in_addr ip_addr;

void client_server(int id)
{
    client_responded[id] = true;
    // int new_socket;
    // int server_fd, new_socket;
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        printf("%d",addrlen);
        printf("The source IP address is %s\n", inet_ntoa(ip_addr));
        // printf(inet_ntoa(address));
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    char buffer[MAX_BUFFER_SIZE] = {0};
    read(new_socket, buffer, MAX_BUFFER_SIZE);
    printf("Message from client: %s\n", buffer);
    //assuming maths 
    while(server_responded==false)
    {
        ;
    }
    client_responded[id] = false;
    //now we send Q to the clients   
    send(new_socket, server_side, strlen(server_side), 0);
    //we recieve response from clients and respectively set the client_responded[id] to true
    char response[MAX_BUFFER_SIZE] = {0};
    recv(new_socket, response, 512, 0);
    int data[2] = {0,0};
    if(response=="2")
    {data[0] = 1;}
    else if(response=="3")
    {data[1] = 1; }
    else if(response=="4")
    {data[0] = 1; data[1]=1;}
    Encode(cdma, data, 8, id);
    client_responded[id] = true;
}

int main(){
    server_responded = false;
    
    for(int i=0;i<512;i++)
    {
        client_side[i] = 0;
    }

    if ((subserver_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

     if (connect(subserver_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server \n");

    
    
    ip_addr.s_addr = address.sin_addr.s_addr;
    printf("The source IP address is %s\n", inet_ntoa(ip_addr));

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

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
    setUp(cdma, 8);
    while(1){
    pthread_t client_servers[8];
    int nos[8] = {1, 2, 3, 4, 5, 6, 7, 8};

    for(int i = 0; i < 8; i++) {
        pthread_create(&client_servers[i], NULL, (void *)&client_server, (void *)&nos[i]);
    }
    for(int i=0;i<512;i++)
    server_side[i] = 0;

    int val = recv(subserver_socket, server_side, MAX_BUFFER_SIZE,0);
    server_responded = true;

    //Server has sent the Q and it is set in server_side
    bool all_clients_responded = true;
    
    for(int i=0;i<8;i++)
    {
        if(client_responded[i]==false)
        all_clients_responded = false;
    }
    while(all_clients_responded == false)
    {
        for(int i=0;i<8;i++)
        {
            if(client_responded[i]==false)
            all_clients_responded = false;
        }
    }
    //send the cdma encoded bitstring to server
    send(subserver_socket, cdma->response, sizeof(cdma->response), 0);
    }

}