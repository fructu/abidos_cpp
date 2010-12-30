#-------------------------------------------------------------------------------
#
#       Proyecto             : 
#       Fichero              : Makefile
#       Codigo               : 
#       Descripcion          : 
#       Version              : 0.0.01
#       Autor                : F. Manuel Hevia Garcia
#       Fecha creacion	     : 
#       Fecha modificacion   : 
#
#       Observaciones :
#
#         ->
#
#-------------------------------------------------------------------------------

#-------------------------------------------------------------------------------
# Varibales
#-------------------------------------------------------------------------------

CC=c++

CFLAGS=-s -Wall

EXEC=show_defines

LIB=
#-------------------------------------------------------------------------------

all: bison lex compile link permisos

bison: includes_yacc

lex: includes_lex

compile:	compile_lex\
			compile_yacc\
			compile_ts\
			compile_semantic\
			compile_main

link:	link_1

ejecutar:	ejecutar_p1_main
#-------------------------------------------------------------------------------
includes_yacc: includes_yacc.yac
	bison -v -d includes_yacc.yac
	mv includes_yacc.tab.cac includes_yacc.c
	mv includes_yacc.tab.hac includes_yacc.h
	rm -f includes_yacc.output

includes_lex: includes_lex.lex
	flex -t includes_lex.lex > includes_lex.c

#	flex includes_lex.lex
#	mv lex.yy.c includes_lex.c

#-------------------------------------------------------------------------------
compile_ts: ts.cpp
	$(CC) -O -c $(CFLAGS) ts.cpp

compile_semantic: includes_semantic.cpp
	$(CC) -O -c $(CFLAGS) includes_semantic.cpp

compile_lex:	includes_lex.c
	$(CC) -O -c $(CFLAGS) includes_lex.c

compile_yacc:	includes_yacc.c
	$(CC) -O -c $(CFLAGS) includes_yacc.c

compile_main:	main.cpp
	$(CC) -O -c $(CFLAGS) main.cpp

link_1: includes_semantic.o \
		  ts.o \
		  includes_lex.o \
		  includes_yacc.o \
		  main.o
	$(CC) -o $(EXEC) \
		  main.o \
		  ts.o  \
		  includes_semantic.o  \
		  includes_lex.o \
		  includes_yacc.o $(LIB)


input_generate: input.txt
	find . -name "*.cpp" > input.txt
	find . -name "*.c" >> input.txt
	find . -name "*.h" >> input.txt
	sed -e 's/^\.\///g' input.txt > input.txt_sed
	mv -f input.txt_sed input.txt

#-------------------------------------------------------------------------------
permisos: $(EXEC)
	chmod 777 $(EXEC)
	
clean:
	rm -f *.o
	rm -f *.bak
	rm -f core.*
	rm -f *.tab.cac
	rm -f *.tab.hac
	rm -f *.output
	rm -f includes_lex.c
	rm -f includes_yacc.c
	rm -f includes_yacc.h

execute: $(EXEC)
	./$(EXEC)
	dot out.gv -Tps -o out.ps
	evince out.ps
#	eog out.ps
#-------------------------------------------------------------------------------
