#include <stdio.h>

#include "admin.h"

void input() {
    while (1) {
        int action;
        printf("\n\033[31m[~]\033[0m \033[32mSelect an option:\033[0m ");
        scanf("%d", &action);

        printf("Selected: %d", action);
    }
}
