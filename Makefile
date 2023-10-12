
#thx /TheNetAdmin/ for the template
# tool macros
CC ?= c++				# FILL: the compiler
CXX ?= c++				# FILL: the compiler
CFLAGS := -Wall			# FILL: compile flags
CXXFLAGS := -Wall 		# FILL: compile flags

DEPENDENCIES := -lncurses

DBGFLAGS := -g
COBJFLAGS := $(CFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug

# compile macros
TARGET_FILE := main.cpp # FILL: target name
OUT_NAME := temqo


# default rule
default: makedir build exec


# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)

.PHONY: exec
exec:
	./$(BIN_PATH)/$(OUT_NAME)
	
.PHONY: build
build: 
	@ $(CXX) $(SRC_PATH)/$(TARGET_FILE) $(DEPENDENCIES) $(CXXFLAGS) -o $(BIN_PATH)/$(OUT_NAME)