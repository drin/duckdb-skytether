build_dpath="${PWD}/build-dir-release"

echo "running install:"
sudo cmake --install "${build_dpath}"
echo "install complete"
