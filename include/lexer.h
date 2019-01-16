#ifndef LEXER_H
#define LEXER_H

#include <set>
#include <string>
#include <vector>

#include "token.h"

namespace sympycpp
{

class Lexer
{
private:

  const std::set< char > _ignored = { ' ' };
  const std::set< char > _separators = { ',', '(', ')' };
  std::set< std::string > _operators;

  Token _tokenize_value( const std::string value )
  {
    if ( _operators.find( value ) != _operators.end() )
    {
      return Token( TOKEN_TYPE_OPERATOR, value );
    }
    else
    {
      return Token( TOKEN_TYPE_LITERAL, value );
    }
  }

public:

  Lexer()
    : _operators()
  {
  }

  void register_operator( const std::string name )
  {
    _operators.insert( name );
  }

  void tokenize_expression( const std::string expr, std::vector< Token >& tokens )
  {
    std::string current;

    auto it_c = expr.begin();
    while ( it_c != expr.end() )
    {
      if ( _ignored.find( *it_c ) != _ignored.end() )
      {
        // do nothing for ignored characters
      }
      else if ( _separators.find( *it_c ) != _separators.end() )
      {

        if ( current != "" )
        {
          tokens.push_back( _tokenize_value( current ) );
          current = "";
        }

        current += *it_c;
        tokens.push_back( Token( TOKEN_TYPE_SEPARATOR, current ) );
        current = "";
      }
      else
      {
        current += *it_c;
      }
      ++it_c;
    }

    // if expr does not end on separator, need to add last token
    if ( current != "" )
    {
      tokens.push_back( _tokenize_value( current ) );
      current = "";
    }
  }

};

}

#endif
