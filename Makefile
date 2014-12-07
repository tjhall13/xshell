CC = gcc
CFLAGS = -I .
LD = gcc
LFLAGS = 

OBJS = xsh_parse.o xsh_lex.o xsh_main.o xsh_cmd.o xsh_manager.o xsh_execute_cmd.o

all: xsh

xsh: $(OBJS)
	$(LD) $(LFLAGS) $^ -o $@ -lm

.c.o:
	$(CC) $(CFLAGS) $^ -c

xsh_parse.c: xsh_parse.y
	bison -d -o $@ $^

xsh_lex.c: xsh_lex.l xsh_parse.c
	flex -o $@ xsh_lex.l

clean:
	$(RM) $(OBJS) xsh_parse.c xsh_parse.h xsh_lex.c
