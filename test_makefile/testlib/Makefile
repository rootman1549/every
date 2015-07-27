OBJS := testlib.o 

all : libtestlib.a

libtestlib.a : $(OBJS)
	ar rc libtestlib.a $(OBJS)
	#mv libtestlib.a ../test/lib/ 

$(OBJS) : testlib.c
	gcc -c testlib.c -o $(OBJS)

clean :
	-rm *.o
	-rm *.a

.PHONY : all clean
