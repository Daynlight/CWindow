vcpkg_from_github(
  OUT_SOURCE_PATH SOURCE_PATH
  REPO Daynlight/Crypt

  REF 1b1b9a54b6faf60135f00d25bc1739300143aa71
  SHA512 "0"
  HEAD_REF main
)

vcpkg_cmake_configure(
  SOURCE_PATH "${SOURCE_PATH}"
  OPTIONS
    -DCWINDOW_BUILD_EXAMPLE=OFF
    -DCWINDOW_BUILD_TESTS=OFF
)

vcpkg_cmake_install()
