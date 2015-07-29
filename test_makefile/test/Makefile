OBJ_FILE := main.o fun.o
TESTLIB_FILE := ../testlib/libtestlib.a
TESTLIB_DIR := ../testlib/


all : main

main : $(OBJ_FILE) $(TESTLIB_FILE)
	#gcc $(OBJ_FILE) -o $@ -I./include -I../testlib/ -L./lib -L../testlib/ -ltestlib
	gcc $(OBJ_FILE) -o $@ -I./include -I../testlib/ -L../testlib/ -ltestlib

$(OBJ_FILE) : ./src/main.c ./src/fun.c
	gcc -I./include -I../testlib -c ./src/main.c -o main.o
	gcc -I./include -c ./src/fun.c -o fun.o

$(TESTLIB_FILE) : 
	cd $(TESTLIB_DIR) && $(MAKE) --directory=$(TESTLIB_DIR)

clean :
	cd $(TESTLIB_DIR) && $(MAKE) clean --directory=$(TESTLIB_DIR)
	-rm *.o
	-rm main

.PHONY: all clean
