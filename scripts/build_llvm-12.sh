#!/bin/bash

SCRIPT_DIR=$(cd $(dirname $0); pwd)
SOURCE_DIR=${SCRIPT_DIR}/../llvm-project/llvm
BUILD_DIR=${SCRIPT_DIR}/../build_llvm-12/
INSTALL_DIR=${SCRIPT_DIR}/../llvm-12/

echo "SCRIPT DIR:" ${SCRIPT_DIR}
echo "SOURCE DIR:" ${SOURCE_DIR}
echo "BUILD_DIR:" ${BUILD_DIR}
echo "INSTALL_DIR:" ${INSTALL_DIR}

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

cmake -G Ninja -DCMAKE_BUILD_TYPE=Release \
               -DCMAKE_INSTALL_PREFIX=${INSTALL_DIR} \
               ${SOURCE_DIR}

ninja -j18
ninja install
