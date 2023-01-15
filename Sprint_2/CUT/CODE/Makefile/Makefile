TARGET=client server 
CC=gcc
CFLAGS= -Wall -g
normal: $(TARGET)
client: client.c
	$(CC) $(CFLAGS) client.c -o client
server: server.c
	$(CC) $(CFLAGS) server.c -o server -pthread
clean:
	$(RM) $(TARGET)
