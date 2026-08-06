cc = gcc
cflags = -Wall -Wextra -Iinclude

src := $(wildcard src/*.c)
obj := $(src:.c=.o)

tornado: $(obj)
	$(cc) $(obj) -o tornado -lraylib -lm -lpthread -ldl -lrt -lX11 -lcjson -g

%.o: %.c
	$(cc) $(cflags) -c $< -o $@ -g

clean:
	rm -f $(obj) tornado
