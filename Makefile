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
	flex includes_lex.lex
	mv lex.yy.c includes_lex.c

#-------------------------------------------------------------------------------
compile_semantic: includes_semantic.cpp
	$(CC) -O -c $(CFLAGS) includes_semantic.cpp

compile_lex:	includes_lex.c
	$(CC) -O -c $(CFLAGS) includes_lex.c

compile_yacc:	includes_yacc.c
	$(CC) -O -c $(CFLAGS) includes_yacc.c

compile_main:	main.cpp
	$(CC) -O -c $(CFLAGS) main.cpp

link_1: includes_semantic.o \
		  includes_lex.o \
		  includes_yacc.o \
		  main.o
	$(CC) -o $(EXEC) \
		  main.o \
		  includes_semantic.o  \
		  includes_lex.o \
		  includes_yacc.o $(LIB)

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
#-------------------------------------------------------------------------------
