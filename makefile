TARGET= main.out
CC= gcc
CFLAGS= -ansi -g -pedantic
OBJS= main.o MultiCaster.o

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)

MultiCaster.o: MultiCaster.c MultiCaster.h
	$(CC) $(CFLAGS) -c MultiCaster.c
	
main.o: main.c
	$(CC) $(CFLAGS) -c main.c
	
clean:
	rm -f $(TARGET) $(OBJS)
