CC := gcc
INCLUDE_DIR := include
CFLAGS := -std=c99 -Wall -Wextra -Werror -pedantic -I$(INCLUDE_DIR)

BUILD_DIR := ./build

SOURCE_DIR := src
SOURCES := $(wildcard $(SOURCE_DIR)/*.c)

INCLUDES := $(wildcard $(INCLUDE_DIR)/*.h)

OBJECT_DIR := $(BUILD_DIR)/obj
OBJECTS := $(patsubst $(SOURCE_DIR)/%.c,$(OBJECT_DIR)/%.o,$(SOURCES))


LIBS := -lm -lncurses

TARGET := $(BUILD_DIR)/pacman

all: $(BUILD_DIR) $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Building $@"
	@$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

$(OBJECTS): $(SOURCES) $(INCLUDES)
	@echo "Compiling $@ to $< using $(CFLAGS)"
	@$(CC) -c -o $@ $< $(CFLAGS)

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OBJECT_DIR)

clean:
	@echo "Deleting $(BUILD_DIR)"
	@rm -r $(BUILD_DIR)

.PHONY: all clean
