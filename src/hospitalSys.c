#include <mysql.h> //C-Connector Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dbops.h"
#include "inout.h"
#include "doctor.h"
#include "patient.h"

void printDocMenu(){
        printf("Doctor Menu...\n");
        printf("1- Press i to insert a doctor\n");
        printf("2- Press s to list all doctors\n");
	printf("4- Press f to find a doctor by id\n");
	printf("5- Press n to find doctors by name\n");
	printf("6- Press b to find doctors by special\n");
	printf("7- Press p to find doctors by first and last name\n");
	printf("8- Press d to delete doctor by id\n");
	printf("9- Press u to update doctor by id\n");
        printf("10- Press q to quit\n");
}


void printPatMenu(){
	printf("Patient Menu...\n");
        printf("1- Press i to insert a patient\n");
        printf("2- Press s to list all patients\n");
	printf("3- Press f to find patient by id\n");
	printf("4- Press n to find patients by name\n");
	printf("5- Press g to find patients by gender\n");
	printf("6- Press b to find body_mass_index by id\n");
	printf("7- Press d to delete patient by id\n");
	printf("8- Press u to update patient by id\n");
	printf("9- Press q to quit \n");
}


int main() {
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;
    int ret; //to keep return values
    doctor *d_list;
    patient p;
    int id;
    int size;
    char c;
    char query[1024];
    char temp[128]; //to get input from user like first_name...
    char temp2[128]; //to get input from user like first_name...

    const char *server; //assign your server name
    const char *user; //assign your username
    const char *password; //assign your password
    const char *database; //assign your database name

    // MySQL kutuphanesini baglat
    conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        exit(EXIT_FAILURE);
    }

    int ssl_mode = SSL_MODE_DISABLED;
    mysql_options(conn, MYSQL_OPT_SSL_MODE, &ssl_mode);

    // Veritabanina baglan
    if (mysql_real_connect(conn, server, user, password, database, 3306, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        fprintf(stderr, "Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        exit(EXIT_FAILURE);
    }

    // Baglanti basarili
    printf("Connection successful!\n");

    // Veritabani islemleri burad...
    char Mchoice=0;
    do{
    	printf("p to get patient menu.\n");
	printf("d to get doctor menu.\n");
	printf("q to quit\n\n");
                
	acceptChar(&Mchoice);
	switch(Mchoice){
		case 'p':
			//print patient menu and operations in switch
			char choice2=0;
			do{
				printPatMenu();
				acceptChar(&choice2);
				switch(choice2){
					case 'i':
						insertPatient(conn);
						break;
					case 's':
						selectAllPatients(conn);
						break;
					case 'f':
						printf("Give Id:");
						ret = acceptInt(&id);
						printf("id:%d\n", id);
						printf("ret:%d\n", ret);
						if(ret==0) break;
						findPatientById(id, conn, &p);
						break;
					case 'n':
						printf("Give first name:");
						ret = acceptString2(temp);
						if(!ret) break;

						selectPatients(0, temp, 0, 0, 0, 0, conn);
						break;
					case 'g':
						printf("Give gender:");
						ret = acceptChar(&c);
						if(!ret) break;
						if(c!='F' && c!='M'){
						       	printf("Pls input F or M\n");
							break;
						}

						selectPatients(0, 0, 0, c, 0, 0, conn);
						break;
					case 'b':
						printf("id:");
						ret = acceptInt(&id);
						if(!ret) break;

						ret = findBMIofPat(id, conn);
						
						break;
					case 'd':
						printf("deleting the patient by id...\n");
						printf("id:\n");
						ret = acceptInt(&id);
						if(!ret) break;

						deletePatById(conn, id);
						break;
					case 'u':
						printf("updating a patient by id...\n");
						printf("id:\n");
						ret = acceptInt(&id);
						if(!ret) break;

						patient p;
						acceptPatient(&p);
						ret = updatePatById(conn, id, p);
						if(ret<0){
							printf("error at updatePatById\n");
						}
						break;
					case 'q':
						printf("Quiting patient menu...\n");
						break;
				}
			} while(choice2 != 'q');

			break;
		case 'd':

    			char choice=0;
    			do{
				printDocMenu();
        			acceptChar(&choice);	
				switch(choice){
					case 'i':
						insertDoctor(conn);
						break;
					case 's':
						selectAllDoctors(conn); //res parametresi kalkabilir.
						break;
					case 'f':
						printf("Give Id:");
						ret = acceptInt(&id);
						printf("id:%d\n", id);
						printf("ret:%d\n", ret);
						if(ret==0) break;
						findDoctorById(id, conn);
						break;
					case 'n': 
						printf("Give first name:");
						ret = acceptString2(temp);
						if(!ret) break;
						selectDoctors(temp, 0, 0, conn);
						break;
					case 'b':
						printf("Give special:");
						ret = acceptString2(temp);
						if(!ret) break;
						selectDoctors(0, 0, temp, conn);
						break;
					case 'p': 
						printf("Give first name:");
						ret = acceptString2(temp);
						if(!ret) break;
			
						printf("Give last name:");
						ret = acceptString2(temp2);
						if(!ret) break;

						selectDoctors(temp, temp2, 0, conn);
						break;
					case 'd':
						printf("deleting the doctor by id...\n");
						printf("id:\n");
						ret = acceptInt(&id);
						if(!ret) break;

						deleteDoctorById(conn, id);
						break;
					case 'u':
						printf("updating the doctor by id...\n");
						printf("id:\n");
						ret = acceptInt(&id);
						if(!ret) break;

						doctor d;
						acceptDoctor(&d);
						updateDoctorById(conn, id, d.first_name, d.last_name, d.special);
						break;
   					case 'q':
		        			printf("quitinq doctor menu...\n");
						break;
				}

			} while(choice!='q');
			break;

		case 'q':
			printf("Quiting menu...\n");
			break;

    	}

    } while(Mchoice!='q');


    // Baglantiyi kapat
    mysql_close(conn);
    
    return EXIT_SUCCESS;
} 
