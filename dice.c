#include <stdlib.h>
#include <stdio.h>
#include <editline/readline.h>

int main(int argc, char** argv) {
    while(1) {
        char* input = readline("dice> ");
        add_history(input);
        printf("%s\n", input);
        free(input);
    }
}