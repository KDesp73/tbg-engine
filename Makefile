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
LIBS_INSTALL_DIR = /usr/lib
HEADERS_INSTALL_DIR = /usr/local/include/tbge

# Targets
SHARED = libtbge.so
STATIC = libtbge.a

# Version info
version_file = include/globals.h
VERSION_MAJOR = $(shell sed -n -e 's/\#define TBGE_VERSION_MAJOR \([0-9]*\)/\1/p' $(version_file))
VERSION_MINOR = $(shell sed -n -e 's/\#define TBGE_VERSION_MINOR \([0-9]*\)/\1/p' $(version_file))
VERSION_PATCH = $(shell sed -n -e 's/\#define TBGE_VERSION_PATCH \([0-9]*\)/\1/p' $(version_file))
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)

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
install: all ## Install the libraries and headers
	@echo "[INFO] Installing $(STATIC) to $(LIB_INSTALL_DIR)"
	cp $(STATIC) $(LIB_INSTALL_DIR)/$(STATIC)
	@echo "[INFO] Installing $(SHARED) to $(LIB_INSTALL_DIR)"
	cp $(SHARED) $(LIB_INSTALL_DIR)/$(SHARED)
	@echo "[INFO] Installing headers to $(HEADERS_INSTALL_DIR)"
	mkdir -p $(HEADERS_INSTALL_DIR)
	cp -r include/* $(HEADERS_INSTALL_DIR)

.PHONY: uninstall
uninstall: ## Remove libraries and headers
	@echo "[INFO] Uninstalling $(STATIC)"
	rm -f $(LIBS_INSTALL_DIR)/$(STATIC)
	@echo "[INFO] Uninstalling $(SHARED)"
	rm -f $(LIBS_INSTALL_DIR)/$(SHARED)
	@echo "[INFO] Uninstalling headers"
	rm -rf $(HEADERS_INSTALL_DIR)

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
	cc examples/mission/mission.c -o $(EXAMPLES_BIN)/mission -l:libtbge.a -L. -Iinclude -ggdb
	cc examples/state/state.c -o $(EXAMPLES_BIN)/state -l:libtbge.a -L. -Iinclude -ggdb
	cc examples/save/save.c -o $(EXAMPLES_BIN)/save -l:libtbge.a -L. -Iinclude -ggdb

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
