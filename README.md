
# Hospital Management System (MySQL C Connector)

This is a basic Hospital Management System project implemented in C using the MySQL C Connector. The system allows for adding, deleting, and querying doctor and patient datas in a hospital database.

## Requirements

To compile and run this project, you need:

- GCC compiler
- MySQL C Connector (6.1 or later)
- MySQL server

##MySQL Setup

- Make sure MySQL server is installed and running.
- Create a database and fill the related variables in 'hospitalSys.c' in src directory.
- Create tables 'doctor' and 'patient' according to structures in header files 'doctor.h' and 'patient.h'.

 ##Compile And Run
 - Create a target folder.
 - Ensure that the mysql.h and MySQL library paths are correctly set in the Makefile
 - Execute make command to compile.
 - Execute ./target/main command to run.


