cmake_minimum_required( VERSION 3.12 )

project( sympycpp CXX )
set(CMAKE_BUILD_TYPE Release)
enable_testing()

include( GNUInstallDirs )

include_directories( include )

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/sympycpp")

add_subdirectory( include )
add_subdirectory( src )
add_subdirectory( test )
