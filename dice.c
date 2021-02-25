#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <editline/readline.h>

#include "mpc.h"

typedef struct dice_state {
    int sides;
} dice_state;

void initialize_state(dice_state* state) {
    state->sides = 0;
}

void print_state(dice_state* state) {
    printf("state->sides: %d\n", state->sides);
}

void eval(mpc_ast_t* tree, dice_state* state) {
    mpc_ast_print(tree);
    puts("-----");

    if (strstr(tree->tag, "sides")) {
        state->sides = atoi(tree->contents);
    }

    for(int i = 0; i < tree->children_num; i++) {
        eval(tree->children[i], state);
    }
}

int main(int argc, char* argv[]) {
    mpc_parser_t* Sides = mpc_new("sides");
    mpc_parser_t* Die = mpc_new("die");
    mpc_parser_t* Dice = mpc_new("dice");

    mpc_err_t* err = mpca_lang(MPCA_LANG_DEFAULT,
        "                            \
            sides : /[1-9][0-9]*/;   \
            die   : /d/<sides>;      \
            dice  : /^/ <die> /$/;   \
        ", Sides, Die, Dice);

    if (err != NULL) {
        mpc_err_print(err);
        mpc_err_delete(err);
        return 1;
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
        dice_state state;
        initialize_state(&state);
        eval(result.output, &state);
        print_state(&state);
        mpc_ast_delete(result.output);
    }
    else {
        mpc_err_print(result.error);
        mpc_err_delete(result.error);
    }

    free(Die);
    free(Dice);
}