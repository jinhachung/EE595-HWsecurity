VECTOR_ADD := ./bin/vector_add

CC := g++
CPP_SUFFIC := cpp

INCLUDE_DIR := -I./include
SRC_DIR := ./src
BENCHMARKS_DIR := ./src/benchmarks

CFLAGS := -g -Wall -std=c++11
LDFLAGS :=
LIBS :=

# add sources
SRC = $(wildcard $(SRC_DIR)/*.$(CPP_SUFFIX))

# vector add
vector_add: $(BENCHMARKS_DIR)/vector_add.cpp
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/vector_add.cpp -o $(VECTOR_ADD) $(LIBS)

clean:
	-rm $(VECTOR_ADD)
