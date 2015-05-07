# Makefile for schedulerSim
#
# Enter "make" to recompile/link program.

CFLAGS = -Wall -g 

# Object Modules
OBJS1 = simpletest.o simMem.o simUtils.o memManager.o memListUtils.o
OBJS2 = simMain.o simMem.o simUtils.o  memManager.o memListUtils.o


CC = gcc

# Link modules
all: simple memSim

simple: ${OBJS1}
	gcc -o simple -g ${OBJS1} -lpthread

memSim: ${OBJS2}
	gcc -o memSim -g ${OBJS2} -lpthread 

# Dependencies of modules on the include file
${OBJS1} : mem.h
${OBJS2} : sim.h mem.h

# Test Run: 0 Simple test
test0: 
	./simple

# Test Run: 1 thread/1 meg memory
test1: 
	./memSim 1 1024000 75000 620

# Test Run: 2 thread/300kb memory
test2: 
	./memSim 1 300000 75000 750


# Test Run: 3 thread/1 meg memory/620 byte max block size (should be okay)
test3: 
	./memSim 8 2048000 150000 620

# Test Run: 4 thread/1 meg memory/655 byte max block size (should run out)
test4: 
	./memSim 8 2048000 150000 655

clean:
	rm -f *.o memSim simple
