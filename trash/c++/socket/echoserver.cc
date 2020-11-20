#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

using namespace std;

#define MAXPENDING 5 /* Max connection requests */
#define BUFFSIZE 32
void Die(const char* msg) { perror(msg); exit(1); }

void handle_client(int sock)
{
    char buffer[BUFFSIZE];
    int received = -1;

    /* Reveive message */
    if((received = recv(sock, buffer, BUFFSIZE, 0)) < 0)
    {
        Die("Failed to receive initial bytes from client. ");
    }

    /* Send bytes and check more incoming data in loop. */
    while(received > 0)
    {
        /* Send back received data. */
        if(send(sock, buffer, received, 0) != received)
        {
            Die("Failed to send bytes to client. ");
        }

        /* Check for more data. */
        if((received = recv(sock, buffer, BUFFSIZE, 0)) < 0)
        {
            Die("Failed to receive additional byres from client. ");
        }
    }

    close(sock);
}

int main(int argc, char* argv[])
{
    int serversock, clientsock;
    struct sockaddr_in echoserver, echoclient;

    if(argc != 2)
    {
        fprintf(stderr, "USAGE: echoserver <port>\n");
        exit(1);
    }

    /* Create the TCP socket */
    if((serversock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        Die("Failed to create socket. ");
    }
    /* Construct the server sockaddr_in structure. */
    memset(&echoserver, 0, sizeof(echoserver));
    echoserver.sin_family = AF_INET;
    echoserver.sin_addr.s_addr = htonl(INADDR_ANY);
    echoserver.sin_port = htons(atoi(argv[1]));

    /* Bind the server socket. */
    if(bind(serversock, (struct sockaddr*)&echoserver, sizeof(echoserver)) < 0)
    {
        Die("Failed to bind the server socket.");
    }

    /* Listen on the server socket. */
    if(listen(serversock, MAXPENDING) < 0)
    {
        Die("Failed to listen to server socket. ");
    }

    /* Run until cancelled. */
    while(1)
    {
        unsigned int clientlen = sizeof(echoclient);
        /* Wait for client connection. */
        if((clientsock = accept(serversock,
                                (struct sockaddr*)&echoclient,
                                &clientlen))
                < 0)
        {
            Die("Failed to accept client connection.");
        }
        fprintf(stdout, "Client connected: %s\n", inet_ntoa(echoclient.sin_addr));
        handle_client(clientsock);
    }
}
