NETSYS CSCI 5273
ASSIGNMENT 1 -- UDP SOCKET PROGRAMMING
SUBMITTED BY -- SRISHTI KHARE
STUDENT ID -- 107080296

This programming assignment required building two programs in C, one for the client which will simply send a command and the other for a server which will send a suitable response back to the client.

I have created two sub-folders, "server" and "client", in the main folder submitted.

Following are the contents of the sub-folder "server":
- Makefile
- server.c
- foo1
- foo2
- foo3

Following are the contents of the sub-folder "client":
- Makefile
- client.c

server.c FILE CONTAINS:
- get_server    : Interface used to send file content to the client
- put_server    : Interface used to receive file content from the client
- list_server	: Interface used to list all the files present at server location and send it to client
- delete_server : Interface used to delete a file at the server location
- exit_server   : Interface used to exit server gracefully

client.c FILE CONTAINS:
- get_client    : Interface used to receive file content from the server
- put_client    : Interface used to send file content to the server
- list_client	: Interface used to list all the files present at server location
- delete_client : Interface used to delete a file at the server location
- exit_client   : Interface used to exit server gracefully


HOW TO EXECUTE:

1. Open the "server" and "client" folders simultaneously in separate windows.
2. Run the Makefile using "make clear" and "make" commands to compile the respective server.c and client.c files in their folders.
3. Once server and client executables are created, first run server using "./server PORT_NUMBER" command.
4. Then in client window, run "./client REMOTE_IP_ADDRESS PORT_NUMBER" command to run the client while the server listens.
5. Use the displayed menu for directions regarding commands to run continuously between server and client, till the exit command terminates the server gracefully.

RELIABILITY IMPLEMENTATION

I have implemented Stop-And-Wait protocol for relaibility, where the receiver sends an acknowledgement back to the sender with each packet it receives. The sender waits for a period of time (timeout, tv) for an ACK. If ACK is not receieved by the sender during the timeout period, it resends the last packet to the receiever, else, the next packet.

ENCRYPTION IMPLEMENTATION

I have implemented XOR encryption for this assignment, in a way that before each packet is transmitted, it is XOR'ed with a key(=11). The same key is then used at the receivcer end to decrypt the packet by XOR'ing with it to retrieve the original data.

