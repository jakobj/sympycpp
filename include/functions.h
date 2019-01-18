#ifndef SYMPYCPP_FUNCTIONS_H
#define SYMPYCPP_FUNCTIONS_H

namespace sympycpp
{

inline
double identity( const double x, const double )
{
  return x;
}

inline
double add( const double x, const double y )
{
  return x + y;
}

inline
double sub( const double x, const double y )
{
  return x - y;
}

inline
double mul( const double x, const double y )
{
  return x * y;
}

}

#endif
