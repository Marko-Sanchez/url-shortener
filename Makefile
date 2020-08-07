CC = gcc
LDFLAGS = $(shell pkg-config --libs --cflags libmongoc-1.0)
CFLAGS = -g -Wall -Wextra -Wpedantic -Wshadow -O2 -pthread -I $(LDFLAGS)
DEPS = functions.h 
OBJ = url-shortener.o functions.o
TARGET = url-shortener

%.o:%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm $(TARGET) $(OBJ) 
	
help:
	@echo	"Usage: make [target] ../n"
	@echo	"Miscellaneous:"
	@echo	"help\t\t\tShows this help\n"
	@echo	"Build:"
	@echo	"all\t\t\tBuild all the project\n"
	@echo	"Cleaning:"
	@echo	"clean\t\t\tRemove all intermediate objects"
