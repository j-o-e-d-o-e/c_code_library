#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "code_library.h"

void flags(char **argv) {
    static const unsigned char LIT_LEN = 6;
    static const char *literature[] = {
            "Stephen Prata (2014): C Primer Plus, 6th Edition, Addison-Wesley. [primer]",
            "Brian Hall (2021): Beej's Guide to C Programming [beej.us/guide/bgc/html/split]. [beej]",
            "TutorialsPoint: C Standard Library [tutorialspoint.com/c_standard_library].",
            "GeeksforGeeks: C Programming Language [geeksforgeeks.org/c-programming-language].",
            "Jens Gustedt (2019): Modern C, 2nd Edition, Manning. [modern]",
            "Arpaci-Dusseau/Remzi, Arpaci-Dusseau/Andrea (2018): Operating Systems. Three Easy Pieces. [ostep]",
    };
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        printf("\033[%d;1m%s %s %s\033[0m\n", RED, DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
        printf("\033[%dmCommands:\033[0m\n", GREEN);
        printf("\033[%dm\t- %-3d: Table of Content (or any char)\n\t- %-3d: Exit\n\t- s: : Search\033[0m\n",
               CYAN, TOC, EXIT);
        printf("\033[%dm\nLiterature:\033[0m\n", GREEN);
        for (unsigned char i = 0; i < LIT_LEN; i++) printf("\033[%dm\t- %s[0m\n", CYAN, literature[i]);
        puts("");
    }
    exit(EXIT_SUCCESS);
}

void fgets_no_newline(char *s, int n, FILE *f) {
    fgets(s, n, f);
    while (*s != '\0') {
        if (*s == '\n') {
            *s = '\0';
            break;
        }
        s++;
    }
}

void trim(char *dest, const char *const src) {
    unsigned char index = 0, leading_trimmed = 0;
    for (size_t i = 0; i < strlen(src); i++) {
        const char *c = &src[i];
        if (*c != ' ') {
            leading_trimmed = 1;
            dest[index++] = *c;
            const char *c1 = &src[i + 1];
            if (*c1 == '\0') dest[index] = *c1;
            else if (*c1 == ' ' && *(c + 2) != '\0' && *(c + 2) != ' ') {
                dest[index++] = *c1, dest[index++] = *(c + 2);
                i += 2;
            }
        } else if (leading_trimmed) {
            dest[index] = '\0';
            break;
        }
    }
}
