#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include "code-library.h"
#include <readline/readline.h>
#include <readline/history.h>

Library *search(const Library *lib, char *line);

int main(int argc, char **argv) {
    if (argc > 1) {
        flags(argv);
        return EXIT_SUCCESS;
    }
    Library *lib = setup_lib();
    print_toc(lib);
    char prompt[40];
    sprintf(prompt, "\n\033[%dmWhat would you like to read? \033[0m", RED);
    char *line;
    unsigned int long ch;
    while (true) {
        line = readline(prompt);
        if (line[0] == 's' && line[1] == ':') {
            add_history(line);
            Library *tmp = search(lib, line);
            printf("\n");
            print_toc(tmp);
            free(tmp);
            continue;
        }
        ch = strtol(line, NULL, 10);
        if (strlen(line) > 0 && ch != 0) add_history(line);
        if (ch == TOC) {
            printf("\n");
            print_toc(lib);
            continue;
        } else if (ch == EXIT) {
            printf("\033[%dmDevil's neighbour wishes you a good day.\033[0m\n", RED);
            break;
        } else if (ch > lib->len != 0) {
            printf("\033[%dmNot a valid number.\033[0m", RED);
            continue;
        }
        print_entry(&lib->entries[ch - 1]);
    }
    free(lib);
    return 0;
}

Library *search(const Library *lib, char *line) {
    char *term = lowerAndTrim(line + 2);
    const struct entry *e = &(lib->entries[0]);
    const struct entry *end = e + lib->len;
    int count = 0;
    Library *tmp = malloc(sizeof(Library) + sizeof(struct entry[lib->len]));
    while (e < end) {
        char *tmp_s = lowerAndTrim(e->tags);
        if (strstr(tmp_s, term)) {
            tmp->entries[count] = *e;
            count++;
        }
        e++;
    }
    tmp->len = count;
    return tmp;
}

Library *setup_lib(void) {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("\033[%dmOpening Directory %s failed.\033[0m\n", RED, DIR_PATH);
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
        f_gets(e->tags, S_LEN, f);
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
//    clear();
    printf("\033[%d;1m%s %s %s\033[0m\n", RED, DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    const struct entry *e = &(lib->entries[0]);
    const struct entry *end = e + lib->len;
    int count = 0;
    while (e < end) {
        if (count % 2 == 0) {
            printf("\033[%dm%*d - %-*s%s\033[0m\n", GREEN, 2, e->index, S_LEN - 36, e->title, e->tags);
        } else {
            printf("\033[%d;%dm%*d - %-*s%s\033[0m\n", CYAN, BLACK, 2, e->index, S_LEN - 36, e->title, e->tags);
        }
        e++;
        count++;
    }
}

void print_entry(const struct entry *entry) {
    FILE *f;
    if ((f = fopen(entry->path, "r")) == NULL) {
        printf("\033[%dmOpening File %s failed.\033[0m\n", RED, entry->title);
        return;
    }
    printf("\n\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
//    clear();
    char title[S_LEN];
    f_gets(title, S_LEN, f);
    printf("\033[%d;1;4m%d - %s\033[0m\n\n", colors[entry->index % 2 == 0], entry->index, title);
    char buffer[S_LEN];
    f_gets(buffer, S_LEN, f); // empty line
    f_gets(buffer, S_LEN, f); // src
    f_gets(buffer, S_LEN, f); // empty line
    char content[S_LEN];
    while (1) {
        content[0] = '\0';
        fgets(content, S_LEN, f);
        if (strcmp(content, "EXAMPLE\n") == 0) printf("\033[%d;1m%s\033[0m", colors[entry->index % 2 == 0], content);
        else printf("\033[%dm%s\033[0m", colors[entry->index % 2 == 0], content);
        if (feof(f)) break;
    }
    fclose(f);
    printf("\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
}
