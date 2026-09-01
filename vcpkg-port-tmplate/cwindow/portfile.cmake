vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO Daynlight/CWindow

  REF 
  SHA512 "0"
  HEAD_REF engine
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DCWINDOW_BUILD_EXAMPLE=OFF
    -DCWINDOW_BUILD_TESTS=OFF
)

vcpkg_cmake_install()
