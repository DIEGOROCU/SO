CC = gcc
CFLAGS = -Wall -g

PROG = show_file
OBJECTS = $(PROG).o

all : $(PROG)

$(PROG) : $(OBJECTS)
	$(CC) -g -o $(PROG) $(OBJECTS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean :
	-rm -f $(OBJECTS) $(PROG)
