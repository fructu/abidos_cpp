#!/bin/sh
# Verification program to check C++ Beautifiers 'bcpp', 'indent' or cb
############################################################
# Copyright 
# The copyright policy is GNU/GPL.
# Author: Al Dev (Alavoor Vasudevan) alavoor[AT]yahoo.com
############################################################
run_beautify_now()
{
	FNAME=$1
	echo "file["${FNAME}"]"
	if [ ! -f ${FNAME} ]; then
		echo "Error: The file["${FNAME}"] does not exist!!. Aborting now ...."
		exit
	fi
	aa=`basename $PRGM`
	echo "Running, ["$aa"] on ["${FNAME}"]"
	echo ${PRGM} ${FNAME}
	${PRGM} ${FNAME}
}
########## Main of program begins here ##################3
#PRGM=/usr/bin/bcpp
#PRGM=/usr/bin/cb
PRGM='astyle --style=k&r'
COMPILER=/usr/bin/g++
TMP_FILE=beautify.tmp
TMP_CPPFILE=beautify-tmp_cppfile
#echo "Enter the C++ file name <default is *.cpp> : "
#read ans
for f in $( git ls-files *.cpp ); do
		run_beautify_now ${f}
		echo ""
done

cd ../includes
for f in $( git ls-files *.h ); do
		run_beautify_now ${f}
		echo ""
done
cd ../src
