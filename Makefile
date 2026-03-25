CC = gcc
CFLAGS = -g -std=c11 -Wno-incompatible-pointer-types
LDFLAGS = -lm

BIN=test

BUILD_DIR = ./bin
SOURCE_DIR = ./src
SOURCE_DIRS = $(shell find $(SOURCE_DIR) -type d)

SOURCES := $(shell find $(SOURCE_DIR) -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)

vpath %.c $(SOURCE_DIRS)

all: $(BIN)

$(BIN): $(notdir $(OBJECTS))
	$(CC) $(CFLAGS) $(LDFLAGS) $(addprefix, -I, $(SOURCE_DIRS)) $(addprefix $(BUILD_DIR)/, $^) -o $(BIN)

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(addprefix, -I, $(SOURCE_DIRS)) -c $< -o $(BUILD_DIR)/$@ 

clean:
	@rm -f $(BUILD_DIR)/*.o $(BIN)

show:
	@echo $(notdir $(OBJECTS))