.PHONY: all clean format debug release duckdb_debug duckdb_release pull update

all: release

MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
PROJ_DIR := $(dir $(MKFILE_PATH))

SUBSTRAIT_DIR := "$(PROJ_DIR)subprojects/duckdb-substrait"
ARROW_DIR := "$(PROJ_DIR)subprojects/duckdb-arrow"

EXTRA_CMAKE_VARIABLES :=

# CMake flags for DuckDB to build various extensions
EXTRA_CMAKE_VARIABLES += -DCMAKE_INSTALL_PREFIX="/usr/local/duckdb-skytether"
EXTRA_CMAKE_VARIABLES += -DEXTENSION_STATIC_BUILD=1 -DBUILD_EXTENSIONS="tpch;json" ${OSX_ARCH_FLAG}
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_NAMES="substrait;arrow"

# CMake flags for DuckDB to find info specific to the substrait extension
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_SUBSTRAIT_SHOULD_LINK=1
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_SUBSTRAIT_LOAD_TESTS=1
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_SUBSTRAIT_PATH=$(SUBSTRAIT_DIR)
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_SUBSTRAIT_TEST_PATH="$(SUBSTRAIT_DIR)/test"
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_SUBSTRAIT_INCLUDE_PATH="$(SUBSTRAIT_DIR)/src/include"

# CMake flags for DuckDB to find info specific to the arrow extension
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_ARROW_SHOULD_LINK=1
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_ARROW_LOAD_TESTS=1
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_ARROW_PATH=$(ARROW_DIR)
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_ARROW_TEST_PATH="$(ARROW_DIR)/test"
EXTRA_CMAKE_VARIABLES += -DDUCKDB_EXTENSION_ARROW_INCLUDE_PATH="$(ARROW_DIR)/src/include"

export


# >> Define `DUCKDB_DIRECTORY` for use later
DUCKDB_DIRECTORY=
ifndef DUCKDB_DIR
	DUCKDB_DIRECTORY=./subprojects/duckdb
else
	DUCKDB_DIRECTORY=${DUCKDB_DIR}
endif


# >> Targets to get submodule files and clean build artifacts
pull:
	git submodule init
	git submodule update --recursive --remote

clean:
	rm -rf ${DUCKDB_DIRECTORY}/build
	rm -rf testext
	cd ${DUCKDB_DIRECTORY}              && make clean


# ------------------------------
# Main builds
# These targets (at least debug and release)
# call the makefile from the duckdb directory (hardcoded assumption of duckdb makefile)

debug:
	cd ${DUCKDB_DIRECTORY} && $(MAKE) -C . debug

release:
	cd ${DUCKDB_DIRECTORY} && $(MAKE) -C . release


# ------------------------------
# Client builds
%_js: export BUILD_NODE=1
debug_js: debug
release_js: release

%_r: export BUILD_R=1
debug_r: debug
release_r: release

%_python: export BUILD_PYTHON=1
%_python: export BUILD_FTS=1
%_python: export BUILD_VISUALIZER=1
%_python: export BUILD_TPCDS=1
debug_python: debug
release_python: release


# ------------------------------
# Main tests

test: test_release

test_release: release
	${DUCKDB_DIRECTORY}/build/release/test/unittest "$(PROJ_DIR)test/*"

test_debug: debug
	${DUCKDB_DIRECTORY}/build/debug/test/unittest "$(PROJ_DIR)test/*"


# ------------------------------
# Client tests

test_python: test_debug_python
test_debug_python: debug_python
	cd test/python && python3 -m pytest

test_release_python: release_python
	cd test/python && python3 -m pytest

test_release_r: release_r
	cd test/r && R -f test_substrait.R

test_debug_r: debug_r
	cd test/r && DUCKDB_R_DEBUG=1 R -f test_substrait.R

# >> Common between extensions
format:
	cp ${DUCKDB_DIRECTORY}/.clang-format .
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	cmake-format -i CMakeLists.txt
	rm .clang-format

update:
	git submodule update --remote --merge
