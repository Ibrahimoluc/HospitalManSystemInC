#include "patient.h"
#include "algos.h"

int acceptPatient(patient *p){
	//id,long birth_date,f_name,l_name,gender, float height, weight
	int ret;	

        printf("Birth Date:");
        ret = acceptLong(&p->birth_date);
        if(ret<=0){
                printf("Pls give valid birth date\n");
                return -1;
        }


	//get first_name
        printf("First Name:");
        ret = acceptString(p->first_name);
        if(ret<=0){
                printf("Pls give valid first name\n");
                return -1;
        }

	//get last_name
        printf("Last Name:");
        ret = acceptString(p->last_name);
        if(ret<=0){
                printf("Pls give valid last name\n");
                return -1;
        }
	
	
	//get gender
        printf("Gender(M for Male, F for Female):");
        ret = acceptChar(&p->gender);
        if(ret<=0){
                printf("Pls give valid first name\n");
                return -1;
        }

        //get height
        printf("Height:");
        ret = acceptFloat(&p->height);
        if(ret<=0){
                printf("Pls give valid height(m)\n");
                return -1;
        }


        //get height
        printf("Weight:");
        ret = acceptFloat(&p->weight);
        if(ret<=0){
                printf("Pls give valid weight(kg)\n");
                return -1;
        }

}


int insertPatient(MYSQL *conn){
	int ret;
	char query[1024];

	printf("inserting the patient...\n");
	patient p;
	ret = acceptPatient(&p);
	if(ret<0){
		printf("error at acceptPatient\n");
		return -1;
	}

	sprintf(query, "INSERT INTO patient(birth_date, first_name, last_name, gender, height, weight) VALUES(%ld, \"%s\", \"%s\", \"%c\", %f, %f)", 
			p.birth_date, p.first_name, p.last_name, p.gender, p.height, p.weight);
	
	printf(query);
	ret = db_insert(query, conn);
    	if(ret<0){
    		printf("error at db_insert:\n"); 
		return -1;
	}

	printf("patient succesfull inserted\n");
	return 0;

}

int selectAllPatients(MYSQL *conn){
	int ret;
	MYSQL_RES *res;
	MYSQL_ROW row;
	patient p;
	char query[1024];
	
	printf("selecting all patients...\n");
	strcpy(query,"SELECT * FROM patient");
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
                ret = Prow2Patient(row, &p);
		if(ret!=0){
			printf("error at Prow2Patient\n");
			return -1;
		}

    		//output u yazdir
		printPatient(p);		
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


int Prow2Patient(MYSQL_ROW row, patient *p){
	p->id = atoi(row[0]);
	p->birth_date = atol(row[1]);
	strcpy(p->first_name, row[2]);
	strcpy(p->last_name, row[3]);

	p->gender = *row[4];
	p->height = atof(row[5]);
	p->weight = atof(row[6]);

	return 0;
}


void printPatient(patient p){
        printf("Printing the Patient...\n");
        printf("Patient Id:%d\n", p.id);
        printf("Birth date:%ld\n", p.birth_date);
	printf("First Name:%s\n", p.first_name);
        printf("Last Name:%s\n", p.last_name);
        printf("Gender:%c\n", p.gender);
        printf("Height:%f\n", p.height);
        printf("Weight:%f\n", p.weight);
        printf("\n");
}


void printPArr(patient *p_arr, int size){
	for(int i=0;i<size;i++){
		printPatient(p_arr[i]);
	}
}


int getPatientById(int id, patient *p_arr, int size, patient **adrOfp){
	for(int i=0;i<size;i++){
		if(p_arr[i].id==id){
			*adrOfp = &p_arr[i];
			return 0;
		}
	}

	printf("Patient with given Id not found\n");

	return -1;
}



int findPatientById(int id, MYSQL *conn, patient *adrOfp){
	int ret;
	int size;
	MYSQL_RES *res;
	MYSQL_ROW row;
	patient p;
	char query[1024];
	printf("finding patient by id...\n");
	sprintf(query, "SELECT * FROM patient WHERE id=%d", id);

	ret = db_select(query, conn, &res);
	if(ret!=0){
		printf("error at db_select\n");
		return -1;
    	}

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
		return -1;
	}

	//fetch row
	row = mysql_fetch_row(res);
	if(mysql_errno(conn)!=0){
		printf("error at mysql_fetch_row\n");
		mysql_free_result(res);
		return -1;
	}

	//row to Doctor and print it
	Prow2Patient(row, &p);
	mysql_free_result(res);

	printPatient(p);
	*adrOfp = p; 

	return 0;
}


int selectPatients(long birth_date, char *first_name, char *last_name, char gender, float height, float weight, MYSQL *conn){
	int ret;
	int size;
	int id;
	MYSQL_RES *res;
	patient *p_list;
	patient *p;

	ret = findPatients(birth_date, first_name, last_name, gender, height, weight, conn, &res);
	if(ret<0){
		printf("error at findPatients\n");
		return -1;
	}

	
	size = pRes2PArr(conn, res, &p_list);
	if(size<0){
		printf("error at pRes2PArr");
		return -1;
	}
	mysql_free_result(res);

	int patientIds[size];

	//printPArr(p_list, size);
	
	
	for(int i=0;i<size;i++){
		patientIds[i] = p_list[i].id;
		printf("id:%d\n", p_list[i].id);
		printf("first name:%s\n", p_list[i].first_name);
		printf("last name:%s\n", p_list[i].last_name);
		printf("\n");
	}

	printf("id:");
	ret = acceptInt(&id);
	if(ret==0){
		printf("error at acceptInt\n");
		free(p_list);
		return -1;
	}

	if(!haveInt(patientIds, size, id)){
		printf("Given id is not in the list.\n");
		free(p_list);
		return -1;
	}

	ret = getPatientById(id, p_list, size, &p);
	if(ret<0){
		printf("error at getPatientById.\n");
		free(p_list);
		return -1;
	}

	printPatient(*p);
	free(p_list);
	
	return 0;
}


int findPatients(long birth_date, char *first_name, char *last_name, char gender, float height, float weight, MYSQL *conn, MYSQL_RES **adrOfRes){
	int ret;
	int size;
	MYSQL_RES *res = NULL;
	MYSQL_ROW row;
	char query[1024];
	char q2[1024];


	strcpy(query,"SELECT * FROM patient where 1");

	if(birth_date!=0){
		sprintf(q2, "  AND birth_date=%ld", birth_date);
		strcat(query, q2);
	}
	
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

	if(gender!=0){
		//format string
		sprintf(q2, "  AND gender=\"%c\"", gender);
		//concat for first_name
		strcat(query, q2);
	}	

	if(height!=0){
		sprintf(q2, "  AND height=%.2f", height);
		strcat(query, q2);
	}

	if(weight!=0){
		sprintf(q2, "  AND weight=%.2f", weight);
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


int pRes2PArr(MYSQL *conn, MYSQL_RES *res, patient **adrOfp_list){
	int ret;
	int size;
	MYSQL_ROW row;
	patient *p_list;
	patient p;

	if(res==NULL){
		printf("mysql_res is pointed NULL");
		return -1;
	}

	size = mysql_num_rows(res);
	if(size==0){
		printf("size is 0. return 0 from pRes2PArr");
		mysql_free_result(res);
		return -1;
	}

	p_list = (patient*)calloc(size, sizeof(patient)); //calloca gore duzenle
	*adrOfp_list = p_list;

	for(int i=0;i<size;i++){

		//ffetch row
    		row = mysql_fetch_row(res);
    		if(row==NULL){
			break;		
		}

		//row to Doctor, controls can be added.
                ret = Prow2Patient(row, &p);
		if(ret!=0){
			printf("error at Prow2Patient\n");
			free(p_list);
			mysql_free_result(res);
			return -1;
		}

    		//output u yazdir ve yazdigin seyi yerlestir
		//printDoctor(d);
		p_list[i] = p;		
	}
	
	if(mysql_errno(conn)!=0){
	    	printf("error at mysql_fetch_row\n");
		mysql_free_result(res);
		return -1;
    	}


	//!!!!cagirdigim fonksiyonda array i de salmaliyim(free)

	return size;

	return 0;
}


enum body_class body_mass_index(float height, float weight){
	float ans;
	enum body_class class;

	ans = weight/(height*height);
	printf("body_mass_index:%f\n", ans);

	if(ans<18.5) return UNDER;

	if(ans<25) return NORMAL;

	if(ans<30) return HIGH;

	return OBESE;
}


int findBMIofPat(int id, MYSQL *conn){
	int ret;
	patient p;
	enum body_class class;
	ret = findPatientById(id, conn, &p);
	if(ret<0){
		printf("error at findPatientById\n");
		return -1;
	}

	printPatient(p);
	class = body_mass_index(p.height, p.weight);

	switch(class){
		case UNDER:
			printf("Given patient is underweight\n");
			break;
		case NORMAL:
			printf("Given patient is normalweight\n");
			break;
		case HIGH:
			printf("Given patient is overweight\n");
			break;
		case OBESE:
			printf("Given patient is OBESE\n");
			break;
	}

	return 0;
}


int deletePatById(MYSQL *conn, int id){
	int ret;
	char query[1024];

	sprintf(query, "DELETE FROM patient WHERE id=%d", id);
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
	
	printf("pat succesfully deleted");
	return 0;
}


int updatePatById(MYSQL *conn, int id, patient newPat){
	int ret;
	char query[1024];

	//long birth_date, char *first_name, char *last_name, char gender, float height, float weight,
	sprintf(query, "UPDATE patient SET birth_date=%ld, first_name=\"%s\", last_name=\"%s\", gender=\"%c\", height=%f, weight=%f WHERE id=%d",
			newPat.birth_date, newPat.first_name, newPat.last_name, newPat.gender, newPat.height, newPat.weight, id);
	printf("%s\n", query);
	
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
	
	printf("patient succesfully updated");

	return 0;
}


//some experiments to see how mysql.h functions work
int deneme(MYSQL *conn){
	int ret;
	int size;
	unsigned long coulumn_length;
	unsigned long *col_lengths;
	MYSQL_RES *res;
	MYSQL_ROW row;
	//res struct ini ve icindeki lengthin row un uzunlugunu mu veriyor diye kontrol et
	//BURADA KALDIN...

	ret = db_select("SELECT * FROM patient", conn, &res);
	if(ret!=0){
		//mysql_free_result cagirmiyorum, cunku buraya girdiyse res hala NULL bence.
		printf("error at db_select\n");
		return -1;
	}
	
	/*
	//MYSQL_ROWS icindeki length? 7 bekliyordum ama 0 geldi?
	//col_lengths sacma degerler gosteriyor, neden?
	row = mysql_fetch_row(res);
	
	col_lengths = res->lengths;
	printf("col_length:%p\n", col_lengths); //bunlarin da hepsi 0 geliyor

	col_lengths = mysql_fetch_lengths(res);
	printf("col_length:%p\n", col_lengths); //bunlarin da hepsi 0 geliyor
	
	for(int i=0;i<10;i++){
		printf("col_lengths[%d]:%lu\n", i, col_lengths[i]);
	}
	*/

	//row_count variable in MYSQL_RES
	int rc = res->row_count;
	printf("row_count:%d\n", rc);

	/*rowlara bu sekilde de ulasabilirsin.
	row = res->data_cursor->data; 
	char *buffer = row[0];
	char *buffer2 = row[1];
	char *buffer3 = row[2];
	printf("First row:\n");
	printf("buffer:%s\n", buffer);
	printf("buffer2:%s\n", buffer2);
	printf("buffer3:%s\n", buffer3);

	printf("\n\n");
	*/

	size = mysql_num_rows(res);
	if(size<0){
		printf("error at mysql_num_rows\n");
		return -1;
	}
	printf("size:%d\n", size);
	
	MYSQL_ROWS *rowNode = res->data_cursor;
	for(int i=0;i<size;i++){
		/*fetch_row row degiskenine dondururken RES icindeki data cursor gibi bir seyi bir ileri arttiriyor olabilir
		//row = mysql_fetch_row(res);
		//if(!row) break;
		*/

		//row_fetch yerine boyle de rowlara ulasabilir miyiz? Bu arada rowlarin hepsi res i bir kere cagirdiginda mvemorydeyse
		//onlari gerekli arraye alip res i free lemeke mantikli, cunku res struct i her turlu daha fazla yer kaplar.
		
		//bu ise yariyor, demekki fetch_row buna benzer calisiyor. Yani RES icindeki ROWS(rowNode) dan data(row) donduruyor
		//daha sonra ROWS ptr sini ROWS icindeki next i atiyor. Linked List gibi. 
		row = rowNode->data;
		printf("row[0]:%s\n", row[0]);
		printf("row[1]:%s\n", row[1]);
		printf("row[2]:%s\n", row[2]);
		printf("\n");
		
		rowNode = rowNode->next;
	}
	
	return 0;
}



