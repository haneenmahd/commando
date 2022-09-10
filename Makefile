CC := clang++
INCLUDE_DIR := ./include
TARGET_FILE := src/main.cpp
CPP_STANDARD := c++17
ARGS := -Wall -Wextra -Wpedantic -Wenum-compare
OUTPUT_FILE := ./build/commando

.PHONY: clean-build, test, dev

build:	
	[ -d build ] || mkdir build 
	
	$(CC) $(TARGET_FILE) -I/usr/local/include -I$(INCLUDE_DIR) --std=$(CPP_STANDARD) $(ARGS) -o $(OUTPUT_FILE)

clean-build:
	[ -d build ] || mkdir build 
	
	$(CC) $(TARGET_FILE) -I/usr/local/include -I$(INCLUDE_DIR) --std=$(CPP_STANDARD) $(ARGS) -o $(OUTPUT_FILE)

test:
	$(OUTPUT_FILE) ./commando.json

ci:
	make build
	make test

dev:
	make clean-build
	make test