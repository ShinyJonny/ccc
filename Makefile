CC=clang
STD=c11
CFLAGS=\
	-Wall \
	-Wextra \
	-pedantic \
	-Wno-unknown-pragmas \
	-DCCC_DEBUG \
	-DCCC_USE_LIBC_INTRINSICS \
	-DCCC_ENABLE_GNU_FEATURES \
	-DCCC_RESTRICT_REFERENCES \
	-DCCC_ENABLE_SIZE128 \
	-flto \
	-O2 \

LDFLAGS=-flto

all: testbin

CCC_HDR=$(wildcard ccc/*.h)
CCC_COLL_HDR=$(wildcard ccc_coll/*.h)

TEST_SRC=test.c buffmt.c
TEST_HDR=buffmt.h ${CCC_HDR} ${CCC_COLL_HDR}
TEST_INCLUDE= -I ../ccc -I ../ccc_coll
testbin: ${TEST_SRC} ${TEST_HDR}
	${CC} -std=${STD} ${CFLAGS} ${LDFLAGS} ${TEST_INCLUDE} -o $@ ${TEST_SRC}

.PHONY:
test: testbin
	./testbin

.PHONY:
clean:
	${RM} *.o
	${RM} *.a
	${RM} *.so
	${RM} testbin

.PHONY:
lsp: clean
	bear --config bear.conf -- make -C . all
