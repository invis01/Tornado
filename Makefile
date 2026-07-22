cc = gcc
cflags = -Wall -Wextra -Iinclude

src := $(wildcard src/*.c)
obj := $(src:.c=.o)

tornado: $(obj)
	$(cc) $(obj) -o tornado

%.o: %.c
	$(cc) $(cflags) -c $< -o $@

clean:
	rm -f $(obj) tornado
