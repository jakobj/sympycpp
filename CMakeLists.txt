cmake_minimum_required( VERSION 3.12 )

project( sympycpp CXX )
set(CMAKE_BUILD_TYPE Release)

include( GNUInstallDirs )

include_directories( include )

add_subdirectory( include )
add_subdirectory( src )
