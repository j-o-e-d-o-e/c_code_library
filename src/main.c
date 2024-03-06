#include <stdio.h>
#include <stdlib.h>
#include <readline/history.h>
#include <stdbool.h>
#include <readline/readline.h>
#include "code-library.h"

int main(int argc, char **argv) {
    if (argc > 1) {
        flags(argv);
        return EXIT_SUCCESS;
    }
    Library *lib = setup_lib();
    user_input(lib);
    free(lib);
    return 0;
}

void user_input(Library *lib) {
    print_toc(lib);
    using_history();
    char prompt[40];
    sprintf(prompt, "\n\033[%dmWhat would you like to read? \033[0m", RED);
    while (true) {
        char *line = readline(prompt);
        if (line[0] == 's' && line[1] == ':') {
            Library *tmp = search(lib, line);
            if (tmp->len > 0) {
                if (!check_duplicate(line)) add_history(line);
                printf("\n");
                print_toc(tmp);
            } else {
                printf("\033[%dmNo match found.\033[0m", RED);
                printf("\n");
            }
            free(tmp);
            free(line);
            continue;
        }
        char *error;
        size_t ch = strtol(line, &error, 10);
        if (*error == *line) {
            printf("\033[%dmNot a number.\033[0m", RED);
            printf("\n");
            free(line);
        } else if (ch == TOC) {
            printf("\n");
            print_toc(lib);
            free(line);
        } else if (ch == EXIT) {
            printf("\033[%dmDevil's neighbour wishes you a good day.\033[0m\n", RED);
            free(line);
            break;
        } else if (ch > lib->len != 0) {
            printf("\033[%dmNot a valid number.\033[0m", RED);
            printf("\n");
            free(line);
        } else {
            struct entry *e = &lib->entries[ch - 1];
            char item[TITLE_LEN + sizeof(short) + 2];
            sprintf(item, "%hu (%s)", (unsigned short) ch, e->title);
            if (strlen(line) > 0 && !check_duplicate(item)) add_history(item);
            print_entry(e);
            free(line);
        }
    }
}

void print_toc(const Library *lib) {
    printf("\033[%d;1m%s %s %s\033[0m\n", RED, DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    const struct entry *e = &(lib->entries[0]);
    const struct entry *end = e + lib->len;
    int count = 0;
    while (e < end) {
        if (count % 2 == 0) {
            printf("\033[%dm%*d - %-*s%s\033[0m\n", GREEN, 2, e->index, LINE_SPACE, e->title, e->tags);
        } else {
            printf("\033[%d;%dm%*d - %-*s%s\033[0m\n", CYAN, BLACK, 2, e->index, LINE_SPACE, e->title, e->tags);
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
    char title[TITLE_LEN];
    f_gets(title, TITLE_LEN, f);
    printf("\033[%d;1;4m%d - %s\033[0m\n\n", colors[entry->index % 2 == 0], entry->index, title);
    char buffer[PATH_LEN];
    f_gets(buffer, 1, f); // empty line
    f_gets(buffer, TITLE_LEN, f); // tags
    f_gets(buffer, 1, f); // empty line
    char content[PATH_LEN];
    while (1) {
        content[0] = '\0';
        fgets(content, PATH_LEN, f);
        if (strcmp(content, "EXAMPLE\n") == 0) printf("\033[%d;1m%s\033[0m", colors[entry->index % 2 == 0], content);
        else printf("\033[%dm%s\033[0m", colors[entry->index % 2 == 0], content);
        if (feof(f)) break;
    }
    fclose(f);
    printf("\033[%dm%s\033[0m\n", RED, DELIMITER_ENTRY);
}

int check_duplicate(char *line) {
    HIST_ENTRY **e = history_list();
    for (int i = 0; i < history_length; i++) {
        char *s = e[i]->line;
        if (strcmp(s, line) == 0) return true;
    }
    return false;
}
