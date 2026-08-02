cc = gcc
cflags = -Wall -Wextra -Iinclude

src := $(wildcard src/*.c)
obj := $(src:.c=.o)

tornado: $(obj)
	$(cc) $(obj) -o tornado -lraylib -lm -lpthread -ldl -lrt -lX11

%.o: %.c
	$(cc) $(cflags) -c $< -o $@

clean:
	rm -f $(obj) tornado
