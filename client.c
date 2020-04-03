#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char *argv[])
{
    char message[255];
    int server, portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;
    srand(time(NULL));
    if (argc != 3)
    {
        printf("Call model:%s <IP> <Port#>\n", argv[0]);
        exit(0);
    }
    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    sscanf(argv[2], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);
    if (inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0)
    {
        fprintf(stderr, "inet_pton() has failed\n");
        exit(2);
    }
    if (connect(server, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
    {
        fprintf(stderr, "connect() has failed, exiting\n");
        exit(3);
    }

    while (1)
    {
        if (read(server, message, 255) < 0)
        {
            fprintf(stderr, "read() error\n");
            exit(3);
        }
        fprintf(stdout, "Server Sent: %s\n", message);
        if (strcmp(message, "You can now play") == 0)
        {
            int dice = rand() % 6 + 1;
            fprintf(stdout, "I got %d points\n", dice);
            // Copying dice(int) to message(char *)
            sprintf(message, "%d", dice);
            write(server, message, strlen(message) + 1);
        }
        else if (strcmp(message, "Game over: you won the game") == 0)
        {
            fprintf(stderr, "I won the game");
            close(server);
            exit(0);
        }
        else if (strcmp(message, "Game over: you lost the game") == 0)
        {
            fprintf(stderr, "I lost the game");
            close(server);
            exit(0);
        }
        else
        {
            fprintf(stderr, "Some Error Occured\n");
        }
    }
}
