#ifndef CODE_LIBRARY_CODE_LIBRARY_H
#define CODE_LIBRARY_CODE_LIBRARY_H

#define S_LEN 80
#define DIR_PATH "/media/joe/E/programming/c/code_library/library/" // TODO: relative path "(.)./library/"
#define DELIMITER_TOC "======================================="
#define DELIMITER_ENTRY "-------------------------------------"
#define TOC 0
#define EXIT 667
#define clear() printf("\033[H\033[J")

/* a file representation according to template
        index: sequential num
        path: DIR_PATH + filename
        title: 1st line in file
        src: 3rd line (literature) */
struct entry {
    int index;
    char path[S_LEN];
    char title[S_LEN];
    char src[S_LEN]; // literature
};
/* collection of entries */
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

/* wrapper for fgets(): replaces first newline with null character */
void f_gets(char *s, int n, FILE *f);

#endif // CODE_LIBRARY_CODE_LIBRARY_H
