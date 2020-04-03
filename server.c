/*
    This Program is Create by Ishpreet Singh
    Student Id: 105175335
    Course COMP: 8567
*/

/*
    server.c -> demonstrats the use of server while creating a simple client server game
    Server have the following functionalities:

        - accepts two clients for a connection
        - it forks and calls a service servicePlayers which act as a refree and goes back to wait for next two clients
        - sends a message to first player and recieve its score
        - sends a message to second player and recieve its score
        - repeats the previous two steps until one or both of them reaches a target(100)
        - assign a winner based on score obtained, sends the winning/loosing msg to client and closes the socket

    Syntax to run:
        - gcc server.c -o server
        - ./server <PORT_NUMBER>
*/
#include <stdio.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

/*
    A function that takes two socket decsriptor(sd1, sd2) and serves as a refree between two playerss.
    @sd1 : First File Descriptor     
    @sd2 : Second File Descriptor
*/
void servicePlayers(int sd1, int sd2)
{

    char message[255], response1[255], response2[255], firstPlayer[255], secondPlayer[255];
    int target = 100;

    // Getting First Player Name
    if (!read(sd1, response1, 255))
    {
        close(sd1);
        exit(0);
    }
    strcpy(firstPlayer, response1);

    // Getting Second Player Name
    if (!read(sd2, response2, 255))
    {
        close(sd2);
        exit(0);
    }
    strcpy(secondPlayer, response2);

    fprintf(stdout, "\n--------------------------------------\n");
    fprintf(stdout, "Two players Joined: %s %s\n", firstPlayer, secondPlayer);
    fprintf(stdout, "--------------------------------------\n\n");


    while (1)
    {
        int firstPlayerTotal = 0, secondPlayerTotal = 0;

        // Playing till both player's total is less than target
        while (firstPlayerTotal < target && secondPlayerTotal < target)
        {
            memset(message, 0, sizeof message);

            // 1. First player playing
            strcpy(message, "You can now play");

            fprintf(stdout, "Currently Playing: %s\n", firstPlayer);
            write(sd1, message, strlen(message) + 1);

            // 2. Reading response from first player
            if (!read(sd1, response1, 255))
            {
                close(sd1);
                exit(0);
            }

            // Accumating score for first player
            firstPlayerTotal += atoi(response1);
            fprintf(stdout, "%s send: %d\n", firstPlayer, atoi(response1));
            fprintf(stdout, "%s's Current Total: %d\n", firstPlayer, firstPlayerTotal);

            // Breaking if first player reaches target
            if (firstPlayerTotal >= target)
            {
                break;
            }

            sleep(1);
            fprintf(stdout, "\n------------------------------------------\n\n");

            // 3. Second player playing
            fprintf(stdout, "Currently Playing: %s\n", secondPlayer);
            write(sd2, message, strlen(message) + 1);
            
            // 4. Reading response from second player
            if (!read(sd2, response2, 255))
            {
                close(sd2);
                exit(0);
            }

            // Accumating score for second player
            secondPlayerTotal += atoi(response2);
            fprintf(stdout, "%s send: %d\n", secondPlayer, atoi(response2));
            fprintf(stdout, "%s's Current Total: %d\n", secondPlayer, secondPlayerTotal);

            sleep(1);
            fprintf(stdout, "\n------------------------------------------\n\n");
        }

        // 5. First player wins
        if (firstPlayerTotal >= target)
        {

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you won the game");
            fprintf(stdout, "\n------------------------------------------\n");
            fprintf(stdout, "Game over: %s won the game\n", firstPlayer);
            fprintf(stdout, "------------------------------------------\n\n");
            // Sending Winning msg to first player
            write(sd1, message, strlen(message) + 1);
            // Closing the socket
            close(sd1);

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you lost the game");
            // Sending Lossing msg to second player
            write(sd2, message, strlen(message) + 1);
            // Closing the socket
            close(sd2);

            fprintf(stdout, "Waiting for players to Join\n");
            exit(0);
        }

        // 5. Second player wins
        else if (secondPlayerTotal >= target)
        {

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you won the game");
            fprintf(stdout, "\n------------------------------------------\n");
            fprintf(stdout, "Game over: %s won the game\n", secondPlayer);
            fprintf(stdout, "------------------------------------------\n\n");
            // Sending Winning msg to second player
            write(sd2, message, strlen(message) + 1);
            // Closing the socket
            close(sd2);

            memset(message, 0, sizeof message);
            strcpy(message, "Game over: you lost the game");
            // Sending Lossing msg to first player
            write(sd1, message, strlen(message) + 1);
            // Closing the socket
            close(sd1);

            fprintf(stdout, "Waiting for players to Join\n");
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
    // Creating endpoint for Server
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf(argv[1], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);

    // Binding the socket address
    bind(sd, (struct sockaddr *)&servAdd, sizeof(servAdd));

    // Specifying the Queue
    listen(sd, 5);

    fprintf(stdout, "\nWaiting for players to Join!\n");

    while (1)
    {
        // Waiting for clients to join
        client1 = accept(sd, (struct sockaddr *)NULL, NULL);
        client2 = accept(sd, (struct sockaddr *)NULL, NULL);

        if (!fork())
        {
            // Starting the Game
            servicePlayers(client1, client2);
        }

        // Closing the client
        close(client1);
        close(client2);
    }
}
