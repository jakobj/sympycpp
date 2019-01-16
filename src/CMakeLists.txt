cmake_minimum_required( VERSION 3.12 )

add_library( parser SHARED parser.cpp )

install( TARGETS parser
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/sympycpp
  )
