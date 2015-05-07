# CpSc 322
# Makefile for "thread" management examples

CFLAGS = -Wall -g -m32

# Object Modules
BASEOBJS = threadLib.o listUtils.o tcleaner.o doSwitch.o setalarm.o \
           semaphore.o testandset.o idle.o
OBJS1 = testproc.o ${BASEOBJS}
OBJS2 = hw4test.o stack.o ${BASEOBJS}
OBJS3 = testpriority.o ${BASEOBJS}
OBJS4 = testmutex.o ${BASEOBJS}
OBJS5 = feedtest.o ${BASEOBJS}

CC = gcc

all: testproc hw4 testpriority testmutex feedtest

# Direct thread switch test
testproc: ${OBJS1}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS1} -m32
	@echo

# Timeslice switch test
hw4: ${OBJS2}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS2} -m32
	@echo

# childWait tests
testpriority: ${OBJS3}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS3} -m32
	@echo

.s.o:
	${CC} -c $< -o $@ -m32

# testmutex tests
testmutex: ${OBJS4}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS4} -m32
	@echo

# feedtest
feedtest: ${OBJS5}
	@echo
	@echo Linking $@
	${CC} -o $@ -g ${OBJS5} -m32
	@echo

#.s.o:
#	${CC} -c $< -o $@ -m32

${OBJS1}: userthread.h list.h semaphore.h Makefile
${OBJS2}: userthread.h list.h semaphore.h Makefile
${OBJS3}: userthread.h list.h semaphore.h Makefile
${OBJS4}: userthread.h list.h semaphore.h Makefile
${OBJS5}: userthread.h list.h semaphore.h Makefile

clean:
	rm -f *.o testproc testmutex testpriority feedtest hw4

test1:
	@echo $@: simple testproc test
	./testproc

test2:
	@echo $@: critical section test
	./testmutex

test3:
	@echo "$@: producer/consumer test (homework 4 solution)"
	./hw4

test4:
	@echo $@: priority inversion test
	./testpriority

test5:
	@echo $@: feedtest
	./feedtest
