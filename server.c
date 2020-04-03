#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void servicePlayers(int sd1, int sd2)
{

    char message[255],response1[255], response2[255];
    char player1[5] = "TOTO";
    char player2[5] = "TITI";
    int target = 25;

    while (1)
    {
        int total1 = 0, total2 = 0;

        while (total1 < target && total2 < target)
        {
            memset(message, 0, sizeof message);
            strcpy(message, "You can now play");

            fprintf(stdout, "%s %s\n", message, player1);
            write(sd1, message, strlen(message) + 1);

            if (!read(sd1, response1, 255))
            {
                close(sd1);
                exit(0);
            }

            total1 += atoi(response1);
            fprintf(stdout, "%s send: %d\n", player1, atoi(response1));
            fprintf(stdout, "%s's Current Total: %d\n", player1, total1);

            if (total1 >= target)
            {
                break;
            }

            sleep(1);

            fprintf(stdout, "%s %s\n", message, player2);
            write(sd2, message, strlen(message) + 1);
            
            if (!read(sd2, response2, 255))
            {
                close(sd2);
                exit(0);
            }

            total2 += atoi(response2);
            fprintf(stdout, "%s send: %d\n", player2, atoi(response2));
            fprintf(stdout, "%s's Current Total: %d\n", player2, total2);

            sleep(1);
        }

        // Player 1 Wins
        if (total1 >= target)
        {

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you won the game");
            fprintf(stdout, "Game over: %s won the game\n", player1);
            write(sd1, message, strlen(message) + 1);
            close(sd1);

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you lost the game");
            write(sd2, message, strlen(message) + 1);
            close(sd2);
            exit(0);
        }

        // Player 2 Wins
        else if (total2 >= target)
        {

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you lost the game");
            fprintf(stdout, "Game over: %s won the game\n", player2);
            write(sd2, message, strlen(message) + 1);

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you lost the game");
            fprintf(stdout, "Game over: %s lost the game\n", player1);
            write(sd1, message, strlen(message) + 1);
            exit(0);

        }
    }
}

int main(int argc, char *argv[])
{
    int sd, client1, client2, portNumber;
    socklen_t len;
    struct sockaddr_in servAdd;
    if (argc != 2)
    {
        printf("Call model: %s <Port #>\n", argv[0]);
        exit(0);
    }
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf(argv[1], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);
    bind(sd, (struct sockaddr *)&servAdd, sizeof(servAdd));
    listen(sd, 5);

    while (1)
    {
        fprintf(stdout, "Waiting for players to Join\n");

        client1 = accept(sd, (struct sockaddr *)NULL, NULL);
        client2 = accept(sd, (struct sockaddr *)NULL, NULL);
        
        fprintf(stdout, "Two players Joined\n");

        if (!fork())
        {
            servicePlayers(client1, client2);
        }
        close(client1);
        close(client2);
    }
}
