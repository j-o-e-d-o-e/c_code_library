#ifndef CODE_LIBRARY_CODE_LIBRARY_H
#define CODE_LIBRARY_CODE_LIBRARY_H
#endif //CODE_LIBRARY_CODE_LIBRARY_H

#define LLEN 80
#define DIR_PATH "/media/joe/E/programming/c/code_library/library/"
#define DELIMITER_TOC "======================================="
#define DELIMITER_ENTRY "-------------------------------------"
#define TOC 0
#define EXIT 667

typedef struct entry {
    int index;
    char path[LLEN];
    char title[LLEN];
    char src[LLEN];
} Library;

void flags(char **argv);

int setup_lib(Library lib[]);

void sort_lib(int lib_len, Library lib[lib_len]);

int comp(const void *p1, const void *p2);

void print_toc(int lib_len, const Library lib[lib_len]);

void print_entry(const struct entry *entry);

void fget(char *string, int n, FILE *file);
