#ifndef PATIENT
#define PATIENT

#include "inout.h"
#include "dbops.h"
enum body_class{
	UNDER,
	NORMAL,
	HIGH,
	OBESE
};

typedef struct patient{
        int id;
	long birth_date;
        char first_name[128];
        char last_name[128];
        char gender;
	float height;
	float weight;
} patient;

int acceptPatient(patient *p);
int insertPatient(MYSQL *conn);
int Prow2Patient(MYSQL_ROW row, patient *p);
int selectAllPatients(MYSQL *conn);

int findPatientById(int id, MYSQL *conn, patient *adrOfp);
int selectPatients(long birth_date, char *first_name, char *last_name, char gender, float height, float weight, MYSQL *conn);

int deletePatById(MYSQL *conn, int id);

int updatePatById(MYSQL *conn, int id, patient newPat);
 
//return MYSQL_RES
int findPatients(long birth_date, char *first_name, char *last_name, char gender, float height, float weight, MYSQL *conn, MYSQL_RES **adrOfRes); 
int pRes2PArr(MYSQL *conn, MYSQL_RES *res, patient **adrOfp_list);

int p2Arr(char *filename, patient **p_list);
void printPArr(patient *p_list, int size);
void printPatient(patient p);
int getPatientById(int id, patient *p_arr, int size, patient **adrOfp);
void printPArr(patient *p_arr, int size);

enum body_class body_mass_index(float height, float weight);
int findBMIofPat(int id, MYSQL *conn);

int deneme(MYSQL *conn);
#endif
