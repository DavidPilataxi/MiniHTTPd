CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Iinclude

SRC = src/main.c \
      src/server.c \
      src/http.c \
      src/mime.c \
      src/files.c

OBJ = $(SRC:.c=.o)

TARGET = minihttpd

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

clean:
	rm -f $(OBJ) $(TARGET)