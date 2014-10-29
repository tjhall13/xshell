CC = gcc
CFLAGS = -I .
LD = gcc

OBJS = xsh_parse.o xsh_main.o xsh_cmd.o xsh_manager.o xsh_lex.o

all: xsh

xsh: $(OBJS)
	$(LD) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $^ -c

xsh_parse.c:
	bison -o $@ xsh_parse.y

xsh_lex.c: xsh_lex.lex
	flex -o $@ $^

clean:
	$(RM) $(OBJS) xsh_parse.c xsh_parse.h xsh_lex.c
