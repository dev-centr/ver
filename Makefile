# Makefile for Ver - Universal Version Control Interface

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = 

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source files
CORE_SOURCES = $(SRC_DIR)/core/main.c \
               $(SRC_DIR)/core/dispatcher.c \
               $(SRC_DIR)/core/config.c \
               $(SRC_DIR)/core/detection.c

HANDLER_SOURCES = $(SRC_DIR)/handlers/gitu-handler.c \
                  $(SRC_DIR)/handlers/git-handler.c \
                  $(SRC_DIR)/handlers/local-handler.c

TUI_SOURCES = $(SRC_DIR)/tui/interface.c \
              $(SRC_DIR)/tui/selection.c

UTIL_SOURCES = $(SRC_DIR)/util/string.c \
               $(SRC_DIR)/util/filesystem.c

ALL_SOURCES = $(CORE_SOURCES) $(HANDLER_SOURCES) $(TUI_SOURCES) $(UTIL_SOURCES)

# Object files
OBJECTS = $(ALL_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

# Target executable
TARGET = $(BIN_DIR)/ver

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)/core $(BUILD_DIR)/handlers $(BUILD_DIR)/tui $(BUILD_DIR)/util $(BIN_DIR)

# Build the main executable
$(TARGET): $(OBJECTS)
	@echo "Linking ver..."
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Dependencies
$(BUILD_DIR)/core/main.o: $(SRC_DIR)/core/main.c $(SRC_DIR)/core/dispatcher.h
$(BUILD_DIR)/core/dispatcher.o: $(SRC_DIR)/core/dispatcher.c $(SRC_DIR)/core/dispatcher.h $(SRC_DIR)/core/config.h
$(BUILD_DIR)/core/config.o: $(SRC_DIR)/core/config.c $(SRC_DIR)/core/config.h
$(BUILD_DIR)/core/detection.o: $(SRC_DIR)/core/detection.c $(SRC_DIR)/core/detection.h

$(BUILD_DIR)/handlers/gitu-handler.o: $(SRC_DIR)/handlers/gitu-handler.c $(SRC_DIR)/handlers/handler.h
$(BUILD_DIR)/handlers/git-handler.o: $(SRC_DIR)/handlers/git-handler.c $(SRC_DIR)/handlers/handler.h
$(BUILD_DIR)/handlers/local-handler.o: $(SRC_DIR)/handlers/local-handler.c $(SRC_DIR)/handlers/handler.h

$(BUILD_DIR)/tui/interface.o: $(SRC_DIR)/tui/interface.c $(SRC_DIR)/tui/interface.h
$(BUILD_DIR)/tui/selection.o: $(SRC_DIR)/tui/selection.c $(SRC_DIR)/tui/selection.h

$(BUILD_DIR)/util/string.o: $(SRC_DIR)/util/string.c $(SRC_DIR)/util/string.h
$(BUILD_DIR)/util/filesystem.o: $(SRC_DIR)/util/filesystem.c $(SRC_DIR)/util/filesystem.h

# Install target
install: $(TARGET)
	@echo "Installing ver..."
	install -d $(DESTDIR)/usr/local/bin
	install $(TARGET) $(DESTDIR)/usr/local/bin/ver

# Clean target
clean:
	@echo "Cleaning build files..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Test target
test: $(TARGET)
	@echo "Running tests..."
	@$(BUILD_DIR)/test_runner

# Development target with debug info
debug: CFLAGS += -g -DDEBUG
debug: clean all

# Release target
release: CFLAGS += -DNDEBUG -s
release: clean all

# Help target
help:
	@echo "Available targets:"
	@echo "  all       - Build ver (default)"
	@echo "  clean     - Remove build files"
	@echo "  install   - Install ver system-wide"
	@echo "  test      - Run tests"
	@echo "  debug     - Build with debug symbols"
	@echo "  release   - Build optimized release"
	@echo "  help      - Show this help"

.PHONY: all directories clean install test debug release help
