#compiler
CC = gcc

#header flags
CFLAGS = -I"C:/Program Files/MySQL/MySQL Connector C 6.1/include" -I"./headers"

#linker flags
LDFLAGS = "C:/Program Files/MySQL/MySQL Connector C 6.1/lib/libmysql.dll"

#source codes
SRC = ./src/*.c

#headers
HEADS = ./headers/*.h

#objects


#target indicate
TARGET = ./target/main


#compile
#burada prerequsite le birlikte normal bir rule yaz ve out-of-date durumlarini dene.
TARGET: $(SRC) $(HEADS)
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(TARGET)


