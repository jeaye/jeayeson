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
CXX=c++
CXX_WARN_FLAGS=-Wall -Wextra -pedantic
CXX_DEBUG_FLAGS=
CXX_OPTIM_FLAGS=-O3
CXX_INCLUDE_FLAGS=-Isrc/ 
CXX_FLAGS=-std=c++1y \
					$(CXX_INCLUDE_FLAGS) \
					$(CXX_WARN_FLAGS) \
					$(CXX_DEBUG_FLAGS) \
					$(CXX_OPTIM_FLAGS)

LD_FLAGS=

# Colors
COLOR_OFF=$(shell tput sgr0)
COLOR_GREEN=$(shell tput setaf 2)

.SILENT:

.PHONY: all test clean

all: clean .build_tests
	echo "Done"
	
.build_tests: .build_setup_tests ${TESTS_SRC}
	echo "Building tests"
	for file in ${TESTS_SRC}; \
	do \
		out=$$(echo $(OUT_DIR)/$$(basename $$file) | sed 's/\..*$$//'); \
		echo "  $$out"; \
		$(CXX) $(CXX_FLAGS) $(LD_FLAGS) -o $$out $$file; \
	done
	touch .build_tests

.build_setup_tests:
	mkdir -p $(OUT_DIR)
	touch .build_setup_tests

test:
	echo "Running tests"
	for file in ${TESTS_SRC}; \
	do \
		out=$$(echo $(OUT_DIR)/$$(basename $$file) | sed 's/\..*$$//'); \
		echo "  $$out"; \
		$$out > /dev/null || exit 1; \
	done
	echo "Done"

clean:
	find . -type f -name '.build_*' | xargs rm -f
	rm -rf $(OUT_DIR)
	echo "Cleaned"

