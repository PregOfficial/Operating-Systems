CFLAGS=-std=gnu11 -Wall -Werror -Wextra -O3
LDLIBS=-ldl -pthread
BINS=system_allocator.so membench

all: $(BINS)

%.so: %.c
	$(CC) $(CFLAGS) -fPIC -shared -o $@ $^

membench: membench.c

clean:
	$(RM) $(BINS)