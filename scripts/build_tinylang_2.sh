#!/bin/bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)
LLVM_CMAKE_DIR=${SCRIPT_DIR}/../llvm-12/lib/cmake/llvm
SOURCE_DIR=${SCRIPT_DIR}/../tinylang_2/
BUILD_DIR=${SCRIPT_DIR}/../build_tinylang_2/
INSTALL_DIR=${SCRIPT_DIR}/../tinylang_2/

echo "SCRIPT DIR:" ${SCRIPT_DIR}
echo "LLVM_CMAKE_DIR:" ${LLVM_CMAKE_DIR}
echo "SOURCE_DIR:" ${SOURCE_DIR}
echo "BUILD_DIR:" ${BUILD_DIR}
echo "INSTALL_DIR:" ${INSTALL_DIR}

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
               -DCMAKE_CXX_STANDARD=17 \
               -DLLVM_DIR=${LLVM_CMAKE_DIR} \
               -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
               ${SOURCE_DIR}/calc

ninja 
ninja install
