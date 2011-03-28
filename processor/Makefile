YACC=yacc
LEX=lex

OBJS = 	lexer.o \
	parser_descent.o \
	parser_descent_rules.o \
	symbols_table.o \
	semantic.o \
	main.o

CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
EXECUTABLE=abidos

all: clean tryit_descent

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

debug_put:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.bk
	./put_printf_debug.pl
	cp parser_descent_rules.cpp_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_debug

debug_drop:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.debug.bk
	./drop_printf_debug.pl
	cp parser_descent_rules.cpp_no_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_no_debug

clean:
	rm -f tryit *.o
	rm -f lexer.c parser.c parser.h tokens.h
	rm -f *~
	rm -f *.bk
	rm -f $(EXECUTABLE)

