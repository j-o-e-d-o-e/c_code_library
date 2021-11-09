#include <stdio.h>
#include <string.h>
#include "code-library.h"

void flags(char **argv) {
    static const size_t LIT_LEN = 5;
    static const char *literature[] = {
            "Stephen Prata (2014): C Primer Plus, 6th Edition, Addison-Wesley. [p]",
            "Brian Hall (2021): Beej's Guide to C Programming [beej.us/guide/bgc/html/split]. [b]",
            "TutorialsPoint: C Standard Library [tutorialspoint.com/c_standard_library]. [t]",
            "GeeksforGeeks: C Programming Language [geeksforgeeks.org/c-programming-language]. [g]",
            "Jens Gustedt (2019): Modern C, 2nd Edition, Manning. [m]",
    };
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        printf("%s%s%s\n", DELIMITER_TOC, " C CODE LIBRARY ", DELIMITER_TOC);
        puts("Commands:");
        printf("\t- %d: Table of Content (or any char)\n\t- %d: Exit\n", TOC, EXIT);
        puts("\nLiterature:");
        for (int i = 0; i < LIT_LEN; i++) printf("\t- %s\n", literature[i]);
        puts("");
    }
}


/* wrapper for fgets - replaces first newline with null character */
void f_get(char *string, int n, FILE *f) {
    fgets(string, n, f);
    while (*string != '\0') {
        if (*string == '\n') {
            *string = '\0';
            break;
        }
        string++;
    }
}
