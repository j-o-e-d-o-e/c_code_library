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
    Library *lib = setup_lib();
    print_toc(lib);
    char input[3];
    unsigned int long ch;
    while (true) {
        printf("\nWhat would you like to read? ");
        scanf("%s", input);
        ch = strtol(input, NULL, 10);
        if (ch == TOC) {
            printf("\n");
            print_toc(lib);
            continue;
        } else if (ch == EXIT) {
            printf("Devil's neighbour wishes you a good day.\n");
            break;
        } else if (ch > lib->len || ch < 0) {
            printf("Not a valid number.");
            continue;
        }
        print_entry(&lib->entries[ch - 1]);
    }
    free(lib);
    return 0;
}


Library *setup_lib(void) {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("Opening Directory %s failed.\n", DIR_PATH);
        exit(EXIT_FAILURE);
    }
    int buffer = 10;
    Library *lib = malloc(sizeof(Library) + sizeof(struct entry[buffer]));
    if (lib == NULL) exit(0);
    int count = 0;
    struct dirent *file;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;
        if (count == buffer) {
            buffer *= 2;
            lib = realloc(lib, sizeof(Library) + sizeof(struct entry) * buffer);
            if (lib == NULL) exit(0);
        }
        struct entry *e = &(lib->entries[count]);
        e->index = count + 1;
        char filename[] = DIR_PATH;
        strcat(filename, file->d_name);
        strcpy(e->path, filename);
        FILE *f = fopen(filename, "r");
        f_gets(e->title, S_LEN, f);
        char s[S_LEN];
        f_gets(s, S_LEN, f); // empty line
        f_gets(e->src, S_LEN, f);
        count++;
        fclose(f);
    }
    closedir(d);
    lib->len = count;
    lib = realloc(lib, sizeof(Library) + sizeof(struct entry) * lib->len);
    if (lib == NULL) exit(0);
    sort_lib(lib);
    return lib;
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
    printf("%s %s %s\n", DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    const struct entry *e = &(lib->entries[0]);
    const struct entry *end = e + lib->len;
    while (e < end) {
        int n = e->index < 10 ? 29 : 30;
        printf("%d - %-*s-> (%d) %s\n", e->index, S_LEN - n, e->title, e->index, e->src);
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
    char title[S_LEN];
    f_gets(title, S_LEN, f);
    printf("%s\n\n", title);
    char buffer[S_LEN];
    f_gets(buffer, S_LEN, f); // empty line
    f_gets(buffer, S_LEN, f); // src
    f_gets(buffer, S_LEN, f); // empty line
    char content[S_LEN];
    while (1) {
        content[0] = '\0';
        fgets(content, S_LEN, f);
        printf("%s", content);
        if (feof(f)) break;
    }
    fclose(f);
    printf("%s\n", DELIMITER_ENTRY);
}
