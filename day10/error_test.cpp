#include "stdio.h"
#include "string.h"

#pragma GCC diagnostic ignored "-Wwrite-strings" //  error: ISO C++ forbids converting a string constant to 'char*' [-Werror=write-strings]
                                                 // 7 |     char *text =  "texto";

int main()
{

    const char *text =  "texto";
    printf(" strlen(text) = %lld \n", strlen(text));

    return 0;
}