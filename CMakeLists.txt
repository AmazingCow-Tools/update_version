cmake_minimum_required(VERSION 3.7)
project(update_version)

set(CMAKE_CXX_STANDARD 11)

include_directories(
    ./libs/CoreFile
    ./libs/CoreFS
    ./libs/CoreString
    ./libs/CoreAssert
)

add_executable(update_version
    ./src/update_version.cpp


    ./libs/CoreFile/CoreFile/CoreFile.h
    ./libs/CoreFile/CoreFile/include/Config.h
    ./libs/CoreFile/CoreFile/include/CoreFile.h
    ./libs/CoreFile/CoreFile/include/CoreFile_Utils.h

    ./libs/CoreFS/CoreFS/CoreFS.h
    ./libs/CoreFS/CoreFS/include/CoreFS.h
    ./libs/CoreFS/CoreFS/include/CoreFS_Utils.h
    ./libs/CoreFS/CoreFS/src/Macros.h


    ./libs/CoreString/CoreString/CoreString.h
    ./libs/CoreString/CoreString/include/CoreString.h
    ./libs/CoreString/CoreString/include/CoreString_Utils.h

    ./libs/CoreFile/CoreFile/src/CoreFile.cpp
    ./libs/CoreFS/CoreFS/src/CoreFS.cpp
    ./libs/CoreFS/CoreFS/src/CoreFS_Win32.cpp

    ./libs/CoreString/CoreString/src/CoreString.cpp
    ./libs/CoreFS/CoreFS/src/CoreFS_Unix.cpp

    ./libs/CoreAssert/CoreAssert/src/private/PrivateHelpers.cpp
)

INSTALL(TARGETS update_version RUNTIME DESTINATION /usr/local/bin)
