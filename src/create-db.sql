--sqlite3 breadbank.db < create-db.sql

CREATE TABLE holder
    (id INTEGER PRIMARY KEY AUTOINCREMENT, 
    first_name TEXT NOT NULL,
    last_name TEXT NOTT NULL);

CREATE TABLE account
    (id INTEGER PRIMARY KEY AUTOINCREMENT, 
    account_number TEXT NOT NULL,
    balance REAL NOT NULL,
    holder INTEGER NOT NULL, FOREIGN KEY(holder) REFERENCES holder(id));

CREATE TABLE login
    (id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    password TEXT NOT NULL,
    holder INTEGER NOT NULL, FOREIGN KEY(holder) REFERENCES holder(id));

INSERT INTO holder
    (first_name, last_name) VALUES
    ('William', 'Hedenskog'),
    ('Pontus', 'Loverboy'),
    ('Hugo', 'Loverboy');

INSERT INTO account
    (account_number, balance, holder) VALUES
    ('1337-300', 1000, (SELECT id FROM holder WHERE first_name='William')),
    ('1337-314', 500, (SELECT id FROM holder WHERE first_name='William')),
    ('1337-420', 2000, (SELECT id FROM holder WHERE first_name='William')),
    ('1337-069', 1000, (SELECT id FROM holder WHERE first_name='Pontus')),
    ('1337-666', 1000, (SELECT id FROM holder WHERE first_name='Hugo')),
    ('1337-999', 1000, (SELECT id FROM holder WHERE first_name='Hugo'));

INSERT INTO login
    (username, password, holder) VALUES
    ('riks', 'password1', (SELECT id FROM holder WHERE first_name='William')),
    ('gotland', 'password2', (SELECT id FROM holder WHERE first_name='Pontus')),
    ('småland', 'password3', (SELECT id FROM holder WHERE first_name='Hugo'));
