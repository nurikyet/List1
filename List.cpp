#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#include "List.h"
#include "logfile.h"
#include "Types.h"

#define ABOBA printf("\n<<I work in a FILE %s(%d) in %s>>\n", __FILE__, __LINE__, __func__)

char num[] = "1";

int ListCtor(struct List* my_list, int capacity)
{
    assert(capacity);
    my_list->capacity = capacity;

    int array_size = (my_list->capacity) * sizeof(elem_t);

    my_list->data = (elem_t*) calloc(array_size, sizeof(char));
    ARRAY_OK(my_list->data);
    (my_list->data)[0] = POISON;

    my_list->next = (int*) calloc(array_size, sizeof(char));
    ARRAY_OK(my_list->next);
    for (int i = 0; i < capacity - 1; i++)
    {
        (my_list->next)[i] = i + 1;
    }

    my_list->pred = (int*) calloc(array_size, sizeof(char));
    ARRAY_OK(my_list->pred);
    memset(my_list->pred, VALUE, capacity*sizeof(int));
    my_list->pred[0] = 1;

    my_list->head = 1;
    my_list->tail = 1;
    my_list->free = 1;

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

    my_list->capacity  = POISON;

    my_list->tail = POISON;
    my_list->head = POISON;
    my_list->free = POISON;

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

    fprintf(fp, "<pre>List[%p] \"my_list\" from %s(%d) in function - %s.\n", my_list,  file, line, func);
    fprintf(fp, "{\n");
    fprintf(fp, "capacity = %d\n", my_list->capacity);

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

    fprintf(fp, "HEAD %d\n", my_list->head);
    fprintf(fp, "TAIL %d\n", my_list->tail);
    fprintf(fp, "FREE %d\n", my_list->free);
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

    if (my_list->data == nullptr)
    {
        result |= (int)Error::ERROR_MEMORY;
    }

    int new_result = ListVerify(my_list, result);

    PrintError(fp, new_result);
    return new_result;
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

    if ((result & (int)Error::ERROR_ZERO_ELEMENT) != 0)
        fprintf(fp, "The first element is not equal to the poisonous value\n");

    if ((result & (int)Error::ERROR_DIFFERENCE_BETWEEN_NEXT_AND_PRED) != 0)
        fprintf(fp, "The order in the arrays next and pred is different\n");
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
        return (int)Error::ERROR_MEMORY;
    }

    int free_index = my_list->free;
    my_list->free  = my_list->next[free_index];

    if (index == GetTail(my_list))
    {
        my_list->pred[0]          = free_index;
        my_list->next[free_index] = 0;
        //my_list->pred[my_list->next[my_list->tail]] = my_list->tail;
    }

    else
    {
        my_list->next[free_index]                = my_list->next[index]; //цикл
        my_list->pred[my_list->next[free_index]] = free_index;
    }

    my_list->data[free_index]                = element;
    my_list->next[index]                     = free_index;
    my_list->pred[free_index]                = index;

    return free_index;
}

int ListPushFirst(struct List* my_list, elem_t element)
{
    assert(my_list);

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

    my_list->pred[my_list->next[my_list->tail]] = my_list->tail;

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

    else if (my_list->pred[index] == -1)
    {
        printf("The element with index [%d] has already been deleted\n", index);
    }

    my_list->next[my_list->pred[index]] = my_list->next[index];
    my_list->pred[my_list->next[index]] = my_list->pred[index];


    if (index == GetHead(my_list))
    {
        my_list->next[0] = my_list->next[index];
    }

    if (index == my_list->tail)
    {
        my_list->pred[0] = my_list->pred[index];
    }

    my_list->data[index] = POISON;
    my_list->pred[index] = my_list->pred[my_list->free];
    my_list->next[index] = my_list->free;

    my_list->free = index;

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

int ListVerify(struct List* my_list, int result)
{
    assert(my_list);

    if (my_list->data[0]!= POISON)
    {
        result |= (int)Error::ERROR_ZERO_ELEMENT;
    }

    int new_next[LINE_LEN] = {};
    int new_pred[LINE_LEN] = {};

    int count_next = 0;
    int actual_next = my_list->head;

    while (actual_next != 0)
    {
        new_next[count_next] = my_list->data[count_next];
        actual_next = my_list->next[actual_next];
        count_next ++;
    }

    int count_pred = 0;
    int actual_pred = my_list->tail;

    while (actual_pred != 0)
    {
        new_pred[count_pred] = my_list->data[actual_pred];
        actual_pred = my_list->pred[actual_pred];
        count_pred ++;
    }

    if (count_next != count_pred)
    {
        result |= (int)Error::ERROR_DIFFERENCE_BETWEEN_NEXT_AND_PRED;
    }

    else
    {
        for (int i = 0; i < count_pred; i++)
        {
            if (new_next[i] != new_pred[count_pred - 1 - i])
                result |= (int)Error::ERROR_DIFFERENCE_BETWEEN_NEXT_AND_PRED;
        }
    }

    return result;
}

void GraphicDump(struct List* my_list)
{
    FILE* fp = fopen("list.dot", "wb");
    fprintf(fp, "digraph G{\n");
    fprintf(fp, "  rankdir=LR;\n");

    int size = 1;
    for (int i = 1; i < my_list->capacity; i++)
    {
        if (my_list->pred[i] != VALUE)
        size++;
    }

    fprintf(fp, "  subgraph cluster0 {\n");
    fprintf(fp, "      node [color = \"#4682B4\", fontsize=14];\n");
    //fprintf(fp, "      style=filled;\n");
    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "      %d [shape=record, style=\"filled\", fillcolor=\"#AFEEEE\", label=\" %d | data: %d| <f%d> next: %d| <f%d%d> pred: %d\"];\n", i, i, my_list->data[i], i, my_list->next[i], i, i, my_list->pred[i]);
    }
    fprintf(fp, "      color=blue;\n");
    fprintf(fp, "  }\n");

    fprintf(fp, "  subgraph cluster1 {\n");
    fprintf(fp, "      node [color = \"#800080\", fontsize=14];\n");
    for (int i = size; i < my_list->capacity; i++)
    {
        fprintf(fp, "      %d [shape=record, style=\"filled\", fillcolor=\"#DDA0DD\", label=\" %d | FREE | <f%d> next: %d| pred: %d\"];\n", i, i, i, my_list->next[i], my_list->pred[i]);
    }
    fprintf(fp, "      color=purple;\n");
    fprintf(fp, "  }\n");

    fprintf(fp, "      free [shape=tripleoctagon, style=\"filled\", fillcolor=\"#F08080\", label=\" FREE: %d\"];\n", my_list->free);
    fprintf(fp, "      head [shape=tripleoctagon, style=\"filled\", fillcolor=\"#F08080\", label=\" HEAD: %d\"];\n", GetHead(my_list));
    fprintf(fp, "      tail [shape=tripleoctagon, style=\"filled\", fillcolor=\"#F08080\", label=\" TAIL: %d\"];\n", GetTail(my_list));


    for (int i = 0; i < my_list->capacity - 1; i++)
    {
        fprintf(fp, "  %d -> %d[color=\"white\", weight = 1000]\n", i, i + 1);   //, constraint=false
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%d:<f%d> -> %d:<f%d>[color=\"red\", weight = 0]\n", i, i, my_list->next[i], my_list->next[i]);
    }

    for (int i = size; i < my_list->capacity; i++)
    {
        fprintf(fp, "%d:<f%d> -> %d:<f%d>[style=\"dashed\", color=\"red\", weight = 0]\n", i, i, my_list->next[i], my_list->next[i]);
    }

    for (int i = 0; i < size; i++)
    {
        fprintf(fp, "%d:<f%d%d> -> %d:<f%d%d>[color=\"#3CB371\", weight = 0]\n", i, i, i, my_list->pred[i], my_list->pred[i], my_list->pred[i]);
    }

    fprintf(fp, "free -> %d\n", my_list->free);
    fprintf(fp, "head -> %d\n", GetHead(my_list));
    fprintf(fp, "tail -> %d\n", GetTail(my_list));

    fprintf(fp, "}\n");

    fclose(fp);

    CreateNewPicture();
}

void CreateNewPicture()
{

    char command0[1000] = "dot list.dot -T png -o";
    char command1[1000] = "C:\\Users\\User\\Documents\\дединский\\List\\list";
    char command2[] = ".png";

    strcat(command1, num);
    strcat(command1, command2);

    strcat(command0, command1);
    system(command0);

    fprintf(LOG_FILE, "<img src = \"%s\">\n", command1);   //height=\"400\", width=\"600\"

    snprintf(num, 2, "%d", (1 + atoi(num)));

}

int GetTail(struct List* my_list)
{
    assert(my_list);
    return (my_list->pred[0]);
}

int GetHead(struct List* my_list)
{
    assert(my_list);
    return (my_list->next[0]);
}
