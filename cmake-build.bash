source_dpath="${PWD}/subprojects/duckdb"
build_dpath="${PWD}/build-dir-release"
# build_dpath="${PWD}/build-dir-debug"

substrait_dpath="${PWD}/subprojects/duckdb-substrait"
arrow_dpath="${PWD}/subprojects/duckdb-nanoarrow"

# substrait_dpath="${HOME}/code/duckdb-substrait"

#       -DCMAKE_BUILD_TYPE=Debug                                                   \
#      -DBUILD_PYTHON=1                                                           \
#      -DCMAKE_BUILD_TYPE=Release                                                 \
cmake -S "${source_dpath}"                                                       \
      -B "${build_dpath}"                                                        \
      -DCMAKE_BUILD_TYPE=Release                                                 \
      -DCMAKE_INSTALL_PREFIX="/opt/homebrew"                                     \
      -DEXTENSION_STATIC_BUILD=1                                                 \
      -DBUILD_EXTENSIONS="tpch;json"                                             \
      -DDUCKDB_EXTENSION_NAMES="substrait;nanoarrow"                             \
      -DDUCKDB_EXTENSION_SUBSTRAIT_SHOULD_LINK=1                                 \
      -DDUCKDB_EXTENSION_SUBSTRAIT_LOAD_TESTS=1                                  \
      -DDUCKDB_EXTENSION_SUBSTRAIT_PATH="${substrait_dpath}"                     \
      -DDUCKDB_EXTENSION_SUBSTRAIT_TEST_PATH="${substrait_dpath}/test"           \
      -DDUCKDB_EXTENSION_SUBSTRAIT_INCLUDE_PATH="${substrait_dpath}/src/include" \
      -DDUCKDB_EXTENSION_NANOARROW_SHOULD_LINK=1                                 \
      -DDUCKDB_EXTENSION_NANOARROW_LOAD_TESTS=1                                  \
      -DDUCKDB_EXTENSION_NANOARROW_PATH="${arrow_dpath}"                         \
      -DDUCKDB_EXTENSION_NANOARROW_TEST_PATH="${arrow_dpath}/test"               \
      -DDUCKDB_EXTENSION_NANOARROW_INCLUDE_PATH="${arrow_dpath}/src/include"     \
      -DDUCKDB_EXTENSION_NANOARROW_SHOULD_LINK=1                                 \
      -DENABLE_SANITIZER=OFF                                                     \
      -GNinja

