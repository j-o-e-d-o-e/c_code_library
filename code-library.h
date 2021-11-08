#ifndef CODE_LIBRARY_CODE_LIBRARY_H
#define CODE_LIBRARY_CODE_LIBRARY_H

#define LLEN 80
#define DIR_PATH "/media/joe/E/programming/c/code_library/library/"
#define DELIMITER_TOC "======================================="
#define DELIMITER_ENTRY "-------------------------------------"
#define TOC 0
#define EXIT 667

struct entry {
    int index;
    char path[LLEN];
    char title[LLEN];
    char src[LLEN];
};
typedef struct {
    int len;
    struct entry entries[LLEN];
} Library;

void flags(char **argv);

void setup_lib(Library *lib);

void sort_lib(Library *lib);

int comp(const void *p1, const void *p2);

void print_toc(const Library *lib);

void print_entry(const struct entry *entry);

void fget(char *string, int n, FILE *file);

#endif // CODE_LIBRARY_CODE_LIBRARY_H
