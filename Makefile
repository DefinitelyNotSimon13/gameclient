###############################################################################
# General
###############################################################################
RAYLIB ?= ./external/raylib-5.5/src/
TARGET := build/game

###############################################################################
# Compiler/Linker
###############################################################################
CC := gcc
C_STD := c23
CFLAGS := -Wall -Wextra -std=$(C_STD)
LDFLAGS := -lraylib -lGL -lm -lpthread -ldl -lrt -lwayland-client
FLAGS := -I $(RAYLIB) -L $(RAYLIB)

# Include and library dirs
INCLUDE_DIRS := -I include -I $(RAYLIB)
LIB_DIRS := -L $(RAYLIB)

# -MMD: Generate dependecy info; -MP: Add phony targets for each dependency
DEPFLAGS := -MMD -MP

CFLAGS += $(DEPFLAGS) $(INCLUDE_DIRS)
LDFLAGS += $(LIB_DIRS)

###############################################################################
# Source Files
###############################################################################
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c, build/%.o, $(SRC))
DEP := $(OBJ:.o=.d)

###############################################################################
# Primary Targets
###############################################################################
all: $(TARGET)

$(TARGET): $(OBJ)
	@echo " [LINK]	$@"
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	@echo " [CC]	$<"
	$(CC) $(CFLAGS) -c $< -o $@

###############################################################################
# Utility Targets
###############################################################################
clean:
	@echo " [CLEAN] Removing build directory"
	rm a.out compile_commands.json -f
	rm -rf build

rerun: all run

run: $(TARGET)
	@echo " [RUN]	$(TARGET)"
	./$(TARGET)

###############################################################################
# Debug/Release Config
###############################################################################

debug: CFLAGS += -g -O0
debug: clean all

release: CFLAGS += -O2
release: clean all

-include $(DEP)

.PHONY: all clean run rerun debug release


