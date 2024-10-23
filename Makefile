# Compiler and flags
CC = gcc
CFLAGS = -Wall -Iinclude -fPIC
LDFLAGS =

# Directories
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
DIST_DIR = dist
EXAMPLES_BIN = examples/bin

# Target and version info
SHARED = libtbge.so
STATIC = libtbge.a
VERSION = 0.1.0

# Determine the build type
ifneq ($(type), RELEASE)
	CFLAGS += -DDEBUG -ggdb
else
	CFLAGS += -O3
endif

# Source and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default target
.DEFAULT_GOAL := help

# Total source file count
TOTAL_FILES := $(words $(SRC_FILES))

# Counter to track progress
counter = 0

# Targets

.PHONY: all
all: static shared examples ## Build all libraries and examples
	@echo "Build complete."

.PHONY: shared
shared: $(BUILD_DIR) $(OBJ_FILES) ## Build the dynamic library
	$(CC) -shared -o $(SHARED) $(OBJ_FILES) $(LDFLAGS)
	chmod 755 $(SHARED)

.PHONY: static
static: $(BUILD_DIR) $(OBJ_FILES) ## Build the static library
	ar rcs $(STATIC) $(OBJ_FILES)
	chmod 644 $(STATIC)

.PHONY: check_tools
check_tools: ## Check if necessary tools are available
	@command -v gcc >/dev/null 2>&1 || { echo >&2 "[ERRO] gcc is not installed."; exit 1; }
	@command -v bear >/dev/null 2>&1 || { echo >&2 "[WARN] bear is not installed. Skipping compile_commands.json target."; }
	@command -v ar >/dev/null 2>&1 || { echo >&2 "[WARN] ar is not installed. Consider adding it to your system to build the static library"; }

$(BUILD_DIR): ## Create the build directory if it doesn't exist
	@echo "[INFO] Creating build directory"
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c ## Compile source files with progress
	$(eval counter=$(shell echo $$(($(counter)+1))))
	@echo "[$(counter)/$(TOTAL_FILES)] Compiling $< -> $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: install
install: all ## Install the libraries to /usr/lib/
	@echo "[INFO] Installing $(STATIC) to /usr/lib/"
	cp $(STATIC) /usr/lib/$(STATIC)
	@echo "[INFO] Installing $(SHARED) to /usr/lib/"
	cp $(SHARED) /usr/lib/$(SHARED)

.PHONY: uninstall
uninstall: ## Remove the libraries from /usr/lib/
	@echo "[INFO] Uninstalling $(STATIC)"
	rm -f /usr/lib/$(STATIC)
	@echo "[INFO] Uninstalling $(SHARED)"
	rm -f /usr/lib/$(SHARED)

.PHONY: clean
clean: ## Remove all build files and the executable
	@echo "[INFO] Cleaning up build directory and executable."
	rm -rf $(BUILD_DIR) $(SHARED) $(STATIC) $(EXAMPLES_BIN)

.PHONY: distclean
distclean: clean ## Perform a full clean, including backup and temporary files
	@echo "[INFO] Performing full clean, removing build directory, dist files, and editor backups."
	rm -f *~ core $(SRC_DIR)/*~ $(DIST_DIR)/*.tar.gz

.PHONY: dist
dist: $(SRC_FILES) ## Create a tarball of the project
	@echo "[INFO] Creating a tarball for version $(VERSION)"
	mkdir -p $(DIST_DIR)
	tar -czvf $(DIST_DIR)/$(TARGET)-$(VERSION).tar.gz $(SRC_DIR) $(INCLUDE_DIR) Makefile README.md

.PHONY: examples
examples: static ## Build all examples into binaries
	mkdir -p $(EXAMPLES_BIN)
	cc examples/map/map.c -o $(EXAMPLES_BIN)/map -l:libtbge.a -L. -Iinclude -ggdb
	cc examples/player/player.c -o $(EXAMPLES_BIN)/player -l:libtbge.a -L. -Iinclude -ggdb
	cc examples/commands/commands.c -o $(EXAMPLES_BIN)/commands -l:libtbge.a -L. -Iinclude -ggdb
	cc examples/progress/progress.c -o $(EXAMPLES_BIN)/progress -l:libtbge.a -L. -Iinclude -ggdb

compile_commands.json: $(SRC_FILES) ## Generate compile_commands.json
	@echo "[INFO] Generating compile_commands.json"
	bear -- make all

.PHONY: help
help: ## Show this help message
	@echo "Available commands:"
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

.PHONY: verbose
verbose: CFLAGS += -DVERBOSE
verbose: all ## Build the project in verbose mode

# Phony targets to avoid conflicts with file names
.PHONY: all clean distclean install uninstall dist compile_commands.json help check_tools verbose
