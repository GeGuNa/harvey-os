#!/bin/sh
mkdir -p ../../../../linux_amd64/bin
9c -Wno-char-subscripts -g -I .  asm.c list.c obj.c optab.c pass.c span.c unixcompat.c ../6c/enam.c ../ld/elf.c

9l -o 6l \
asm.o \
enam.o \
elf.o \
list.o \
obj.o \
optab.o \
pass.o \
span.o \
unixcompat.o 

cp 6l ../../../../linux_amd64/bin
