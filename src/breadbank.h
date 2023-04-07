/* Related to the sqlite3 interface */
sqlite3 *db;                
sqlite3_stmt *statement;
int rc;

/* Variables and arrays */
int sfd;
int cfd;

/* Functions */
void login(char*);
void transfer(char*);
void get_account_details(char*);
void get_holder_id(char*);