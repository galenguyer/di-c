#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <editline/readline.h>

#include "mpc.h"

int main(int argc, char* argv[]) {
    mpc_parser_t* Die = mpc_new("die");
    mpc_parser_t* Dice = mpc_new("dice");

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        "                           \
            die  : /d[0-9][0-9]*/;  \
            dice : /^/ <die> /$/;   \
        ", Die, Dice);

    if (err != NULL) {
        printf("%s\n", err->failure);
    }

    if (argc == 1) {
        while(1) {
            char* input = readline("dice> ");
            add_history(input);

            mpc_result_t result;
            if (mpc_parse("<stdin>", input, Dice, &result)) {
                mpc_ast_print(result.output);
                mpc_ast_delete(result.output);
            }
            else {
                mpc_err_print(result.error);
                mpc_err_delete(result.error);
            }

            free(input);
        }
    }
    else {
        char input[1024];
        int i = 0;
        for (int arg = 1; arg < argc; arg++) {
            for (int pos = 0; argv[arg][pos] != '\0'; pos++) {
                input[i++] = argv[arg][pos];
            }
        }
        input[i] = '\0';
        
        mpc_result_t result;
        if (mpc_parse("<stdin>", input, Dice, &result)) {
            mpc_ast_print(result.output);
            mpc_ast_delete(result.output);
        }
        else {
            mpc_err_print(result.error);
            mpc_err_delete(result.error);
        }
    }

    free(Die);
    free(Dice);
}