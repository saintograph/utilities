#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>


clock_t start, end;
double cpu_time_used;
MYSQL *con;

long getMemoryUsage() 
{
  struct rusage usage;
  if (0 == getrusage(RUSAGE_SELF, &usage)) {
    return usage.ru_maxrss;
  } else {
    return 0;
  }
}


void error()
{
    printf("errno = %d %s\n", mysql_errno(con), mysql_error(con));
    mysql_close(con);
    exit(1);
}


const char* main(int argc, char* argv[])
{
    int i, ncols;
    MYSQL_RES *sqlQueryResult;
    con = mysql_init(NULL);
    MYSQL_ROW row;
    char buf[1024];
    char* result = (char *) malloc(1);

    if (mysql_real_connect(
        con,
        "ip", // argv[1]
        "username", // argv[2]
        "password", // argv[3]
        "database", // argv[4]
        3306,
        NULL,
        0
    ))
    {
        int counter;

        char queryHead[512] = "ENTER QUERY HERE";
        char* queryString = (char *) malloc(strlen(queryHead) + strlen(argv[1]) + strlen(queryTail) + 1);
        strcat(queryString, queryHead);
        strcat(queryString, argv[1]);
        strcat(queryString, queryTail);
        
        mysql_query(con, queryString);
        sqlQueryResult = mysql_store_result(con);
        ncols = mysql_num_fields(sqlQueryResult);
        // printf("Column numbers = %d\n", ncols);
        while(row = mysql_fetch_row(sqlQueryResult)) {
            for (i = 0; i < ncols; i++) {
                // printf("%-8s ", row[i]);
                // printf("\n");
                result = (char *) realloc(result, strlen(result) + strlen(row[i]) + 1);
                strcat(result, row[i]);
                printf(result);
            }
        }
        return result;
        // row = mysql_fetch_row(result);
        // return result;
        // printf(result);
    }
    else
    {
        error();
    }
    free(queryString);
    // free(result);
    mysql_close(con);
    return result;
}
