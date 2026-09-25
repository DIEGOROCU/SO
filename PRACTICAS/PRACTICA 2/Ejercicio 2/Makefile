CC = gcc
CFLAGS = -Wall -g

PROGS = write_strings read_strings

all : $(PROGS)

$(PROGS) : % : %.o
	$(CC) -g -o $@ $^

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	-rm -f *.o $(PROGS)
