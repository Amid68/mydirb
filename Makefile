CC = gcc
CFLAGS = -Wall -g
LIBS = -lcurl

TARGET = mydirb
SRC = main.c get_url.c wordlist.c

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET)

