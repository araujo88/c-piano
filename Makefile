CC=gcc
CC_FLAGS=-g -Wall -Wextra -Wpedantic
CC_LIBS=-lasound -lm -pthread

SRC_DIR=src
HDR_DIR=include
OBJ_DIR=obj

# source files
SRC_FILES=$(SRC_DIR)/main.c $(HDR_DIR)/inih/ini.c
OBJ_FILES=$(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRC_FILES)))

VPATH = $(sort $(dir $(SRC_FILES)))

BIN_FILE=piano

all: $(OBJ_DIR) $(BIN_FILE)

$(BIN_FILE): $(OBJ_FILES)
	$(CC) $(CC_FLAGS) $^ -o $@ $(CC_LIBS)

$(OBJ_DIR)/%.o: %.c
	$(CC) $(CC_FLAGS) -c $< -I$(HDR_DIR) -o $@

$(OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_FILE) $(OBJ_DIR)
