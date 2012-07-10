#
# Makefile for sorting algorithms
#

DEFS	= -g
#DEFS	=

#CC	= gcc 
CC	= cc

program: hashval hashhist hashtest

hashval: hashval.o hashfun.o
	$(CC) $(DEFS) -o hashval hashval.o hashfun.o

hashval.o: hashval.c hashfun.h
	$(CC) $(DEFS) -c hashval.c

hashhist: hashhist.o hashfun.o
	$(CC) $(DEFS) -o hashhist hashhist.o hashfun.o

hashhist.o: hashhist.c hashfun.h
	$(CC) $(DEFS) -c hashhist.c

hashtest: hashtest.o hashfun.o linear.o quadratic.o
	$(CC) $(DEFS) -o hashtest hashtest.o hashfun.o linear.o quadratic.o

hashtest.o: hashtest.c hash.h hashfun.h
	$(CC) $(DEFS) -c hashtest.c

linear.o: linear.c hash.h
	$(CC) $(DEFS) -c linear.c

quadratic.o: quadratic.c hash.h
	$(CC) $(DEFS) -c quadratic.c

hashfun.o: hashfun.c
	$(CC) $(DEFS) -c hashfun.c

clean:
	rm -f *~ *.o

allclean:
	rm -f *~ *.o hashval hashhist hashtest
