#!/bin/sh
# Verification program to check C++ Beautifiers 'bcpp', 'indent' or cb
############################################################
# Copyright 
# The copyright policy is GNU/GPL.
# Author: Al Dev (Alavoor Vasudevan) alavoor[AT]yahoo.com
############################################################
check_beautify_now()
{
	# Remove all the temp files....
	\rm -f ${TMP_FILE}
	\rm -f ${TMP_CPPFILE}*.*
	FNAME=$1
	echo "file" ${FNAME}
	if [ ! -f ${FNAME} ]; then
		echo "Error: The file["${FNAME}"] does not exist!!. Aborting now ...."
		exit 1
	fi
	\cp  -f ${FNAME} ${TMP_CPPFILE}.cpp
	${COMPILER} -c ${TMP_CPPFILE}.cpp -I../includes
	if [ ! -f ${TMP_CPPFILE}.o ]; then
		echo "Fatal Error: Failed to compile ["${FNAME}"]. Aborting now... "
		exit 1
	fi
	\mv -f ${TMP_CPPFILE}.o ${TMP_CPPFILE}_orig.o
	aa=`basename $PRGM`
	echo "Running, verifying ["$aa"] on ["${FNAME}"]"
	echo ${PRGM} ${TMP_CPPFILE}.cpp
	${PRGM} ${TMP_CPPFILE}.cpp
	echo ${COMPILER} -c ${TMP_CPPFILE}.cpp -I../includes
	${COMPILER} -c ${TMP_CPPFILE}.cpp -I../includes
	\rm -f $TMP_FILE
	diff ${TMP_CPPFILE}.o ${TMP_CPPFILE}_orig.o 1> $TMP_FILE 2>> $TMP_FILE
	result=""
	result=`wc -c $TMP_FILE | awk '{print $1}' `
	if [ "$result" = "0" ]; then
		echo "success!! Beautifier [" $aa "] is working properly!!"
	else
		echo "fatal Error Beautifier is not working!!"
		exit 1
	fi
#	${COMPILER} -S ${TMP_CPPFILE}.cpp -I../includes
#	diff ${TMP_CPPFILE}.s ${TMP_CPPFILE}_orig.s
# Remove all the temp files....
	\rm -f ${TMP_FILE}
	\rm -f ${TMP_CPPFILE}*.*
}
########## Main of program begins here ##################3
#PRGM=/usr/bin/bcpp
#PRGM=/usr/bin/cb
PRGM='astyle --style=k&r'
COMPILER=/usr/bin/g++
TMP_FILE=beautify.tmp
TMP_CPPFILE=beautify-tmp_cppfile
# Remove all the temp files....
\rm -f ${TMP_FILE}
\rm -f ${TMP_CPPFILE}*.*

for f in $( git ls-files *.cpp ); do
		check_beautify_now ${f}
		echo ""
done

cd ../includes
for f in $( git ls-files *.h ); do
		check_beautify_now ${f}
		echo ""
done
cd ../src
