#include <stdio.h>

#include "List.h"
#include "logfile.h"

int main()
{
    OpenLog("logfile.html");

    LIST_CONSTRUCT(my_list, 15);

    ListInsertAfter(&my_list, 0, 10);

    ListInsertAfter(&my_list, 1, 40);

    LIST_DUMP(&my_list);

    ListInsertAfter(&my_list, 2, 50);

    LIST_DUMP(&my_list);

    ListDelete(&my_list, 3);

    LIST_DUMP(&my_list);

    ListInsertAfter(&my_list, 2, 60);

    LIST_DUMP(&my_list);

    printf("logical index is %d\n", FindLogicalIndex(&my_list, 2));

    printf("Physical index is %d\n", FindPhysicalIndex(&my_list, 2));

    ListSort(&my_list);

    printf("index is %d\n", ListFind(&my_list, 60));

    LIST_DUMP(&my_list);

    ListDtor(&my_list);

    return (int)Error::NO_ERROR;
}
