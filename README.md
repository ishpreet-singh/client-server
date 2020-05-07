# Client Server interaction using Socket Programming 
A dice game to interact between multiple clients with server using socket programming


***Project Hierarchy***
```
client-server
    |
    ---- client.c
    |
    ---- server.c
    |
    ---- README.md
    |
    ---- question.pdf
    |
    ---- LISCENSE
```

**server.c:**  Demonstrats the use of server while creating a simple client server game

Server have the following functionalities:

        - accepts two clients for a connection
        - it forks and calls a service servicePlayers which act as a refree and goes back to wait for next two clients
        - sends a message to first player and recieve its score
        - sends a message to second player and recieve its score
        - repeats the previous two steps until one or both of them reaches a target(100)
        - assign a winner based on score obtained, sends the winning/loosing msg to client and closes the socket

    Syntax to run:
        - `gcc server.c -o server`
        - `./server <PORT_NUMBER>`


**client.c:**  creates a client and tries to eastablish a connection with server (mentioned in server.c) client act as a player. 
    
Client have the following functionalities:
    
        - sends its name to server
        - sends its current dice score to server
        - reads msg from server if client has won/loss the game and closes the socket

    Syntax to run: 
        - `gcc client.c -o client`
        - `./client <SERVER_IP_ADDRESS> <PORT_NUMBER> <CLIENT_NAME>`
