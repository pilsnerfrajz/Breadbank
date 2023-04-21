#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include "breadbank.h"

/*
**  To compile: 
**  install sqlite3
**  run sudo apt-get install libsqlite3-dev
**  run make
*/

int main(void){
    struct sockaddr_in server_info = {0};
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(1337);
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);

    struct sockaddr client_info = {0};
    socklen_t server_info_len = sizeof(server_info);
    socklen_t client_info_len = sizeof(client_info);

    // Create socket
    sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0) {
        perror("socket");
        return -1;
    }

    // Bind to port
    if(0 > bind(sfd, (struct sockaddr *)&server_info, sizeof(server_info))) {
        perror("Bind");
        return -1;
    }

    while(1) {
        // Listen for clients
        if(0 > listen(sfd, 5)) {
            perror("Listen");
            return -1;
        }

        printf("Listening for clients.\n");

        // accept connection 
        cfd = accept(sfd, (struct sockaddr *)&server_info, &client_info_len);
        if(0 > cfd) {
            perror("Accept");
            return -1;
        }

        printf("Client connected.\n");

        // recieve data from client
        char client_data[256];
        if(0 > recvfrom(cfd, client_data, sizeof(client_data), 0, (struct sockaddr *)&client_info, &client_info_len)) {
            perror("Recieve");
            return -1;
        }

        printf("Received data from client:\n%s\n", client_data);

        /* Parse data from client and isolate method */
        int index = 0;
        char method[9] = "";
        char queries[256] = "";
        char lgn[6] = "LOGIN";
        char trnsfr[9] = "TRANSFER";

        while(client_data[index] != ' ') {
            method[index] = client_data[index];
            index++;
        }

        index = index + 1;
        for(int i = 0; i < sizeof(client_data); i++) {
            queries[i] = client_data[index++];
        }

        if((strcmp(method, lgn) == 0)) {
            printf("Method is LOGIN\n");
            login(queries);
        } else if((strcmp(method, trnsfr) == 0)){
            printf("Method is TRANSFER\n");
            transfer(queries);
        } else{
            printf("Bad request!\n");
        }

        sqlite3_close(db);

        // close socket
        close(cfd);
        
        printf("Closed connection to client.\n");
    }
}