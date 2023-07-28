#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "code-library.h"

void flags(char **argv) {
    static const size_t LIT_LEN = 6;
    static const char *literature[] = {
            "Stephen Prata (2014): C Primer Plus, 6th Edition, Addison-Wesley. [primer]",
            "Brian Hall (2021): Beej's Guide to C Programming [beej.us/guide/bgc/html/split]. [beej]",
            "TutorialsPoint: C Standard Library [tutorialspoint.com/c_standard_library].",
            "GeeksforGeeks: C Programming Language [geeksforgeeks.org/c-programming-language].",
            "Jens Gustedt (2019): Modern C, 2nd Edition, Manning. [modern]",
            "Arpaci-Dusseau/Remzi, Arpaci-Dusseau/Andrea (2018): Operating Systems. Three Easy Pieces. [ostep]",
    };
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        printf("%s %s %s\n", DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
        puts("Commands:");
        printf("\t- %d: Table of Content (or any char)\n\t- %d: Exit\n", TOC, EXIT);
        puts("\nLiterature:");
        for (int i = 0; i < LIT_LEN; i++) printf("\t- %s\n", literature[i]);
        puts("");
    }
}

void f_gets(char *s, int n, FILE *f) {
    fgets(s, n, f);
    while (*s != '\0') {
        if (*s == '\n') {
            *s = '\0';
            break;
        }
        s++;
    }
}

void lowerAndTrim(char *t, const char *s) {
    int i = 0, flag = 0;
    while (*s) {
        if (*s != ' ') {
            flag = 1;
            t[i] = tolower(*s);
            i++;
        } else if (flag) {
            t[i] = '\0';
            break;
        }
        s++;
    }
}
