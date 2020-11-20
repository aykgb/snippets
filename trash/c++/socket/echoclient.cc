#include <iostream>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

using namespace std;

#define BUFFSIZE 32
void Die(const char* msg) { perror(msg); exit(1); }

int main(int argc, char* argv[])
{
    int sock;
    struct sockaddr_in echoserver;
    char buffer[BUFFSIZE];
    unsigned int echolen;
    unsigned int received = 0;

    if (argc != 4)
    {
        fprintf(stderr, "USAGE: TCPecho <server ip> <word> <port>\n");
        exit(1);
    }

    /* Create the TCP socket */
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        Die("Failed to create socket");
    }

    /* Construct the server sockaddr_in structure */
    memset(&echoserver, 0, sizeof(echoserver));
    echoserver.sin_family = AF_INET;                // IPv4 Protocol
    echoserver.sin_addr.s_addr = inet_addr(argv[1]);// IP Address
    echoserver.sin_port = htons(atoi(argv[3]));     // Server Port

    /* Establish connection */
    if(connect(sock, (struct sockaddr* )&echoserver, sizeof(echoserver)) < 0)
    {
        Die("Failed to connect with server.");
    }

    /* Send the word to the server */
    echolen = strlen(argv[2]);
    if(send(sock, argv[2], echolen, 0) != echolen)
    {
        Die("Missmatch in number of sent bytes. ");
    }

   /* Receive the word back from the server */
    fprintf(stdout, " Reveived.");
    while(received < echolen)
    {
        int bytes = 0;
        if((bytes = recv(sock, buffer, BUFFSIZE - 1, 0)) < 1)
        {
            Die("Failed to receive bytes from server. ");
        }
         received += bytes;
         buffer[bytes] = '\0';
         fprintf(stderr, buffer);
    }

    fprintf(stdout, "\n");
    close(sock);
    exit(0);
}
