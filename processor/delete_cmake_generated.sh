#!/bin/sh
rm	-f CMakeCache.txt
rm -rf CMakeFiles/
rm 	-f Makefile
rm 	-f cmake_install.cmake
rm 	-f includes/config.h
rm 	-f includes/parser.h
rm 	-f includes/tokens.h
#rm 	-f "src/\n  /"
rm 	-f src/CMakeCache.txt
rm -rf src/CMakeFiles/
#	src/Makefile.bk
rm  -f src/cmake_install.cmake
rm 	-f src/lexer.c
#	src/make_out.txt
rm	-f src/parser.c
rm 	-f src/ts.cpp
rm 	-f src/ts.h
rm 	-f src/abidos
rm 	-f src/Makefile

rm  -rf includes/CMakeFiles/
rm  -f	includes/Makefile
rm 	-r  includes/cmake_install.cmake
rm  -rf out/*
rm  -rf test_out/*

rm  -rf src/.abidos/
rm  -f  install_manifest.txt
rm  -rf .abidos/

rm CTestTestfile.cmake
rm  -rf Testing/
rm src/CTestTestfile.cmake

rm -rf build/
