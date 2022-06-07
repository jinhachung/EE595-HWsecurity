VECTOR_ADD := ./bin/vector_add
SORT := ./bin/sort
STRIDED := ./bin/strided_access
CACHE_INTENSIVE := ./bin/cache_intensive
CACHE_PROBE := ./bin/cache_probe

CC := g++
CPP_SUFFIX := cpp

INCLUDE_DIR := -I./include
SRC_DIR := ./src
BENCHMARKS_DIR := ./src/benchmarks

CFLAGS := -g -Wall -std=c++11 -O0
LDFLAGS :=
LIBS :=

# add sources
SRC = $(wildcard $(SRC_DIR)/*.$(CPP_SUFFIX))

# vector add
vector_add: $(BENCHMARKS_DIR)/vector_add.cpp
	mkdir -p bin
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/vector_add.cpp -o $(VECTOR_ADD) $(LIBS)

# sort
sort: $(BENCHMARKS_DIR)/sort.cpp
	mkdir -p bin
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/sort.cpp -o $(SORT) $(LIBS)

# sort
strided_access: $(BENCHMARKS_DIR)/strided_access.cpp
	mkdir -p bin
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/strided_access.cpp -o $(STRIDED) $(LIBS)

cache_intensive: $(BENCHMARKS_DIR)/cache_intensive.cpp
	mkdir -p bin
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/cache_intensive.cpp -o $(CACHE_INTENSIVE) $(LIBS)

cache_probe: $(BENCHMARKS_DIR)/cache_probe.cpp
	mkdir -p bin
	$(CC) $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) $(SRC) $(BENCHMARKS_DIR)/cache_probe.cpp -o $(CACHE_PROBE) $(LIBS)

all: vector_add sort strided_access cache_intensive

clean:
	-rm -r ./bin
