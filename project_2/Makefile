VECTOR_ADD := ./bin/vector_add
SORT := ./bin/sort
STRIDED := ./bin/strided_access

CC := g++
CPP_SUFFIX := cpp

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

# sort
sort: $(BENCHMARKS_DIR)/sort.cpp
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/sort.cpp -o $(SORT) $(LIBS)

# sort
strided_access: $(BENCHMARKS_DIR)/strided_access.cpp
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/strided_access.cpp -o $(STRIDED) $(LIBS)

clean:
	-rm $(VECTOR_ADD) $(SORT) $(STRIDED)
