# CpSc 322
# Makefile for "thread" management examples

CFLAGS = -Wall -g -m32

# Object Modules
BASEOBJS = threadLib.o listUtils.o tcleaner.o doSwitch.o setalarm.o
OBJS1 = testproc.o ${BASEOBJS}
OBJS2 = ts.o ${BASEOBJS}
OBJS3 = testwait.o ${BASEOBJS}
OBJS4 = testsuspend.o ${BASEOBJS}

CC = gcc

all: testproc ts testwait testsuspend

# Direct thread switch test
testproc: ${OBJS1}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS1} -m32
	@echo

# Timeslice switch test
ts: ${OBJS2}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS2} -m32
	@echo

# childWait tests
testwait: ${OBJS3}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS3} -m32
	@echo

.s.o:
	${CC} -c $< -o $@ -m32

# testkill tests
testsuspend: ${OBJS4}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS4} -m32
	@echo

${OBJS1}: userthread.h list.h Makefile
${OBJS2}: userthread.h list.h Makefile
${OBJS3}: userthread.h list.h Makefile
${OBJS4}: userthread.h list.h Makefile

clean:
	rm -f *.o ts testproc testsuspend testwait
