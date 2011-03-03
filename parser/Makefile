YACC=yacc
LEX=flex

OBJS = parser_descent.o lexer.o main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
EXECUTABLE=abidos

all: tryit_descent

.c.o:
	cc -c $<

.cpp.o:
	$(CC) $(CFLAGS) $<

tryit_descent:  $(OBJS)
	$(CC) -o $(EXECUTABLE)  $(OBJS)

tryit: parser.o lexer.o
	cc -o tryit parser.o lexer.o

parser.c parser.h: parser.y
	$(YACC) -dt parser.y
	mv -f y.tab.c parser.c
	mv -f y.tab.h parser.h

lexer.c: lexer.l
	$(LEX) -t lexer.l >lexer.c

lexer.o: parser.h

clean:
	rm -f tryit *.o
	rm -f lexer.c parser.c parser.h
