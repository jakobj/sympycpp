#include "catch.hpp"

#include "sympycpp_parser.h"

using namespace sympycpp;

TEST_CASE( "Add two inputs" ) {
  Parser parser = Parser();
  parser.parse( "Add(Symbol('x_0'), Symbol('x_1'))", 2 );
  const double x_0 = 3.;
  const double x_1 = 2.;
  REQUIRE( parser.eval( { x_0, x_1 } ) == Approx( x_0 + x_1 ) );
}

TEST_CASE( "Multiply two inputs" ) {
  Parser parser = Parser();
  parser.parse( "Mul(Symbol('x_0'), Symbol('x_1'))", 2 );
  const double x_0 = 3.;
  const double x_1 = 2.;
  REQUIRE( parser.eval( { x_0, x_1 } ) == Approx( x_0 * x_1 ) );
}
