YACC=yacc
LEX=lex

OBJS = options.o \
	trace.o \
	lexer.o \
	parser_descent.o \
	parser_descent_rules.o \
	parser_descent_preprocessor.cpp \
	symbols_table.o \
	semantic.o \
	generator_class_diagram.o \
	generator_original.o \
	main.o

CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
EXECUTABLE=abidos

all: clean make_descent test_run

indent_run: clean make_descent
	./check_indent.sh
	./indent_run.sh

.c.o:
	cc -c $<

.cpp.o:
	$(CC) $(CFLAGS) $<

make_descent:  $(OBJS)
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

generator_class_diagram.cpp: generate_begin_graph.cpp

generate_begin_graph.cpp: python/out_begin_2.gv
	./generate_begin_graph.pl

test_run: tests_run.pl
	./tests_run.pl

debug_put:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.bk
	./put_printf_debug.pl
	cp parser_descent_rules.cpp_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_debug

debug_graph_put:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.bk
	./put_trace_graph_debug.pl
	cp parser_descent_rules.cpp_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_debug

FILE_TEST=t034.cpp
#FILE_TEST=mass_clases.cpp
#FILE_TEST=test_symbols_table.h
execute: $(EXECUTABLE)
	cp xdot2.py out/
	./$(EXECUTABLE) --verbose --ts_show test/$(FILE_TEST)
	cat python/out_begin.py > out/out_run.py
	cat out/out_$(FILE_TEST).gv >> out/out_run.py
	cat python/out_end.py >> out/out_run.py
	chmod +x out/out_run.py
	out/out_run.py

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
	rm -f out/*
	rm -f test_out/*
	rm -f *.orig
	rm -f *.pyc
	rm -f test_out/*.pyc		
	


