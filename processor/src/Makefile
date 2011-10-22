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
	ts.o \
	main.o

INCLUDES=../includes

CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -I$(INCLUDES)
LFLAGS = -Wall $(DEBUG) -I$(INCLUDES)
EXECUTABLE=abidos
SCRIPTS_DIR=../scripts/

all: clean check_directories $(EXECUTABLE) test_run

indent_run: clean $(EXECUTABLE) $(SCRIPTS_DIR)check_indent.sh $(SCRIPTS_DIR)indent_run.sh
	$(SCRIPTS_DIR)check_indent.sh
	$(SCRIPTS_DIR)indent_run.sh

check_directories: $(SCRIPTS_DIR)check_directories.pl
	$(SCRIPTS_DIR)check_directories.pl

.c.o:
	cc -c $(CFLAGS) $<

.cpp.o:
	$(CC) $(CFLAGS) $<

ts.o: ../../preprocessor/ts.cpp
	$(CC) $(CFLAGS) ../../preprocessor/ts.cpp

$(EXECUTABLE):  $(OBJS)
	$(CC) -o $(EXECUTABLE)  $(OBJS) $(LFLAGS)

# extract_symbols.pl tokens.h
parser.c parser.h: parser.y
	$(YACC) -dt parser.y
	mv -f y.tab.c parser.c
	mv -f y.tab.h $(INCLUDES)/parser.h
	$(SCRIPTS_DIR)extract_symbols.pl
	mv -f tokens.h $(INCLUDES)

lexer.c: lexer.l
	$(LEX) -t lexer.l >lexer.c

parser_descent.o: lex_yacc.h

lexer.o: parser.h

lex_yacc.h: parser.h

generator_class_diagram.cpp: generate_begin_graph.cpp

generate_begin_graph.cpp: ../python/out_begin_2.gv
	$(SCRIPTS_DIR)generate_begin_graph.pl

test_run: $(SCRIPTS_DIR)tests_run.pl $(EXECUTABLE)
	$(SCRIPTS_DIR)tests_run.pl

debug_put:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.bk
	$(SCRIPTS_DIR)put_printf_debug.pl
	cp parser_descent_rules.cpp_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_debug

debug_graph_put:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.bk
	$(SCRIPTS_DIR)put_trace_graph_debug.pl
	cp parser_descent_rules.cpp_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_debug

#FILE_TEST=t031.cpp
#FILE_TEST=mass_clases.cpp
#FILE_TEST=test_symbols_table.h
FILE_TEST=t035.cpp
execute: $(EXECUTABLE)
	cp ../python/xdot2.py ../out/
	./$(EXECUTABLE) --verbose --ts_show ../test/$(FILE_TEST)
	cat ../python/out_begin.py > ../out/out_run.py
	cat ../out/out_$(FILE_TEST).gv >> ../out/out_run.py
	cat ../python/out_end.py >> ../out/out_run.py
	chmod +x ../out/out_run.py
	../out/out_run.py

debug_drop:
	cp parser_descent_rules.cpp parser_descent_rules.cpp.debug.bk
	$(SCRIPTS_DIR)drop_printf_debug.pl
	cp parser_descent_rules.cpp_no_debug parser_descent_rules.cpp
	rm parser_descent_rules.cpp_no_debug

clean:
	rm -f *.o
	rm -f lexer.c parser.c parser.h tokens.h
	rm -f $(INCLUDES)/parser.h
	rm -f $(INCLUDES)/tokens.h
	rm -f *~
	rm -f *.bk
	rm -f $(EXECUTABLE)
	rm -f ../out/*
	rm -f ../test_out/*
	rm -f *.orig
	rm -f *.pyc
	rm -f ../test_out/*.pyc

