CC = gcc
CFLAGS = -I .
LD = gcc

OBJS = xsh_parse.o xsh_lex.o xsh_main.o xsh_cmd.o xsh_manager.o

all: xsh

xsh: $(OBJS)
	$(LD) $^ -o $@

.c.o:
	$(CC) $(CFLAGS) $^ -c

xsh_parse.c: xsh_parse.y
	bison -d -o $@ $^

xsh_lex.c: xsh_lex.l xsh_parse.c
	flex -o $@ xsh_lex.l

clean:
	$(RM) $(OBJS) xsh_parse.c xsh_parse.h xsh_lex.c
