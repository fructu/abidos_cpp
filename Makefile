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

CFLAGS=-s

LIB=
#-------------------------------------------------------------------------------

todo: bison lex compilar linkar permisos borrar

bison: includes_yacc

lex: includes_lex

compilar:	compile_lex\
			compile_yacc\
			compile_semanti

linkar:	linkar_1

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
compile_semanti: includes_semantic.cpp
	$(CC) -O -c $(CFLAGS) includes_semantic.cpp

compile_lex:	includes_lex.c
	$(CC) -O -c $(CFLAGS) includes_lex.c

compile_lex:	includes_yacc..c
	$(CC) -O -c $(CFLAGS) includes_yacc.c

linkar_1: semantico.o \
		  web_filter_lexico.o \
		  web_filter_sintactico.c \
		  uri_scan.o
	$(CC) -o uri_scan \
		  semantico.o \
		  web_filter_lexico.o \
		  web_filter_sintactico.o \
		  uri_scan.o $(LIB)

#-------------------------------------------------------------------------------
permisos: uri_scan
	chmod 777 uri_scan
	
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
