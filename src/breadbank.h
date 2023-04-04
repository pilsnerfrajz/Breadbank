/* Related to the sqlite3 interface */
sqlite3 *db;                
sqlite3_stmt *statement;
int rc;

void login(char*);
void get_account_details(char* query);