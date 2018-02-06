cmake_minimum_required(VERSION 3.5)
project(update_version)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ./libs/acow_cpp_goodies
    ./libs/CoreFile
    ./libs/CoreFS
    ./libs/CoreString
    ./libs/CoreAssert
)

add_subdirectory(libs/CoreAssert)
add_subdirectory(libs/CoreFile)
add_subdirectory(libs/CoreFS)
add_subdirectory(libs/CoreString)

add_executable(update_version
    ./src/update_version.cpp
)

target_link_libraries (update_version LINK_PUBLIC CoreFile)
target_link_libraries (update_version LINK_PUBLIC CoreFS)
target_link_libraries (update_version LINK_PUBLIC CoreString)
target_link_libraries (update_version LINK_PUBLIC CoreAssert)


INSTALL(TARGETS update_version RUNTIME DESTINATION /usr/local/bin)
