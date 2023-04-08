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

void send_account_details(char* to_client) {
    char *space = " ";
    if(0 > send(cfd, (void *)to_client, strlen(to_client), 0)) {
        perror("Send");
        sqlite3_finalize(statement);
        sqlite3_close(db);
        exit(0);
    }
     if(0 > send(cfd, (void *)space, strlen(space), 0)) {
        perror("Send");
        sqlite3_finalize(statement);
        sqlite3_close(db);
        exit(0);
    }
    return;
}

void get_account_details(char* query) {
    char account_query[50] = "SELECT * FROM account WHERE holder=";
    char semicolon[2] = ";";

    strcat(account_query, (const char*)query);
    strcat(account_query, semicolon);

    // prepare SQL-statement
    rc = sqlite3_prepare_v2(db, account_query, -1, &statement, 0);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }

    const unsigned char *account_number;
    const unsigned char *balance;
    
    while((rc = sqlite3_step(statement)) == SQLITE_ROW) {
        account_number = sqlite3_column_text(statement, 1);
        balance = sqlite3_column_text(statement, 2);
        send_account_details((char*) account_number);
        send_account_details((char*) balance);
    }
    sqlite3_finalize(statement);
    return;
}

void login(char* query) {
    // open database connection
    rc = sqlite3_open("/Users/williamhedenskog/breadbank.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }
    printf("Connected to database.\n");
/*
    const unsigned char *holder_id;
    //char account_query[50] = "SELECT * FROM account WHERE holder=";
    //char semicolon[2] = ";";

    // prepare SQL-statement
    rc = sqlite3_prepare_v2(db, query, -1, &statement, 0);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }
    printf("Prepared SQL-statement.\n");

    // evaluate SQL-statement
    rc = sqlite3_step(statement);
    if(rc == SQLITE_ROW) {
        holder_id = sqlite3_column_text(statement, 3);
        char copy_id[10] = "";
        //strcat(account_query, (const char*)holder_id);
        //strcat(account_query, semicolon);
        //printf("%s\n", account_query);
        strcat(copy_id, (const char*)holder_id);
        sqlite3_finalize(statement);
        get_account_details(copy_id);//account_query);
    } else {
        printf("Wrong username or password!\n"); // SEND TO CLIENT
        sqlite3_finalize(statement);
    }
    */
    get_holder_send_info(query);
    return;
}