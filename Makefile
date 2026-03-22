CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: reader writer

reader: reader.c common.h
	@$(CC) $(CFLAGS) -o reader reader.c

writer: writer.c common.h
	@$(CC) $(CFLAGS) -o writer writer.c

clean:
	@rm -f reader writer	/tmp/files.txt /tmp/dirs.txt