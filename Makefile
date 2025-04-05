PROJECT_NAME := pacman

SOURCE_DIR := src
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/obj

SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

CC := gcc
HFLAGS := -I$(SOURCE_DIR)
CFLAGS := -Wall -Wextra -Wfatal-errors -pedantic -std=c11 -g
LDFLAGS := -lm -lncurses

TARGET := $(BUILD_DIR)/$(PROJECT_NAME)

all: $(BUILD_DIR) $(OBJECT_DIR) $(TARGET)

# Link object files into the final executable
$(TARGET): $(OBJECT_FILES)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile .c files into .o files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(CC) $(CFLAGS) $(HFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(OBJECT_DIR):
	mkdir -p $(OBJECT_DIR)

clean:
	rm -rf $(BUILD_DIR)

run: all
	@printf "\tRunning $(PROJECT_NAME) ...\n"
	@$(TARGET)

debug: all
	./debug.sh $(TARGET)

.PHONY: all clean run debug
