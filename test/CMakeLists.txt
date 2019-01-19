cmake_minimum_required( VERSION 3.12 )

add_library( test_main test_main.cpp )
add_executable( test_sympycpp_parser test_sympycpp_parser.cpp )
target_link_libraries( test_sympycpp_parser test_main parser )

add_test( NAME test_sympy_parser COMMAND ${CMAKE_INSTALL_BUILDIR}/test_sympycpp_parser )
