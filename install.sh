#!/bin/sh
# Change the following env variable to change the name of the executable generated with cmake and build type (restart all the building process)
export EXE_TITLE="NN-Maker"
# Debug | Release
BUILD_TYPE=Release

printb() {
    tput setaf $2
    tput bold
    echo $1
    tput sgr0
}

printb "#==== Installation ====#" 2
cd build
cmake . -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
cmake --build . 
cmake --install .
cd ..
mkdir bin 2>/dev/null
mv ./build/${EXE_TITLE} ./bin 2>/dev/null	
printb "#==== Finished ====#" 2
