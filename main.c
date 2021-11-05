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
    DIR *d = read_dir();
    Library lib[LLEN];
    int lib_len = setup_lib(d, lib);
    sort_lib(lib_len, lib);
    print_toc(lib_len, lib);
    while (true) {
        printf("\nWhat would you like to read? ");
        int ch;
        scanf("%d", &ch);
        if (ch == TOC) {
            printf("\n");
            print_toc(lib_len, lib);
            continue;
        } else if (ch == EXIT) {
            printf("Devil's neighbour wishes you a good day.\n");
            break;
        } else if (ch > lib_len || ch < 0) {
            printf("Not a valid number.");
            continue;
        }
        print_entry(&lib[ch - 1]);
    }
    return 0;
}

const char *literature[LIT_LEN] = {
        "Stephen Prata (2014): C Primer Plus, 6th Edition, Addison-Wesley.",
        "Jens Gustedt (2019): Modern C, 2nd Edition, Manning.",
        "TutorialsPoint: C Standard Library [tutorialspoint.com/c_standard_library]."
};

void flags(char **argv) {
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        printf("%s%s%s\n", DELIMITER_TOC, " C CODE LIBRARY ", DELIMITER_TOC);
        puts("Commands:");
        printf("\t- %d: Table of Content\n\t- %d: Exit\n", TOC, EXIT);
        puts("\nLiterature:");
        for (int i = 0; i < LIT_LEN; i++) printf("\t- %s\n", literature[i]);
        puts("");
    }
}

DIR *read_dir() {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("Opening Directory %s failed.\n", DIR_PATH);
        exit(EXIT_FAILURE);
    }
    return d;
}

int setup_lib(DIR *d, Library lib[]){
    int count = 0;
    struct dirent *file;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;
        lib[count].index = count + 1;
        char filename[] = DIR_PATH;
        strcat(filename, file->d_name);
        strcpy(lib[count].path, filename);
        FILE *f = fopen(filename, "r");
        fget(lib[count].title, LLEN, f);
        char s[LLEN];
        fget(s, LLEN, f); // empty line
        fget(lib[count].src, LLEN, f);
        count++;
        fclose(f);
    }
    closedir(d);
    return count;
}

void sort_lib(int lib_len, Library lib[]) {
    qsort(lib, lib_len, sizeof(Library), comp);
    for (int i = 0; i < lib_len; i++) lib[i].index = i + 1;
}

int comp(const void *p1, const void *p2) {
    const Library *ps1 = (const Library *) p1;
    const Library *ps2 = (const Library *) p2;
    return strcmp(ps1->title, ps2->title); // sort by title
}

void print_toc(int lib_len, const Library lib[]) {
    printf("%s%s%s\n", DELIMITER_TOC, " C CODE LIBRARY ", DELIMITER_TOC);
    for (int i = 0; i < lib_len; i++) {
        int n = i < 9 ? 29 : 30;
        printf("%d - %-*s-> (%d) %s\n", lib[i].index, LLEN - n, lib[i].title, lib[i].index, lib[i].src);
    }
}

void print_entry(const Library *lib) {
    FILE *f;
    if ((f = fopen(lib->path, "r")) == NULL) {
        printf("Opening File %s failed.\n", lib->title);
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
