OBJS=client.o lib.o

client: $(OBJS)
	gcc -Wall -Wextra -g -o $@ $^

lib.o: lib.c lib.h
	gcc -Wall -Wextra -c $<

.PHONY: clean

clean:
	rm -f $(OBJS)