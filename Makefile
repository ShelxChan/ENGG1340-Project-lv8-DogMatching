CC = g++
CFLAGS = -std=c++11 -Wall
# Use ncursesw instead of ncurses to support UTF-8 wide characters.
LIBS = -lncursesw

# Target file
OBJS = main.o DogMatchingGame.o

all: dog_matching

dog_matching: $(OBJS)
	$(CC) $(CFLAGS) -o dog_matching $(OBJS) $(LIBS)

main.o: main.cpp DogMatchingGame.h
	$(CC) $(CFLAGS) -c main.cpp

DogMatchingGame.o: DogMatchingGame.cpp DogMatchingGame.h DogBreed.h
	$(CC) $(CFLAGS) -c DogMatchingGame.cpp

run: dog_matching
	./dog_matching

clean:
	rm -f dog_matching $(OBJS)

.PHONY: all clean run 