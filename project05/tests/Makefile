C_FILES := $(wildcard *.c)
TARGETS := ${C_FILES:.c=}

all: ${TARGETS}

%: %.c
	gcc -g -m32 -L.. -o $@ $< -lmem -Xlinker -rpath=.

clean:
	rm -rf ${TARGETS} *.o
