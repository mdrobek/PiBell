#!/bin/sh
# ./ctags_with_dep.sh file1.c file2.c ... to generate a tags file for these files

gcc -M -I/usr/lib/x86_64-linux-gnu $* | sed -e 's/[\\ ]/\n/g' | \
        sed -e '/^$/d' -e '/\.o:[ \t]*$/d' | \
        ctags -L - --c++-kinds=+p --fields=+iaS --extra=+q
