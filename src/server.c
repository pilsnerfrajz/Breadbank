#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>

/*
To compile: 
    install sqlite3
    run sudo apt-get install libsqlite3-dev
    gcc server.c -o server -lsqlite3
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
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sfd < 0)
    {
        perror("socket");
        return -1;
    }

    // Bind to port
    if(0 > bind(sfd, (struct sockaddr *)&server_info, sizeof(server_info)))
    {
        perror("bind");
        return -1;
    }

    // Listen for clients
    if(0 > listen(sfd, 5)){
        perror("listen");
        return -1;
    }

    printf("Listening for clients.\n");

    // accept connection 
    int cfd = accept(sfd, (struct sockaddr *)&server_info, &client_info_len);
    if(0 > cfd)
    {
        perror("accept");
        return -1;
    }

    printf("Client connected.\n");

    // recieve data from client
    char client_data[256];
    if(0 > recvfrom(cfd, client_data, sizeof(client_data), 0, (struct sockaddr *)&client_info, &client_info_len))
    {
        perror("recieve");
        return -1;
    }

    printf("Received data from client:\n%s\n", client_data);

    // connect to database and stuff 

    sqlite3 *db;                // database connection
    sqlite3_stmt *statement;    // statement object
    int rc;                     // return code

    // open database connection
    rc = sqlite3_open("test.db", &db);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    printf("Connected to database.\n");

    // prepare SQL-statement
    rc = sqlite3_prepare_v2(db, client_data, -1, &statement, 0);
    if(rc != SQLITE_OK){
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    printf("Prepared SQL-statement.\n");

    // evaluate SQL-statement
    rc = sqlite3_step(statement);
    if(rc != SQLITE_DONE){
        fprintf(stderr, "Failed to evaluate statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(statement);
        sqlite3_close(db);
        return 1;
    }
    printf("Statement successfully evaluated.\n");

    // destroy statement
    sqlite3_finalize(statement);

    // close connection to db. TODO: add errorchecks?
    sqlite3_close(db);

    //send data back
    char *response = "Transfer complete!\n";
    if(0 > send(cfd, (void *)response, strlen(response), 0))
    {
        perror("send");
        return -1;
    }

    printf("Sent response to client.\n");

    // close socket
    close(cfd);
    
    printf("Closed connection to client.\n");
}