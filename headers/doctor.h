#ifndef DOCTOR
#define DOCTOR

#include "dbops.h"
#include "inout.h"

typedef struct doctor{
        int id;
        char first_name[128];
        char last_name[128];
        char special[128];
} doctor;

//getBy olanlar arrayden ceksin, findBy olanlar dbden.

//get doctor from user
int acceptDoctor(doctor *d);

int insertDoctor(MYSQL *conn);

int deleteDoctorById(MYSQL *conn, int id);

int updateDoctorById(MYSQL *conn, int id, char *first_name, char *last_name, char *special);

int selectAllDoctors(MYSQL *conn);

void printDoctor(doctor d);
void printDArr(doctor *d_arr, int size);

int drow2Doctor(MYSQL_ROW row, doctor *d);

int dRes2DArr(MYSQL *conn, MYSQL_RES *res, doctor **adrOfd_list);

int findDoctorById(int id, MYSQL *conn);

int findDoctors(char *first_name, char *last_name, char *special, MYSQL *conn, MYSQL_RES **adrOfRes); //return MYSQL_RES

int selectDoctors(char *first_name, char *last_name, char *special, MYSQL *conn);

int getDoctorById(int id, doctor *d_arr, int size, doctor **adrOfd);
#endif
