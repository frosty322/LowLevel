#include <stdio.h>
#include <sys/mman.h>
#include "mem.h"

int main() {

    char *str = _malloc(5000);
    char *str2 = _malloc(228);

    scanf("%s", str);
    printf("Your string is: %s\n", str);
    puts("Memory state after writing to it:");

    puts("Freed first block:");
    _free(str);
    puts("Freed second block:");
    _free(str2);

    return 0;
}
