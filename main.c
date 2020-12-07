#include <sqlite3.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

// Colori
#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define RESET   "\x1b[0m"

int count = 0;
int summ = 0;
int plus = 0;
int minus = 0;

unsigned int sleep(unsigned int seconds);
void exit();
//void d_find();
void add();
void view();
int callback(void *, int, char **, char **);

int main(void) {
    int choose;

    printf(MAGENTA "Hi! This is your moneybox. \n\n" RESET);
    printf("Choose: \n");
    printf(GREEN "[1] View data and stacs; \n");
    printf(CYAN "[2] Add a record; \n\n" RESET);

    printf(">>> ");
    scanf("%d", &choose);

    if (choose == 1) {
        view();
    }
    else if (choose == 2) {
        add();
    }
    else {
        exit(0);
    }
}

void view() {
    system("clear");

    sqlite3 *db;
    char *err_msg = 0;
    int rc = sqlite3_open("data.db", &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(0);
    }
    
    char *sql = "SELECT budget, date, note FROM money";

    printf("Lasts: \n");
    sleep(1);
        
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        exit(0);
    } 
    
    sqlite3_close(db);

    printf("\n\n There are %d records.", count);
    printf(BLUE "\n\n You have %d incomes.", plus);
    printf(GREEN "\n\n You have %d exits", minus);
    
    if (summ < 0) {
        printf (RED "\n\n You are in NEGATIVE! \n\n" RESET);
    }
    else {
        printf(RESET "\n\n Now you have %d euro at your disposition.\n\n", summ);
    }
}

void add() {
    sqlite3 *db;
    char *err_msg = 0;

    char note[1000], date[1000], insert[1000];

    int rc = sqlite3_open("data.db", &db);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);

        exit(0);
    }

    system("clear");
    
    printf("Add a record. \n\n");

    printf("Enter the " BLUE "budget variation" RESET " (if negative use the minus): \n");
    printf(">>> ");
    scanf("%s", insert);

    printf("Enter some " GREEN "notes" RESET " (blank mean NONE): \n");
    printf(">>> ");
    scanf("%s", note);

    printf(YELLOW "Date " RESET "(blank mean NONE): \n");
    printf(">>> ");
    scanf("%s", date); 


    char sql[5000];
    sql[0] = '\0';
    strcat(sql,"INSERT INTO money(budget, note, date) VALUES('");
    strcat(sql, insert);
    strcat(sql, "', '");
    strcat(sql, note);
    strcat(sql, "', '");
    strcat(sql, date);
    strcat(sql, "');");

    //Debug:
    //printf("SQL: %s", sql);

    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "SQL error: %s\n", err_msg);        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        exit(0);
    } 
    sqlite3_close(db);

    printf(BLUE "\nData entered correctly.\n\n" RESET);
}

int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    NotUsed = 0;

    for (int i = 0; i < argc; i++) {        
        printf(CYAN "\n %s:  %s", azColName[i], argv[i]);
    }

    int tmp = atoi(argv[0]);
    summ = summ + tmp;
    if (tmp < 0) {
        minus++;
    }
    else if (tmp > 0) {
        plus++;
    }

    count++;

    printf(RESET "\n");
    return 0;
}