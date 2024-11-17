CC = g++

SRC = src
LIB = $(SRC)/include
BLD = build

test:
	mkdir -p $(BLD)/{$(SRC),$(LIB)}
	mkdir -p $(BLD)/$(LIB)/board
	$(CC) -c $(SRC)/test.cc -o $(BLD)/$(SRC)/test.o
	$(CC) -c $(LIB)/board/can.cc -o $(BLD)/$(LIB)/board/can.o
	$(CC) $(BLD)/$(SRC)/test.o $(BLD)/$(LIB)/board/can.o -o $(BLD)/test

.PHONY: clean
clean:
	rm -rf $(BLD)/*
