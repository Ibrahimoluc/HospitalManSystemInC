#ifndef DB_OPS
#define DB_OPS

#include <mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int db_select(char *query, MYSQL *conn, MYSQL_RES **res);

int db_insert(char *query, MYSQL *conn); //Buradaki res fonksiyon icinde local de var da olabilir

int findObjects(MYSQL *conn, char *table_name, char **fields_val);

#endif
