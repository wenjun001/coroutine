#
# @file  Makefile
# @author chenxueyou
# @version 0.1
# @brief   :A asymmetric coroutine library for C++
# History
#      1. Date: 2014-12-12 
#          Author: chenxueyou
#          Modification: this file was created 
#

all:
	gcc  coroutine.c  -g -c
	gcc  -pthread main.c -g -o main coroutine.o
clean:
	rm -f coroutine.o main
