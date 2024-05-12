.PHONY: all clean

CC := gcc
DEPS := umem-user.h ../umem.h
OBJS := main.o umem-user.o

all: main

main: $(OBJS)
	$(CC) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $<

clean:
	rm -f $(OBJS) main
