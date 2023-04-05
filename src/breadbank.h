/* Related to the sqlite3 interface */
sqlite3 *db;                
sqlite3_stmt *statement;
int rc;

/* Variables and arrays */
int sfd;
int cfd;

/* Functions */
void login(char*);