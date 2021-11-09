#ifndef CODE_LIBRARY_CODE_LIBRARY_H
#define CODE_LIBRARY_CODE_LIBRARY_H

#define S_LEN 80
#define DIR_PATH "/media/joe/E/programming/c/code_library/library/"
#define DELIMITER_TOC "======================================="
#define DELIMITER_ENTRY "-------------------------------------"
#define TOC 0
#define EXIT 667

struct entry {
    int index;
    char path[S_LEN];
    char title[S_LEN];
    char src[S_LEN];
};
typedef struct {
    int len;
    struct entry entries[];
} Library;

void flags(char **argv);

Library *setup_lib(void);

void sort_lib(Library *lib);

int comp(const void *p1, const void *p2);

void print_toc(const Library *lib);

void print_entry(const struct entry *entry);

void fget(char *string, int n, FILE *file);

#endif // CODE_LIBRARY_CODE_LIBRARY_H
