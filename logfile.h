#ifndef LOGFILE_H_INCLUDED
#define LOGFILE_H_INCLUDED

#include <stdio.h>

extern FILE* LOG_FILE;
static const int MAX_LEN = 256;

//! @brief file creation
//!
//! a file is created and opened
void OpenLog (const char* FILE_NAME);

//! @brief closing a file
//!
//! closes the file
void CloseLogFile ();

#endif // LOGFILE_H_INCLUDED
