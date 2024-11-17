CC = g++

SRC = src
LIB = $(SRC)/include
BLD = build

bldstruct = $(addprefix $(BLD)/, \
			$(shell find $(SRC) -type d))

testfiles = $(SRC)/test $(LIB)/board/can

.PHONY: genstruct
genstruct:
	@echo > /dev/null
	$(foreach d,$(bldstruct), \
	$(shell mkdir -p $(d)))

test: genstruct
	for f in $(testfiles); do \
		$(CC) -c $${f}.cc -o $(BLD)/$${f}.o; \
	done
	$(CC) $(addsuffix .o,
		  $(addprefix $(BLD)/,$(testfiles))) -o $(BLD)/test

.PHONY: clean
clean:
	rm -rf $(BLD)/*
