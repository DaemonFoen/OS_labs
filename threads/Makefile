TARGET = thread-example
SRCS = thread.c

TARGET = thread1.2
SRC = thread1.2.c

CC=gcc
RM=rm
CFLAGS= -g -Wall
LIBS=-pthread
INCLUDE_DIR="."

all: ${TARGET}

${TARGET}: ${SRCS}
	${CC} ${CFLAGS} -I${INCLUDE_DIR} ${SRCS} ${LIBS} -o ${TARGET}

clean:
	${RM} -f *.o ${TARGET}
