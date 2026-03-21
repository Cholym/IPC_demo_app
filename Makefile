CC=gcc
CFLAGS=-Wall -Werror -Wextra

run: request.o
	@$(CC) $(CFLAGS) request.c -n request
	@rm -f request.o

clang:
	@clang-format -i request.c

clean:
	@rm -f request