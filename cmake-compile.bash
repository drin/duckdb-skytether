# build_dpath="${PWD}/build-dir-debug"
# cmake --build "${build_dpath}" --config Debug

build_dpath="${PWD}/build-dir-release"
cmake --build "${build_dpath}" --config Release
