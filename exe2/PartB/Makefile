CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lcrypto -Wno-deprecated-declarations
TARGET = md5crack
SRC = md5_cracker.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)
