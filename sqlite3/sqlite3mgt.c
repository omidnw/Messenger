/*------------------------------*
 *				                *
 *	Sqlite3 Management.	        *
 *	omid reza keshtkar.	        *
 *				                *
 *------------------------------*/

#include <time.h> // get time for log file.
#include <stdio.h> // standard library input & output.
#include <stdarg.h>
#include <stdlib.h>
#include <string.h> // standard library string.
#include "sqlite3.h" // sqlite3 library.
#include <sys/stat.h> // check file and folder exists
#include "sqlite3mgt.h"
#include "../toolkit/main.h"


#ifndef Sqlite3_Management_C
#define Sqlite3_Management_C

int fileexists(const char* filename)
{
    file_exists(filename);
}
int folderexists(const char* foldername)
{
    folder_exists(foldername);
}

#define LOG_TO_STDERR
// logs to the stream and stdout/stderr
#define logprintf(...) _logprintf(__FILE__, __LINE__, __VA_ARGS__)

void _logprintf(const char *fname, int line, FILE *stream, const char *format, ...) {
    va_list args;

    va_start(args, format);
    // add this line to print log in console : fprintf(stream, "LOG: %s:%d :", fname, line);
    vfprintf(stream, format, args);
    va_end(args);
#if defined(LOG_TO_STDERR)
    va_start(args, format);
    // add this line to print log in console : fprintf(stderr, "LOG: %s:%d :", fname, line);
    vfprintf(stderr, format, args);
    va_end(args);
#elif defined(LOG_TO_STDOUT)
    va_start(args, format);
    // add this line to print log in console : fprintf(stdout, "LOG: %s:%d :", fname, line);
    vfprintf(stdout ,format, args);
    va_end(args);
#endif
}

int sqlexecute(struct Sqlite3_MGT SqliteSTC)
{
    printf("Coming Soon!");
    return 0;
}

int createdatabase(struct Sqlite3_MGT SqliteSTC)
{
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], databaseaddress[255], sqlarray[1024], *err_msg = 0, *foldername = "logs", *databasefoldername = "database";
    snprintf(databaseaddress, sizeof(databaseaddress), "./%s/%s", databasefoldername, SqliteSTC.databasename);
    int rc, exist, existsdatabase = folderexists(databasefoldername);
    if(existsdatabase)
    {
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    else
    {
        mkdir("database", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if (exist)
    {
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Create DataBase Function.) - %s\n", ctime(&t));
            sqlite3_close(db);
            return 0;
        }
    }
    else
    {
        mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Create DataBase Function.) - %s\n", ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
}

int createtable(struct Sqlite3_MGT SqliteSTC)
{
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], databaseaddress[255], sqlarray[1024], *err_msg = 0, *foldername = "logs", *databasefoldername = "database";
    snprintf(databaseaddress, sizeof(databaseaddress), "./%s/%s", databasefoldername, SqliteSTC.databasename);
    int rc, exist, existsdatabase = folderexists(databasefoldername);
    if(existsdatabase)
    {
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    else
    {
        mkdir("database", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if(exist)
    {
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Create Table Function.) - %s\n", ctime(&t));
            snprintf(sqlarray, sizeof(sqlarray), "CREATE TABLE %s(%s);", SqliteSTC.tablename, SqliteSTC.columnname);
            logprintf(log, "SQL command : %s - %s\n", sqlarray, ctime(&t));
            rc = sqlite3_exec(db, sqlarray, 0, 0, &err_msg);
        }
        if(rc != SQLITE_OK)
        {
            logprintf(log, "SQL error: %s - %s\n", err_msg, ctime(&t));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
    else
    {
        mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Create Table Function.) - %s\n", ctime(&t));
            snprintf(sqlarray, sizeof(sqlarray), "CREATE TABLE %s(%s);", SqliteSTC.tablename, SqliteSTC.columnname);
            logprintf(log, "SQL command : %s - %s\n", sqlarray, ctime(&t));
            rc = sqlite3_exec(db, sqlarray, 0, 0, &err_msg);
        }
        if(rc != SQLITE_OK)
        {
            logprintf(log, "SQL error: %s - %s\n", err_msg, ctime(&t));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
}

int deletetable(struct Sqlite3_MGT SqliteSTC) {
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], databaseaddress[255], sqlarray[1024], *err_msg = 0, *foldername = "logs", *databasefoldername = "database";
    snprintf(databaseaddress, sizeof(databaseaddress), "./%s/%s", databasefoldername, SqliteSTC.databasename);
    int rc, exist, existsdatabase = folderexists(databasefoldername);
    if(existsdatabase)
    {
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    else
    {
        mkdir("database", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if(exist)
    {
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Delete Table Function.) - %s\n", ctime(&t));
            snprintf(sqlarray, sizeof(sqlarray), "DROP TABLE IF EXISTS %s;", SqliteSTC.tablename);
            logprintf(log, "SQL command : %s - %s\n", sqlarray, ctime(&t));
            rc = sqlite3_exec(db, sqlarray, 0, 0, &err_msg);
        }
        if(rc != SQLITE_OK)
        {
            logprintf(log, "SQL error: %s - %s\n", err_msg, ctime(&t));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
    else
    {
        mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Delete Table Function.) - %s\n", ctime(&t));
            snprintf(sqlarray, sizeof(sqlarray), "DROP TABLE IF EXISTS %s;", SqliteSTC.tablename);
            logprintf(log, "SQL command : %s - %s\n", sqlarray, ctime(&t));
            rc = sqlite3_exec(db, sqlarray, 0, 0, &err_msg);
        }
        if(rc != SQLITE_OK)
        {
            logprintf(log, "SQL error: %s - %s\n", err_msg, ctime(&t));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
}

int insertvalue(struct Sqlite3_MGT SqliteSTC) {
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], databaseaddress[255], sqlarray[1024], *err_msg = 0, *foldername = "logs", *databasefoldername = "database";
    snprintf(databaseaddress, sizeof(databaseaddress), "./%s/%s", databasefoldername, SqliteSTC.databasename);
    int rc, exist, existsdatabase = folderexists(databasefoldername);
    if(existsdatabase)
    {
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    else
    {
        mkdir("database", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        rc = sqlite3_open(databaseaddress, &db);
        exist = folderexists(foldername);
    }
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if(exist)
    {
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK)
        {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Insert Value Function.) - %s\n", ctime(&t));
            snprintf(sqlarray, sizeof(sqlarray), "INSERT INTO %s(%s) VALUES(%s);", SqliteSTC.tablename,
                     SqliteSTC.columnname, SqliteSTC.value);
            logprintf(log, "SQL command : %s - %s\n", sqlarray, ctime(&t));
            rc = sqlite3_exec(db, sqlarray, 0, 0, &err_msg);
        }
        if(rc != SQLITE_OK)
        {
            logprintf(log, "SQL error: %s - %s\n", err_msg, ctime(&t));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
    else
    {
        mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        log = fopen(filename, "a+");
        if(rc != SQLITE_OK) {
            logprintf(log, "Cannot open database: %s - %s\n", sqlite3_errmsg(db), ctime(&t));
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            logprintf(log, "Database opened successfully. (Insert Value Function.) - %s\n", ctime(&t));
            snprintf(sqlarray, sizeof(sqlarray), "INSERT INTO %s VALUES(%s);", SqliteSTC.tablename, SqliteSTC.value);
            logprintf(log, "SQL command : %s - %s\n", sqlarray, ctime(&t));
            rc = sqlite3_exec(db, sqlarray, 0, 0, &err_msg);
        }
        if(rc != SQLITE_OK)
        {
            logprintf(log, "SQL error: %s - %s\n", err_msg, ctime(&t));
            sqlite3_free(err_msg);
            sqlite3_close(db);
            fclose(log);
            return 1;
        }
        else
        {
            sqlite3_close(db);
            fclose(log);
            return 0;
        }
    }
}

int readvalue(struct Sqlite3_MGT SqliteSTC)
{
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], sqlarray[1024], *err_msg = 0, *foldername = "logs";
    int rc = sqlite3_open(SqliteSTC.databasename, &db), exist = folderexists(foldername);
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if(rc != SQLITE_OK)
    {

        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    char *sql = "DROP TABLE IF EXISTS Cars;"
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK)
    {

        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }

    sqlite3_close(db);
    return 0;
}

int updatevalue(struct Sqlite3_MGT SqliteSTC)
{
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], sqlarray[1024], *err_msg = 0, *foldername = "logs";
    int rc = sqlite3_open(SqliteSTC.databasename, &db), exist = folderexists(foldername);
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    char *sql = "DROP TABLE IF EXISTS Cars;"
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return 1;
    }
    sqlite3_close(db);
    return 0;
}

int deletevalue(struct Sqlite3_MGT SqliteSTC)
{
    time_t t;
    FILE *log;
    sqlite3 *db;
    char filename[255], sqlarray[1024], *err_msg = 0, *foldername = "logs";
    int rc = sqlite3_open(SqliteSTC.databasename, &db), exist = folderexists(foldername);
    time(&t);
    snprintf(filename, sizeof(filename), "./logs/%s.txt", SqliteSTC.databasename);
    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    char *sql = "DROP TABLE IF EXISTS Cars;"
                "CREATE TABLE Cars(Id INT, Name TEXT, Price INT);"
                "INSERT INTO Cars VALUES(1, 'Audi', 52642);"
                "INSERT INTO Cars VALUES(2, 'Mercedes', 57127);"
                "INSERT INTO Cars VALUES(3, 'Skoda', 9000);"
                "INSERT INTO Cars VALUES(4, 'Volvo', 29000);"
                "INSERT INTO Cars VALUES(5, 'Bentley', 350000);"
                "INSERT INTO Cars VALUES(6, 'Citroen', 21000);"
                "INSERT INTO Cars VALUES(7, 'Hummer', 41400);"
                "INSERT INTO Cars VALUES(8, 'Volkswagen', 21600);";

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);

    if(rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }

    sqlite3_close(db);
    return 0;
}

void sqlite3mgt(struct Sqlite3_MGT SqliteSTC)
{
    if(strcmp(SqliteSTC.condition, "0") == 0) // Sqlite Shell Coming Soon!
    {
        sqlexecute(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "1") == 0) // Create DataBase.
    {
        createdatabase(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "2") == 0) // Create Table.
    {
        createtable(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "3") == 0) // Delete Table.
    {
        deletetable(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "4") == 0) // Insert Value.
    {
        insertvalue(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "5") == 0) // Update Value.
    {
        updatevalue(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "6") == 0) // Read Value.
    {
        readvalue(SqliteSTC);
    }
    else if(strcmp(SqliteSTC.condition, "7") == 0) // Delete Value.
    {
        deletevalue(SqliteSTC);
    }
    else
    {
        printf("0=Shell (Coming Soon.)\n1=Create DataBase.\n2=Create Table.\n3=Delete Table.\n4=Insert Value.\n5=Update Value.\n6=Read Value.\n7=Delete Value.\n");
        printf("condition not found!");
    }
    //printf("\n test : %s\n sql", SqliteSTC.databasename);
}
#endif