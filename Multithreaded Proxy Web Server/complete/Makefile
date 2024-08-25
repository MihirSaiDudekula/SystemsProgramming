CC=g++
CFLAGS= -g -Wall

all: proxy

proxy: proxy_server_with_cache.c
	$(CC) $(CFLAGS) -o proxy_parse.o -c proxy_parse.c -lpthread
	$(CC) $(CFLAGS) -o proxy.o -c proxy_server_with_cache.c -lpthread
	$(CC) $(CFLAGS) -o proxy proxy_parse.o proxy.o -lpthread

clean:
	rm -f proxy *.o

# CC = g++
# CFLAGS = -g -Wall
# LDFLAGS = -lpthread

# # The final target executable
# all: proxy

# # Rule to link the final executable
# proxy: proxy_parse.o proxy.o
# 	$(CC) $(CFLAGS) -o proxy proxy_parse.o proxy.o $(LDFLAGS)

# # Rule to compile proxy_parse.o
# proxy_parse.o: proxy_parse.c proxy_parse.h
# 	$(CC) $(CFLAGS) -c proxy_parse.c

# # Rule to compile proxy.o
# proxy.o: proxy_server_with_cache.c proxy_parse.h
# 	$(CC) $(CFLAGS) -c proxy_server_with_cache.c

# # Clean up build artifacts
# clean:
# 	rm -f proxy *.o
