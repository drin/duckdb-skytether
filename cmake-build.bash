source_dpath="${PWD}/subprojects/duckdb"
build_dpath="${PWD}/build-dir-release"

substrait_dpath="${PWD}/subprojects/duckdb-substrait"
arrow_dpath="${PWD}/subprojects/duckdb-arrow"

cmake -S "${source_dpath}"                                                       \
      -B "${build_dpath}"                                                        \
      -DCMAKE_BUILD_TYPE=Release                                                 \
      -DCMAKE_INSTALL_PREFIX="/usr/local/duckdb-skytether"                       \
      -DEXTENSION_STATIC_BUILD=1                                                 \
      -DBUILD_EXTENSIONS="tpch;json"                                             \
      -DDUCKDB_EXTENSION_NAMES="substrait;arrow"                                 \
      -DDUCKDB_EXTENSION_SUBSTRAIT_SHOULD_LINK=1                                 \
      -DDUCKDB_EXTENSION_SUBSTRAIT_LOAD_TESTS=1                                  \
      -DDUCKDB_EXTENSION_SUBSTRAIT_PATH="${substrait_dpath}"                     \
      -DDUCKDB_EXTENSION_SUBSTRAIT_TEST_PATH="${substrait_dpath}/test"           \
      -DDUCKDB_EXTENSION_SUBSTRAIT_INCLUDE_PATH="${substrait_dpath}/src/include" \
      -DDUCKDB_EXTENSION_ARROW_SHOULD_LINK=1                                     \
      -DDUCKDB_EXTENSION_ARROW_LOAD_TESTS=1                                      \
      -DDUCKDB_EXTENSION_ARROW_PATH="${arrow_dpath}"                             \
      -DDUCKDB_EXTENSION_ARROW_TEST_PATH="${arrow_dpath}/test"                   \
      -DDUCKDB_EXTENSION_ARROW_INCLUDE_PATH="${arrow_dpath}/src/include"         \
      -GNinja

