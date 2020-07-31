CC = gcc
LDFLAGS = $(shell pkg-config --libs --cflags libmongoc-1.0)
CFLAGS = -g -Wall -Wextra -Wpedantic -Wshadow -O2 -pthread
TARGET = url-shortener

$(TARGET): $(TARGET).o
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).o $(LDFLAGS)

$(TARGET).o: $(TARGET).c
	$(CC) $(CFLAGS) -c $(TARGET).c
clean:
	rm $(TARGET) $(TARGET).o
	
help:
	@echo	"Usage: make [target] ../n"
	@echo	"Miscellaneous:"
	@echo	"help\t\t\tShows this help\n"
	@echo	"Build:"
	@echo	"all\t\t\tBuild all the project\n"
	@echo	"Cleaning:"
	@echo	"clean\t\t\tRemove all intermediate objects"
