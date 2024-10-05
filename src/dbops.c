#include "dbops.h"

int db_select(char *query, MYSQL *conn, MYSQL_RES **res){
    int ret;
    //MYSQL_ROW row;

    ret = mysql_query(conn, query);
    if(ret!=0){
	    printf("error at mysql_query\n");
	    return -1;
    }

    *res = mysql_store_result(conn);
    if(*res==NULL){
	    printf("error occured or res set is empty in mysql_store_result\n");
	    return -1;
    }
    

    return 0;
}


int db_insert(char *query, MYSQL *conn){
    int ret;
    int affectedRows;

    ret = mysql_query(conn, query);
    if(ret!=0){
	    printf("error at mysql_query\n");
	    return -1;
    }

    affectedRows = mysql_affected_rows(conn);

    ret = mysql_commit(conn);
    if(ret!=0){
	    printf("mysql_commit error, return not 0\n");
	    return -1;
    }

    return affectedRows;
}    


//it is not completed.
int findObjects(MYSQL *conn, char *table_name, char **fields_val){
	char query[1024] = "SELECT ";
	char tempstr[128];
	int ret;
	char *p = query+7;
	MYSQL_ROW row;

	//get list from result, buraya kontrol ekleyebilirsin
	sprintf(tempstr, "SHOW COLUMNS FROM %s", table_name);
	ret = mysql_query(conn, tempstr);
	MYSQL_RES *res = mysql_store_result(conn);
	//MYSQL_RES *res = mysql_list_fields(conn, table_name, 0);
	//MYSQL_FIELD *fields = mysql_fetch_fields(res);

	int size = mysql_num_rows(res);
	//mysql_free_result(res);
	printf("Number of columns: %d\n", size);
	char *field_names[size];
	

	for(int i=0;i<size;i++){
		row = mysql_fetch_row(res);
		if(row[0]==NULL) break;
		field_names[i] = row[0];
		p = stpcpy(p, field_names[i]);
		p = stpcpy(p, ",");
	}


	//delete last ',' from string
	*(p-1) = ' ';
	
	//table name
	p = stpcpy(p, "FROM ");
	p = stpcpy(p, table_name);

	
	//where filter if any
	p = stpcpy(p, "WHERE 1 ");

	for(int i=0;i<size;i++){
		if(fields_val[i]!=NULL){
			sprintf(tempstr, " AND %s=%s", field_names[i], fields_val[i]);
			p = stpcpy(p, tempstr);
		}
	}
	

	printf("%s\n", query);
	return 0;

}
