CFLAGS := -ansi -pedantic-errors

CWARNINGS := all error
CFLAGS += $(addprefix -W,$(CWARNINGS))

all: main
	gcc -o main $(CFLAGS) main.c

