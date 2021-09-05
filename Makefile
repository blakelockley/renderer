# Usage:
# make        		# compile sample
# make clean  		# remove output files

CC = gcc
CFLAGS = -Wall -g -Iincludes
LFLAGS = -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit

TARGET = main
SRCS   = ${wildcard src/*.c}

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(LFLAGS) $(SRCS) -o $(TARGET)

.PHONY: clean
clean:
	rm $(TARGET)
