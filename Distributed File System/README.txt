NETSYS CSCI 5273
ASSIGNMENT 3
SUBMITTED BY -- SRISHTI KHARE
STUDENT ID -- 107080296

This programming assignment required creating a distributed file system for reliable and secure file storage.

I have created two sub-folders, "server" and "client", in the main folder submitted.

Following are the contents of the sub-folder "server":
- Makefile
- dfs.c
- dfs.conf
- Files

Following are the contents of the sub-folder "client":
- Makefile
- dfc.c
- dfc.conf
- Files

dfs.c FILE CONTAINS:
- GET    : Interface used to send file content to the client
- PUT    : Interface used to receive file content from the client
- LIST	: Interface used to list all the files present at server location and send it to client
- MKDIR : Interface used to create a folder within user folder

dfc.c FILE CONTAINS:
- GET    : Interface used to receive file content from the server
- PUT    : Interface used to send file content to the server
- LIST	: Interface used to list all the files present at server location
- MKDIR : Interface used to create a folder within user folder

HOW TO EXECUTE:

1. Open the "server" and "client" folders simultaneously in separate windows.
2. Run the Makefile using "make clear" and "make" commands to compile the respective server.c and client.c files in their folders.
3. Once server and client executables are created, first run server using "./dfs /DFS1 10001" & "./dfs /DFS2 10002" & "./dfs /DFS3 10003" & "./dfs /DFS4 10004" commands.
4. Then in client window, run "./dfc dfs.conf" command to run the client.
5. Give commands in the client terminal to test code funcationality.

SUBFOLDER IMPLEMENTATION

I have implemented subfolder functionality in this assignment, as a part of which, a folder can be created inside the parent folder on the server side.

TRAFFIC OPTIMIZATION

Since parts of each file go to different servers, in the "GET" function, I am receiving each part of a file just once, hence implmenting traffic optimization.

ENCRYPTION IMPLEMENTATION

I have implemented XOR encryption for this assignment, in a way that before each packet is transmitted, it is XOR'ed with the password mentioned in the configuration file.. The same key is then used at the receivcer end to decrypt the packet by XOR'ing with it to retrieve the original data.

