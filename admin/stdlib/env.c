#include <direct.h>
#include <stdio.h>
#include <string.h>

#include "env.h"

#define MAX_LINE_LENGTH 1024

char* get_env_value(const char* key) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));

    char* last_slash = strrchr(cwd, '\\');
    if (last_slash) *last_slash = '\0';

    char env_path[2048];
    snprintf(env_path, sizeof(env_path), "%s\\.env", cwd);

    FILE* file = fopen(env_path, "r");
    if (!file) {
        printf("Failed open .env file\n");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];
    char* value = NULL;
    char* token;

    while(fgets(line, sizeof(line), file)) {
        token = strtok(line, "=");
        if (token && strcmp(token, key) == 0) {
            token = strtok(NULL, "\n");
            if (token) {
                value = strdup(token);
                break;
            }
        }
    }

    fclose(file);
    return value;
}
