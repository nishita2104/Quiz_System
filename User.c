#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8090
#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "172.19.235.128" // Replace with the actual IP address of the server

int main() {
    int client_socket;
    struct sockaddr_in server_address;

    // Creating socket file descriptor
    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
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

    // Connecting to the server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }
    printf("Welcome to Maths Quiz \n");

    char buffer[2][MAX_BUFFER_SIZE] = {0};
    printf("started receiving \n");
    int val = recv(client_socket, buffer, sizeof(buffer),0);
    printf("id is: %s \n", buffer[0]);
    printf("%s \n", buffer[1]);
    char answer[512];
    fgets(answer, sizeof(answer), stdin);
    answer[strlen(answer) - 1] = '\0';
    send(client_socket, answer, strlen(answer), 0);
    printf("answer sent\n");
    memset(buffer,'\0', sizeof(buffer));
    val = recv(client_socket, buffer, MAX_BUFFER_SIZE,0);
    printf("%s \n", buffer);
    close(client_socket);

    return 0;
}
