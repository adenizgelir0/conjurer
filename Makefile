CC ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -g -O2
CPPFLAGS ?= -Iinclude

BUILD := build
BIN := $(BUILD)/bin
OBJ := $(BUILD)/obj

CORE := $(filter-out src/main.c,$(wildcard src/*.c))
OBJS := $(CORE:src/%.c=$(OBJ)/%.o)
HEADERS := $(wildcard include/*.h)

.PHONY: all clean run test parse forms

all: $(BIN)/conjurer $(BIN)/test_conj $(BIN)/parse_expr $(BIN)/print_forms

$(BIN) $(OBJ):
	mkdir -p $@

$(OBJ)/%.o: src/%.c $(HEADERS) | $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

$(BIN)/conjurer: src/main.c $(OBJS) $(HEADERS) | $(BIN)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ src/main.c $(OBJS)

$(BIN)/test_conj: tests/test_conj.c $(OBJS) $(HEADERS) | $(BIN)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ tests/test_conj.c $(OBJS)

$(BIN)/parse_expr: tools/parse_expr.c $(OBJS) $(HEADERS) | $(BIN)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ tools/parse_expr.c $(OBJS)

$(BIN)/print_forms: tools/print_forms.c $(OBJS) $(HEADERS) | $(BIN)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ tools/print_forms.c $(OBJS)

run: $(BIN)/conjurer
	./$<

test: $(BIN)/test_conj
	./$<

parse: $(BIN)/parse_expr
	./$<

forms: $(BIN)/print_forms
	./$<

clean:
	rm -rf $(BUILD)
