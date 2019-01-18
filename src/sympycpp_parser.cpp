#include <iostream>

#include "sympycpp_parser.h"

#include "factories.h"
#include "functions.h"
#include "lexer.h"

namespace sympycpp
{

Parser::Parser()
  : _literal_regexs()
  , _operator_names()
  ,  _input_nodes()
  , root( nullptr )
{

  // register operators and corresponding factory functions
  _operator_names.insert( std::make_pair( "Symbol", &create_operator_node_binaryop< identity > ) );
  _operator_names.insert( std::make_pair( "Integer", &create_operator_node_binaryop< identity > ) );
  _operator_names.insert( std::make_pair( "Float", &create_operator_node_float ) );
  _operator_names.insert( std::make_pair( "Add", &create_operator_node_binaryop< add > ) );
  _operator_names.insert( std::make_pair( "Mul", &create_operator_node_binaryop< mul > ) );
  _operator_names.insert( std::make_pair( "Pow", &create_operator_node_binaryop< pow > ) );

  // register rexpressions to parse literals and corresponding
  // factory functions
  _literal_regexs.insert( std::make_pair( str_regex_symbol, &create_literal_node_symbol ) );
  _literal_regexs.insert( std::make_pair( "-?[0-9]+", &create_integer_node ) );
  _literal_regexs.insert( std::make_pair( "'-?[0-9]+\\.[0-9]+'", &create_float_node ) );
  _literal_regexs.insert( std::make_pair( "precision=[0-9]+", &create_precision_node ) );
  _literal_regexs.insert( std::make_pair( "-?[0-9]+\\.[0-9]+", &create_free_float_node ) );
}



bool Parser::_is_symbol( const Token& token ) const
{
  return std::regex_match( token.value, std::regex( str_regex_symbol ) );
}


void Parser::_expect_value( const std::vector< Token >::const_iterator& it_token, const std::set< std::string > what ) const
{
  if ( what.find( it_token->value ) == what.end() )
  {
    std::string msg;
    msg += "unexpected token value '" + it_token->value + "' but expected one of '";
    for ( auto it = what.begin(); it != what.end(); ++it )
    {
      msg += *it + "', '";
    }
    msg.erase( msg.end() - 3, msg.end() );
    msg += " after token '" + ( it_token - 1 )->value + "'";
    if ( it_token->value == "\0" )
    {
      msg += ". missing parenthesis?";
    }
    throw std::runtime_error( msg );
  }
}


void Parser::_skip_value( std::vector< Token >::const_iterator& it_token, const std::set< std::string > what ) const
{
  _expect_value( it_token, what );
  ++it_token;
}


void Parser::_expect_type( const std::vector< Token >::const_iterator& it_token, const std::set< token_type > what ) const
{
  if ( what.find( it_token->type ) == what.end() )
  {
    std::string msg = "unexpected token '" + it_token->value + "' after token '" + ( it_token - 1 )->value + "'";
    throw std::runtime_error( msg );
  }
}


void Parser::_skip_type( std::vector< Token >::const_iterator& it_token, const std::set< token_type > what ) const
{
  _expect_type( it_token, what );
  ++it_token;
}


Token read_token( std::vector< Token >::const_iterator& it_token )
{
  Token token = *it_token;
  ++it_token;
  return token;
}


std::shared_ptr< Node > Parser::_get_input_node( const std::string name )
{
  const std::string input_name = name.substr( 1, name.size() - 2 );
  // full expression can contain the same symbol multiple times,
  // make sure to only maintain a single input node
  if ( _input_nodes.find( input_name ) == _input_nodes.end() )
  {
    const std::string msg = "undefined symbol '" + input_name + "'. check number of inputs";
    throw std::runtime_error( msg );
  }
  else
  {
    return _input_nodes[ input_name ];
  }
}


void Parser::_read_next_arg( std::vector< Token >::const_iterator& it_token, const std::vector< Token >::const_iterator& end, std::vector< Token >& tokens ) const
{
  int level = 0;
  while ( true )
  {
    if ( ( it_token->value == "," and level == 0 )
         or it_token == end )
    {
      break;
    }

    if ( it_token->value == "(" )
    {
      ++level;
    }
    else if ( it_token->value == ")" )
    {
      --level;
    }

    tokens.push_back( *it_token );
    ++it_token;

  }
}

std::shared_ptr< Node > Parser::_parse_recursively( const std::vector< Token >& tokens )
{
  std::shared_ptr< Node > node;
  
  auto it_token = tokens.begin();

  if ( it_token->type == TOKEN_TYPE_OPERATOR )
  {
    const Token func = read_token( it_token );
    _skip_value( it_token, { "(" } );

    std::vector< std::vector< Token > > args;
    while ( it_token != tokens.end() )
    {
      args.resize( args.size() + 1 );

      // read until "," or at most until second to last character
      _read_next_arg( it_token, tokens.end() - 1, args[ args.size() - 1] );

      if ( it_token == tokens.end() - 1 )
      {
        _skip_value( it_token, { ")" } );
      }
      else
      {
        _skip_value( it_token, { "," } );
      }
    }

    std::vector< std::shared_ptr< Node > > children;
    for ( auto it_arg = args.begin(); it_arg != args.end(); ++it_arg )
    {
      children.push_back( _parse_recursively( *it_arg ) );
    }

    node = _create_new_node( func, children );
  }
  else
  {
    assert( it_token->type == TOKEN_TYPE_LITERAL );

    // literals do not have children, no need to parse further
    std::vector< std::shared_ptr< Node > > children;
    node = _create_new_node( *it_token, children );
  }

  return node;
}

std::shared_ptr< Node > Parser::_create_new_node( const Token token, const std::vector< std::shared_ptr< Node > >& children )
{
  if ( token.type == TOKEN_TYPE_OPERATOR )
  {
    for ( auto it_operator_name = _operator_names.begin(); it_operator_name != _operator_names.end(); ++it_operator_name )
    {
      if ( token.value == it_operator_name->first )
      {
        return it_operator_name->second( children );
      }
    }

    // reaching this means operator could not be parsed
    const std::string msg = "unknown operator '" + token.value + "'";
    throw std::runtime_error( msg );
  }
  else
  {
    assert( token.type == TOKEN_TYPE_LITERAL );

    // try to parse literal, emit error if not successful
    for ( auto it_literal_regex = _literal_regexs.begin(); it_literal_regex != _literal_regexs.end(); ++it_literal_regex )
    {
      const std::regex regex_literal = std::regex( it_literal_regex->first );
      if ( std::regex_match( token.value, regex_literal ) )
      {
        // symbol nodes need special handling as the are used to
        // provide input data
        if ( _is_symbol( token ) )
        {
          return _get_input_node( token.value );
        }
        else
        {
          const std::shared_ptr< Node > node = it_literal_regex->second( token.value );
          return node;
        }

      }
    }

    // reaching this means literal could not be parsed
    const std::string msg = "can not handle literal '" + token.value + "'";
    throw std::runtime_error( msg );
  }
}

/*
 * Parses a sympy srepr expression and builds an evaluation tree
 *
 */
void Parser::parse( const std::string expr, const size_t n_inputs )
{
    if ( expr == "" )
    {
      throw std::runtime_error( "empty expression" );
    }

    for ( size_t i = 0; i < n_inputs; ++i )
    {
      _input_nodes.insert( std::make_pair( "x_" + std::to_string( i ), create_literal_node_symbol( "" ) ) );
    }

    Lexer lexer;

    for ( auto it_operator_name = _operator_names.begin(); it_operator_name != _operator_names.end(); ++it_operator_name )
    {
      lexer.register_operator( it_operator_name->first );
    }

    std::vector< Token > tokens;
    lexer.tokenize_expression( expr, tokens );

    root = _parse_recursively( tokens );
}


double Parser::eval( const std::vector< double >& inputs )
{
  if ( inputs.size() != _input_nodes.size() )
  {
    throw std::runtime_error( "input size mismatch" );
  }

  size_t i = 0;
  for ( auto it_input_nodes = _input_nodes.begin(); it_input_nodes != _input_nodes.end(); ++it_input_nodes )
  {
    std::static_pointer_cast< ValueNode >( it_input_nodes->second )->value = inputs[ i ];
    ++i;
  }

  return root->eval();
}

}
