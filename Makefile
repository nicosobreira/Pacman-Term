PROJECT_NAME := pacman

PREFIX ?= $(HOME)/.local
ASSETS_DIR := assets
SOURCE_DIR := src
BUILD_DIR := build
OBJECT_DIR := $(BUILD_DIR)/obj

SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*.c)
OBJECT_FILES := $(patsubst $(SOURCE_DIR)/%.c, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

CC := gcc
HFLAGS := -I$(SOURCE_DIR)

CFLAGS := -Wall -Wextra -Wfatal-errors -pedantic -std=c99
LDFLAGS := -lm -lncurses

RELEASE_CFLAGS := -O3 -march=native -flto -fno-plt -pipe
RELEASE_LDFLAGS := -flto -fno-plt -s

DEBUG_CFLAGS := -g3 -Og
# DEBUG_CFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer

DEBUG_LDFLAGS :=
# DEBUG_LDFALGS += -fsanitize=address,undefined

TARGET := $(BUILD_DIR)/$(PROJECT_NAME)

all: CFLAGS += $(DEBUG_CFLAGS)
all: LDFLAGS += $(DEBUG_LDFLAGS)
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

release: CFLAGS += $(RELEASE_CFLAGS)
release: LDFLAGS += $(RELEASE_LDFLAGS)
release: all

install: $(PREFIX)
install: CFLAGS += -DNDEBUG -fstack-protector-strong
install: LDFLAGS += -Wl,-z,now,-z,relro
install: release
	@echo "Installing to $(PREFIX)"
	install -Dm755 $(TARGET) $(PREFIX)/bin/$(PROJECT_NAME)
	@echo "Installation complete"

$(PREFIX):
	mkdir -p $(PREFIX)

.PHONY: all clean run debug release install
