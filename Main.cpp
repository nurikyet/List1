#include <stdio.h>

#include "List.h"
#include "logfile.h"

int main(/*const int argc, const char* argv[]*/)
{
    OpenLog("logfile.log");
    LIST_CONSTRUCT(my_list, 15);

    ListPushAfter(&my_list, 0, 10);

    LIST_DUMP(&my_list);

    ListPushAfter(&my_list, 3, 40);

    LIST_DUMP(&my_list);

    ListPushAfter(&my_list, 4, 50);

    ListDtor(&my_list);

    return (int)Error::NO_ERROR;
}
