# turn on RPATH for tests so we can run them in the deb build
set(CMAKE_SKIP_RPATH OFF)

add_subdirectory(unit)
