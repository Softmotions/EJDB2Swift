#!/bin/bash
set -e
set -x

SCRIPTPATH="$(
  cd "$(dirname "$0")"
  pwd -P
)"
cd $SCRIPTPATH

OWNSRC=""
if [[ -z "${SOURCE_ROOT}" ]]; then
  OWNSRC="yes"
  SOURCE_ROOT=`mktemp -d`
  echo "Source root: ${SOURCE_ROOT}"
  echo "Checkout https://github.com/Softmotions/ejdb"
  git clone "https://github.com/Softmotions/ejdb" "${SOURCE_ROOT}"
else
  echo "Source root: ${SOURCE_ROOT}"
fi

LIBDIR="${SCRIPTPATH}/lib"
INCDIR="${SCRIPTPATH}/include"
PLATFORMS="SIMULATOR64 OS64"
BUILD_ROOT="${SOURCE_ROOT}/build-xcode"
INSTALL_ROOT="${SOURCE_ROOT}/install-xcode"

# (
#   PLATFORM="OSX"
#   rm -rf "${BUILD_ROOT}"
#   mkdir -p "${BUILD_ROOT}"
#   INSTALL_PREFIX="${INSTALL_ROOT}/${PLATFORM}"
#   rm -rf "${INSTALL_PREFIX}"
#   cd "${BUILD_ROOT}"
#   cmake .. \
#         -G "Unix Makefiles" \
#         -DCMAKE_BUILD_TYPE=Release \
#         -DBUILD_SHARED_LIBS=OFF \
#         -DENABLE_HTTP=ON \
#         -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}"
#   cmake --build . --target install;
# )

for PLATFORM in ${PLATFORMS}; do
  rm -rf "${BUILD_ROOT}"
  mkdir -p "${BUILD_ROOT}"
  INSTALL_PREFIX="${INSTALL_ROOT}/${PLATFORM}"
  rm -rf "${INSTALL_PREFIX}"
  cd "${BUILD_ROOT}"
  cmake .. \
        -G "Unix Makefiles" \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DENABLE_HTTP=OFF \
        -DCMAKE_TOOLCHAIN_FILE="${SOURCE_ROOT}/ios-tc.cmake" \
        -DPLATFORM="${PLATFORM}" \
        -DCMAKE_INSTALL_PREFIX="${INSTALL_PREFIX}"
  cmake --build . --target install;
done

rm -rf "${LIBDIR}"
rm -rf "${INCDIR}"

mkdir -p "${INCDIR}"
mkdir -p "${LIBDIR}/IOS"
mkdir -p "${LIBDIR}/OSX"

lipo -create "${INSTALL_ROOT}/SIMULATOR64/lib/libejdb2-2.a" \
             "${INSTALL_ROOT}/OS64/lib/libejdb2-2.a" \
              -o "${LIBDIR}/IOS/libejdb2-2.a"

lipo -create "${INSTALL_ROOT}/SIMULATOR64/lib/libiowow-1.a" \
             "${INSTALL_ROOT}/OS64/lib/libiowow-1.a" \
              -o "${LIBDIR}/IOS/libiowow-1.a"

cp "${INSTALL_ROOT}/OSX/lib/libejdb2-2.a" "${LIBDIR}/OSX/libejdb2-2.a"
cp "${INSTALL_ROOT}/OSX/lib/libiowow-1.a" "${LIBDIR}/OSX/libiowow-1.a"
cp "${INSTALL_ROOT}/OSX/lib/libfacilio-1.a" "${LIBDIR}/OSX/libfacilio-1.a"

cp -R "${INSTALL_ROOT}/OS64/include" "${SCRIPTPATH}"

rm -rf "${BUILD_ROOT}"
rm -rf "${INSTALL_ROOT}"
test -z "${OWNSRC}" || rm -rf "${SOURCE_ROOT}"
