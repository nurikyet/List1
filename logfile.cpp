#include <stdlib.h>
#include <string.h>

#include "logfile.h"

FILE* LOG_FILE = stderr;

void CloseLogFile()
{
    fprintf (LOG_FILE, "CLOSE\n");
    fprintf (LOG_FILE, "\\----------------------------------------------------\\\n");
    fclose (LOG_FILE);
}

void OpenLog (const char* FILE_NAME)
{
    char* file_name = strdup(FILE_NAME);

    LOG_FILE = fopen(file_name, "w");

    if (LOG_FILE == NULL)
    {
        printf("your log_file is NULL\n");
        LOG_FILE = stderr;
    }

    fprintf (LOG_FILE, "\\----------------------------------------------------\\\n");
    fprintf (LOG_FILE, "OPEN\n");
    atexit (CloseLogFile);
}

/*
void OpenLogFile(const char* FILE_NAME)
{
    char* file_name = strdup(FILE_NAME);

    LOG_SITE = fopen(file_name, "w");

    if (LOG_FILE == NULL)
    {
        printf("your log_file is NULL\n");
        LOG_FILE = stderr;
    }

    fprintf(LOG_FILE, "\\-----------------------------------------------------\\\n");
}
*/

