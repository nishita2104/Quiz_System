# Quiz_System
## Steps to run
1. replace SERVER_IP with the IP address of the server (device)
2. replace NUMBER_OF_CONNECTIONS by the nearest power of 2 >= required number of connections
3. compile Server.c, subserver.c and User.c
4. run server file followed by subserver and ensure connection gets established
5. run the required number of clients(users)
**Note: All the devices must be under the same subnet**
## Flow
1. Server takes the question, 4 options along with the correct option as the input
2. Server sends the question to the subserver which forwards it to all the clients.
3. User sends the answer and the response is stored on server side as correct/incorrect
4. Every User is assigned an id and a CDMA code for identification.
5. The subserver receives the responses from all connected clients and sends the CDMA encoded message to the server
6. The server receives the encoded message and decodes the responses of all clients and stores the result in the output file.
