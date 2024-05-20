CC=gcc
STD=c11
CFLAGS=-Wall \
	-Wextra \
	-pedantic \
	-Wno-stringop-truncation \
	-Wno-unknown-warning-option \
	-DCCC_DEBUG \
	-DCCC_USE_LIBC_INTRINSICS \
	-DCCC_ENABLE_GNU_FEATURES \
	-DCCC_RESTRICT_REFERENCES \
	-DCCC_ENABLE_SIZE128 \
	-flto \
	-O2
LDFLAGS=-flto

LIBS=ccc ccc_coll

all: arlibs dynlibs

arlibs: ${LIBS:%=lib%.a}

dynlibs: ${LIBS:%=lib%.so}

CCC_SRC=$(wildcard ccc/*.c)
CCC_HDR=$(wildcard ccc/*.h)
CCC_OBJ=${CCC_SRC:ccc/%.c=%.o}
${CCC_OBJ}: ${CCC_SRC} ${CCC_HDR}
	${CC} -std=${STD} ${CFLAGS} -c ${CCC_SRC}

libccc.a: ${CCC_OBJ}
	${AR} rcs $@ ${CCC_OBJ}

libccc.so: ${CCC_OBJ}
	${CC} ${LDFLAGS} -shared -o $@ ${CCC_OBJ}

COLL_SRC=$(wildcard ccc_coll/*.c)
COLL_HDR=$(wildcard ccc_coll/*.h)
COLL_OBJ=${COLL_SRC:ccc_coll/%.c=%.o}
COLL_INCLUDE= -I ../ccc
${COLL_OBJ}: ${COLL_SRC} ${COLL_HDR}
	${CC} -std=${STD} ${CFLAGS} ${COLL_INCLUDE} -c ${COLL_SRC}

libccc_coll.a: ${COLL_OBJ}
	${AR} rcs $@ ${COLL_OBJ}

libccc_coll.so: ${COLL_OBJ}
	${CC} ${LDFLAGS} -shared -o $@ ${COLL_OBJ}

TEST_SRC=test.c buffmt.c
TEST_HDR=buffmt.h
TEST_AR_LIBS=libccc.a libccc_coll.a
TEST_INCLUDE= -I ../ccc -I ../ccc_coll
test_bin: ${TEST_SRC} ${TEST_HDR} ${TEST_AR_LIBS}
	${CC} -std=${STD} ${CFLAGS} ${LDFLAGS} ${TEST_INCLUDE} -o $@ ${TEST_SRC} ${TEST_AR_LIBS}

.PHONY:
test: test_bin
	./test_bin

.PHONY:
clean:
	${RM} *.o
	${RM} *.a
	${RM} *.so
	${RM} test

.PHONY:
lsp: clean
	bear -- make -C . all test
