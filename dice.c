#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <editline/readline.h>

#include "mpc.h"

int main(int argc, char* argv[]) {
    mpc_parser_t* Count = mpc_new("count");
    mpc_parser_t* Die = mpc_new("die");
    mpc_parser_t* Dice = mpc_new("dice");

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        "                            \
            count : /[1-9][0-9]*/;   \
            die   : /d/<count>;      \
            dice  : /^/ <die> /$/;   \
        ", Count, Die, Dice);

    if (err != NULL) {
        mpc_err_print(err);
    }

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

    free(Die);
    free(Dice);
}