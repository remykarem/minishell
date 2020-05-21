SOURCE_DIR=.
OBJECTS=$(SOURCE_DIR)/preview.c

CFLAGS=-std=c99 -Wall -Werror -pedantic

#need to have .exe extension for windows
TARGET=preview.exe


all: 
	cc -o $(TARGET) $(CFLAGS) $(OBJECTS) -Iinclude -lreadline


#for linux use rm $(TARGET)
#for windows use del $(TARGET)
clean:
	rm $(TARGET)
