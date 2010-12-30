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

bison: web_filter_bison web_filter_bison_renombrar

lex: includes_lex

compilar:	compilar_semantico\
			compilar_web_filter_lex\
			compilar_web_filter_yacc\
			compilar_main

compilar_1:	compilar_prueba_1

linkar:	linkar_1

ejecutar:	ejecutar_p1_main
#-------------------------------------------------------------------------------

web_filter_bison: web_filter_sintactico.yac
	bison -v -d web_filter_sintactico.yac
	
web_filter_bison_renombrar:
	cp web_filter_sintactico.tab.cac web_filter_sintactico.c
	rm -f web_filter_sintactico.tab.cac
	cp web_filter_sintactico.tab.hac web_filter_sintactico.h
	rm -f web_filter_sintactico.tab.hac	

includes_lex: includes_lex.lex
	flex includes_lex.lex
	mv lex.yy.c includes_lex.c

#-------------------------------------------------------------------------------
compilar_semantico: semantico.cpp
	$(CC) -O -c $(CFLAGS) semantico.cpp

compilar_web_filter_lex:	web_filter_lexico.c
	$(CC) -O -c $(CFLAGS) web_filter_lexico.c
	
compilar_web_filter_yacc: web_filter_sintactico.c	
	$(CC) -O -c $(CFLAGS) web_filter_sintactico.c

compilar_main: uri_scan.cpp
	$(CC) -O -c $(CFLAGS) uri_scan.cpp

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
	rm -f includes_lex.c
#-------------------------------------------------------------------------------
