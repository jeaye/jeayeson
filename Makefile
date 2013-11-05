# Copyright © 2013 Jesse 'Jeaye' Wilkerson
# See licensing at:
#   http://opensource.org/licenses/BSD-3-Clause

# File: Makefile
# Author: Jesse 'Jeaye' Wilkerson

# Sources
TESTS_SRC=$(shell find ./src/tests -type f -name '*.cpp')
TESTS_OBJ=$(TESTS_SRC:.cpp=.o)
OUT_DIR=bin

# C++
CXX=g++
CXX_WARN_FLAGS=-Wall -Wextra -pedantic -Wno-unknown-pragmas
CXX_DEBUG_FLAGS=-ggdb
CXX_INCLUDE_FLAGS=-Isrc/ 
CXX_FLAGS=-std=c++11 $(CXX_INCLUDE_FLAGS) $(CXX_WARN_FLAGS) $(CXX_DEBUG_FLAGS)

LD_FLAGS=

# Colors
COLOR_OFF=$(shell tput sgr0)
COLOR_GREEN=$(shell tput setaf 2)
PREFIX=$(COLOR_GREEN)»»»$(COLOR_OFF)

.SILENT:

.PHONY: all clean

all: clean .build_tests
	echo "${PREFIX} Finished \o/"
	
.build_tests: .build_setup_tests ${TESTS_SRC}
	echo "${PREFIX} Building tests"
	$(foreach file, ${TESTS_SRC}, \
		echo "  $(PREFIX) $(OUT_DIR)/$(basename $(notdir $(file)))" && \
		$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o $(OUT_DIR)/$(basename $(notdir $(file))) $(file) \
	 )
	touch .build_tests

.build_setup_tests:
	mkdir -p $(OUT_DIR)
	touch .build_setup_tests

clean:
	find . -type f -name '.build_*' | xargs rm -f
	rm -rf $(OUT_DIR)
	echo "$(PREFIX) Cleaned"

