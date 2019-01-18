cmake_minimum_required( VERSION 3.12 )

set( headers
  factories.h
  functions.h
  lexer.h
  node.h
  sympycpp_parser.h
  token.h )

install( FILES ${headers}
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/sympycpp)
