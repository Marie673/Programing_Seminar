PROG := client
SRCS := HTTP_client.c send_request.c
OBJS := $(SRCS:%.c=%.o)

CC := gcc

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) -c $<