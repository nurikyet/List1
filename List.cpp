#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "List.h"
#include "logfile.h"
#include "Types.h"

#define ABOBA printf("\n<<I work in a FILE %s(%d) in %s>>\n", __FILE__, __LINE__, __func__)

int ListCtor(struct List* my_list, int capacity)
{
    assert(capacity);
    my_list->capacity = capacity;

    int array_size = (my_list->capacity) * sizeof(elem_t);

    my_list->data = (elem_t*) calloc(array_size, sizeof(char));
    ARRAY_OK(data);
    (my_list->data)[0] = POISON;

    my_list->next = (int*) calloc(array_size, sizeof(char));
    ARRAY_OK(next);
    for (int i = 0; i < capacity - 1; i++)
    {
        (my_list->next)[i] = i + 1;
    }
    //(my_list->next)[0] = FIRST_VALUE;

    my_list->pred = (int*) calloc(array_size, sizeof(char));
    ARRAY_OK(pred);
    memset(my_list->pred, VALUE, capacity*sizeof(int));
    //(my_list->pred)[0] = FIRST_VALUE;
    //(my_list->pred)[1] = FIRST_VALUE;

    my_list->head = 0;
    my_list->tail = 0;
    my_list->free = 1;

    LIST_DUMP(my_list);

    return (int)Error::NO_ERROR;
}

int ListDtor(struct List* my_list)
{
    free(my_list->data);
    free(my_list->next);
    free(my_list->pred);

    my_list->data = nullptr;
    my_list->next = nullptr;
    my_list->pred = nullptr;

    my_list->capacity  = 0;

    my_list->tail = 0;
    my_list->head = 0;
    my_list->free = 0;

    return (int)Error::NO_ERROR;
}

void ListDump(FILE* fp, struct List* my_list, const char* func, const char* file, const int line)
{
    assert(my_list);
    assert(func);
    assert(file);
    assert(my_list->data);
    assert(my_list->pred);
    assert(my_list->next);

    fprintf(fp, "List[%p] \"my_list\" from %s(%d) in function - %s.\n", my_list,  file, line, func);
    fprintf(fp, "{\n");
    fprintf(fp, "capacity = %d\n", my_list->capacity);

    ABOBA;

    char liner[LINE_LEN]= "";
    memset(liner, '-', (7*my_list->capacity - 2));

    for (int i = 0; i < my_list->capacity; i++)
    {
        fprintf(fp, "|%5d ", i);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%s\n", liner);

    for (int i = 0; i < my_list->capacity; i++)
    {
        fprintf(fp, "|%5d ", my_list->data[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%s\n", liner);

    for (int i = 0; i < my_list->capacity; i++)
    {
        fprintf(fp, "|%5d ", my_list->next[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%s\n", liner);

    for (int i = 0; i < my_list->capacity; i++)
    {
        fprintf(fp, "|%5d ", my_list->pred[i]);
    }
    fprintf(fp, "\n");
    fprintf(fp, "%s\n", liner);

    fprintf(fp, "<<HEAD %d>>\n", my_list->head);
    fprintf(fp, "<<TAIL %d>>\n", my_list->tail);
    fprintf(fp, "<<FREE %d>>\n", my_list->free);
    fprintf(fp, "\n");
}

int ListOk(FILE* fp, struct List* my_list)
{
    int result = 0;

    if (!my_list->capacity)
        {
        result |= (int)Error::ERROR_CAPACITY;
        }

    if (my_list->capacity < 0)
        {
        result |= (int)Error::ERROR_CAPACITY;
        }

    if (!my_list->data)
        {
        result |= (int)Error::ERROR_DATA;
        }

    if (my_list->next)
        {
        result |= (int)Error::ERROR_NEXT;
        }

    if (my_list->pred)
        {
        result |= (int)Error::ERROR_PRED;
        }

    if (my_list == nullptr)
        {
        result |= (int)Error::ERROR_STRUCT;
        }

    PrintError(fp, result);
    return result;
}

void PrintError(FILE* fp, int result)
{
    assert(fp);

    if ((result & (int)Error::ERROR_CAPACITY) != 0)
        fprintf(fp, "Capacity must be > 0\n");

    if ((result & (int)Error::ERROR_DATA) != 0)
        fprintf(fp, "address of data != nullptr\n");

    if ((result & (int)Error::ERROR_NEXT) != 0)
        fprintf(fp, "address of next != nullptr\n");

    if ((result & (int)Error::ERROR_PRED) != 0)
        fprintf(fp, "address of pred != nullptr\n");

    if ((result & (int)Error::ERROR_STRUCT) != 0)
        fprintf(fp, "address of struct != nullptr\n");
}

int ListPushAfter(struct List* my_list, int index, elem_t element)
{
    if (index < 0)
    {
        printf("The index must be greater than zero, and you entered - %d\n", index);
    }

    else if (index > my_list->capacity)
    {
        printf("Index cannot be greater than capacity, but you entered index - %d, capacity - %d\n", index, my_list->capacity);
    }

    else if (index == 0)
    {
        return ListPushFirst(my_list, element);
        ABOBA;
    }

    else if (index == my_list->tail)
    {
        return ListPushAfterTail(my_list, element);
    }

    ABOBA;

    int free_index = my_list->free;
    my_list->free  = my_list->next[free_index];

    ABOBA;

    my_list->data[free_index]                = element;
    my_list->next[free_index]                = my_list->next[index];
    my_list->pred[my_list->next[free_index]] = free_index;
    my_list->next[index]                     = free_index;
    my_list->pred[free_index]                = index;

    return free_index;
}

int ListPushFirst(struct List* my_list, elem_t element)
{
    assert(my_list);

    ABOBA;
    printf("YA VOSHELLLLL!!!!!!!!!!!!!!!!!!\n");

    int free_index = my_list->free;
    my_list->free  = my_list->next[free_index];

    my_list->data[free_index]                = element;
    my_list->next[free_index]                = my_list->head;
    my_list->pred[free_index]                = FIRST_VALUE;

    if (my_list->head != 0)
    {
        my_list->pred[my_list->head] = free_index;
    }

    if (my_list->tail == 0)
    {
        my_list->tail = free_index;
    }
    else
    {
    //my_list->tail = ;
    }

    my_list->head = free_index;

    LIST_DUMP(my_list);

    return free_index;
}

int ListPushAfterTail(struct List* my_list, elem_t element)
{
    assert(my_list);

    int free_index = my_list->free;
    my_list->free  = my_list->next[free_index];

    my_list->data[free_index] = element;
    my_list->next[free_index] = 0;

    my_list->pred[free_index]    = my_list->tail;
    my_list->next[my_list->tail] = free_index;

    my_list->tail = free_index;

    return free_index;
}

int ListPop(struct List* my_list, int index)
{
    assert(my_list);

    if (index <= 0)
    {
        printf("The index must be greater than zero, and you entered - %d\n", index);
    }

    else if (index > my_list->capacity)
    {
        printf("Index cannot be greater than capacity, but you entered index - %d, capacity - %d\n", index, my_list->capacity);
    }

    else if (index == my_list->head)
    {
        return ListPopFirstElement(my_list);
    }

    my_list->next[my_list->pred[index]] = my_list->next[index];
    my_list->pred[my_list->next[index]] = my_list->pred[index];

    my_list->data[index] = POISON;
    my_list->pred[index] = VALUE;
    my_list->next[index] = my_list->free;

    return (int)Error::NO_ERROR;
}

int ListPopFirstElement(struct List* my_list)
{
    assert(my_list);

    my_list->pred[my_list->next[my_list->head]] = my_list->pred[my_list->head];

    my_list->data[my_list->head] = POISON;
    my_list->pred[my_list->head] = VALUE;
    my_list->next[my_list->head] = my_list->free;

    my_list->head = my_list->next[my_list->head];
    return (int)Error::NO_ERROR;
}
