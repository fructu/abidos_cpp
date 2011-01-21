#!/bin/bash
find . -name "*.cpp" > input.txt
find . -name "*.c" >> input.txt
find . -name "*.h" >> input.txt
find . -name "*.cc" >> input.txt
sed -e 's/^\.\///g' input.txt > input.txt_sed
mv -f input.txt_sed input.txt
/opt/hevia_tools/show_includes -batch input.txt -follow -print -no-sharp 
eog    out.gv -Tpng -o out_fdp.png
evince out_fdp.png
