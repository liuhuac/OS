# CpSc 322: Makefile for program 5

OBJS=vmmain.o pageFault.o MMU.o utils.o

CC=gcc
CFLAGS= -g -Wall

vmsim: ${OBJS}
	${CC} -o vmsim ${OBJS}

${OBJS}: vmsim.h Makefile

test1:
	./vmsim shortTrace.txt  4 

test2:
	./vmsim shortTrace.txt 10

test3:
	./vmsim shortTrace.txt 20

test4:
	./vmsim shortTrace.txt 40

clean:
	rm vmsim *.o
