CFLAGS=-std=c99 -fsanitize=address -fno-omit-frame-pointer -Wall -Wextra -ggdb -O2 -ledit -lm

dice: dice.c
	gcc $(CFLAGS) -o dice dice.c mpc.c