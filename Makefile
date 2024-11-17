CC = g++

SRC = src
LIB = $(SRC)/include
BLD = build

define BLD_OBJS
	$(addsuffix .o,
	$(addprefix $(BLD)/,$(1)))
endef

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
	$(CC) $(call BLD_OBJS,$(testfiles)) -o $(BLD)/test

.PHONY: clean
clean:
	rm -rf $(BLD)/*
