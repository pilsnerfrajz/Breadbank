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

void exec_transfer(char* query) {
    char* comp = "";
    if(strcmp(query, comp) == 0)
        return;
    
    // contains remaining query
    const char* pzTail;

    rc = sqlite3_prepare_v2(db, query, -1, &statement, &pzTail);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }

    rc = sqlite3_step(statement);
    if(rc == SQLITE_DONE) {
        sqlite3_finalize(statement);
        exec_transfer((char*)pzTail);
    } else if(rc != SQLITE_DONE) {
        fprintf(stderr, "Failed to evaluate statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(statement);
        sqlite3_close(db);
        exit(0);
    }
    return;
}

int comp_balance(char* query, char* transfer_sum) {
    rc = sqlite3_open("/Users/williamhedenskog/breadbank.db", &db);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }

    rc = sqlite3_prepare_v2(db, query, -1, &statement, 0);
    if(rc != SQLITE_OK) {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }

    const unsigned char *current_balance;
    
    rc = sqlite3_step(statement);
    if(rc == SQLITE_ROW) {
        current_balance = sqlite3_column_text(statement, 0);
    }

    char to_float[100];
    int i = 0;
    while(*current_balance != '\0'){
        to_float[i++] = *current_balance;
        current_balance++;
    }
    to_float[i] = '\0';

    float f1 = atof(transfer_sum + 1);
    float f2 = atof(to_float);

    if(f1 <= f2) {
        sqlite3_finalize(statement);
        return 1;
    } else {
        sqlite3_finalize(statement);
        return 0;
    }
}

void transfer(char* query) {
    /* First check if account has enough money */
    const char delim1 = '-';
    const char delim2 = '\'';
    char* ret1;
    char* ret2;
    char sum[20];
    char acc[20];

    ret1 = strchr((const char *)query, delim1);
    int i = 1;
    while(ret1[i] != ' ') {
        sum[i] = ret1[i];
        i++;
    }
    sum[i] = '\0';

    ret2 = strchr((const char *)ret1, delim2);
    i = 0;
    while(ret2[i] != 'U') {
        acc[i] = ret2[i];
        i++;
    }
    acc[i] = '\0';

    char check_bal[70] = "SELECT balance FROM account WHERE account_number=";
    strcat(check_bal, acc);
    int enough_balance = comp_balance(check_bal, sum);

    if(enough_balance) {
        exec_transfer(query);
    } else {
        // send error to client
        printf("NOT ENOUGH MONEY\n");
    }
    return;
}