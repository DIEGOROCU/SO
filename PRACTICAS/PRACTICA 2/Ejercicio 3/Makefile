CC = gcc
CFLAGS = -Wall -g

PROGS = write_records_text read_records_text write_records_bin read_records_bin conversion

all : $(PROGS)

$(PROGS) : % : %.o
	$(CC) -g -o $@ $^

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	-rm -f *.o $(PROGS)
