#include <stdlib.h>
#include <stdio.h>
#include <editline/readline.h>

#include "mpc.h"

int main() {
    mpc_parser_t* Die = mpc_new("die");
    mpc_parser_t* Dice = mpc_new("dice");

    mpca_lang(MPCA_LANG_DEFAULT, 
        "                           \
            die  : /d[0-9][0-9]*/;  \
            dice : /^/ <die> /$/;   \
        ", Die, Dice);

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