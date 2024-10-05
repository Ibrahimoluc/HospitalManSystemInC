#include "doctor.h"
#include "algos.h"

int acceptDoctor(doctor *d){
	int ret;

        printf("First Name:");
        ret = acceptString(d->first_name);
	if(ret<0){
		printf("Pls give valid first name\n");
		return -1;
	}

        printf("Last Name:");
        ret = acceptString(d->last_name);
	if(ret<0){
		printf("Pls give valid last name\n");
		return -1;
	}

        printf("Special:");
        ret = acceptString(d->special);
	if(ret<0){
		printf("Pls give valid special\n");
		return -1;
	}

	return 0;
}


int insertDoctor(MYSQL *conn){
	int ret;
	char query[1024];

	printf("inserting the doctor...\n");
	doctor d;
	ret = acceptDoctor(&d);
	if(ret<0){
		printf("error at acceptDoctor\n");
		return -1;
	}

	sprintf(query, "INSERT INTO doctor(first_name, last_name, special) VALUES(\"%s\", \"%s\", \"%s\")", 
			d.first_name, d.last_name, d.special);
	
	printf(query);
	ret = db_insert(query, conn);
    	if(ret<0){
    		printf("error at db_insert:\n"); 
		return -1;
	}

	printf("doctor succesfull inserted\n");
	return 0;
}


int deleteDoctorById(MYSQL *conn, int id){
	int ret;
	char query[1024];

	sprintf(query, "DELETE FROM doctor WHERE id=%d", id);
	printf(query);
	printf("\n");

	//query match eklenmeli
	ret = db_insert(query, conn);
    	if(ret<0){
		printf("error at query\n");
		return -1;
	}

	if(ret==0){
		printf("no match row found.\n");
		return -1;
	}
	
	printf("doctor succesfully deleted");
	return 0;
}


int updateDoctorById(MYSQL *conn, int id, char *first_name, char *last_name, char *special){
	int ret;
	char query[1024];

	sprintf(query, "UPDATE doctor SET first_name=\"%s\", last_name=\"%s\", special=\"%s\" WHERE id=%d", first_name, last_name, special, id);
	printf(query);
	printf("\n");

	ret = db_insert(query, conn);

	//query match eklenmeli
	if(ret<0){
		printf("error at query\n");
		return -1;
	}
	if(ret==0){
		printf("no match row found.\n");
		return -1;
	}
	
	printf("doctor succesfully updated");
	return 0;
}


int selectAllDoctors(MYSQL *conn){
	int ret;
	MYSQL_RES *res;
	MYSQL_ROW row;
	doctor d;
	char query[1024];
	
	printf("selecting all doctors...\n");
	strcpy(query,"SELECT id,first_name,last_name,special FROM doctor");
	ret = db_select(query, conn, &res);
	if(ret!=0){
		printf("error at db_select\n");
		return -1;
    	}
	
	while(1){
		//ffetch row
    		row = mysql_fetch_row(res);
    		if(row==NULL){
			break;		
		}

		//row to Doctor, controls can be added.
                ret = drow2Doctor(row, &d);
		if(ret!=0){
			printf("error at drow2Doctor\n");
			return -1;
		}

    		//output u yazdir
		printDoctor(d);		
	} 


	if(mysql_errno(conn)!=0){
	    	printf("error at mysql_fetch_row\n");
		mysql_free_result(res);
		return -1;
    	}

	//free result set
	mysql_free_result(res);

	return 0;
}

//kendisi db_select cagirip ayri bir res olusturdugu icin ve freeledigi icin res i param degil local var yapiyorum.
int findDoctorById(int id, MYSQL *conn){
	int ret;
	int size;
	MYSQL_RES *res;
	MYSQL_ROW row;
	doctor d;
	char query[1024];
	printf("finding doctor by id...\n");
	sprintf(query, "SELECT id, first_name, last_name, special FROM doctor WHERE id=%d", id);

	ret = db_select(query, conn, &res);
	if(ret!=0){
		printf("error at db_select\n");
		return -1;
    	}

	//problem yoksa sadece bir tane row gelmesi lazim, ama yine de kontrol edelim
	size = mysql_num_rows(res);
	printf("size:%d", size);
	if(size>1){
		printf("There are multiple rows with given ID");
		mysql_free_result(res);
		return -1;
	}

	if(size==0){
		printf("No row found with given Id.\n");
		mysql_free_result(res);
		return 0;
	}

	//fetch row
	row = mysql_fetch_row(res);
	if(mysql_errno(conn)!=0){
		printf("error at mysql_fetch_row\n");
		mysql_free_result(res);
		return -1;
	}

	//row to Doctor and print it
	drow2Doctor(row, &d);
	mysql_free_result(res);

	printDoctor(d);

	return 0;
}
	
int drow2Doctor(MYSQL_ROW row, doctor *d){
	d->id = atoi(row[0]);
	strcpy(d->first_name, row[1]);
	strcpy(d->last_name, row[2]);
	strcpy(d->special, row[3]);

	return 0;
}


void printDoctor(struct doctor d){
        printf("Printing the Doctor...\n");
        printf("Doctor Id:%d\n", d.id);
        printf("First Name:%s\n", d.first_name);
        printf("Last Name:%s\n", d.last_name);
        printf("Speciality:%s\n", d.special);
        printf("\n");
}


void printDArr(doctor *d_arr, int size){
	for(int i=0;i<size;i++){
		printDoctor(d_arr[i]);
	}
}


//su an sadece print ediyorsun ama baska islemlerde yapiyor olsaydin ustunde, arrayin ustunde yapmak daha rahat olacakti
int findDoctors(char *first_name, char *last_name, char *special, MYSQL *conn, MYSQL_RES **adrOfRes){
	int ret;
	int size;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	char query[1024];
	char q2[1024];


	strcpy(query,"SELECT id,first_name,last_name,special FROM doctor where 1");

	//buralara last_name ve special da eklenecek
	if(first_name!=NULL){
		//format string
		sprintf(q2, "  AND first_name=\"%s\"", first_name);
		//concat for first_name
		strcat(query, q2);
	}	

	if(last_name!=NULL){
		//format string
		sprintf(q2, "  AND last_name=\"%s\"", last_name);
		//concat for first_name
		strcat(query, q2);
	}	

	if(special!=NULL){
		//format string
		sprintf(q2, "  AND special=\"%s\"", special);
		//concat for first_name
		strcat(query, q2);
	}	

	//alttaki kontrollere dikkatlice bak
	printf(query);	
	printf("\n");
	
	ret = db_select(query, conn, &res);
	if(ret!=0){
		//mysql_free_result cagirmiyorum, cunku buraya girdiyse res hala NULL bence.
		printf("error at db_select\n");
		return -1;
	}

	size = mysql_num_rows(res);
	if(size==0){
		printf("No row found with given attiributes.\n");
		mysql_free_result(res);
		return -1;
	}

	
	
	*adrOfRes = res;

	return 0;
}


int selectDoctors(char *first_name, char *last_name, char *special, MYSQL *conn){
	int ret;
	int size;
	int id;
	MYSQL_RES *res;
	doctor *d_list;
	doctor *d;

	ret = findDoctors(first_name, last_name, special, conn, &res);
	if(ret<0){
		printf("error at findDoctors\n");
		return -1;
	}

	//printf("adrOfRes from selectDoctors:%p", res);
	
	size = dRes2DArr(conn, res, &d_list);
	if(size<0){
		printf("error at dRes2DArr");
		return -1;
	}
	mysql_free_result(res);

	int doctorIds[size];

	//printDArr(d_list, size);
	for(int i=0;i<size;i++){
		doctorIds[i] = d_list[i].id;
		printf("id:%d\n", d_list[i].id);
		printf("first name:%s\n", d_list[i].first_name);
		printf("last name:%s\n", d_list[i].last_name);
		printf("\n");
	}

	printf("id:");
	ret = acceptInt(&id);
	if(ret==0){
		printf("error at acceptInt\n");
		free(d_list);
		return -1;
	}

	if(!haveInt(doctorIds, size, id)){
		printf("Given id is not in the list.\n");
		free(d_list);
		return -1;
	}

	ret = getDoctorById(id, d_list, size, &d);
	if(ret<0){
		printf("error at getDoctorById.\n");
		free(d_list);
		return -1;
	}

	printDoctor(*d);
	free(d_list);
	return 0;
}


//RES alip basan bir print yapilabilir


int getDoctorById(int id, doctor *d_arr, int size, doctor **adrOfd){
	for(int i=0;i<size;i++){
		if(d_arr[i].id==id){
			*adrOfd = &d_arr[i];
			return 0;
		}
	}

	printf("Doctor with given Id not found\n");

	return -1;
}


int dRes2DArr(MYSQL *conn, MYSQL_RES *res, doctor **adrOfd_list){
	int ret;
	int size;
	MYSQL_ROW row;
	doctor *d_list;
	doctor d;
	
	
	if(res==NULL){
		printf("mysql_res is pointed NULL");
		return -1;
	}

	size = mysql_num_rows(res);
	if(size==0){
		printf("size is 0. return 0 from dRes2DArr");
		mysql_free_result(res);
		return -1;
	}

	d_list = (doctor*)calloc(size, sizeof(doctor)); //calloca gore duzenle
	*adrOfd_list = d_list;

	for(int i=0;i<size;i++){

		//ffetch row
    		row = mysql_fetch_row(res);
    		if(row==NULL){
			break;		
		}

		//row to Doctor, controls can be added.
                ret = drow2Doctor(row, &d);
		if(ret!=0){
			printf("error at drow2Doctor\n");
			free(d_list);
			mysql_free_result(res);
			return -1;
		}

		//printDoctor(d);
		d_list[i] = d;		
	}
	
	if(mysql_errno(conn)!=0){
	    	printf("error at mysql_fetch_row\n");
		mysql_free_result(res);
		return -1;
    	}


	//!!!!cagirdigim fonksiyonda array i de salmaliyim(free)

	return size;
}

