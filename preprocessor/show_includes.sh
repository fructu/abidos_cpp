#!/bin/bash
find . -name "*.cpp" > /tmp/hevia/input.txt
find . -name "*.c" >> /tmp/hevia/input.txt
find . -name "*.h" >> /tmp/hevia/input.txt
find . -name "*.cc" >> /tmp/hevia/input.txt
sed -e 's/^\.\///g' /tmp/hevia/input.txt > /tmp/hevia/input.txt_sed
mv -f /tmp/hevia/input.txt_sed /tmp/hevia/input.txt
/opt/hevia_tools/show_includes -batch /tmp/hevia/input.txt -follow -print -no-sharp
mv out.gv /tmp/hevia/
#eog    out.gv -Tpng -o out_fdp.png
#evince out_fdp.png
cat /opt/hevia_tools/out_begin.py > /tmp/hevia/out_run.py
cat /tmp/hevia/out.gv >> /tmp/hevia/out_run.py
cat /opt/hevia_tools/out_end.py >> /tmp/hevia/out_run.py
chmod +x /tmp/hevia/out_run.py
cp /opt/hevia_tools/xdot.py /tmp/hevia/
/tmp/hevia/out_run.py

