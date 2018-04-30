#
# Makefile para programas de teste
# 

FLAGS=-Wall -g
CC=gcc
LIB_DIR=../lib

all: test_cidentify test_csuspend

test_cidentify: test_cidentify.c $(LIB_DIR)/libcthread.a
	$(CC) $(FLAGS) -o test_cidentify test_cidentify.c -L$(LIB_DIR) -lcthread -Wall

test_csuspend: test_csuspend.c $(LIB_DIR)/libcthread.a
	$(CC) $(FLAGS) -o test_csuspend test_csuspend.c -L$(LIB_DIR) -lcthread -Wall
	
clean:
	rm -rf test_cidentify test_csuspend *.o *~




