#!/bin/bash

if [ -z "${CGSN_MOORING_CMAKE_FLAGS}" ]; then
    CGSN_MOORING_CMAKE_FLAGS=
fi

if [ -z "${CGSN_MOORING_MAKE_FLAGS}" ]; then
    CGSN_MOORING_MAKE_FLAGS=
fi

set -e -u
mkdir -p build

echo "Configuring..."
echo "cmake .. ${CGSN_MOORING_CMAKE_FLAGS}"
pushd build >& /dev/null
cmake .. ${CGSN_MOORING_CMAKE_FLAGS}
echo "Building..."
echo "make ${CGSN_MOORING_MAKE_FLAGS} $@"
make ${CGSN_MOORING_MAKE_FLAGS} $@
popd >& /dev/null
