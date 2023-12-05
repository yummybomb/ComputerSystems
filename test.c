#include <stdio.h>
#include <glob.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

char** find_matches(int argc, char **argv, int *count, glob_t *glob_result) {
    if(argc < 1) {
        printf("Usage: %s <pattern>\n", argv[0]);
        return NULL;
    }

    glob_result->gl_pathv = NULL;

    // Loop over all the arguments
    for(int j = 0; j < argc; ++j) {
        glob_t temp;
        glob(argv[j], GLOB_TILDE, NULL, &temp);

        // If no matches, include the original string
        if(temp.gl_pathc == 0) {
            if(glob_result->gl_pathv == NULL) {
                glob_result->gl_pathv = malloc(sizeof(*glob_result->gl_pathv));
            } else {
                glob_result->gl_pathv = realloc(glob_result->gl_pathv, sizeof(*glob_result->gl_pathv) * (glob_result->gl_pathc + 1));
            }
            glob_result->gl_pathv[glob_result->gl_pathc] = strdup(argv[j]);
            glob_result->gl_pathc++;
        } else {
            glob(argv[j], GLOB_TILDE | GLOB_APPEND, NULL, glob_result);
        }

        globfree(&temp);
    }

    // New code: Filter out non-regular files
    int j = 0;
    struct stat path_stat;
    for(int i = 0; i < glob_result->gl_pathc; ++i) {
        stat(glob_result->gl_pathv[i], &path_stat);
        if(S_ISREG(path_stat.st_mode)) {
            glob_result->gl_pathv[j++] = glob_result->gl_pathv[i];
        }
    }
    glob_result->gl_pathc = j;

    *count = glob_result->gl_pathc;
    return glob_result->gl_pathv;
}


int main(int argc, char **argv) {
    int count;
    glob_t glob_result;
    char **matches = find_matches(argc, argv, &count, &glob_result);
    if(matches == NULL) {
        return 1;
    }
    printf("args: %d\n", count);
    for(int i = 0; i < count; ++i) {
        printf("%s\n", matches[i]);
    }

    globfree(&glob_result);
    return 0;
}

