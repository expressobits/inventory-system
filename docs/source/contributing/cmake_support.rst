##############
CMake support
##############

Git submodule setup in main repository

:command:`git submodule init`

:command:`git submodule update`

Cmake configure in main repository

:command:`cmake -GNinja -DCMAKE_BUILD_TYPE=Debug -B cmake-build-debug .`

Define variables GODOTCPP_PRECISION=single|double and/or BITS=32|64 for further build variants if needed.

Cmake build/install in specified build folder cmake-build-debug

:command:`cmake --build . --target install --config Debug`

This installs the built library in the bin folder of the repository.