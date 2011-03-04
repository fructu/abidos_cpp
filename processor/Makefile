YACC=yacc
LEX=lex

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

# extract_symbols.pl tokens.h
parser.c parser.h: parser.y
	$(YACC) -dt parser.y
	mv -f y.tab.c parser.c
	mv -f y.tab.h parser.h
	./extract_symbols.pl


lexer.c: lexer.l
	$(LEX) -t lexer.l >lexer.c

parser_descent.o: lex_yacc.h

lexer.o: parser.h

lex_yacc.h: parser.h

clean:
	rm -f tryit *.o
	rm -f lexer.c parser.c parser.h tokens.h
	rm -f *~
