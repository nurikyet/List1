#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include "Types.h"
#include "logfile.h"

#define LIST_CONSTRUCT(nick, quantity) struct List nick = {};          \
                                       ListCtor(&nick, quantity);      \
                                       nick.name = #nick;

#define ARRAY_OK(arr) if (arr == nullptr)                                                                      \
                      {                                                                                        \
                          my_list->capacity = 0;                                                               \
                          fprintf(LOG_FILE, "Function %s: not enough memory to allocate for " #arr, __func__); \
                          return (int)Error::ERROR_MEMORY;                                                     \
                      }

#define LIST_DUMP(my_list){ ListDump(LOG_FILE, my_list, __func__, __FILE__, __LINE__);                         \
                            GraphicDump(my_list);                                                              \
                          }

#define VERIFY(my_list) {int error = ListOk(LOG_FILE, my_list); printf("error in verify is %d\n", error);      \
                        if (error != 0)                                                                        \
                        {                                                                                      \
                            PrintError(stderr, error);                                                         \
                            LIST_DUMP(my_list);                                                                \
                            return error;                                                                      \
                        }                                                                                      \
                        }



const int POISON      = -777;
const int FIRST_VALUE =    0;
const int VALUE       =   -1;
const int LINE_LEN    =  100;

struct List
{
    const char* name;
    int capacity;
    int size;

    elem_t* data;
    int* next;
    int* pred;

    int free;
};

enum class Error
{
    NO_ERROR                               = 0,
    ERROR_CAPACITY                         = 1 << 0,
    ERROR_LEN                              = 1 << 1,
    ERROR_DATA                             = 1 << 2,
    ERROR_MEMORY                           = 1 << 3,
    ERROR_NEXT                             = 1 << 4,
    ERROR_PRED                             = 1 << 5,
    ERROR_STRUCT                           = 1 << 6,
    ERROR_SCANF                            = 1 << 7,
    ERROR_ZERO_ELEMENT                     = 1 << 8,
    ERROR_DIFFERENCE_BETWEEN_NEXT_AND_PRED = 1 << 9,
};

int ListCtor(struct List* my_list, int capacity);
int ListDtor(struct List* my_list);
int ListOk(FILE* fp, struct List* my_list);
int ListInsertAfter(struct List* my_list, int index, elem_t element);
int ListDelete(struct List* my_list, int index);
int ListVerify(struct List* my_list, int result);
int GetTail(struct List* my_list);
int GetHead(struct List* my_list);

void ListDump(FILE* fp, struct List* my_list, const char* func, const char* file, const int line);
void PrintError(FILE* fp, int result);
void GraphicDump(struct List* my_list);
void CreateNewPicture();
#endif // LIST_H_INCLUDED
