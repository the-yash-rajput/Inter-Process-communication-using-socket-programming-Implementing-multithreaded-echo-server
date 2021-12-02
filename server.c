
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void *executing_function(void *);

int main()
{
    char initial_server_message[100] = "Connection message from server\n";
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
    if (server_socket==-1){
	perror("Socket initialization failed");
	exit(EXIT_FAILURE);
	}
    else
	printf("Server socket created successfully\n");

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(9999);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    //bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr))!=0){
	printf("Binding of Socket failed\n"); 
        exit(0);
	}
    else
	printf("Socket binding is successful\n"); 
     int n;
    printf("Enter how many clients to handle: ");
    scanf("%d",&n);
    if (listen(server_socket, n)!=0){
		printf("Listen Error\n"); 
        exit(0); 
    } 
    else
        printf("Server is listening\n"); 

    int no_threads=0;
   
    pthread_t threads[n];
    while (no_threads<n){
	printf("Listening on thread no: %d\n",no_threads);
	int client_socket = accept(server_socket, NULL, NULL);
	puts("Connection accepted");
	if( pthread_create( &threads[no_threads], NULL ,executing_function , &client_socket) < 0){
	perror("Thread creation error");
	return 1;}
    	if (client_socket < 0) { 
        	printf("Server does not accept client\n"); 
        	exit(0); 
    		} 
    	else
        	printf("Server accepts the client\n");
	puts("Handler assigned");
	no_threads++;
    }
    int k;
    for (k=0;k<n;k++){
	pthread_join(threads[k],NULL);
    }

    close(server_socket);
    
    return 0;
}

void *executing_function(void *client_socket){
	int socket = *(int*) client_socket;
	int read_len;
	char server_message[100]="Server has started running\n";
	int send_status;
    	send_status=send(socket, server_message, sizeof(server_message), 0);
    	char client_message[256];
    	if(send_status){
	while( (read_len=recv(socket,client_message, 256,0))>0)
	{
		if(strcmp(client_message,"exit")==0){break;}
		send_status=send(socket , client_message , strlen(client_message),0);	
	}}
	
	return 0;
}
