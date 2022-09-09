CC := clang++
INCLUDE_DIR := ./include
TARGET_FILE := src/main.cpp
CPP_STANDARD := c++17
ARGS := -Wall -Wextra -Wpedantic -Wenum-compare
OUTPUT_FILE := ./build/watcher

.PHONY: build

build:	
	[ -d build ] || mkdir build 
	
	$(CC) $(TARGET_FILE) -I/usr/local/include -I$(INCLUDE_DIR) --std=$(CPP_STANDARD) $(ARGS) -o $(OUTPUT_FILE)

test:
	$(OUTPUT_FILE) ./watcher.json

ci:
	make build
	make test