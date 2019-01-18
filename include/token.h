#ifndef SYMPYCPP_TOKEN_H
#define SYMPYCPP_TOKEN_H

#include <string>

namespace sympycpp
{

enum token_type
{
  TOKEN_TYPE_SEPARATOR,
  TOKEN_TYPE_OPERATOR,
  TOKEN_TYPE_LITERAL,
};

class Token
{
public:
  token_type type;
  std::string value;

  Token( const token_type type, const std::string value )
    : type( type )
    , value( value )
  {
  }
};

}

#endif
