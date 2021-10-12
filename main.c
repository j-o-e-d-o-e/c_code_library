#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#define LLEN 70
#define LIT_LEN 2
#define DIR_PATH "/media/joe/E/programming/c/code_library/library/"
#define DELIMITER_TOC "====================================="
#define DELIMITER_ENTRY "-------------------------------------"
#define TOC 0
#define EXIT 667

void flags(char **argv);

int read_dir(void);

void print_toc(int library_len);

void print_entry(int i);

void fget(char *string, int n, FILE *file);

const char *literature[LIT_LEN] = {
        "Stephen Prata (2014): C Primer Plus, 6th Edition, Addison-Wesley.",
        "..."
};

struct file_struct {
    int index;
    char path[LLEN];
    char title[LLEN];
    char src[LLEN];
};
struct file_struct library[LLEN];

int main(int argc, char **argv) {
    if (argc > 1) {
        flags(argv);
        return 0;
    }
    int library_len = read_dir();
    print_toc(library_len);
    while (1) {
        printf("\nWhat would you like to read? ");
        int ch;
        scanf("%d", &ch);
        if (ch == TOC) {
            printf("\n");
            print_toc(library_len);
            continue;
        } else if (ch == EXIT) {
            printf("Devil's neighbour wishes you a good day.\n");
            break;
        } else if (ch > library_len || ch < 0) {
            printf("Not a valid number.");
            continue;
        }
        print_entry(ch);
    }
    return 0;
}

void flags(char **argv) {
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "-help") == 0) {
        printf("%s%s%s\n", DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
        puts("Commands:");
        printf("\t- %d: Table of Content\n\t- %d: Exit\n", TOC, EXIT);
        puts("\nLiterature:");
        for (int i = 0; i < LIT_LEN; i++) printf("\t- %s\n", literature[i]);
    }
}

int read_dir(void) {
    DIR *d;
    if (!(d = opendir(DIR_PATH))) {
        printf("Opening Directory %s failed.\n", DIR_PATH);
        exit(EXIT_FAILURE);
    }
    int count = 0;
    struct dirent *file;
    while ((file = readdir(d))) {
        if (file->d_type != 8) continue;
        library[count].index = count + 1;
        char filename[] = DIR_PATH;
        strcat(filename, file->d_name);
        strcpy(library[count].path, filename);
        FILE *f = fopen(filename, "r");
        fget(library[count].title, LLEN, f);
        char s[LLEN];
        fget(s, LLEN, f); // empty line
        fget(library[count].src, LLEN, f);
        count++;
        fclose(f);
    }
    closedir(d);
    return count;
}

void print_toc(int library_len) {
    printf("%s%s%s\n", DELIMITER_TOC, "C CODE LIBRARY", DELIMITER_TOC);
    for (int i = 0; i < library_len; i++) {
        int n = i < 9 ? 19 : 20;
        printf("%d - %-*s-> (%d) %s\n", library[i].index, LLEN - n, library[i].title, library[i].index, library[i].src);
    }
}

void print_entry(int i) {
    FILE *f;
    if ((f = fopen(library[i - 1].path, "r")) == NULL) {
        printf("Opening File %s failed.\n", library[i - 1].title);
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
