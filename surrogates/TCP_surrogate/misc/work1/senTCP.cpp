// C++ program to illustrate the client application in the 
// socket programming 
#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 
  
int main() 
{ 
// creating socket 
int clientSocket = socket(AF_INET, SOCK_STREAM, 0); 

// specifying address 
sockaddr_in serverAddress; 
serverAddress.sin_family = AF_INET; 
serverAddress.sin_port = htons(50001); 
serverAddress.sin_addr.s_addr = INADDR_ANY; 

// Forcefully attaching socket to the port 50001
int opt = 1;
setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT|SO_BROADCAST, 
&opt, 
sizeof(opt));

// sending connection request 
connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)); 
  
// sending data 
const char* message = "Hello, server!"; 
send(clientSocket, message, strlen(message), 0); 
  
// closing socket 
close(clientSocket); 
  
return 0; 
}
