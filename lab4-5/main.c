#include <stdio.h>
#include <limits.h>
#include <errno.h>

#include "linked_list.h"

static __int32_t sum(__int32_t value, __int32_t acc)
{
    return value + acc;
}

static __int32_t max(__int32_t value, __int32_t max)
{
    return value > max ? value : max;
}

static __int32_t min(__int32_t value, __int32_t min)
{
    return value < min ? value : min;
}

static __int32_t mod(__int32_t value)
{
    return value > 0 ? value : -value;
}

static __int32_t twice(__int32_t value)
{
    return value * 2;
}

static void print_int(__int32_t value)
{
    printf("%d ", value);
}

static void println_int(int value) 
{
    printf("%d\n", value);
}

static __int32_t square(__int32_t value)
{
    __int32_t max = 1 << 16;

    if (value >= max || value <= -max) 
    {
        printf("Possible value overflow: square(%d), returning -1\n", value);
        return -1;
    }

    return value * value;
}

static __int32_t cube(__int32_t value)
{
    __int32_t max = 1 << 10;

    if (value >= max || value <= -max) 
    {        
        printf("Possible value overflow: cube(%d), returning -1\n", value);
        return -1;
    }

    return value * value * value;
}

static void functional_checks(struct l_list** list)
{
    __int32_t value;
    size_t index = 3;

    puts("--- functional_checks ---");

    printf("Lenght of list: %ld\n", list_lenght(*list));
    printf("List sum: %d\n", list_sum(*list));

    errno = 0;

    value = list_get(*list, index);

    if (errno) 
    {
        perror("Error at list_get");
    }
    else 
    {
        printf("Value of element at index %lu is '%d\n'", index, value);
    }
}

static void functionalOfHighOrder_checks(struct l_list** list)
{
    struct l_list* new_start = NULL;
    struct l_list** new_list;

    puts("--- functionalOfHighOrder_checks ---");

    puts("foreach test:");

    foreach(*list, print_int);
    puts("");

    foreach(*list, println_int);

    puts("map test:");

    new_start = map(*list, square);
    foreach(new_start, print_int);
    puts("");

    list_free(new_start);

    new_start = map(*list, cube);
    foreach(new_start, print_int);
    puts("");

    puts("foldl test:");

    printf("Sum = %d\n", foldl(*list, 0, sum));
    printf("Max = %d\n", foldl(*list, INT_MIN, max));
    printf("Min = %d\n", foldl(*list, INT_MAX, min));

    puts("map_mut test:");

    new_list = &new_start;
    map_mut(new_list, mod);
    foreach(*new_list, print_int);
    puts("");

    list_free(*new_list);

    puts("iterate test:");

    new_start = iterate(1, 10, twice);
    foreach(new_start, print_int);
    puts("");

    list_free(new_start);
}

static void file_checks(struct l_list** list)
{
    struct l_list* new_iter = NULL;        
    struct l_list** new_list = &new_iter;

    puts("--- file_checks ---");

    if (!save(*list, "list.txt")) 
    {
        perror("Error at save");
        return;
    }

    if (!load(new_list, "list.txt")) 
    {
        perror("Error at load");
        list_free(*new_list);
        return;
    }

    foreach(*list, print_int); puts("");
    foreach(*new_list, print_int); puts("");

    list_free(*new_list);

    puts("--- serialize/deserialize test ---");

    if (!serialize(*list, "list.bin")) 
    {
        perror("Error at serialize");
        return;
    }

    if (!deserialize(new_list, "list.bin")) 
    {
        perror("Error at deserialize");
        list_free(*new_list);
        return;
    }

    foreach(*list, print_int); 
    puts("");

    foreach(*new_list, print_int);
    puts("");

    list_free(*new_list);
}

 int main(void) 
 {
     __int32_t value;
    struct l_list* iter = NULL;
    struct l_list** list = &iter;

    for(;;)
    {
        if (!scanf("%d", &value)) break;

        list_add_front(list, value);
    }

    functional_checks(list);
    functionalOfHighOrder_checks(list);

    file_checks(list);

    list_free(*list);
    return 0;
}
