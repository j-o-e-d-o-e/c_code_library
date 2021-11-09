#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "code-library.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        flags(argv);
        return EXIT_SUCCESS;
    }
    Library lib;
    setup_lib(&lib);
    print_toc(&lib);
    char input[3];
    unsigned int long ch;
    while (true) {
        printf("\nWhat would you like to read? ");
        scanf("%s", input);
        ch = strtol(input, NULL, 10);
        if (ch == TOC) {
            printf("\n");
            print_toc(&lib);
            continue;
        } else if (ch == EXIT) {
            printf("Devil's neighbour wishes you a good day.\n");
            break;
        } else if (ch > lib.len || ch < 0) {
            printf("Not a valid number.");
            continue;
        }
        print_entry(&lib.entries[ch - 1]);
    }
    return 0;
}

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

void setup_lib(Library *lib) {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("Opening Directory %s failed.\n", DIR_PATH);
        exit(EXIT_FAILURE);
    }
    int count = 0;
    struct dirent *file;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;
        struct entry *e = &lib->entries[count];
        e->index = count + 1;
        char filename[] = DIR_PATH;
        strcat(filename, file->d_name);
        strcpy(e->path, filename);
        FILE *f = fopen(filename, "r");
        fget(e->title, LLEN, f);
        char s[LLEN];
        fget(s, LLEN, f); // empty line
        fget(e->src, LLEN, f);
        count++;
        fclose(f);
    }
    closedir(d);
    lib->len = count;
    sort_lib(lib);
}

void sort_lib(Library *lib) {
    qsort(lib->entries, lib->len, sizeof(struct entry), comp);
    for (int i = 0; i < lib->len; i++) lib->entries[i].index = i + 1;
}

int comp(const void *p1, const void *p2) {
    const struct entry *ps1 = p1;
    const struct entry *ps2 = p2;
    return strcmp(ps1->title, ps2->title); // sort by title
}

void print_toc(const Library *lib) {
    printf("%s%s%s\n", DELIMITER_TOC, " C CODE LIBRARY ", DELIMITER_TOC);
    const struct entry *e = &(lib->entries[0]);
    const struct entry *end = e + lib->len;
    while (e < end) {
        int n = e->index < 10 ? 29 : 30;
        printf("%d - %-*s-> (%d) %s\n", e->index, LLEN - n, e->title, e->index, e->src);
        e++;
    }
}

void print_entry(const struct entry *entry) {
    FILE *f;
    if ((f = fopen(entry->path, "r")) == NULL) {
        printf("Opening File %s failed.\n", entry->title);
        return;
    }
    printf("\n%s\n", DELIMITER_ENTRY);
    char title[LLEN];
    fget(title, LLEN, f);
    printf("%s\n\n", title);
    char buffer[LLEN];
    fget(buffer, LLEN, f); // empty line
    fget(buffer, LLEN, f); // src
    fget(buffer, LLEN, f); // empty line
    char content[LLEN];
    while (1) {
        content[0] = '\0';
        fgets(content, LLEN, f);
        printf("%s", content);
        if (feof(f)) break;
    }
    fclose(f);
    printf("%s\n", DELIMITER_ENTRY);
}

/* wrapper for fgets - replaces first newline with null character */
void fget(char *string, int n, FILE *f) {
    fgets(string, n, f);
    while (*string != '\0') {
        if (*string == '\n') {
            *string = '\0';
            break;
        }
        string++;
    }
}
