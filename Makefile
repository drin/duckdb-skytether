
# ------------------------------
# Configuration of extension
PROJ_DIR := $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

EXT_NAME=skytether
EXT_CONFIG=${PROJ_DIR}extension_config.cmake

CORE_EXTENSIONS='tpch;tpcds;json'


# Set this flag during building to enable the benchmark runner
ifeq (${BUILD_BENCHMARK}, 1)
	TOOLCHAIN_FLAGS:=${TOOLCHAIN_FLAGS} -DBUILD_BENCHMARKS=1
endif


# ------------------------------
# Build dependencies (external makefiles)

# Include the Makefile from extension-ci-tools
include extension-ci-tools/makefiles/duckdb_extension.Makefile


# ------------------------------
# Build targets

# >> Common between extensions

all: release

pull:
	git submodule init
	git submodule update --recursive --remote

format:
	cp ${DUCKDB_DIRECTORY}/.clang-format .
	find src/ -iname *.hpp -o -iname *.cpp | xargs clang-format --sort-includes=0 -style=file -i
	cmake-format -i CMakeLists.txt
	rm .clang-format

update:
	git submodule update --remote --merge


# >> client-specific targets

# javascript
%_js: export BUILD_NODE=1
debug_js: debug
release_js: release

# R
%_r: export BUILD_R=1
debug_r: debug
release_r: release

# Python
%_python: export BUILD_PYTHON=1
%_python: export BUILD_FTS=1
%_python: export BUILD_VISUALIZER=1
%_python: export BUILD_TPCDS=1
debug_python: debug
release_python: release


# >> test targets

test_debug: debug
	build/debug/test/unittest "$(PROJ_DIR)test/*"

# R
test_release_r: release_r
	cd test/r && R -f test_substrait.R

test_debug_r: debug_r
	cd test/r && DUCKDB_R_DEBUG=1 R -f test_substrait.R

# python
test_python: test_debug_python
test_debug_python: debug_python
	cd test/python && python3 -m pytest

test_release_python: release_python
	cd test/python && python3 -m pytest

