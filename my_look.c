#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const int BUFF_MAX = 256;  // maximum buffer size

// Helper function that strips a line of non-alphanumeric characters
void line_strip(char *line, char *ret_line) {
    // add only alphanumeric characters to strip_buffer
    for (int i = 0; i < strlen(line); i++) {
        if (isalnum(line[i])) {
            strncat(ret_line, &line[i], 1);
        }
    }

    // convert all capital letters to lowercase in strip_buffer
    for (int i = 0; i < strlen(ret_line); i++) {
        if (ret_line[i] >= 65 && ret_line[i] <= 90) {
            ret_line[i] = ret_line[i] + 32;
        }
    }

    return;
}


// Main method
int main(int argc, char *argv[]) {
    int file_arg_num = 0;
    int file_flag = 0;

    char word[256] = "";
    snprintf(word, sizeof(word), "%s", argv[argc-1]);

    // handle command-line arguments
    for (int i = 1; i < argc; i++) {
        if (!(strcmp(argv[i], "-V"))) {  // -V option
            printf("my-look from CS537 Summer 2022\n");
            exit(0);
        } else if (!(strcmp(argv[i], "-h"))) {  // -h option
            printf("Add a file using option -f, or use command line.\n");
            exit(0);
        } else if (!(strcmp(argv[i], "-f"))) {  // -f option
            file_arg_num = i+1;
            file_flag = 1;
        } else if ((file_flag && (i == file_arg_num))) {  // file path argument
            continue;
        } else if ((file_flag && (i == file_arg_num+1))) {  // word to search
            continue;
        } else if ((!file_flag) && (i == argc - 1)) {
            // checks if the command-line argument is alphanumeric
            int isalnum_flag = 0;
            for (int j = 0; j < strlen(argv[i]); j++) {
                if (isalnum(argv[i][j])) {
                    isalnum_flag = 1;
                } else {
                    isalnum_flag = 0;
                    break;
                }
            }

            // invalid command line if word to search for is not alphanumeric
            if (isalnum_flag) {
                continue;
            } else {
                printf("my-look: invalid command line\n");
                exit(1);
            }

        } else {  // All other command line arguments are not valid
            printf("my-look: invalid command line\n");
            exit(1);
        }
    }

    char buffer[BUFF_MAX];  // real buffer that stores file data or stdin data
    char strip_buffer[BUFF_MAX];   // stripped buffer alnum characters only
    FILE *fp = NULL;  // file pointer for both stdin and real file

    // handle file and file path
    if (file_flag) {
        // use fopen for file path
        char *file_path = argv[file_arg_num];

        // File handling
        fp = fopen(file_path, "r");
        if (fp == NULL) {
            printf("my-look: cannot open file\n");
            exit(1);
        }
    }

    // handle stdin code
    if (!file_flag) {
        fp = stdin;
        if (fp == NULL) {
            printf("my-look: cannot open file\n");
            exit(1);
        }
    }

        // clear both buffers
        memset(buffer, 0, sizeof(buffer));
        memset(strip_buffer, 0, sizeof(strip_buffer));

        // loops while file pointer has data
        while (fgets(buffer, sizeof(buffer), fp)) {
            line_strip(buffer, strip_buffer);  // strip_buffer has alnum chars

            int match_flag = 0;

            // checks strip_buffer by each character to make sure it matches
            for (int i = 0; i < strlen(argv[argc-1]); i++) {
                if (word[i] == strip_buffer[i]) {
                    match_flag = 1;
                } else {
                    match_flag = 0;
                    break;
                }
            }

            // if there is a match, print the buffer
            if (match_flag) {
                printf("%s", buffer);
            }

            // clear strip_buffer for next line
            memset(strip_buffer, 0, sizeof(strip_buffer));
        }

        // close file if file was opened
        if (file_flag) {
            fclose(fp);
        }
    return 0;
}
